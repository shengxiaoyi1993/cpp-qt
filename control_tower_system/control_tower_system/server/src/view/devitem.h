#ifndef DEVITEM_H
#define DEVITEM_H

#include <QDialog>
#include "../../../server_service/src/ds_def/tcs_ds_def.h"


namespace Ui {
class DevItem;
}

/// 使用前需要先设置模式，先设置是否是add模式，然后设置是否是直显模式
/// 对于配置直显相机需要先设置service的串口列表，

/// 自判断逻辑
/// 若设备类型设置成直显相机，则串口设置界面可用；否则串口配置界面不可用


class DevItem : public QDialog
{
  Q_OBJECT

public:
  explicit DevItem(QWidget *parent = nullptr);
  ~DevItem();
  /// 更新数据
  void updateCamItem(const ns_tcs_ds_def::CamDev v_cam);
  /// 清空数据
  void empty();

  /// 添加模式下，删除按钮不可用
  void setAddMode(bool v_flag);

  /// 将列表存储到成员变量
  /// 更新多选框中的值
  void setTTYList(const vector<string> l_tty);

  ///设置直显相机，非直显相机串口选择多选框不可用
  void setDirectMode(bool v_flag);



private slots:

  void on_pushButton_connecttest_clicked();
  void on_pushButton_del_clicked();

  /// 点击应用按钮后，
  ///   若处于添加设备模式，则发出添加设备的请求；将请求删除按钮置为可用状态
  ///   若处于修改设备模式，则发出修改设备的请求
  ///
  void on_pushButton_apply_clicked();

  /// 同应用按钮，但是会隐藏界面
  void on_pushButton_confirm_clicked();

  /// 仅隐藏界面，不做其他操作
  void on_pushButton_cancel_clicked();


  void on_comboBox_type_currentIndexChanged(int index);

signals:
  void sgl_addCam(ns_tcs_ds_def::CamDev v_cam);
  void sgl_rmCam(ns_tcs_ds_def::CamDev v_cam);
  void sgl_modifyCam(ns_tcs_ds_def::CamDev v_cam);
  void sgl_testCamConnect(ns_tcs_ds_def::CamDev v_cam);

private:
  Ui::DevItem *ui;
  bool _is_addmode;
  bool _is_directmode;
  vector<string> _tty;
  /// 将控件上的数据收集起来组合成一条cam数据
  ns_tcs_ds_def::CamDev getUserInfo() const;



};

#endif // DEVITEM_H
