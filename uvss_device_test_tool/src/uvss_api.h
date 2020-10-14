#ifndef UVSS_API_H
#define UVSS_API_H

#include<iostream>
#include<QString>
#include<QJsonObject>
#include<QStringList>
#include<QJsonDocument>
#include<QJsonArray>
#include<QTcpServer>
#include<QRect>
#include<QVector>

namespace    uvss_api
{

enum DeviceManagerErrorType{
  Normal=1000,
  CanNotConnect=1001,
  CanNotGetInfo=1002,
  OperateFail=1003,
  QuitConnect=1004,
  Unauthorized = 1005
};


/**
  *  GetUVSSCamera ID by http request.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.178"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @param   res_id             save the wanted UVSSCamera to this param.
  *  @return                           return 1000 if normal
  *
  *  @example
  *  int operate_flag=dm::GetUVSSCameraID("192.168.1.178","root","admin",result_id);
  */

int GetUVSSCameraID(QString camera_IP,QString username
                    ,QString password,QString &res_id);




/**
  *  GetUVSSCamera Info by http request.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.178"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @param   delay_times   remain times to prolong the device use.
  *  @param   product           unit of the device.
  *  @param   region             the  device use in which region.
  *  @param   remain_time  the remain times to send image.
  *  @return                           return 1000 if normal
  *
  *  @example
  *  int operate_flag=dm::GetUVSSCameraID("192.168.1.178","root","admin",delay_times,product,region,remain_number);
  */
int GetUVSSCameraInfo(QString camera_IP,
                      QString username
                      ,QString password,
                      int &delay_times,
                      int &product,
                      QString &region,
                      int &remain_number);


/**
  *  Get server IP of UVSSCamera by http request.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.178"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @param   server_IP       save the wanted ServerIP to this param.
  *  @return                           return 1000 if normal
  *
  *  @example
  *  int operate_flag=dm::GetUVSSCameraID("192.168.1.178","root","admin",server_IP);
  */
int GetServerIPOfUVSSCamera(QString camera_IP,QString username
                            ,QString password,QString &server_IP);

/**
  *  Get test Image from UVSSCamera by http request.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.178"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @return                           return 1000 if normal
  *
  *  @example
  *  int operate_flag=dm::GetUVSSCameraID("192.168.1.178","root","admin");
  */
int RequestTestImageFromUVSSCamera(QString camera_IP,QString username
                                   ,QString password);


/**
  *  change ServerIP of UVSSCamera by http request.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.178"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @param   target_serverIP  serverIP of UVSSCamera is replaced by it.
  *  @return                           return 1000 if normal
  *
  *  @example
  *  int operate_flag=dm::GetUVSSCameraID("192.168.1.178","root","admin",new_serverIP);
  */
int ChangeServerIPOfUVSSCamera(QString camera_IP,QString username
                               ,QString password,QString  new_serverIP);

/**
  *   restart the UVSSCamera.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.178"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @return
  *
  *  @example
  *  int operate_flag=dm::RestartUVSSCamera("192.168.1.178","root","admin");
  */
int RestartUVSSCamera(QString camera_IP,QString username
                      ,QString password);

/**
  *   start to monitor the info. from UVSSCamera by port "60800".
  *
  *  @param   port                 the info. from UVSSCamera submit to it.
  *  @param int (*p_fun)(QString)   callback function in DBProcess to reveive info
  *  @return                           return 0 if successful
  *
  *  @example
  *  int operate_flag=dm::BuildPortToWaitInfoFromUVSSCamera(60800,dm::CallBackFunc)
  */
int BuildPortToWaitInfoFromUVSSCamera(int port,int (*p_fun)(QString&));

void StopListenUVSSCamera();

/**
  *   get image from UVSSCamera.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.118"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @param   image_name image_name in UVSSCamera.
  *  @param   image_path    save Image to it
  *  @return                           return 1000 if  normal
  *
  *  @example
  *GetImageFromUVSSCamera("192.168.1.178","root"
                                     , "admin", "jpeg_3_128.jpg","D:/test7.jpg" )
  */
int GetImageFromUVSSCamera(QString camera_IP,
                           QString username
                           ,QString password,
                           QString image_name,
                           QString image_path);


/**
  *   delete image from UVSSCamera.
  *
  *  @param   camera_IP      IP of UVSSCamera like "192.168.1.118"
  *  @param   username       Username to login to Camera.
  *  @param   password       Password to login to Camera.
  *  @param   image_name the image to delete.
  *  @return                          return 1000 if  succeed to delete
  *
  *  @example
  *
  */
int DeleteImageInUVSSCamera(QString camera_IP,
                            QString username,
                            QString password,
                            QString image_name);

int CheckUVSSCommunityAuthority(const QString &camera_IP,
                                const QString &username,
                                const QString &password);


};

#endif // UVSS_API_H
