#include "nvrreview.h"
#include "ui_nvrreview.h"

NvrReview::NvrReview(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::NvrReview)
{
  ui->setupUi(this);
}

NvrReview::~NvrReview()
{
  delete ui;
}
