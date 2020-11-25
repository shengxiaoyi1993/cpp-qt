#include "directcamview.h"
#include "ui_directcamview.h"
#include "../../../server_service/src/ds_def/tcs_ds_def.h"
#include <QDebug>


DirectCamView::DirectCamView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DirectCamView)
{
  ui->setupUi(this);
  setAllDisable();
  _l_pushbutton_a.push_back(ui->pushButton_a1);
  _l_pushbutton_a.push_back(ui->pushButton_a2);
  _l_pushbutton_a.push_back(ui->pushButton_a3);
  _l_pushbutton_a.push_back(ui->pushButton_a4);
  _l_pushbutton_a.push_back(ui->pushButton_a5);
  _l_pushbutton_a.push_back(ui->pushButton_a6);
  _l_pushbutton_a.push_back(ui->pushButton_a7);
  _l_pushbutton_a.push_back(ui->pushButton_a8);
  _l_pushbutton_a.push_back(ui->pushButton_a9);
  _l_pushbutton_a.push_back(ui->pushButton_a10);
  _l_pushbutton_a.push_back(ui->pushButton_a11);
  _l_pushbutton_a.push_back(ui->pushButton_a12);

  _l_pushbutton_b.push_back(ui->pushButton_b1);
  _l_pushbutton_b.push_back(ui->pushButton_b2);
  _l_pushbutton_b.push_back(ui->pushButton_b3);
  _l_pushbutton_b.push_back(ui->pushButton_b4);
  _l_pushbutton_b.push_back(ui->pushButton_b5);
  _l_pushbutton_b.push_back(ui->pushButton_b6);
  _l_pushbutton_b.push_back(ui->pushButton_b7);
  _l_pushbutton_b.push_back(ui->pushButton_b8);
  _l_pushbutton_b.push_back(ui->pushButton_b9);
  _l_pushbutton_b.push_back(ui->pushButton_b10);
  _l_pushbutton_b.push_back(ui->pushButton_b11);
  _l_pushbutton_b.push_back(ui->pushButton_b12);

  _l_pushbutton_move={
    ui->pushButton_up,
    ui->pushButton_left,
    ui->pushButton_down,
    ui->pushButton_right
  };

  _l_radiobutton_point={
    ui->radioButton_tl,
    ui->radioButton_tm,
    ui->radioButton_tr,
    ui->radioButton_ml,
    ui->radioButton_mm,
    ui->radioButton_mr,
    ui->radioButton_bl,
    ui->radioButton_bm,
    ui->radioButton_br
  };


  for(uint i=0;i<_l_pushbutton_a.size();i++){
    connect((_l_pushbutton_a[i]),SIGNAL(clicked()),
            this,SLOT(on_pushbutton_a_click()));
  }

  for(uint i=0;i<_l_pushbutton_move.size();i++){
    connect((_l_pushbutton_move[i]),SIGNAL(clicked()),
            this,SLOT(on_pushbutton_op()));
  }


  //  setAllDisable();

  _l_point={
    QPoint(0,0),
    QPoint(10,0),
    QPoint(20,0),
    QPoint(0,10),
    QPoint(10,10),
    QPoint(20,10),
    QPoint(0,20),
    QPoint(10,20),
    QPoint(20,20)
  };

  _l_pad_move={
    QPoint(0,1),
    QPoint(-1,0),
    QPoint(-1,0),
    QPoint(0,1)
  };
  ui->radioButton_tl->setChecked(true);



}

DirectCamView::~DirectCamView()
{
  delete ui;
}


/// 先判断是那个按钮被触发
void DirectCamView::on_pushbutton_a_click(){
  for(uint i=0;i<_l_pushbutton_a.size();i++){
//    qDebug()<<_l_pushbutton_a[i]->geometry();
//    qDebug()<<ui->gridLayout_7->geometry();

//    qDebug()<<this->mapFromGlobal(cursor().pos());
//    qDebug()<<"cursor:"<<cursor().pos();
//    qDebug()<<"gridLayout:"<<ui->gridLayout->geometry().topLeft();
//    qDebug()<<"groupBox:"<<ui->groupBox->geometry().topLeft();
//    qDebug()<<"gridLayout_5:"<<ui->gridLayout_5->geometry().topLeft();
//    qDebug()<<"gridLayout_7:"<<ui->gridLayout_7->geometry().topLeft();


//    qDebug()<<cursor().pos()
//              -ui->gridLayout->geometry().topLeft()
//              -ui->groupBox->geometry().topLeft()
              //              -ui->gridLayout_5->geometry().topLeft()
              //              -this->geometry().topLeft()
              ;


    if(_l_pushbutton_a[i]->geometry().contains(cursor().pos()
                                               -ui->gridLayout->geometry().topLeft()
                                               -ui->groupBox->geometry().topLeft()
                                               )){
      cout<<"_cam change"<<endl;
      //      _cam=_l_cam_a[i];
    }
  }

}


void DirectCamView::on_pushbutton_b_click(){
  throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"+string(__FUNCTION__)
                        +":Error（unaccomplish）!");
}


void DirectCamView::setAllDisable(){

  for(uint i=0;i<_l_pushbutton_a.size();i++){
    _l_pushbutton_a[i]->setText("Cam"+QString::number(i));
    _l_pushbutton_a[i]->setDisabled(true);
  }

  for(uint i=0;i<_l_pushbutton_b.size();i++){
    _l_pushbutton_b[i]->setText("Cam"+QString::number(i));
    _l_pushbutton_b[i]->setDisabled(true);
  }

}


/// 对于每一个设备全都设置一个可选的按钮
/// 当点击该按钮后，当前编辑的设备切换到指定按钮
void DirectCamView::updateDirctCamList(const vector<ns_tcs_ds_def::CamDev>  &l_cam){
  setAllDisable();
  _l_cam_a=l_cam;

  if (_l_cam_a.size()>0) {
    _cam=_l_cam_a[0];
  }
  cout<<"l_cam.size():"<<l_cam.size()<<endl;
  cout<<"_l_pushbutton_a.size():"<<_l_pushbutton_a.size()<<endl;

  for (uint i=0;i<l_cam.size();i++) {
    if(i<_l_pushbutton_a.size()){
      _l_pushbutton_a[i]->setDisabled(false);
      _l_pushbutton_a[i]->setText(QString::fromStdString(l_cam[i]._camname));
    }
  }
}

int DirectCamView::getPoint(QPoint &v_point){
  for(uint i=0;i<_l_radiobutton_point.size();i++){
    if(_l_radiobutton_point[i]->isChecked()){
      v_point=_l_point[i];
      return 0;
    }
  }
  cout<<__func__<<endl;
  return -1;
}

int DirectCamView::getMvPad(QPoint &v_point){
  for(uint i=0;i<_l_pushbutton_move.size();i++){

    if(_l_pushbutton_move[i]->geometry().contains(this->mapFromGlobal(cursor().pos()))){
      v_point=_l_pad_move[i];
      return 0;
    }
  }
  cout<<__func__<<endl;

  return -1;
}

void DirectCamView::on_pushbutton_op(){
  QPoint point_tmp;
  QPoint pad_tmp;
  cout<<__func__<<endl;
  if(getPoint(point_tmp)==0&&getMvPad(pad_tmp)==0){
    cout<<__func__<<"get para"<<endl;
    emit sgl_directcamera_op(ns_tcs_ds_def::DirectCamOp(
                               _cam._camname,
                               ns_tcs_ds_def::PointAndPad(
                                 ns_tcs_ds_def::Point(point_tmp.rx(),point_tmp.ry()),
                                 ns_tcs_ds_def::Pad(pad_tmp.rx(),pad_tmp.ry()))
                               ));
  }
}

