#ifndef CLIENT_DEVICE_INTERFACE_H
#define CLIENT_DEVICE_INTERFACE_H

#include<iostream>
#include<QString>
#include<QString>
#include<QJsonObject>
#include<QStringList>
#include<QJsonDocument>
#include<QJsonArray>
#include<QTcpServer>
#include<QRect>
namespace cm {



enum ClientManagerErrorType{
  Normal=2000,
  CanNotConnect=2001,
  CanNotGetInfo=2002,
  OperateFail=2003,
  QuitConnect=2004
};



/**
*  build the port to receive the signal to set up websocket,and keep connection.
*
*  @param   port                 the port to receive the signal of HTTP Signal, like "60801".
*  @param   p_fun               call back func to deal wth request from http.
*  @return                           return Normal if successful
*
*  @example
*  int operate_flag=  cm::BuildPortWaitForHTTPSignal(cm::callback_DealWithRequestFromHTTP,60801);

*/
int BuildPortWaitForHTTPSignal(int (*p_fun)(QString ,QByteArray&),int port);



/**
*  build the port to set up websocket,and keep connection.
*
*  @param   port                the port to receive the signal of build websocket,like "60801".
*  @param   p_fun             call back func to deal wth request from websocket.
*  @return                           return Normal if successful
*
*  @example
*  int operate_flag=  cm::BuildWebsocketServer(cm::callback_DealWithRequestFromWebsocket,cm::server_ProveAuthen,60802);
*/
int BuildWebsocketServer(int (*p_fun)(QString &,QString),int (*p_ProveAuthen)(QString ,QString),int port=60801);



/**
*  break the websocket connection with certain Client.
*
*  @param   username        the wesocket with its Client will be broken.
*  @return                           return Normal if successful
*
*  @example
*  int operate_flag=dm::BreakWebsocketWithClient("sxy);
*/
int BreakWebsocketWithClient(QString username);



/**
*  exam the websocket connection with certain Client.
*
*  @param   username        the wesocket with its Client will be broken.
*  @return                            return Normal if connection is availiable
*
*  @example
*  int operate_flag=dm::ExamWebsocketWithClient("sxy");
*/
int ExamWebsocketWithClient(QString username);



/**
*  Ask certain Client to restart the Client GUI and update the settings in Server.
*
*  @param   username      the wesocket with its Client will be broken.
*  @return                           return Normal if successful
*
*  @example
*  int operate_flag=dm::RestartTheClientAndUpdataSetting("sxy");
*/
int RestartTheClientAndUpdataSetting(QString username);


/**
*  Ask certain Client to reconnect with Server.
*
*  @param   username        the wesocket with its Client will be broken.
*  @return                           return Normal if successful
*
*  @example
*  int operate_flag=dm::MakeTheClientRelogin("sxy");
*/
int MakeTheClientRelogin(QString username);

/**
*  submit data to Client by websocket.
*  @param   data                 the data submited to Client.
*  @param   username        the wesocket with its Client will be broken.
*
*  @example
*  int operate_flag=dm::SubmitDataToClientByWebsoketByWebsoket("sxy","{"image":"hgh.jpg"}");
*/
int SubmitDataToClientByWebsoket(QString username,QString json_data);

/**
*  submit binary data to Client by websocket.
*  @param   user_name     the user name of client to login in Server.
*  @param   data        files like research result .csv.
*
*  @example
*  int operate_flag=dm::SendBinaryDataToClient("sxy",bi_data);
*/
int SendBinaryDataToClient(QString user_name,QByteArray data);



/**
*  callback function to deal with the request from Client HTTP.
*  @param   data                 Resquest string from clent.
*  @param  res_data          save result to it.
*  @example
*  int operate_flag=BuildPortWaitForHTTPSignal(cm::callback_DealWithRequestFromHTTP,60801);
*/
int callback_DealWithRequestFromHTTP(QString data,QByteArray &res_data);

/**
*  callback function to deal with the request from Client Websocket
*  @param   data                 Resquest string from clent.
*  @param   username          received from which user.
*
*  @example
*  int operate_flag=  cm::BuildWebsocketServer(cm::callback_DealWithRequestFromWebsocket,guisrv::gs_ProveAuthen,60802);
*/
int callback_DealWithRequestFromWebsocket(QString &data,QString username);

}

#endif
