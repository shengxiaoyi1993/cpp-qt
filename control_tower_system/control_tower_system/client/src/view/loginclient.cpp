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


void LoginClient::on_pushButton_login_clicked()
{
  emit sgl_reqToLogin(
         ui->lineEdit_ip->text().toStdString(),
        ui->lineEdit_username->text().toStdString(),
        ui->lineEdit_password->text().toStdString()
        );
}

void LoginClient::on_pushButton_cancel_clicked()
{
  emit sgl_reqToQuit();
}
