#include "mainwindow.h"
#include <QFileDialog>
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_select_inputdir_clicked()
{
  QString fileNames = QFileDialog::getExistingDirectory(
        this,
        tr("src"),
        "/storage/Data/Data_tianjin_2020_09_23/sort/Data_Tianjin_20200922/divide_result");
  __dir_input=fileNames.toStdString()+"/";

  emit sgl_selectInputDir(fileNames+"/");
  ui->label_input->setText(fileNames+"/");

}

void MainWindow::on_pushButton_select_output_dir_clicked()
{

  QString fileNames = QFileDialog::getExistingDirectory(
        this,
        tr("src"),
        "/storage/Data/Data_tianjin_2020_09_23/sort/Data_Tianjin_20200922/stitch_result");
  __dir_output=fileNames.toStdString()+"/";
  emit sgl_selectOutputDir(fileNames+"/");
  ui->label_output->setText(fileNames+"/");

}




void MainWindow::on_pushButton_addclient_clicked()
{
  emit sgl_addClientIP(ui->lineEdit_clientip->text());

}

void MainWindow::on_pushButton_initconfig_clicked()
{
  emit sgl_init();

}

void MainWindow::on_pushButton_startwork_clicked()
{
  emit sgl_startWork();
}

void MainWindow::on_pushButton_stoplisten_clicked()
{
  emit sgl_stopListen();
}

void MainWindow::on_pushButton_setip_clicked()
{
  if (ui->lineEdit_ip->text()!="") {
    emit sgl_setIP(ui->lineEdit_ip->text());
  }

}

void MainWindow::on_pushButton_stopwork_clicked()
{
  emit sgl_stopWork();
}

void MainWindow::on_pushButton_startlisten_clicked()
{
  emit sgl_startListen();
}

void MainWindow::slt_on_ctr_statuschange( map<ds_df_diss::ENUM_CTR_STATUS,bool> l_status){
  __l_ctr_status = l_status;
  updateCtrStatus();

}

/// 相应控件发生相应的状态
void MainWindow::updateCtrStatus(){
  ui->checkBox_status_complete->setCheckState(checkStateFromBool(__l_ctr_status[ds_df_diss::ENUM_CTR_STATUS_COMPLETE]) );
  ui->checkBox_status_dir->setCheckState(checkStateFromBool(__l_ctr_status[ds_df_diss::ENUM_CTR_STATUS_SETDIR]) );
  ui->checkBox_status_init->setCheckState(checkStateFromBool(__l_ctr_status[ds_df_diss::ENUM_CTR_STATUS_INIT]) );
  ui->checkBox_status_ip->setCheckState(checkStateFromBool(__l_ctr_status[ds_df_diss::ENUM_CTR_STATUS_SETIP]) );
  ui->checkBox_status_lsten->setCheckState(checkStateFromBool(__l_ctr_status[ds_df_diss::ENUM_CTR_STATUS_LISTEN]) );
  ui->checkBox_status_work->setCheckState(checkStateFromBool(__l_ctr_status[ds_df_diss::ENUM_CTR_STATUS_WORK]) );
}

Qt::CheckState MainWindow::checkStateFromBool(bool v_b){
  if (v_b == true) {
    return static_cast<Qt::CheckState>(2);
  }
  else
  {
    return static_cast<Qt::CheckState>(0);
  }
}

void MainWindow::slt_showWarning(QString v_warn){
  QMessageBox::information(this, tr("Warning"),
                           v_warn);


}







