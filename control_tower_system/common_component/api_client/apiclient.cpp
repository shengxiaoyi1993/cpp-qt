#include "apiclient.h"
#include "ds_def/tcs_ds_def.h"
#include "cjsonobject/CJsonObject.hpp"
#include <QMetaType>
static APIClient* pThis=nullptr;


APIClient::APIClient(QObject *parent) : QObject(parent)
{
  qRegisterMetaType<string>("string");
  qRegisterMetaType<vector<ns_tcs_ds_def::UserStatus>>("vector<ns_tcs_ds_def::UserStatus>");

  pThis=this;
  int flag_init=InitHTTPEnv();
  if(flag_init != 0){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
  }
}

APIClient::~APIClient(){
  DisableHTTPEnv();
  Logout();

}


int APIClient::setAuthPara(const AuthParam &v_authpara){
  return copyAuthParam(_authpara,v_authpara);
}


int APIClient::login(){
  return Login(_authpara,cb_CallbackOnNewMessage,cb_CallbackOnClosed,cb_CallbackOnLogin);
}

AuthParam APIClient::getAuthPara()const{
  AuthParam ret;
  copyAuthParam(ret,_authpara);
  return ret;
}

bool APIClient::getStatus()const{
  return _status;
}

int APIClient::logout(){
  Logout();
  return 0;
}

// 请务必提前申请好 output_type/buffer_len 的内存
// 函数内部会申请内存存储返回值，请务必在用完后释放 (*output)
int APIClient::sendHTTPRequest(const string& v_input,string &output_type,
                               unsigned char **output, int &buffer_len){
  char ctype[32];
  char *cstr = new char[v_input.length() + 1];
  strcpy(cstr, v_input.c_str());
  int flag=SendHTTPRequest(_authpara.server_ip,cstr,ctype,output,&buffer_len);
  output_type=string(ctype);
  delete [] cstr;
  return flag;
}

// 请务必提前申请好 output_type/buffer_len 的内存
// 函数内部会申请内存存储返回值，请务必在用完后释放 (*output)
int APIClient::sendSecretRequest(const string& v_input,string &output_type,
                                 unsigned char** output, int &buffer_len){
  char ctype[32];
  int size_ouput;
  char *cstr = new char[v_input.length() + 1];
  strcpy(cstr, v_input.c_str());
  cout<<"username:"<<_authpara.username<<endl;
  cout<<"server_ip:"<<_authpara.server_ip<<endl;
  cout<<"password:"<<_authpara.password<<endl;

  int flag=SendSecretRequest(_authpara,cstr,ctype,output,&size_ouput);
  cout<<"size_ouput:"<<size_ouput<<endl;

  output_type=string(ctype);
  buffer_len=size_ouput;
  delete [] cstr;
  return flag;
}



///
int APIClient::getServerTimeCode(unsigned int& timecode){
  return GetServerTimeCode(_authpara.server_ip,&timecode);
}


ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_querySINGLEUSER(const string &v_username,
                                                                ns_tcs_ds_def::User& v_user){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_SINGLEUSER)
                             .jsonobjWithData(ns_tcs_ds_def::SingleString(v_username).jsonobj()).ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_user=ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::User>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }

}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryALLUSER( vector<ns_tcs_ds_def::User>& v_users){

  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_ALLUSER).jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );
  cout<<"ouput:"<<ouput<<endl;

  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_users=ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::User>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }

}



///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryRUNTME(ns_tcs_ds_def::TimeRecord& v_info){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_RUNTME).jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );
  cout<<"ouput:"<<ouput<<endl;

  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_info=ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::TimeRecord>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryUSERSTAUS(vector<ns_tcs_ds_def::UserStatus>& v_list){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_USERSTAUS).jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );

  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_list=ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::UserStatus>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryDEVARRAY(ns_tcs_ds_def::CameraArray& v_array){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_DEVARRAY).jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );
  cout<<"ouput:"<<ouput<<endl;

  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_array=ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::CameraArray>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }
}

///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryDEVSTATUS(vector<ns_tcs_ds_def::CamStatus>& v_list){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_DEVSTATUS).jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );

  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_list=ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::CamStatus>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryQRYNVR(ns_tcs_ds_def::NvrDev &v_nvr){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_QRYNVR).jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );
  cout<<"ouput:"<<ouput<<endl;

  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      v_nvr=ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::NvrDev>(obj_return["data"]);
    }
    return retinfo.getReturnCode();
  }
}

ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_querySerialPort(vector<string> &l_port){
    string otype;
    unsigned char* ouput=nullptr;
    int size_output=0;
    int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_QRYSERIALPORT).jsonobj()
                               .ToString(),
                               otype,&ouput,size_output
                               );
    cout<<"ouput:"<<ouput<<endl;

    if(flag!=0){
      return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
    }
    else{
      neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
      ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
      if(retinfo.getReturnCode()==ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
        l_port=ns_tcs_ds_def::arrayFromJson<string>(obj_return["data"]);
      }
      return retinfo.getReturnCode();
    }
}



///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryADDDEV(const  ns_tcs_ds_def::CamDev& v_data){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_ADDDEV).jsonobjWithData(v_data.jsonobj())
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_queryADDUSER(const ns_tcs_ds_def::User& v_data){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_ADDUSER)
                             .jsonobjWithData(v_data.jsonobj())
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}



///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_DELDEV(const string& v_cmaname){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_DELDEV)
                             .jsonobjWithData(ns_tcs_ds_def::SingleString(v_cmaname)
                                              .jsonobj()).ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_DELUSER(const string &v_username){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_DELUSER)
                             .jsonobjWithData(ns_tcs_ds_def::SingleString(v_username)
                                              .jsonobj()).ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE APIClient::req_EMPTYNVR(){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_EMPTYNVR)
                             .jsonobj().ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}

///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE
APIClient::req_SETDEV(const ns_tcs_ds_def::CamDev &v_data){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_SETDEV)
                             .jsonobjWithData(v_data.jsonobj())
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}

///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE
APIClient::req_SETUSER(const ns_tcs_ds_def::User &v_data){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_SETUSER)
                             .jsonobjWithData(v_data.jsonobj())
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}

///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE
APIClient::req_SETNVR(const ns_tcs_ds_def::NvrDev v_data){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_SETNVR)
                             .jsonobjWithData(v_data.jsonobj())
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE
APIClient::req_STOPSERVICE(){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_STOPSERVICE)
                             .jsonobj()
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}


///
ns_tcs_ds_def::ENUM_HTTP_ERRCODE
APIClient::req_DIRECTCAM_OP(const ns_tcs_ds_def::DirectCamOp v_data){
  string otype;
  unsigned char* ouput=nullptr;
  int size_output=0;
  int flag=sendSecretRequest(ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_REQ_DIRECTCAM_OP)
                             .jsonobjWithData(v_data.jsonobj())
                             .ToString(),
                             otype,&ouput,size_output
                             );
  if(flag!=0){
    return ns_tcs_ds_def::ENUM_HTTP_ERRCODE_DISCONNECT;
  }
  else{
    neb::CJsonObject obj_return(std::string(reinterpret_cast<char*>(ouput)));
    ns_tcs_ds_def::ReturnInfo retinfo(obj_return);
    return retinfo.getReturnCode();
  }
}




int cb_CallbackOnNewMessage(char* message){
  cout<<__func__<<message<<endl;
  string str_msg(message);
  neb::CJsonObject root(str_msg);
  ns_tcs_ds_def::CtlInfo ctlinfo_tmp(root);

  switch (ctlinfo_tmp._req_type) {
    case  ns_tcs_ds_def::ENUM_WS_PUSHLOG:{
      emit pThis->sgl_WS_PUSHLOG(ns_tcs_ds_def::SingleString(root["data"])._str);
            break;
    }
    case  ns_tcs_ds_def::ENUM_WS_DEVSTATUS:{

      emit pThis->sgl_WS_DEVSTATUS(ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::CamStatus>(root["data"]));

      break;
    }
    case  ns_tcs_ds_def::ENUM_WS_RESTART:{
      emit pThis->sgl_WS_RESTART();

      break;
    }
    case  ns_tcs_ds_def::ENUM_WS_USERSTAUS:{
      emit pThis->sgl_WS_USERSTAUS(ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::UserStatus>(root["data"]));
      break;
    }
  }
  cout<<"end to send sgl_WS_PUSHLOG"<<endl;

  return 0;
}

// Websocket 连接被断开时的回调
int cb_CallbackOnClosed(){
  cout<<__func__<<endl;

  pThis->_status=false;
  emit pThis->sgl_WS_OnClosed();
  return 0;
}

// Websocket 连上，成功接收到服务器的 Hello 时的回调。
int cb_CallbackOnLogin(){
  cout<<__func__<<endl;

  pThis->_status=true;
  emit pThis->sgl_WS_OnLogin();
  return 0;
}








int copyAuthParam(AuthParam& v_para_dst,const AuthParam& v_para)
{
  memcpy(v_para_dst.password,v_para.password,sizeof (v_para.password));
  memcpy(v_para_dst.server_ip,v_para.server_ip,sizeof (v_para.server_ip));
  memcpy(v_para_dst.username,v_para.username,sizeof (v_para.username));
  return  0;
}

