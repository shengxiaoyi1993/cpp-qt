#include "ds_df_diss.h"

namespace ds_df_diss {
const vector<string> __g_l_req={};

const int PortConfig::__port_client=50070;
const int PortConfig::__port_server=50071 ;





ClientStatus_JE::ClientStatus_JE(const neb::CJsonObject& v_cjo){
  v_cjo.Get("__id_client",__id_client);
  v_cjo.Get("__count_total_thread",__count_total_thread);
  v_cjo.Get("__count_working_thread",__count_working_thread);
  v_cjo.Get("__count_total_task",__count_total_task);
}


ClientStatus_JE::ClientStatus_JE(
    const string &v_id_client,
    int v_count_total_thread,
    int v_count_working_thread,
    int v_count_total_task):
  __id_client(v_id_client),
  __count_total_thread(v_count_total_thread),
  __count_working_thread(v_count_working_thread),
  __count_total_task(v_count_total_task)
{

}

ClientStatus_JE::ClientStatus_JE():
  __id_client(""),
  __count_total_thread(0),
  __count_working_thread(0),
  __count_total_task(0)
{}


ClientStatus_JE::~ClientStatus_JE(){}


neb::CJsonObject ClientStatus_JE::jsonObj(){
  neb::CJsonObject v_cjo;
  v_cjo. Add("__id_client",__id_client);
  v_cjo. Add("__count_total_thread",__count_total_thread);
  v_cjo. Add("__count_working_thread",__count_working_thread);
  v_cjo. Add("__count_total_task",__count_total_task);
  return v_cjo;
}

ClientStatus_JE ClientStatus_JE::operator=(const ClientStatus_JE& other){
  if (this!=&other) {
    this->__id_client=other.__id_client;
    this->__count_total_thread=other.__count_total_thread;
    this->__count_working_thread=other.__count_working_thread;
    this->__count_total_task=other.__count_total_task;
    return *this;
  }
}



//=========================================================================

Task_JE::Task_JE(const neb::CJsonObject& v_cjo){
  v_cjo.Get("__id_server",__id_server);
  v_cjo.Get("__id_client",__id_client);
  v_cjo.Get("__id_project",__id_project);
  v_cjo.Get("__group",__group);
  v_cjo.Get("__block",__block);
  v_cjo.Get("__status",__status);

}

Task_JE::Task_JE():
  __id_server(""),
  __id_client(""),
  __id_project(""),
  __group(0),
  __block(0)
{

}


Task_JE::Task_JE( const string  &v_id_server,
                  const string  &v_id_client,
                  const string  &v_id_project,
                  int   v_group,
                  int  v_block,
                  int v_status):
  __id_server(v_id_server),
  __id_client(v_id_client),
  __id_project(v_id_project),
  __group(v_group),
  __block(v_block),
  __status(v_status)
{

}

Task_JE Task_JE::operator=(const Task_JE& other){
  if (this!=&other) {
    this->__id_server=other.__id_server;
    this->__id_client=other.__id_client;
    this->__id_project=other.__id_project;
    this->__group=other.__group;
    this->__block=other.__block;
    this->__status=other.__status;


  }
  return *this;
}

Task_JE::~Task_JE(){}
neb::CJsonObject Task_JE::jsonObj(){
  neb::CJsonObject v_cjo;
  v_cjo.Add("__id_server",__id_server);
  v_cjo.Add("__id_client",__id_client);
  v_cjo.Add("__id_project",__id_project);
  v_cjo.Add("__group",__group);
  v_cjo.Add("__block",__block);
  v_cjo.Add("__status",__status);

  return v_cjo;
}

bool Task_JE::operator==(const Task_JE& other)
{
  if (this->__id_project == other.__id_project
      && this->__group == other.__group
      &&this->__block == other.__block)
  {
    return true;
  }
  return false;
}

bool Task_JE::isSucceed() const{
  bool ret=true;
  if (__status==1) {
    ret=true;
  }
  else{
    ret=false;
  }
  return ret;
}



//=========================================================================
Resources_JE::Resources_JE(const neb::CJsonObject& v_cjo){
  neb::CJsonObject obj__task;
  v_cjo.Get("__task",obj__task);
  __task=Task_JE(obj__task);
  v_cjo.Get("__type_data",__type_data);
  v_cjo.Get("__name",__name);
  v_cjo.Get("__type_resources",__type_resources);


}

Resources_JE::Resources_JE():
  __type_data(0),
  __task(Task_JE()),
  __name(""),
  __type_resources(){

}

Resources_JE::Resources_JE( const Task_JE &v_task ,
                            const string& v_name ,//资源名
                            int v_type_resources , //文本文件或者图片文件
                            int v_type_data):
  __type_data(v_type_data),
  __task(v_task),
  __name(v_name),
  __type_resources(v_type_resources)
{

}


Resources_JE::~Resources_JE(){}
neb::CJsonObject Resources_JE::jsonObj(){
  neb::CJsonObject v_cjo;
  neb::CJsonObject obj__task=__task.jsonObj();
  v_cjo. Add("__task",obj__task);
  v_cjo. Add("__type_data",__type_data);
  v_cjo. Add("__name",__name);
  v_cjo. Add("__type_resources",__type_resources);
  return v_cjo;
}


//=========================================================================

TaskList_JE::TaskList_JE(const neb::CJsonObject& v_cjo){
  neb::CJsonObject tmpobj ;
  v_cjo.Get("__l_task",tmpobj);
  __l_task=arrayFromJson<Task_JE>(tmpobj);

}
TaskList_JE::TaskList_JE():
  __l_task({})
{}
TaskList_JE::TaskList_JE(const vector<Task_JE>  &v_l_task):
  __l_task(v_l_task)
{
}

TaskList_JE::~TaskList_JE(){}

neb::CJsonObject TaskList_JE::jsonObj(){
  neb::CJsonObject v_cjo;
  v_cjo.Add("__l_task",jsonFromArray<Task_JE>(__l_task));
  return v_cjo;
}

////=========================================================================

String_JE::String_JE(const neb::CJsonObject& v_cjo){
  v_cjo.Get("__str",__str);
}

String_JE::String_JE():__str(""){

}

String_JE::String_JE(const string  &v_str):__str(v_str){

}

String_JE::~String_JE(){

}

neb::CJsonObject String_JE::jsonObj(){
  neb::CJsonObject v_cjo;
  v_cjo.Add("__str",__str);
  return v_cjo;
}

////=========================================================================


};
