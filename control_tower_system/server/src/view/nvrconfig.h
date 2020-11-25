#ifndef NVRCONFIG_H
#define NVRCONFIG_H

#include <QDialog>
#include "ds_def/tcs_ds_def.h"


namespace Ui {
class NvrConfig;
}

class NvrConfig : public QDialog
{
  Q_OBJECT

public:
  explicit NvrConfig(QWidget *parent = nullptr);
  ~NvrConfig();
  void setNvrDev(const ns_tcs_ds_def::NvrDev v_dev);
  void empty();
  ns_tcs_ds_def::NvrDev getCurrentNvrInfo() const;


private slots:
  void on_pushButton_connecttest_clicked();
  void on_pushButton_cancel_clicked();

  void on_pushButton_nvrsetting_clicked();

  void on_pushButton_apply_clicked();

  void on_pushButton_confirm_clicked();

signals:
  void sgl_emptynvr();
  void sgl_setnvr(ns_tcs_ds_def::NvrDev);
  void sgl_testconnetct_nvr(ns_tcs_ds_def::NvrDev);
  void sgl_startnvrApp(ns_tcs_ds_def::NvrDev);


private:
  Ui::NvrConfig *ui;
  ns_tcs_ds_def::NvrDev _nvrdev;


};

#endif // NVRCONFIG_H
