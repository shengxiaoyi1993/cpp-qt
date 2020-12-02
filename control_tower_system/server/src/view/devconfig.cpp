#include "devconfig.h"
#include "ui_devconfig.h"

DevConfig::DevConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevConfig)
{
    ui->setupUi(this);
    _devitem_instance=new DevItem(this);


    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* , int )),
            this,SLOT(on_treewidget_itemClicked(QTreeWidgetItem* , int )));
    connect(_devitem_instance,SIGNAL(sgl_addCam(ns_tcs_ds_def::CamDev)),
            this,SLOT(slot2DevItem_addCam(ns_tcs_ds_def::CamDev)));
    connect(_devitem_instance,SIGNAL(sgl_rmCam(ns_tcs_ds_def::CamDev))
            ,this,SLOT(slot2DevItem_rmCam(ns_tcs_ds_def::CamDev)));
    connect(_devitem_instance,SIGNAL(sgl_modifyCam(ns_tcs_ds_def::CamDev))
            ,this,SLOT(slot2DevItem_modifyCam(ns_tcs_ds_def::CamDev)));
    connect(_devitem_instance,SIGNAL(sgl_testCamConnect(ns_tcs_ds_def::CamDev))
            ,this,SLOT(slot2DevItem_testCamConnect(ns_tcs_ds_def::CamDev)));



}

DevConfig::~DevConfig()
{
    delete ui;
}


void DevConfig::on_treewidget_itemClicked(QTreeWidgetItem* p_item , int v_index){
    bool is_topitem=false;
    for (int i=0;i<ui->treeWidget->topLevelItemCount();i++) {
        if(ui->treeWidget->topLevelItem(i) == p_item){
            is_topitem=true;
            break;
        }
    }
    if (is_topitem != true) {
        ns_tcs_ds_def::CamDev cam_tmp;
        int flag = _camarray.findCam(p_item->text(1).toStdString(),cam_tmp);
        if (flag == 0) {
            bool is_direct_tmp=(cam_tmp._type==ns_tcs_ds_def::CameraType_direct)?true:false;
            _devitem_instance->updateCamItem(cam_tmp);
            _devitem_instance->setAddMode(false);
            _devitem_instance->setDirectMode(is_direct_tmp);
            _devitem_instance->show();
        }
    }

}


/// search subitem in tablewidget
///
void DevConfig::updateDevStatus(const vector<ns_tcs_ds_def::CamStatus>& l_status){
    cout<<__func__<<endl;

    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++){

        for (int j=0;j<ui->treeWidget->topLevelItem(i)->childCount() ;j++ ) {
            ui->treeWidget->topLevelItem(i)->child(j)->setText(3,"未连接");

            for (auto it=l_status.begin();it!=l_status.end() ;it++ ) {
                if (ui->treeWidget->topLevelItem(i)->child(j)->text(1).toStdString() == it->_camname ) {
                    cout<<"text(1):"<<ui->treeWidget->topLevelItem(i)->child(j)->text(1).toStdString()<<endl;
                    cout<<"it->_camname:"<<it->_camname<<endl;
                    if (it->_status) {
                        ui->treeWidget->topLevelItem(i)->child(j)->setText(3,"已连接");
                    }
                }
            }
        }


    }
}




void DevConfig::on_pushButton_adddev_clicked()
{
    _devitem_instance->setAddMode(true);
    _devitem_instance->empty();
    _devitem_instance->setDirectMode(false);
    _devitem_instance->show();


}

void DevConfig::updateSerialPort(const vector<string> &l_port){
    _l_serialport=l_port;
    _devitem_instance->setTTYList(_l_serialport);

}


/// 对每一项设备都发射删除信号，设备成功删除后导致的见面变化由controler进行控制
void DevConfig::on_pushButton_removealldev_clicked()
{

    ns_tcs_ds_def::CameraArray tmparray=_camarray;
    for(int i=0;i<tmparray.ground_camera_list.size();i++){
        emit sgl_rmCam(tmparray.ground_camera_list[i]);
    }

    for(int i=0;i<tmparray.air_camera_list.size();i++){
        emit sgl_rmCam(tmparray.air_camera_list[i]);
    }

    for(int i=0;i<tmparray.direct_camera_list.size();i++){
        emit sgl_rmCam(tmparray.direct_camera_list[i]);
    }

}


void DevConfig::on_pushButton_close_clicked()
{
    this->hide();
    _devitem_instance->hide();
}


void DevConfig::slot2DevItem_addCam(ns_tcs_ds_def::CamDev v_cam){
    emit sgl_addCam(v_cam);
}

void DevConfig::slot2DevItem_rmCam(ns_tcs_ds_def::CamDev v_cam){
    emit sgl_rmCam(v_cam);

}

void DevConfig::slot2DevItem_modifyCam(ns_tcs_ds_def::CamDev v_cam){
    emit sgl_modifyCam(v_cam);

}
void DevConfig::slot2DevItem_testCamConnect(ns_tcs_ds_def::CamDev v_cam){
    emit sgl_testCamConnect(v_cam);

}



/// 保存设备阵列数据
/// 清除设备组中的子项
/// 然后为每一top项添加数据 top项的的标识为0对地 1对空 2直显
void DevConfig::updateCamArray(const ns_tcs_ds_def::CameraArray &v_array){
    cout<<__func__<<endl;
    _camarray=v_array;
    /// 清空
    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++){
        while (ui->treeWidget->topLevelItem(i)->childCount()>0) {
            ui->treeWidget->topLevelItem(i)->removeChild(ui->treeWidget->topLevelItem(i)->child(0));
        }
    }

    for(int i=0;i<v_array.ground_camera_list.size();i++){
        QTreeWidgetItem *tmpitem=new QTreeWidgetItem({QString::number(i),
                                                      QString::fromStdString(v_array.ground_camera_list[i]._camname),
                                                      QString::fromStdString(v_array.ground_camera_list[i]._ip),
                                                      QString::fromStdString("未连接")
                                                     });
        ui->treeWidget->topLevelItem(0)->addChild(tmpitem);
    }


    for(int i=0;i<v_array.air_camera_list.size();i++){
        QTreeWidgetItem *tmpitem=new QTreeWidgetItem({QString::number(i),
                                                      QString::fromStdString(v_array.air_camera_list[i]._camname),
                                                      QString::fromStdString(v_array.air_camera_list[i]._ip),
                                                      QString::fromStdString("未连接")
                                                     });
        ui->treeWidget->topLevelItem(1)->addChild(tmpitem);
    }


    for(int i=0;i<v_array.direct_camera_list.size();i++){
        QTreeWidgetItem *tmpitem=new QTreeWidgetItem({QString::number(i),
                                                      QString::fromStdString(v_array.direct_camera_list[i]._camname),
                                                      QString::fromStdString(v_array.direct_camera_list[i]._tty),
                                                      QString::fromStdString("未连接")
                                                     });
        ui->treeWidget->topLevelItem(2)->addChild(tmpitem);
    }
}
