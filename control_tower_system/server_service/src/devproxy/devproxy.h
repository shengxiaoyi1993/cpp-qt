#ifndef DEVPROXY_H
#define DEVPROXY_H

#include <QObject>
#include <vector>
#include <iostream>
#include "basecam.h"
#include "../ds_def/tcs_ds_def.h"

/// 该类为设备的管理类
/// 一经初始化就开试运行，监控设备的运行情况
class DevProxy : public QObject
{
  Q_OBJECT
public:
  explicit DevProxy(QObject *parent = nullptr);

  /// 检索已运行的程序中缺少的设备，进行初始化，添加到已运行设备列表，更新状态
  void setCameraArray(const ns_tcs_ds_def::CameraArray& v_array);

   vector<ns_tcs_ds_def::CamStatus> getCamStatusList()const {return _list_camstatus;}
   int opDirectCam(const ns_tcs_ds_def::DirectCamOp &v_data);

signals:
   void sgl_camDisconnect(string v_camname);
   void sgl_camReconnect(string v_camname);


public slots:
   void slot_onCamDisconnect(string v_cam);

private:
  vector<ns_tcs_ds_def::CamStatus> _list_camstatus;
  vector<DirectCam*> _list_directcam;
  vector<NormalCam*> _list_cam_toair;
  vector<NormalCam*> _list_cam_toground;

};

#endif // DEVPROXY_H
