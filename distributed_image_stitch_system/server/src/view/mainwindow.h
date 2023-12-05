#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"


using namespace std;

namespace Ui {
class MainWindow;
}

/// 更新输入输出文件
/// 配置客户端

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  string __dir_input;
  string __dir_output;


signals:
  void sgl_startListen();
  void sgl_stopListen();

  void sgl_startWork();
  void sgl_stopWork();

  void sgl_selectInputDir(QString);
  void sgl_selectOutputDir(QString);
  void sgl_init();

  void sgl_setIP(QString);
  void sgl_addClientIP(QString);


private slots:
  void on_pushButton_select_inputdir_clicked();

  void on_pushButton_select_output_dir_clicked();

  void on_pushButton_addclient_clicked();

  void on_pushButton_initconfig_clicked();

  void on_pushButton_startwork_clicked();

  void on_pushButton_stoplisten_clicked();

  void on_pushButton_setip_clicked();

  void on_pushButton_stopwork_clicked();

  void on_pushButton_startlisten_clicked();

  void slt_on_ctr_statuschange( map<ds_df_diss::ENUM_CTR_STATUS,bool>);
  void slt_showWarning(QString);


private:
  Ui::MainWindow *ui;
  map<ds_df_diss::ENUM_CTR_STATUS,bool> __l_ctr_status;

  Qt::CheckState checkStateFromBool(bool v_b);
  void updateCtrStatus();

};

#endif // MAINWINDOW_H
