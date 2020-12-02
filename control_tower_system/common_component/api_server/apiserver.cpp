#include "apiserver.h"

APIServer::APIServer(QObject *parent):QObject(parent)
{
    qRegisterMetaType<string>("string");
    qRegisterMetaType<vector<ns_tcs_ds_def::UserStatus>>("vector<ns_tcs_ds_def::UserStatus>");
    qRegisterMetaType<ns_tcs_ds_def::CtlInfo>("ns_tcs_ds_def::CtlInfo");
    qRegisterMetaType<ns_tcs_ds_def::DirectCamOp>("ns_tcs_ds_def::DirectCamOp");
//    qRegisterMetaType<ns_tcs_ds_def::CtlInfo>("ns_tcs_ds_def::CtlInfo");
//    qRegisterMetaType<ns_tcs_ds_def::CtlInfo>("ns_tcs_ds_def::CtlInfo");

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
void APIServer::slot_OnNewHttpRequest(long long int fd,
                                      char *username,
                                      char *req_data,
                                      void *ptr){
    (void)ptr;
    cout<<endl<<__func__<<req_data<<endl;
    cout.flush();
    fflush(stdout);
    neb::CJsonObject root(req_data);
    if(root.IsEmpty()){
        throw text_exception(__func__);
    }
    cout<<"_req_type:"<<ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::CtlInfo>(root)._req_type<<endl;

    switch (ns_tcs_ds_def::dataStructFromJsonObj<ns_tcs_ds_def::CtlInfo>(root)._req_type) {
    case ns_tcs_ds_def::ENUM_REQ_SINGLEUSER:
    {
        emit sgl_qrySingleUser(fd,username,root["data"]("_str"));
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
        cout<<" sgl_addDev"<<endl;
        emit sgl_addDev(fd,username,root["data"]);
        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_ADDUSER:
    {
        emit sgl_addUser(fd,username,root["data"]);

        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DELDEV:
    {
        emit sgl_delDev(fd,username,root["data"]("_str"));
        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_DELUSER:
    {
        emit sgl_delUser(fd,username,root["data"]("_str"));
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
        cout<<"recv:"<<root["data"].ToFormattedString();
        ns_tcs_ds_def::DirectCamOp data_tmp(root["data"]);
        cout<<"data_tmp:"<<data_tmp.jsonobj().ToFormattedString()<<endl;
        emit sgl_directCamOp(fd,username,data_tmp);
        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_QRYNVR:
    {
        emit sgl_qryNVR(fd,username);
        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_QRYSERIALPORT:
    {
        emit sgl_qrySerialPort(fd,username);
        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_EMPTYNVR:
    {
        emit sgl_emptyNVR(fd,username);
        break;
    }
    case ns_tcs_ds_def::ENUM_REQ_SETNVR:
    {
        emit sgl_setNVR(fd,username,root["data"]);
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
    cout<<__func__<<endl;
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


int APIServer::sendWSMsg_RESTART(const string& v_user){
    return sendWebsocketMessage(v_user,ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_WS_RESTART)
                                .jsonobjWithData(ns_tcs_ds_def::SingleString("restart").jsonobj())
                                .ToString());
}


int APIServer::sendWSMsg_PUSHLOG(const string& v_user,const string& v_log){
    return sendWebsocketMessage(v_user,ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_WS_PUSHLOG)
                                .jsonobjWithData(ns_tcs_ds_def::SingleString(
                                                     QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ").toStdString()+v_log).jsonobj())
                                .ToString());
}

int APIServer::sendWSMsg_USERSTAUS(const string& v_user,const vector<ns_tcs_ds_def::UserStatus>& v_list){
    return sendWebsocketMessage(v_user,ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_WS_USERSTAUS)
                                .jsonobjWithData(ns_tcs_ds_def::jsonFromArray(v_list))
                                .ToString());
}

int APIServer::sendWSMsg_DEVSTATUS(const string& v_user,const vector<ns_tcs_ds_def::CamStatus>& v_list){
    return sendWebsocketMessage(v_user,ns_tcs_ds_def::CtlInfo(ns_tcs_ds_def::ENUM_WS_USERSTAUS)
                                .jsonobjWithData(ns_tcs_ds_def::jsonFromArray(v_list))
                                .ToString());
}


int APIServer::sendHTTPMsg_REQ_SINGLEUSER(long long int v_fd,const ns_tcs_ds_def::User& v_user ){
    cout<<"v_user:"<<v_user.jsonobj().ToString()<<endl;
    cout.flush();
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(v_user.jsonobj()).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_ALLUSER(long long int v_fd,const vector<ns_tcs_ds_def::User>& v_users){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::User>(v_users)).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_RUNTME(long long int v_fd,const ns_tcs_ds_def::TimeRecord& v_info){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(v_info.jsonobj()).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_USERSTAUS(long long int v_fd,const vector<ns_tcs_ds_def::UserStatus>& v_list){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::UserStatus>(v_list)).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_DEVARRAY(long long int v_fd,const ns_tcs_ds_def::CameraArray& v_array){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(v_array.jsonobj()).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_DEVSTATUS(long long int v_fd,const vector<ns_tcs_ds_def::CamStatus>& v_list){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::CamStatus>(v_list)).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_QRYNVR(long long int v_fd,const ns_tcs_ds_def::NvrDev &v_nvr){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(v_nvr.jsonobj()).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}

int APIServer::sendHTTPMsg_REQ_QRYSERIALPORT(long long int v_fd,const vector<string> &l_port){
    string str_res=ns_tcs_ds_def::ReturnInfo(ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED)
            .jsonobjWithData(ns_tcs_ds_def::jsonFromArray<string>(l_port)).ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}


int APIServer::sendHTTPMsg_RETURNCODE(long long int v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE v_code){
    string str_res=ns_tcs_ds_def::ReturnInfo(v_code)
            .jsonobj().ToString() ;
    int flag_send= sendHttpResponse(v_fd,"text",reinterpret_cast<unsigned char*>(const_cast<char*>(str_res.c_str())),
                                    str_res.length());
    int flag_close=closeHttpConnection(v_fd);
    if(!((flag_send==0)&&(flag_close==0))){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Error!");
    }
    return flag_send;
}





