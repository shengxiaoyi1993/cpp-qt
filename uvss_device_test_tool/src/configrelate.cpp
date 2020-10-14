#include "configrelate.h"
#include <QDebug>
#include "cjsonobject/CJsonObject.hpp"



//{
////预先定义的参数列表
//"task_settings":[
//{
//"name":"getip",
//"file_input":"",
//"file_output":"",
//"input_params":[
//"deviceip",
//"username",
//"password",
//"expect_return",
//"errcode",
//"corcode"
//],
//"output_params":[
//"deviceip",
//"username",
//"password",
//"expect_return",
//"errcode",
//"corcode",

//"return",
//"deviceip_return"
//],
////将要执行的任务列表
//"tasks_to_do":[
//{
//  "task":"getip"
//}
//]
//}
//]
//}

int loadTaskConfigFromFile(const QString& v_file,TaskConfig& v_taskconfig){
  QFile f(v_file);
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return -1;
  }
  QString data=f.readAll();
  neb::CJsonObject root(data.toStdString());
  if(root.IsEmpty()){
    return -1;
  }

  if(!root["task_settings"].IsEmpty()){
    if(root["task_settings"].IsArray()){
      int size=root["task_settings"].GetArraySize();
      for(uint i=0;i<size;i++){
        TaskParam taskparam_tmp;

        taskparam_tmp._name=QString::fromStdString(root["task_settings"][i]("name"));
        taskparam_tmp._file_input=QString::fromStdString(root["task_settings"][i]("file_input"));
        taskparam_tmp._file_output=QString::fromStdString(root["task_settings"][i]("file_output"));
        int size_param_input=root["task_settings"][i]["input_params"].GetArraySize();
        for(uint l=0;l<size_param_input;l++){
          taskparam_tmp._input_params.push_back(QString::fromStdString(root["task_settings"][i]["input_params"](l)));
        }

        int size_param_output=root["task_settings"][i]["output_params"].GetArraySize();
        for(uint m=0;m<size_param_output;m++){
          taskparam_tmp._output_params.push_back(QString::fromStdString(root["task_settings"][i]["output_params"](m)));
        }

        v_taskconfig._defined_params.append(taskparam_tmp);

      }
    }
  }
  else{
    throw QString(__FUNCTION__)+" throw Error:task_settings is empty !";
  }


  if(!root["tasks_to_do"].IsEmpty()){
    if(root["tasks_to_do"].IsArray()){
      int size=root["tasks_to_do"].GetArraySize();
      for(uint i=0;i<size;i++){
        v_taskconfig._tasks_to_do.append(QString::fromStdString(root["tasks_to_do"](i)));
      }
    }
  }
  else{
    throw QString(__FUNCTION__)+" throw Error:tasks_to_do is empty !";
  }

  return 0;

}


QString printTaskConfig(const TaskConfig& v_taskconfig){
  neb::CJsonObject root;
  root.AddEmptySubArray("task_settings");
  //  root["task_settings"]
  for(uint i=0;i<v_taskconfig._defined_params.size();i++){
    neb::CJsonObject tempitem;
    tempitem.Add("name",v_taskconfig._defined_params[i]._name.toStdString());
    tempitem.Add("file_input",v_taskconfig._defined_params[i]._file_input.toStdString());
    tempitem.Add("file_output",v_taskconfig._defined_params[i]._file_output.toStdString());
    tempitem.AddEmptySubArray("input_params");
    for(uint j=0;j<v_taskconfig._defined_params[i]._input_params.size();j++){
      tempitem["input_params"].Add(v_taskconfig._defined_params[i]._input_params[j].toStdString());
    }
    tempitem.AddEmptySubArray("output_params");
    for(uint j=0;j<v_taskconfig._defined_params[i]._output_params.size();j++){
      tempitem["output_params"].Add(v_taskconfig._defined_params[i]._output_params[j].toStdString());
    }

    root["task_settings"].Add(tempitem);
  }

  root.AddEmptySubArray("tasks_to_do");
  for(uint i=0;i<v_taskconfig._tasks_to_do.size();i++){
    root["tasks_to_do"].Add(v_taskconfig._tasks_to_do[i].toStdString());
  }
  return QString::fromStdString(root.ToFormattedString());

}


int getParamsFromFile(const QString& v_file,
                      const QString& v_itemname,
                      QList<QString> v_keys,
                      QList<QMap<QString, QString> > &v_params){
  v_params.clear();
  QFile f(v_file);
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return -1;
  }
  QString data=f.readAll();
  neb::CJsonObject root(data.toStdString());

  if(!root.IsEmpty()){
    if(root[v_itemname.toStdString()].IsArray()){

      int size=root[v_itemname.toStdString()].GetArraySize();
      for(uint m=0;m<size;m++){
        QMap<QString, QString> tmpmap;
        for(uint i=0;i<v_keys.size();i++){
          tmpmap[v_keys[i]]=QString::fromStdString(root[v_itemname.toStdString()][m](v_keys[i].toStdString()));
        }
        v_params.push_back(tmpmap);
      }
      return 0;
    }
  }

  return -1;

}

int saveParamsToFile(const QString& v_file,
                     const QString& v_itemname,
                     const QList<QMap<QString, QString>> &v_params){
  neb::CJsonObject root;
  if (transformParamsToJson(v_itemname,v_params,root)!=0){
    return -1;
  }

  QFile f(v_file);
  if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return -1;
  }
  QString data=QString::fromStdString(root.ToFormattedString());
  QTextStream ts(&f);
  ts<<data;
  return 0;
}

int transformParamsToJson(const QString& v_itemname,
                          const QList<QMap<QString, QString>> &v_params,
                          neb::CJsonObject & v_jsonobj
                          ){
  v_jsonobj.Clear();
  v_jsonobj.AddEmptySubArray(v_itemname.toStdString());
  for(uint i=0;i<v_params.size();i++){
    neb::CJsonObject item_tmp;
    for (uint m=0;m<v_params[i].size();m++) {
      item_tmp.Add(v_params[i].keys()[m].toStdString(),v_params[i][v_params[i].keys()[m]].toStdString());
    }
    v_jsonobj[v_itemname.toStdString()].Add(item_tmp);
  }
  return 0;
}

int saveResultOutput(const QString v_file,
                     const QMap<QString,QList<QMap<QString,QString>>>& v_ouput){
  neb::CJsonObject  root;
  root.AddEmptySubArray("Data");

  for(uint i=0;i<v_ouput.size();i++){
    neb::CJsonObject tmpitem;

    tmpitem.Add("api",v_ouput.keys()[i].toStdString());
    neb::CJsonObject tmpsubitem;
    transformParamsToJson("results",v_ouput[v_ouput.keys()[i]],tmpsubitem);
    tmpitem.Add("results",tmpsubitem["results"]);
    root["Data"].Add(tmpitem);
  }

  QFile f(v_file);
  if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return -1;
  }
  QString data=QString::fromStdString(root.ToFormattedString());
  QTextStream ts(&f);
  ts<<data;
  return 0;

}

///逐个分析各项
/// 将每项的数量和正确数量相加，记录出错项（正确与否使用expect_return与return_value比较得到，若相同则正确）
///
int getStatisticalCopy(const QMap<QString, QList<QMap<QString, QString>>> v_res,FinalResult& v_fres){
  v_fres._total=0;
  v_fres._normal=0;

  for(uint i=0;i<v_res.size();i++){
    v_fres._total+=v_res[v_res.keys()[i]].size();
    for(uint j=0;j<v_res[v_res.keys()[i]].size();j++){
      if(v_res[v_res.keys()[i]][j]["expect_return"]==v_res[v_res.keys()[i]][j]["return_value"]){
        v_fres._normal++;
      }
      else{
        QMap<QString,QString> tmpmap;
        tmpmap["name"]=v_res.keys()[i];
        tmpmap["type"]=v_res[v_res.keys()[i]][j]["type"];
        v_fres._entries.push_back(tmpmap);
      }

    }
  }

  return 0;
}

QString stringFromFinalResult(const FinalResult& v_fres){
  neb::CJsonObject  root;
  root.Add("total",v_fres._total);
  root.Add("normal",v_fres._normal);
  root.AddEmptySubArray("exception");

  for (uint i=0;i<v_fres._entries.size();i++) {
    neb::CJsonObject  item;
    item.Add("name",v_fres._entries[i]["name"].toStdString());
    item.Add("type",v_fres._entries[i]["type"].toStdString());
    root["exception"].Add(item);
  }

 return QString::fromStdString(root.ToString()) ;

}



int saveFinalResultOutput(const QString v_file,
                     const FinalResult&v_ouput){

  QFile f(v_file);
  if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return -1;
  }
  QString data=stringFromFinalResult(v_ouput);
  QTextStream ts(&f);
  ts<<data;
  return 0;

}







