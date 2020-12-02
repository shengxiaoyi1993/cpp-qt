#include "directcamview.h"
#include "ui_directcamview.h"
#include "ds_def/tcs_ds_def.h"
#include <QDebug>
#include <QStyle>


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
        ui->radioButton_00,
        ui->radioButton_01,
        ui->radioButton_02,
        ui->radioButton_03,
        ui->radioButton_04,

        ui->radioButton_10,
        ui->radioButton_20,
        ui->radioButton_30,
        ui->radioButton_40,
        ui->radioButton_50,
        ui->radioButton_60,
        ui->radioButton_70,

        ui->radioButton_14,
        ui->radioButton_24,
        ui->radioButton_34,
        ui->radioButton_44,
        ui->radioButton_54,
        ui->radioButton_64,
        ui->radioButton_74,

        ui->radioButton_80,
        ui->radioButton_81,
        ui->radioButton_82,
        ui->radioButton_83,
        ui->radioButton_84

    };


    QPushButton t;

    for(uint i=0;i<_l_pushbutton_a.size();i++){
        _l_pushbutton_a[i]->setChecked(true);

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
        QPoint(0,1),
        QPoint(0,2),
        QPoint(0,3),
        QPoint(0,4),

        QPoint(1,0),
        QPoint(2,0),
        QPoint(3,0),
        QPoint(4,0),
        QPoint(5,0),
        QPoint(6,0),
        QPoint(7,0),

        QPoint(1,4),
        QPoint(2,4),
        QPoint(3,4),
        QPoint(4,4),
        QPoint(5,4),
        QPoint(6,4),
        QPoint(7,4),

        QPoint(8,0),
        QPoint(8,1),
        QPoint(8,2),
        QPoint(8,3),
        QPoint(8,4)
    };

    _l_pad_move={
        QPoint(0,-1),
        QPoint(-1,0),
        QPoint(0,1),
        QPoint(1,0)
    };
    ui->radioButton_00->setChecked(true);



}

DirectCamView::~DirectCamView()
{
    delete ui;
}
void DirectCamView::mousePressEvent(QMouseEvent *event){
    _point_cursor=event->pos();
    qDebug()<<"_point_cursor:"<<_point_cursor;

    qDebug()<<"event->pos:"<<event->pos();
    qDebug()<<"event->globalPos:"<<event->globalPos();


}
void DirectCamView::setLeftTop(const QPoint& v_point){
    _point_lefttop=v_point;
}




/// 先判断是那个按钮被触发
void DirectCamView::on_pushbutton_a_click(){


//    qDebug()<<"_l_pushbutton_a[0]:"<<_l_pushbutton_a[0]->rect();
//    qDebug()<<"_l_pushbutton_a[0]:"<<_l_pushbutton_a[0]->geometry();
//    qDebug()<<"_l_pushbutton_a[1]:"<<_l_pushbutton_a[1]->geometry();


//    qDebug()<<"gridLayout:"<<ui->gridLayout->geometry().topLeft();
//    qDebug()<<"groupBox:"<<ui->groupBox->geometry().topLeft();
//    qDebug()<<"gridLayout_5:"<<ui->gridLayout_5->geometry().topLeft();
//    qDebug()<<"gridLayout_7:"<<ui->gridLayout_7->geometry().topLeft();
//    qDebug()<<"cursor().pos():"<<cursor().pos();
//    qDebug()<<"_point_cursor:"<<_point_cursor;
//    qDebug()<<"_point_lefttop:"<<_point_lefttop;





    for(uint i=0;i<_l_pushbutton_a.size();i++){

        if(_l_pushbutton_a[i]->geometry().contains(this->mapFromGlobal(ui->groupBox->mapFromParent(cursor().pos())))){
            _index_cam_a =i;
            activateCertainButtonA(i);

        }
    }

}

void DirectCamView::activateCertainButtonA(uint v_seq){
    for (uint i=0;i<_l_pushbutton_a.size() ;i++ ) {
        if (i != v_seq) {
            _l_pushbutton_a[i]->setStyleSheet("QPushButton{}");
        }
        else{
            _l_pushbutton_a[i]->setStyleSheet("QPushButton{background-color: rgb(255, 85, 0);}");
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
        _index_cam_a=0;
        activateCertainButtonA(0);
    }

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
            v_point=_l_pad_move[i]*ui->comboBox->currentText().toInt();
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
                                     _l_cam_a[_index_cam_a]._camname,
                                     ns_tcs_ds_def::PointAndPad(
                                         ns_tcs_ds_def::Point(point_tmp.rx(),point_tmp.ry()),
                                         ns_tcs_ds_def::Pad(pad_tmp.rx(),pad_tmp.ry()))
                                     ));
    }
}

