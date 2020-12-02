#ifndef TCS_DS_DEF_H
#define TCS_DS_DEF_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include "cjsonobject/CJsonObject.hpp"



using namespace std;

class text_exception : public std::exception {
private:
    char* text_;
public:
    text_exception(const char* text) {
        text_ = new char[std::strlen(text) + 1];
        std::strcpy(text_, text);
    }
    text_exception(const text_exception& e) {
        text_ = new char[std::strlen(e.text_) + 1];
        std::strcpy(text_, e.text_);
    }

    ~text_exception() noexcept /*throw()*/ {
        delete[] text_;
    }
    const char* what() const noexcept /*noexcept*/ override { return text_; }

};

class StringException:public std::exception{
private:
    string _string;
public:
    StringException(const string &v_str):_string(v_str){

    }

    StringException(const StringException &v_obj):_string(v_obj._string){

    }

    ~StringException() noexcept /*throw()*/ {
    }

    const char* what() const noexcept /*noexcept*/ override { return _string.c_str(); }

};




namespace ns_tcs_ds_def {


typedef struct AuthPara{
    AuthPara( const string &v_username,const string &v_password):_username(v_username),_password(v_password){}
    neb::CJsonObject jsonobj() const;
    AuthPara( const neb::CJsonObject& v_json);


    string _username;
    string _password;
}AuthPara;

typedef struct Point{
    Point():_x(0),_y(0){}
    Point(int v_x,int v_y):_x(v_x),_y(v_y){}
    neb::CJsonObject jsonobj() const;
    Point( const neb::CJsonObject& v_json);

    int _x;
    int _y;
}Point;

typedef struct Pad{
    Pad():_x(0),_y(0){}
    Pad(int v_x,int v_y):_x(v_x),_y(v_y){}
    neb::CJsonObject jsonobj() const;

    Pad( const neb::CJsonObject& v_json);
    Pad operator +(const Pad& v_cam)//赋值运算符
    {
        Pad tmp(0,0);
        tmp._x=v_cam._x+this->_x;
        tmp._y=v_cam._y+this->_y;
        return tmp;
    }

    int _x;
    int _y;
}Pad;



typedef struct PointAndPad{
    PointAndPad(const Point&v_point,const Pad&v_pad):_point(v_point),_pad(v_pad){}
    PointAndPad(){}
    neb::CJsonObject jsonobj() const;
    PointAndPad(neb::CJsonObject v_json);

    Point _point;
    Pad _pad;
}PointAndPad;


typedef struct DirectCamOp{
    DirectCamOp(const string& v_camname,const PointAndPad v_op):
        _op(v_op),_camname(v_camname){}
    DirectCamOp(){

    }
    neb::CJsonObject jsonobj() const;
    DirectCamOp(neb::CJsonObject v_json);

    DirectCamOp& operator =(const DirectCamOp& v_cam)//赋值运算符
    {
        if (this != &v_cam)
        {
            _camname=v_cam._camname;
            _op=v_cam._op;
        }
        return *this;
    }

    DirectCamOp(const DirectCamOp &v_data){
        if (this != &v_data)
        {
            _camname=v_data._camname;
            _op=v_data._op;
        }
    }

    PointAndPad _op;
    string _camname;

}DirectCamOp;


typedef struct TimeRecord{
    TimeRecord():_time_start(0),_time_cur(0),_time_interval(0){}
    TimeRecord(unsigned v_time_start,unsigned v_time_cur,unsigned v_time_interval):
        _time_start(v_time_start),_time_cur(v_time_cur),_time_interval(v_time_interval){}
    neb::CJsonObject jsonobj() const;
    TimeRecord( const neb::CJsonObject& v_json);

    long _time_start;
    long _time_cur;
    long _time_interval;
}TimeRecord;




typedef struct UserStatus{
    UserStatus(const string&v_username,bool v_status):_username(v_username),_status(v_status){}
    neb::CJsonObject jsonobj() const;
    UserStatus( const neb::CJsonObject& v_json);

    string _username;
    bool   _status;
}UserStatus;



typedef struct CamStatus{
    CamStatus(const string&v_camname,bool v_status):_camname(v_camname),_status(v_status){}
    neb::CJsonObject jsonobj() const;
    CamStatus( const neb::CJsonObject& v_json);

    string _camname;
    bool _status;
}CamStatus;



///================================DevStruct==================================///

enum CameraType{CameraType_to_air,CameraType_to_ground,CameraType_direct};

typedef struct CamDev{
    CamDev();
    CamDev(const string& v_camname):_camname(v_camname){}

    CamDev(
            const string& v_camname,
            const string& v_ip,
            const string& v_username,
            const string& v_password,
            const string& v_tty,
            CameraType v_type

            );
    CamDev(const neb::CJsonObject& v_json);
    neb::CJsonObject jsonobj() const;
    CamDev& operator =(const CamDev& v_cam)//赋值运算符
    {
        if (this != &v_cam)
        {
            _camname=v_cam._camname;
            _ip=v_cam._ip;
            _username=v_cam._username;
            _password=v_cam._password;
            _tty=v_cam._tty;
            _type=v_cam._type;
        }
        return *this;
    }

    string _camname;
    string _ip;
    string _username;
    string _password;
    string _tty;
    CameraType _type;

}CamDev;

typedef struct CameraArray{
public:
    CameraArray();
    CameraArray(neb::CJsonObject &v_json);
    /// 检测没有相同_camname的设备，就在响应队列插入
    int addCam(const CamDev& v_dev);

    ///
    /// \brief modifyCam 若有重名的，对重名设备进行修改
    /// \param v_dev
    /// \return
    ///
    int modifyCam(const CamDev& v_dev);
    int removeCam(const CamDev& v_dev);
    int findCam(const string &v_camname, CamDev &v_cam) const;


    ///
    /// \brief checkRepeat 检查是否有相同_camname的设备
    /// \param v_dev
    /// \param v_rpl       0 不做处理；1：替换；2：删除
    /// \return            若有重复的返回0
    ///
    bool checkRepeat(const CamDev& v_dev,int v_rpl=0);
    int clearAllData();
    neb::CJsonObject jsonobj() const;
    CameraArray& operator =(const CameraArray& v_camarray)//赋值运算符
    {
        if (this != &v_camarray)
        {
            air_camera_list.resize(v_camarray.air_camera_list.size());
            ground_camera_list.resize(v_camarray.ground_camera_list.size());
            direct_camera_list.resize(v_camarray.direct_camera_list.size());
            copy(v_camarray.air_camera_list.begin(),v_camarray.air_camera_list.end(),
                 air_camera_list.begin());
            copy(v_camarray.ground_camera_list.begin(),v_camarray.ground_camera_list.end(),
                 ground_camera_list.begin());
            copy(v_camarray.direct_camera_list.begin(),v_camarray.direct_camera_list.end(),
                 direct_camera_list.begin());
        }
        return *this;
    }

    std::vector<CamDev> air_camera_list;
    std::vector<CamDev> ground_camera_list;
    std::vector<CamDev> direct_camera_list;

}CameraArray;


///================================UserStruct==================================///
enum UserType{
    UserType_admin,    ///管理员
    UserType_ordinary, ///普通用户
    UserType_assit     ///辅助用户，管理员创建创建，可以由admin推导得到，用于登陆server
};
enum CamCtlFlag{CamCtlFlag_noright,CamCtlFlag_read,CamCtlFlag_modify};

typedef  struct CamCtl{
    CamCtl();
    CamCtl(string v_camname,CamCtlFlag v_flag,CameraType v_type);
    CamCtl(neb::CJsonObject &v_json);
    neb::CJsonObject jsonobj() const;

    string _camname;
    CamCtlFlag _flag;
    CameraType _type;


}CamCtl;


typedef struct User{
    User();
    User(const string &v_username,
         const string &v_password,
         const UserType &v_type,
         const std::vector<CamCtl> &v_cams);

    User(neb::CJsonObject &v_json);
    neb::CJsonObject jsonobj() const;

    string _username;
    string _password;
    UserType _type;
    std::vector<CamCtl> _cams;

}User;


///================================NVRStruct==================================///
typedef struct NvrDev{
    NvrDev();
    NvrDev(  string v_ip,  string v_username,  string v_password);
    NvrDev(neb::CJsonObject &v_json);
    neb::CJsonObject jsonobj() const;
    int setEmpty();
    NvrDev& operator =(const NvrDev& v_nvr)//赋值运算符
    {
        if (this != &v_nvr)
        {
            _ip=v_nvr._ip;
            _username=v_nvr._username;
            _password=v_nvr._password;
        }
        return *this;
    }

    string _ip;
    string _username;
    string _password;

}NvrDev;



///================================request_type======================================///

enum ENUM_REQ_TYPE{
    ///HTTP
    ENUM_REQ_SINGLEUSER=0,
    ENUM_REQ_ALLUSER,
    ENUM_REQ_RUNTME,
    ENUM_REQ_USERSTAUS,  ///WS
    ENUM_REQ_DEVARRAY,
    ENUM_REQ_DEVSTATUS,
    ENUM_REQ_QRYNVR,
    ENUM_REQ_QRYSERIALPORT,

    ENUM_REQ_ADDDEV,
    ENUM_REQ_ADDUSER,

    ENUM_REQ_DELDEV,
    ENUM_REQ_DELUSER,
    ENUM_REQ_EMPTYNVR,

    ENUM_REQ_SETDEV,
    ENUM_REQ_SETUSER,
    ENUM_REQ_SETNVR,

    ENUM_REQ_STOPSERVICE,
    ENUM_REQ_DIRECTCAM_OP,


    ///WS
    ENUM_WS_RESTART,
    ENUM_WS_PUSHLOG,
    ENUM_WS_USERSTAUS,
    ENUM_WS_DEVSTATUS
};

///================================error_type======================================///
/// 错误列表
enum ENUM_HTTP_ERRCODE{
    ENUM_HTTP_ERRCODE_SUCCEED=200,  ///正常
    ENUM_HTTP_ERRCODE_WRONGFORMAT=400,  ///错误请求 — 请求中有语法问题，或不能满足请求。
    ENUM_HTTP_ERRCODE_UNAUTHORIZED =401,  ///未授权 — 未授权客户机访问数据。
    ENUM_HTTP_ERRCODE_OPERATEFAILURE=404,  ///404  找不到 — 服务器找不到给定的资源/数据；文档不存在。
    ENUM_HTTP_ERRCODE_DISCONNECT=-1      ///未成功连接

};



///================================request_convert======================================///

///对请求和相应进行处理的辅助数据结构
typedef struct CtlInfo{
    CtlInfo():_req_type(){}
    CtlInfo(ENUM_REQ_TYPE v_type):_req_type(v_type){}
    neb::CJsonObject jsonobj() const;
    CtlInfo( const neb::CJsonObject& v_json);
    neb::CJsonObject jsonobjWithData(const neb::CJsonObject & v_json ) const;

    ENUM_REQ_TYPE _req_type;
}CtlInfo;

typedef struct ReturnInfo{
    ReturnInfo(ENUM_HTTP_ERRCODE v_return_code):_return_code(v_return_code){}
    neb::CJsonObject jsonobj() const;
    ReturnInfo( const neb::CJsonObject& v_json);
    neb::CJsonObject jsonobjWithData(const neb::CJsonObject & v_json ) const;
    ENUM_HTTP_ERRCODE getReturnCode() const{return _return_code;}

    ENUM_HTTP_ERRCODE _return_code;
}ReturnInfo;


template<class T>
inline T dataStructFromJsonObj(const neb::CJsonObject& v_json){
    return T(v_json);
}

template<class T>
inline T dataStructFromJsonObj( neb::CJsonObject& v_json){
    return T(v_json);
}

template<class T>
inline neb::CJsonObject jsonFromArray(const vector<T> &v_list){
    neb::CJsonObject obj_root;
    for(auto it=v_list.begin();it!=v_list.end();it++){
        obj_root.Add(it->jsonobj());
    }
    return obj_root;
}

template<> inline neb::CJsonObject jsonFromArray<string>(const vector<string> &v_list){
    neb::CJsonObject obj_root;
    for(auto it=v_list.begin();it!=v_list.end();it++){
        obj_root.Add(*it);
    }
    return obj_root;
}

template <class T>
inline vector<T> arrayFromJson(neb::CJsonObject& v_json){
    vector<T> list_ret;
    int size=v_json.GetArraySize();
    for (int i=0;i<size;i++) {
        T it(v_json[i]);
        list_ret.push_back(it);
    }
    return list_ret;
}

template <>
inline vector<string> arrayFromJson(neb::CJsonObject& v_json){
    vector<string> list_ret;
    int size=v_json.GetArraySize();
    for (int i=0;i<size;i++) {
        string it=v_json(i);
        list_ret.push_back(it);
    }
    return list_ret;
}



typedef struct SingleString{
    SingleString(const string &v_str):_str(v_str){}
    neb::CJsonObject jsonobj() const;
    SingleString( const neb::CJsonObject& v_json);

    string _str;
}SingleString;






}

/// 转换已秒为单位的一段时间
string formatTimeInterval(long v_time);


#endif
