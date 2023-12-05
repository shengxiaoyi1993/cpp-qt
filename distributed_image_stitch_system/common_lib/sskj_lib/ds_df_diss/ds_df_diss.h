#ifndef DS_DF_DISS_H
#define DS_DF_DISS_H

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include "../../third_party_lib/cjsonobject/CJsonObject.hpp"


using namespace std;
namespace ds_df_diss {

extern const vector<string> __g_l_req;

class PortConfig{
public:
  const static int __port_server;
  const static int __port_client;

};



inline
long getTimeNs()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME,&ts);
  return ts.tv_sec*1000000000+ts.tv_nsec;
}


inline
int getRandomNum(){

  srand(static_cast<uint>(getTimeNs()));
  return rand() ;
}

class DebugHelp{
public:
  DebugHelp(const string &v_str):
    __info(v_str)
  {
    __time_record=getTimeNs();
    cout<<__info<<" ---start"<<endl;

  }
  ~DebugHelp(){
    long timecost=getTimeNs()-__time_record;
    cout<<__info<<" ---end timecost:"<<timecost<<endl;
  }



#define LIFETIMETRACE(x) DebugHelp  DebugHelp_tmp(x);

private:
  string __info;
  long __time_record;


};


enum ENUM_DISS_CTR_NOTI{
  ENUM_DISS_CTR_NOTI_IP_WRONG,
  ENUM_DISS_CTR_NOTI_DIRNOTSET,
  ENUM_DISS_CTR_NOTI_INPUT_NOT_COMPLETE,
  ENUM_DISS_CTR_NOTI_SETHTTPFAIL,
  ENUM_DISS_CTR_NOTI_STOPLISTTENFAIL,
  ENUM_DISS_CTR_NOTI_PREVIOUS_NOT_READY,
  ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK,
  ENUM_DISS_CTR_NOTI_CLIENT_NOT_CONNECT
};


enum ENUM_CTR_STATUS{
  ENUM_CTR_STATUS_SETIP=0,
  ENUM_CTR_STATUS_LISTEN,
  ENUM_CTR_STATUS_SETDIR,
  ENUM_CTR_STATUS_INIT,
  ENUM_CTR_STATUS_WORK,
  ENUM_CTR_STATUS_COMPLETE
};



class JsonEle{
public:
  virtual ~JsonEle(){};
  virtual neb::CJsonObject jsonObj()=0;
  JsonEle(){}

};


class ClientStatus_JE:public JsonEle {

public:
  ClientStatus_JE(const neb::CJsonObject& v_cjo);
  ClientStatus_JE(const string &v_id_client,
                  int v_count_total_thread,
                  int v_count_working_thread,
                  int v_count_total_task);
  ClientStatus_JE();

  ~ClientStatus_JE();
  neb::CJsonObject jsonObj();
  ClientStatus_JE operator=(const ClientStatus_JE& other);

  string __id_client;
  int __count_total_thread;
  int __count_working_thread;
  int __count_total_task;
};




class Task_JE:public JsonEle{
public:
  Task_JE(const neb::CJsonObject& v_cjo);
  Task_JE();
  Task_JE( const string  &v_id_server,
           const string  &v_id_client,
           const string  &v_id_project,
           int   v_group,
           int  v_block,
           int __status=0
           );
  ~Task_JE();
  neb::CJsonObject jsonObj();
  bool operator==(const Task_JE& other);
  Task_JE operator=(const Task_JE& other);
  bool isSucceed() const;

  string __id_server;
  string __id_client;
  string __id_project;
  int  __group;
  int __block;
  /// 执行状态
  /// 0：未执行
  /// 1：执行成功
  /// -1：执行失败
  int __status;
};





class Resources_JE:public JsonEle{
public:
  Resources_JE(const neb::CJsonObject& v_cjo);
  Resources_JE();
  Resources_JE( const Task_JE &v_task ,
                const string& v_name ,//资源名
                int v_type_resources , //文本文件或者图片文件
                int v_type_data);
  ~Resources_JE();
  neb::CJsonObject jsonObj();

//private:
  Task_JE __task;
  string __name;//资源名0
  int __type_resources; //0文本文件或者1图片文件
  int __type_data;//路径，是属于0源数据还是1结果数据

};


class TaskList_JE:public JsonEle{
public:
  TaskList_JE(const neb::CJsonObject& v_cjo);
  TaskList_JE();
  TaskList_JE(const vector<Task_JE>  &v_l_task);
  ~TaskList_JE();
  neb::CJsonObject jsonObj();
  vector<Task_JE>  __l_task;
};



class String_JE:public JsonEle{
public:
  String_JE(const neb::CJsonObject& v_cjo);
  String_JE();
  String_JE(const string  &v_str);
  ~String_JE();
  neb::CJsonObject jsonObj();
private:
  string __str;
};


template <typename T>
class MsgWithType_JE:public JsonEle{
public:


  MsgWithType_JE(const neb::CJsonObject& v_cjo){
    neb::CJsonObject obj_data;
    v_cjo.Get("__data",obj_data);
    __data=T(obj_data);
    v_cjo.Get("__type",__type);
  }


  MsgWithType_JE():
    __type(-1),__data()
  {

  }


  MsgWithType_JE(  int v_req_type,
                   const T &v_data=T()):
    __type(v_req_type),__data(v_data)
  {

  }


  ~MsgWithType_JE(){}


  neb::CJsonObject jsonObj()
  {
    neb::CJsonObject v_cjo;
    v_cjo.Add("__data",__data.jsonObj());
    v_cjo.Add("__type",__type);
    return v_cjo;
  }


public:
  int __type;
  T __data;
};






template<class T>
inline neb::CJsonObject jsonFromArray(const vector<T> &v_list){
  neb::CJsonObject obj_root;

  for (uint i=0;i<v_list.size();i++) {
    neb::CJsonObject tmp=((T)v_list[i]).jsonObj();
    obj_root.Add(tmp);

  }

  return obj_root;
}



template <class T>
inline vector<T> arrayFromJson( neb::CJsonObject& v_json){
  vector<T> list_ret;
  int size=v_json.GetArraySize();
  for (int i=0;i<size;i++) {
    T it(v_json[i]);
    list_ret.push_back(it);
  }
  return list_ret;
}


};






#endif // DS_DF_DISS_H
