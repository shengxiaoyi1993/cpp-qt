#include "nvrconfig.h"
#include "ui_nvrconfig.h"


NvrConfig::NvrConfig(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NvrConfig)
{
  ui->setupUi(this);

  /// 临时禁用项
  ui->pushButton_connecttest->setDisabled(true);
//  ui->pushButton_nvrsetting->setDisabled(true);
}

NvrConfig::~NvrConfig()
{
  delete ui;
}

void NvrConfig::setNvrDev(const ns_tcs_ds_def::NvrDev v_dev){
  _nvrdev=v_dev;
  ui->lineEdit_ip->setText(QString::fromStdString(_nvrdev._ip));
  ui->lineEdit_username->setText(QString::fromStdString(_nvrdev._username));
  ui->lineEdit_password->setText(QString::fromStdString(_nvrdev._password));
}

void NvrConfig::empty(){
  _nvrdev=ns_tcs_ds_def::NvrDev();
  ui->lineEdit_ip->setText("");
  ui->lineEdit_username->setText("");
  ui->lineEdit_password->setText("");
}

void NvrConfig::on_pushButton_connecttest_clicked()
{
  emit sgl_testconnetct_nvr(getCurrentNvrInfo());
}

void NvrConfig::on_pushButton_cancel_clicked()
{
  this->hide();
  empty();
}

void NvrConfig::on_pushButton_nvrsetting_clicked()
{

  emit sgl_startnvrApp(getCurrentNvrInfo());
}

void NvrConfig::on_pushButton_apply_clicked()
{
  emit sgl_setnvr(getCurrentNvrInfo());
}

void NvrConfig::on_pushButton_confirm_clicked()
{
  emit sgl_setnvr(getCurrentNvrInfo());
  this->hide();

}

ns_tcs_ds_def::NvrDev NvrConfig::getCurrentNvrInfo() const{
  ns_tcs_ds_def::NvrDev v_nvr;
  v_nvr._ip=ui->lineEdit_ip->text().toStdString();
  v_nvr._password=ui->lineEdit_password->text().toStdString();
  v_nvr._username=ui->lineEdit_username->text().toStdString();
  return v_nvr;
}


