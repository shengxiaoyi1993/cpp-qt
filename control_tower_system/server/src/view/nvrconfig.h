#ifndef NVRCONFIG_H
#define NVRCONFIG_H

#include <QDialog>

namespace Ui {
class NvrConfig;
}

class NvrConfig : public QDialog
{
  Q_OBJECT

public:
  explicit NvrConfig(QWidget *parent = nullptr);
  ~NvrConfig();

private:
  Ui::NvrConfig *ui;
};

#endif // NVRCONFIG_H
