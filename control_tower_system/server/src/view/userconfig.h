#ifndef USERCONFIG_H
#define USERCONFIG_H

#include <QDialog>
#include "useritem.h"

namespace Ui {
class UserConfig;
}

class UserConfig : public QDialog
{
  Q_OBJECT

public:
  explicit UserConfig(QWidget *parent = nullptr);
  ~UserConfig();

private slots:
  void on_pushButton_3_clicked();

  void on_pushButton_clicked();

private:
  Ui::UserConfig *ui;
  UserItem *_useritem_instance;

};

#endif // USERCONFIG_H
