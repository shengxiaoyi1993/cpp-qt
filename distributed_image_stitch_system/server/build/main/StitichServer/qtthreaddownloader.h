#ifndef QTTHREADDOWNLOADER_H
#define QTTHREADDOWNLOADER_H

#include <QObject>

class QtThreadDownLoader : public QObject
{
  Q_OBJECT
public:
  explicit QtThreadDownLoader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QTTHREADDOWNLOADER_H