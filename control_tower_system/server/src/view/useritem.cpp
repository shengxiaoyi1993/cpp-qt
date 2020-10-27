#include "useritem.h"
#include "ui_useritem.h"
#include <QDebug>

UserItem::UserItem(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::UserItem)
{
  ui->setupUi(this);

  QTreeWidgetItemIterator iter(ui->treeWidget);
  while (*iter) {
    //do something like
    qDebug()<<(*iter)->text(0);
    (*iter)->setCheckState(1,Qt::CheckState::Unchecked);
    (*iter)->setCheckState(2,Qt::CheckState::Unchecked);
    ++iter;
  }

}

UserItem::~UserItem()
{
  delete ui;
}

void UserItem::on_pushButton_2_clicked()
{
    this->hide();
}

void UserItem::on_pushButton_3_clicked()
{
  this->hide();
}
