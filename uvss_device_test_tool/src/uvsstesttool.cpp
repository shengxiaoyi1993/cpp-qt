#include "uvsstesttool.h"
#include <iostream>
#include <thread>
#include <QFuture>
#include <QThread>
#include <QtConcurrent>
#include <QUrlQuery>
#include <algorithm>
#include <QImage>
#include "uvss_api.h"

using namespace std;


const QString UVSSTestTool::_default_path_device_setting="device_setting.config";
const QString UVSSTestTool::_default_path_test_config="resources/test_config.config";
const  QString UVSSTestTool::_default_file_test_output="test_output.output";
const  QString UVSSTestTool::_default_file_test_output_final="test_output_final.output";
static bool g_endflag_upload=false;
static QString g_error_note;
static bool hasbig=false;
static bool hasinfo=false;
static bool hassmall=false;
static QList<QPair<QString,QString>> g_infos;

UVSSTestTool::UVSSTestTool()
{
  _bCheckRunOutTimes=true;
  _nPushImageTimes_activate=2048;
  _path_device_setting=_default_path_device_setting;
  _path_test_config=_default_path_test_config;
  _file_test_output=_default_file_test_output;
  _file_test_output_final=_default_file_test_output_final;
  loadConfig();

}



int UVSSTestTool::start(){
  qDebug()<<"=======================start to check========================";

  /// 逐一读取配置文件，并执行配置文件中列出任务
  /// 每项任务会记录任务执行的结果并保存在本地
  for (uint i=0;i<_taskconfig._tasks_to_do.size();i++) {
    bool bexecute=false;
    for(uint j=0;j<_taskconfig._defined_params.size();j++){
      if(_taskconfig._tasks_to_do[i]==_taskconfig._defined_params[j]._name){
        concludeTask(_taskconfig._defined_params[j]);
        bexecute=true;
        break;
      }

    }
    if(!bexecute)
      throw QString(__FUNCTION__)+" throw Error:task define in config not found!"+_taskconfig._tasks_to_do[i];

  }

  /// 将详细测试结果保存到本地文件
  saveResultOutput(_file_test_output,_result_ouput);

  /// 对详细的结果进行统计，得到直观的测试结果统计
  FinalResult fres;
  getStatisticalCopy(_result_ouput,fres);
  qDebug()<<"test result:"<<stringFromFinalResult(fres);
  saveFinalResultOutput(_file_test_output_final,fres);

  qDebug()<<"=======================end to test========================";
  return 0;
}





int UVSSTestTool::taskDistribute(const QString & v_taskname,
                                 const QMap<QString,QString>& v_input_params,
                                 QMap<QString,QString>&  v_output_params){
  int ret=-1;
  if(v_taskname =="GetUVSSCameraID"){
    ret=test_GetUVSSCameraID(v_input_params,v_output_params);
  }
  else if(v_taskname =="GetUVSSCameraInfo"){
    ret= test_GetUVSSCameraInfo(v_input_params,v_output_params);
  }
  else if(v_taskname =="GetServerIPOfUVSSCamera"){
    ret= test_GetServerIPOfUVSSCamera(v_input_params,v_output_params);
  }
  else if(v_taskname =="ChangeServerIPOfUVSSCamera"){
    ret= test_ChangeServerIPOfUVSSCamera(v_input_params,v_output_params);
  }
  else if(v_taskname =="CheckUVSSCommunityAuthority"){
    ret= test_CheckUVSSCommunityAuthority(v_input_params,v_output_params);
  }
  else if(v_taskname =="RestartUVSSCamera"){
    ret= test_RestartUVSSCamera(v_input_params,v_output_params);
  }
  else if(v_taskname =="uploadimage"){
    if(v_input_params["trigger"]=="request"){
      ret= test_uploadImage(v_input_params,v_output_params,1);

    }
    else{
      ret= test_uploadImage(v_input_params,v_output_params,0);

    }
  }



  return ret;

}



QString UVSSTestTool::getStringInFile(const QString& v_file){
  QFile f(v_file);
  if(!f.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    qDebug() << "Open failed." << endl;
    return "";
  }
  QString data=f.readAll();
  return data;

}



void UVSSTestTool::concludeTask(const TaskParam& v_taskparam){
  /// 1.根据给定的参数格式从任务输入参数表中导入数据
  /// 2.依次用导入的参数列表中的数据执行任务，将输出结果保存到输出文件路径下
  QList<QMap<QString,QString>> list_input_params;
  QList<QMap<QString,QString>> list_output_params;

  if (0==getParamsFromFile(v_taskparam._file_input,"params",v_taskparam._input_params,list_input_params)){

    for(uint ntask=0;ntask<list_input_params.size();ntask++){
      qDebug()<<v_taskparam._name<<":"<<ntask;
      QMap<QString,QString> input_params=list_input_params[ntask];

      qDebug()<<"input_params:"<<input_params;
      QMap<QString,QString> output_params;

      ///由taskDistribute函数进行分发，按照TaskParam中的_name
      int ret_getip=taskDistribute(v_taskparam._name,input_params,output_params);

      ///返回值的比对结果
      if(ret_getip == input_params["expect_return"].toInt()){
        output_params["return_result"]=input_params["corcode"];
      }
      else{
        output_params["return_result"]=input_params["errcode"];
      }

      list_output_params.push_back(output_params);
      qDebug()<<"output_params:"<<output_params;
      _result_ouput[v_taskparam._name].push_back(output_params);
    }
    saveParamsToFile(v_taskparam._file_output,"params",list_output_params);

  }
  else{
    throw QString(__FUNCTION__)+"Error: fail to getParamsFromFile";
  }

}


//"deviceip",
//"username",
//"password",
//"expect_return",
//"errcode",
//"corcode"
int UVSSTestTool::test_GetUVSSCameraID(const QMap<QString,QString>& v_input_params,
                             QMap<QString,QString>&  v_output_params){


  v_output_params=v_input_params;
  QString ip_get;
  int ret_getip=uvss_api::GetUVSSCameraID(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"],
      ip_get
      );


  //返回
  v_output_params["return_value"]=QString::number(ret_getip);
  v_output_params["return_deviceip"]=QString(ip_get);
  return ret_getip;

}




int UVSSTestTool::loadConfig(){
  return loadTaskConfigFromFile(_path_test_config,_taskconfig);
}


//"input_params":[
//"type",
//"deviceip",
//"username",
//"password",
//"expect_return",
//"errcode",
//"corcode"
//],
//"output_params":[
//"type",
//"deviceip",
//"username",
//"password",
//"expect_return",
//"errcode",
//"corcode",
//"return_value",
//"return_delay_times",
//"return_product",
//"return_region",
//"return_remain_number",
//"return_result"
//]
//}
int UVSSTestTool::test_GetUVSSCameraInfo(const QMap<QString,QString>& v_input_params,
                                         QMap<QString,QString>&  v_output_params){
  v_output_params=v_input_params;
  int delay_times;
  int product;
  QString region;
  int remain_number;

  int ret=uvss_api::GetUVSSCameraInfo(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"],
      delay_times,
      product,
      region,
      remain_number
      );


  //返回
  v_output_params["return_value"]=QString::number(ret);
  v_output_params["return_delay_times"]=QString::number(delay_times);
  v_output_params["return_product"]=QString::number(product);
  v_output_params["return_region"]=region;
  v_output_params["return_remain_number"]=QString::number(remain_number);

  return ret;

}

int UVSSTestTool::test_GetServerIPOfUVSSCamera(const QMap<QString, QString> &v_input_params,
                                               QMap<QString, QString> &v_output_params){
  v_output_params=v_input_params;

  QString serverip;

  int ret=uvss_api::GetServerIPOfUVSSCamera(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"],
      serverip
      );


  //返回
  v_output_params["return_value"]=QString::number(ret);
  v_output_params["return_serverip"]=serverip;

  return ret;

}


int UVSSTestTool::test_ChangeServerIPOfUVSSCamera(const QMap<QString, QString> &v_input_params,
                                                  QMap<QString, QString> &v_output_params){
  v_output_params=v_input_params;

  ///获取修改serverip前
  QString serverip_old;
  int ret=uvss_api::GetServerIPOfUVSSCamera(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"],
      serverip_old
      );


  int ret_change=uvss_api::ChangeServerIPOfUVSSCamera(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"],
      v_input_params["serverip_new"]
      );

  QString serverip_new;
  ret=uvss_api::GetServerIPOfUVSSCamera(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"],
      serverip_new
      );


  //返回

  if(serverip_new == v_input_params["serverip_new"]||ret_change!=uvss_api::Normal){
    v_output_params["return_value"]=QString::number(ret_change);
  }
  else{
    v_output_params["return_value"]=QString::number(-1);
    v_output_params["return_note"]="new server ip unexpected";

  }

  return ret;
}


int UVSSTestTool::test_CheckUVSSCommunityAuthority(const QMap<QString, QString> &v_input_params,
                                                   QMap<QString, QString> &v_output_params){
  v_output_params=v_input_params;

  int ret=uvss_api::CheckUVSSCommunityAuthority(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"]
      );

  //返回
  v_output_params["return_value"]=QString::number(ret);
  return ret;
}


int UVSSTestTool::test_RestartUVSSCamera(const QMap<QString, QString> &v_input_params,
                                         QMap<QString, QString> &v_output_params){
  v_output_params=v_input_params;

  int ret=uvss_api::RestartUVSSCamera(v_input_params["deviceip"],
      v_input_params["username"],
      v_input_params["password"]
      );

  ///先需要轮询设备的连接状态，检测设备是否成功关机
  ///  不关机，若等待maxwaittime后仍不关机，则认为执行失败
  ///  关机：检测到第一个获取设备id失败，人为设备停止运行
  ///     关机后开始轮询设备的启动状态
  ///       若等待maxwaittime时间后仍不启动，则认为启动失败
  ///       若正长启动则认为执行成功

  bool hasshutdown=false;
  bool hasrestart=false;

  int  maxwaittime=120;///最长等待时间120s
  int  waittime_shutdown=0;
  int  waittime_start=0;

  ///测试设备是否关机
  while ((waittime_shutdown<maxwaittime)&&(!hasshutdown)&&(ret==uvss_api::Normal)) {
    sleep(1);
    waittime_shutdown+=1;

    QString id_get;
    int ret_getip=uvss_api::GetUVSSCameraID(v_input_params["deviceip"],
        v_input_params["username"],
        v_input_params["password"],
        id_get
        );
    if(ret_getip != uvss_api::Normal){
      hasshutdown=true;
    }
  }


  ///测试设备重新启动
  if(hasshutdown == true){
    while ((waittime_start<maxwaittime)&&!hasrestart) {
      sleep(1);
      waittime_start+=1;
      QString id_get;
      int ret_getip=uvss_api::GetUVSSCameraID(v_input_params["deviceip"],
          v_input_params["username"],
          v_input_params["password"],
          id_get
          );
      if((ret_getip == uvss_api::Normal)&&(id_get!="")){
        hasrestart=true;
      }
    }
  }


  //返回
  if(ret ==uvss_api::Normal){
    if(hasshutdown == true&&hasrestart==true){
      v_output_params["return_value"]=QString::number(ret);

    }
    else if(hasshutdown == true&&hasrestart==false){
      v_output_params["return_value"]=QString::number(-1);
      v_output_params["return_note"]="function return is normal,but device succeeds to shutdown,but fails to start";
    }
    else{
      v_output_params["return_value"]=QString::number(-1);
      v_output_params["return_note"]="function return is normal,but device fails to shutdown";
    }
  }
  else{
    v_output_params["return_value"]=QString::number(ret);
  }
  return ret;
}


///接收到设备发送的消息后，判断消息的类型
/// 期间需要规定设备的状态以避免错误的设备输入影响测试结果
///测试的错误类型
///  1.是否完整接收数据帧
///  2.是否按顺序接收数据帧
///  3.是否正常接收到图片（图片的结果是否正常）
/// 正确的顺序
///   info small big
///   接收到信息帧之后取获取small和big图片，接收到图片后判断图片是否正确
///
int cb_msgFromDev(QString& v_msg){

  qDebug()<<"监测到信息帧："<<v_msg;
  /// 解析数据
  QUrl url(v_msg);
  QUrlQuery qry(url.query());
  g_infos.push_back(QPair<QString,QString>(qry.queryItemValue("type"),
                                           qry.queryItemValue("url").section("/",1,1)));

  //分析数据是否正常


  for(size_t i=0;i<g_infos.size();i++){

    if(g_infos[i].first=="info"){
      hasinfo=true;
    }
    if(g_infos[i].first=="small"){
      hassmall=true;
      if(!hasinfo){
        g_error_note+="small frame recv,lack info frame;";
      }
    }
    if(g_infos[i].first=="big"){
      hasbig=true;
      if(!hasinfo){
        g_error_note+="big frame recv,lack info frame;";
      }
      if(!hassmall){
        g_error_note+="big frame recv,lack small frame;";
      }
    }
  }


  if(hasbig&&hasinfo&&hassmall){

    //重置
    hasbig=false;
    hasinfo=false;
    hassmall=false;
    g_endflag_upload=true;
  }
  return 0;
}

///上图过程中各个接口的测试，将各个接口统一进行测试，只要测试过程可用结果正常即可
/// 在开始上图测试时需要进行设备的初始化测试（修改serverIP，重启，建立监听端口）
/// 手动上图过程
/// 开始监听端口
/// 等待手动上图
/// 监测到上图帧
/// 结束监听端口(停止监听端口)
int UVSSTestTool::test_uploadImage(const QMap<QString, QString> &v_input_params,
                                   QMap<QString, QString> &v_output_params,
                                   int mode
                                   ){
  v_output_params =v_input_params;

  ///一律修改serverIP
  int ret_change=test_ChangeServerIPOfUVSSCamera(v_input_params,v_output_params);
  if(ret_change != uvss_api::Normal){
    return ret_change;
  }

  int ret_restart=test_RestartUVSSCamera(v_input_params,v_output_params);
  if(ret_restart != uvss_api::Normal){
    return ret_restart;
  }


  ///建立监听
  ///
  int ret_listen=uvss_api::BuildPortToWaitInfoFromUVSSCamera(
        v_input_params["port"].toInt(),cb_msgFromDev);
  if(ret_listen != uvss_api::Normal){
    return ret_listen;
  }

  if(mode ==0 ){
    qDebug()<<"提醒用户手动上图:灯熄灭之后输入Enter";
    getchar();
  }
  else{
    qDebug()<<"暂停一段时间后请求设备上图";

    this_thread::sleep_for(std::chrono::seconds(30));
    uvss_api::RequestTestImageFromUVSSCamera(v_input_params["deviceip"],
        v_input_params["username"],
        v_input_params["password"]);
  }



  int ret_upload=-2;
  int waittime_max=240;
  int waittime_record=0;
  ///提醒用户手动上图
  while ((!g_endflag_upload)&&(waittime_record<waittime_max)) {
    this_thread::sleep_for(std::chrono::seconds(1));
    waittime_record+=1;
    QCoreApplication::processEvents();
  }
  g_endflag_upload=false;
  hasbig=false;
  hasinfo=false;
  hassmall=false;

  ///测试接收到的图片是否能够拿到
  for(uint i=0;i<g_infos.size();i++){
    if(g_infos[i].first=="small"){
      int ret_getimage=uvss_api::GetImageFromUVSSCamera(v_input_params["deviceip"],
          v_input_params["username"],
          v_input_params["password"],
          g_infos[i].second,
          g_infos[i].second
          );
      if(ret_getimage == uvss_api::Normal){
        QImage tmptmage(g_infos[i].second);
        if(tmptmage.isNull()){
          g_error_note+="small image is succeed to download,but is broken";
          ret_upload=-1;
        }
        else{
          g_error_note+="small image is succeed to download;";
          ret_upload=(ret_upload==-1)?-1:uvss_api::Normal;

        }
      }
      else{
        g_error_note+="small image is failed to download;";
        ret_upload=-1;
      }

    }
    else if(g_infos[i].first=="big"){
      int ret_getimage=uvss_api::GetImageFromUVSSCamera(v_input_params["deviceip"],
          v_input_params["username"],
          v_input_params["password"],
          g_infos[i].second,
          g_infos[i].second
          );
      if(ret_getimage == uvss_api::Normal){
        QImage tmptmage(g_infos[i].second);
        if(tmptmage.isNull()){
          g_error_note+="big image is succeed to download,but is broken";
          ret_upload=-1;
        }
        else{
          g_error_note+="big image is succeed to download;";
          ret_upload=uvss_api::Normal;
        }
      }
      else{
        g_error_note+="big image is failed to download;";
        ret_upload=-1;
      }
    }
  }
  g_infos.clear();


  uvss_api::StopListenUVSSCamera();

  v_output_params["return_value"]=QString::number(ret_upload);
  v_output_params["return_note"]=g_error_note;
  g_error_note="";
  return ret_upload;

}
