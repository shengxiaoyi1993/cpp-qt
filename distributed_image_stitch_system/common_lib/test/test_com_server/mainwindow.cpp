#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
//  UserMap tmp[1];
//  strcpy(tmp[0].username,"pub");
//  strcpy(tmp[0].hash,"0017dea7770f7ecff7ab3c20506546129e96bdeba2f544bb8e5414eb79786122");
//  SetAllUserMap(tmp,1);


  SetupListenHttpWithPort(cb_CallbackOnNewHttpRequest,cb_CallbackOnHttpError,nullptr,40400);
}

MainWindow::~MainWindow()
{
  delete ui;
}
