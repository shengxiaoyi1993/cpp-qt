#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../view/loginclient.h"
#include "../view/toaircamview.h"
#include "../view/togroudcamview.h"
#include "../view/directcamview.h"
#include "../view/electronicsituationmap.h"
#include "../view/nvrreview.h"



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
  void on_pushButton_6_clicked();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

private:
  Ui::MainWindow *ui;
  LoginClient    *_loginview;
  ToAirCamView  * _view_toair;
  ToGroudCamView *_view_togroud;
  DirectCamView *_view_direct;
  ElectronicSituationMap * _view_esmap;
  NvrReview       *_view_nvr;

};

#endif // MAINWINDOW_H
