#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>

#include <QMainWindow>

class Client : public QMainWindow
{
    Q_OBJECT //为了使用信号与槽，需要写这个宏

public:
    Client(QWidget *parent = 0);
    ~Client();

    void startConnect(QString host, quint16 port); // 定义一个外部接口，用于连接外部主机

protected slots:
    void onConnected(); // 连接服务器
    void onReadyRead(); // 读取数据

private:
    QTcpSocket *m_socket;

};

#endif // CLIENT_H
