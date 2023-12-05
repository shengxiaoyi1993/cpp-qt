#include "../view/mainwindow.h"
#include "../view/initview.h"
#include "../controller/controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  InitView initview;
  initview.show();
  Controller ctr(&initview,&w);

//  w.show();

  return a.exec();
}
