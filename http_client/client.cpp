#include "client.h"
#include <QDebug>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
}

Client::~Client()
{

}

// 实现外部接口的功能用于连接外部主机
void Client::startConnect(QString host, quint16 port)
{
    m_socket = new QTcpSocket(this);
    // 连接信号与槽
    connect(m_socket, SIGNAL(connected()),
            this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    // 连接远程主机
    m_socket->connectToHost(host, port);
}

// 实现连接服务器
void Client::onConnected()
{
    m_socket->write("GET / HTTP/1.1\r\n\r\n"); // 发送一个Http头部
}

// 读取服务器返回的数据
void Client::onReadyRead()
{
    static int count=0;
    count++;
    qDebug() << m_socket->readAll();
    qDebug()<<"count:"<<count<<endl;
}
