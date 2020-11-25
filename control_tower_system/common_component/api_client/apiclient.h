#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <iostream>
#include <string>
#include <QMetaType>
#include "com_client_export.h"
#include "ds_def/tcs_ds_def.h"


using namespace std;

/// 需要增加的接口
/// 1.获取service的串口列表：此接口用于配置直显相机
/// 2.检测某一设备的可连接性
/// 3.请求NVR的连接状态



/// 设计成与某单一服务器建立连接并进行WS或者http信息交互的工具
/// 状态
/// 1.启动 可以发送http消息和请求建立ws连接
/// 2.建立WS连接 可以接收http消息和监听WS的状态
class APIClient : public QObject
{
  Q_OBJECT
public:
  explicit APIClient(QObject *parent = nullptr);
  int setAuthPara(const AuthParam &v_authpara);
  ~APIClient();
  int login();
  AuthParam getAuthPara()const;
  bool getStatus()const;
  int logout();

  ///基础的接口
  // 请务必提前申请好 output_type/buffer_len 的内存
  // 函数内部会申请内存存储返回值，请务必在用完后释放 (*output)
  int sendHTTPRequest(const string& v_input,string &output_type,
                      unsigned char **output, int &buffer_len);

  // 请务必提前申请好 output_type/buffer_len 的内存
  // 函数内部会申请内存存储返回值，请务必在用完后释放 (*output)
  int sendSecretRequest(const string& v_input,string &output_type,
                        unsigned char** output, int &buffer_len);
  int getServerTimeCode(unsigned int& timecode);

  ///具体的API
  /// 需要对返回值进行判断，是否等于ENUM_HTTP_ERRCODE_SUCCEED，成功后才能读取返回的数据结构，否则可能是个空结构
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_querySINGLEUSER(const string &v_username, ns_tcs_ds_def::User& v_user);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryALLUSER( vector<ns_tcs_ds_def::User>& v_users);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryRUNTME(ns_tcs_ds_def::TimeRecord& v_info);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryUSERSTAUS(vector<ns_tcs_ds_def::UserStatus>& v_list);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryDEVARRAY(ns_tcs_ds_def::CameraArray& v_array);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryDEVSTATUS(vector<ns_tcs_ds_def::CamStatus>& v_list);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryQRYNVR(ns_tcs_ds_def::NvrDev &v_nvr);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_querySerialPort(vector<string> &l_port);

  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryADDDEV(const ns_tcs_ds_def::CamDev& v_data);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_queryADDUSER(const ns_tcs_ds_def::User& v_data);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_DELDEV(const string& v_cmaname);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_DELUSER(const string &v_username);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_EMPTYNVR();
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_SETDEV(const ns_tcs_ds_def::CamDev &v_data);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_SETUSER(const ns_tcs_ds_def::User &v_data);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_SETNVR(const ns_tcs_ds_def::NvrDev v_data);
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_STOPSERVICE();
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE req_DIRECTCAM_OP(const ns_tcs_ds_def::DirectCamOp v_data);

signals:
  void sgl_WS_OnNewMessage(string);
  void sgl_WS_OnClosed();
  void sgl_WS_OnLogin();

  /// 解析ws发过来的消息
  void sgl_WS_RESTART();
  void sgl_WS_PUSHLOG(string log);
  void sgl_WS_USERSTAUS(vector<ns_tcs_ds_def::UserStatus> v_list );
  void sgl_WS_DEVSTATUS(vector<ns_tcs_ds_def::CamStatus> v_list);



public slots:

private:
  AuthParam _authpara;
  bool      _status;

protected:
  friend int cb_CallbackOnNewMessage(char* message);

  // Websocket 连接被断开时的回调
  friend int cb_CallbackOnClosed();

  // Websocket 连上，成功接收到服务器的 Hello 时的回调。
  friend int cb_CallbackOnLogin();
};

int cb_CallbackOnNewMessage(char* message);

// Websocket 连接被断开时的回调
int cb_CallbackOnClosed();

// Websocket 连上，成功接收到服务器的 Hello 时的回调。
int cb_CallbackOnLogin();

int copyAuthParam(AuthParam& v_para_dst,const AuthParam& v_para);
Q_DECLARE_METATYPE(string);

#endif // APICLIENT_H
