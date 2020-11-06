#ifndef APISERVER_H
#define APISERVER_H

#include <QObject>
#include "comserver.h"
#include "../ds_def/tcs_ds_def.h"

/// 设计原则：尽量不考虑上层使用；对于自身来说充分表达自身的状态而不考虑外界的具体使用

/// 封装和解析具体的请求
/// 或者设计成针对某几个函数的解释或者单纯的字符串操作


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
                       int data_length){return _com->sendHttpResponse(fd,data_type,data,data_length); }

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

  int sendWebsocketMessage(char *user, char *data){return _com->sendWebsocketMessage(user,data); }
  int sendWebsocketHeartBeat(){return _com->sendWebsocketHeartBeat(); }
  int stopListenWebsocket(){return _com->stopListenWebsocket(); }
  int closeWebsocketConnection(char *user){return _com->closeWebsocketConnection(user); }


  bool isListenHTTP() const{return _com->isListenHTTP(); }
  bool isListenWS() const{return _com->isListenWS(); }
  map<UserItem,bool> getUserStatus()const{return _com->getUserStatus(); }
  vector<pair<long long int,unsigned>> getFds()const{return _com->getFds(); }




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

void sgl_addDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
void sgl_addUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);

void sgl_delDev( long long v_fd,string v_usr,string v_cmaname);
void sgl_delUser( long long v_fd,string v_usr,string v_username);
void sgl_emptyNVR( long long v_fd,string v_usr);

void sgl_setDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
void sgl_setUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);
void sgl_setNVR( long long v_fd,string v_usr,ns_tcs_ds_def::NvrDev v_data);

void sgl_stopService( long long v_fd,string v_usr);
void sgl_directCamOp( long long v_fd,string v_usr,ns_tcs_ds_def::PointAndPad v_data);

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
