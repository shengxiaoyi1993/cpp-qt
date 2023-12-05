#include "../view/mainwindow.h"
#include "../controller/controller.h"
#include <QCryptographicHash>
#include <QCryptographicHash>

#include <QApplication>

int main(int argc, char *argv[])
{

  cout<<"getpid: main "<<std::this_thread::get_id()<<endl;

  QApplication a(argc, argv);
  MainWindow w;
  ThreadPool *threadpoo0l=new ThreadPool(6);

  Controller  controller_main(&w,threadpoo0l);
  controller_main.start();
  w.show();
  return a.exec();
}
