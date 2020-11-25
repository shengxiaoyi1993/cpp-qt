#include "togroudcamview.h"
#include "ui_togroudcamview.h"
#include <QDebug>

ToGroudCamView::ToGroudCamView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ToGroudCamView)
{
  ui->setupUi(this);

}

ToGroudCamView::~ToGroudCamView()
{
  delete ui;
}


void ToGroudCamView::paintEvent(QPaintEvent* v_event){
  QPalette PAllbackground = ui->label->palette();
  QImage ImgAllbackground("/home/sxy/Github/cpp-qt/control_tower_system/client/resources/to-ground-detail1.jpg");
  QImage fitimgpic=ImgAllbackground.scaled(ui->label->width(),ui->label->height(), Qt::IgnoreAspectRatio);
  PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
  ui->label->setPalette(PAllbackground);
}

