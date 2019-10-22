#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{

}
void MainWindow::downloadFileFromURL(const QString &url, const QString &filePath)
{
    if (!m_isReady)
        return;

    m_isReady = false;

    qDebug() << filePath;
    m_file = new QFile();
    m_file->setFileName(filePath);
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
}

void MainWindow::onDownloadFileComplete(QNetworkReply *reply)
{
    if (!m_file->isWritable()) {
        m_isReady = true;
        return;
    }

    m_file->write(reply->readAll());
    m_file->close();
    m_isReady = true;
}
