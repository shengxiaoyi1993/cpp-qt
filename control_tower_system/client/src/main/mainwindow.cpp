#include "mainwindow.h"
#include "ui_mainwindow.h"

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

  _loginview=new LoginClient(this);
  _loginview->show();

  ui->stackedWidget->addWidget(_view_toair);
  ui->stackedWidget->addWidget(_view_togroud);
  ui->stackedWidget->addWidget(_view_direct);
  ui->stackedWidget->addWidget(_view_esmap);
  ui->stackedWidget->addWidget(_view_nvr);


  connect(_loginview,SIGNAL(signal_login()),this,SLOT(showFullScreen()));


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
  ui->stackedWidget->setCurrentIndex(4);
}
