#include "toaircamview.h"
#include "ui_toaircamview.h"

ToAirCamView::ToAirCamView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ToAirCamView)
{
  ui->setupUi(this);
}

ToAirCamView::~ToAirCamView()
{
  delete ui;
}
