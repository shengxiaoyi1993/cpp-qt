#include "apiserver.h"

APIServer::APIServer(QObject *parent):QObject(parent)
{
  _com=new ComServer();
  connect(_com,SIGNAL(sgl_OnNewHttpRequest(long long int , char *, char *,void *)),
          this,SLOT(slot_OnNewHttpRequest(long long int , char *, char *,void *)));
  connect(_com,SIGNAL(sgl_OnHttpError(int , void*)),
          this,SLOT(slot_OnHttpError(int , void*)));
  connect(_com,SIGNAL(sgl_OnUserLogin(char *, void *)),
          this,SLOT(slot_OnUserLogin(char *, void *)));
  connect(_com,SIGNAL(sgl_OnUserLogout(char *, void *)),
          this,SLOT(slot_OnUserLogout(char *, void *)));
  connect(_com,SIGNAL(sgl_OnWebsocketError(int , void *)),
          this,SLOT(slot_OnWebsocketError(int , void *)));

}


/// {
///   "request_type":1,
///   "data":{
///     ...
///   }
/// }
void APIServer::slot_OnNewHttpRequest(long long int fd, char *username, char *req_data,void *ptr){
  (void)ptr;

  neb::CJsonObject root(req_data);
  if(root.IsEmpty()){
    throw text_exception(__func__);
  }

  switch (ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::CtlInfo>(root)._req_type) {
    case ns_tcs_ds_def::ENUM_REQ_SINGLEUSER:
    {
      emit sgl_qrySingleUser(fd,username,root["data"]("_user"));
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_ALLUSER:
    {
      emit sgl_qryAllUser(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_RUNTME:
    {
      emit sgl_qryRunTime(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_USERSTAUS:
    {
      emit sgl_qryUserStatus(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DEVARRAY:
    {
      emit sgl_qryDevArray(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DEVSTATUS:
    {
      emit sgl_qryDevStatus(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_ADDDEV:
    {
      emit sgl_qryNVR(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_ADDUSER:
    {
      emit sgl_addDev(fd,username,root["data"]);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DELDEV:
    {
      emit sgl_delDev(fd,username,root["data"]("_dev"));
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DELUSER:
    {
      emit sgl_delUser(fd,username,root["data"]("_user"));
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_SETDEV:
    {
      emit sgl_setDev(fd,username,root["data"]);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_SETUSER:
    {
      emit sgl_setUser(fd,username,root["data"]);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_STOPSERVICE:
    {
      emit sgl_stopService(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DIRECTCAM_OP:
    {
      emit sgl_directCamOp(fd,username,root["data"]);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_QRYNVR:
    {
      emit sgl_qryNVR(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_EMPTYNVR:
    {
      emit sgl_emptyNVR(fd,username);
      break;
    }
    case ns_tcs_ds_def::ENUM_REQ_SETNVR:
    {
      emit sgl_setDev(fd,username,root["data"]);
      break;
    }
    default:
      throw text_exception("request_type not find!");
  }
}


void APIServer::slot_OnHttpError(int error_code, void*ptr){
  (void)ptr;
  emit sgl_OnHttpError(error_code);
}

void APIServer::slot_OnUserLogin(char *username, void *ptr){
  (void)ptr;

  emit sgl_OnUserLogin(username);
}

void APIServer::slot_OnUserLogout(char *username, void *ptr){
  (void)ptr;
  emit sgl_OnUserLogout(username);

}

void APIServer::slot_OnWebsocketError(int error_code, void *ptr){
  (void)ptr;
  emit sgl_OnWebsocketError(error_code);

}
