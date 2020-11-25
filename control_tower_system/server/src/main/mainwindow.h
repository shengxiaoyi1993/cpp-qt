#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <vector>
#include "../view/userconfig.h"
#include "../view/loginserver.h"
#include "../view/devconfig.h"
#include "../view/nvrconfig.h"
#include "ds_def/tcs_ds_def.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QAction* action_userconfig();
  QAction* action_devconfig();
  QAction* action_nvrconfig();
  void pushbackLog(const string &v_log);
  bool autoLauchStaus()const;

  ///
  /// \brief uopdateServerStaus 更改服务器状态栏的内容，若传入的参数为默认值该项不予修改
  /// \param v_serverip  默认值""
  /// \param v_status    默认值-1，0：运行出错 1：运行正常
  /// \param v_lasttime  默认值<0, server-service 持续运行的时间
  /// \param v_user      默认值-1, 用户登陆的数量
  ///
  void uopdateServerStaus(const string &v_serverip,
                          int v_status,
                          int v_lasttime,
                          int v_user);


  ///
  /// \brief updateUserTatusTable  更新设备状态表中的，注意各个列表中的元素数量应该相同
  /// \param l_user
  /// \param l_type
  /// \param l_status
  /// \param l_aircam
  /// \param l_groundcam
  /// \param l_directcam
  ///
  void updateUserTatusTable(const vector<string> &l_user,
                            const vector<string>&l_type,
                            const vector<bool>&l_status,
                            const vector<int>&l_aircam,
                            const vector<int>&l_groundcam,
                            const vector<int>&l_directcam
                            );



signals:
  void sgl_setAutoLauch(bool v_flag);
  void sgl_stopService();
  void sgl_exitGUI();

private slots:
  void on_pushButton_cancel_clicked();
  void on_pushButton_start_clicked();
private:
  Ui::MainWindow *ui;
  bool _status_autorestart;



};

#endif // MAINWINDOW_H
