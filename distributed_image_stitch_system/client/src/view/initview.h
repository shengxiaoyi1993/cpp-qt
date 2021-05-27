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

private:
  Ui::InitView *ui;
};

#endif // INITVIEW_H
