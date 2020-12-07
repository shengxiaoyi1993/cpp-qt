#include "useritem.h"
#include "ui_useritem.h"
#include <QDebug>
#include <QTreeWidgetItem>

UserItem::UserItem(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::UserItem)
{
  ui->setupUi(this);

  QTreeWidgetItemIterator iter(ui->treeWidget);
  while (*iter) {
    //do something like
    qDebug()<<(*iter)->text(0);
    (*iter)->setCheckState(1,Qt::CheckState::Unchecked);
    (*iter)->setCheckState(2,Qt::CheckState::Unchecked);
    ++iter;
  }

  connect(ui->pushButton_apply,SIGNAL(clicked(bool)),
          this,SLOT(on_pushButton_apply_click()));
  connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),
          this,SLOT(on_pushButton_cancel_click()));
  connect(ui->pushButton_confirm,SIGNAL(clicked(bool)),
          this,SLOT(on_pushButton_confirm_click()));
  connect(ui->pushButton_del,SIGNAL(clicked(bool)),
          this,SLOT(on_pushButton_del_click()));


}

UserItem::~UserItem()
{
  delete ui;
}


void UserItem::empty(){
  ui->lineEdit_password->setText("");
  ui->lineEdit_username->setText("");
  ui->combox_usertype->setCurrentIndex(0);
}


void UserItem::setUserItem(const ns_tcs_ds_def::User &v_user){
  ui->lineEdit_username->setText(QString::fromStdString(v_user._username));
  ui->lineEdit_password->setText(QString::fromStdString(v_user._password));

  int index_tmp;
  if (v_user._type== ns_tcs_ds_def::UserType_admin) {
    index_tmp=0;
  }
//  else   if (v_user._type== ns_tcs_ds_def::UserType_assit) {
//    index_tmp=1;
//  }
  else   if (v_user._type== ns_tcs_ds_def::UserType_ordinary) {
    index_tmp=1;
  }
  else{
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                          +": Error(UserType not match)!");

    index_tmp=-1;
  }
  ui->combox_usertype->setCurrentIndex(index_tmp);

  updateCamCtlTable(v_user._cams);
}


void UserItem::on_pushButton_apply_click(){
  ns_tcs_ds_def::User v_user=getUserInfo();
  if (_is_addmode) {
    emit sgl_addOneUser(v_user);
  }
  else{
    emit sgl_modifyOneUser(v_user);
  }
}

void UserItem::on_pushButton_cancel_click(){
  this->hide();
}


void UserItem::on_pushButton_confirm_click(){
  ns_tcs_ds_def::User v_user=getUserInfo();

  if (_is_addmode) {
    emit sgl_addOneUser(v_user);
  }
  else{
    emit sgl_modifyOneUser(v_user);
  }
  this->hide();

}

void UserItem::on_pushButton_del_click(){
  ns_tcs_ds_def::User v_user=getUserInfo();
  if (!_is_addmode) {
    emit sgl_rmOneUser(v_user);
  }
  this->hide();



}


void UserItem::setAddMode(bool v_flag){
  _is_addmode =v_flag;
  ui->pushButton_del->setDisabled(v_flag);
}



/// 先清空原数据
/// 根据原数据配置初始列表 遍历所有设备，
void UserItem::updateCamCtlTable(const vector<ns_tcs_ds_def::CamCtl> &l_camctl){
  /// 确保treewidget的topitem为4项
  if(ui->treeWidget->topLevelItemCount()!=4){
    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+": Error(topLevelItemCount not match)!");
  }



  ///设置设备控制表
  for(int i=0;i<ui->treeWidget->topLevelItemCount();i++){
    while (ui->treeWidget->topLevelItem(i)->childCount()>0) {
      ui->treeWidget->topLevelItem(i)->removeChild(ui->treeWidget->topLevelItem(i)->child(0));
    }
  }

  for(auto it=l_camctl.begin();it!=l_camctl.end();it++){
    Qt::CheckState checkstate_read_tmp;
    Qt::CheckState checkstate_control_tmp;
    if (it->_flag==ns_tcs_ds_def::CamCtlFlag_noright) {
      checkstate_read_tmp=Qt::CheckState::Unchecked;
      checkstate_control_tmp=Qt::CheckState::Unchecked;
    }
    else if(it->_flag==ns_tcs_ds_def::CamCtlFlag_modify) {
      checkstate_read_tmp=Qt::CheckState::Checked;
      checkstate_control_tmp=Qt::CheckState::Checked;
    }
    else if(it->_flag==ns_tcs_ds_def::CamCtlFlag_read) {
      checkstate_read_tmp=Qt::CheckState::Checked;
      checkstate_control_tmp=Qt::CheckState::Unchecked;
    }
    else{
      throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)+":Selection Error!");
    }

    if (it->_type == ns_tcs_ds_def::CameraType_direct) {
      QTreeWidgetItem *tmpitem=new QTreeWidgetItem({QString::fromStdString(it->_camname)});
      tmpitem->setCheckState(1,checkstate_read_tmp);
      tmpitem->setCheckState(2,checkstate_control_tmp);
      ui->treeWidget->topLevelItem(2)->addChild(tmpitem);
    }
    else if (it->_type == ns_tcs_ds_def::CameraType_to_air) {
      QTreeWidgetItem *tmpitem=new QTreeWidgetItem({QString::fromStdString(it->_camname)});
      tmpitem->setCheckState(1,checkstate_read_tmp);
      tmpitem->setCheckState(2,checkstate_control_tmp);
      ui->treeWidget->topLevelItem(1)->addChild(tmpitem);
    }
    else if (it->_type == ns_tcs_ds_def::CameraType_to_ground) {
      QTreeWidgetItem *tmpitem=new QTreeWidgetItem({QString::fromStdString(it->_camname)});
      tmpitem->setCheckState(1,checkstate_read_tmp);
      tmpitem->setCheckState(2,checkstate_control_tmp);
      ui->treeWidget->topLevelItem(0)->addChild(tmpitem);
    }
  }
  QTreeWidget t;

  ///<w>先不做nvr设置的更改
  ui->treeWidget->topLevelItem(3)->setCheckState(1,Qt::CheckState::Unchecked);
  ui->treeWidget->topLevelItem(3)->setCheckState(2,Qt::CheckState::Unchecked);
  ui->treeWidget->topLevelItem(3)->setDisabled(true);


}



/// 对于每一设备控制top项进行以下工作
/// 读取各个类型的top项下的子项，对子项分别进行以下操作
///   读取设备名
///   读取设备控制状态
///   将设备控制状态转换成数据结构中的枚举类型
vector<ns_tcs_ds_def::CamCtl> UserItem::getCamCtl(){
 vector<ns_tcs_ds_def::CamCtl> l_camctl;
 for(int i=0;i<ui->treeWidget->topLevelItemCount();i++){
   for(int j_ch=0;j_ch<ui->treeWidget->topLevelItem(i)->childCount();j_ch++){
     ns_tcs_ds_def::CamCtl camctl_tmp;
     camctl_tmp._camname=ui->treeWidget->topLevelItem(i)->child(j_ch)->text(0).toStdString();
     Qt::CheckState checkstate_read_tmp=ui->treeWidget->topLevelItem(i)->child(j_ch)->checkState(1);
     Qt::CheckState checkstate_control_tmp=ui->treeWidget->topLevelItem(i)->child(j_ch)->checkState(2);
     if (checkstate_control_tmp == Qt::CheckState::Checked) {
       camctl_tmp._flag=ns_tcs_ds_def::CamCtlFlag_modify;
     }
     else if(checkstate_control_tmp == Qt::CheckState::Unchecked
             &&checkstate_read_tmp == Qt::CheckState::Checked){
       camctl_tmp._flag=ns_tcs_ds_def::CamCtlFlag_read;
     }
     else{
       camctl_tmp._flag=ns_tcs_ds_def::CamCtlFlag_noright;
     }
     ///设置类型
     if(i == 0){
       camctl_tmp._type=ns_tcs_ds_def::CameraType_to_ground;
     }
     else if(i == 1){
       camctl_tmp._type=ns_tcs_ds_def::CameraType_to_air;
     }
     else if(i == 2){
       camctl_tmp._type=ns_tcs_ds_def::CameraType_direct;
     }
     else{
       throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                             +":Error(no such selection)!");
     }


     l_camctl.push_back(camctl_tmp);
   }
 }
 return l_camctl;
}



/// 对于top项，先执行与其他项的交互还是先执行自身的交互:自身
/// 由于同步时会产生无限循环过程，在在函数执行过程中调用该函数会被跳过
/// 步骤
/// 判断改变项，然后读取另一项，是否是符合规则
///   若自身改为read check ,则合理
///   若自身改为read uncheck,若crtl check 则不合理，应设置crtl为uncheck
///   若自身改为crtl check ,若read uncheck 不合理，该为read check
///   合理
/// 然后判断自身是否是top项,
///   若为top项，则更新子项的状态
///     top项有三种状态，子项均设置为相同状态
///   若为子项，判断所有子项的状态
///     若全为check 状态就设置为check状态
///     否则设置为uncheck
void UserItem::on_treeWidget_itemClicked(QTreeWidgetItem *v_item, int column)
{
  /// 使子身状态正确
  Qt::CheckState checkstate_read_tmp=v_item->checkState(1);
  Qt::CheckState checkstate_control_tmp=v_item->checkState(2);


  if(checkstate_read_tmp == Qt::CheckState::Unchecked
     && checkstate_control_tmp == Qt::CheckState::Checked){
    if(column == 1){
      v_item->setCheckState(2,Qt::CheckState::Unchecked);
      checkstate_control_tmp=Qt::CheckState::Unchecked;
    }
    else if(column == 2){
      v_item->setCheckState(1,Qt::CheckState::Checked);
      checkstate_read_tmp=Qt::CheckState::Checked;
    }
    else{
      throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                            +": Error(wrong column)!");
    }
  }

  /// 对子项或者父项的影响
  bool is_topitem=false;
  for(int i=0;i<ui->treeWidget->topLevelItemCount();i++){
    if(v_item == ui->treeWidget->topLevelItem(i)){
      is_topitem =true;
      break;
    }
  }
  if(is_topitem == true){
    ///对于每个子项都设置为top项的状态
    for(int i=0;i<v_item->childCount();i++){
      v_item->child(i)->setCheckState(1,checkstate_read_tmp);
      v_item->child(i)->setCheckState(2,checkstate_control_tmp);
    }
  }
  else{
    ///对于每个子项都设置为top项的状态
    QTreeWidgetItem *item_parent=v_item->parent();
    Qt::CheckState all_child_read_check=Qt::CheckState::Checked;
    Qt::CheckState all_child_crtl_check=Qt::CheckState::Checked;

    for(int i=0;i<item_parent->childCount();i++){
      if(item_parent->child(i)->checkState(1)==Qt::CheckState::Unchecked){
        all_child_read_check= Qt::CheckState::Unchecked;
      }
      if(item_parent->child(i)->checkState(2)==Qt::CheckState::Unchecked){
        all_child_crtl_check= Qt::CheckState::Unchecked;
      }
    }

    item_parent->setCheckState(1,all_child_read_check);
    item_parent->setCheckState(2,all_child_crtl_check);

  }

}


ns_tcs_ds_def::User UserItem::getUserInfo(){
  ns_tcs_ds_def::User v_user;
  v_user._password=ui->lineEdit_password->text().toStdString();
  v_user._username=ui->lineEdit_username->text().toStdString();

  int index_type=ui->combox_usertype->currentIndex();
  if(index_type == 0){
    v_user._type=ns_tcs_ds_def::UserType_admin;
  }
  else{
    v_user._type=ns_tcs_ds_def::UserType_ordinary;
  }

  v_user._cams=getCamCtl();
  return v_user;
}




