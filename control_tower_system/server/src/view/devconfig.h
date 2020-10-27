#ifndef DEVCONFIG_H
#define DEVCONFIG_H

#include <QDialog>
#include "devitem.h"
namespace Ui {
class DevConfig;
}

class DevConfig : public QDialog
{
  Q_OBJECT

public:
  explicit DevConfig(QWidget *parent = nullptr);
  ~DevConfig();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_3_clicked();

private:
  Ui::DevConfig *ui;
  DevItem       *_devitem_instance;
};

#endif // DEVCONFIG_H
