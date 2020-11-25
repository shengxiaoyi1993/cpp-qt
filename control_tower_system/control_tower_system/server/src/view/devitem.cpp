#include "devitem.h"
#include "ui_devitem.h"

DevItem::DevItem(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DevItem)
{
  ui->setupUi(this);
  setTTYList({});
  ui->pushButton_connecttest->setDisabled(true);
}

DevItem::~DevItem()
{
  delete ui;
}

/// 测试设备是否能够连接，由于此时设备不一定被添加所以需要适用另外的接口
void DevItem::on_pushButton_connecttest_clicked()
{
  emit sgl_testCamConnect(getUserInfo());

}

void DevItem::on_pushButton_del_clicked()
{
  emit sgl_rmCam(getUserInfo());

}



void DevItem::on_pushButton_apply_clicked()
{
  if(_is_addmode){
    emit sgl_addCam(getUserInfo());
    setAddMode(false);
  }
  else{
    emit sgl_modifyCam(getUserInfo());
  }

}

void DevItem::on_pushButton_confirm_clicked()
{
  if(_is_addmode){
    emit sgl_addCam(getUserInfo());
    setAddMode(false);
  }
  else{
    emit sgl_modifyCam(getUserInfo());
  }
  this->hide();

}

void DevItem::on_pushButton_cancel_clicked()
{
  this->hide();
}


/// comobox index 0(ground) 1(air) 2(direct)
void DevItem::updateCamItem(const ns_tcs_ds_def::CamDev v_cam){

  ///_camname
  ui->lineEdit_camname->setText(QString::fromStdString(v_cam._camname));


  ///_type
  int index_type;
  switch (v_cam._type) {
    case ns_tcs_ds_def::CameraType_to_ground:{
      index_type=0;
      break;
    }
    case ns_tcs_ds_def::CameraType_to_air:{
      index_type=1;
      break;
    }
    case ns_tcs_ds_def::CameraType_direct:{
      index_type=2;
      break;
    }
    default:
      throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                            +": Error(wrong selection)!");
  }
  ui->comboBox_type->setCurrentIndex(index_type);


  ///
  ui->lineEdit_ip->setText(QString::fromStdString(v_cam._ip));

  ///窗口需要根据当前service所在的接口进行初始化
  int index_tty=0;
  for (uint i=0;i<_tty.size();i++) {
    if(_tty[i] == v_cam._tty){
      index_tty=i;
      break;
    }
  }
  ui->comboBox_tty->setCurrentIndex(index_tty);

  ui->lineEdit_username->setText(QString::fromStdString(v_cam._username));
  ui->lineEdit_password->setText(QString::fromStdString(v_cam._password));
}


void DevItem::empty(){
  updateCamItem(ns_tcs_ds_def::CamDev());
}


void DevItem::setAddMode(bool v_flag){
  _is_addmode=v_flag;
  ui->pushButton_del->setDisabled(v_flag);

}

void DevItem::setTTYList(const vector<string> l_tty){
  _tty=l_tty;
  _tty.insert(_tty.begin(),"UNDEFINED");
  ui->comboBox_tty->clear();
  for (auto it=_tty.begin();it!=_tty.end();it++) {
    ui->comboBox_tty->addItem(QString::fromStdString(*it));
  }
}

/// 直显相机与普通相机有一些差别
/// 就是串口栏不可用
/// 若设置为直显相机相机配置模式，则时tty多选栏显示的是UNDEFINED，且不可用
void DevItem::setDirectMode(bool v_flag){
  if(v_flag){
    ui->comboBox_tty->setCurrentIndex(0);
  }
  ui->comboBox_tty->setDisabled(!v_flag);

}

ns_tcs_ds_def::CamDev DevItem::getUserInfo() const{
  ns_tcs_ds_def::CamDev v_cam;
  v_cam._camname=ui->lineEdit_camname->text().toStdString();
  v_cam._ip=ui->lineEdit_ip->text().toStdString();
  v_cam._password=ui->lineEdit_password->text().toStdString();
  v_cam._tty=ui->comboBox_tty->currentText().toStdString();

  if(ui->comboBox_type->currentIndex()==0){
    v_cam._type=ns_tcs_ds_def::CameraType_to_ground;
  }
  else if(ui->comboBox_type->currentIndex()==1) {
    v_cam._type=ns_tcs_ds_def::CameraType_to_air;

  }
  else if(ui->comboBox_type->currentIndex()==2) {
    v_cam._type=ns_tcs_ds_def::CameraType_direct;
  }
  else{
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                          +": Error(wrong index)!");
  }
  v_cam._username=ui->lineEdit_username->text().toStdString();
  return v_cam;
}






void DevItem::on_comboBox_type_currentIndexChanged(int index)
{
  if(index==2){
    ui->comboBox_tty->setDisabled(false);
  }
  else{
    ui->comboBox_tty->setDisabled(true );
  }
}
