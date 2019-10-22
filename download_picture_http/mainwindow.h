#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QEventLoop>
#include<QNetworkAccessManager>
#include<QUrl>
#include<QByteArray>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QVariant>
#include<QString>
#include<QUrlQuery>
#include<QFile>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void downloadFileFromURL(const QString &url, const QString &filePath);

private slots:
    void onDownloadFileComplete(QNetworkReply *reply);

private:
    QFile *m_file;
    bool m_isReady;
};


#endif // MAINWINDOW_H
