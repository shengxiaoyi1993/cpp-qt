#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QTranslator>
#include<QDebug>
#include<QPushButton>
#include<QGridLayout>
#include<QFile>
#include<QEvent>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void languageChanged(int flag) ;
  QTranslator *qtrans;



private:
  //0 english
  //1 chinese
  int _flag_language=1;
  QLabel* label;
  QPushButton* pushbutton;

protected:
  void changeEvent(QEvent * event);

public slots:
  void on_pushbutton_click();
};

#endif // MAINWINDOW_H
