#include <QCoreApplication>
#include <iostream>
#include <QCryptographicHash>
#include "../datamanage/datamanage.h"
#include "../controler/controler.h"



using namespace std;

void testCom();

int main(int argc, char *argv[])
{

  QCoreApplication a(argc, argv);


  Controler *ctlr;
  try {
    ctlr=new Controler("default.config");

  } catch (std::exception v_ex) {
    cout<<"exception:"<<v_ex.what()<<endl;
  }


  return a.exec();
}



