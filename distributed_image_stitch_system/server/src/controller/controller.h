#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <map>
#include "../datamanage/datamanage.h"
#include "../../../common_lib/sskj_lib/runway_scan_stitch/include/runwayio/runwayio.h"
#include "../../../common_lib/sskj_lib/runway_scan_stitch/include/basic/basic.h"
#include "../view/mainwindow.h"
#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/threadpool/concretetaskexecutor.h"
#include "../../../common_lib/sskj_lib/threadpool/threadpool.h"

#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
//#include <algorithm>



/// 初始化过程应包括服务IP的正确设置
/// 启动监听与开始工作应该是两个不同的过程
///   设置IP后就启动监听
///   资源正常配置后才开始工作，规定必须在工作与资源配置（初始化）是两个互斥的过程
/// 对于的监听到的请求，应该

class Controller : public QThread
{
  Q_OBJECT


public:

  explicit Controller( MainWindow* v_mw,ThreadPool* v_tp,QObject *parent = nullptr);
  void resetInputFrame(string v_str);
  void resetOutputFrame(string v_str);
  ~Controller();
  void run();
  void initConfig();


  /// 1. 在已经配置输入与输出文件时调用
  /// 2. 初始化出错就中断，提示用户解决
  /// 3. 初始化过程主要是主要载入任务列表|检查资源的完整性|检查输出文件|生成输出文件夹结构|
  /// 4. 由于初始化过程的会消耗大量时间可放在线程中执行


signals:
  void sgl_on_warning(QString);
  void sgl_on_status_change(map<ds_df_diss::ENUM_CTR_STATUS,bool>);

public slots:
  void slt_on_changeInputDir(QString);
  void slt_on_changeOutputDir(QString);
  void slt_on_changeIP(QString);
  void slt_on_addclient(QString);

  void slt_on_startListen();
  void slt_on_stopListen();
  /// 1. 启动拼接，此时应保证初始化已完成
  /// 2. 开始正式任务，包括
  ///   向客户端列表请求状态
  ///   更新已分配任务的的执行状态
  ///   分配新的任务
  void slt_on_startWork();
  void slt_on_stopWork();
  /// 初始化程序
  void slt_on_init();
  void slt_on_timer_pushworkloop_timeout();






private:
  TaskManage_JE* __tasklist_wait;
  TaskManage_JE* __tasklist_op;

  /// 规定这两个数据结构必须同步
  runwayio::InputFrame __frame_input;
  runwayio::InputData __info_input;

  /// 规定这两个数据结构必须同步
  runwayio::OutputFrame __frame_output;
  runwayio::OutputData __info_output;

  MainWindow* __mainwindow;


  vector<ds_df_diss::ClientStatus_JE> __l_client_status;

  string __ip_server;

  /// 隔一段时间将工作循环加入线程执行列表
  QTimer* __timer_pushworkloop;
  /// s
  int __time_interval;
  bool __status_run;


  map<ds_df_diss::ENUM_DISS_CTR_NOTI,string> __l_noti;

  map<ds_df_diss::ENUM_CTR_STATUS,bool> __l_status;

  enum ENUM_CTR_FUNC{
    ENUM_CTR_FUNC_INIT=0,
    ENUM_CTR_FUNC_WORKLOOP,
    ENUM_CTR_FUNC_REQ_PROCESS
  };
  vector<ENUM_CTR_FUNC> __l_func;

  vector<ds_df_diss::Task_JE> __l_task_completee;

  ThreadPool* __threadpool_req;

  QMutex  __mutex_data;



  /// 更新文件的设置 输入文件和输出文件
  void updateDirFrame();

  /// 更新任务列表，将输入程序写入到
  void updateTaskList();


  static int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                         void *ptr);

  static int cb_CallbackOnHttpError(int error_code, void*ptr);

  /// 进程内函数
  /// 根据标识判断需要执行的任务
  void executeFunc(ENUM_CTR_FUNC v_func);

  /// 根据已有公共资源，进行
  ///   向客户端列表请求状态
  ///   更新已分配任务的的执行状态
  ///   分配新的任务
  void workLoop();


  /// 执行client的请求
  /// 请求资源类型的请求，单独开线程进行处理
  /// 需要告知srever一些信息的请求，将该数据放到一请求列表中，进行协调处理
  /// - 告知某任务已经完成
  void processRequest();

//  /// 根据当前项目及给定的图片块位置计算出该图片块中包含的文件
//  vector<ds_df_diss::Resources_JE> getBlockRscList(const ds_df_diss::Task_JE&v_task);

  /// 对于获取资源的程序，单独建立线程处理


};

#endif // CONTROLLER_H
