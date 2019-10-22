#include <QCoreApplication>
#include "downloader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Downloader downloader;
    downloader.downloadFileFromURL("http://192.168.1.118/cgi-bin/snapshot.cgi",
                                   "D:/sxy/project/test/Images/erl2.txt");

//    qDebug()<<downloadURL("http://root:admin123@192.168.1.178/jpeg_2_128.jpg"
//                ,"D:/sxy/project/test/Images/erl.jpg");
    return a.exec();
}
