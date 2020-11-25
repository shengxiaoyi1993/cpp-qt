#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "../view/loginclient.h"
#include "../main/mainwindow.h"
#include "api_client/apiclient.h"

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject *parent = nullptr);
  ~Controller();
  static int const REBOOT_CODE=32554;

signals:

public slots:
  void slot2LoginServer_reqToLogin(string v_serverip,string v_username,string v_password);
  void slot2LoginServer_reqToQuit();

  void slot2APIClient_WS_OnLogin();
  void slot2APIClient_WS_OnClosed();
  void slot2APIClient_WS_RESTART();
  void slot2APIClient_WS_PUSHLOG(string log);
  void slot2APIClient_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> v_list );
  void slot2APIClient_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus> v_list);

  void slot2DirectCamView_directcamera_op( ns_tcs_ds_def::DirectCamOp v_data);


private:
  LoginClient * _loginclient;
  MainWindow *_mainwindow;
  APIClient *_apiclient;

  string _serverip;
  string _username;
  string _password;

  void connectWithAPIClient();

  /// 客户端登陆成功后需要刷新的界面 暂时只考虑直显相机界面
  void synAfterConnect();

};


inline AuthParam AuthParamFromString(string v_serverip,string v_username,string v_password){
  AuthParam ret;
  memset(ret.server_ip,0,sizeof (ret.server_ip));
  memset(ret.password,0,sizeof (ret.password));
  memset(ret.username,0,sizeof (ret.username));

  memcpy(ret.server_ip, v_serverip.c_str(),v_serverip.size());
  memcpy(ret.username, v_username.c_str(),v_username.size());
  memcpy(ret.password, v_password.c_str(),v_password.size());

  return ret;
}



#endif // CONTROLLER_H
