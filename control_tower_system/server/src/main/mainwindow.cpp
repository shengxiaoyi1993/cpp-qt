#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  _userconfig=new UserConfig(this);
  _diag_login=new LoginServer(this);
  _devconfig=new DevConfig(this);
  _nvrconfig =new NvrConfig(this);


  _diag_login->show();


  connect(ui->action_userconfig,SIGNAL(triggered()),_userconfig,SLOT(show()));
  connect(_diag_login,SIGNAL(signal_Login()),this,SLOT(show()));
  connect(_diag_login,SIGNAL(signal_Login()),_diag_login,SLOT(hide()));
  connect(ui->action_devconfig,SIGNAL(triggered()),_devconfig,SLOT(show()));
  connect(ui->action_nvrconfig,SIGNAL(triggered()),_nvrconfig,SLOT(show()));

  QMenu i;
}

MainWindow::~MainWindow()
{
  delete ui;
}
