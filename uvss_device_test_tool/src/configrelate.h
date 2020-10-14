#ifndef CONFIGRELATE_H
#define CONFIGRELATE_H

#include <QString>
#include <QList>
#include <QPair>
#include "cjsonobject/CJsonObject.hpp"


typedef struct TaskParam{
  QString _name;
  QString _file_input;
  QString _file_output;
  QList<QString> _input_params;
  QList<QString> _output_params;
}TaskParam;


///测试设备使用的参数配置数据结构
typedef struct TaskConfig{
  QList<TaskParam> _defined_params;
  QList<QString> _tasks_to_do;
}TaskConfig;

typedef struct FinalResult{
  int _total;
  int _normal;
  QList<QMap<QString,QString>> _entries;
}FinalResult;


int loadTaskConfigFromFile(const QString& v_file,TaskConfig& v_taskconfig);

QString printTaskConfig(const TaskConfig& v_taskconfig);


/// 参数的key-value对列表，保存在"平"
int getParamsFromFile(const QString& v_file,
                      const QString& v_itemname,
                      QList<QString> v_keys,
                      QList<QMap<QString, QString>> &v_params);

//{
//  "params":{
//  ...
//  }
//}
int saveParamsToFile(const QString& v_file,
                     const QString& v_itemname,
                     const QList<QMap<QString, QString>> &v_params);

int transformParamsToJson(const QString& v_itemname,
                     const QList<QMap<QString, QString>> &v_params,
                     neb::CJsonObject & v_jsonobj
                     );


int saveResultOutput(const QString v_file,
                     const QMap<QString, QList<QMap<QString, QString> > > &v_ouput);


///从详细结果中得到统计结果
int getStatisticalCopy(const QMap<QString, QList<QMap<QString, QString>>> v_res,FinalResult& v_fres);

QString stringFromFinalResult(const FinalResult& v_fres);

int saveFinalResultOutput(const QString v_file,
                     const FinalResult&v_ouput);


#endif // CONFIGRELATE_H
