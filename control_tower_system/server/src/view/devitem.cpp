#include "devitem.h"
#include "ui_devitem.h"

DevItem::DevItem(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DevItem)
{
  ui->setupUi(this);
}

DevItem::~DevItem()
{
  delete ui;
}

void DevItem::on_pushButton_4_clicked()
{
    this->hide();
}
