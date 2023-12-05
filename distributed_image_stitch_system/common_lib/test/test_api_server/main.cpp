#include <QCoreApplication>
#include <fstream>
#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/api_diss/dissapi.h"


void test_ds_def();


 int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                        void *ptr);

  int cb_CallbackOnHttpError(int error_code, void*ptr);

  class CDF{
  public:
    CDF(){
      ns_DissApi::initHTTPEnv();
      ns_DissApi::setupListenHttp(cb_CallbackOnNewHttpRequest,cb_CallbackOnHttpError,nullptr);
    }
    ~CDF(){
      ns_DissApi::uinitHTTPEnv();
    }
  };


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
//  test_ds_def();

//  ns_DissApi::setupListenHttp(cb_CallbackOnNewHttpRequest,cb_CallbackOnHttpError,nullptr);
CDF *tmp=new CDF();

  return a.exec();
}


int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                       void *ptr){
  string file_image="/home/sxy/Desktop/0_0_4.jpg";
  ifstream ifs(file_image);
  string tmp;

  ifs.seekg(0,std::ios::end);
  int len = ifs.tellg();
  ifs.seekg(0,std::ios::beg);

  char *buff = new char[len];

  ifs.read(buff , len);
  char* type="jpeg";
  cout<<"len:"<<len<<endl;
  SendHttpResponse(fd,type,(unsigned char*)buff,len);

  ns_DissApi::closeHttpConnection(fd);
  cout<<__func__<<endl;
  return 0;

}

 int cb_CallbackOnHttpError(int error_code, void*ptr){
   cout<<__func__<<endl;
   return 0;
 }


void test_ds_def(){

  ///====================================================
  {
    ds_df_diss::ClientStatus_JE cs_0;
    cout<<"cs_0:"<<cs_0.jsonObj().ToFormattedString()<<endl;

    ds_df_diss::ClientStatus_JE cs_1("",1,2,3);
    cout<<"cs_1:"<<cs_1.jsonObj().ToFormattedString()<<endl;

    ds_df_diss::ClientStatus_JE cs_3(cs_1.jsonObj());
    cout<<"cs_3:"<<cs_3.jsonObj().ToFormattedString()<<endl;
  }

  ///====================================================
  {
    ds_df_diss:: Task_JE  tk_0;
    cout<<" tk_0:"<< tk_0.jsonObj().ToFormattedString()<<endl;

    ds_df_diss:: Task_JE  tk_1("server","client","proj",1,2);
    cout<<" tk_1:"<< tk_1.jsonObj().ToFormattedString()<<endl;

    ds_df_diss:: Task_JE  tk_2( tk_1.jsonObj());
    cout<<" tk_2:"<< tk_2.jsonObj().ToFormattedString()<<endl;
  }

  ///====================================================
  {
    ds_df_diss::  Resources_JE  rsc_0;
    cout<<" rsc_0:"<< rsc_0.jsonObj().ToFormattedString()<<endl;


    ds_df_diss::  Resources_JE  rsc_1(ds_df_diss:: Task_JE("server","client","proj",1,2),"name",1,2);
    cout<<" rsc_1:"<< rsc_1.jsonObj().ToFormattedString()<<endl;

    ds_df_diss::  Resources_JE  rsc_2( rsc_1.jsonObj());
    cout<<" rsc_2:"<< rsc_2.jsonObj().ToFormattedString()<<endl;
  }

  ///====================================================
  {
    ds_df_diss::TaskList_JE  tkl_0;
    cout<<" tkl_0:"<< tkl_0.jsonObj().ToFormattedString()<<endl;


    ds_df_diss::TaskList_JE  tkl_1({ds_df_diss:: Task_JE("server","client","proj",1,2),
                                    ds_df_diss:: Task_JE("server","client","proj",1,3)
                                   });
    cout<<" tkl_1:"<< tkl_1.jsonObj().ToFormattedString()<<endl;

    ds_df_diss::TaskList_JE  tkl_2( tkl_1.jsonObj());
    cout<<" tkl_2:"<< tkl_2.jsonObj().ToFormattedString()<<endl;
  }


  ///====================================================
  {
    ds_df_diss::String_JE  tkl_0;
    cout<<" tkl_0:"<< tkl_0.jsonObj().ToFormattedString()<<endl;


    ds_df_diss::String_JE  tkl_1("string");
    cout<<" tkl_1:"<< tkl_1.jsonObj().ToFormattedString()<<endl;

    ds_df_diss::String_JE  tkl_2( tkl_1.jsonObj());
    cout<<" tkl_2:"<< tkl_2.jsonObj().ToFormattedString()<<endl;
  }


  ///====================================================
  {
    ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE>  msg_0;
    cout<<" msg_0:"<< msg_0.jsonObj().ToFormattedString()<<endl;


    ds_df_diss::TaskList_JE  tkl_1({ds_df_diss:: Task_JE("server","client","proj",1,2),
                                    ds_df_diss:: Task_JE("server","client","proj",1,3)
                                   });
    ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE>  msg_1(1,tkl_1);
    cout<<" msg_1:"<< msg_1.jsonObj().ToFormattedString()<<endl;

    ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE>  msg_2( msg_1.jsonObj());
    cout<<" msg_2:"<< msg_2.jsonObj().ToFormattedString()<<endl;
  }










}
