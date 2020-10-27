#include "electronicsituationmap.h"
#include "ui_electronicsituationmap.h"
#include <QDebug>

ElectronicSituationMap::ElectronicSituationMap(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ElectronicSituationMap)
{
  ui->setupUi(this);
  ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

  ui->tableWidget->setCellWidget(0,4,new QPushButton("删除"));
  ui->tableWidget->setCellWidget(1,4,new QPushButton("删除"));
  ui->tableWidget->setCellWidget(2,4,new QPushButton("删除"));
  ui->tableWidget->setCellWidget(3,4,new QPushButton("删除"));

}

ElectronicSituationMap::~ElectronicSituationMap()
{
  delete ui;
}
