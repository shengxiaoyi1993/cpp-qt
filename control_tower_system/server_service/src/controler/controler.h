#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <iostream>
#include <string>
#include "../com/apiserver.h"
#include "../datamanage/datamanage.h"
#include "../devproxy/devproxy.h"
#include "../ds_def/tcs_ds_def.h"

/// 流程：实例化,初始化，开试运行

/// 初始化各个模块建立模块间的交互
/// 对传输的数据进行一定的处理
///
using namespace std;

class Controler : public QObject
{
  Q_OBJECT
public:
  explicit Controler(string v_config,QObject *parent = nullptr);

signals:

public slots:

  void slot_OnHttpError(int v_error_code);
  void slot_OnUserLogin(string v_username);
  void slot_OnUserLogout(string v_username);
  void slot_OnWebsocketError(int v_error_code);

  void slot_OnDevDisconnect(string v_username);
  void slot_OnDevReconnect(string v_username);

  void slot_qrySingleUser( long long v_fd,string v_usr,string v_data);
  void slot_qryAllUser( long long v_fd,string v_usr);
  void slot_qryRunTime( long long v_fd,string v_usr);
  ///权限下的所有用户的状态
  void slot_qryUserStatus( long long v_fd,string v_usr);
  void slot_qryDevArray( long long v_fd,string v_usr);
  ///权限下的所有设备的状态
  void slot_qryDevStatus( long long v_fd,string v_usr);
  void slot_qryNVR( long long v_fd,string v_usr);

  void slot_addDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
  void slot_addUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);

  void slot_delDev( long long v_fd,string v_usr,string v_cmaname);
  void slot_delUser( long long v_fd,string v_usr,string v_username);
  void slot_emptyNVR( long long v_fd,string v_usr);

  void slot_setDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
  void slot_setUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);
  void slot_setNVR( long long v_fd,string v_usr,ns_tcs_ds_def::NvrDev v_data);

  void slot_stopService( long long v_fd,string v_usr);
  void slot_directCamOp( long long v_fd,string v_usr,ns_tcs_ds_def::PointAndPad v_data);


private:
  APIServer* _apiserver;
  DevProxy* _devproxy;
  DataManage::DataColl *_datacoll;
};

#endif // CONTROLER_H
