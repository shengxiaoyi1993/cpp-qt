#ifndef DISSAPI_H
#define DISSAPI_H

#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/com_client/include/com_client_export.h"
#include "../../../common_lib/sskj_lib/com_server/include/com_server_export.h"

/// 该模块中包含了server和client的代码
/// 为了区分两者调用的函数，server调用的函数以"s_"开头，client调用的函数以"s_"开头，其他函数两者均可调用

namespace ns_DissApi{


enum ENUM_HTTP_ERRCODE{
  ENUM_HTTP_ERRCODE_SUCCEED=200,  ///正常
  ENUM_HTTP_ERRCODE_WRONGFORMAT=400,  ///错误请求 — 请求中有语法问题，或不能满足请求。
  ENUM_HTTP_ERRCODE_UNAUTHORIZED =401,  ///未授权 — 未授权客户机访问数据。
  ENUM_HTTP_ERRCODE_OPERATEFAILURE=404,  ///404  找不到 — 服务器找不到给定的资源/数据；文档不存在。
  ENUM_HTTP_ERRCODE_DISCONNECT=-1      ///未成功连接

};


enum ENUM_REQ_TYPE{
  ///HTTP
  ENUM_REQ_S_reqClientStatus,
  ENUM_REQ_S_reqClientTaskList,
  ENUM_REQ_S_reqClientResource,
  ENUM_REQ_S_reqTaskReady,
  ENUM_REQ_C_reqTaskComplete,
  ENUM_REQ_C_reqServerResource

};

/// 初始化HTTP通讯环境，仅在程序开始时调用一次
int initHTTPEnv();

/// 释放HTTP通讯资源，仅在程序结束或者不使用HTTP后调用
int uinitHTTPEnv();


// 开始监听http端口
// 其中，ptr 会在接收到请求时，做为运行 callback_on_http_request
// 的最后一个参数，可以是 NULL
int setupListenHttp(CallbackOnNewHttpRequest callback_on_http_request,
                    CallbackOnHttpError callback_on_http_error, void *ptr);

int setupListenHttp(CallbackOnNewHttpRequest callback_on_http_request,
                    CallbackOnHttpError callback_on_http_error, void *ptr,int port);

// 发送http response
// fd 连接句柄
// data_type 数据类型（json,jpeg,png,mp4,xls），用来标记 content_type
// data 要发送的数据
// data_length 数据长度
int sendHttpResponse(long long int fd, char *data_type, unsigned char *data,
                     int data_length);

// 关闭 http 连接
int closeHttpConnection(long long int fd);

// 关闭所有 http 连接并停止监听 http 端口
int stopListenHttp();


/// 返回数据只有状态码时的简化接口
int sendHttpResponseWithReturnCode(long long int fd,ENUM_HTTP_ERRCODE v_vode);


}

/// 请求的函数形式与请求处理的处理过程
/// 请求函数形式
/// 1. 返回值表示服务器返回的错误码
/// 2. 参数表中，给定的参数在前，返回的参数在后


/// 请求的处理过程
/// 1. 请求的接收在回调函数中
/// 2. 由该回调函数进行数据的解析，然后传递给上层的数据处理队列中
/// 3. 传递给上层的一个任务分发函数，该函数对数据进行解析

namespace ns_DissApi{
namespace  ns_server{
///
int reqClientStatus(const string &v_client,ds_df_diss::ClientStatus_JE& v_status);

int reqClientTaskList(const string &v_client,const string &v_server, ds_df_diss::TaskList_JE& v_tasklist);

int reqClientResource(const string &v_client,
                      const ds_df_diss::Resources_JE &v_rec, unsigned char **output,int &bufsize
                      );

int reqTaskPush(const string &v_client,const ds_df_diss::Task_JE& v_task);

}

}

namespace ns_DissApi{
namespace  ns_client{

int reqTaskComplete(const string &v_client,const ds_df_diss::Task_JE& v_task);

int reqServerResource(const string &v_client,
                      const ds_df_diss::Resources_JE &v_rec, unsigned char **output, int &bufsize);


}


}






#endif // DISSAPI_H
