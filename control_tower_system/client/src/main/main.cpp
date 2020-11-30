#include "../controller/controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  int execcode=0;
  do {
    QApplication a(argc, argv);
    Controller *control;
    try {
      control=new Controller;
    } catch (std::exception v_ex) {
       cout<<"exception:"<<v_ex.what()<<endl;
    }
    execcode=a.exec();
  } while (execcode == Controller::REBOOT_CODE);
  return execcode;

}
