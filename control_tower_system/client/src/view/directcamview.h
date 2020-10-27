#ifndef DIRECTCAMVIEW_H
#define DIRECTCAMVIEW_H

#include <QWidget>

namespace Ui {
class DirectCamView;
}

class DirectCamView : public QWidget
{
  Q_OBJECT

public:
  explicit DirectCamView(QWidget *parent = nullptr);
  ~DirectCamView();

private:
  Ui::DirectCamView *ui;
};

#endif // DIRECTCAMVIEW_H
