#ifndef CONCRETETASKEXECUTOR_H
#define CONCRETETASKEXECUTOR_H


#include <iostream>
#include <string>
#include "taskexecutor.h"
#include "../ds_df_diss/ds_df_diss.h"
#include "../runway_scan_stitch/include/runwayio/runwayio.h"
#include "../runway_scan_stitch/include/process_module/process_runway.h"




class ConcreteTaskExecutor
{
public:
  ConcreteTaskExecutor();
};


class RscGetTaskExecutor:public TaskExecutor{
public:
  RscGetTaskExecutor(const ds_df_diss::Resources_JE &v_para,const string &v_str,long long v_fd ):
    __rsc_para(v_para),__dir(v_str),__fd(v_fd)
  {}
  int conclude(const string &v_command){return -1;}
  int conclude();
  ~RscGetTaskExecutor(){}

private:
  ds_df_diss::Resources_JE __rsc_para;
  string __dir;
  long long __fd;
};


class Notice{
public:
  enum Notice_Type{
    Notice_Type_succeed,
    Notice_Type_fail
  };

  Notice(){}
  virtual ~Notice(){}
  virtual void sendNotice(ds_df_diss::Task_JE v_task)=0;
private:

};

/// 将有关数据保存到任务执行器
/// 当该任务直至其执行时
///   1. 获取请求所有资源，保存在本地
///   2. 根据本地保存的文件进行拼接
///   3. 获得拼接结果，将拼接结果告知给控制程序
class StitchImageInDirTaskExecutor:public TaskExecutor{
public:
  StitchImageInDirTaskExecutor(const ds_df_diss::Task_JE &v_para,const string &v_str,long long v_fd ,Notice* __noticer):
    __task(v_para),__dir(v_str),__fd(v_fd),__noticer(__noticer)
  {}
  int conclude(const string &v_command){return -1;}
  int conclude();
  ~StitchImageInDirTaskExecutor(){}

private:
  ds_df_diss::Task_JE __task;
  string __dir;
  long long __fd;
  Notice *__noticer;
};

string getBlockDir(const ds_df_diss::Task_JE& v_task);
string getBlockResultDir(const ds_df_diss::Task_JE& v_task);



#endif // CONCRETETASKEXECUTOR_H
