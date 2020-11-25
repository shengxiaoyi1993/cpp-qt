#ifndef USERCONFIG_H
#define USERCONFIG_H

#include <QDialog>
#include "useritem.h"
#include "ds_def/tcs_ds_def.h"

namespace Ui {
class UserConfig;
}

class UserConfig : public QDialog
{
  Q_OBJECT

public:
  explicit UserConfig(QWidget *parent = nullptr);
  ~UserConfig();
  /**
   * @brief updateUserList 更新表格中用户的信息
   * @param v_users        用户信息列表
   */
  void updateUserList(const vector<ns_tcs_ds_def::User>& v_users);

private slots:
  void on_pushButton_adduser_clicked();
  void on_pushButton_close_clicked();
  void on_useritem_instance_rmOneUser(ns_tcs_ds_def::User v_user);
  void on_useritem_instance_addOneUser(ns_tcs_ds_def::User v_user);
  void on_useritem_instance_modifyOneUser(ns_tcs_ds_def::User v_user);
  void on_item_click(int v_x,int v_y);


  void on_pushButton_rmall_clicked();

signals:
  void sgl_rmAllUser();

  /// 这三个信号是从UserItem传递过来的
  void sgl_rmOneUser( ns_tcs_ds_def::User v_user);
  void sgl_addOneUser(ns_tcs_ds_def::User v_user);
  void sgl_modifyOneUser(ns_tcs_ds_def::User v_user);

private:
  Ui::UserConfig *ui;
  UserItem *_useritem_instance;
  vector<ns_tcs_ds_def::User> _l_users;
};

#endif // USERCONFIG_H
