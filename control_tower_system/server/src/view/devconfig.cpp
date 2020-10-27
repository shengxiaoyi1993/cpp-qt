#include "devconfig.h"
#include "ui_devconfig.h"

DevConfig::DevConfig(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DevConfig)
{
  ui->setupUi(this);
  _devitem_instance=new DevItem(this);


  connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* , int )),_devitem_instance,SLOT(show()));

}

DevConfig::~DevConfig()
{
  delete ui;
}

void DevConfig::on_pushButton_clicked()
{
  _devitem_instance->show();
}

void DevConfig::on_pushButton_3_clicked()
{
    this->hide();
  _devitem_instance->hide();
}
