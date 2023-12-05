#ifndef DATAMANAGE_H
#define DATAMANAGE_H

/// 这里维护两个列表 待分配任务列表|已分配任务列表
/// 这里根据设计成支持多个项目同时进行，即每个任务包含项目名和执行端 根据项目名去寻找相应的输入和输出文件夹（在另一区域进行管理）
/// 均支持任务的增减和任务的提取
/// 接口设计按照严格的对象管理

#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"




class TaskManage_JE:public ds_df_diss::TaskList_JE
{

public:
  TaskManage_JE();
  TaskManage_JE(const vector<ds_df_diss::Task_JE> &v_l_task);
  void pushback(const ds_df_diss::Task_JE& v_task);
  void pushback(const vector<ds_df_diss::Task_JE>& l_task);
  void pushfront(const vector<ds_df_diss::Task_JE>& l_task);
  /// 取出最上层的数据并将该数据删除
  int popFront(ds_df_diss::Task_JE &v_task);

  void erase(const ds_df_diss::Task_JE& v_task);
  void erase(const vector<ds_df_diss::Task_JE>& l_task);

  vector<ds_df_diss::Task_JE> getTaskList() const ;
  unsigned long size();

//private:
//  vector<ds_df_diss::Task_JE>  __l_task;

};



//class ClientList_JE:public ds_df_diss::JsonEle
//{
//public:


//};




#endif // DATAMANAGE_H
