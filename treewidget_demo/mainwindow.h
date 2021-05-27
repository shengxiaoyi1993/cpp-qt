#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTreeWidget>
#include <QMenu>
#include <QMessageBox>
#include "comboboxdelegate.h"
#include "closebutton.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QDialog  __dialog_test;
  QMenu __menu_right_click;

  QAction __action_rm;
  QString __string_item;
  ComboBoxDelegate __boxdelegate;




public slots:
//  void slt_dialog_test_show();

private slots:
 void on_treeWidget_customContextMenuRequested(const QPoint &pos);
  void slt_on___action_rm_trigger();

};

#endif // MAINWINDOW_H
