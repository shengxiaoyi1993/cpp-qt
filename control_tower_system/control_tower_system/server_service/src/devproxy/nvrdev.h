#ifndef NVRDEV_H
#define NVRDEV_H

#include <QObject>

class NVRDev : public QObject
{
  Q_OBJECT
public:
  explicit NVRDev(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NVRDEV_H