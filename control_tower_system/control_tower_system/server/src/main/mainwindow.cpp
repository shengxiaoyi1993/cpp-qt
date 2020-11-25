#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QDateTime>
#include <assert.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->textEdit_log->clear();
  ui->pushButton_start->setDisabled(true);
  ui->pushButton_disconnect->setDisabled(true);
  QMenu i;
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_pushButton_cancel_clicked()
{
  //  exit(0);
  emit sgl_exitGUI();

}

QAction* MainWindow::action_userconfig(){
  return ui->action_userconfig;
}

QAction* MainWindow::action_devconfig(){
  return ui->action_devconfig;
}

QAction* MainWindow::action_nvrconfig(){
  return ui->action_nvrconfig;
}

bool MainWindow::autoLauchStaus()const{
  return _status_autorestart;
}

void MainWindow::on_pushButton_start_clicked()
{
  emit sgl_setAutoLauch(!_status_autorestart);
}


void MainWindow::pushbackLog(const string &v_log){
  ui->textEdit_log->append(QString::fromStdString(v_log));
}



void MainWindow::uopdateServerStaus(const string &v_serverip,
                                    int v_status,
                                    int v_lasttime,
                                    int v_user){

  cout<<__func__<<"v_lasttime:"<<v_lasttime<<endl;

  /// IP设置
  if(v_serverip != ""){
    ui->label_ip->setText(QString::fromStdString(v_serverip));
  }


  /// 运行状态设置
  if (v_status !=-1) {
    if (v_status == 1) {
      ui->label_status->setText("运行正常");
    }
    else if (v_status == 0) {
      ui->label_status->setText("运行出错");
    }
  }

  /// set lasttime
  if(v_lasttime > 0){

    ui->label_lasttime->setText(QString::fromStdString(formatTimeInterval(v_lasttime)));
  }

  /// set usercount
  if(v_user>=0){
    ui->label_usercount->setText(QString::number(v_user));
  }

}


void MainWindow::updateUserTatusTable(const vector<string> &l_user,
                                      const vector<string>&l_type,
                                      const vector<bool>&l_status,
                                      const vector<int>&l_aircam,
                                      const vector<int>&l_groundcam,
                                      const vector<int>&l_directcam
                                      ){
  assert(l_user.size()==l_type.size()
         &&l_user.size()==l_status.size()
         &&l_user.size()==l_aircam.size()
         &&l_user.size()==l_groundcam.size()
         &&l_user.size()==l_directcam.size());


  if (ui->tableWidget->rowCount()!=l_user.size()) {
    while (ui->tableWidget->rowCount()>0) {
      ui->tableWidget->removeRow(0);
    }
    for (uint i=0;i<l_user.size();i++) {
      ui->tableWidget->insertRow(i);
    }
  }

  for (uint i=0;i<l_user.size();i++) {
    ui->tableWidget->setItem(i,0,new QTableWidgetItem( QString::fromStdString(l_user[i])));
    ui->tableWidget->setItem(i,1,
                             new QTableWidgetItem(QString::fromStdString(l_type[i])));
    QString status_tmp=(l_status[i])?"已连接":"未连接";
    ui->tableWidget->setItem(i,2,new QTableWidgetItem(status_tmp));
    ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(l_aircam[i])));
    ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(l_groundcam[i])));
    ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(l_directcam[i])));
  }

}


