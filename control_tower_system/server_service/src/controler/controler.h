#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <iostream>
#include <string>
#include <QTimer>
#include "../com/apiserver.h"
#include "../datamanage/datamanage.h"
#include "../devproxy/devproxy.h"
#include "../ds_def/tcs_ds_def.h"

/// 流程：实例化,初始化，开试运行

/// 初始化各个模块建立模块间的交互
/// 对传输的数据进行一定的处理

///  对于关键的操作都需要将log通知admin/assit类型的用户
///  未避免冲突与保持发送消息的一致性，发送log时都向一个列表中发送信息，然后一个定时器进行处理
using namespace std;

class Controler : public QObject
{
  Q_OBJECT
public:
  explicit Controler(string v_config,QObject *parent = nullptr);
  ~Controler();
  void on_UserStatusChange();
  void on_DevStatusChange(const string &v_dev);

  ///
  /// \brief updateUserMapInAPI 以配置文件的数据将API中用户初始化
  ///
  void updateUserMapInAPI();




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
  void slot_qrySerialPort( long long v_fd,string v_usr);

  void slot_addDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
  void slot_addUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);

  void slot_delDev( long long v_fd,string v_usr,string v_cmaname);
  void slot_delUser( long long v_fd,string v_usr,string v_username);
  void slot_emptyNVR( long long v_fd,string v_usr);

  void slot_setDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data);
  void slot_setUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data);
  void slot_setNVR( long long v_fd,string v_usr,ns_tcs_ds_def::NvrDev v_data);

  void slot_stopService( long long v_fd,string v_usr);
  void slot_directCamOp( long long v_fd,string v_usr,ns_tcs_ds_def::DirectCamOp v_data);

  void initTimeRecord();
  void on_checkAndConcludeTask();

  ns_tcs_ds_def::TimeRecord getCurTimeRecord();

private:
  APIServer* _apiserver;
  DevProxy* _devproxy;
  DataManage::DataColl *_datacoll;
  ns_tcs_ds_def::TimeRecord  _timerecord;
  /// 用于发现任务队列中的任务并执行
  QTimer*   _timer_task;

  /// 将log通过WS发送给所有admin/assit类型的用户
  /// 小心不要在WS处理过程中调用，否则会产生死锁
  void pushLogToAdvanceUser(string v_log);
  enum ENUM_LOOPTASK{
    ENUM_LOOPTASK_WS_PUSHLOG,  //{"user","log"}
    ENUM_LOOPTASK_WS_RESTART,  //{"user"}
    ENUM_LOOPTASK_WS_USERSTAUS,//{"user",["data"]}
    ENUM_LOOPTASK_WS_DEVSTATUS //{"user","[data]"}
  };
  vector<pair<ENUM_LOOPTASK,vector<string>>> _list_task;

  /// 添加某项任务，任务都是与WS相关，是因为底层WS设计的缺陷，导致WS的收发同时被调用时会产生死锁现象
  ///
  void addLoopTask(ENUM_LOOPTASK v_type,vector<string> l_para);
};

#endif // CONTROLER_H
