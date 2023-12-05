#ifndef LOGTOOL_H
#define LOGTOOL_H
#include <string>
#include<iostream>
#include <time.h>
using namespace std;

enum LogResType{
  LogResType_succeed,
  LogResType_fail,
  LogResType_notexecute
};

enum LogOutputType{
    LogOutputType_file
    ,LogOutputType_cout
//    ,LogOutputType_no
};

inline
string getTime()
{
  time_t timep;
  time (&timep);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
  return tmp;
}

inline
string getLogInfo(const string &v_operate,LogResType v_type,const string v_reason=""){
  string resultstring;
  switch (v_type) {
    case LogResType_succeed:{
      resultstring="succeed";
      break;
    }
    case LogResType_fail:{
      resultstring="fail";
      break;
    }
    case LogResType_notexecute:{
      resultstring="not execute";
      break;
    }
  }

  string ret;
  ret+=getTime()+" {"+v_operate+"} "
      +"<"+resultstring+">"
      +" for ("+v_reason+")";
  return ret;
}

class LogTool{
public:
  LogTool( ostream* v_ostream,const string &v_func):
    _m_ostream(v_ostream),_m_func(v_func)
  {
    (*_m_ostream)<<getTime()<<" {"<<_m_func<<"} <start>"<<endl;
  }
  ~LogTool(){
    (*_m_ostream)<<getTime()<<" {"<<_m_func<<"} <end>"<<endl;
  }

private:
  ostream* _m_ostream;
  string _m_func;
};


//(*_ostream)<<getLogInfo("sendWSMsg_DEVSTATUS:"+_list_task[0].second[0],
//        LogResType_succeed
//        );





#endif
