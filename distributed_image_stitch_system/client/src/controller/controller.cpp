#include "controller.h"

Controller::Controller(InitView *v_init,MainWindow *v_mw,QObject *parent) :
  QObject(parent),__threadpool(nullptr),__threadpool_smalltask(new ThreadPool(4)),__initview(v_init),__mainwindow(v_mw)
{
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_SETHTTPFAIL]="FAIL TO SET HTTPLISTEN";

  ns_DissApi::initHTTPEnv();
  connect(__initview,SIGNAL(sgl_on_initConfig(QString ,int)),
          this,SLOT(slt_on_initConfig(QString ,int )));
  connect(this,SIGNAL(sgl_on_warning(QString)),
          __mainwindow,SLOT(slt_showWarning(QString)));

  int ret =ns_DissApi::setupListenHttp(Controller::cb_CallbackOnNewHttpRequest,Controller::cb_CallbackOnHttpError,this
                                       ,ds_df_diss::PortConfig::__port_client
                                       );
  if (ret== 0) {
    cout<<"succeed to setup http listen"<<endl;
  }
  else{
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_SETHTTPFAIL]));
  }
}



Controller::~Controller(){
  cout<<__func__<<endl;

  if (__threadpool!= nullptr) {
    delete  __threadpool;
  }
  ns_DissApi::stopListenHttp();
  ns_DissApi::uinitHTTPEnv();
}



void Controller::slt_on_initConfig(QString v_ip,int v_count){
  __clientip=v_ip.toStdString();
  __count_thread=v_count;
  __initview->hide();
  __mainwindow->show();
  if (__threadpool== nullptr) {
    __threadpool=new ThreadPool(__count_thread);
  }
}




int Controller::cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                            void *ptr){
  cout<<__func__<<" fd:"<<fd<<endl;
  printf("req_data:%s\n",req_data);
  if (ptr!=nullptr) {
    Controller* ctr=static_cast<Controller*>(ptr);
    neb::CJsonObject v_root(req_data);
    ds_df_diss::MsgWithType_JE<ds_df_diss::String_JE> msg(v_root);

    if (msg.__type==static_cast<int>(ns_DissApi::ENUM_REQ_S_reqTaskReady)) {
      ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE>  msg_rsc(v_root);
      ctr->__l_task.push_back(msg_rsc.__data);
      ctr->__threadpool->add_task(new StitchImageInDirTaskExecutor(msg_rsc.__data,"",fd,nullptr));
      ns_DissApi::sendHttpResponseWithReturnCode(fd,ns_DissApi::ENUM_HTTP_ERRCODE_SUCCEED);
      ns_DissApi::closeHttpConnection(fd);

    }
    else if(msg.__type==static_cast<int>(ns_DissApi::ENUM_REQ_S_reqClientResource)){
      ds_df_diss::MsgWithType_JE<ds_df_diss::Resources_JE>  msg_rsc(v_root);
      string dir_res=getBlockResultDir(msg_rsc.__data.__task);
      ctr->__threadpool_smalltask->add_task(new RscGetTaskExecutor(msg_rsc.__data,
                                                                   dir_res,fd));
    }

    else if(msg.__type==static_cast<int>(ns_DissApi::ENUM_REQ_S_reqClientStatus)){
      QMutexLocker locker_tmp(&ctr->__mutex_data);

      ds_df_diss::ClientStatus_JE status(ctr->__clientip,ctr->__count_thread,0,ctr->__l_task.size());
      ds_df_diss::MsgWithType_JE<ds_df_diss::ClientStatus_JE> data_return(ns_DissApi::ENUM_HTTP_ERRCODE_SUCCEED,status);
      string response=data_return.jsonObj().ToString();
      ns_DissApi::sendHttpResponse(fd,"json",(unsigned char*)(response.c_str()),response.size());
      ns_DissApi::closeHttpConnection(fd);

    }
    else if(msg.__type==static_cast<int>(ns_DissApi::ENUM_REQ_S_reqClientTaskList)){
      QMutexLocker locker_tmp(&ctr->__mutex_data);
      ds_df_diss::TaskList_JE tasklist(ctr->__l_task);
      ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE> data_return(ns_DissApi::ENUM_HTTP_ERRCODE_SUCCEED,tasklist);
      string response=data_return.jsonObj().ToString();
      ns_DissApi::sendHttpResponse(fd,"json",(unsigned char*)(response.c_str()),response.size());
      ns_DissApi::closeHttpConnection(fd);
    }
    else{
      ds_df_diss::MsgWithType_JE<ds_df_diss::TaskList_JE> data_return(ns_DissApi::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
      string response=data_return.jsonObj().ToString();
      ns_DissApi::sendHttpResponse(fd,"json",(unsigned char*)(response.c_str()),response.size());
      ns_DissApi::closeHttpConnection(fd);
    }

  }

  return 0;
}


int Controller::cb_CallbackOnHttpError(int error_code, void*ptr){
  cout<<__func__<<endl;
  return 0;
}
