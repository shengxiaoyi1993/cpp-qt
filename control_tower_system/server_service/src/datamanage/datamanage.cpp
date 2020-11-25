#include "datamanage.h"

#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include "../../../common_lib/third_party_lib/cjsonobject/CJsonObject.hpp"
#include "../../../common_component/ds_def/tcs_ds_def.h"



namespace DataManage {



///================================DataColl==================================///



DataColl::DataColl(){
    createDefaultConfig("");
}

DataColl::DataColl(string v_path):
  _path_save(v_path)
{
  ifstream input(_path_save);
  if(input.is_open()){
    std::string str((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
    neb::CJsonObject root(str);
    loadJsonobj(root);
  }
  else{
      createDefaultConfig(_path_save);
  }

}

int DataColl::createDefaultConfig(const string &v_path){
    _users.push_back(User("admin","admin",UserType_admin,{}));
    if (v_path == "") {
        _path_save="default.config";
    }
    else {
        _path_save=v_path;
    }
    saveToFile();

}


DataColl::~DataColl(){
  saveToFile();

}

bool DataColl::checkRepeatUser(const User& v_user,int v_rpl){
  bool flag_ret=false;
  for(vector<User>::iterator it=_users.begin();
      it!=_users.end();it++){
    if(v_user._username==it->_username){
      if(v_rpl == 1){*it=v_user;}
      else if(v_rpl == 2){
        _users.erase(it);
      }
      flag_ret= true;
    }
  }

  saveToFile();

  return flag_ret;
}


/// 对内部数据进行修改的接口
int DataColl::addUser(const User& v_user){

  if(checkRepeatUser(v_user) == false){
    _users.push_back(v_user);
    saveToFile();
    return 0;
  }
  else{
    return -1;
  }
}

int DataColl::addCam(const CamDev& v_dev){
  _camarray.addCam(v_dev);

  /// 给所有用户加上该设备的权限
  for (auto it=_users.begin();it!=_users.end();it++) {
    auto pos_find=find_if(it->_cams.begin(),it->_cams.end(),[=](CamCtl v_ctl){
      return (v_ctl._camname==v_dev._camname);
    });
    if(pos_find == it->_cams.end()){
      if (it->_type != ns_tcs_ds_def::UserType_ordinary) {
        it->_cams.push_back(CamCtl(v_dev._camname,CamCtlFlag_modify,v_dev._type));
      }
      else{
        it->_cams.push_back(CamCtl(v_dev._camname,CamCtlFlag_noright,v_dev._type));
      }
    }

  }
  saveToFile();
  return 0;
}

int DataColl::modifyCam(const CamDev& v_dev){
   int flag=_camarray.modifyCam(v_dev);
   saveToFile();
   return flag;
}

int DataColl::modifyUser(const User& v_user){
  if(checkRepeatUser(v_user,1) == true){
    saveToFile();
    return 0;
  }
  else{
    return -1;
  }
}

int DataColl::delUser(const User& v_user){
  if(checkRepeatUser(v_user,2) == true){
    saveToFile();
    return 0;
  }
  else{
    return -1;
  }
}

/// 删除设备时为保证数据一致性，某一设备删除成功后需要在各个用户的设备控制列表下删除
int DataColl::delCam(const CamDev& v_dev){
  /// 给所有用户加上该设备的权限

  int flag=_camarray.removeCam(v_dev);

  if (flag == 0) {
    for (auto it=_users.begin();it!=_users.end();it++) {
      auto pos_find=find_if(it->_cams.begin(),it->_cams.end(),[=](CamCtl v_ctl){
        return (v_ctl._camname==v_dev._camname);
      });
      if(pos_find != it->_cams.end()){
        it->_cams.erase(pos_find);
      }
    }
  }
  saveToFile();
  return flag;
}

int DataColl::clearAllData(){
  _camarray.clearAllData();
  _users.clear();
  _nvr.setEmpty();
  saveToFile();
  return 0;
}

int DataColl::clearAllCam(){
   int flag=_camarray.clearAllData();
   saveToFile();
   return flag;

}

int DataColl::clearAllUser(){
  _users.clear();
  saveToFile();
  return 0;
}

int DataColl::setSavePath(const string &v_path){
  _path_save=v_path;
  return 0;

}

int DataColl::saveToFile() const{
  neb::CJsonObject obj_root=jsonobj();
  ofstream ouput(_path_save);
  if(ouput.is_open()){
    ouput<<obj_root.ToFormattedString();
    ouput.flush();
  }

  else{
    throw text_exception("Fail to save data!");
  }
  return 0;

}

/// 字符串处理相关
neb::CJsonObject DataColl::jsonobj() const{
  neb::CJsonObject obj_root;

  neb::CJsonObject obj_user;
  for(uint i=0;i<_users.size();i++){
    obj_user.Add(_users[i].jsonobj());
  }
  obj_root.Add("_users",obj_user);
  obj_root.Add("_camarray",_camarray.jsonobj());
  obj_root.Add("_nvr",_nvr.jsonobj());
  return obj_root;
}


int DataColl::setNVR(const NvrDev &v_nvr) {
  _nvr=v_nvr;
  saveToFile();
  return 0;
}

int DataColl::emptyNVR() {
   int flag=_nvr.setEmpty();
   saveToFile();
   return flag;
}

int DataColl::loadJsonobj(neb::CJsonObject &v_obj ){
  neb::CJsonObject obj_user=v_obj["_users"];
  for (int i=0;i<obj_user.GetArraySize();i++) {
    _users.insert(_users.begin(),User(obj_user[i]));
  }
  _camarray=CameraArray(v_obj["_camarray"]);
  _nvr=NvrDev(v_obj["_nvr"]);
  return 0;
}

};
