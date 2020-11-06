#include "tcs_ds_def.h"
#include <iterator>
#include <utility>
#include <vector>
#include <string>




namespace ns_tcs_ds_def {
///================================CamDev==================================///

CamDev::CamDev()
  :_camname(),_ip(),_username(),_password(),_tty(),_type(){

}

CamDev::CamDev(
    const string& v_camname,
    const string& v_ip,
    const string& v_username,
    const string& v_password,
    const string& v_tty,
    CameraType v_type)
  :_camname(v_camname),_ip(v_ip),_username(v_username),
    _password(v_password),_tty(v_tty),_type(v_type){

}

CamDev::CamDev(const neb::CJsonObject& v_json){
  v_json.Get("_camname",_camname);
  v_json.Get("_ip",_ip);
  v_json.Get("_username",_username);
  v_json.Get("_password",_password);
  v_json.Get("_tty",_tty);
  int tmp;
  v_json.Get("_type",tmp);
  _type=static_cast<CameraType>(tmp);
}


neb::CJsonObject CamDev::jsonobj() const{
  neb::CJsonObject  rjsonobj;
  rjsonobj.Add("_camname",_camname);
  rjsonobj.Add("_ip",_ip);
  rjsonobj.Add("_username",_username);
  rjsonobj.Add("_password",_password);
  rjsonobj.Add("_tty",_tty);
  rjsonobj.Add("_type",_type);
  return rjsonobj;
}



///================================CameraArray==================================///
CameraArray::CameraArray(){

}

int CameraArray::addCam(const CamDev& v_dev){
  if(checkRepeat(v_dev)){
    return -1;
  }

  switch (v_dev._type) {
    case CameraType_to_air:{
      air_camera_list.push_back(v_dev);
      break;
    }
    case CameraType_to_ground:{
      ground_camera_list.push_back(v_dev);
      break;
    }
    case CameraType_direct:{
      direct_camera_list.push_back(v_dev);
      break;
    }
    default:
      throw string(__func__)+"zz";
  }
  return 0;
}

CameraArray::CameraArray( neb::CJsonObject& v_json){
  neb::CJsonObject obj_air=v_json["air_camera_list"];
  for (int i=0;i<obj_air.GetArraySize();i++) {
    air_camera_list.push_back(CamDev(obj_air[i]));
  }
  neb::CJsonObject obj_ground=v_json["ground_camera_list"];
  for (int i=0;i<obj_ground.GetArraySize();i++) {
    ground_camera_list.push_back(CamDev(obj_ground[i]));
  }
  neb::CJsonObject obj_direct=v_json["direct_camera_list"];
  for (int i=0;i<obj_direct.GetArraySize();i++) {
    direct_camera_list.push_back(CamDev(obj_direct[i]));
  }
}

bool CameraArray::checkRepeat(const CamDev& v_dev,int v_rpl)  {
  for(vector<CamDev>::iterator it=air_camera_list.begin();
      it!=air_camera_list.end();it++){
    if(v_dev._camname==it->_camname){
      if(v_rpl == 1){*it=v_dev;}
      else if(v_rpl == 2){
        air_camera_list.erase(it);
      }
      return true;
    }
  }

  for(vector<CamDev>::iterator it=ground_camera_list.begin();
      it!=ground_camera_list.end();it++){
    if(v_dev._camname==it->_camname){
      if(v_rpl == 1){*it=v_dev;}
      else if(v_rpl == 2){
        ground_camera_list.erase(it);
      }
      return true;
    }
  }

  for(vector<CamDev>::iterator it=direct_camera_list.begin();
      it!=direct_camera_list.end();it++){
    if(v_dev._camname==it->_camname){
      if(v_rpl == 1){*it=v_dev;}
      else if(v_rpl == 2){
        direct_camera_list.erase(it);
      }
      return true;
    }
  }
  return false;

}

int CameraArray::modifyCam(const CamDev& v_dev){
  if(checkRepeat(v_dev,1)){
    return 0;
  }
  else{
    return -1;
  }
}

int CameraArray::removeCam(const CamDev& v_dev){
  if(checkRepeat(v_dev,2)){
    return 0;
  }
  else{
    return -1;
  }
}




neb::CJsonObject CameraArray::jsonobj() const{
  neb::CJsonObject rrootobj;
  neb::CJsonObject obj_air;
  neb::CJsonObject obj_ground;
  neb::CJsonObject obj_direct;

  for (const CamDev &it: air_camera_list) {
    obj_air.Add(it.jsonobj());
  }

  for (const CamDev &it: ground_camera_list) {
    obj_ground.Add(it.jsonobj());
  }

  for (const CamDev &it: direct_camera_list) {
    obj_direct.Add(it.jsonobj());
  }



  rrootobj.Add("air_camera_list",obj_air);
  rrootobj.Add("ground_camera_list",obj_ground);
  rrootobj.Add("direct_camera_list",obj_direct);

  return rrootobj;
}





int CameraArray::clearAllData(){
  air_camera_list.clear();
  ground_camera_list.clear();
  direct_camera_list.clear();
  return 0;

}






///================================CamCtl==================================///


CamCtl::CamCtl(){
  _camname="";
}

CamCtl::CamCtl(string v_camname,CamCtlFlag v_flag):
  _camname(v_camname),_flag(v_flag)
{
}

CamCtl::CamCtl(neb::CJsonObject &v_json){
  v_json.Get("_camname",_camname);
  int tmp;
  v_json.Get("_flag",tmp);
  _flag=static_cast<CamCtlFlag>(tmp);
}

neb::CJsonObject CamCtl::jsonobj() const{
  neb::CJsonObject  rroot;
  rroot.Add("_camname",_camname);
  rroot.Add("_flag",_flag);
  return rroot;
}



///================================User==================================///

User::User():_username(),_password(),_type(){

}

User::User(neb::CJsonObject &v_json){
  v_json.Get("_username",_username);
  v_json.Get("_password",_password);
  int tmp;
  v_json.Get("_type",tmp);
  _type=static_cast<UserType>(tmp);
  neb::CJsonObject  obj_cams=v_json["_cams"];
  for (int i=0;i<obj_cams.GetArraySize();i++) {
    _cams.push_back(CamCtl(obj_cams[i]));
  }
}

User::User(const string &v_username,
           const string &v_password,
           const UserType &v_type,
           const std::vector<CamCtl> &v_cams):
  _username(v_username),_password(v_password),_type(v_type),_cams(v_cams)
{


}

neb::CJsonObject User::jsonobj() const{
  neb::CJsonObject  rroot;
  rroot.Add("_username",_username);
  rroot.Add("_password",_password);
  rroot.Add("_type",_type);

  neb::CJsonObject  obj_cams;
  for(auto it:_cams){
    obj_cams.Add(it.jsonobj());
  }

  rroot.Add("_cams",obj_cams);

  return rroot;
}



///================================NvrDev==================================///



NvrDev::NvrDev():_ip(),_username(),_password(){

}

NvrDev::NvrDev(  string v_ip,  string v_username,  string v_password)
  :_ip(v_ip),_username(v_username),_password(v_password)
{

}

NvrDev::NvrDev(neb::CJsonObject &v_json){
  v_json.Get("_username",_username);
  v_json.Get("_password",_password);
  v_json.Get("_ip",_ip);
}

neb::CJsonObject NvrDev::jsonobj() const{
  neb::CJsonObject  rroot;
  rroot.Add("_username",_username);
  rroot.Add("_password",_password);
  rroot.Add("_ip",_ip);
  return rroot;
}

int NvrDev::setEmpty(){
  _ip="";
  _username="";
  _password="";
  return 0;
}



///================================CtlInfo==================================///

neb::CJsonObject CtlInfo::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_req_type",_req_type);
  return obj_root;
}
CtlInfo::CtlInfo( const neb::CJsonObject& v_json){
  int tmp;
  v_json.Get("_req_type",tmp);
  _req_type=static_cast<ENUM_REQ_TYPE>(tmp);
}

///================================AuthPara==================================///

neb::CJsonObject AuthPara::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_username",_username);
  obj_root.Add("_password",_password);
  return obj_root;

}
AuthPara::AuthPara( const neb::CJsonObject& v_json){
  v_json.Get("_username",_username);
  v_json.Get("_password",_password);

}


///================================Point==================================///

neb::CJsonObject Point::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_x",_x);
  obj_root.Add("_y",_y);
  return obj_root;

}
Point::Point( const neb::CJsonObject& v_json){
  v_json.Get("_x",_x);
  v_json.Get("_y",_y);
}


///================================Pad==================================///

neb::CJsonObject Pad::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_x",_x);
  obj_root.Add("_y",_y);
  return obj_root;

}
Pad::Pad( const neb::CJsonObject& v_json){
  v_json.Get("_x",_x);
  v_json.Get("_y",_y);
}


///================================PointAndPad==================================///


neb::CJsonObject PointAndPad::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_point",_point.jsonobj());
  obj_root.Add("_pad",_pad.jsonobj());
  return obj_root;
}

PointAndPad::PointAndPad( const neb::CJsonObject& v_json){
  _point=Point(neb::CJsonObject(v_json("_point")));
  _pad=Pad(neb::CJsonObject(v_json("_pad")));
}

///================================DirectCamOp==================================///


neb::CJsonObject DirectCamOp::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_op",_op.jsonobj());
  obj_root.Add("_camname",_camname);
  return obj_root;
}

DirectCamOp::DirectCamOp( const neb::CJsonObject& v_json):
  _op(PointAndPad(neb::CJsonObject(v_json("_op")))),
  _camname(v_json("_camname"))
{
}


///================================TimeRecord==================================///

neb::CJsonObject TimeRecord::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_time_start",_time_start);
  obj_root.Add("_time_cur",_time_cur);
  obj_root.Add("_time_interval",_time_interval);
  return obj_root;
}

TimeRecord::TimeRecord( const neb::CJsonObject& v_json){
  v_json.Get("_time_start",_time_start);
  v_json.Get("_time_cur",_time_cur);
  v_json.Get("_time_interval",_time_interval);
}

///================================UserStatus==================================///


neb::CJsonObject UserStatus::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_username",_username);
  obj_root.Add("_status",_status);
  return obj_root;
}

UserStatus::UserStatus( const neb::CJsonObject& v_json){
  v_json.Get("_username",_username);
  v_json.Get("_status",_status);
}


///================================UserStatus==================================///


neb::CJsonObject CamStatus::jsonobj() const{
  neb::CJsonObject obj_root;
  obj_root.Add("_username",_camname);
  obj_root.Add("_status",_status);
  return obj_root;
}

CamStatus::CamStatus( const neb::CJsonObject& v_json){
  v_json.Get("_username",_camname);
  v_json.Get("_status",_status);
}


}///end of ns_ds_def




