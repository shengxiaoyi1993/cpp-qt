#include "userconfig.h"
#include "ui_userconfig.h"

UserConfig::UserConfig(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::UserConfig)
{
  ui->setupUi(this);
  _useritem_instance=new UserItem(this);

ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

//QTableWidget it;

connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),_useritem_instance,SLOT(show()));

}

UserConfig::~UserConfig()
{
  delete ui;
}

void UserConfig::on_pushButton_3_clicked()
{
    this->hide();
  _useritem_instance->hide();
}

void UserConfig::on_pushButton_clicked()
{
    _useritem_instance->show();
}
