#include "uvss_api.h"

#include<iostream>
#include "dm_httpobject.h"
#include "toml/src/toml.h"
#include "sha/sha256.h"


namespace    uvss_api{


int GetUVSSCameraID(QString camera_IP,QString username,QString password
                    ,QString &res_id){
#ifdef DM_DEBUG
    //qDebug()<<"*********"<<__FUNCTION__<<"*********"<<endl;
#endif
    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/cgi-bin/UVSSapi.cgi?{\"cmd\":\"get\",\"key\":\"device id\"}";
    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){

#ifdef _DEBUG_MODE_
        QString log_info = "INFO_UVSS_CAMERA_SN_ID_FROM_HTTP: " + str_response;
        systemLog(log_info);
#endif
        //remove escape（转义字符）
        str_response = str_response.replace("\n","");
        str_response = str_response.replace("\r","");
        str_response = str_response.replace("\t","");
        str_response = str_response.replace("\\\"","\"");

        //get value of UVSSCamera ID
        QJsonDocument jsonDocument = QJsonDocument::fromJson(str_response.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {
            return CanNotGetInfo;
        }
        QJsonObject jsonObject = jsonDocument.object();
        QJsonValue tmpvalue=jsonObject["value"];
        QJsonArray tmparray=tmpvalue.toArray();
        QJsonValue tmp_value_1=tmparray.at(0);
        QJsonObject jsonObject_1=tmp_value_1.toObject();
        res_id=jsonObject_1["value"].toString();
        if(res_id!=""){
            return Normal;
        }
        else{
            return CanNotGetInfo;
        }
    }

    else{
        return CanNotConnect;
    }
}


int GetUVSSCameraEncryptedString(QString camera_IP,QString username
                       ,QString password,QString &res_enc){

#ifdef DM_DEBUG
    //qDebug()<<"*********"<<__FUNCTION__<<"*********"<<endl;
#endif
    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/cgi-bin/UVSSapi.cgi?{\"cmd\":\"get\",\"key\":\"encrypted\"}";
    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){

#ifdef _DEBUG_MODE_
        QString log_info = "INFO_UVSS_CAMERA_SN_ID_FROM_HTTP: " + str_response;
        systemLog(log_info);
#endif
        //remove escape（转义字符）
        str_response = str_response.replace("\n","");
        str_response = str_response.replace("\r","");
        str_response = str_response.replace("\t","");
        str_response = str_response.replace("\\\"","\"");

        //get value of UVSSCamera ID
        QJsonDocument jsonDocument = QJsonDocument::fromJson(str_response.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {
            return CanNotGetInfo;
        }
        QJsonObject jsonObject = jsonDocument.object();
        QJsonValue tmpvalue=jsonObject["value"];
        QJsonArray tmparray=tmpvalue.toArray();
        QJsonValue tmp_value_1=tmparray.at(0);
        QJsonObject jsonObject_1=tmp_value_1.toObject();
        res_enc=jsonObject_1["value"].toString();
        if(res_enc!=""){
            return Normal;
        }
        else{
            return CanNotGetInfo;
        }
    }

    else{
        return CanNotConnect;
    }
}



int GetUVSSCameraInfo(QString camera_IP,QString username
                      ,QString password, int &delay_times,int &product,QString &region,int &remain_number){
    int tag;
    if((tag = CheckUVSSCommunityAuthority(camera_IP, username, password)) != Normal) {
        return tag;
    }
#ifdef DM_DEBUG
    //qDebug()<<"*********"<<__FUNCTION__<<"*********"<<endl;
#endif

    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/config.toml";

    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){
#ifdef _DEBUG_MODE_
        QString log_info = "INFO_UVSS_CAMERA_INFO_FROM_HTTP: " + str_response;
        systemLog(log_info);
#endif

        QByteArray buff_byte = str_response.toUtf8();

        region = "";

        FILE* fp = fopen("toml.txt", "wb+");
        fwrite(buff_byte.data(), buff_byte.size() , 1, fp);
        fclose(fp);

        Toml *d = new Toml("toml.txt");
        d->Get("delay_times", delay_times);
        d->Get("product", product);
        string tmp_str;
        d->Get("region",tmp_str);
        region=QString::fromStdString(tmp_str);
        d->Get("remain_number", remain_number);

        delete d;

        if(region.isEmpty()){

            return CanNotGetInfo;
        }
        else{

            return Normal;
        }
    }
    else{

        return CanNotConnect;
    }
}

int GetServerIPOfUVSSCamera(QString camera_IP,QString username
                            ,QString password,QString &server_IP){
    int tag;
    if((tag = CheckUVSSCommunityAuthority(camera_IP, username, password)) != Normal) {
        return tag;
    }

    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/cgi-bin/UVSSapi.cgi?{\"cmd\":\"get\",\"key\":\"server ip\"}";
    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){
        //remove escape（转义字符）
        str_response = str_response.replace("\n","");
        str_response = str_response.replace("\r","");
        str_response = str_response.replace("\t","");
        str_response = str_response.replace("\\\"","\"");

        //get value of UVSSCamera ID
        QJsonDocument jsonDocument = QJsonDocument::fromJson(str_response.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {
            return CanNotGetInfo;
        }
        QJsonObject jsonObject = jsonDocument.object();
        QJsonValue tmpvalue=jsonObject["value"];
        QJsonArray tmparray=tmpvalue.toArray();
        QJsonValue tmp_value_1=tmparray.at(0);
        QJsonObject jsonObject_1=tmp_value_1.toObject();

        server_IP=jsonObject_1["value"].toString();

        if(server_IP!=""){
            return Normal;
        }
        else{
            return CanNotGetInfo;
        }
    }
    else{
        return CanNotConnect;
    }

}


int RequestTestImageFromUVSSCamera(QString camera_IP,QString username
                                   ,QString password){
    int tag;
    if((tag = CheckUVSSCommunityAuthority(camera_IP, username, password)) != Normal) {
        return tag;
    }
#ifdef DM_DEBUG
    //qDebug()<<"*********"<<__FUNCTION__<<"*********"<<endl;
#endif

    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/cgi-bin/UVSSapi.cgi?{\"cmd\":\"get\",\"key\":\"test image\"}";
    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){
#ifdef DM_DEBUG
        //qDebug()<<"succeed to connect and get response"<<endl;
#endif
        //remove escape（转义字符）
        str_response = str_response.replace("\n","");
        str_response = str_response.replace("\r","");
        str_response = str_response.replace("\t","");
        str_response = str_response.replace("\\\"","\"");
        //get value of UVSSCamera ID
        QJsonDocument jsonDocument = QJsonDocument::fromJson(str_response.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {

#ifdef DM_DEBUG
            //qDebug()<<"OperateFail"<<endl;
#endif

            return OperateFail;
        }
        else{
            QJsonObject object = jsonDocument.object();  // 转化为对象

            if (object.contains("key")) {  // 包含指定的 key
                QJsonValue value = object.value("key");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    if(value.toString() == "test image"){
#ifdef DM_DEBUG
                        //qDebug()<<"succeed to get TestImageFromUVSSCamera"<<endl;
#endif
                        return Normal;
                    }
                    else{
#ifdef DM_DEBUG
                        //qDebug()<<"OperateFail"<<endl;
#endif
                        return OperateFail;
                    }

                }
                else{
                    return OperateFail;
                }

            }
        }
    }
    else{
#ifdef DM_DEBUG
        //qDebug()<<"can not connect to UVSSCamera"<<endl;
#endif
        return CanNotConnect  ;
    }

    return OperateFail;

}

int CheckUVSSCommunityAuthority(const QString &camera_IP,
                                const QString &username,
                                const QString &password) {
    QString res_enc;
    QString res_sn;

    int tag;
    if((tag = GetUVSSCameraID(camera_IP, username, password, res_sn)) != Normal) {
        return tag;
    }
    if((tag = GetUVSSCameraEncryptedString(camera_IP, username, password, res_enc)) != Normal) {
        return tag;
    }
    string confirm = picosha2::hash256_hex_string(res_sn.toStdString()+"sskj9900");

    //将使用SHA256加密的前24个字符串与从设备获得的EncryptedString进行比较，若两者相同则验证通过
    if(confirm.substr(0,24) == res_enc.toStdString()){
        return Normal;
    }
    else {
        return Unauthorized;
    }
}

int ChangeServerIPOfUVSSCamera(QString camera_IP,QString username
                               ,QString password,QString  new_serverIP){
    // add UVSS camera anthority check step
    int tag;
    if((tag = CheckUVSSCommunityAuthority(camera_IP, username, password)) != Normal) {
        return tag;
    }

    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/cgi-bin/UVSSapi.cgi?{\"cmd\":\"set\",\"key\":\"server ip\",\"params\":[{\"key\":\"ip\",\"value\":\""
            +new_serverIP+"\"},{\"key\":\"port\",\"value\":\"60800\"}]}";
    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){
        //remove escape（转义字符）
        str_response = str_response.replace("\n","");
        str_response = str_response.replace("\r","");
        str_response = str_response.replace("\t","");
        str_response = str_response.replace("\\\"","\"");
        //  //qDebug()<<"str_response:"<<str_response<<endl;
        //get value of UVSSCamera ID
        QJsonDocument jsonDocument = QJsonDocument::fromJson(str_response.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {
            return OperateFail;
        }
        else{
            QJsonObject object = jsonDocument.object();  // 转化为对象
            if (object.contains("code")) {  // 包含指定的 key
                QJsonValue value = object.value("code");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    if(value.toString() == "ok"){
                        return Normal;
                    }
                    else{
                        return OperateFail;
                    }

                }
                else{
                    return OperateFail;
                }

            }
            else{
                return OperateFail;
            }
        }

    }
    else{
        return CanNotConnect  ;
    }

}



int RestartUVSSCamera(QString camera_IP,QString username
                      ,QString password){
    int tag;
    if((tag = CheckUVSSCommunityAuthority(camera_IP, username, password)) != Normal) {
        return tag;
    }
#ifdef DM_DEBUG
    //qDebug()<<"*********"<<__FUNCTION__<<"*********"<<endl;
#endif

    QString str_response;
    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+
            "/cgi-bin/UVSSapi.cgi?{\"cmd\":\"set\",\"key\":\"reboot\"}";

    if(!dm_HTTPObject::ConnectToHTTP(complete_http,str_response)){
#ifdef DM_DEBUG
        //qDebug()<<"succeed to make UVSSCamera restart"<<endl;
#endif
        return Normal;
    }
    else{
#ifdef DM_DEBUG
        //qDebug()<<"succeed to make UVSSCamera restart"<<endl;
#endif
        return Normal;
    }

}

static dm_HTTPObject *_uvss_listen_port = nullptr;


int BuildPortToWaitInfoFromUVSSCamera(int port,int (*p_fun)(QString&)){
    _uvss_listen_port = new dm_HTTPObject();
    //传入端口和对端请求消息处理回调
    if(!_uvss_listen_port->ListenToHttpSignalFromPort(port,(*p_fun))){
        return Normal;
    }
    else{
        QString err_info = "ERROR_UVSS_CAMERA_LISTEN: open listen port of uvss camera fail.";
        _uvss_listen_port->deleteLater();
        _uvss_listen_port = nullptr;
        return OperateFail;
    }
}

void StopListenUVSSCamera(){
    if(_uvss_listen_port != nullptr) {
        _uvss_listen_port->StopListen();
        delete _uvss_listen_port;
        _uvss_listen_port = nullptr;
    }
}

int GetImageFromUVSSCamera(QString camera_IP,QString username
                           ,QString password,QString image_name,QString image_path){
    int tag;
    if((tag = CheckUVSSCommunityAuthority(camera_IP, username, password)) != Normal) {
        return tag;
    }
#ifdef DM_DEBUG
    //qDebug()<<"*********"<<__FUNCTION__<<"*********"<<endl;
#endif

    QString complete_http;
    complete_http="http://"+username+":"+password+"@"+camera_IP+"/"+
            image_name;
    if(!dm_HTTPObject::HttpApiToDownloadImage(complete_http
                                              ,image_path)){

#ifdef DM_DEBUG
        //qDebug()<<"succeed to GetImageFromUVSSCamera"<<endl;
#endif
        return Normal;
    }
    else{
#ifdef DM_DEBUG
        //qDebug()<<"OperateFail"<<endl;
#endif
        return OperateFail;
    }
}





}
