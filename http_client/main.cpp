#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client w;
    w.startConnect("http://admin:admin123@192.168.1.118/cgi-bin/snapshot.cgi", 80); // 调用外部接口

    return a.exec();
}
