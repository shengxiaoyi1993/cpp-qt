#include "initview.h"
#include "ui_initview.h"

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
