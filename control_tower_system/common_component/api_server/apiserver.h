#ifndef APISERVER_H
#define APISERVER_H

#include <QObject>
#include <QMetaType>
#include <QDateTime>
#include "../comcpp_server/comserver.h"
#include "../ds_def/tcs_ds_def.h"


/// 设计原则：尽量不考虑上层使用；对于自身来说充分表达自身的状态而不考虑外界的具体使用

/// 封装和解析具体的请求
/// 或者设计成针对某几个函数的解释或者单纯的字符串操作


/// Sat Nov 21 11:09:00 CST 2020
/// 优化ws发送的log显示
/// 1. 格式 时间+信息

class APIServer: public QObject
{
  Q_OBJECT
public:
  APIServer(QObject *parent = nullptr);
  ~APIServer(){
    delete _com;
  }

  ///
  /// \brief SetAllUserMap 建立可连接用户名列表；用户变更后需要调用用该函数进行更新
  /// \param v_users
  /// \return
  ///
  int setAllUserMap(const vector<UserItem>& v_users){return _com->setAllUserMap(v_users); }


  ///================================HTTP===============================///

  int setupListenHttp(){return _com->setupListenHttp(); }

  int sendHttpResponse(long long int fd, char *data_type, unsigned char *data,
                       int data_length){
//    cout<<"send response:"<<data<<endl;
    return _com->sendHttpResponse(fd,data_type,data,data_length); }

  int closeHttpConnection(long long int fd){return _com->closeHttpConnection(fd); }

  int stopListenHttp(){return _com->stopListenHttp(); }

  ///================================WS===============================///
  int setupListenWebsocket(){return _com->setupListenWebsocket(); }

  ///
  /// \brief SendWebsocketMessage 向ws通道中发送消息；用户在_users_login且标识为true
  /// \param user
  /// \param data
  /// \return
  ///

  int sendWebsocketMessage(const string& user, const string &data)
  {
//    cout<<endl<<"****************************************"<<endl;
//    cout<<"sendWebsocketMessage:"<<data<<endl;
//    cout<<endl<<"****************************************"<<endl;

     int flag=_com->sendWebsocketMessage(const_cast<char*>(user.c_str()),const_cast<char*>(data.c_str()));
//     cout<<__func__<<"end !"<<endl;
     return flag;
  }

  int sendWebsocketHeartBeat(){return _com->sendWebsocketHeartBeat(); }
  int stopListenWebsocket(){return _com->stopListenWebsocket(); }
  int closeWebsocketConnection(char *user){return _com->closeWebsocketConnection(user); }


  bool isListenHTTP() const{return _com->isListenHTTP(); }
  bool isListenWS() const{return _com->isListenWS(); }
  map<UserItem,bool> getUserStatus()const{return _com->getUserStatus(); }
  vector<pair<long long int,unsigned>> getFds()const{return _com->getFds(); }


  int sendWSMsg_RESTART(const string& v_user);
  int sendWSMsg_PUSHLOG(const string& v_user,const string& v_log);
  int sendWSMsg_USERSTAUS(const string& v_user,const vector<ns_tcs_ds_def::UserStatus>& v_list);
  int sendWSMsg_DEVSTATUS(const string& v_user,const vector<ns_tcs_ds_def::CamStatus>& v_list);


  int sendHTTPMsg_REQ_SINGLEUSER(long long int v_fd,const ns_tcs_ds_def::User& v_user );
  int sendHTTPMsg_REQ_ALLUSER(long long int v_fd,const vector<ns_tcs_ds_def::User>& v_users);
  int sendHTTPMsg_REQ_RUNTME(long long int v_fd,const ns_tcs_ds_def::TimeRecord& v_info);
  int sendHTTPMsg_REQ_USERSTAUS(long long int v_fd,const vector<ns_tcs_ds_def::UserStatus>& v_list);
  int sendHTTPMsg_REQ_DEVARRAY(long long int v_fd,const ns_tcs_ds_def::CameraArray& v_array);
  int sendHTTPMsg_REQ_DEVSTATUS(long long int v_fd,const vector<ns_tcs_ds_def::CamStatus>& v_list);
//  int sendHTTPMsg_REQ_ADDDEV();
//  int sendHTTPMsg_REQ_ADDUSER();
//  int sendHTTPMsg_REQ_DELDEV();
//  int sendHTTPMsg_REQ_DELUSER();
//  int sendHTTPMsg_REQ_SETDEV();
//  int sendHTTPMsg_REQ_SETUSER();
//  int sendHTTPMsg_REQ_STOPSERVICE();
//  int sendHTTPMsg_REQ_DIRECTCAM_OP();
  int sendHTTPMsg_REQ_QRYNVR(long long int v_fd,const ns_tcs_ds_def::NvrDev &v_nvr);
  int sendHTTPMsg_REQ_QRYSERIALPORT(long long int v_fd,const vector<string> &l_port);

  //  int sendHTTPMsg_REQ_EMPTYNVR();
//  int sendHTTPMsg_REQ_SETNVR();
  int sendHTTPMsg_RETURNCODE(long long int v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE v_code);




private:
  ComServer *_com;

  /// 具体的接口
signals:
  void sgl_qrySingleUser( long long v_fd,string v_usr,string v_data);
  void sgl_qryAllUser( long long v_fd,string v_usr);
  void sgl_qryRunTime( long long v_fd,string v_usr);
  ///权限下的所有用户的状态
  void sgl_qryUserStatus( long long v_fd,string v_usr);
  void sgl_qryDevArray( long long v_fd,string v_usr);
  ///权限下的所有设备的状态
  void sgl_qryDevStatus( long long v_fd,string v_usr);
  void sgl_qryNVR( long long v_fd,string v_usr);

  void sgl_qrySerialPort(long long v_fd,string v_usr);



  void sgl_addDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
  void sgl_addUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);

  void sgl_delDev( long long v_fd,string v_usr,string v_cmaname);
  void sgl_delUser( long long v_fd,string v_usr,string v_username);
  void sgl_emptyNVR( long long v_fd,string v_usr);

  void sgl_setDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
  void sgl_setUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);
  void sgl_setNVR( long long v_fd,string v_usr,ns_tcs_ds_def::NvrDev v_data);

  void sgl_stopService( long long v_fd,string v_usr);
  void sgl_directCamOp( long long v_fd,string v_usr,ns_tcs_ds_def::DirectCamOp v_data);

  void sgl_OnHttpError(int error_code);
  void sgl_OnUserLogin(string username);
  void sgl_OnUserLogout(string username);
  void sgl_OnWebsocketError(int error_code);

public slots:
  void slot_OnNewHttpRequest(long long int fd, char *username, char *req_data,void *ptr);
  void slot_OnHttpError(int error_code, void*ptr);
  void slot_OnUserLogin(char *username, void *ptr);
  void slot_OnUserLogout(char *username, void *ptr);
  void slot_OnWebsocketError(int error_code, void *ptr);


};

#endif // APISERVER_H
