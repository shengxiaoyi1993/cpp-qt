#ifndef CONTROLLER_H
#define CONTROLLER_H

/// 转接api与gui
/// 完全不存储数据，只做更新

#include <QObject>
#include <QTimer>
#include "../main/mainwindow.h"
#include "api_client/apiclient.h"

/// 对于所有信息都需要同步的情况
///   1.增删设备
///   2.增删用户

class controller : public QObject
{
  Q_OBJECT
public:
  explicit controller(QObject *parent = nullptr);
  void initialize();
  static int const REBOOT_CODE=32554;


signals:

public slots:
  void on_action_nvrconfig_triggered();
  void on_action_devconfig_triggered();
  void on_action_userconfig_triggered();


  // slot2
  void slot2LoginServer_reqToLogin(string v_serverip,string v_username,string v_password);
  void slot2LoginServer_reqToQuit();

  void slot2APIClient_WS_OnLogin();
  void slot2APIClient_WS_OnClosed();
  void slot2APIClient_WS_RESTART();
  void slot2APIClient_WS_PUSHLOG(string log);
  void slot2APIClient_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> v_list );
  void slot2APIClient_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus> v_list);

  void slot_on_timer_1s();

  void slot2MainWindow_exitGUI();
  void slot2MainWindow_setAutoLauch(bool v_flag);
  void slot2MainWindow_stopService();

  void slot2UserConfig_rmOneUser(ns_tcs_ds_def::User v_user);
  void slot2UserConfig_addOneUser(ns_tcs_ds_def::User v_user);
  void slot2UserConfig_modifyOneUser(ns_tcs_ds_def::User v_user);

  /// 添加设备的请求 发送请求/给出提示，说明请求是否成功；更新devconfig和mainwindow中的数据
  void slot2DevConfig_addCam(ns_tcs_ds_def::CamDev v_cam);
  void slot2DevConfig_rmCam(ns_tcs_ds_def::CamDev v_cam);
  void slot2DevConfig_modifyCam(ns_tcs_ds_def::CamDev v_cam);
  void slot2DevConfig_testCamConnect(ns_tcs_ds_def::CamDev v_cam);

  /// 与nvrconfig的交互
  void slot2NvrDev_emptynvr();
  void slot2NvrDev_setnvr(ns_tcs_ds_def::NvrDev);
  void slot2NvrDev_testconnetct_nvr(ns_tcs_ds_def::NvrDev);
  void slot2NvrDev_startnvrApp(ns_tcs_ds_def::NvrDev);




private:
  MainWindow *_mainwindow;
  UserConfig *_userconfig;
  LoginServer*_diag_login;
  DevConfig *_devconfig;
  NvrConfig *_nvrconfig;
  APIClient *_apiclient;
  string _serverip;
  string _username;
  string _password;
  QTimer *_timer_1s;

  void setGUIInteract();
  void synAfterConnect();
  void connectWithMainWindow();
  void connectWithUserConfig();
  void connectWithLoginServer();
  void connectWithDevConfig();
  void connectWithNvrConfig();
  void connectWithAPIClient();
  void updateMainwindow();
  void updateUserConfig();
  void updataDevConfig();
  void updateNVRConfig();

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
