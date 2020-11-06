#include "global_tool_func.h"

#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QApplication>
#include <QDir>
#include <QTime>
#include <stdlib.h>
#include <QJsonObject>

void systemLog(QString &info) {
    if(log_config::LOG_TYPE == log_config::MODE_CMD) {
        qDebug() << info << endl;
    }
    else if(log_config::LOG_TYPE == log_config::MODE_FILE) {
        QFile file(log_config::DEFAULT_LOG_FILE_PATH);
        QString err_info;
        if(file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)) {
            QDate date = QDate::currentDate();
            if(last_data.isNull()) {
                last_data = date;
                std::string data = QString("===================DATE: " + date.toString("dd.MM.yyyy") + "=====================\n").toStdString();
                file.write(data.data(), static_cast<int>(strlen(data.data())));
            }
            else if(last_data == date) {
                last_data = date;
                std::string data = QString("===================DATE: " + date.toString("dd.MM.yyyy") + "=====================\n").toStdString();
                file.write(data.data(), static_cast<int>(strlen(data.data())));
            }
            QTime time = QTime::currentTime();
            std::string data = QString(time.toString("hh:mm:ss") + " " + info + "\n").toStdString();
            file.write(data.data(), static_cast<int>(strlen(data.data())));
            file.close();
        }
    }
    else {
        qDebug() << info << endl;
    }
}

bool readJsonFileToQJsonDocument(const QString &file_path, QJsonDocument &doc) {
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString info = "READ_FILE_ERROR: " + file.fileName() + " open fail";
        systemLog(info);
        return false;
    }
    QByteArray buff = file.readAll();
    file.close();

    QJsonParseError err;
    doc = QJsonDocument::fromJson(buff, &err);
    if(doc.isNull() == true) {
        QString info = "JSON_PARSE_ERROR: " + err.errorString();
        systemLog(info);
        return false;
    }
    if(doc.isEmpty() == true) {
        QString info = "JSON_FILE_EMPTY: " + file_path + "is a empty json file";
        systemLog(info);
        return false;
    }
    return true;
}

//modify file will read file, clear source file, modify file and rewrite file?
bool writeJsonFileFromQJsonDocument(const QString &file_path, const QJsonDocument &doc) {
    QFile file(file_path);
    QString info;
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        info = "WRITE_FILE_ERROR: " + file_path + " open fail";
        systemLog(info);
        return false;
    }
    QByteArray byte_array = doc.toJson();
    qint64 num = file.write(byte_array);
    if(num == -1) {
        info = "WRITE_FILE_ERROR: " + file_path + " write fail";
        systemLog(info);
        file.close();
        return false;
    }
    file.close();
    return true;
}

int transfromNumberFromStringToInt(const QString &str) {
    QString num_str = "0123456789";
    int sum = 0;
    for(int i = 0; i < str.size(); i++) {
        int num = 0;
        for(int j = 0; j < num_str.size(); j++) {
            if(str.at(i) == num_str.at(j)) {
                num = j;
                break;
            }
        }
        sum = sum*10 + num;
    }
    return sum;
}

QString transfromNumberFromIntToString(int num) {
    QString num_str = "0123456789";
    QString str;
    while(num > 0) {
        int single_num = num%10;
        num = (num - single_num)/10;
        str.insert(0, num_str[single_num]);
    }
    return str;
}

QStringList CurrentPCAllIPS() {
  QStringList result;
  QList<QHostAddress> list = QNetworkInterface::allAddresses();
  foreach (QHostAddress address, list) {
    if (address.protocol() == QAbstractSocket::IPv4Protocol)
      result << address.toString();
  }
  return result;
}

QString getApplicationDirPath() {
    return QApplication::applicationDirPath();
}

QString getCurrentPath() {
    return QDir::currentPath();
}

bool isFileExists(QString &file_path) {
    QFile file(file_path);
    return file.exists();
}

QJsonObject tranformStringToObject(QString &str) {
    QJsonObject obj;
    QByteArray buff = QByteArray::fromStdString(str.toStdString());
    QJsonDocument doc = QJsonDocument::fromJson(buff);
    if (doc.isObject()) {
      obj = doc.object();
    }
    return obj;
}
