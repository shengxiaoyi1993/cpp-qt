#ifndef LOGINCLIENT_H
#define LOGINCLIENT_H

#include <QDialog>
#include <string>

using namespace std;
namespace Ui {
class LoginClient;
}

class LoginClient : public QDialog
{
  Q_OBJECT

public:
  explicit LoginClient(QWidget *parent = nullptr);
  ~LoginClient();

private:
  Ui::LoginClient *ui;

signals:
  void signal_login();
  void sgl_reqToLogin(string v_serverip,string v_username,string v_password);
  void sgl_reqToQuit();
private slots:
  void on_pushButton_login_clicked();
  void on_pushButton_cancel_clicked();
};

#endif // LOGINCLIENT_H
