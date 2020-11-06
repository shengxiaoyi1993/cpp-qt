#ifndef COMSERVER_H
#define COMSERVER_H

#include <QObject>
#include <string>
#include <QMap>
#include <QSet>
#include <map>


using namespace std;


/// 该类内部由http与ws实现，用于与client进行交互
///


typedef struct  UserItem{
  UserItem(const string& v_username,const string &v_hash):
    _username(v_username),_hash(v_hash)
  {}
  string _username;  ///max:32
  string _hash;      ///max:65
}UserItem;

class ComServer : public QObject
{
  Q_OBJECT
public:
  explicit ComServer(QObject *parent = nullptr);
  ///
  /// \brief SetAllUserMap 建立可连接用户名列表；用户变更后需要调用用该函数进行更新
  /// \param v_users
  /// \return
  ///
  int setAllUserMap(const vector<UserItem>& v_users);


  ///================================HTTP===============================///

  int setupListenHttp();

  int sendHttpResponse(long long int fd, char *data_type, unsigned char *data,
                       int data_length);

  int closeHttpConnection(long long int fd);

  int stopListenHttp();




  ///================================WS===============================///
  int setupListenWebsocket();

  ///
  /// \brief SendWebsocketMessage 向ws通道中发送消息；用户在_users_login且标识为true
  /// \param user
  /// \param data
  /// \return
  ///
  int sendWebsocketMessage(char *user, char *data);

  int sendWebsocketHeartBeat();

  int stopListenWebsocket();

  int closeWebsocketConnection(char *user);


  bool isListenHTTP() const{return _is_listen_http; }
  bool isListenWS() const{return _is_listen_ws;}
  map<UserItem,bool> getUserStatus()const{return _users_login;}
  vector<pair<long long int,unsigned>> getFds()const{return _fds_tm;}


  static int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                         void *ptr);
  static int cb_CallbackOnHttpError(int error_code, void*ptr);

  // 当用户登录websocket时的回调
  // username 用户名
  // ptr 在 Setup 时设置好的指针，c++类静态成员函数做为回调函数时可能需要用到
  static int cb_CallbackOnUserLogin(char *username, void *ptr);

  // 当用户退出websocket时的回调
  static int cb_CallbackOnUserLogout(char *username, void *ptr);

  // 当 Websocket 监听发生错误时的回调
  static int cb_CallbackOnWebsocketError(int error_code, void *ptr);

signals:

  void sgl_OnNewHttpRequest(long long int fd, char *username, char *req_data,void *ptr);
  void sgl_OnHttpError(int error_code, void*ptr);
  void sgl_OnUserLogin(char *username, void *ptr);
  void sgl_OnUserLogout(char *username, void *ptr);
  void sgl_OnWebsocketError(int error_code, void *ptr);


public slots:

private:
  /// 建立ws连接的用户
  map<UserItem,bool> _users_login;


  /// 是否在监听http端口
  bool _is_listen_http;

  /// 是否在监听ws
  bool _is_listen_ws;

  /// 保存正在处理的句柄和建立的时间
  /// 对于客户端断开的连接或者异常断开的连接，无法正常删除句柄
  vector<pair<long long int,unsigned>> _fds_tm;

};

#endif // COMSERVER_H
