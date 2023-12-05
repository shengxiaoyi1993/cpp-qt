#include <QCoreApplication>
#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/api_diss/dissapi.h"


void test_ds_def();


 int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                        void *ptr);

  int cb_CallbackOnHttpError(int error_code, void*ptr);


  void test_request();



int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
//  test_ds_def();
  test_request();

//  ns_DissApi::setupListenHttp(cb_CallbackOnNewHttpRequest,cb_CallbackOnHttpError,nullptr);



  return a.exec();
}


int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                       void *ptr){
  cout<<__func__<<endl;

}

 int cb_CallbackOnHttpError(int error_code, void*ptr){
   cout<<__func__<<endl;
 }


 void test_request(){
   ns_DissApi::initHTTPEnv();

   string id_client="192.168.1.126";

   {
     ds_df_diss::  Resources_JE  rsc_1(ds_df_diss:: Task_JE("server","client","proj",1,2),"name",1,2);
     unsigned char * pdata=nullptr;
     int datasize;
     int rval=ns_DissApi::ns_client::reqServerResource(id_client,rsc_1,&pdata,datasize);
     if (rval==0) {
       cout<<" succeed to reqServerResource"<<endl;
     }
     else{
       cout<<" fail to reqServerResource"<<endl;
     }
   }

   {
     ds_df_diss:: Task_JE  tk_1("server","client","proj",1,2);

     int rval=ns_DissApi::ns_client::reqTaskComplete(id_client,tk_1);
     if (rval==0) {
       cout<<" succeed to reqTaskComplete"<<endl;
     }
     else{
       cout<<" fail to reqTaskComplete"<<endl;
     }
   }

   ns_DissApi::uinitHTTPEnv();
 }
