#ifndef SERVERAPI_H
#define SERVERAPI_H

#include <QObject>

class ServerApi : public QObject
{
  Q_OBJECT
public:
  explicit ServerApi(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERVERAPI_H