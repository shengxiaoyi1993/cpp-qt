#ifndef USERITEM_H
#define USERITEM_H

#include <QDialog>

namespace Ui {
class UserItem;
}

class UserItem : public QDialog
{
  Q_OBJECT

public:
  explicit UserItem(QWidget *parent = nullptr);
  ~UserItem();

private slots:
  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

private:
  Ui::UserItem *ui;
};

#endif // USERITEM_H
