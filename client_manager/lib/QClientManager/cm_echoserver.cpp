
/****************************************************************************
**
** Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "cm_echoserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

EchoServer::EchoServer(int (*p_fun)(QString &,QString),int (*p_ProveAuthen)(QString ,QString),quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_debug(debug)
{
  upload_fun=p_fun;
  ProveAuthen_fun=p_ProveAuthen;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            //qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &EchoServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);
    }
}

EchoServer::~EchoServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void EchoServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &EchoServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &EchoServer::socketDisconnected);
   QString response= pSocket->requestUrl().toString();
   QUrl url(response);
   QUrlQuery query(url.query());
//   if(query.hasQueryItem("username")&&query.hasQueryItem("password")){
//    //qDebug()<<"username:"<< query.queryItemValue("username")<<endl;
//    //qDebug()<<"password:"<< query.queryItemValue("password")<<endl;
//   }
   QString username= query.queryItemValue("username");
   QString password = query.queryItemValue("password");
   if(ProveAuthen_fun(username,password)){
     //qDebug()<<"prove auth failed"<<username<<" "<<password<<endl;
     m_clients.removeAll(pSocket);
     pSocket->deleteLater();
     return ;
   }
   //qDebug()<<"prove auth succeed"<<endl;

   if(UserClientMatchList.contains(username)){
     pSocket->sendTextMessage("{\"request_type\":2,\"response\":\"error\"}");
     UserClientMatchList.remove(username);
     m_clients.removeAll(pSocket);
     pSocket->deleteLater();
   }
   else{
     UserClientMatchList.insert(username,pSocket);
     m_clients<<pSocket;
    #ifdef CMWS_DEBUG
      //qDebug()<<endl<<"***********websocket connected*********"<<endl;
    #endif
     pSocket->sendTextMessage("{\"request_type\":2,\"response\":\"ok\"}");
   }
}



void EchoServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QString username;
    if (m_debug){
        //qDebug() << "Message received:" << message;
    }

    QHash<QString, QWebSocket*>::iterator i;
    for( i=UserClientMatchList.begin(); i!=UserClientMatchList.end(); ++i){
      if(i.value()==pClient){
        username=i.key();
      }
    }

    upload_fun(message,username);
    //qDebug() << "******upload_fun finished*******" ;

//    if (pClient) {
//        pClient->sendTextMessage(message);
//    }

}



void EchoServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        //qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void EchoServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        //qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
      QHash<QString, QWebSocket*>::iterator i;
      for( i=UserClientMatchList.begin(); i!=UserClientMatchList.end(); ++i){
        if(i.value()==pClient){
          UserClientMatchList.remove(i.key());
        }
      }
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}


int EchoServer::ws_BreakOffWebsocketWithClient(QString username){
  if(!UserClientMatchList.contains(username)){
  }
  else{
    QWebSocket *pClient=UserClientMatchList[username];
    if (pClient) {
      UserClientMatchList.remove(username);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
  }
  return 0;
}




int EchoServer::ws_SubmitDataToClientByWebsoket(QString username,QString json_data){
  if(UserClientMatchList.contains(username)){
    QWebSocket *pClient=UserClientMatchList[username];
    if (pClient) {
      pClient->sendTextMessage(json_data);
      return 0;
    }
  }
  return 1;
}



int EchoServer::ws_SendBinaryDataToClient(QString username,QByteArray data){
  if(UserClientMatchList.contains(username)){
    QWebSocket *pClient=UserClientMatchList[username];
    if (pClient) {
      pClient->sendBinaryMessage(data);
      return 0;
    }
  }
  return 1;
}


int EchoServer::ws_ExamWebsocketWithClient(QString username){
    if(UserClientMatchList.contains(username)){
      QWebSocket *pClient=UserClientMatchList[username];
      if (pClient) {
        return 0;
      }
    }
    return 1;
}
