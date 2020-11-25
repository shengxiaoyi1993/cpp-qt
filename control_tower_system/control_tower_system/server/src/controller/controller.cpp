#include "controller.h"
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>

controller::controller(QObject *parent) : QObject(parent)
{
  _mainwindow=new MainWindow();
  _userconfig=new UserConfig();
  _diag_login=new LoginServer();
  _devconfig=new DevConfig();
  _nvrconfig=new NvrConfig();
  _apiclient=new APIClient();
  _timer_1s=new QTimer;
  initialize();

  _diag_login->show();


}




void controller::initialize(){
  setGUIInteract();
  connectWithAPIClient();
  connectWithLoginServer();
  connectWithMainWindow();
  connectWithUserConfig();
  connectWithDevConfig();
  connectWithNvrConfig();

  _diag_login->show();

}
//  connect(_diag_login,SIGNAL(),this,SLOT());

void controller::setGUIInteract(){
  connect(_mainwindow->action_devconfig(),SIGNAL(triggered()),this,SLOT(on_action_devconfig_triggered()));
  connect(_mainwindow->action_nvrconfig(),SIGNAL(triggered()),this,SLOT(on_action_nvrconfig_triggered()));
  connect(_mainwindow->action_userconfig(),SIGNAL(triggered()),this,SLOT(on_action_userconfig_triggered()));

}

void controller::on_action_devconfig_triggered(){
  updataDevConfig();
  _devconfig->show();
}

void controller::on_action_userconfig_triggered(){
  _userconfig->show();
}

void controller::on_action_nvrconfig_triggered(){
  updateNVRConfig();
  _nvrconfig->show();
}


void controller::synAfterConnect(){
  updateMainwindow();
  updataDevConfig();
  updateNVRConfig();
  connect(_timer_1s,SIGNAL(timeout()),
          this,SLOT(slot_on_timer_1s()));
  _timer_1s->start(1000);
}

void controller::updateMainwindow(){
  ///req_queryRUNTME
  ns_tcs_ds_def::TimeRecord  v_info;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_runtime=_apiclient->req_queryRUNTME(v_info);
  cout<<"req_queryRUNTME:"<<code_runtime<<endl;
  if(code_runtime != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryRUNTME Error!");
  }


  ///req_queryUSERSTAUS
  ns_tcs_ds_def::CameraArray v_array;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_devarray=_apiclient->req_queryDEVARRAY(v_array);
  cout<<"req_queryUSERSTAUS:"<<code_devarray<<endl;
  if(code_devarray != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryDEVARRAY Error!");

  }

  vector<ns_tcs_ds_def::User> v_users;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_alluser=_apiclient->req_queryALLUSER(v_users);
  cout<<"req_queryALLUSER:"<<code_alluser<<endl;
  if(code_devarray != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryALLUSER Error!");
  }


  ///req_queryUSERSTAUS
  vector<ns_tcs_ds_def::UserStatus> v_list_userstatus;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_userstatus=_apiclient->req_queryUSERSTAUS(v_list_userstatus);
  cout<<"req_queryUSERSTAUS:"<<code_userstatus<<endl;
  if(code_userstatus != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryUSERSTAUS Error!");
  }

  vector<ns_tcs_ds_def::CamStatus> v_list_camstatus;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_camstatus=_apiclient->req_queryDEVSTATUS(v_list_camstatus);
  if(code_camstatus != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryDEVSTATUS Error!");
  }

  /// 更新服务器状态列表
  int count_user_login=0;
  for (auto it=v_list_userstatus.begin();it!=v_list_userstatus.end();it++) {
    if (it->_status == true) {
      count_user_login++;
    }
  }
  _mainwindow->uopdateServerStaus(_serverip,1,v_info._time_interval,count_user_login);


  /// 更新主界面上的用户状态列表，需要使用前面请求的数据
  {
    vector<string> l_user;
    vector<string>l_type;
    vector<bool>l_status;
    vector<int>l_aircam;
    vector<int>l_groundcam;
    vector<int>l_directcam;
    for(auto it=v_users.begin();it!=v_users.end();it++){
      l_user.push_back(it->_username);
      string type_tmp;
      switch (it->_type) {
        case  ns_tcs_ds_def::UserType_admin:{
          type_tmp="管理员";
          break;
        }
        case  ns_tcs_ds_def::UserType_assit:{
          type_tmp="管理员";
          break;
        }
        case  ns_tcs_ds_def::UserType_ordinary:{
          type_tmp="普通用户";
          break;
        }
      }
      l_type.push_back(type_tmp);

      ///  l_status
      auto pos_finduserstatus=find_if(v_list_userstatus.begin(),v_list_userstatus.end(),
                                      [=](const ns_tcs_ds_def::UserStatus &v_userstatus){
        return (v_userstatus._username==it->_username);
      });
      if (pos_finduserstatus != v_list_userstatus.end()) {
        l_status.push_back(pos_finduserstatus->_status);
      }
      else{
        l_status.push_back(-1);
      }

      /// l_aircam
      /// 寻找权限内的设备，验证正常的数量
      int count_aircam_connect=0;
      int count_groundcam_connect=0;
      int count_directcam_connect=0;

      /// 对权限内的设备逐台进行遍历
      for(auto it_dev=it->_cams.begin();it_dev!=it->_cams.end();it_dev++){
          if (it_dev->_flag==ns_tcs_ds_def::CamCtlFlag_noright) {
              continue;
          }

        /// 在设备状态表中找出连接正常的
        auto pos_finddev=find_if(v_list_camstatus.begin(),v_list_camstatus.end(),
                                 [=](ns_tcs_ds_def::CamStatus v_cam){
          return (v_cam._camname==it_dev->_camname);
        });
        if(pos_finddev != v_list_camstatus.end()){
          if(pos_finddev->_status){
            /// judge dev type
            ns_tcs_ds_def::CamDev  v_cam_tmp;
            if(v_array.findCam(it_dev->_camname, v_cam_tmp)==0){
              if(v_cam_tmp._type==ns_tcs_ds_def::CameraType_to_air){
                count_aircam_connect++;
              }
              else if(v_cam_tmp._type==ns_tcs_ds_def::CameraType_to_ground){
                count_groundcam_connect++;
              }
              else if(v_cam_tmp._type==ns_tcs_ds_def::CameraType_direct){
                count_directcam_connect++;
              }
              else{
                throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                                      +" :Error(wrong selection !)");
              }
            }
            else{
              throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                                    +" :Error(camera has no complete info !)");
            }
          }
        }
        else {
          throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                                +" :Error(camera has no status info !)");
        }
      }
      l_aircam.push_back(count_aircam_connect);
      l_groundcam.push_back(count_groundcam_connect);
      l_directcam.push_back(count_directcam_connect);
    }
    _mainwindow->updateUserTatusTable(l_user,l_type,l_status,l_aircam,l_groundcam,l_directcam);
  }

  _userconfig->updateUserList(v_users);

}




void controller::connectWithMainWindow(){
  connect(_mainwindow,SIGNAL(sgl_exitGUI()),
          this,SLOT(slot2MainWindow_exitGUI()));
  connect(_mainwindow,SIGNAL(sgl_stopService()),
          this,SLOT(slot2MainWindow_stopService()));
  connect(_mainwindow,SIGNAL(sgl_setAutoLauch(bool)),
          this,SLOT(slot2MainWindow_setAutoLauch(bool)));
}

void controller::connectWithUserConfig(){
  connect(_userconfig,SIGNAL(sgl_rmOneUser( ns_tcs_ds_def::User )),
          this,SLOT(slot2UserConfig_rmOneUser(ns_tcs_ds_def::User)));
  connect(_userconfig,SIGNAL(sgl_addOneUser( ns_tcs_ds_def::User )),
          this,SLOT(slot2UserConfig_addOneUser(ns_tcs_ds_def::User)));
  connect(_userconfig,SIGNAL(sgl_modifyOneUser( ns_tcs_ds_def::User )),
          this,SLOT(slot2UserConfig_modifyOneUser(ns_tcs_ds_def::User)));

}

void controller::connectWithLoginServer(){

  connect(_diag_login,SIGNAL(sgl_reqToLogin(string ,string ,string )),
          this,SLOT(slot2LoginServer_reqToLogin(string ,string ,string )));
  connect(_diag_login,SIGNAL(sgl_reqToQuit()),
          this,SLOT(slot2LoginServer_reqToQuit()));

}

void controller::connectWithDevConfig(){
  connect(_devconfig,SIGNAL(sgl_addCam(ns_tcs_ds_def::CamDev)),
          this,SLOT(slot2DevConfig_addCam(ns_tcs_ds_def::CamDev)));
  connect(_devconfig,SIGNAL(sgl_rmCam(ns_tcs_ds_def::CamDev)),
          this,SLOT(slot2DevConfig_rmCam(ns_tcs_ds_def::CamDev)));
  connect(_devconfig,SIGNAL(sgl_modifyCam(ns_tcs_ds_def::CamDev)),
          this,SLOT(slot2DevConfig_modifyCam(ns_tcs_ds_def::CamDev)));
  connect(_devconfig,SIGNAL(sgl_testCamConnect(ns_tcs_ds_def::CamDev)),
          this,SLOT(slot2DevConfig_testCamConnect(ns_tcs_ds_def::CamDev)));
}


void controller::connectWithNvrConfig(){
  connect(_nvrconfig,SIGNAL(sgl_emptynvr()),
          this,SLOT(slot2NvrDev_emptynvr()));

  connect(_nvrconfig,SIGNAL(sgl_setnvr(ns_tcs_ds_def::NvrDev)),
          this,SLOT(slot2NvrDev_setnvr(ns_tcs_ds_def::NvrDev)));
  connect(_nvrconfig,SIGNAL(sgl_testconnetct_nvr(ns_tcs_ds_def::NvrDev)),
          this,SLOT(slot2NvrDev_testconnetct_nvr(ns_tcs_ds_def::NvrDev)));
  connect(_nvrconfig,SIGNAL(sgl_startnvrApp(ns_tcs_ds_def::NvrDev)),
          this,SLOT(slot2NvrDev_startnvrApp(ns_tcs_ds_def::NvrDev)));


}

void controller::connectWithAPIClient(){
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


void controller::slot2MainWindow_exitGUI(){
  exit(0);
}

void controller::slot2MainWindow_setAutoLauch(bool v_flag){
  throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                        +": Error(unacomplish)!");

}

void controller::slot2MainWindow_stopService(){
  throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                        +": Error(unacomplish)!");
}

void controller::slot2LoginServer_reqToLogin(string v_serverip,
                                             string v_username,
                                             string v_password){
  cout<<__func__
     <<" v_serverip: "<<v_serverip
    <<" v_username: "<<v_username
   <<" v_password: "<<v_password
  <<endl;
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

}


void controller::slot2LoginServer_reqToQuit(){
  cout<<__func__<<endl;

  exit(0);
}

void controller::slot2APIClient_WS_OnLogin(){
  cout<<__func__<<endl;
  _diag_login->hide();
  synAfterConnect();
  _mainwindow->show();
}

void controller::slot2APIClient_WS_OnClosed(){
  cout<<__func__<<endl;
  QMessageBox:: StandardButton result=  QMessageBox::information(NULL, "Warning", "Logout ! WebSocket is close.",
                                                                 QMessageBox::Ok , QMessageBox::Ok);
  exit(REBOOT_CODE);
}

void controller::slot2APIClient_WS_RESTART(){
  qApp->exit(REBOOT_CODE);
}


void controller::slot2APIClient_WS_PUSHLOG(string log){
  cout<<__func__<<": log:"<<log<<endl;
  _mainwindow->pushbackLog(log);
  synAfterConnect();

}

void controller::slot2APIClient_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> v_list ){
  cout<<__func__<<endl;

  updateMainwindow();
}

void controller::slot2APIClient_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus> v_list){
  cout<<__func__<<endl;
  updateMainwindow();
}


void controller::slot_on_timer_1s(){
  ///req_queryRUNTME
  ns_tcs_ds_def::TimeRecord  v_info;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_runtime=_apiclient->req_queryRUNTME(v_info);
  cout<<"req_queryRUNTME:"<<code_runtime<<endl;
  if(code_runtime != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryRUNTME Error!");
  }

  ///req_queryUSERSTAUS
  vector<ns_tcs_ds_def::UserStatus> v_list_userstatus;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_userstatus=_apiclient->req_queryUSERSTAUS(v_list_userstatus);
  cout<<"req_queryUSERSTAUS:"<<code_userstatus<<endl;
  if(code_userstatus != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryUSERSTAUS Error!");
  }

  /// 更新服务器状态列表
  int count_user_login=0;
  for (auto it=v_list_userstatus.begin();it!=v_list_userstatus.end();it++) {
    if (it->_status == true) {
      count_user_login++;
    }
  }
  _mainwindow->uopdateServerStaus(_serverip,1,v_info._time_interval,count_user_login);

}

void controller::updateUserConfig(){
  vector<ns_tcs_ds_def::User> v_users;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_alluser=_apiclient->req_queryALLUSER(v_users);
  cout<<"req_queryALLUSER:"<<code_alluser<<endl;
  if(code_alluser != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryALLUSER Error!");
  }
  _userconfig->updateUserList(v_users);
}


void controller::slot2UserConfig_rmOneUser(ns_tcs_ds_def::User v_user){
  cout<<__func__<<"v_user._username:"<<v_user._username<<endl;

  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_DELUSER(v_user._username);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to DEL USER !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to DEL USER !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }

  updateUserConfig();

}

void controller::slot2UserConfig_addOneUser(ns_tcs_ds_def::User v_user){

  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_queryADDUSER(v_user);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to ADD USER !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to ADD USER  !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }

  updateUserConfig();

}

void controller::slot2UserConfig_modifyOneUser(ns_tcs_ds_def::User v_user){
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_SETUSER(v_user);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to MODIFY USER !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to MODIFY USER  !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }

  updateUserConfig();

}

/// 添加设备的请求 发送请求/给出提示，说明请求是否成功；更新devconfig和mainwindow中的数据
void controller::slot2DevConfig_addCam(ns_tcs_ds_def::CamDev v_cam){
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_queryADDDEV(v_cam);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to add Camera !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to add Camera !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  updataDevConfig();

}



void controller::updataDevConfig(){
  ns_tcs_ds_def::CameraArray v_camarray;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_camarray=_apiclient->req_queryDEVARRAY(v_camarray);
  cout<<"req_queryALLUSER:"<<code_camarray<<endl;
  if(code_camarray != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryALLUSER Error!");
  }
  _devconfig->updateCamArray(v_camarray);

  vector<string> list_port;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE code_port=_apiclient->req_querySerialPort(list_port);
  cout<<"req_querySerialPort:"<<code_port<<endl;
  if(code_port != ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":req_queryALLUSER Error!");
  }
  _devconfig->updateSerialPort(list_port);
}


void controller::slot2DevConfig_rmCam(ns_tcs_ds_def::CamDev v_cam){
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_DELDEV(v_cam._camname);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to del Camera !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to del Camera !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  updataDevConfig();

}

void controller::slot2DevConfig_modifyCam(ns_tcs_ds_def::CamDev v_cam){
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_SETDEV(v_cam);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to modify Camera !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to modify Camera !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
  updataDevConfig();

}

void controller::slot2DevConfig_testCamConnect(ns_tcs_ds_def::CamDev v_cam){
  throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                        +": Error(test dev connection unacomplish)!");
}



///NvrDev
/// server-service 发出请求
/// 弹窗显示执行结果
/// 若成功则清空_nvrconfig的数据
void controller::slot2NvrDev_emptynvr(){
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_EMPTYNVR();
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to empty NVR setting !",
                             QMessageBox::Ok, QMessageBox::Ok);
    _nvrconfig->empty();
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to empty NVR setting !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
}

void controller::slot2NvrDev_setnvr(ns_tcs_ds_def::NvrDev v_dev){
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_SETNVR(v_dev);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    QMessageBox::information(NULL, "Warning", "Succeed to Modify NVR setting !",
                             QMessageBox::Ok, QMessageBox::Ok);
    updateNVRConfig();
  }
  else{
    QMessageBox::information(NULL, "Warning", "Fail to Modify NVR setting !",
                             QMessageBox::Ok, QMessageBox::Ok);
  }
}

void controller::updateNVRConfig(){
  ns_tcs_ds_def::NvrDev v_dev;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag= _apiclient->req_queryQRYNVR(v_dev);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    //    QMessageBox::information(NULL, "Warning", "Succeed to Modify NVR setting !",
    //                             QMessageBox::Ok, QMessageBox::Ok);
    _nvrconfig->setNvrDev(v_dev);
  }
  else{
    //    QMessageBox::information(NULL, "Warning", "Fail to Modify NVR setting !",
    //                             QMessageBox::Ok, QMessageBox::Ok);
  }
}




void controller::slot2NvrDev_testconnetct_nvr(ns_tcs_ds_def::NvrDev v_dev){
  throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                        +": Error(test NVR connection unacomplish)!");
}

void controller::slot2NvrDev_startnvrApp(ns_tcs_ds_def::NvrDev){
    QString program = "C:\\Program Files\\Smart Professional Surveillance System\\SmartPSS\\SmartPSS.exe";//指定要调用的可执行程序的路径
    QProcess *myProcess = new QProcess(this);
    //this表示myprocess的父窗口是本程序中的主窗口，当主窗口关闭时，进程也会终止，
    //如果不填加this,myprocess是独立的进程，主窗口关闭，进程依然运行
    myProcess->start(program,QStringList());
    myProcess->pid();

//  throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
//                        +": Error(test NVR connection unacomplish)!");
}
