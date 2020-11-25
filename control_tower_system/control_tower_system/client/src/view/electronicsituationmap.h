#ifndef ELECTRONICSITUATIONMAP_H
#define ELECTRONICSITUATIONMAP_H

#include <QWidget>

namespace Ui {
class ElectronicSituationMap;
}

class ElectronicSituationMap : public QWidget
{
  Q_OBJECT

public:
  explicit ElectronicSituationMap(QWidget *parent = nullptr);
  ~ElectronicSituationMap();

private:
  Ui::ElectronicSituationMap *ui;
};

#endif // ELECTRONICSITUATIONMAP_H
