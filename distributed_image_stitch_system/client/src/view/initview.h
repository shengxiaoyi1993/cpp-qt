#ifndef INITVIEW_H
#define INITVIEW_H

#include <QDialog>

namespace Ui {
class InitView;
}

class InitView : public QDialog
{
  Q_OBJECT

public:
  explicit InitView(QWidget *parent = nullptr);
  ~InitView();
signals:
  void sgl_on_initConfig(QString v_ip,int v_count);


private slots:
  void on_pushButton_ok_clicked();

  void on_pushButton_cancel_clicked();

private:
  Ui::InitView *ui;
};

#endif // INITVIEW_H
