#ifndef DATAMANAGE_H
#define DATAMANAGE_H

#include <QObject>
#include <string>
#include <cstring>
#include <exception>
#include <utility>
#include "../../lib/cjsonobject/CJsonObject.hpp"
#include "../ds_def/tcs_ds_def.h"


using namespace std;



namespace DataManage {


using namespace ns_tcs_ds_def;



///================================WholeStruct==================================///




typedef struct DataColl{
public:
  ///
  /// \brief DataColl 使用空的数据初始化
  ///
  DataColl();
  ///
  /// \brief DataColl 使用本地缓存文件进行初始化
  /// \param v_path   缓存文件路径
  /// 若无法正常初始化，抛出异常
  ///
  DataColl(string v_path);
  ~DataColl();


  /// 输出成员变量的引用，只读
   std::vector<User>  Users()const{return _users;}
   CameraArray  CamArary()const {return _camarray;}
   NvrDev  NVR()const {return _nvr;}

  /// 对内部数据进行修改的接口
  bool checkRepeatUser(const User& v_user,int v_rol=0);
  int addUser(const User& v_user);
  /// 当添加设备时，需要给所有用户都加上该设备的权限
  int addCam(const CamDev& v_dev);
  int modifyCam(const CamDev& v_dev);
  int modifyUser(const User& v_user);
  int delUser(const User& v_user);
  int delCam(const CamDev& v_dev);
  int clearAllData();
  int clearAllCam();
  int clearAllUser();
  int createDefaultConfig(const string &v_path);

  int setSavePath(const string &v_path);
  ///
  /// \brief saveToFile 一旦数据发生改变，就要将新数据保存导本地
  /// \return
  ///
  int saveToFile() const;

  int setNVR(const NvrDev &v_nvr) ;
  int emptyNVR() ;



  /// 字符串处理相关
  neb::CJsonObject jsonobj() const;
  int loadJsonobj(  neb::CJsonObject &v_obj );


private:
  std::vector<User> _users;
  CameraArray _camarray;
  NvrDev _nvr;

  string _path_save;

}DataColl;

};


#endif // DATAMANAGE_H
