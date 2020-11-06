#include "dm_httpobject.h"
#include <QUrl>
#include <QUrlQuery>
#include "global_tool_func.h"
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"

using namespace std;
dm_HTTPObject::dm_HTTPObject() {
    tcpserver = nullptr;
}

dm_HTTPObject::~dm_HTTPObject() {
    if(tcpserver != nullptr) {
        tcpserver->close();
        delete tcpserver;
        tcpserver = nullptr;
    }
}

void dm_HTTPObject::ReplyFinished(QNetworkReply *reply) {
  //获取响应的信息，状态码为200表示正常
  QVariant status_code =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  //无错误返回
  if (reply->error() == QNetworkReply::NoError) {
    QByteArray bytes = reply->readAll();
    reply->seek(0);
    QString result(bytes);  //转化为字符串
    //qDebug() << "result1:" << result;

  } else {
    //qDebug() << "error :" << reply->error() << endl;
    //处理错误
  }
  reply ->deleteLater();  //要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;
  reply=nullptr;
}

//向对端请求httpurl资源，再规定超时时间内将响应存入responce
int dm_HTTPObject::Get(QString httpurl, QByteArray &response, int timeout) {
    QNetworkAccessManager accessManager;
    QNetworkRequest request;
//    QUrl url(httpurl);
//    request.setUrl(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader,
//                      QVariant("application/x-www-form-urlencoded"));
//    QNetworkReply *reply = nullptr;
//    reply = accessManager.get(request);  //通过发送数据，返回值保存在reply指针里.
//    int status = 0;

//    QTimer _timer;
//    QEventLoop _event_loop;
//    connect(&_timer, SIGNAL(timeout()), &_event_loop, SLOT(quit()));
//    connect(&accessManager, SIGNAL(finished(QNetworkReply*)), &_event_loop, SLOT(quit()));
//    _timer.setSingleShot(true);
//    _timer.start(timeout * 1000);

//    _event_loop.exec();

//    if(!_timer.isActive()) {//超时
//        status = 1;
//    } else {
//        _timer.stop();
//        response = reply->readAll();
//        if(response.size() > 0) {
//            status = 0;
//        } else {
//            status = 1;
//        }
//    }

//    reply->close();
//    //reply->deleteLater();
//    delete reply;
//    reply = nullptr;

    ///note：修改于20200105，sxy
    int status = 0;
    RestClient::Response ret;
    RestClient::Connection *conn = new RestClient::Connection("");
    conn->SetTimeout(timeout);
    ret = conn->get(QByteArray::fromStdString(httpurl.toStdString()).toPercentEncoding("/:{},@.?").toStdString());
    response=QByteArray::fromStdString(ret.body);
    return status;
}

int dm_HTTPObject::ConnectToHTTP(QString httpurl, QString &response) {
  QByteArray buff;
  int status = Get(httpurl, buff, 3);
  response = buff;
  return status;
}

int dm_HTTPObject::HttpApiToDownloadImage(QString httpurl, QString image_path) {
    QByteArray buff;
    int status = Get(httpurl, buff, 120);
    if(status != 0) {
        return status;
    }

    if(buff.size() < 2) {
        return 1;
    }

    char flag = buff[1];
    unsigned char *u_flag = (unsigned char*)(&flag);
    if(*u_flag != 216) {
        return 1;
    }

    QFile file(image_path);
    file.open(QIODevice::WriteOnly);
    file.write(buff);
    file.close();
    return 0;
}

void dm_HTTPObject::ReadyToReadReply() {
  // //qDebug()<<"get reply"<<endl;
  QByteArray bytes = reply->readAll();
  file_stream.writeRawData(bytes, bytes.size());
}

void dm_HTTPObject::TestSlot() {
    //qDebug() << "signal received !" << endl;
}

bool dm_HTTPObject::downloadURL(const QString &url, const QString &fileName) {
    QByteArray buff;
    int status = Get(url, buff, 120);
    if(status != 0) {
        return false;
    }

    if(buff.size() < 2) {
        return false;
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(buff);
    file.close();
    return true;
}

int dm_HTTPObject::ListenToHttpSignalFromPort(int port, int (*p_fun)(QString &)) {
  tcpserver = new QTcpServer(this);
  tcpserver->listen(QHostAddress::Any, port);
  connect(tcpserver, &QTcpServer::newConnection, [=]() { GetInfoFromHttpPort((*p_fun)); });
  return 0;
}



void dm_HTTPObject::StopListen() {
    if(tcpserver != nullptr) {
        tcpserver->close();
        delete tcpserver;
        tcpserver = nullptr;
    }
}

int dm_HTTPObject::ListenToHttpSignalFromPortForClient(
    int port, int (*p_fun)(QString &)) {
  tcpserver = new QTcpServer(this);
  tcpserver->listen(QHostAddress::Any, port);
  connect(tcpserver, &QTcpServer::newConnection,
          [=]() { GetInfoFromHttpPort((*p_fun)); });

  return 0;
}


// for client
int dm_HTTPObject::ListenToHttpSignalFromPort(
    int port, int (*p_treatdata)(QString, QByteArray &)) {
  p_DealWithData = p_treatdata;
  tcpserver = new QTcpServer(this);
  tcpserver->listen(QHostAddress::Any, port);
  connect(tcpserver, &QTcpServer::newConnection,
          [=]() { GetInfoFromHttpPort((*p_treatdata)); });

  return 0;
}

OneConnection::OneConnection(QTcpSocket *socket, int(*p_fun)(QString &)):QObject (nullptr) {
    _socket = socket;
    _p_fun = p_fun;
    connect(_socket, SIGNAL(disconnected()), this, SLOT(OnCloseByClient()));
}

OneConnection::~OneConnection() {
    if(_socket != nullptr) {
        //TODO: I don`t know if I don`t write any data to socket but call this function whether it cause block.
        _socket->waitForBytesWritten();
        _socket->close();
        _socket->deleteLater();
        _socket = nullptr;
    }

    if(_timer != nullptr) {
        if(_timer->isActive()) {
            _timer->stop();
        }

        _timer->deleteLater();
        _timer = nullptr;
    }
}

void OneConnection::OnCloseByClient(){
    // 如果尚未处理完，连接就被客户端主动关闭
    // 在这里是暂时不必处理的。
    _closed_by_client = true;
}

void OneConnection::StartParse(){
    _timer = new QTimer(this);
    _timer->setSingleShot(true);
    connect(_timer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(OnMessageReady()));
    _timer->start(120000);
}

void OneConnection::OnTimeout(){
    if(!_message_ready) {
        deleteLater();
    }
}

// 这里的消息分为两种，一种http消息为uvss camera发送的消息：
//e.g: GET http://192.168.1.195:60800/newimage?type=small&imgid=36&groupid=0&url=192.168.1.168/jpeg_0_64.jpg&listurl=192.168.1.168/header_0_64.txt HTTP/1.1
//     Host: 192.168.1.195:60800
//     Connection: close
//一种是客户端程序发送的消息：
//e.g: GET /?q={"request_type":0} HTTP/1.1
//     User-Agent: restclient-cpp/
//     Host: 192.168.1.195:60801
//     Accept: */*
void OneConnection::OnMessageReady() {
    _message_ready = true;
    QByteArray buff = _socket->readAll();
    // 因为不用回复，所以读完请求后，可以直接关闭连接。

    _socket->write("HTTP/1.1 200 OK\nContent-Type: application/octet-stream\nContent-Length: 0\n\n");
    _socket->waitForBytesWritten();
    _socket->close();
    _socket->deleteLater();
    _socket = nullptr;

    buff = QByteArray::fromPercentEncoding(buff);

    QString str = QString::fromUtf8(buff);

#ifdef _DEBUG_MODE_
    QString log_info = "INFO_HTTP_QUEST: " + str;
    systemLog(log_info);
#endif

    QStringList para_list = str.split(QRegExp("[ \r\n][ \r\n]*"));

    if(para_list.size() < 2) {
        QString err_info = "ERROR_HTTP_REQUSET: the quest did not carry parameters.\nPara_list:\n";
        systemLog(err_info);
        for(int i = 0; i < para_list.size(); i++) {
          QString info_str = "para_list: " + para_list[i] + "\n";
          systemLog(info_str);
        }
        CloseAndDelete();
        return;
    }
    
    QString para = para_list[1];

    QStringList para2_list = para.split("newimage");
    if(para2_list.size() <= 1) {
        QString err_info = "ERROR_HTTP_REQUSET: the quest carry parameters error.\nPara_list:\n";
        systemLog(err_info);
        for(int i = 0; i < para_list.size(); i++) {
          QString info_str = "para_list: " + para_list[i] + "\n";
          systemLog(info_str);
        }
        QString info_para = "para: \n" + para + "\nPara2_list:\n";
        systemLog(info_para);
        for(int i = 0; i < para2_list.size(); i++) {
          QString info_str = "para2_list: " + para2_list[i] + "\n";
          systemLog(info_str);
        }
        CloseAndDelete();
        return;
    }

    //qDebug() << "new connection " << para;

    _p_fun(para);
    CloseAndDelete();
}

void OneConnection::CloseAndDelete() {
    this->deleteLater();
}

// 这个函数好像没有地方调用
void dm_HTTPObject::GetInfoFromHttpPort(int (*p_fun)(QString, QByteArray &)) {
  QTcpSocket *tcpSocket = tcpserver->nextPendingConnection();

  connect(tcpSocket, &QTcpSocket::readyRead, [=]() {
    //从通信套接字中取出内容
    QByteArray array = tcpSocket->readAll();
    QString tmp_str = QString::fromUtf8(array);

#ifdef _DEBUG_MODE_
    systemLog(tmp_str);
#endif
    QStringList paras = tmp_str.split("q=");
    bool is_web_http = false;
    if(paras.count() == 1) {//处理uvss camera消息
        //e.g
        //GET http://192.168.1.195:60800/newimage?type=small&imgid=36&groupid=0&url=192.168.1.168/jpeg_0_64.jpg&listurl=192.168.1.168/header_0_64.txt HTTP/1.1
        //Host: 192.168.1.195:60800
        //Connection: close
        tmp_str = paras[0];

        QStringList paras2 = tmp_str.split("newimage");
        if(paras2.count() > 1) {
            is_web_http = true;
        }
    } else {//处理客户端程序请求
        //e.g
        //GET /?q={"request_type":0} HTTP/1.1
        //Host: 192.168.1.195:60801
        //User-Agent: restclient-cpp/
        //Accept: */*
        tmp_str = QByteArray::fromPercentEncoding(array);
        paras = tmp_str.split("q=");
        is_web_http = true;

        QStringList paras2 = paras[1].split(" ");
        tmp_str = paras2[0];
    }

    QByteArray res_data;
    //对端请求处理回
    p_fun(tmp_str, res_data);
    /*需要处理*/
    if(is_web_http) {
        tcpSocket->write("HTTP/1.1 200 OK\nContent-Type: application/octet-stream\n\n");
    }
    tcpSocket->write(res_data);
    tcpSocket->waitForBytesWritten();
    tcpSocket->close();
    tcpSocket->deleteLater();

  });

}

void dm_HTTPObject::GetInfoFromHttpPort(int (*p_fun)(QString &)) {
    // 获取连接
  QTcpSocket *tcpSocket = tcpserver->nextPendingConnection();

  // 因为每个连接的处理方法都比较复杂，要考虑很多情况，所以独立一个类出来处理。
  // 1. 连接成功创建，但客户端迟迟不送消息请求参数，也不关闭连接。
  // 2. 连接成功创建，客户端送了参数，但未等服务器处理完就把连接关闭了。
  OneConnection *_connection = new OneConnection(tcpSocket, p_fun);
  // _connection 会在执行完回调函数后，自动销毁。
  _connection->StartParse();
}
