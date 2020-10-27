#ifndef DEVITEM_H
#define DEVITEM_H

#include <QDialog>

namespace Ui {
class DevItem;
}

class DevItem : public QDialog
{
  Q_OBJECT

public:
  explicit DevItem(QWidget *parent = nullptr);
  ~DevItem();

private slots:
  void on_pushButton_4_clicked();

private:
  Ui::DevItem *ui;
};

#endif // DEVITEM_H
