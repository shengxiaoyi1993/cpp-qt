#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>

class APIClient : public QObject
{
  Q_OBJECT
public:
  explicit APIClient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APICLIENT_H