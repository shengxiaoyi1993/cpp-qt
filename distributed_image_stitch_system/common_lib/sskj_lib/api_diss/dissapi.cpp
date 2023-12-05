#include "dissapi.h"
#include <cstring>
#include "stdlib.h"
#include <cstdlib>
namespace ns_DissApi{



int initHTTPEnv(){



      int ret =InitHTTPEnv();
      UserMap tmp[1];
      strcpy(tmp[0].username,"pub");
      strcpy(tmp[0].hash,"0017dea7770f7ecff7ab3c20506546129e96bdeba2f544bb8e5414eb79786122");
      SetAllUserMap(tmp,1);
      return ret;

}

int uinitHTTPEnv(){
  return DisableHTTPEnv();

}



int setupListenHttp(CallbackOnNewHttpRequest callback_on_http_request,
                    CallbackOnHttpError callback_on_http_error, void *ptr){
  return SetupListenHttp(callback_on_http_request,callback_on_http_error,ptr);
}


int setupListenHttp(CallbackOnNewHttpRequest callback_on_http_request,
                    CallbackOnHttpError callback_on_http_error, void *ptr,int port){
  return SetupListenHttpWithPort(callback_on_http_request,callback_on_http_error,ptr,port);

}

int sendHttpResponse(long long int fd, char *data_type, unsigned char *data,
                     int data_length){
  return SendHttpResponse(fd,data_type,data,data_length);
}

int closeHttpConnection(long long int fd){
  return CloseHttpConnection(fd);
}

int stopListenHttp(){
  return StopListenHttp();
}


int sendHttpResponseWithReturnCode(long long int fd,ENUM_HTTP_ERRCODE v_vode){
  ds_df_diss::MsgWithType_JE<ds_df_diss::String_JE> tt(v_vode);
  string response=tt.jsonObj().ToString();

  return sendHttpResponse(fd,"json",(unsigned char*)response.c_str(),response.size());

}

}


namespace ns_DissApi{
namespace  ns_server{

int reqClientStatus(const string &v_client,ds_df_diss::ClientStatus_JE& v_status){
  char output_type[32];
  unsigned char * output=nullptr;
  int bufsize=0;
  string request= ds_df_diss::MsgWithType_JE<ds_df_diss::String_JE>(ENUM_REQ_S_reqClientStatus).jsonObj().ToString();

  AuthParam  auth_para;
  strcpy(auth_para.username,"pub");
  strcpy(auth_para.password,"pub");
  strcpy(auth_para.server_ip,v_client.c_str());

  int ret=SendSecretRequestToPort(auth_para,
                  (char*)(request.c_str()),
                  output_type,&output,&bufsize
                  ,ds_df_diss::PortConfig::__port_client);
  cout<<__func__<<" ret:"<<ret<<endl;
  if (ret == 0) {
    cout<<"output:"<<output<<endl;
    if (strcmp("json",output_type) == 0 ) {
      char *str=new char[bufsize];
      memcpy(str,output,bufsize);
      neb::CJsonObject tmpobj(str);
      ds_df_diss::MsgWithType_JE<ds_df_diss::ClientStatus_JE> data_return(tmpobj);
      cout<<"data_return:"<<data_return.jsonObj().ToFormattedString()<<endl;

      if (data_return.__type==ENUM_HTTP_ERRCODE_SUCCEED) {
        v_status=data_return.__data;
      }
      delete []str;

    }
  }

  if (output !=nullptr) {
    free(output);
    output=nullptr;
  }


  return ret;
//  return -1;
}


int reqClientTaskList(const string &v_client,const string &v_server,
                      ds_df_diss::TaskList_JE& v_tasklist){
  AuthParam  auth_para;
  strcpy(auth_para.username,"pub");
  strcpy(auth_para.password,"pub");
  strcpy(auth_para.server_ip,v_client.c_str());
  char output_type[32];
  unsigned char * output=nullptr;
  int bufsize;
  int ret=SendSecretRequestToPort(auth_para,
                  (char*)(ds_df_diss::MsgWithType_JE<ds_df_diss::String_JE>(ENUM_REQ_S_reqClientTaskList,v_server).jsonObj().ToFormattedString().c_str()),
                  output_type,&output,&bufsize,ds_df_diss::PortConfig::__port_client
                  );
  if (ret == 0) {
    if (strcmp("json",output_type) == 0) {

      char *str=new char[bufsize];
      memcpy(str,output,bufsize);
      neb::CJsonObject tmpobj(str);
      ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE> data_return(tmpobj);
      if (data_return.__type==ENUM_HTTP_ERRCODE_SUCCEED) {
        v_tasklist=data_return.__data;
      }
      delete []str;


    }
  }

  if (output !=nullptr) {
    free(output);
    output=nullptr;
  }

  return ret;
}


int reqClientResource(const string &v_client,
                      const ds_df_diss::Resources_JE &v_rec,unsigned char **output,int &bufsize
                      ){
  AuthParam  auth_para;
  strcpy(auth_para.username,"pub");
  strcpy(auth_para.password,"pub");
  strcpy(auth_para.server_ip,v_client.c_str());
  char output_type[32];
  int ret=SendSecretRequestToPort(auth_para,
                  (char*)(ds_df_diss::MsgWithType_JE<ds_df_diss::Resources_JE>(ENUM_REQ_S_reqClientResource,v_rec).jsonObj().ToFormattedString().c_str()),
                  output_type,output,&bufsize,ds_df_diss::PortConfig::__port_client
                  );
  if (ret == 0) {
    if (strcmp("jpeg",output_type) == 0) {
    }
    else{
      ret=-1;
    }
  }
  if (output !=nullptr) {
    free(output);
    output=nullptr;
  }

  return ret;
}


int reqTaskPush(const string &v_client, const ds_df_diss::Task_JE& v_task){
  AuthParam  auth_para;
  strcpy(auth_para.username,"pub");
  strcpy(auth_para.password,"pub");
  strcpy(auth_para.server_ip,v_client.c_str());
  char output_type[32];
  unsigned char * output=nullptr;
  int bufsize;
  int ret=SendSecretRequestToPort(auth_para,
                  (char*)(ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE>(ENUM_REQ_S_reqTaskReady,v_task).jsonObj().ToFormattedString().c_str()),
                  output_type,&output,&bufsize,ds_df_diss::PortConfig::__port_client
                  );
  if (ret == 0) {
    if (strcmp("json",output_type) == 0) {
      char *str=new char[bufsize];
      memcpy(str,output,bufsize);
      neb::CJsonObject tmpobj(str);
      ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE> data_return(tmpobj);
//      neb::CJsonObject tmpobj((output));
//      ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE> data_return(tmpobj);
//      ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE> data_return(tmpobj);

      if (data_return.__type!=ENUM_HTTP_ERRCODE_SUCCEED) {
        ret=data_return.__type;
      }
      delete [] str;

    }
  }

  if (output !=nullptr) {
    free(output);
    output=nullptr;
  }
  return ret;


}



}
}



namespace ns_DissApi{
namespace  ns_client{
int reqTaskComplete(const string &v_client, const ds_df_diss::Task_JE& v_task){
  AuthParam  auth_para;
  strcpy(auth_para.username,"pub");
  strcpy(auth_para.password,"pub");
  strcpy(auth_para.server_ip,v_client.c_str());
  char output_type[32];
  unsigned char * output=nullptr;
  int bufsize;
  int ret=SendSecretRequestToPort(auth_para,
                  (char*)(ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE>(ENUM_REQ_C_reqTaskComplete,v_task).jsonObj().ToFormattedString().c_str()),
                  output_type,&output,&bufsize,ds_df_diss::PortConfig::__port_server
                  );
  if (ret == 0) {
    if (strcmp("json",output_type) == 0) {
//      neb::CJsonObject tmpobj(string(output));
//      ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE> data_return;

      char *str=new char[bufsize];
      memcpy(str,output,bufsize);
      neb::CJsonObject tmpobj(str);
      ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE> data_return(tmpobj);

      if (data_return.__type!=ENUM_HTTP_ERRCODE_SUCCEED) {
        ret=data_return.__type;
      }


    }
  }

  if (output !=nullptr) {
    free(output);
    output=nullptr;
  }

  return ret;


}

int reqServerResource(const string &v_client,
                      const ds_df_diss::Resources_JE &v_rec,unsigned char **output,int &bufsize){
  AuthParam  auth_para;
  strcpy(auth_para.username,"pub");
  strcpy(auth_para.password,"pub");
  strcpy(auth_para.server_ip,v_client.c_str());
  char output_type[32];
  int ret=SendSecretRequestToPort(auth_para,
                  (char*)(ds_df_diss::MsgWithType_JE<ds_df_diss::Resources_JE>(ENUM_REQ_C_reqServerResource,v_rec).jsonObj().ToFormattedString().c_str()),
                  output_type,output,&bufsize,ds_df_diss::PortConfig::__port_server
                  );
  if (ret == 0) {
    if (strcmp("jpeg",output_type) == 0||strcmp("xml",output_type) == 0) {
    }
    else{
      ret=-1;
    }
  }



  return ret;
}


}


}
