#include "downloader.h"
#include <QDebug>
#include <QEventLoop>

void Downloader::downloadFileFromURL(const QString &url, const QString &filePath)
{
  if (!m_isReady)
    return;
  m_isReady = false;

  const QString fileName = filePath;
  qDebug() << fileName;
  m_file = new QFile();
  m_file->setFileName(fileName);
  m_file->open(QIODevice::WriteOnly);
  if (!m_file->isOpen()) {
    m_isReady = true;
    return;
  }

  QNetworkAccessManager *manager = new QNetworkAccessManager;

  QNetworkRequest request;
  request.setUrl(QUrl(url));

  connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onDownloadFileComplete(QNetworkReply *)));

  manager->get(request);
  qDebug()<<"downloadFileFromURL end"<<endl;
}

void Downloader::onDownloadFileComplete(QNetworkReply *reply)
{
  if (!m_file->isWritable()) {
    m_isReady = true;
    return;
  }

  m_file->write(reply->readAll());
  m_file->close();
  m_isReady = true;
  qDebug()<<"onDownloadFileComplete end"<<endl;

}

bool downloadURL(const QString &url, const QString &fileName)
{
  QNetworkAccessManager manager;
  QNetworkRequest request;
  request.setUrl(url);
  QNetworkReply *reply = manager.get(request);

  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if (reply->error() != QNetworkReply::NoError)
  {
    qDebug()<<"connect error"<<endl;

    return false;
  }

  QFile f(fileName);
  qDebug() << fileName;
  if(!f.open(QIODevice::WriteOnly)){
    qDebug()<<"can not open file"<<endl;
    qDebug()<<false<<endl;
    return false;
  }
  f.write(reply->readAll());
  f.close();
  delete reply;
  return true;
}
