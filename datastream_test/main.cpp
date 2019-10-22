#include <QCoreApplication>
#include<QFile>
#include<QDataStream>
#include<QDebug>
using namespace std;
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QFile file("file.txt");
  file.open(QIODevice::WriteOnly);
  QDataStream out(&file);   // we will serialize the data into the file
  //使用操作符输入会使out带有无效字符，输出结果为30
  //out << QString("the answer is");   // serialize a string

  //输出结果为14，没有无效字符
  //out.writeRawData("the answer is",sizeof("the answer is"));

  qDebug()<<"length : "<<file.size();
  file.close();
  exit(1);
  return a.exec();
}
