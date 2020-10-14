#ifndef UVSSTESTTOOL_H
#define UVSSTESTTOOL_H
///功能
/// 1. 测试API是否正常
/// 2. 测试设备运行是否正常
/// 以上都是 通过设备状态和API的输出来判断
/// 设备状态由人为控制
/// 在规定设备状态下的API输出即可判断设备和API是否运行正常

#include <string>
#include <vector>
#include <QString>
#include <QList>
#include <QMap>
#include "configrelate.h"


using namespace std;
///是否单独编写设备类

typedef struct UVSSDeviceSetting{
  ///provided by user
  QString _device_ip;
  QString _user_name;
  QString _password;
  UVSSDeviceSetting(){
    _device_ip="";
    _user_name="";
    _password="";
  }

  UVSSDeviceSetting(QString v_device_ip,QString v_user_name,QString v_password):
    _device_ip(v_device_ip),_user_name(v_user_name),_password(v_password)
  {
  }

}UVSSDeviceSetting;



//调用流程
//1. 初始化
//2. 修改参数或者导入配置文件
//3. 检测模块的自检，输入参数符合预期
//4. 检测模块不接入设备进行空转，
//5. 开始测试，按照检测队列中的任务依次进行测试。对需要人为配合的操作给出提示。记录测试结果
//6. 结束测试，输出测试结果
class UVSSTestTool
{
public:
  UVSSTestTool();


  ///开始检测
  int start();

  /// 设备参数获取测试
  /// 传入参数即UVSSDeviceSetting
  /// 需要测试ip,username,password等各个参数的正确与错误的情况
  /// 输入使用文件数据,对于文件中的每一行,均执行一次操作,输入文件中给定输出的正确结果,除了条件外需要给定错误说明
  /// 对于每一台设备都不同的参数输入文件中无法给定固定的值,此处由函数实现具体的判断条件,而不需要输入文件设置
  /// 对于输出结果保存到输出结果文件中,输出结果保留给定的条件,为了以后的拓展性,使用JSON格式；为了避免每一个任务都定义一个数据结构，采用简单的Map方式保存
  /// 输出的统计结果指定指出错误的类型和错误的输入文件和输出文件条目序号
  /// 每一个API接口都需要单独的输入输出文件

  /// 设备参数修改测试
  void test_changeParams();

  /// 上图次数耗尽测试
  void test_RunOutTimesToPushImages();

  /// 当设备被激活时的重复上图
  void test_PushImagesWhenActivate();

  ///测试上传图片的时间间隔
  /// 1. 请求图片到得到info帧
  /// 2. 得到info帧到接收small帧
  /// 3. 接收small帧到接收big帧
  /// 4. 请求小图到接收到小图
  /// 5. 请求大图到接收到大图
  void test_GetIntervalOfPush();

  ///简单测试
  /// GetUVSSCameraInfo
  /// GetUVSSCameraID
  /// GetServerIPOfUVSSCamera
  /// ChangeServerIPOfUVSSCamera  需要将serverip进行恢复，否则不利于后续的操作；验证方法，重启后测试设备能否正常上图；最终需要将设备的参数恢复为出厂状态
  /// CheckUVSSCommunityAuthority

  void concludeTask(const TaskParam &v_taskparam);
  int test_GetUVSSCameraID(const QMap<QString,QString>& v_input_params
                  ,QMap<QString,QString>&  v_output_params);
  int test_GetUVSSCameraInfo(const QMap<QString, QString> &v_input_params,
                             QMap<QString, QString> &v_output_params);
  int test_GetServerIPOfUVSSCamera(const QMap<QString, QString> &v_input_params,
                             QMap<QString, QString> &v_output_params);

  int test_ChangeServerIPOfUVSSCamera(const QMap<QString, QString> &v_input_params,
                                      QMap<QString, QString> &v_output_params);
  int test_CheckUVSSCommunityAuthority(const QMap<QString, QString> &v_input_params,
                                      QMap<QString, QString> &v_output_params);

  /// 需要特殊的验证方法
  /// RestartUVSSCamera
  int test_RestartUVSSCamera(const QMap<QString, QString> &v_input_params,
                             QMap<QString, QString> &v_output_params);

  /// 需要预知设备内部的信息
  /// GetImageFromUVSSCamera
  /// DeleteImageInUVSSCamera
  /// mode==0:by manual mode=1:by hand
  int test_uploadImage(const QMap<QString, QString> &v_input_params,
                               QMap<QString, QString> &v_output_params ,
                               int mode=0);


  ///从`_path_test_config`本地文件载入配置
  /// 每次更改配置文件路径需要执行该函数方能生效
  int loadConfig();


  ///按照标识找到对应地测试函数并执行
  int taskDistribute(const QString & v_taskname,
                      const QMap<QString,QString>& v_input_params,
                      QMap<QString,QString>&  v_output_params);


private:
  QString _path_device_setting;
  QString _path_test_config;

  vector<string> _tasks;

  ///是否测试上图次数耗尽次数测试
  ///若为true,则在上图次数耗尽后请求图片10次
  bool _bCheckRunOutTimes;

  ///在设备激活后测试多次重复上图
  int _nPushImageTimes_activate;

  ///测试过程详细测试结果
  QString _file_test_output;
  ///测试过程中测试总结
  QString _file_test_output_final;

  ///配置文件位置
  ///  设备的配置
  ///  测试流程和测试参数的选择
  const static QString _default_path_device_setting;
  const static QString _default_path_test_config;

  ///默认输出文件路径
  const static QString _default_file_test_output;
  const static QString _default_file_test_output_final;

  ///记录各个任务的参数列表；参数输入的文件和将要执行的任务列表
  TaskConfig  _taskconfig;
  ///该变量用于记录测试结果
  QMap<QString,QList<QMap<QString,QString>> > _result_ouput;


  QString getStringInFile(const QString& v_file);


};

 int cb_msgFromDev(QString& v_msg);

#endif // UVSSTESTTOOL_H




