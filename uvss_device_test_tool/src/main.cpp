//#include "mainwindow.h"
//#include <QApplication>
#include "uvsstesttool.h"
#include "cjsonobject/CJsonObject.hpp"
#include "configrelate.h"
#include <iostream>
#include <QDebug>
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include <QCoreApplication>

void test_restclient();

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  //  MainWindow w;
  //  w.show();

  UVSSTestTool testtool;
  testtool.start();

//  test_restclient();

  return a.exec();

}


void test_taskconfig(){
  try {
    TaskConfig v_taskconfig;
    int ret_load=loadTaskConfigFromFile("../../resources/test_config.config",v_taskconfig);
    qDebug()<<"ret_load:"<<ret_load;
    qDebug()<<"printTaskConfig:"<<printTaskConfig(v_taskconfig);

  } catch (QString v_msg) {
    qDebug()<<"v_msg:"<<v_msg;
  }

}

void test_restclient(){
  int status = 0;
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->SetTimeout(3);
  ret = conn->get(QByteArray::fromStdString("192.168.1.254").toPercentEncoding("/:{},@.?").toStdString());
  QString response=QByteArray::fromStdString(ret.body);
  qDebug()<<"response:"<<response;
  delete conn;
}
