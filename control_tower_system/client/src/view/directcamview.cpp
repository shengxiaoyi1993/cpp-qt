#include "directcamview.h"
#include "ui_directcamview.h"

DirectCamView::DirectCamView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DirectCamView)
{
  ui->setupUi(this);
}

DirectCamView::~DirectCamView()
{
  delete ui;
}
