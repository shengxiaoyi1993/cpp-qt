#ifndef GLOBAL_TOOL_FUNC_H
#define GLOBAL_TOOL_FUNC_H
/*
 *all the tool functions will declaration in this file and define in .cpp file.
 *tool functions will be called whenever and wherever you want to call it.
 * as tools for other function, help to process some works which are the same or alike.
*/

#include "constants.h"
#include <QString>
#include <QJsonDocument>
#include <QDate>

static QDate last_data;

//=====================================help func=========================================

void systemLog(QString &info);

//====================================file process=======================================

bool readJsonFileToQJsonDocument(const QString &file_path, QJsonDocument &doc);

bool writeJsonFileFromQJsonDocument(const QString &file_path, const QJsonDocument &doc);

//================================data type process======================================

int transfromNumberFromStringToInt(const QString &str);

QString transfromNumberFromIntToString(int num);

QJsonObject tranformStringToObject(QString &str);

//====================================data gain==========================================

QStringList CurrentPCAllIPS();

QString getCurrentPath();

QString getApplicationDirPath();

//==================================condition judge======================================
bool isFileExists(QString &file_path);



#endif // GLOBAL_TOOL_FUNC_H
