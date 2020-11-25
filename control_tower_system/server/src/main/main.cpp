#include "mainwindow.h"
#include <QApplication>
#include "api_client/apiclient.h"
#include "../controller/controller.h"

void testAPI();
int main(int argc, char *argv[])
{
//  MainWindow w;
//    w.show();
//    testAPI();

  int execcode=0;
  do {
    QApplication a(argc, argv);
    controller control;
    execcode=a.exec();
  } while (execcode == controller::REBOOT_CODE);

  return execcode;
}


void testAPI(){
  APIClient apiclient;
  AuthParam authpara;
  memset(authpara.server_ip,0,sizeof (authpara.server_ip));
  memset(authpara.username,0,sizeof (authpara.username));
  memset(authpara.password,0,sizeof (authpara.password));

  string ip="192.168.1.126";
  string user="admin";
  string pw="admin";
  memcpy(authpara.server_ip,ip.c_str(),ip.size());
  memcpy(authpara.username,user.c_str(),user.size());
  memcpy(authpara.password,pw.c_str(),pw.size());

  apiclient.setAuthPara(authpara);
  int flag_login=apiclient.login();
  cout<<"flag_login:"<<flag_login<<endl;
  unsigned timecode;
  apiclient.getServerTimeCode(timecode);
  cout<<"timecode:"<<timecode<<endl;


  ///req_querySINGLEUSER
  ns_tcs_ds_def::User v_user;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag=apiclient.req_querySINGLEUSER("admin",v_user);
  if(flag == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<v_user.jsonobj().ToString()<<endl;
  }
  cout<<"req_querySINGLEUSER:"<<flag<<endl;


  ///req_queryALLUSER
  vector<ns_tcs_ds_def::User> list_user;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryALLUSER=apiclient.req_queryALLUSER(list_user);
  if(flag_req_queryALLUSER == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<"req_queryALLUSER:"
       <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::User>(list_user).ToString()<<endl;
  }
  cout<<"req_queryALLUSER:"<<flag_req_queryALLUSER<<endl;


  /// req_queryRUNTME
  ns_tcs_ds_def::TimeRecord time_info;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryRUNTME=apiclient.req_queryRUNTME(time_info);
  if(flag_req_queryRUNTME == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<"req_queryRUNTME:"<<time_info.jsonobj().ToString()<<endl;
  }
  cout<<"req_queryRUNTME:"<<flag_req_queryRUNTME<<endl;


  ///req_queryUSERSTAUS
  vector<ns_tcs_ds_def::UserStatus> list_userstatus;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryUSERSTAUS=apiclient.req_queryUSERSTAUS(list_userstatus);
  if(flag_req_queryUSERSTAUS == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<"req_queryUSERSTAUS:"
       <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::UserStatus>(list_userstatus).ToString()<<endl;
  }
  cout<<"req_queryUSERSTAUS:"<<flag_req_queryUSERSTAUS<<endl;



  /// req_queryRUNTME
  ns_tcs_ds_def::CameraArray camarray;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVARRAY=apiclient.req_queryDEVARRAY(camarray);
  if(flag_req_queryDEVARRAY == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<"req_queryDEVARRAY:"<<camarray.jsonobj().ToString()<<endl;
  }
  cout<<"req_queryDEVARRAY:"<<flag_req_queryDEVARRAY<<endl;


  ///req_queryUSERSTAUS
  vector<ns_tcs_ds_def::CamStatus> list_camstatus;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVSTATUS=apiclient.req_queryDEVSTATUS(list_camstatus);
  if(flag_req_queryDEVSTATUS == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<"req_queryDEVSTATUS:"
       <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::CamStatus>(list_camstatus).ToString()<<endl;
  }
  cout<<"req_queryDEVSTATUS:"<<flag_req_queryDEVSTATUS<<endl;


  /// req_queryQRYNVR
  ns_tcs_ds_def::NvrDev nvrdev;
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryQRYNVR=apiclient.req_queryQRYNVR(nvrdev);
  if(flag_req_queryQRYNVR == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
    cout<<"req_queryQRYNVR:"<<nvrdev.jsonobj().ToString()<<endl;
  }
  cout<<"req_queryQRYNVR:"<<flag_req_queryQRYNVR<<endl;



  /// req_queryADDDEV
  ns_tcs_ds_def::CamDev camdev_add(
        "driectcam0",
        "192.168.1.222",
        "",
        "",
        "tty0",
        ns_tcs_ds_def::CameraType_direct
        );
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryADDDEV=apiclient.req_queryADDDEV(camdev_add);
  cout<<"req_queryADDDEV:"<<flag_req_queryADDDEV<<endl;

  {
    cout<<"=========================req_queryADDDEV result start======================"<<endl;
    /// req_queryRUNTME
    ns_tcs_ds_def::CameraArray camarray;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVARRAY=apiclient.req_queryDEVARRAY(camarray);
    if(flag_req_queryDEVARRAY == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryDEVARRAY:"<<camarray.jsonobj().ToString()<<endl;
    }
    cout<<"req_queryDEVARRAY:"<<flag_req_queryDEVARRAY<<endl;


    ///req_queryUSERSTAUS
    vector<ns_tcs_ds_def::CamStatus> list_camstatus;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVSTATUS=apiclient.req_queryDEVSTATUS(list_camstatus);
    if(flag_req_queryDEVSTATUS == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryDEVSTATUS:"
         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::CamStatus>(list_camstatus).ToString()<<endl;
    }
    cout<<"req_queryDEVSTATUS:"<<flag_req_queryDEVSTATUS<<endl;

    cout<<"=========================req_queryADDDEV result end======================"<<endl;

  }


  ///
  ns_tcs_ds_def::User data_add("sxy","123456",ns_tcs_ds_def::UserType_ordinary,{});
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryADDUSER=apiclient.req_queryADDUSER(data_add);
  cout<<"req_queryADDUSER:"<<flag_req_queryADDUSER<<endl;
  {
    cout<<"=========================req_queryADDUSER result start======================"<<endl;

    ///req_queryALLUSER
    vector<ns_tcs_ds_def::User> list_user;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryALLUSER=apiclient.req_queryALLUSER(list_user);
    if(flag_req_queryALLUSER == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryALLUSER:"
         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::User>(list_user).ToString()<<endl;
    }
    cout<<"req_queryALLUSER:"<<flag_req_queryALLUSER<<endl;

    cout<<"=========================req_queryADDUSER result end======================"<<endl;


  }


  //  ///
  //  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_DELDEV=apiclient.req_DELDEV("driectcam0");
  //  cout<<"req_DELDEV:"<<flag_req_DELDEV<<endl;
  //  {
  //    cout<<"=========================req_DELDEV result start======================"<<endl;
  //    /// req_queryRUNTME
  //    ns_tcs_ds_def::CameraArray camarray;
  //    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVARRAY=apiclient.req_queryDEVARRAY(camarray);
  //    if(flag_req_queryDEVARRAY == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
  //      cout<<"req_queryDEVARRAY:"<<camarray.jsonobj().ToString()<<endl;
  //    }
  //    cout<<"req_queryDEVARRAY:"<<flag_req_queryDEVARRAY<<endl;


  //    ///req_queryUSERSTAUS
  //    vector<ns_tcs_ds_def::CamStatus> list_camstatus;
  //    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVSTATUS=apiclient.req_queryDEVSTATUS(list_camstatus);
  //    if(flag_req_queryDEVSTATUS == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
  //      cout<<"req_queryDEVSTATUS:"
  //         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::CamStatus>(list_camstatus).ToString()<<endl;
  //    }
  //    cout<<"req_queryDEVSTATUS:"<<flag_req_queryDEVSTATUS<<endl;

  //    cout<<"=========================req_DELDEV result end======================"<<endl;

  //  }



  //  ///
  //  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_DELUSER=apiclient.req_DELUSER("sxy");
  //  cout<<"req_DELUSER:"<<flag_req_DELUSER<<endl;
  //  {
  //    cout<<"=========================req_DELUSER result start======================"<<endl;

  //    ///req_queryALLUSER
  //    vector<ns_tcs_ds_def::User> list_user;
  //    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryALLUSER=apiclient.req_queryALLUSER(list_user);
  //    if(flag_req_queryALLUSER == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
  //      cout<<"req_queryALLUSER:"
  //         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::User>(list_user).ToString()<<endl;
  //    }
  //    cout<<"req_queryALLUSER:"<<flag_req_queryALLUSER<<endl;

  //    ///req_queryUSERSTAUS
  //    vector<ns_tcs_ds_def::UserStatus> list_userstatus;
  //    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryUSERSTAUS=apiclient.req_queryUSERSTAUS(list_userstatus);
  //    if(flag_req_queryUSERSTAUS == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
  //      cout<<"req_queryUSERSTAUS:"
  //         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::UserStatus>(list_userstatus).ToString()<<endl;
  //    }
  //    cout<<"req_queryUSERSTAUS:"<<flag_req_queryUSERSTAUS<<endl;

  //    cout<<"=========================req_DELUSER result end======================"<<endl;

  //  }


  ///
  ns_tcs_ds_def::NvrDev  nvrdev_set("192.168.1.2","nvradmin","nvr123");
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_SETNVR=apiclient.req_SETNVR(nvrdev_set);
  cout<<"req_SETNVR:"<<flag_req_SETNVR<<endl;
  {
    cout<<"=========================req_SETNVR result start======================"<<endl;

    /// req_queryQRYNVR
    ns_tcs_ds_def::NvrDev nvrdev;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryQRYNVR=apiclient.req_queryQRYNVR(nvrdev);
    if(flag_req_queryQRYNVR == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryQRYNVR:"<<nvrdev.jsonobj().ToString()<<endl;
    }
    cout<<"req_queryQRYNVR:"<<flag_req_queryQRYNVR<<endl;



    cout<<"=========================req_SETNVR result end======================"<<endl;

  }

  ///
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_EMPTYNVR=apiclient.req_EMPTYNVR();
  cout<<"req_EMPTYNVR:"<<flag_req_EMPTYNVR<<endl;
  {
    cout<<"=========================req_EMPTYNVR result start======================"<<endl;

    /// req_queryQRYNVR
    ns_tcs_ds_def::NvrDev nvrdev;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryQRYNVR=apiclient.req_queryQRYNVR(nvrdev);
    if(flag_req_queryQRYNVR == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryQRYNVR:"<<nvrdev.jsonobj().ToString()<<endl;
    }
    cout<<"req_queryQRYNVR:"<<flag_req_queryQRYNVR<<endl;

    cout<<"=========================req_EMPTYNVR result end======================"<<endl;

  }


  ///
  ns_tcs_ds_def::CamDev camdev_set(
        "driectcam0",
        "192.168.1.456",
        "",
        "",
        "tty0",
        ns_tcs_ds_def::CameraType_direct
        );
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_SETDEV=apiclient.req_SETDEV(camdev_set);
  cout<<"req_SETDEV:"<<flag_req_SETDEV<<endl;
  {
    cout<<"=========================req_SETDEV result start======================"<<endl;
    /// req_queryRUNTME
    ns_tcs_ds_def::CameraArray camarray;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVARRAY=apiclient.req_queryDEVARRAY(camarray);
    if(flag_req_queryDEVARRAY == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryDEVARRAY:"<<camarray.jsonobj().ToString()<<endl;
    }
    cout<<"req_queryDEVARRAY:"<<flag_req_queryDEVARRAY<<endl;


    ///req_queryUSERSTAUS
    vector<ns_tcs_ds_def::CamStatus> list_camstatus;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryDEVSTATUS=apiclient.req_queryDEVSTATUS(list_camstatus);
    if(flag_req_queryDEVSTATUS == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryDEVSTATUS:"
         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::CamStatus>(list_camstatus).ToString()<<endl;
    }
    cout<<"req_queryDEVSTATUS:"<<flag_req_queryDEVSTATUS<<endl;

    cout<<"=========================req_SETDEV result end======================"<<endl;

  }


  ///
  ns_tcs_ds_def::User user_set("sxy","66666",ns_tcs_ds_def::UserType_ordinary,{});
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_SETUSER=apiclient.req_SETUSER(user_set);
  cout<<"req_SETUSER:"<<flag_req_SETUSER<<endl;
  {
    cout<<"=========================req_SETUSER result start======================"<<endl;
    ///req_queryALLUSER
    vector<ns_tcs_ds_def::User> list_user;
    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_queryALLUSER=apiclient.req_queryALLUSER(list_user);
    if(flag_req_queryALLUSER == ns_tcs_ds_def::ENUM_HTTP_ERRCODE_SUCCEED){
      cout<<"req_queryALLUSER:"
         <<ns_tcs_ds_def::jsonFromArray<ns_tcs_ds_def::User>(list_user).ToString()<<endl;
    }
    cout<<"req_queryALLUSER:"<<flag_req_queryALLUSER<<endl;

    cout<<"=========================req_SETUSER result end======================"<<endl;
  }

  ///
  ns_tcs_ds_def::DirectCamOp directcamop("driectcam0",ns_tcs_ds_def::PointAndPad(
                                           ns_tcs_ds_def::Point(1,2),ns_tcs_ds_def::Pad(3,4)));
  ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_DIRECTCAM_OP=apiclient.req_DIRECTCAM_OP(directcamop);
  cout<<"req_DIRECTCAM_OP:"<<flag_req_DIRECTCAM_OP<<endl;


  //    ///
  //    ns_tcs_ds_def::ENUM_HTTP_ERRCODE flag_req_STOPSERVICE=apiclient.req_STOPSERVICE();
  //    cout<<"req_STOPSERVICE:"<<flag_req_STOPSERVICE<<endl;


}
