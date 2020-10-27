#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <QDialog>

namespace Ui {
class LoginServer;
}

class LoginServer : public QDialog
{
  Q_OBJECT

public:
  explicit LoginServer(QWidget *parent = nullptr);
  ~LoginServer();

private:
  Ui::LoginServer *ui;

public slots:

signals:
  void signal_Login();




private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
};

#endif // LOGINSERVER_H
