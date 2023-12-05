#include "concretetaskexecutor.h"
#include "../api_diss/dissapi.h"
#include <fstream>


ConcreteTaskExecutor::ConcreteTaskExecutor()
{

}



/// 根据给定的信息计算出本的文件的路径
/// 然后获取本的文件
///   若获取成功，则向fd传入文件
///   若获取失败，则传入错误码
/// 关闭socket连接
int RscGetTaskExecutor::conclude(){
  /// 源数据
  int ret=0;

  string file=__dir+__rsc_para.__name;

  string content_type;
  if (__rsc_para.__type_resources==0) {
    content_type="xls";
    ret=0;
  }
  else if(__rsc_para.__type_resources==1) {
    content_type="jpeg";
    ret=0;
  }
  else{
    ret=-1;
  }


  if(ret==0){
    ifstream ifs(file);
    ifs.seekg(0,std::ios::end);
    int len = ifs.tellg();
    ifs.seekg(0,std::ios::beg);
    char *buff = new char[len];
    ifs.read(buff , len);
    if(len > 0){
      int ret_send= ns_DissApi::sendHttpResponse(__fd,const_cast<char*>(content_type.c_str()),(unsigned char*)buff,len);
      if(ret_send == 0){
        cout<<"Succeed to Send"<<endl;
      }
      else{
        ret=-1;
      }
    }
    else{
      ret=-1;
    }
  }


  if (ret==-1) {
    ns_DissApi::sendHttpResponseWithReturnCode(__fd,ns_DissApi::ENUM_HTTP_ERRCODE_OPERATEFAILURE);
    cout<<"Fail to Send"<<endl;
  }

  ns_DissApi::closeHttpConnection(__fd);



  return 0;
}


string getBlockDir(const string &v_proj,int v_group,int v_block){
  string ret="bdir";
  ret=ret+"_"+v_proj;
  ret=ret+"_group"+basic::integerToString(v_group);
  ret=ret+"_block"+basic::integerToString(v_block)+"/";
  return ret;
}
string getBlockResultDir(const string &v_proj,int v_group,int v_block){
  string ret="bdir";
  ret=ret+"_"+v_proj;
  ret=ret+"_group"+basic::integerToString(v_group);
  ret=ret+"_block"+basic::integerToString(v_block);
  ret=ret+"/stitchresult/";
  return ret;
}

string getBlockResultDir(const ds_df_diss::Task_JE& v_task){
  return getBlockResultDir(v_task.__id_project,v_task.__group,v_task.__block);;
}
string getBlockDir(const ds_df_diss::Task_JE& v_task){
  return getBlockDir(v_task.__id_project,v_task.__group,v_task.__block);;
}



///   1. 获取请求所有资源，保存在本地
///   2. 根据本地保存的文件进行拼接
///   3. 获得拼接结果，将拼接结果告知给控制程序
int StitchImageInDirTaskExecutor::conclude(){
  int ret=0;
  if (__task.__status == 0) {
    string blockdir_save=getBlockDir(__task);
    string blockstitch_save=getBlockResultDir(__task);
    runwayio::BlockFame blockframe_tmp("");

    runwayio::BlockFame blockframe_save(blockdir_save);
    basic::mkDir(blockdir_save);
    vector<string> l_image_tmp=blockframe_tmp.getImageInBlock();
    vector<string> l_image_save_tmp=blockframe_save.getImageInBlock();

    cout<<"下载图片"<<endl;

    for (uint i=0;i<l_image_tmp.size();i++) {
      ds_df_diss::Resources_JE rsc_tmp(__task,l_image_tmp[i],1,0);
      unsigned char* pdata=nullptr;
      int datasize=0;
      int ret_get=ns_DissApi::ns_client::reqServerResource(__task.__id_server,rsc_tmp,&pdata,datasize);
      cout<<"ret_get："<<ret_get<<" :"<<l_image_tmp[i]<<endl;

      cout<<"__task:"<<__task.jsonObj().ToFormattedString()<<endl;
      if (ret_get == 0&&datasize>0) {
        FILE *fp=fopen(l_image_save_tmp[i].c_str(),"w");
        if (fp!=nullptr) {
          fwrite((char*)pdata,datasize,1,fp);
          fclose(fp);
        }
        free(pdata);
        pdata=nullptr;
      }
      else{
        ret=-1;
        break;
      }
    }


    cout<<"下载并保存信息文件"<<endl;

    /// 下载并保存信息文件
    if (ret == 0) {
      string into_tmp=blockframe_tmp.getBlockInfo();
      string into_save_tmp=blockframe_save.getBlockInfo();
      ds_df_diss::Resources_JE rsc_tmp(__task,into_tmp,0,0);
      unsigned char* pdata=nullptr;
      int datasize=0;
      int ret_get=ns_DissApi::ns_client::reqServerResource(__task.__id_client,rsc_tmp,&pdata,datasize);
      if (ret_get == 0&&datasize>0) {
        FILE *fp=fopen(into_save_tmp.c_str(),"w");
        if (fp!=nullptr) {
          fwrite((char*)pdata,datasize,1,fp);
          fclose(fp);
        }
        free(pdata);
        pdata=nullptr;
      }
      else{
        ret=-1;
      }

    }

    cout<<"开始拼接过程"<<endl;

    /// 开始拼接过程
    if (ret == 0) {
      Mat mat_target;
      runwayio::SplitLinePos pos_splitline;
      runwayio::BlockData  v_blokdata;
      //        string path_debug="";
      //        process_runway::CorrectType v_correcttype=process_runway::CorrectType_right_and_left;

      int flag_stitch =process_runway::stitchImageInDir(blockdir_save,mat_target,pos_splitline,v_blokdata);
      if (flag_stitch == 0) {
        string pos_record=pos_splitline.getString(pos_splitline);
        basic::saveFile(const_cast<char*>(pos_record.c_str()),blockstitch_save+"/info.json");

        Mat mat_rmoffset=mat_target(Rect(pos_splitline.pos_x_left,
                                         pos_splitline.pos_y_up,
                                         pos_splitline.pos_x_right-pos_splitline.pos_x_left,
                                         pos_splitline.pos_y_down-pos_splitline.pos_y_up
                                         ));
        imwrite(blockstitch_save+"/block_rm.jpg",mat_rmoffset);
        imwrite(blockstitch_save+"/block.jpg",mat_target);
      }
      else{
        ret=-1;
      }
    }

  }


  if (ret == 0) {
    __task.__status=1;
  }
  else{
    __task.__status=-1;
  }

  if (__noticer !=nullptr) {
    __noticer->sendNotice(__task);
  }

  return ret;

}
