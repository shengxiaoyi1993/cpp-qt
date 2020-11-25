#include "../controller/controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  int execcode=0;
  do {
    QApplication a(argc, argv);
    Controller control;
    execcode=a.exec();
  } while (execcode == Controller::REBOOT_CODE);
  return execcode;

}
