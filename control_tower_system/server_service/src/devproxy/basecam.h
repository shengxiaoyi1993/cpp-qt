#ifndef BASECAM_H
#define BASECAM_H

#include <QObject>
#include <string>
#include <iostream>
#include <QSerialPort>
#include <fstream>
#include "../../../common_component/ds_def/tcs_ds_def.h"
//#include "../../../common_component/syslogger/syslogger.h"

#include "uartconvert.h"


using namespace std;

class BaseCam
{
public:
  BaseCam(){
    cout<<__func__<<endl;

  }
  virtual ~BaseCam(){
    cout<<__func__<<endl;

  }

  virtual bool isConnected()=0;
  virtual int connect()=0;
  virtual int disConnect()=0;

};

class DirectCam:public QObject,public BaseCam
{
  Q_OBJECT
public:
  DirectCam(QObject *parent = nullptr):QObject(parent),_isconnected(false){

    cout<<__func__<<endl;

  }

  DirectCam(const DirectCam& v_vam ,QObject *parent = nullptr)
      :QObject(parent),_isconnected(false){
    if(&v_vam != this){
      this->_isconnected=v_vam._isconnected;
      this->_camname=v_vam._camname;
      this->_tty=v_vam._tty;

    }
  }


  ~DirectCam(){
    cout<<__func__<<endl;
    disConnect();
  }

  int setTTY(const string& v_camname,const string& v_cam){
    _camname=v_camname;
    _tty=v_cam;

    return 0;
  }

  int op(const ns_tcs_ds_def::PointAndPad &v_pad);
  int op_pad(const ns_tcs_ds_def::PointAndPad &v_pad);



  bool isConnected(){
    return _serial_port.isOpen();
  }

  int connect()
  {
    _isconnected=true;
    _serial_port.setPortName(QString::fromStdString(_tty));
    _serial_port.setBaudRate(115200);
    _serial_port.open(QSerialPort::ReadWrite);
    _logstreamm.open(_tty+".log");
    if (_serial_port.isOpen()) {
        initCam();
        return 0;
    }

    return -1;
  }


  int disConnect(){
      _serial_port.close();
      if (!_serial_port.isOpen()) {
          return 0;
      }

    return -1;
  }




  string camName()const{return _camname;}


signals:
  void sgl_disconnect(string v_camname);

public slots:

private:
  string _camname;
  string _tty;
  QSerialPort _serial_port;
  bool _isconnected;
  ofstream _logstreamm;
  unsigned int _array_addr[5][9];
  ns_tcs_ds_def::Pad _array_pad[5][9];
  void getRegValue();
  int initCam();


};


class NormalCam:public QObject,public BaseCam
{
  Q_OBJECT
public:
  NormalCam(QObject *parent = nullptr):QObject(parent),_isconnected(false){
    cout<<__func__<<endl;

  }
  NormalCam(const NormalCam& v_vam ,QObject *parent = nullptr):QObject(parent){
    if(&v_vam != this){
      this->_isconnected=v_vam._isconnected;
      this->_camname=v_vam._camname;
      this->_ip=v_vam._ip;
      this->_username=v_vam._username;
      this->_password=v_vam._password;
    }
  }

  ~NormalCam(){
    cout<<__func__<<endl;
    disConnect();
  }

  int setInfo(const string&v_name,const string&v_ip,const string&v_user,const string&v_pw){
    _camname=v_name;
    _ip=v_ip;
    _username=v_user;
    _password=v_pw;
    return 0;
  }

  bool isConnected(){
    return _isconnected;
  }

  int connect()
  {
    _isconnected=true;
    return 0;
  }
  int disConnect(){
    _isconnected=false;
    return 0;
  }
  string camName()const{return _camname;}

signals:
  void sgl_disconnect(string v_camname);

public slots:

private:
  bool _isconnected;
  string _camname;
  string _ip;
  string _username;
  string _password;
};





#endif // BASECAM_H
