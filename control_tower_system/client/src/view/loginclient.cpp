#include "loginclient.h"
#include "ui_loginclient.h"

LoginClient::LoginClient(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoginClient)
{
  ui->setupUi(this);
}

LoginClient::~LoginClient()
{
  delete ui;
}

void LoginClient::on_pushButton_clicked()
{
  this->hide();
  emit signal_login();
}

void LoginClient::on_pushButton_2_clicked()
{
  exit(0);
}
