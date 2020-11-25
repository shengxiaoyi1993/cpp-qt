#ifndef DIRECTCAMVIEW_H
#define DIRECTCAMVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QPoint>
#include <vector>
#include <string>
#include "../../../server_service/src/ds_def/tcs_ds_def.h"

/// 直显相机操作界面
/// 接口
///   1. 根据server-service的数据将改控件进行初始化 所需要的数据有该用户权限下的直显相机列表
///   2. 响应用户的操作，转换操作参数，向controller发出信号
///   3. 用按钮调整当前操作界面控制的设备


using namespace std;

namespace Ui {
class DirectCamView;
}

class DirectCamView : public QWidget
{
  Q_OBJECT

public:
  explicit DirectCamView(QWidget *parent = nullptr);
  ~DirectCamView();
  void updateDirctCamList(const vector<ns_tcs_ds_def::CamDev>  &l_cam);

private:
  Ui::DirectCamView *ui;
  vector<ns_tcs_ds_def::CamDev> _l_cam_a;
  ns_tcs_ds_def::CamDev _cam;
  vector<QPushButton*>  _l_pushbutton_a;
  vector<QPushButton*>  _l_pushbutton_b;
  vector<QPushButton*>  _l_pushbutton_move;
  vector<QRadioButton*>  _l_radiobutton_point;
  void setAllDisable();
  int getPoint(QPoint &v_point);

  vector<QPoint> _l_point;
  vector<QPoint> _l_pad_move;
  int getMvPad(QPoint &v_point);



signals:
  void sgl_directcamera_op( ns_tcs_ds_def::DirectCamOp v_data);

private slots:
  void on_pushbutton_a_click();
  void on_pushbutton_b_click();
  void on_pushbutton_op();
};

#endif // DIRECTCAMVIEW_H
