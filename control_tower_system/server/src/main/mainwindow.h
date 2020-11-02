#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../view/userconfig.h"
#include "../view/loginserver.h"
#include "../view/devconfig.h"
#include "../view/nvrconfig.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_pushButton_3_clicked();

private:
  Ui::MainWindow *ui;
  UserConfig *_userconfig;
  LoginServer*_diag_login;
  DevConfig *_devconfig;
  NvrConfig *_nvrconfig;

};

#endif // MAINWINDOW_H
