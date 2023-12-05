#include "comserver.h"
#include "com_server_export.h"
#include <cstring>
#include <iostream>
#include <ctime>


ComServer::ComServer(QObject *parent) :
  QObject(parent),_is_listen_http(false),_is_listen_ws(false)
{

}

UserMap fromUserItem(const UserItem& v_item){
  UserMap rmap;
  strcpy(rmap.username,v_item._username.c_str());
  strcpy(rmap.hash,v_item._hash.c_str());
  return rmap;
}


int ComServer::setAllUserMap(const vector<UserItem>& v_users){
//  cout<<__func__<<v_users.size()<<endl;
  UserMap *maps=new UserMap[v_users.size()];

  for (uint i=0;i<v_users.size();i++) {

    auto pos_find=find_if(_users_login.begin(),_users_login.end(),[=](pair<UserItem,bool> v_it){
      return v_it.first._username==v_users[i]._username;
    });
    if(pos_find==_users_login.end()){
      _users_login.insert(pair<UserItem,bool>(v_users[i],false));
    }
    maps[i]=fromUserItem(v_users[i]);
  }
  int flag= SetAllUserMap(maps,v_users.size());
  delete []maps;
  return flag;
}


///================================HTTP===============================///

int ComServer::setupListenHttp(){
   int flag=SetupListenHttp(cb_CallbackOnNewHttpRequest,cb_CallbackOnHttpError,this);
  if(flag==0){
    _is_listen_http=true;
  }

  return flag;
}


int ComServer::sendHttpResponse(long long int fd, char *data_type, unsigned char *data,
                                int data_length){
  return SendHttpResponse(fd,data_type,data,data_length);

}

int ComServer::closeHttpConnection(long long int fd){

  int flag=CloseHttpConnection(fd);
  if(flag == 0){
    auto it_pos= remove_if(_fds_tm.begin(),_fds_tm.end(),[=](pair<long long int,unsigned> fdt){return (fdt.first==fd);});
    if(it_pos != _fds_tm.end()){
      _fds_tm.erase(it_pos);
    }
  }
  return flag;

}

int ComServer::stopListenHttp(){

  int flag=StopListenHttp();
  if(flag == 0){
    _is_listen_http=false;
  }
  return flag;

}




///================================WS===============================///
int ComServer::setupListenWebsocket(){
  _is_listen_ws= SetupListenWebsocket(cb_CallbackOnUserLogin,cb_CallbackOnUserLogout,
                                      cb_CallbackOnWebsocketError,this);
  return _is_listen_ws;

}

int ComServer::sendWebsocketMessage(char *user, char *data){
  return SendWebsocketMessage(user,data);
}

int ComServer::sendWebsocketHeartBeat(){
  return SendWebsocketHeartBeat();
}

int ComServer::closeWebsocketConnection(char *user){
  auto it=find_if(_users_login.begin(),_users_login.end(),
                  [=](pair<UserItem,bool> v_user){
    return (v_user.first._username == string(user));
  });
  if (it!=_users_login.end()){
    (*it).second=false;
  }

  return CloseWebsocketConnection(user);
}


int ComServer::stopListenWebsocket(){
  int flag=StopListenWebsocket();
  _is_listen_ws=(flag==0)?false:_is_listen_ws;
  return flag;
}



int ComServer::cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                           void *ptr){
//  cout<<endl<<__func__<<endl;
//  cout.flush();
  ComServer* tmpserver=static_cast<ComServer*>(ptr);
  if(  tmpserver->_fds_tm.end() ==
       find_if(tmpserver->_fds_tm.begin(),tmpserver->_fds_tm.end(),
               [=](pair<long long int,unsigned> fdt){return (fdt.first==fd);})){
    time_t tm_cur;
    time(&tm_cur);
    tmpserver->_fds_tm.push_back(pair<long long int,unsigned>(fd,tm_cur));
  }

  emit tmpserver->sgl_OnNewHttpRequest(fd,username,req_data,ptr);
  return 0;

}

int ComServer::cb_CallbackOnHttpError(int error_code, void*ptr){
  ComServer* tmpserver=static_cast<ComServer*>(ptr);
  emit tmpserver->sgl_OnHttpError(error_code,ptr);
  return 0;
}

// 当用户登录websocket时的回调
// username 用户名
// ptr 在 Setup 时设置好的指针，c++类静态成员函数做为回调函数时可能需要用到
int ComServer::cb_CallbackOnUserLogin(char *username, void *ptr){
//  cout<<__func__<<":"<<username<<endl;

  ComServer* tmpserver=static_cast<ComServer*>(ptr);

  auto it=find_if(tmpserver->_users_login.begin(),tmpserver->_users_login.end(),
                  [=](pair<UserItem,bool> v_user){
    return (v_user.first._username == string(username));
  });
  if (it!=tmpserver->_users_login.end()){
    (*it).second=true;
  }

  emit tmpserver->sgl_OnUserLogin(username,ptr);
  return 0;
}


// 当用户退出websocket时的回调
int ComServer::cb_CallbackOnUserLogout(char *username, void *ptr){
//  cout<<__func__<<":"<<username<<endl;


  ComServer* tmpserver=static_cast<ComServer*>(ptr);

  auto it=find_if(tmpserver->_users_login.begin(),tmpserver->_users_login.end(),
                  [=](pair<UserItem,bool> v_user){
    return (v_user.first._username == string(username));
  });
  if (it!=tmpserver->_users_login.end()){
    (*it).second=false;
  }

  emit tmpserver->sgl_OnUserLogout(username,ptr);
  return 0;
}

// 当 Websocket 监听发生错误时的回调
int ComServer::cb_CallbackOnWebsocketError(int error_code, void *ptr){
  ComServer* tmpserver=static_cast<ComServer*>(ptr);
  emit tmpserver->sgl_OnWebsocketError(error_code,ptr);
  return 0;
}
