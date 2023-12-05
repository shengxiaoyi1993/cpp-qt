#ifndef COM_SERVER_EXPORT_H
#define COM_SERVER_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

// 接收到HTTP请求时的回调
// fd 句柄
// username 解析出的用户名
// req_data 解密后的实际数据
// ptr 在 Setup 时设置好的指针，c++类静态成员函数做为回调函数时可能需要用到
typedef int (*CallbackOnNewHttpRequest)(long long int fd, char *username, char *req_data,
                                        void *ptr);

// 当 HTTP 监听发生错误时的回调
typedef int (*CallbackOnHttpError)(int error_code, void*ptr);

// 当用户登录websocket时的回调
// username 用户名
// ptr 在 Setup 时设置好的指针，c++类静态成员函数做为回调函数时可能需要用到
typedef int (*CallbackOnUserLogin)(char *username, void *ptr);

// 当用户退出websocket时的回调
typedef int (*CallbackOnUserLogout)(char *username, void *ptr);

// 当 Websocket 监听发生错误时的回调
typedef int (*CallbackOnWebsocketError)(int error_code, void *ptr);

// 用户名和密码的hash
// 用于解密request data
typedef struct UserMap {
  char username[32];
  char hash[65];
} UserMap;

// 启动前，必须把所有用户的用户名、密码hash都设置好，否则进来的请求消息无法解密
// user_map UserMap 的数组
// count user_map 的长度
int SetAllUserMap(UserMap *user_map, int count);

// 开始监听http端口
// 其中，ptr 会在接收到请求时，做为运行 callback_on_http_request
// 的最后一个参数，可以是 NULL
int SetupListenHttp(CallbackOnNewHttpRequest callback_on_http_request,
                    CallbackOnHttpError callback_on_http_error, void *ptr);

int SetupListenHttpWithPort(CallbackOnNewHttpRequest callback_on_http_request,
                    CallbackOnHttpError callback_on_http_error, void *ptr,int port);

// 发送http response
// fd 连接句柄
// data_type 数据类型（json,jpeg,png,mp4,xls），用来标记 content_type
// data 要发送的数据
// data_length 数据长度
int SendHttpResponse(long long int fd, char *data_type, unsigned char *data,
                     int data_length);

// 关闭 http 连接
int CloseHttpConnection(long long int fd);

// 关闭所有 http 连接并停止监听 http 端口
int StopListenHttp();

// 开始监听 Websocket 端口
// 其中，ptr 会在连接创建或断开时，做为运行
// callback_on_user_login、callback_on_user_logout 的最后一个参数，可以是 NULL
int SetupListenWebsocket(CallbackOnUserLogin callback_on_user_login,
                         CallbackOnUserLogout callback_on_user_logout,
                         CallbackOnWebsocketError callback_on_websocket_error,
                         void *ptr);

// 发送 websocket 消息
int SendWebsocketMessage(char *user, char *data);

int SendWebsocketHeartBeat();

// 关闭 websocket 连接
int CloseWebsocketConnection(char *user);

// 关闭所有 websocket 连接并停止坚听 websocket 端口
int StopListenWebsocket();

static const int kErrorFailToListen = 9001;
static const int kErrorFailToAuth = 9002;

#ifdef __cplusplus
}
#endif

#endif // COM_SERVER_EXPORT_H
