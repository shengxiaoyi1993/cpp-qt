#include "loginserver.h"
#include "ui_loginserver.h"

LoginServer::LoginServer(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoginServer)
{
  ui->setupUi(this);


  QString IpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegularExpression IpRegex ("^" + IpRange
                                  + "(\\." + IpRange + ")"
                                  + "(\\." + IpRange + ")"
                                  + "(\\." + IpRange + ")$");
   _ipValidator= new QRegularExpressionValidator(IpRegex, ui->lineEdit_ip);

   ui->lineEdit_password->setText("admin");
   ui->lineEdit_ip->setText("192.168.1.126");
   ui->lineEdit_username->setText("admin");

}

LoginServer::~LoginServer()
{
  delete ui;
}



void LoginServer::on_pushButton_login_clicked()
{
  emit sgl_reqToLogin(
         ui->lineEdit_ip->text().toStdString(),
        ui->lineEdit_username->text().toStdString(),
        ui->lineEdit_password->text().toStdString()
        );
}

void LoginServer::on_pushButton_cancel_clicked()
{
  emit sgl_reqToQuit();
}
