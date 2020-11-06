#include <QCoreApplication>
#include <iostream>
#include "../datamanage/datamanage.h"
#include "../controler/controler.h"

using namespace std;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

//  DataManage::CamDev cam0("Cam0","192.168.1.168","admin","admin123","",DataManage::CameraType_to_air);
//  cout<<cam0.jsonobj().ToFormattedString()<<endl;

//  DataManage::CameraArray camarray;
//  camarray.addCam(DataManage::CamDev("Cam0","192.168.1.160","admin","admin123","",DataManage::CameraType_to_air));
//  camarray.addCam(DataManage::CamDev("Cam1","192.168.1.161","admin","admin123","",DataManage::CameraType_to_air));
//  camarray.addCam(DataManage::CamDev("Cam4","192.168.1.170","admin","admin123","",DataManage::CameraType_to_ground));
//  camarray.addCam(DataManage::CamDev("Cam5","192.168.1.171","admin","admin123","",DataManage::CameraType_to_ground));
//  camarray.addCam(DataManage::CamDev("Cam8","192.168.1.180","admin","admin123","",DataManage::CameraType_direct));
//  camarray.addCam(DataManage::CamDev("Cam9","192.168.1.181","admin","admin123","",DataManage::CameraType_direct));
//  cout<<camarray.jsonobj().ToFormattedString()<<endl;

//  neb::CJsonObject tmpobj=camarray.jsonobj();
//  DataManage::CameraArray camarray_cp(tmpobj);
//  cout<<camarray_cp.jsonobj().ToFormattedString()<<endl;


//  DataManage::DataColl datacoll("default.config");
//  cout<<datacoll.jsonobj().ToFormattedString()<<endl;
//  datacoll.addUser(DataManage::User("admin0","admin",DataManage::UserType_admin,{}));
//  cout<<datacoll.jsonobj().ToFormattedString()<<endl;
//  datacoll.addCam(DataManage::CamDev("Cam5","192.168.1.171","admin","admin123","",DataManage::CameraType_to_ground));
//  cout<<datacoll.jsonobj().ToFormattedString()<<endl;
//  datacoll.saveToFile();


  Controler ctlr("default.config");

  return a.exec();
}
