#include "datamanage.h"

#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include "../../lib/cjsonobject/CJsonObject.hpp"
#include "../ds_def/tcs_ds_def.h"



namespace DataManage {



///================================DataColl==================================///



DataColl::DataColl(){
  _users.push_back(User("admin","admin",UserType_admin,{}));
  _path_save="default.config";
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
    throw text_exception("Fail to open file!");
  }

}

DataColl::~DataColl(){
  saveToFile();

}

bool DataColl::checkRepeatUser(const User& v_user,int v_rpl){
  for(vector<User>::iterator it=_users.begin();
      it!=_users.end();it++){
    if(v_user._username==it->_username){
      if(v_rpl == 1){*it=v_user;}
      else if(v_rpl == 2){
        _users.erase(it);
      }
      return true;
    }
  }

  return false;
}


/// 对内部数据进行修改的接口
int DataColl::addUser(const User& v_user){

  if(checkRepeatUser(v_user) == false){
    _users.push_back(v_user);
    return 0;
  }
  else{
    return -1;
  }
}

int DataColl::addCam(const CamDev& v_dev){
  _camarray.addCam(v_dev);
  return 0;
}

int DataColl::modifyCam(const CamDev& v_dev){
  return _camarray.modifyCam(v_dev);
}

int DataColl::modifyUser(const User& v_user){
  checkRepeatUser(v_user,true);
  return 0;
}

int DataColl::delUser(const User& v_user){
  if(checkRepeatUser(v_user,2) == false){
    return 0;
  }
  else{
    return -1;
  }
}

int DataColl::delCam(const CamDev& v_dev){
  return _camarray.removeCam(v_dev);
}

int DataColl::clearAllData(){
  _camarray.clearAllData();
  _users.clear();
  _nvr.setEmpty();
  return 0;
}

int DataColl::clearAllCam(){
  return _camarray.clearAllData();

}

int DataColl::clearAllUser(){
  _users.clear();
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
  return 0;
}

int DataColl::emptyNVR() {
  return _nvr.setEmpty();
}

int DataColl::loadJsonobj(neb::CJsonObject &v_obj ){
  neb::CJsonObject obj_user=v_obj["_users"];
  for (int i=0;i<obj_user.GetArraySize();i++) {
    _users.insert(_users.begin(),User(obj_user[1]));
  }
  _camarray=CameraArray(v_obj["_camarray"]);
  _nvr=NvrDev(v_obj["_nvr"]);
  return 0;
}

};
