#include<iostream>
#include<QString>
#include<QString>
#include<QJsonObject>
#include<QStringList>
#include<QJsonDocument>
#include<QJsonArray>
#include<QTcpServer>
#include<QRect>
#include<QHash>
#include"client_manager_top.hpp"
#include"client_manager_interface.hpp"
//#include "database_processor_interface.hpp"
//#include "gui_server_interface.hpp"
namespace cm {



int BuildPortWaitForHTTPSignal(int (*p_fun)(QString ,QByteArray&),int port){
  dm_HTTPObject* tmpobj=new dm_HTTPObject();
  tmpobj->ListenToHttpSignalFromPort(port,p_fun);
  return Normal;
}



int BreakWebsocketWithClient(QString username){
  server->ws_BreakOffWebsocketWithClient(username);
  return Normal;
}



int ExamWebsocketWithClient(QString username){
  int flag=server->ws_ExamWebsocketWithClient(username);
  if(flag){
    return CanNotConnect;
  }
  else{
    return Normal;

  }
}




int BuildWebsocketServer(int (*p_fun)(QString &,QString),int (*p_ProveAuthen)(QString ,QString),int port){
  server = new EchoServer(p_fun,p_ProveAuthen,port, 1);
  return Normal;
}



int SubmitDataToClientByWebsoket(QString username,QString json_data){
  int flag=server->ws_SubmitDataToClientByWebsoket(username,json_data);
  if(flag){
    return OperateFail;
  }
  else{
    return Normal;

  }
}



int SendBinaryDataToClientByWebsoket(QString user_name,QByteArray data){
  int flag=server->ws_SendBinaryDataToClient(user_name,data);
  if(flag){
    return OperateFail;
  }
  else{
    return Normal;
  }
}



int callback_DealWithRequestFromHTTP(QString data,QByteArray &res_data){
  //qDebug()<<"data from http:"<<data<<endl;
  //分析数据
  QString username="sxy";
  QString password="12345";
  int         request_type=1;
  QString str_response = data;
  str_response = str_response.replace("\n","");
  str_response = str_response.replace("\r","");
  str_response = str_response.replace("\t","");
  str_response = str_response.replace("\\\"","\"");

  //get value of UVSSCamera ID
  QJsonDocument jsonDocument = QJsonDocument::fromJson(str_response.toLocal8Bit().data());
  if(!jsonDocument.isNull()){
    //qDebug()<<"data:"<<data<<endl;

    QJsonObject jsonObject = jsonDocument.object();
    if (jsonObject.contains("request_type")) {  // 包含指定的 key
      QJsonValue tmpvalue=jsonObject["request_type"];
      if(tmpvalue.toInt()<1000){

//        guisrv::gs_DealWithRequestForHTTP(data,res_data);
        //假数据
        //res_data="{\"request_type\":1,\"response\":\"ok\",\"wsurl\":\"ws://192.168.1.100:60802\"}";
      }
      else{
        //qDebug()<<"request type>1000........"<<endl;
//        dbp::dbp_DealWithRequestForHTTP(data,res_data);

       /*
        QFile file;

          switch (jsonDocument["request_type"].toInt()) {
          case 1010:
            file.setFileName("D:/svn/UVSScure/Client/build/jpeg_2_128.jpg");
            break;
          case 1009:
            file.setFileName("D:/svn/UVSScure/Client/build/Images/logo.png");
            break;

          default:
            break;
          }
        int flag=file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        QByteArray total_block;
        int buffer_size=65535;
        char datRawArray[buffer_size];
        while(!in.atEnd()){
          in.readRawData(datRawArray, buffer_size);
          QByteArray block(datRawArray,buffer_size);
          total_block+=block;
  //        //qDebug()<<"block_size:"<<block.size()<<endl;
        }
        res_data=total_block;
        */
      }


    }
    else{
      return OperateFail;
    }


    return Normal;
  }
  else{
    return 1;
  }

}



int callback_DealWithRequestFromWebsocket(QString &data,QString username){
  //qDebug()<<"data from websocket:"<<data<<endl;
  //qDebug()<<"username:"<<username<<endl;

  //分析数据
  std::string str = data.toStdString();
  QByteArray buff = QByteArray::fromStdString(str);
  QJsonDocument doc = QJsonDocument::fromJson(buff);
  if(!doc.isObject()) {
      return 1;
  }

  QJsonObject obj = doc.object();
  int request_type= obj["request_type"].toInt();
  if(request_type<1000){
//    guisrv::gs_DealWithRequest(data,username);
  //假操作
    SubmitDataToClientByWebsoket(username,"meassage from server");
  }
  else{
//    dbp::dbp_DealWithRequest(data,username);
  }
  return Normal;
}



}

