#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "../view/mainwindow.h"
#include "../view/initview.h"
#include <string>
#include <iostream>
#include <QMutex>
#include <QMutexLocker>
#include "../../../common_lib/sskj_lib/threadpool/threadpool.h"
#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/api_diss/dissapi.h"
#include "../../../common_lib/sskj_lib/threadpool/concretetaskexecutor.h"





using namespace std;
class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(InitView *v_init,MainWindow *v_mw,QObject *parent = nullptr);
  ~Controller();

signals:
  void sgl_on_warning(QString);

public slots:
  void slt_on_initConfig(QString v_ip,int v_count);


private:
  string __clientip;
  int __count_thread;
  ThreadPool *__threadpool;
  ThreadPool *__threadpool_smalltask;

  InitView *__initview;
  MainWindow *__mainwindow;

  vector<ds_df_diss::Task_JE> __l_task;
  QMutex __mutex_data;


  static int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                         void *ptr);

  static int cb_CallbackOnHttpError(int error_code, void*ptr);

  map<ds_df_diss::ENUM_DISS_CTR_NOTI,string> __l_noti;



};

#endif // CONTROLLER_H
