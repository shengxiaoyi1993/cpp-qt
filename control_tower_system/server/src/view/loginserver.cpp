#include "loginserver.h"
#include "ui_loginserver.h"

LoginServer::LoginServer(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoginServer)
{
  ui->setupUi(this);
}

LoginServer::~LoginServer()
{
  delete ui;
}

void LoginServer::on_pushButton_clicked()
{
    emit signal_Login();
}

void LoginServer::on_pushButton_2_clicked()
{
    exit(0);
}
