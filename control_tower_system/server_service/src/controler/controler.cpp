#include "controler.h"
#include "controler_top.h"
#include <iostream>
#include <string>
#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QtConcurrent/QtConcurrent>
#include <sstream>


using namespace std;

Controler::Controler(string v_config, QObject *parent) : QObject(parent),
    _apiserver(new APIServer),
    _devproxy(new DevProxy()),
    _datacoll(new DataManage::DataColl(v_config)),
    _timer_task(new QTimer)
{

    initTimeRecord();
    ///_apiserver初始化
    updateUserMapInAPI();
    connect(_apiserver,SIGNAL(sgl_OnHttpError(int)),
            this,SLOT(slot_OnHttpError(int)));
    connect(_apiserver,SIGNAL(sgl_OnUserLogin(string)),
            this,SLOT(slot_OnUserLogin(string)));
    connect(_apiserver,SIGNAL(sgl_OnUserLogout(string)),
            this,SLOT(slot_OnUserLogout(string)));
    connect(_apiserver,SIGNAL(sgl_OnWebsocketError(int)),
            this,SLOT(slot_OnWebsocketError(int)));


    connect(_apiserver,SIGNAL(sgl_qrySingleUser( long long ,string ,string )),
            this,SLOT(slot_qrySingleUser( long long ,string ,string )));
    connect(_apiserver,SIGNAL(sgl_qryAllUser( long long ,string )),
            this,SLOT(slot_qryAllUser( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_qryRunTime( long long ,string )),
            this,SLOT(slot_qryRunTime( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_qryUserStatus( long long ,string )),
            this,SLOT(slot_qryUserStatus( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_qryDevArray( long long ,string )),
            this,SLOT(slot_qryDevArray( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_qryDevStatus( long long ,string )),
            this,SLOT(slot_qryDevStatus( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_qryNVR( long long ,string )),
            this,SLOT(slot_qryNVR( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_qrySerialPort( long long ,string )),
            this,SLOT(slot_qrySerialPort( long long ,string )));

    connect(_apiserver,SIGNAL(sgl_addDev( long long ,string ,ns_tcs_ds_def::CamDev )),
            this,SLOT(slot_addDev( long long ,string ,ns_tcs_ds_def::CamDev )));
    connect(_apiserver,SIGNAL(sgl_addUser( long long ,string ,ns_tcs_ds_def::User )),
            this,SLOT(slot_addUser( long long ,string ,ns_tcs_ds_def::User )));

    connect(_apiserver,SIGNAL(sgl_delDev( long long ,string ,string )),
            this,SLOT(slot_delDev( long long ,string ,string )));
    connect(_apiserver,SIGNAL(sgl_delUser( long long ,string ,string )),
            this,SLOT(slot_delUser( long long ,string ,string )));
    connect(_apiserver,SIGNAL(sgl_emptyNVR( long long ,string )),
            this,SLOT(slot_emptyNVR( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_setDev( long long ,string ,ns_tcs_ds_def::CamDev )),
            this,SLOT(slot_setDev( long long ,string ,ns_tcs_ds_def::CamDev )));
    connect(_apiserver,SIGNAL(sgl_setUser( long long ,string ,ns_tcs_ds_def::User )),
            this,SLOT(slot_setUser( long long ,string ,ns_tcs_ds_def::User )));
    connect(_apiserver,SIGNAL(sgl_setNVR( long long ,string ,ns_tcs_ds_def::NvrDev )),
            this,SLOT(slot_setNVR( long long ,string ,ns_tcs_ds_def::NvrDev )));
    connect(_apiserver,SIGNAL(sgl_stopService( long long ,string )),
            this,SLOT(slot_stopService( long long ,string )));
    connect(_apiserver,SIGNAL(sgl_directCamOp( long long ,string ,ns_tcs_ds_def::DirectCamOp )),
            this,SLOT(slot_directCamOp( long long ,string ,ns_tcs_ds_def::DirectCamOp )));


    ///
    connect(_timer_task,SIGNAL(timeout())
            ,this,SLOT(on_checkAndConcludeTask()));
    _apiserver->setupListenHttp();
    _apiserver->setupListenWebsocket();
    _timer_task->start(1000);

    ///_devproxy初始化
    connect(_devproxy,SIGNAL(sgl_camDisconnect(string)),
            this,SLOT(slot_OnDevDisconnect(string)));
    connect(_devproxy,SIGNAL(sgl_camReconnect(string)),
            this,SLOT(slot_OnDevReconnect(string)));
    _devproxy->setCameraArray(_datacoll->CamArary());

    cout<<"CamArary:"<<_datacoll->CamArary().jsonobj().ToFormattedString()<<endl;
    cout<<"isListenHTTP:"<<_apiserver->isListenHTTP()<<endl;
    auto list_camstatus=_devproxy->getCamStatusList();

    for(auto it=list_camstatus.begin();it<list_camstatus.end();it++){
        cout<<""<<it->jsonobj().ToString()<<endl;
    }
    map<UserItem,bool> list_user= _apiserver->getUserStatus();
    cout<<"list_user.size:"<<list_user.size()<<endl;
    for (auto it = list_user.begin();it!=list_user.end();it++) {
        cout<<"_username:"<<it->first._username<<endl;
        cout<<"second:"<<it->second<<endl;
    }

}

Controler::~Controler(){
    delete _apiserver;
    delete _devproxy;
    delete _datacoll;
    delete _timer_task;

}




void Controler::on_checkAndConcludeTask(){

    stringstream ss;

    while(_list_task.size()>0){
        switch (_list_task[0].first) {
        case ENUM_LOOPTASK_WS_PUSHLOG:{
            pushLogToAdvanceUser(_list_task[0].second[1]);
            break;
        }
        case ENUM_LOOPTASK_WS_RESTART:{
            _apiserver->sendWSMsg_RESTART(_list_task[0].second[0]);
            break;
        }
        case ENUM_LOOPTASK_WS_USERSTAUS:{
            neb::CJsonObject tmppara(_list_task[0].second[1]);
            _apiserver->sendWSMsg_USERSTAUS(_list_task[0].second[0],
                    ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::UserStatus>(tmppara)
                    );

            break;
        }
        case ENUM_LOOPTASK_WS_DEVSTATUS:{
            neb::CJsonObject tmppara(_list_task[0].second[1]);
            _apiserver->sendWSMsg_DEVSTATUS(_list_task[0].second[0],
                    ns_tcs_ds_def::arrayFromJson<ns_tcs_ds_def::CamStatus>(tmppara));
            break;
        }
        }
        _list_task.erase(_list_task.begin());
    }
}


void Controler::pushLogToAdvanceUser(string v_log){
    auto l_user=_datacoll->Users();
    for(auto it=l_user.begin();it!=l_user.end();it++){
        if (it->_type==ns_tcs_ds_def::UserType_admin
                ||it->_type==ns_tcs_ds_def::UserType_assit) {
            _apiserver->sendWSMsg_PUSHLOG(it->_username,v_log);
        }
    }
}





void Controler::initTimeRecord(){
    time_t time_cur;
    time(&time_cur);
    _timerecord._time_start=time_cur;

}

void Controler::updateUserMapInAPI(){
    vector<UserItem> list_useritem;
    auto list_user=_datacoll->Users();
    for(auto it=list_user.begin();it<list_user.end();it++){
        string hash_tmp= QCryptographicHash::hash(QByteArray::fromStdString(it->_password), QCryptographicHash::Sha256)
                .toHex().toStdString();
        list_useritem.insert(list_useritem.begin(),UserItem(it->_username,hash_tmp));
        cout<<"hash_tmp:"<<hash_tmp<<endl;
        cout<<"it->_username:"<<it->_username<<endl;

    }
    //  _apiserver->stopListenHttp();
    //  _apiserver->stopListenWebsocket();
    _apiserver->setAllUserMap(list_useritem);

    //  _apiserver->setupListenHttp();
    //  _apiserver->setupListenWebsocket();

}


ns_tcs_ds_def::TimeRecord Controler::getCurTimeRecord(){
    time_t time_cur;
    time(&time_cur);
    _timerecord._time_cur=time_cur;
    _timerecord._time_interval=_timerecord._time_cur-_timerecord._time_start;
    return _timerecord;
}


/// 由于返回的错误码未知，因此测试后再进行
void Controler::slot_OnHttpError(int v_error_code){
    cout<<__func__<<" v_error_code:"<<v_error_code<<endl;
}

/// 登陆后向assit类型的用户通过ws发送状态改变后的用户登陆信息
/// 先找到已登陆的用户 (仅对assist类型的用户进行发送)
/// 然后查找各个用户权限下的用户的状态
void Controler::slot_OnUserLogin(string v_username){
    (void)v_username;
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"",v_username+" login"});
    on_UserStatusChange();

}

void Controler::slot_OnUserLogout(string v_username){
    (void)v_username;
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"",v_username+" logout"});


    on_UserStatusChange();
}


/// 由于返回的错误码未知，因此测试后再进行
void Controler::slot_OnWebsocketError(int v_error_code){
    cout<<__func__<<" v_error_code:"<<v_error_code<<endl;
}

void Controler::slot_OnDevDisconnect(string v_camname){
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"",v_camname+" Disconnect "});

    on_DevStatusChange(v_camname);
}

void Controler::slot_OnDevReconnect(string v_camname){
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"",v_camname+" Reconnect "});


    on_DevStatusChange(v_camname);
}

/// 在datacoll中获取总数据
/// 判断是否在权限内
/// 获取该用户的信息,向句柄返回数据
void Controler::slot_qrySingleUser( long long v_fd,string v_usr,string v_data){
    /// 该用户或者更高级的用户可以访问该用户
    /// admin,assit 可被admin ,assit 类型的访问,可以访问任意类型的
    /// ordinary    可以访问自身
    cout<<__func__<<endl;

    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();
    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),[=](ns_tcs_ds_def::User v_useritem){
        cout<<"v_useritem._username:"<<v_useritem._username<<endl;
        cout<<"v_usr:"<<v_usr<<endl;

        return (v_useritem._username==v_usr);
    });
    cout<<__func__<<" 2"<<endl;

    if(pos_user!=list_user.end()){
        if((v_usr == v_data)||(pos_user->_type!=ns_tcs_ds_def::UserType_ordinary)){
            cout<<__func__<<" judge0:"<<v_usr << v_data<<endl;
            cout<<__func__<<" judge1:"<<(pos_user->_type!=ns_tcs_ds_def::UserType_ordinary)<<endl;


            auto pos_target=find_if(list_user.begin(),list_user.end(),[=](ns_tcs_ds_def::User v_useritem){
                cout<<"v_useritem._username:"<<v_useritem._username<<endl;
                cout<<"v_usr:"<<v_usr<<endl;
                return (v_useritem._username==v_data);
            });
            if(pos_target != list_user.end()){
                _apiserver->sendHTTPMsg_REQ_SINGLEUSER(v_fd,*pos_target);
                return ;
            }
        }
    }
    cout<<__func__<<" 3"<<endl;

    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    return ;

}

void Controler::slot_qryAllUser( long long v_fd,string v_usr){

    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();
    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),[=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });
    /// 是否在文件名列表中
    if(pos_user!=list_user.end()){
        /// 判断类型，admin/assit输出全部 ordinary 输出自身
        switch (pos_user->_type) {
        case ns_tcs_ds_def::UserType_ordinary :{
            vector<ns_tcs_ds_def::User> tmplist_users;
            tmplist_users.push_back(*pos_user);
            _apiserver->sendHTTPMsg_REQ_ALLUSER(v_fd,tmplist_users);
            return;
        }
        case ns_tcs_ds_def::UserType_admin:
        case ns_tcs_ds_def::UserType_assit:{
            _apiserver->sendHTTPMsg_REQ_ALLUSER(v_fd,list_user);
            return;
        }
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
}

/// 只要用户在用户信息列表中，就可以返回
void Controler::slot_qryRunTime( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();
    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),[=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });
    if(pos_user!=list_user.end()){
        _apiserver->sendHTTPMsg_REQ_RUNTME(v_fd,getCurTimeRecord());
    }
    else{
        _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    }
}

///权限下的所有用户的状态
/// 同上，admin/assit输出全部 ordinary输出自身
void Controler::slot_qryUserStatus( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();
    map<UserItem,bool> list= _apiserver->getUserStatus();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        vector<ns_tcs_ds_def::UserStatus> v_list;
        if(pos_user->_type==ns_tcs_ds_def::UserType_ordinary){
            for (auto it=list.begin();it!=list.end();it++) {
                if(it->first._username==v_usr){
                    v_list.push_back(ns_tcs_ds_def::UserStatus(it->first._username,it->second));
                }
            }
            _apiserver->sendHTTPMsg_REQ_USERSTAUS(v_fd,v_list);
            return;


        }
        else if(pos_user->_type==ns_tcs_ds_def::UserType_admin
                ||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            for (auto it=list.begin();it!=list.end();it++) {
                v_list.push_back(ns_tcs_ds_def::UserStatus(it->first._username,it->second));
            }
            _apiserver->sendHTTPMsg_REQ_USERSTAUS(v_fd,v_list);
            return;
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);

}


/// 输出用户权限内的设备
/// 同上，admin/assit对所有设备具有权限 ordinary输出自身
void Controler::slot_qryDevArray( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();
    ns_tcs_ds_def::CameraArray camarray_tmp=_datacoll->CamArary();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });
    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_ordinary){
            ns_tcs_ds_def::CameraArray camarray_new;
            ///对于每个设备都要判断是否在该用户的权限列表上
            for(auto it_dev=camarray_tmp.air_camera_list.begin();
                it_dev!=camarray_tmp.air_camera_list.end();it_dev++){
                auto pos_devctl=find_if(pos_user->_cams.begin(),pos_user->_cams.end(),
                                        [=](ns_tcs_ds_def::CamCtl v_ctl){
                    return (v_ctl._camname==it_dev->_camname);
                });
                if(pos_devctl!=pos_user->_cams.end()){
                    camarray_new.addCam(*it_dev);
                }
            }

            for(auto it_dev=camarray_tmp.ground_camera_list.begin();
                it_dev!=camarray_tmp.ground_camera_list.end();it_dev++){
                auto pos_devctl=find_if(pos_user->_cams.begin(),pos_user->_cams.end(),
                                        [=](ns_tcs_ds_def::CamCtl v_ctl){
                    return (v_ctl._camname==it_dev->_camname);
                });
                if(pos_devctl!=pos_user->_cams.end()){
                    camarray_new.addCam(*it_dev);
                }
            }

            for(auto it_dev=camarray_tmp.direct_camera_list.begin();
                it_dev!=camarray_tmp.direct_camera_list.end();it_dev++){
                auto pos_devctl=find_if(pos_user->_cams.begin(),pos_user->_cams.end(),
                                        [=](ns_tcs_ds_def::CamCtl v_ctl){
                    return (v_ctl._camname==it_dev->_camname);
                });
                if(pos_devctl!=pos_user->_cams.end()){
                    camarray_new.addCam(*it_dev);
                }
            }

            _apiserver->sendHTTPMsg_REQ_DEVARRAY(v_fd,camarray_new);

        }
        else if(pos_user->_type==ns_tcs_ds_def::UserType_admin
                ||pos_user->_type==ns_tcs_ds_def::UserType_assit){

            _apiserver->sendHTTPMsg_REQ_DEVARRAY(v_fd,camarray_tmp);

        }
    }

    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);


}

///权限下的所有设备的状态
void Controler::slot_qryDevStatus( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();
    vector<ns_tcs_ds_def::CamStatus> list_camstatus= _devproxy->getCamStatusList();
    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){

        if(pos_user->_type==ns_tcs_ds_def::UserType_ordinary){
            /// 对于每个在用户权限内的设备查询并保存
            vector<ns_tcs_ds_def::CamStatus> list_camstatus_new;
            for(auto it=pos_user->_cams.begin();it<pos_user->_cams.end();it++){
                auto pos_dev=find_if(list_camstatus.begin(),list_camstatus.end(),
                                     [=](ns_tcs_ds_def::CamStatus v_cs){
                    return (v_cs._camname==it->_camname);
                });
                if(pos_dev != list_camstatus.end()){
                    list_camstatus_new.push_back(*pos_dev);
                }
            }
            _apiserver->sendHTTPMsg_REQ_DEVSTATUS(v_fd,list_camstatus_new);

            return;
        }
        else if(pos_user->_type==ns_tcs_ds_def::UserType_admin
                ||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            _apiserver->sendHTTPMsg_REQ_DEVSTATUS(v_fd,list_camstatus);
            return;
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
}

void Controler::slot_qryNVR( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        _apiserver->sendHTTPMsg_REQ_QRYNVR(v_fd,_datacoll->NVR());
        return;
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);

}


void Controler::slot_qrySerialPort( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        vector<string> list_port;
        _devproxy->getSerialPortList(list_port);

        _apiserver->sendHTTPMsg_REQ_QRYSERIALPORT(v_fd,list_port);
        return;
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);

}


/// 只有admin/assist才能添加设备
void Controler::slot_addDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data){
    cout<<"slot_addDev:"<<v_data.jsonobj().ToString();
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->addCam(v_data);
            cout<<"addcom:"<<v_data.jsonobj().ToString()<<endl;
            if(flag==0){
                cout<<"添加成功后，在devproxy实例化一个对象"<<endl;

                /// 添加成功后，在devproxy实例化一个对象
                _devproxy->setCameraArray(_datacoll->CamArary());
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                on_DevStatusChange(v_data._camname);
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to add camera:"+v_data._camname});
                return ;
            }
        }
    }

    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to add camera:"+v_data._camname});
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
}

/// 只有admin/assist具有添加用户的权限
void Controler::slot_addUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->addUser(v_data);
            if(flag==0){
                /// 添加成功后，在devproxy实例化一个对象
                updateUserMapInAPI();
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);

                on_UserStatusChange();
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to add user:"+v_data._username});

                return ;
            }
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to add user:"+v_data._username});

}

/// 同上
void Controler::slot_delDev( long long v_fd,string v_usr,string v_cmaname){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->delCam(ns_tcs_ds_def::CamDev(v_cmaname,"","","","",ns_tcs_ds_def::CameraType_to_air));
            if(flag==0){
                /// 添加成功后，在devproxy实例化一个对象
                _devproxy->rmCamDev(v_cmaname);
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                on_DevStatusChange(v_cmaname);
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to del camera:"+v_cmaname});

                return ;
            }
        }
    }
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to del camera:"+v_cmaname});
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);

}

/// 只有admin/assist才能
void Controler::slot_delUser( long long v_fd,string v_usr,string v_username){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->delUser(ns_tcs_ds_def::User(v_username,"",ns_tcs_ds_def::UserType_ordinary,{}));
            if(flag==0){
                /// 添加成功后，在devproxy实例化一个对象
                ///
                updateUserMapInAPI();

                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                on_UserStatusChange();
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to del user:"+v_username});

                return ;
            }
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to del user:"+v_username});

}

/// 只有admin/assist才能
void Controler::slot_emptyNVR( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin
                ||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->emptyNVR();
            if(flag == 0){
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to empty NVR "});

                return ;
            }

        }
    }

    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to empty NVR "});

}

void Controler::slot_setDev( long long v_fd,string v_usr,ns_tcs_ds_def::CamDev v_data){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->modifyCam(v_data);
            if(flag==0){
                /// 添加成功后，在devproxy实例化一个对象
                _devproxy->setCameraArray(_datacoll->CamArary());
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to modify camera "});

                return ;
            }
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to modify camera "});

}

void Controler::slot_setUser( long long v_fd,string v_usr,ns_tcs_ds_def::User v_data){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->modifyUser(v_data);
            if(flag==0){
                /// 添加成功后，在devproxy实例化一个对象
                ///
                updateUserMapInAPI();
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to modify user: "+v_data._username});

                return ;
            }
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to modify user: "+v_data._username});

}

void Controler::slot_setNVR( long long v_fd,string v_usr,ns_tcs_ds_def::NvrDev v_data){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag=_datacoll->setNVR(v_data);
            if(flag==0){
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to modify NVR "});

                return ;
            }
        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to modify NVR "});

}

void Controler::slot_stopService( long long v_fd,string v_usr){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
            //            exit(0);

        }
    }
    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
}


/// 当该设备在该用户的权限内，则可以操作
void Controler::slot_directCamOp( long long v_fd,string v_usr,ns_tcs_ds_def::DirectCamOp v_data){
    std::vector<ns_tcs_ds_def::User> list_user=  _datacoll->Users();

    /// 现在用户信息列表中找到用户类型
    auto pos_user=find_if(list_user.begin(),list_user.end(),
                          [=](ns_tcs_ds_def::User v_useritem){
        return (v_useritem._username==v_usr);
    });

    if(pos_user!=list_user.end()){
        if(pos_user->_type==ns_tcs_ds_def::UserType_admin||pos_user->_type==ns_tcs_ds_def::UserType_assit){
            int flag =  _devproxy->opDirectCam(v_data);
            if(flag==0){
                _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                return;

            }
        }
        else if(pos_user->_type==ns_tcs_ds_def::UserType_ordinary){
            auto pos_dev=find_if(pos_user->_cams.begin(),pos_user->_cams.end(),[=](ns_tcs_ds_def::CamCtl v_ctl){
                return (v_ctl._camname==v_data._camname);
            });
            if(pos_dev!=pos_user->_cams.begin()){
                int flag = _devproxy->opDirectCam(v_data);
                if(flag==0){
                    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED);
                    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","succeed to operate direct camera： "+v_data._camname});
                    return;
                }
            }
        }
    }

    _apiserver->sendHTTPMsg_RETURNCODE(v_fd,ns_tcs_ds_def::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    addLoopTask(ENUM_LOOPTASK_WS_PUSHLOG,{"","fail to operate direct camera： "+v_data._camname});

}


void Controler::on_UserStatusChange(){
    ///查找用户的类型
    map<UserItem,bool> list=_apiserver->getUserStatus();
    vector<ns_tcs_ds_def::UserStatus> v_list_status;
    for(auto it=list.begin();it!=list.end();it++){
        v_list_status.push_back(ns_tcs_ds_def::UserStatus(it->first._username,it->second));
    }

    std::vector<ns_tcs_ds_def::User> v_users= _datacoll->Users();


    ///在登陆的用户名列表中寻找
    for(auto it=v_list_status.begin();it!=v_list_status.end();it++){
        ///跳过未登陆的用户
        if(it->_status==false){continue;}
        /// 在配置列表中找到对应的表项，查看表中的用户是否是assist类型
        auto pos_find=find_if(v_users.begin(),v_users.end(),[=](ns_tcs_ds_def::User v_useritem){
            return (v_useritem._username==it->_username)&&(v_useritem._type==ns_tcs_ds_def::UserType_admin);
        });
        if(pos_find != v_users.end()){
            addLoopTask(ENUM_LOOPTASK_WS_USERSTAUS,{it->_username,
                                                    ns_tcs_ds_def::jsonFromArray(v_list_status).ToString()});
        }
    }
}

void Controler::addLoopTask(ENUM_LOOPTASK v_type,vector<string> l_para){
    _list_task.push_back(pair<ENUM_LOOPTASK,vector<string>>(v_type,l_para));
}



/// 查找所有有该设备权限且已经登陆的用户，组织各个用户权限下的设备状态，然后发送
void Controler::on_DevStatusChange(const string &v_dev){
    std::vector<ns_tcs_ds_def::User> v_users= _datacoll->Users();
    map<UserItem,bool> list=_apiserver->getUserStatus();
    ns_tcs_ds_def::CameraArray camarray= _datacoll->CamArary();
    vector<ns_tcs_ds_def::CamStatus> list_cams= _devproxy->getCamStatusList();

    for(auto it_user=list.begin();it_user!=list.end();it_user++){
        if(it_user->second == false){continue;}
        ///找到已登陆用户
        auto pos_find=find_if(v_users.begin(),v_users.end(),[=](ns_tcs_ds_def::User v_useritem){
            return  (v_useritem._username==it_user->first._username);

        });

        ///并查询是否具有设备权限
        if(pos_find != v_users.end()){
            auto pos_cam= find_if(pos_find->_cams.begin(),pos_find->_cams.end(),[=](ns_tcs_ds_def::CamCtl v_camctl){
                return (v_camctl._camname==v_dev);
            });

            /// 若用户有设备权限
            if(pos_cam!=pos_find->_cams.end()){
                /// 组织该用户权限内所有设备的状态
                /// 对于pos_find->_cams中的设备，都需要查询状态，保存到vector<ns_tcs_ds_def::CamStatus>中
                vector<ns_tcs_ds_def::CamStatus> tmp_list_camstatus;
                for(auto it_cam=pos_find->_cams.begin();it_cam!=pos_find->_cams.end();it_cam++){
                    auto pos_finddev=find_if(list_cams.begin(),list_cams.end(),[=](ns_tcs_ds_def::CamStatus v_tmpstatus){
                        return ( v_tmpstatus._camname==it_cam->_camname);
                    });
                    if(pos_finddev!=list_cams.end()){
                        tmp_list_camstatus.push_back(*pos_finddev);
                    }
                }

                /// 向该用户发送该设备状态列表
                addLoopTask(ENUM_LOOPTASK_WS_DEVSTATUS,{pos_find->_username,
                                                        ns_tcs_ds_def::jsonFromArray(tmp_list_camstatus).ToString()});

            }
        }
    }

}


