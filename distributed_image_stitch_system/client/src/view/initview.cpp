#include "initview.h"
#include "ui_initview.h"
#include <QMessageBox>

InitView::InitView(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::InitView)
{
  ui->setupUi(this);
}

InitView::~InitView()
{
  delete ui;
}

void InitView::on_pushButton_ok_clicked()
{
  if (ui->lineEdit_ip->text().isEmpty()||ui->spinBox_threadcount->value()==0) {
    QMessageBox::information(this, tr("Warning"),
                             tr("Input Data cannot be NULL !"));
  }
  else{
    emit sgl_on_initConfig(ui->lineEdit_ip->text(),ui->spinBox_threadcount->value());
  }

}

void InitView::on_pushButton_cancel_clicked()
{
    exit(0);
}
