#include "datamanage.h"
#include <vector>
#include <iostream>
#include <algorithm>



TaskManage_JE::TaskManage_JE()
{
__l_task={};
}


TaskManage_JE::TaskManage_JE(const vector<ds_df_diss::Task_JE> &v_l_task)
{
    __l_task=(v_l_task);
}

void TaskManage_JE::pushback(const vector<ds_df_diss::Task_JE>& l_task){

  for (auto it=l_task.begin();it!=l_task.end();it++) {
    auto pos_find=find_if(__l_task.begin(),__l_task.end(),[=](ds_df_diss::Task_JE tmp){
      return tmp==*it;
    });

    if (pos_find == __l_task.end()) {
      __l_task.push_back(*it);
    }
  }
}


void TaskManage_JE::pushfront(const vector<ds_df_diss::Task_JE>& l_task){

  for (auto it=l_task.begin();it!=l_task.end();it++) {
    auto pos_find=find_if(__l_task.begin(),__l_task.end(),[=](ds_df_diss::Task_JE tmp){
      return tmp==*it;
    });

    if (pos_find == __l_task.end()) {
//      __l_task.push_back(*it);
      __l_task.insert(__l_task.begin(),*it);

    }
  }

}



void TaskManage_JE::pushback(const ds_df_diss::Task_JE& v_task)
{

  auto pos_find=find_if(__l_task.begin(),__l_task.end(),[=](ds_df_diss::Task_JE tmp){
    return tmp==v_task;
  });

  if (pos_find == __l_task.end()) {
    __l_task.push_back(v_task);
  }

}




void TaskManage_JE::erase(const ds_df_diss::Task_JE& v_task){

  bool find=false;
  do {
    auto  pos_find=find_if(__l_task.begin(),__l_task.end(),[=](ds_df_diss::Task_JE tmp){
      return tmp==v_task;
    });

    if (pos_find != __l_task.end()) {
//      __l_task.push_back(v_task);
      __l_task.erase(pos_find);
      find=true;
    }
    else{
      find=false;
    }
  } while (find == true);

}


vector<ds_df_diss::Task_JE> TaskManage_JE::getTaskList() const {

  return __l_task;

}

unsigned long TaskManage_JE::size(){
  return __l_task.size();
}



void TaskManage_JE::erase(const vector<ds_df_diss::Task_JE>& l_task){
  for (auto it =l_task.begin();it!=l_task.end();it++) {
    bool find=false;
    do {
      auto pos_find=find_if(__l_task.begin(),__l_task.end(),[=](ds_df_diss::Task_JE tmptask){
        return tmptask==*it;
      });
      if (pos_find != __l_task.end()) {
        __l_task.erase(pos_find);
        find=true;
      }
      else{
        find=false;
      }
    }while (find == true);
  }

}

int TaskManage_JE::popFront(ds_df_diss::Task_JE &v_task){

  int ret=0;
  if (!__l_task.empty()) {
    v_task=__l_task[0];
    ret=0;
  }
  else{
    ret=-1;
  }
  return ret;
}



