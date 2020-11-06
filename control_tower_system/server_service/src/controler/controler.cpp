#include "controler.h"
#include <iostream>
#include <string>


using namespace std;

Controler::Controler(string v_config, QObject *parent) : QObject(parent),
  _apiserver(new APIServer),
  _devproxy(new DevProxy()),
  _datacoll(new DataManage::DataColl(v_config))
{

  ///_apiserver初始化
  vector<UserItem> list_useritem;
  auto list_user=_datacoll->Users();
  for(auto it=list_user.begin();it<list_user.end();it++){
    list_useritem.insert(list_useritem.begin(),UserItem(it->_username,""));
  }
  _apiserver->setAllUserMap(list_useritem);
  connect(_apiserver,SIGNAL(sgl_OnHttpError(int)),
          this,SLOT(slot_OnHttpError(int)));
  connect(_apiserver,SIGNAL(sgl_OnUserLogin(string)),
          this,SLOT(slot_OnUserLogin(string)));
  connect(_apiserver,SIGNAL(sgl_OnUserLogout(string)),
          this,SLOT(slot_OnUserLogout(string)));
  connect(_apiserver,SIGNAL(sgl_OnWebsocketError(int)),
          this,SLOT(slot_OnWebsocketError(int)));

  connect(_apiserver,SIGNAL(sgl_qrySingleUser( long long ,string ,string )),
          this,SLOT(slot_qrySingleUser( long long ,string ,string )));
  connect(_apiserver,SIGNAL(sgl_qryAllUser( long long ,string )),
          this,SLOT(slot_qryAllUser( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_qryRunTime( long long ,string )),
          this,SLOT(slot_qryRunTime( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_qryUserStatus( long long ,string )),
          this,SLOT(slot_qryUserStatus( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_qryDevArray( long long ,string )),
          this,SLOT(slot_qryDevArray( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_qryDevStatus( long long ,string )),
          this,SLOT(slot_qryDevStatus( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_qryNVR( long long ,string )),
          this,SLOT(slot_qryNVR( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_addDev( long long ,string ,ns_tcs_ds_def::CamDev )),
          this,SLOT(slot_addDev( long long ,string ,ns_tcs_ds_def::CamDev )));
  connect(_apiserver,SIGNAL(sgl_addUser( long long ,string ,ns_tcs_ds_def::User )),
          this,SLOT(slot_addUser( long long ,string ,ns_tcs_ds_def::User )));

  connect(_apiserver,SIGNAL(sgl_delDev( long long ,string ,string )),
          this,SLOT(slot_delDev( long long ,string ,string )));
  connect(_apiserver,SIGNAL(sgl_delUser( long long ,string ,string )),
          this,SLOT(slot_delUser( long long ,string ,string )));
  connect(_apiserver,SIGNAL(sgl_emptyNVR( long long ,string )),
          this,SLOT(slot_emptyNVR( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_setDev( long long ,string ,ns_tcs_ds_def::CamDev )),
          this,SLOT(slot_setDev( long long ,string ,ns_tcs_ds_def::CamDev )));
  connect(_apiserver,SIGNAL(sgl_setUser( long long ,string ,ns_tcs_ds_def::User )),
          this,SLOT(slot_setUser( long long ,string ,ns_tcs_ds_def::User )));
  connect(_apiserver,SIGNAL(sgl_setNVR( long long ,string ,ns_tcs_ds_def::NvrDev )),
          this,SLOT(slot_setNVR( long long ,string ,ns_tcs_ds_def::NvrDev )));
  connect(_apiserver,SIGNAL(sgl_stopService( long long ,string )),
          this,SLOT(slot_stopService( long long ,string )));
  connect(_apiserver,SIGNAL(sgl_directCamOp( long long ,string ,ns_tcs_ds_def::PointAndPad )),
          this,SLOT(slot_directCamOp( long long ,string ,ns_tcs_ds_def::PointAndPad )));


  _apiserver->setupListenHttp();
  _apiserver->setupListenWebsocket();

  ///_devproxy初始化
  connect(_devproxy,SIGNAL(sgl_camDisconnect(string)),
          this,SLOT(slot_OnDevDisconnect(string)));
  connect(_devproxy,SIGNAL(sgl_camReconnect(string)),
          this,SLOT(slot_OnDevReconnect(string)));
  _devproxy->setCameraArray(_datacoll->CamArary());

  cout<<"CamArary:"<<_datacoll->CamArary().jsonobj().ToFormattedString()<<endl;
  cout<<"isListenHTTP:"<<_apiserver->isListenHTTP()<<endl;
  auto list_camstatus=_devproxy->getCamStatusList();
//  for_each(list_camstatus.begin(),list_camstatus.begin(),[=](ns_tcs_ds_def::CamStatus v_cs){
//    cout<<""<<v_cs.jsonobj().ToString()<<endl;
//  });

  for(auto it=list_camstatus.begin();it<list_camstatus.end();it++){
    cout<<""<<it->jsonobj().ToString()<<endl;
  }
}



void Controler::slot_OnHttpError(int v_error_code){
  cout<<__func__<<" v_error_code:"<<v_error_code<<endl;
}

void Controler::slot_OnUserLogin(string v_username){
  cout<<__func__<<" v_username:"<<v_username<<endl;
}

void Controler::slot_OnUserLogout(string v_username){
  cout<<__func__<<" v_username:"<<v_username<<endl;

}
void Controler::slot_OnWebsocketError(int v_error_code){
  cout<<__func__<<" v_error_code:"<<v_error_code<<endl;
}


void Controler::slot_OnDevDisconnect(string v_camname){
  cout<<__func__<<" v_camname:"<<v_camname<<endl;
}

void Controler::slot_OnDevReconnect(string v_camname){
  cout<<__func__<<" v_camname:"<<v_camname<<endl;
}

/// 在datacoll中获取总数据
/// 判断是否在权限内
/// 获取该用户的信息,向句柄返回数据
void Controler::slot_qrySingleUser( long long v_fd,string v_usr,string v_data){

}

void Controler::slot_qryAllUser( long long v_fd,string v_usr){

}

void Controler::slot_qryRunTime( long long v_fd,string v_usr){

}

///权限下的所有用户的状态
void Controler::slot_qryUserStatus( long long v_fd,string v_usr){

}

void Controler::slot_qryDevArray( long long v_fd,string v_usr){

}

///权限下的所有设备的状态
void Controler::slot_qryDevStatus( long long v_fd,string v_usr){

}

void Controler::slot_qryNVR( long long v_fd,string v_usr){

}

void Controler::slot_addDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data){

}

void Controler::slot_addUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data){

}

void Controler::slot_delDev( long long v_fd,string v_usr,string v_cmaname){

}

void Controler::slot_delUser( long long v_fd,string v_usr,string v_username){

}

void Controler::slot_emptyNVR( long long v_fd,string v_usr){

}

void Controler::slot_setDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data){

}

void Controler::slot_setUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data){

}

void Controler::slot_setNVR( long long v_fd,string v_usr,ns_tcs_ds_def::NvrDev v_data){

}

void Controler::slot_stopService( long long v_fd,string v_usr){

}

void Controler::slot_directCamOp( long long v_fd,string v_usr,ns_tcs_ds_def::PointAndPad v_data){

}

