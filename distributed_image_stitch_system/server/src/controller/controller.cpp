#include "controller.h"
#include <QThread>
#include <QMetaType>
#include <cstdlib>
#include <cstdio>
#include "../../../common_lib/sskj_lib/api_diss/dissapi.h"

Controller::Controller(MainWindow* v_mw, ThreadPool *v_tp, QObject *parent) :
  QThread(parent),__mainwindow(v_mw),__time_interval(10),
  __threadpool_req(v_tp),__status_run(true),__timer_pushworkloop(new QTimer),
  __tasklist_wait(new TaskManage_JE()),__tasklist_op(new TaskManage_JE())

{
  /// 对于一些错误提醒的统一声明
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_DIRNOTSET]=
      "PATH OF INPUT OR OUTPUT DIR NOT SET！";
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_INPUT_NOT_COMPLETE]=
      "INPUT DATA IS NOT COMPLETE！";
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_IP_WRONG]=
      "IP is WRONG！";
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_SETHTTPFAIL]=
      "FAIL TO SET IP！";
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_STOPLISTTENFAIL]=
      "FAIL TO LISTEN HTTP！";
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_PREVIOUS_NOT_READY]=
      "PREVIOUS STATUS IS NOT READY！";
  __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]=
      "THREAD IS RUNNING! OPERATION IS REFUSED！";

  for (uint i= ds_df_diss::ENUM_CTR_STATUS_SETIP;i<ds_df_diss::ENUM_CTR_STATUS_COMPLETE;i++) {
    __l_status[static_cast<ds_df_diss::ENUM_CTR_STATUS>(i)]=false;

  }

  ns_DissApi::initHTTPEnv();

  qRegisterMetaType<ds_df_diss::ENUM_CTR_STATUS>("ds_df_diss::ENUM_CTR_STATUS");
  qRegisterMetaType<map<ds_df_diss::ENUM_CTR_STATUS,bool>>("map<ds_df_diss::ENUM_CTR_STATUS,bool>");


  connect(__mainwindow,SIGNAL(sgl_startWork()),
          this,SLOT(slt_on_startWork()));
  connect(__mainwindow,SIGNAL(sgl_stopWork()),
          this,SLOT(slt_on_stopWork()));

  connect(__mainwindow,SIGNAL(sgl_startListen()),
          this,SLOT(slt_on_startListen()));
  connect(__mainwindow,SIGNAL(sgl_stopListen()),
          this,SLOT(slt_on_stopListen()));
  connect(__mainwindow,SIGNAL(sgl_selectInputDir(QString)),
          this,SLOT(slt_on_changeInputDir(QString)));
  connect(__mainwindow,SIGNAL(sgl_selectOutputDir(QString)),
          this,SLOT(slt_on_changeOutputDir(QString)));
  connect(__mainwindow,SIGNAL(sgl_setIP(QString)),
          this,SLOT(slt_on_changeIP(QString)));
  connect(__mainwindow,SIGNAL(sgl_init()),
          this,SLOT(slt_on_init()));
  connect( this,SIGNAL(sgl_on_status_change(map<ds_df_diss::ENUM_CTR_STATUS,bool>)),
           __mainwindow,SLOT(slt_on_ctr_statuschange(map<ds_df_diss::ENUM_CTR_STATUS,bool>)));
  connect(this,SIGNAL(sgl_on_warning(QString)),
          __mainwindow,SLOT(slt_showWarning(QString)));
  connect(__mainwindow,SIGNAL(sgl_addClientIP(QString)),
          this,SLOT(slt_on_addclient(QString)));


  connect(__timer_pushworkloop,SIGNAL(timeout()),
          this,SLOT(slt_on_timer_pushworkloop_timeout()));

  __timer_pushworkloop->start(__time_interval*1000);
}


void Controller::resetInputFrame(string v_str){
  if (__frame_input.dir_root != v_str) {
    __frame_input.dir_root=v_str;
    __info_input=runwayio::InputData::loadFromFile(__frame_input.getTotalInfoFile());
    updateDirFrame();
    if (__frame_input.dir_root==""||__frame_output.dir_root==""){
      __l_status[ds_df_diss::ENUM_CTR_STATUS_SETDIR]=false;

    }
    else{
      __l_status[ds_df_diss::ENUM_CTR_STATUS_SETDIR]=true;

    }
    emit sgl_on_status_change(__l_status);
  }
}


void Controller::slt_on_timer_pushworkloop_timeout(){
  LIFETIMETRACE(__FUNCTION__);

  QMutexLocker tmplocker(&__mutex_data);
  if (      __l_status[ds_df_diss::ENUM_CTR_STATUS_WORK]==true) {
    __l_func.push_back(ENUM_CTR_FUNC_WORKLOOP);
  }
}


void Controller::slt_on_addclient(QString v_ip){
  QMutexLocker tmplocker(&__mutex_data);
  __l_client_status.push_back(ds_df_diss::ClientStatus_JE(v_ip.toStdString(),0,0,0));
  __l_func.push_back(ENUM_CTR_FUNC_WORKLOOP);

}


void Controller::resetOutputFrame(string v_str){

  if (__frame_output.dir_root != v_str) {
    __frame_output.dir_root=v_str;
    __info_output=runwayio::OutputData::loadFromFile(__frame_output.getTotalInfoFile());
    updateDirFrame();
    if (__frame_input.dir_root==""||__frame_output.dir_root==""){
      __l_status[ds_df_diss::ENUM_CTR_STATUS_SETDIR]=false;
    }
    else{
      __l_status[ds_df_diss::ENUM_CTR_STATUS_SETDIR]=true;

    }
    emit sgl_on_status_change(__l_status);

  }
}


void Controller::slt_on_changeInputDir(QString v_str){
  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK] == true) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]));
    return;
  }

  resetInputFrame(v_str.toStdString());

}


void Controller::slt_on_changeOutputDir(QString v_str){
  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK] == true) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]));
    return;
  }

  resetOutputFrame(v_str.toStdString());
}


/// <>可更换成由用户决定
void Controller::updateDirFrame(){
  if (!basic::isFileExist(__frame_input.dir_root)) {
    __frame_input.createDir(__info_input);
  }

  if (!basic::isFileExist(__frame_output.dir_root)) {
    __frame_output.createDir(__info_output);
  }

}


Controller::~Controller(){
  ns_DissApi::uinitHTTPEnv();
  __status_run=false;
}




void Controller::slt_on_init(){
  LIFETIMETRACE(__FUNCTION__);

  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK] == true) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]));
    return;
  }

  __l_func.push_back(ENUM_CTR_FUNC_INIT);

  //  initConfig();

}


/// 1. 检查文件夹路径是否设置
/// 2. 检查输入文件夹是否完整，否则数处错误
/// 3. 检查输出文件夹是否正常构建，否则重新构建
/// 4. 载入输出结果中未完成的任务项，导入到待分配任务列表
void Controller::initConfig(){
  LIFETIMETRACE(__FUNCTION__);

  if (__frame_input.dir_root==""||__frame_output.dir_root=="")
  {
    emit sgl_on_warning(QString::fromStdString(__l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_DIRNOTSET]));
  }
  else
  {
    cout<<"__frame_input.dir_root:"<<__frame_input.dir_root<<endl;

    if (runwayio::isInputDataComplete_loose(__frame_input.dir_root)) {
      cout<<"ENUM_DISS_CTR_NOTI_INPUT_IS_COMPLETE"<<endl;

      //<>此处可以优化
      if (!basic::isFileExist(__frame_output.dir_root)) {
        __frame_output.createDir(__info_output);
      }

      for(uint i=0;i<__info_input.list_group.size();i++){
        //逐块判断
        for(int seq=__info_input.list_group[i].seq_start;
            seq<=__info_input.list_group[i].seq_end;seq++){
          cout<<"push"<<endl;
          __tasklist_wait->pushback(ds_df_diss::Task_JE(__ip_server,"","",
                                                        __info_input.list_group[i].num_group,
                                                        seq
                                                        ));
        }
      }
      __l_status[ds_df_diss::ENUM_CTR_STATUS_INIT]=true;
      emit sgl_on_status_change(__l_status);

      /// 获取任务列表

    }
    else{
      cout<<"ENUM_DISS_CTR_NOTI_INPUT_NOT_COMPLETE"<<endl;
      emit sgl_on_warning(QString::fromStdString(
                            __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_INPUT_NOT_COMPLETE]));

    }
    cout.flush();
  }

}


void Controller::slt_on_changeIP(QString v_ip){

  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK] == true) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]));
    return;
  }

  __ip_server=v_ip.toStdString();
  __l_status[ds_df_diss::ENUM_CTR_STATUS_SETIP]=true;
  emit sgl_on_status_change(__l_status);

}


void Controller::slt_on_startListen(){

  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK] == true) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]));
    return;
  }

  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_SETIP]==true) {
    if (__l_status[ds_df_diss::ENUM_CTR_STATUS_LISTEN]==false) {
      cout<<"ns_DissApi::setupListenHttp"<<endl;
      int ret =ns_DissApi::setupListenHttp(Controller::cb_CallbackOnNewHttpRequest,
                                           Controller::cb_CallbackOnHttpError,this,
                                           ds_df_diss::PortConfig::__port_server
                                           );
      if (ret== 0) {
        __l_status[ds_df_diss::ENUM_CTR_STATUS_LISTEN]=true;
        emit sgl_on_status_change(__l_status);
      }
      else{
        emit sgl_on_warning(QString::fromStdString(
                              __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_SETHTTPFAIL]));
      }
    }
  }
  else{
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_IP_WRONG]));

  }

}


void Controller::slt_on_stopListen(){

  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK] == true) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_REFUSE_FOR_WORK]));
    return;
  }

  if (__l_status[ds_df_diss::ENUM_CTR_STATUS_LISTEN]==true) {
    int ret=ns_DissApi::stopListenHttp();
    if (ret==0) {
      __l_status[ds_df_diss::ENUM_CTR_STATUS_LISTEN]=false;
    }
    else{
      emit sgl_on_warning(QString::fromStdString(
                            __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_STOPLISTTENFAIL]));

    }

  }
}


/// 1. 验证所有工作已就绪
/// 2. 设置进程的run为启动状态为true
/// 3. 将标示设置为已启动
void Controller::slt_on_startWork(){
  bool allready=true;
  for (int i=ds_df_diss::ENUM_CTR_STATUS_SETIP;
       i<ds_df_diss::ENUM_CTR_STATUS_INIT;i++) {
    if (__l_status[static_cast<ds_df_diss::ENUM_CTR_STATUS>(i)] == false) {
      allready=false;
      break;
    }
  }
  if (allready == false) {
    emit sgl_on_warning(QString::fromStdString(
                          __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_PREVIOUS_NOT_READY]));
  }
  else{
    __l_status[ds_df_diss::ENUM_CTR_STATUS_WORK]=true;
    emit sgl_on_status_change(__l_status);

  }

}

void Controller::slt_on_stopWork(){
  __l_status[ds_df_diss::ENUM_CTR_STATUS_WORK]=false;
  emit sgl_on_status_change(__l_status);

}


int Controller::cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                            void *ptr){
  LIFETIMETRACE(__FUNCTION__);

  if (ptr!=nullptr) {
    Controller* ctr=static_cast<Controller*>(ptr);
    neb::CJsonObject v_root(req_data);
    ds_df_diss::MsgWithType_JE<ds_df_diss::String_JE> msg(v_root);

    if (msg.__type==static_cast<int>(ns_DissApi::ENUM_REQ_C_reqServerResource)) {
      ds_df_diss::MsgWithType_JE<ds_df_diss::Resources_JE>  msg_rsc(v_root);
      //      ns_DissApi::sendHttpResponseWithReturnCode(fd,ns_DissApi::ENUM_HTTP_ERRCODE_SUCCEED);
      //      ns_DissApi::closeHttpConnection(fd);
      //      ctr->__ip_server="zzzzzzzzzzzzzzzz";
      cout<<"getpid: cb "<<std::this_thread::get_id()<<endl;

      ctr->__threadpool_req->add_task(
            new RscGetTaskExecutor(
              msg_rsc.__data,
              ctr->__frame_input.getBlockDir(msg_rsc.__data.__task.__group,
                                             msg_rsc.__data.__task.__block),
              fd));

    }
    else if(msg.__type==static_cast<int>(ns_DissApi::ENUM_REQ_C_reqTaskComplete)){
      ds_df_diss::MsgWithType_JE<ds_df_diss::Task_JE>  msg_task(v_root);
      QMutexLocker tmplocker(&ctr->__mutex_data);
      ctr->__l_task_completee.push_back(msg_task.__data);
      ctr->__l_func.push_back(ENUM_CTR_FUNC_REQ_PROCESS);
      ns_DissApi::sendHttpResponseWithReturnCode(fd,ns_DissApi::ENUM_HTTP_ERRCODE_SUCCEED);
      ns_DissApi::closeHttpConnection(fd);
    }

  }

  fflush(stdout);

  return 0;
}

int Controller::cb_CallbackOnHttpError(int error_code, void*ptr){
  cout<<__func__<<endl;
  return 0;
}

/// 图片拼接的控值过程
/// 对于一些耗时的任务也可以穿插在该进程中执行，具体实现可以维护一张ctr_任务表，每次取顶端的任务进行执行
/// 任务类型：client状态获取与任务分配； initconfig ；下载任务，与拼接进度更新
/// 1. http获取client的状态
///   若client有空闲进程，则分配一定任务
///   若client忙，则不考虑
///   若client断开连接，则将该client移除，将已分配给该client列表中的任务转移到未分配任务，在列表前侧

void Controller::run(){
  cout<<"getpid: run "<<std::this_thread::get_id()<<endl;

  while (__status_run) {

    if (__l_status[ds_df_diss::ENUM_CTR_STATUS_WORK]==false) {
      continue;
    }

    ENUM_CTR_FUNC func_cur_tmp;
    if (!__l_func.empty()) {
      func_cur_tmp =__l_func[0];
      __l_func.erase(__l_func.begin());
    }
    else{
      //      cout<<__func__<<"no work"<<endl;
      continue;
    }
    QMutexLocker tmplocker(&__mutex_data);
    executeFunc(func_cur_tmp);
  }
}

/// 返回的任务执行完成请求，若没有找到对应的任务，则不会去处理
/// 查询待分配任务列表中的数据与已分配任务列表中的任务，
///   若有任务剩余，则查询客户端的状态
///     先确保已分配的任务被正确分配到各个客户端，若已分配任务列表中的任务没有分配给client，则转移至未分配列表
///     若有线程空闲则，则从待分配列表中发送新的给client，同时任务转移到已分配列表
///   没有任务剩余，则将已完成置为complete为true
void Controller::workLoop(){
  LIFETIMETRACE(__FUNCTION__);

  cout<<__func__<<endl;
  cout<<"size__tasklist_wait:"<<__tasklist_wait->size()<<endl;
  cout<<"size__tasklist_op:"<<__tasklist_op->size()<<endl;
  cout<<"size__l_client_status:"<<__l_client_status.size()<<endl;


  /// 检查client的状态
  vector<ds_df_diss::ClientStatus_JE> l_clientstatus_tmp;
  for (auto it =__l_client_status.begin();it!=__l_client_status.end();it++) {
    cout<<"it:"<<it->jsonObj().ToFormattedString()<<endl;
    ds_df_diss::ClientStatus_JE tmpstatus;
    int ret=ns_DissApi::ns_server::reqClientStatus(it->__id_client,tmpstatus);
    cout<<"tmpstatus:"<<tmpstatus.jsonObj().ToFormattedString()<<endl;

    if (ret==0) {
      l_clientstatus_tmp.push_back( tmpstatus);
    }
    else{
      emit sgl_on_warning(QString::fromStdString(
                            __l_noti[ds_df_diss::ENUM_DISS_CTR_NOTI_CLIENT_NOT_CONNECT])+":"
          +QString::fromStdString(it->__id_client));
      cout<<"rm client:"<<it->__id_client<<endl;
    }
  }
  __l_client_status=l_clientstatus_tmp;

  cout<<"after 检查client的状态"<<endl;


  /// 分配任务
  if (__tasklist_wait->size()+__tasklist_op->size() > 0) {
    /// 临时任务列表，在client中找到匹配就删除对应的，余下的就是没有被分配的
    auto l_tashk_tmp=__tasklist_op->getTaskList();
    for (uint i=0;i<__l_client_status.size();i++) {
      ds_df_diss::TaskList_JE tmptasklist;
      ns_DissApi::ns_server::reqClientTaskList(__l_client_status[i].__id_client,
                                               __ip_server,tmptasklist);
      cout<<"tmptasklist: "<<tmptasklist.jsonObj().ToFormattedString()<<endl;

      for (auto it =tmptasklist.__l_task.begin();it!=tmptasklist.__l_task.end();it++) {
        auto pos_find=find_if(l_tashk_tmp.begin(),l_tashk_tmp.end(),
                              [=](ds_df_diss::Task_JE tmptask){
          return tmptask==*it;
        });
        if (pos_find != l_tashk_tmp.end()) {
          l_tashk_tmp.erase(pos_find);
        }
      }
    }

    cout<<"after 检查任务分配"<<endl;

    /// 转移没有分配的任务
    __tasklist_op->erase(l_tashk_tmp);
    __tasklist_wait->pushfront(l_tashk_tmp);



    cout<<"after 转移没有分配的任务"<<endl;
    cout<<"__tasklist_op:"<<__tasklist_op->jsonObj().ToFormattedString()<<endl;
    //    cout<<"__tasklist_wait:"<<__tasklist_wait->jsonObj().ToFormattedString()<<endl;


    ///检查客户端的线程空闲情况，然后将待分配列表中的任务分配给空闲的client
    /// 发送成功后，将任务从待分配转移至已分配
    for (auto it_c=__l_client_status.begin();it_c!=__l_client_status.end();it_c++) {
      int count_free=it_c->__count_total_thread*2-it_c->__count_total_task;
      if (count_free>0) {
        for (int i=0;i<count_free;i++) {
          ds_df_diss::Task_JE task_tmp;
          int ret_get=__tasklist_wait->popFront(task_tmp);
          cout<<"task_tmp:"<<task_tmp.jsonObj().ToFormattedString()<<endl;
          if (ret_get == 0) {
            int ret_push=ns_DissApi::ns_server::reqTaskPush(it_c->__id_client,task_tmp);
            cout<<"ret_push:"<<ret_push<<endl;
            /// 若成功则转移
            if (ret_push==0) {
              __tasklist_op->pushback(task_tmp);
              __tasklist_wait->erase(task_tmp);
            }
          }
          else{
            break;
          }
        }
      }
    }

  }
  else{
    __l_status[ds_df_diss::ENUM_CTR_STATUS_COMPLETE] =true;
    emit sgl_on_status_change(__l_status);
  }
  cout<<"after 分配任务";

}


void Controller::executeFunc(ENUM_CTR_FUNC v_func){
  LIFETIMETRACE(__FUNCTION__);

  switch (v_func) {
    case ENUM_CTR_FUNC_INIT:
      initConfig();
      break;
    case ENUM_CTR_FUNC_WORKLOOP:
      workLoop();
      break;
    case ENUM_CTR_FUNC_REQ_PROCESS:
      processRequest();
      break;
  }
}


/// 处理所有任务已完成的条目
/// 对于每一项数据
///
///     1. 若执行成功，根据定义的资源列表，逐项获取资源；若不成功则跳过
///     2. 将任务从已分配列表中删除
///     3. 将执行状态保存到本的文件中
void Controller::processRequest(){
  vector<ds_df_diss::Task_JE> l_completetask_tmp=__l_task_completee;
  __l_task_completee.clear();

  runwayio::OutputData onputdata=runwayio::OutputData::loadFromFile(__frame_output.getTotalInfoFile());

  for (auto it=l_completetask_tmp.begin();it!=l_completetask_tmp.end();it++) {
    if (it->isSucceed()) {

      /// 下载信息文件并保存
      ds_df_diss::Resources_JE  rsc_info(*it,"info.json",0,1);
      unsigned char* data_infofile=nullptr;
      int size_infofile;
      int ret_getinfo=ns_DissApi::ns_server::reqClientResource(it->__id_client,
                                                               rsc_info,
                                                               &data_infofile,
                                                               size_infofile
                                                               );
      if (ret_getinfo == 0) {
        string path_info_file_save=__frame_output.getBlockInfo(it->__group,it->__block);
        FILE* fp=fopen(path_info_file_save.c_str(),"w");
        if (fp !=nullptr) {
          fwrite(data_infofile,size_infofile,1,fp);
          fclose(fp);
        }
      }

      /// 下载图片并保存
      ds_df_diss::Resources_JE  rsc_image(*it,"block.jpg",0,1);
      unsigned char* data_image=nullptr;
      int size_image;
      int ret_getimage=ns_DissApi::ns_server::reqClientResource(it->__id_client,
                                                                rsc_image,
                                                                &data_image,
                                                                size_image
                                                                );
      if (ret_getimage == 0) {
        string path_info_file_save=__frame_output.getBlockImage(it->__group,it->__block);
        FILE* fp=fopen(path_info_file_save.c_str(),"w");
        if (fp !=nullptr) {
          fwrite(data_image,size_image,1,fp);
          fclose(fp);
        }
      }



      /// 下载图片2并保存
      ///
      /// 下载图片并保存
      ds_df_diss::Resources_JE  rsc_image_rm(*it,"block.jpg",0,1);
      unsigned char* data_image_rm=nullptr;
      int size_image_rm;
      int ret_getimage_rm=ns_DissApi::ns_server::reqClientResource(it->__id_client,
                                                                   rsc_image_rm,
                                                                   &data_image_rm,
                                                                   size_image_rm
                                                                   );
      if (ret_getimage_rm == 0) {
        string path_info_file_save=__frame_output.getBlockImage_rm(it->__group,it->__block);
        FILE* fp=fopen(path_info_file_save.c_str(),"w");
        if (fp !=nullptr) {
          fwrite(data_image_rm,size_image_rm,1,fp);
          fclose(fp);
        }
      }


      if(onputdata.isNull()){
        onputdata=runwayio::OutputData(__info_input);
      }
      if (ret_getinfo == 0
          &&ret_getimage == 0
          &&ret_getimage_rm == 0 ) {
        onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
            seq_start)].status=runwayio::ProcessStatus_succeed;
        onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
            seq_start)].status_correct=runwayio::ProcessStatus_succeed;
        onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
            seq_start)].status_stitch=runwayio::ProcessStatus_succeed;
      }
      else{
        onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
            seq_start)].status=runwayio::ProcessStatus_error;
        onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
            seq_start)].status_correct=runwayio::ProcessStatus_error;
        onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
            seq_start)].status_stitch=runwayio::ProcessStatus_error;
      }

    }
    else{
      onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
          seq_start)].status=runwayio::ProcessStatus_error;
      onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
          seq_start)].status_correct=runwayio::ProcessStatus_error;
      onputdata.list_group[it->__group].list_block[static_cast<uint>(it->__block-__info_input.list_group[it->__group].
          seq_start)].status_stitch=runwayio::ProcessStatus_error;
    }
    runwayio::OutputData::saveJsonString(__frame_output.getTotalInfoFile(),onputdata);

    __tasklist_op->erase(*it);
  }


}

//vector<ds_df_diss::Resources_JE> Controller::getBlockRscList(const ds_df_diss::Task_JE&v_task){

//}




