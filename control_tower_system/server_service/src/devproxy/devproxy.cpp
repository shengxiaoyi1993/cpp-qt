#include "devproxy.h"
#include <iterator>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>


DevProxy::DevProxy(QObject *parent) : QObject(parent)
{

}


void DevProxy::setCameraArray(const ns_tcs_ds_def::CameraArray& v_array){
  for(auto it=v_array.air_camera_list.begin();it<v_array.air_camera_list.end();it++){
    auto it_pos=find_if(_list_cam_toair.begin(),_list_cam_toair.end(),
                        [=](NormalCam* &v_cam){
      return (v_cam->camName()==it->_camname);
    });
    if(it_pos==_list_cam_toair.end()){
      NormalCam *tmp=new NormalCam();
      tmp->setInfo(it->_camname,it->_ip,it->_username,it->_password);
      int flag= tmp->connect();
      if(flag == 0){
        cout<<"_list_camstatus.push_back0"<<endl;
        _list_camstatus.push_back(ns_tcs_ds_def::CamStatus(it->_camname,true));
      }
      connect(tmp,SIGNAL(sgl_disconnect(string)),this,SLOT(slot_onCamDisconnect(string)));
      _list_cam_toair.push_back(tmp);
    }
  }

  for(auto it=v_array.ground_camera_list.begin();it<v_array.ground_camera_list.end();it++){
    auto it_pos=find_if(_list_cam_toground.begin(),_list_cam_toground.end(),
                        [=](NormalCam* &v_cam){
      return (v_cam->camName()==it->_camname);
    });
    if(it_pos==_list_cam_toground.end()){
      NormalCam *tmp=new NormalCam();
      tmp->setInfo(it->_camname,it->_ip,it->_username,it->_password);
      int flag= tmp->connect();
      if(flag == 0){
        cout<<"_list_camstatus.push_back1"<<endl;
        _list_camstatus.push_back(ns_tcs_ds_def::CamStatus(it->_camname,true));
      }
      connect(tmp,SIGNAL(sgl_disconnect(string)),this,SLOT(slot_onCamDisconnect(string)));
      _list_cam_toground.push_back(tmp);
    }
  }

  for(auto it=v_array.direct_camera_list.begin();it<v_array.direct_camera_list.end();it++){
    auto it_pos=find_if(_list_directcam.begin(),_list_directcam.end(),
                        [=](/*vector<DirectCam>::iterator v_it*/DirectCam* &v_cam){
      return (v_cam->camName()==it->_camname);
    });
    if(it_pos==_list_directcam.end()){
      DirectCam *tmp=new DirectCam();
      tmp->setTTY(it->_camname,it->_tty);
     int flag= tmp->connect();

     if(flag == 0){
       cout<<"_list_camstatus.push_back3"<<endl;

       _list_camstatus.push_back(ns_tcs_ds_def::CamStatus(it->_camname,true));
     }
     else{
         _list_camstatus.push_back(ns_tcs_ds_def::CamStatus(it->_camname,false));
     }

      connect(tmp,SIGNAL(sgl_disconnect(string)),this,SLOT(slot_onCamDisconnect(string)));
      _list_directcam.push_back(tmp);
    }
  }

}
void DevProxy::getSerialPortList(vector<string> &l_port){
    l_port.clear();
    QList<QSerialPortInfo> list_port=QSerialPortInfo::availablePorts();
    for (int i=0;i< list_port.size();i++ ) {
        l_port.push_back(list_port[i].portName().toStdString());
    }
//    return;
}



int DevProxy::rmCamDev(const string & v_cam){
  int ret=1;
  for (auto it=_list_directcam.begin();it!=_list_directcam.end();it++) {
    if((*it)->camName()==v_cam){
      delete *it;
      _list_directcam.erase(it);
      ret=0;
      break;
    }
  }
  for (auto it=_list_cam_toair.begin();it!=_list_cam_toair.end();it++) {
    if((*it)->camName()==v_cam){
      delete *it;
      _list_cam_toair.erase(it);
      ret=0;
      break;
    }
  }
  for (auto it=_list_cam_toground.begin();it!=_list_cam_toground.end();it++) {
    if((*it)->camName()==v_cam){
      delete *it;
      _list_cam_toground.erase(it);
      ret=0;

      break;
    }
  }

  for (auto it=_list_camstatus.begin();it!=_list_camstatus.end();it++) {
    if(it->_camname==v_cam){
      _list_camstatus.erase(it);
      ret=0;
      break;
    }
  }
  return ret;
}



int DevProxy::opDirectCam(const ns_tcs_ds_def::DirectCamOp &v_data){
//    cout<<v_data.jsonobj().ToFormattedString()<<endl;
  auto pos_it=find_if(_list_directcam.begin(),_list_directcam.end(),[=](DirectCam* &v_cam){
    return (v_cam->camName()==v_data._camname);
  });
  if(pos_it!=_list_directcam.end()){
    return (*pos_it)->op_pad(v_data._op);
  }
  return -1;

}


void DevProxy::slot_onCamDisconnect(string v_cam){
  auto it_pos=find_if(_list_camstatus.begin(),_list_camstatus.end(),
                      [=](ns_tcs_ds_def::CamStatus &v_camst){
    return (v_camst._camname==v_cam);
  });

  if(it_pos != _list_camstatus.end()){
    it_pos->_status=false;
  }
  emit sgl_camDisconnect(v_cam);
}
