#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QObject>

class ClientApi : public QObject
{
  Q_OBJECT
public:
  explicit ClientApi(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CLIENTAPI_H