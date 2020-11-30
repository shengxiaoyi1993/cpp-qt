#include "userconfig.h"
#include "ui_userconfig.h"

UserConfig::UserConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserConfig)
{
    ui->setupUi(this);
    _useritem_instance=new UserItem(this);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),
            this,SLOT(on_item_click(int,int)));
    connect(_useritem_instance,SIGNAL(sgl_rmOneUser(ns_tcs_ds_def::User)),
            this,SLOT(on_useritem_instance_rmOneUser(ns_tcs_ds_def::User)));
    connect(_useritem_instance,SIGNAL(sgl_addOneUser(ns_tcs_ds_def::User)),
            this,SLOT(on_useritem_instance_addOneUser(ns_tcs_ds_def::User)));
    connect(_useritem_instance,SIGNAL(sgl_modifyOneUser(ns_tcs_ds_def::User)),
            this,SLOT(on_useritem_instance_modifyOneUser(ns_tcs_ds_def::User)));
}

UserConfig::~UserConfig()
{
    delete ui;
}


void UserConfig::on_pushButton_adduser_clicked()
{
    auto pos_find=find_if(_l_users.begin(),_l_users.end(),
                          [=](ns_tcs_ds_def::User v_user){
        return v_user._username=="admin";
    });
    if (pos_find != _l_users.end()) {
        _useritem_instance->setUserItem(*pos_find);
        auto l_csm=  (*pos_find)._cams;
        for (auto it=l_csm.begin();it!=l_csm.end() ;it++ ) {
            (*it)._flag=ns_tcs_ds_def::CamCtlFlag_noright;
        }
        _useritem_instance->updateCamCtlTable(l_csm);
        _useritem_instance->empty();
        _useritem_instance->show();
        _useritem_instance->setAddMode(true);
    }
    else{
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                              +": Error(no admin)!");
    }

}

void UserConfig::on_pushButton_close_clicked()
{
    this->hide();
    _useritem_instance->hide();
}


void UserConfig::updateUserList(const vector<ns_tcs_ds_def::User>& v_users){
    if (ui->tableWidget->rowCount()!=v_users.size()) {
        while (ui->tableWidget->rowCount()>0) {
            ui->tableWidget->removeRow(0);
        }

        for (uint i=0;i<v_users.size();i++) {
            ui->tableWidget->insertRow(i);
        }
    }

    for (uint i=0;i<v_users.size();i++) {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem( QString::fromStdString(v_users[i]._username)));
        QString user_type_tmp;
        if(v_users[i]._type==ns_tcs_ds_def::UserType_admin){
            //      user_type_tmp="admin";
            user_type_tmp="管理员";

        }
        else if(v_users[i]._type==ns_tcs_ds_def::UserType_assit){
            //      user_type_tmp="assit";
            user_type_tmp="配置用户";


        }
        else if(v_users[i]._type==ns_tcs_ds_def::UserType_ordinary){
            //      user_type_tmp="ordinary";
            user_type_tmp="普通用户";
        }
        else{
            //      user_type_tmp="undefined";
            user_type_tmp="未定义";
        }
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(user_type_tmp));
    }

    _l_users=v_users;

    auto pos_find=find_if(_l_users.begin(),_l_users.end(),
                          [=](ns_tcs_ds_def::User v_user){
        return v_user._username=="admin";
    });
    if (pos_find != _l_users.end()) {
        _useritem_instance->updateCamCtlTable(pos_find->_cams);
    }
}


void UserConfig::on_useritem_instance_rmOneUser(ns_tcs_ds_def::User v_user){
  if (v_user._username!="admin") {
    emit sgl_rmOneUser(v_user);
  }
}

void UserConfig::on_useritem_instance_addOneUser(ns_tcs_ds_def::User v_user){
    emit sgl_addOneUser(v_user);

}

void UserConfig::on_useritem_instance_modifyOneUser(ns_tcs_ds_def::User v_user){
    emit sgl_modifyOneUser(v_user);
}


void UserConfig::on_item_click(int v_x,int v_y){

    if(v_x>=_l_users.size()){
        throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+": Error!");
    }
    _useritem_instance->setUserItem(_l_users[v_x]);
    _useritem_instance->setAddMode(false);
    _useritem_instance->show();

}

/// 逐项删除除了admin以外的所有用户
void UserConfig::on_pushButton_rmall_clicked()
{
    vector<ns_tcs_ds_def::User>tmplist= _l_users;
    for(int i=0;i<tmplist.size();i++){
        if(tmplist[i]._username != "admin"){
          cout<<"000000000000000000000:"<<tmplist[i]._username<<endl;
            emit sgl_rmOneUser(tmplist[i]);
        }
    }
}



