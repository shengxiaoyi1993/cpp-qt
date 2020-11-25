#ifndef NVRREVIEW_H
#define NVRREVIEW_H

#include <QWidget>

namespace Ui {
class NvrReview;
}

class NvrReview : public QWidget
{
  Q_OBJECT

public:
  explicit NvrReview(QWidget *parent = nullptr);
  ~NvrReview();

private:
  Ui::NvrReview *ui;
};

#endif // NVRREVIEW_H
