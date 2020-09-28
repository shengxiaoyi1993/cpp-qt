#include "mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  _pushbutton=new QPushButton();
  _pushbutton->setText(tr("Push"));
  _linedit=new QLineEdit();
  _label=new QLabel();

  QWidget* widget_main=new QWidget();
  this->setCentralWidget(widget_main);
  QGridLayout* gridlayout_main=new QGridLayout();
  widget_main->setLayout(gridlayout_main);

  gridlayout_main->addWidget(new QLabel("Input:"),0,0);
  gridlayout_main->addWidget(_linedit,0,1);
  gridlayout_main->addWidget(_pushbutton,0,2);
  gridlayout_main->addWidget(new QLabel("Output:"),1,0);
  gridlayout_main->addWidget(_label,1,1);
  connect(_pushbutton,SIGNAL(clicked(bool)),this,SLOT(on_pushbutton_click()));

}

MainWindow::~MainWindow()
{
  delete _pushbutton;
  delete _linedit;
  delete _label;
}

void MainWindow::on_pushbutton_click(){
  _label->setText(_linedit->text());
}

