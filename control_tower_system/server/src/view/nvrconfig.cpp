#include "nvrconfig.h"
#include "ui_nvrconfig.h"

NvrConfig::NvrConfig(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NvrConfig)
{
  ui->setupUi(this);
}

NvrConfig::~NvrConfig()
{
  delete ui;
}
