#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

public  slots:
  void on_pushbutton_click();


private:
  QPushButton *_pushbutton;
  QLineEdit   *_linedit;
  QLabel      *_label;


};

#endif // MAINWINDOW_H
