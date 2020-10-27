#ifndef LOGINCLIENT_H
#define LOGINCLIENT_H

#include <QDialog>

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
private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
};

#endif // LOGINCLIENT_H
