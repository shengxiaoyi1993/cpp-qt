#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <QDialog>
#include <QRegularExpressionValidator>

using namespace std;

namespace Ui {
class LoginServer;
}

/// 只是界面
/// 显示
/// 隐藏
/// 接收用户名/密码
/// 退出请求
/// 发出登陆的用户名/密码
class LoginServer : public QDialog
{
  Q_OBJECT

public:
  explicit LoginServer(QWidget *parent = nullptr);
  ~LoginServer();

private:
  Ui::LoginServer *ui;
  QRegularExpressionValidator *_ipValidator;

public slots:

signals:
  void sgl_reqToLogin(string v_serverip,string v_username,string v_password);
  void sgl_reqToQuit();






private slots:
  void on_pushButton_login_clicked();
  void on_pushButton_cancel_clicked();
};

#endif // LOGINSERVER_H
