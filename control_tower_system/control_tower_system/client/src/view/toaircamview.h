#ifndef TOAIRCAMVIEW_H
#define TOAIRCAMVIEW_H

#include <QWidget>

namespace Ui {
class ToAirCamView;
}

class ToAirCamView : public QWidget
{
  Q_OBJECT

public:
  explicit ToAirCamView(QWidget *parent = nullptr);
  ~ToAirCamView();

private:
  Ui::ToAirCamView *ui;
};

#endif // TOAIRCAMVIEW_H
