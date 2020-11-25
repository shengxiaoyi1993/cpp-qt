#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  _view_toair=new ToAirCamView();
  _view_togroud=new ToGroudCamView();
  _view_direct=new DirectCamView();
  _view_esmap=new ElectronicSituationMap();
  _view_nvr=new NvrReview();


  ui->stackedWidget->addWidget(_view_toair);
  ui->stackedWidget->addWidget(_view_togroud);
  ui->stackedWidget->addWidget(_view_direct);
  ui->stackedWidget->addWidget(_view_esmap);
  ui->stackedWidget->addWidget(_view_nvr);

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
  exit(0);
}

void MainWindow::on_pushButton_clicked()
{
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
  ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_4_clicked()
{
  ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::on_pushButton_5_clicked()
{
//  ui->stackedWidget->setCurrentIndex(4);
    QString program = "C:\\Program Files\\Smart Professional Surveillance System\\SmartPSS\\SmartPSS.exe";//指定要调用的可执行程序的路径
    QProcess *myProcess = new QProcess(this);
    //this表示myprocess的父窗口是本程序中的主窗口，当主窗口关闭时，进程也会终止，
    //如果不填加this,myprocess是独立的进程，主窗口关闭，进程依然运行
    myProcess->start(program,QStringList());
    myProcess->pid();
}







