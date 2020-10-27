#ifndef TOGROUDCAMVIEW_H
#define TOGROUDCAMVIEW_H

#include <QWidget>
#include <QPaintEvent>

namespace Ui {
class ToGroudCamView;
}

class ToGroudCamView : public QWidget
{
  Q_OBJECT

public:
  explicit ToGroudCamView(QWidget *parent = nullptr);
  ~ToGroudCamView();

private:
  Ui::ToGroudCamView *ui;


  void paintEvent(QPaintEvent* v_event);
};

#endif // TOGROUDCAMVIEW_H
