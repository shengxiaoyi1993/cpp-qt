#include "controller.h"
#include <QMessageBox>


Controller::Controller(QObject *parent) : QObject(parent),
  _loginclient(new LoginClient()),_mainwindow(new MainWindow()),_apiclient(new APIClient())

{

  connect(_loginclient,SIGNAL(sgl_reqToLogin(string ,string ,string )),
          this,SLOT(slot2LoginServer_reqToLogin(string ,string ,string )));
  connect(_loginclient,SIGNAL(sgl_reqToQuit()),
          this,SLOT(slot2LoginServer_reqToQuit()));

  connect(_mainwindow->view_direct(),SIGNAL(sgl_directcamera_op(ns_tcs_ds_def::DirectCamOp)),
          this,SLOT(slot2DirectCamView_directcamera_op(ns_tcs_ds_def::DirectCamOp)));

  _loginclient->show();

  connectWithAPIClient();


}



Controller::~Controller(){
  delete _loginclient;
  delete _mainwindow;
}

void Controller::slot2DirectCamView_directcamera_op( ns_tcs_ds_def::DirectCamOp v_data){
//  ns_tcs_ds_def::CameraArray v_camarray;
  cout<<__func__<<": op "<<v_data.jsonobj().ToString()<<endl;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_camarray=_apiclient->req_DIRECTCAM_OP(v_data);
  if(code_camarray != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_DIRECTCAM_OP Error!");
  }

}



void Controller::slot2LoginServer_reqToLogin(string v_serverip,string v_username,string v_password){
  _serverip=v_serverip;
  _username=v_username;
  _password=v_password;
  _apiclient->setAuthPara(AuthParamFromString(v_serverip,
                                              v_username,
                                              v_password
                                              ));

  int flag_login=_apiclient->login();
  if (flag_login != 0) {
    QMessageBox::information(NULL, "Warning", "Login Error !",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
  }
  synAfterConnect();

}

void Controller::slot2LoginServer_reqToQuit(){
  exit(0);
}


void Controller::connectWithAPIClient(){
  connect(_apiclient,SIGNAL(sgl_WS_OnLogin()),
          this,SLOT(slot2APIClient_WS_OnLogin()));
  connect(_apiclient,SIGNAL(sgl_WS_OnClosed()),
          this,SLOT(slot2APIClient_WS_OnClosed()));
  connect(_apiclient,SIGNAL(sgl_WS_RESTART()),
          this,SLOT(slot2APIClient_WS_RESTART()));
  connect(_apiclient,SIGNAL(sgl_WS_PUSHLOG(string)),
          this,SLOT(slot2APIClient_WS_PUSHLOG(string)));
  connect(_apiclient,SIGNAL(sgl_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> )),
          this,SLOT(slot2APIClient_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> )));
  connect(_apiclient,SIGNAL(sgl_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus>)),
          this,SLOT(slot2APIClient_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus>)));

}

void Controller::synAfterConnect(){

  ns_tcs_ds_def::CameraArray v_camarray;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_camarray=_apiclient->req_queryDEVARRAY(v_camarray);
  if(code_camarray != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryALLUSER Error!");
  }

  ns_tcs_ds_def::User v_user;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_queryusr=_apiclient->req_querySINGLEUSER(_username,v_user);
  if(code_queryusr != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_querySINGLEUSER Error!");
  }

  cout<<v_camarray.jsonobj().ToFormattedString()<<endl;
  cout<<v_user.jsonobj().ToFormattedString()<<endl;

  /// 然后选出有查看权限的直显相机
  vector<ns_tcs_ds_def::CamDev> l_direct;
  for (auto it=v_user._cams.begin();it!=v_user._cams.end();it++) {
    if(it->_flag!=ns_tcs_ds_def::CamCtlFlag_noright
       &&it->_type==ns_tcs_ds_def::CameraType_direct){
      ns_tcs_ds_def::CamDev cam_tmp;
      int flag_find=v_camarray.findCam(it->_camname,cam_tmp);
      if(flag_find == 0){
        l_direct.push_back(cam_tmp);
      }
    }
  }

  _mainwindow->updateDirctCamList(l_direct);
}


void Controller::slot2APIClient_WS_OnLogin(){
  _loginclient->hide();
  _mainwindow->showFullScreen();
}


void Controller::slot2APIClient_WS_OnClosed(){
  QMessageBox::information(NULL, "Warning", "WS Disconnect ! Please Login again",
                           QMessageBox::Ok, QMessageBox::Ok);
  exit(REBOOT_CODE);
}

void Controller::slot2APIClient_WS_RESTART(){

}

void Controller::slot2APIClient_WS_PUSHLOG(string log){

}

void Controller::slot2APIClient_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> v_list ){

}

void Controller::slot2APIClient_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus> v_list){

}
