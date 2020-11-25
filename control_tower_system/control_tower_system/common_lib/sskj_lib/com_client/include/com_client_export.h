#ifndef API_H
#define API_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AUTHPARAM
#define AUTHPARAM
// 用户的登录信息
typedef struct AuthParam {
  char server_ip[32];
  char username[50];
  char password[50];
} AuthParam;
#endif

//程序开始时立即调用以设置环境
int InitHTTPEnv();

//程序结束时须调用以处理环境
int DisableHTTPEnv();

// 请务必提前申请好 output_type/buffer_len 的内存
// 函数内部会申请内存存储返回值，请务必在用完后释放 (*output)
int SendHTTPRequest(char server_ip[32], char *input, char *output_type,
                    unsigned char **output, int *buffer_len);

// 请务必提前申请好 output_type/buffer_len 的内存
// 函数内部会申请内存存储返回值，请务必在用完后释放 (*output)
int SendSecretRequest(AuthParam auth_para, char* input, char* output_type,
                      unsigned char** output, int* buffer_len);


int GetServerTimeCode(char server_ip[32], unsigned int* timecode);

// Websocket 接收到新消息时的回调
// 绝对不能在回调函数内释放 char* 参数
typedef int (*CallbackOnNewMessage)(char* message);

// Websocket 连接被断开时的回调
typedef int (*CallbackOnClosed)();

// Websocket 连上，成功接收到服务器的 Hello 时的回调。
typedef int (*CallbackOnLogin)();

// Websocket 登录
int Login(AuthParam auth_para, CallbackOnNewMessage cb_msg,
          CallbackOnClosed cb_close, CallbackOnLogin cb_login);

// Websocket 退出
void Logout();

static const int kErrorFailToCreateThread = 2001;
static const int kErrorFailToGetTimecode = 2003;

#ifdef __cplusplus
}
#endif
#endif // API_H
