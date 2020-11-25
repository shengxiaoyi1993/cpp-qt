#ifndef USERITEM_H
#define USERITEM_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "../../../server_service/src/ds_def/tcs_ds_def.h"



/// 点击设备控制项时，空间内部进行自行判断的逻辑；因此控制项被建立时需要爱建立信号和槽响应机制
///   1. 无效的控制项，先使用不可编辑状态
///   2. 对于更改控制项的操作
///     - top项的选中或取消选中某一项，子项的相应项发生相应修改
///     - 每一项具有控制范围的判断，可操作一定可看，不可看一定不可操作


/// 删除按钮可以设置为不可用
/// 删除按钮用于删除用户
/// 确认按钮用于修改或者添加用户
/// 取消按钮用于退出按钮编辑界面

/// 对于添加用户时，需要清空UserItem中的数据，用户名/密码和设备类新/设备信息使用admin的

namespace Ui {
class UserItem;
}

class UserItem : public QDialog
{
  Q_OBJECT

public:
  explicit UserItem(QWidget *parent = nullptr);
  ~UserItem();
  void setUserItem(const ns_tcs_ds_def::User &v_user);
  void setAddMode(bool v_flag);
  void empty();
  void updateCamCtlTable(const vector<ns_tcs_ds_def::CamCtl> &l_camctl);



private slots:
  void on_pushButton_apply_click();
  void on_pushButton_cancel_click();
  void on_pushButton_confirm_click();
  void on_pushButton_del_click();
  vector<ns_tcs_ds_def::CamCtl> getCamCtl();
  void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

signals:
  void sgl_rmOneUser( ns_tcs_ds_def::User v_user);
  void sgl_addOneUser(ns_tcs_ds_def::User v_user);
  void sgl_modifyOneUser(ns_tcs_ds_def::User v_user);

private:
  Ui::UserItem *ui;
  bool _is_addmode;

  /// 获取控件中的数据，组织成一个一个用户
  ns_tcs_ds_def::User getUserInfo();
};

#endif // USERITEM_H
