#ifndef DEVCONFIG_H
#define DEVCONFIG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "devitem.h"
#include "ds_def/tcs_ds_def.h"


/// 用于显示设备和配置设备，将配置好的信息组合传递给controler

namespace Ui {
class DevConfig;
}

class DevConfig : public QDialog
{
  Q_OBJECT

public:
  explicit DevConfig(QWidget *parent = nullptr);
  ~DevConfig();
  void updateCamArray(const ns_tcs_ds_def::CameraArray &v_array);
  void updateSerialPort(const vector<string> &l_port);

private slots:
  void on_pushButton_adddev_clicked();

  void on_pushButton_removealldev_clicked();

  void on_pushButton_close_clicked();
  void on_treewidget_itemClicked(QTreeWidgetItem* , int );

  void slot2DevItem_addCam(ns_tcs_ds_def::CamDev v_cam);
  void slot2DevItem_rmCam(ns_tcs_ds_def::CamDev v_cam);
  void slot2DevItem_modifyCam(ns_tcs_ds_def::CamDev v_cam);
  void slot2DevItem_testCamConnect(ns_tcs_ds_def::CamDev v_cam);

  signals:
  void sgl_addCam(ns_tcs_ds_def::CamDev v_cam);
  void sgl_rmCam(ns_tcs_ds_def::CamDev v_cam);
  void sgl_modifyCam(ns_tcs_ds_def::CamDev v_cam);
  void sgl_testCamConnect(ns_tcs_ds_def::CamDev v_cam);


private:
  Ui::DevConfig *ui;
  DevItem       *_devitem_instance;
  ns_tcs_ds_def::CameraArray _camarray;
  vector<string> _l_serialport;

};

#endif // DEVCONFIG_H
