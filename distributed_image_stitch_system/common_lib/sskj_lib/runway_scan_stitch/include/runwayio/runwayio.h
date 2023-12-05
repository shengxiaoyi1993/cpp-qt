#ifndef RUNWAYIO_H
#define RUNWAYIO_H

#include <iostream>
#include <vector>
#ifdef __linux__
#include <unistd.h>
#include <dirent.h>
#endif
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#endif
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "../basic/basic.h"
#include "../config_module/commonpara.h"
#include "../basic/cJSON.h"
#include "../data_visualization/DataVisualization.h"



using namespace std;
using namespace cv;


namespace runwayio {

/**
 * @function: 获取cate_dir目录下的所有文件名
 * @param: cate_dir - string类型
 * @result：vector<string>类型
*/
vector<string> getFiles(string cate_dir);

typedef struct SplitLinePos{
    int pos_y_up;
    int pos_y_down;
    int pos_x_left;
    int pos_x_right;
    SplitLinePos(){
        pos_y_up=0;
        pos_y_down=0;
        pos_x_left=0;
        pos_x_right=0;
    }

    SplitLinePos(int v_y_up,int v_y_down,int v_x_left,int v_x_right){
        pos_y_up=v_y_up;
        pos_y_down=v_y_down;
        pos_x_left=v_x_left;
        pos_x_right=v_x_right;
    }

    string getString(const SplitLinePos& v_pos){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "pos_y_up", v_pos.pos_y_up);
        cJSON_AddNumberToObject(root, "pos_y_down", v_pos.pos_y_down);
        cJSON_AddNumberToObject(root, "pos_x_left", v_pos.pos_x_left);
        cJSON_AddNumberToObject(root, "pos_x_right", v_pos.pos_x_right);
        char* str_res=cJSON_Print(root);
        string string_res;
        string_res=str_res;
        return string_res;
    }
    void print(const SplitLinePos& v_pos){
        cout<<"<SplitLinePos>"<<endl;
        cout<<"pos_y_up : "<<v_pos.pos_y_up<<endl;
        cout<<"pos_y_down: "<<v_pos.pos_y_down<<endl;
        cout<<"pos_x_left: "<<v_pos.pos_x_left<<endl;
        cout<<"pos_x_right: "<<v_pos.pos_x_right<<endl;
    }

   static int loadFile(string path,SplitLinePos &pos_result ){
        char* buffer;
        basic::getStringFromFile(path,&buffer);

        cJSON* root=cJSON_Parse(buffer);
        char* str=cJSON_PrintUnformatted(root);

        if(str==NULL||root==NULL){
            if(root!=NULL){
                cJSON_Delete(root);
            }
            return -1;
        }

        if(root!=NULL){
            cJSON* item=cJSON_GetObjectItem(root,"pos_y_up");
            pos_result.pos_y_up=item->valueint;
            item=cJSON_GetObjectItem(root,"pos_y_down");
            pos_result.pos_y_down=item->valueint;
            item=cJSON_GetObjectItem(root,"pos_x_left");
            pos_result.pos_x_left=item->valueint;
            item=cJSON_GetObjectItem(root,"pos_x_right");
            pos_result.pos_x_right=item->valueint;
        }

        if(str!=NULL){
            cJSON_free(str);
        }

        if(root!=NULL){
            cJSON_Delete(root);
        }

        return 0;
    }


}SplitLinePos;

enum ProcessStatus{
    ProcessStatus_error=0,
    ProcessStatus_succeed,
    ProcessStatus_wait
};

//用于记录分组结果
typedef struct Partition{
    int flag_size;
    int count_block;

    Partition(){
        flag_size=0;
        count_block=0;
    }

    Partition(int v_flag_size,int v_count_block){
        flag_size=v_flag_size;
        count_block=v_count_block;
    }

}Partition;



//!输入数据结构类型
typedef struct InputGroup{
    int num_group;
    int seq_start;
    int seq_end;
    InputGroup(){
        num_group=-1;
        num_group=-1;
        num_group=-1;
    }

    InputGroup(int v_num_group,int v_seq_start,int v_seq_end){
        num_group=v_num_group;
        seq_start=v_seq_start;
        seq_end=v_seq_end;
    }

    static cJSON* createJsonObj(const InputGroup& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "num_group", v_info.num_group);
        cJSON_AddNumberToObject(root, "seq_start", v_info.seq_start);
        cJSON_AddNumberToObject(root, "seq_end", v_info.seq_end);
        return root;
    }

    static InputGroup loadFromJsonObj(cJSON* v_obj){
        InputGroup v_data;
        if(v_obj!= nullptr){
            cJSON* item_child=cJSON_GetObjectItem(v_obj,"num_group");
            v_data.num_group=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"seq_start");
            v_data.seq_start=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"seq_end");
            v_data.seq_end=item_child->valueint;

        }
        return v_data;
    }

    static string jsonString(const InputGroup & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }
    string jsonString(){
        return jsonString(*this);
    }

}InputGroup;



typedef struct InputData{
    int group_start;
    int group_end;
    std::vector<InputGroup> list_group;

    InputData(){
        group_start=-1;
        group_end=-1;
    }

    bool isNull(){
        if((group_start>=0)
                &&(group_end>=group_start)){
            return false;
        }
        else{
            return true;
        }
    }


    static cJSON* createJsonObj(const InputData& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "group_start", v_info.group_start);
        cJSON_AddNumberToObject(root, "group_end", v_info.group_end);

        cJSON * arrayitem=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_group.size();i++){
            cJSON* item_tmp=InputGroup::createJsonObj(v_info.list_group[i]);
            cJSON_AddItemToArray(arrayitem,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_group",arrayitem);
        return root;
    }


    static InputData loadFromJsonObj(cJSON* v_obj){
        InputData v_datainfo;
        if(v_obj!= nullptr){

            cJSON* item_child=cJSON_GetObjectItem(v_obj,"group_start");
            v_datainfo.group_start=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"group_end");
            v_datainfo.group_end=item_child->valueint;

            cJSON *array=cJSON_GetObjectItem(v_obj,"list_group");
            if(cJSON_IsArray(array)){
                int num_array = cJSON_GetArraySize(array);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array,i);
                    v_datainfo.list_group.push_back(InputGroup::loadFromJsonObj(subitem));
                }
            }
        }
        return v_datainfo;
    }



    static string jsonString(const InputData & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }
    string jsonString(){
        return jsonString(*this);
    }

    static int  saveJsonString(std::string path_file,const InputData & v_datainfo){
        string jsonstring=jsonString(v_datainfo);
        char* buffer_json;
        buffer_json=const_cast<char*>(jsonstring.c_str());
        basic::saveFile(buffer_json,path_file);
        return 0;
    }

    static InputData loadFromJsonString(std::string v_jsonstring){
        InputData datainfo_src;
        char* buffer;
        buffer=const_cast<char*>(v_jsonstring.c_str());
        cJSON* root=cJSON_Parse(buffer);

        if(root!=nullptr){
            datainfo_src=loadFromJsonObj(root);
            cJSON_Delete(root);

        }

        return datainfo_src;

    }

    static InputData loadFromFile(std::string v_filename){
        char* buffer;
        basic::getStringFromFile(v_filename,&buffer);
        std::string string_json=buffer;
        InputData data_tmp=loadFromJsonString(string_json);

        return data_tmp;
    }



}InputData;




//!输出数据结构类型
typedef struct BlockData{
    ProcessStatus status;
    bool status_stitch;
    bool status_correct;
    BlockData(){
        status=ProcessStatus_wait;
        status_stitch=false;
        status_correct=false;
    }

    static cJSON* createJsonObj(const BlockData& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "status", v_info.status);
        cJSON_AddBoolToObject(root, "status_stitch", v_info.status_stitch);
        cJSON_AddBoolToObject(root, "status_correct", v_info.status_correct);
        return root;
    }

    static BlockData loadFromJsonObj(cJSON* v_obj){
        BlockData v_data;
        if(v_obj!= nullptr){
            cJSON* item_child=cJSON_GetObjectItem(v_obj,"status");
            v_data.status=static_cast<ProcessStatus>(item_child->valueint);
            item_child=cJSON_GetObjectItem(v_obj,"status_stitch");
            v_data.status_stitch=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"status_correct");
            v_data.status_correct=item_child->valueint;

        }
        return v_data;
    }


    static string jsonString(const BlockData & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }
    string jsonString(){
        return jsonString(*this);
    }


}BlockData;


typedef struct GroupData{
    int num_group;
    int col_start;
    int col_end;
    std::vector<BlockData> list_block;

    static cJSON* createJsonObj(const GroupData& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "num_group", v_info.num_group);
        cJSON_AddNumberToObject(root, "col_start", v_info.col_start);
        cJSON_AddNumberToObject(root, "col_end", v_info.col_end);

        cJSON * arrayitem=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_block.size();i++){
            cJSON* item_tmp=BlockData::createJsonObj(v_info.list_block[i]);
            cJSON_AddItemToArray(arrayitem,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_block",arrayitem);
        return root;
    }



    static GroupData loadFromJsonObj(cJSON* v_obj){
        GroupData v_datainfo;
        if(v_obj!= nullptr){

            cJSON* item_child=cJSON_GetObjectItem(v_obj,"num_group");
            if (item_child !=nullptr) {
              v_datainfo.num_group=item_child->valueint;
            }
            item_child=cJSON_GetObjectItem(v_obj,"col_start");
            if (item_child !=nullptr) {
              v_datainfo.col_start=item_child->valueint;
            }

            item_child=cJSON_GetObjectItem(v_obj,"col_end");
            if (item_child !=nullptr) {
              v_datainfo.col_end=item_child->valueint;
            }

            cJSON *array=cJSON_GetObjectItem(v_obj,"list_block");
            if(item_child !=nullptr&&cJSON_IsArray(array)){
                int num_array = cJSON_GetArraySize(array);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array,i);
                    v_datainfo.list_block.push_back(BlockData::loadFromJsonObj(subitem));
                }
            }
        }
        return v_datainfo;
    }

    static string jsonString(const GroupData & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }
    string jsonString(){
        return jsonString(*this);
    }

}GroupData;


typedef struct  OutputData{
    int group_start;
    int group_end;
    std::vector<GroupData> list_group;

    OutputData(){
        group_start=-1;
        group_end=-1;
    }

    OutputData(const InputData& v_inptudata){
        group_start=v_inptudata.group_start;
        group_end=v_inptudata.group_end;
        assert(group_end>=group_start);
        list_group.resize(static_cast<uint>(group_end-group_start+1));
        for(uint i=0;i<v_inptudata.list_group.size();i++){
            list_group[i].col_start=v_inptudata.list_group[i].seq_start;
            list_group[i].col_end=v_inptudata.list_group[i].seq_end;
            list_group[i].num_group=v_inptudata.list_group[i].num_group;
            list_group[i].list_block.resize(
                        static_cast<uint>(list_group[i].col_end-list_group[i].col_start+1));
        }
    }
    bool isNull(){
        if((group_start>=0)
                &&(group_end>=group_start)){
            return false;
        }
        else{
            return true;
        }
    }

    static cJSON* createJsonObj(const OutputData& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "group_start", v_info.group_start);
        cJSON_AddNumberToObject(root, "group_end", v_info.group_end);

        cJSON * arrayitem=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_group.size();i++){
            cJSON* item_tmp=GroupData::createJsonObj(v_info.list_group[i]);
            cJSON_AddItemToArray(arrayitem,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_group",arrayitem);
        return root;
    }


    static OutputData loadFromJsonObj(cJSON* v_obj){
        OutputData v_datainfo;
        if(v_obj!= nullptr){

            cJSON* item_child=cJSON_GetObjectItem(v_obj,"group_start");
            v_datainfo.group_start=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"group_end");
            v_datainfo.group_end=item_child->valueint;

            cJSON *array=cJSON_GetObjectItem(v_obj,"list_group");
            if(cJSON_IsArray(array)){
                int num_array = cJSON_GetArraySize(array);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array,i);
                    v_datainfo.list_group.push_back(GroupData::loadFromJsonObj(subitem));
                }
            }
        }
        return v_datainfo;
    }



    static string jsonString(const OutputData & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }
    string jsonString(){
        return jsonString(*this);
    }

    static int  saveJsonString(std::string path_file,const OutputData & v_datainfo){
        string jsonstring=jsonString(v_datainfo);
        char* buffer_json;
        buffer_json=const_cast<char*>(jsonstring.c_str());
        basic::saveFile(buffer_json,path_file);
        return 0;
    }

    static OutputData loadFromJsonString(std::string v_jsonstring){
        OutputData datainfo_src;
        if (v_jsonstring == "") {
          return datainfo_src;
        }
        char* buffer;
        buffer=const_cast<char*>(v_jsonstring.c_str());
        cJSON* root=cJSON_Parse(buffer);

        if(root!=nullptr){

            datainfo_src=loadFromJsonObj(root);
            cJSON_Delete(root);

        }
        else{
        }

        return datainfo_src;
    }

    static OutputData loadFromFile(std::string v_filename){
        char* buffer;
        if(v_filename != ""){

            std::string string_json=basic::getStringFromFile(v_filename);
            return loadFromJsonString(string_json);
        }
        else{
            OutputData output;
            return output;
        }

    }

    static Mat getVisuaField(const OutputData& v_data){
        int v_cols=v_data.group_end+1;
        int v_rows=0;

        int max_start=INT_MIN;
        int min_end=INT_MAX;

        for(uint i=0;i<v_data.list_group.size();i++){
            max_start=max_start>v_data.list_group[i].col_start?max_start:v_data.list_group[i].col_start;
            min_end=min_end<v_data.list_group[i].col_end?min_end:v_data.list_group[i].col_end;
        }

        cout<<"max_start:"<<max_start<<endl;
        cout<<"min_end:"<<min_end<<endl;
        v_rows=min_end-max_start+1;

        datavisualization::VisualField field(static_cast<uint>(v_cols),
                                             static_cast<uint>(v_rows));

        int length_block=64;
        for(uint c=0;c<field.list.size();c++){
            for(int r=max_start;r<=min_end;r++){
                if(c<static_cast<uint>(v_data.group_start)){
                    cout<<"改行不存在"<<endl;

                    field.list[c][r-max_start]=datavisualization::VisualBasic(1,1,length_block,length_block);
                    vector<Point2i> list_error;
                    list_error.push_back(Point2i(0,0));
                    field.list[c][r-max_start].setErrorBlock(list_error);
                }
                else{
                    if(r<v_data.list_group[c-v_data.group_start].col_start){
                        cout<<"每行的起始部分缺少"<<endl;
                        field.list[c][r-max_start]=datavisualization::VisualBasic(1,1,length_block,length_block);
                        vector<Point2i> list_error;
                        list_error.push_back(Point2i(0,0));
                        field.list[c][r-max_start].setErrorBlock(list_error);
                    }
                    else{
                        if(v_data.list_group[c-v_data.group_start].list_block[r-v_data.list_group[c-v_data.group_start].col_start].status
                                == ProcessStatus_succeed){
                            cout<<"图片块符合"<<endl;
                            field.list[c][r-max_start]=datavisualization::VisualBasic(1,1,length_block,length_block);
                            //                            vector<Point2i> list_error;
                            //                            list_error.push_back(Point2i(0,0));
                            //                            field.list[c][r].setErrorBlock(list_error);
                        }
                        else{
                            cout<<"图片块不符合"<<endl;

                            field.list[c][r-max_start]=datavisualization::VisualBasic(1,1,length_block,length_block);
                            vector<Point2i> list_error;
                            list_error.push_back(Point2i(0,0));
                            field.list[c][r-max_start].setErrorBlock(list_error);
                        }
                    }
                }
            }
        }//end of while
        Mat mat_res=field.getMatField();
        return mat_res;
    }



}OutputData;


typedef  struct BlockFame{
    string root_block;

    BlockFame(/*string*/ /*v_root*/){
//        root_block=v_root;
    }

    BlockFame(string v_root){
        root_block=v_root;
    }

    void setRoot(const string &v_root){
      root_block=v_root;
    }

    vector<string> getImageInBlock(){
        vector<string> list_image;
        string dirpath_left=root_block+"leftimage.jpg";
        string dirpath_mid=root_block+"midimage.jpg";
        string dirpath_right=root_block+"rightimage.jpg";
        list_image.push_back(dirpath_left);
        list_image.push_back(dirpath_mid);
        list_image.push_back(dirpath_right);
        return list_image;
    }

    static   vector<string> getImageInBlock(string v_root){
        vector<string> list_image;
        string dirpath_left=v_root+"leftimage.jpg";
        string dirpath_mid=v_root+"midimage.jpg";
        string dirpath_right=v_root+"rightimage.jpg";
        list_image.push_back(dirpath_left);
        list_image.push_back(dirpath_mid);
        list_image.push_back(dirpath_right);
        return list_image;
    }
    string getBlockInfo(){
          string dirpath=root_block+"/"+"info.json";
          return  dirpath;
      }

    static  string getBlockInfo(string v_root){
      string dirpath=v_root+"/"+"info.json";
      return  dirpath;
  }

}BlockFame;

typedef struct InputFrame{

    string dir_root;
    InputFrame(){
        dir_root="";
    }

    InputFrame(string v_rootdir){
        dir_root=v_rootdir;
    }

    bool createDir(const InputData &v_inputdata){
        if(dir_root.empty()){
            return false;
        }
        else{
            basic::mkDir(dir_root);
            for(uint n_list=0;n_list<v_inputdata.list_group.size();n_list++){
                for(int n_block=v_inputdata.list_group[n_list].seq_start;
                    n_block<=v_inputdata.list_group[n_list].seq_end;n_block++
                    ){

                    basic::mkDir(getBlockDir(v_inputdata.list_group[n_list].num_group,
                                             n_block ));
                }
            }

            return true;
        }
    }
    string getTotalInfoFile(){
        return dir_root+"resultinfo.json";
    }


    string getGroupDir(int v_group){
        string dirpath=dir_root+"group_"+basic::integerToString(v_group)+"/";
        return dirpath;
    }

    string getBlockDir(int v_group,int v_block){
        string dirpath=dir_root+"group_"+basic::integerToString(v_group)+"/"+"block_"
                +basic::integerToString(v_block)+"/";
        return  dirpath;
    }

    string getBlockInfo(int v_group,int v_block){
        string dirpath=dir_root+"group_"+basic::integerToString(v_group)+"/"+"block_"
                +basic::integerToString(v_block)+"/"+"info.json";
        return  dirpath;
    }

    /**
     * @brief getImageInBlock:输出图片地址列表
     * @param v_group
     * @param v_block
     * @return 0：left 1:mid 2:right
     */
    vector<string> getImageInBlock(int v_group,int v_block){
        string dir_block=dir_root+"group_"+basic::integerToString(v_group)+"/"
                +"block_"+basic::integerToString(v_block)+"/";
        vector<string> list_image=BlockFame::getImageInBlock(dir_block);
        return list_image;
    }

}InputFrame;






typedef struct OutputFrame{

    string dir_root;
    OutputFrame():
      __l_rsc_inblock{"info.json","block.jpg","block_rm.jpg"}
    {
        dir_root="";
    }

    OutputFrame(string v_rootdir):
      __l_rsc_inblock{"info.json","block.jpg","block_rm.jpg"}
    {
        dir_root=v_rootdir;
    }

    string getTotalInfoFile(){
        return dir_root+"resultinfo.json";
    }



    string getGroupDir(int v_group){
        string dirpath=dir_root+"group_"+basic::integerToString(v_group)+"/";
        return dirpath;
    }

    string getBlockDir(int v_group,int v_block){
        string dirpath=dir_root+"group_"+basic::integerToString(v_group)+"/"
                +"block_"+basic::integerToString(v_block)+"/";
        return  dirpath;
    }


    bool createDir(const OutputData &v_outputdata){
        if(dir_root.empty()){
            return false;
        }
        else{
            basic::mkDir(dir_root);
            for(uint n_list=0;n_list<v_outputdata.list_group.size();n_list++){
                for(int n_block=v_outputdata.list_group[n_list].col_start;
                    n_block<=v_outputdata.list_group[n_list].col_end;n_block++
                    ){
                    basic::mkDir(getBlockDir(v_outputdata.list_group[n_list].num_group,
                                             n_block ));
                }
            }

            return true;
        }
    }

    string getBlockInfo(int v_group,int v_block){
        string dirpath=dir_root+"group_"+basic::integerToString(v_group)+"/"
                +"block_"+basic::integerToString(v_block)+"/"+"info.json";
        return  dirpath;
    }

    string getBlockImage(int v_group,int v_block){
        string path_blockimage=dir_root+"group_"+basic::integerToString(v_group)+"/"
                +"block_"+basic::integerToString(v_block)+"/"+"block.jpg";
        return path_blockimage;
    }

    string getBlockImage_rm(int v_group,int v_block){
        string path_blockimage=dir_root+"group_"+basic::integerToString(v_group)+"/"
                +"block_"+basic::integerToString(v_block)+"/"+"block_rm.jpg";
        return path_blockimage;
    }

    vector<string> getRscListInBlock(){return __l_rsc_inblock;}

    vector<string> __l_rsc_inblock;




}OutputFrame;


inline cJSON* createPointObj(const Point& v_info){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "x", v_info.x);
    cJSON_AddNumberToObject(root, "y", v_info.y);
    return root;

}
inline Point loadFromPointObj(cJSON* v_obj){
    Point point_tmp;
    cJSON* item=cJSON_GetObjectItem(v_obj,"x");
    point_tmp.x=item->valueint;
    item=cJSON_GetObjectItem(v_obj,"y");
    point_tmp.y=item->valueint;
    return point_tmp;
}




typedef struct InputBlockInfo{

    int group;
    int seq;
    int flag_size;
    int margin_up;
    int margin_down;
    int flag_pos;
    vector<Point2i> list_point_up_left;
    vector<Point2i> list_point_down_left;
    vector<Point2i> list_point_up_mid;
    vector<Point2i> list_point_down_mid;
    vector<Point2i> list_point_up_right;
    vector<Point2i> list_point_down_right;
    bool flag_trust;

    InputBlockInfo(){
        group=-1;
        seq=-1;
        flag_trust=false;

    }


    static cJSON* createJsonObj(const InputBlockInfo& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "group", v_info.group);
        cJSON_AddNumberToObject(root, "seq", v_info.seq);

        cJSON_AddBoolToObject(root, "flag_trust", v_info.flag_trust);
        cJSON_AddNumberToObject(root, "flag_pos", v_info.flag_pos);
        cJSON_AddNumberToObject(root, "flag_size", v_info.flag_size);
        cJSON_AddNumberToObject(root, "margin_up", v_info.margin_up);
        cJSON_AddNumberToObject(root, "margin_down", v_info.margin_down);


        //        cout<<"list_point_up_left:"<<v_info.list_point_up_left.size()<<endl;
        //        cout<<"list_point_down_left:"<<v_info.list_point_down_left.size()<<endl;
        //        cout<<"list_point_up_mid:"<<v_info.list_point_up_mid.size()<<endl;
        //        cout<<"list_point_down_mid:"<<v_info.list_point_down_mid.size()<<endl;
        //        cout<<"list_point_up_right:"<<v_info.list_point_up_right.size()<<endl;
        //        cout<<"list_point_down_right:"<<v_info.list_point_down_right.size()<<endl;

        cJSON * arrayitem_up=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_point_up_left.size();i++){
            cJSON* item_tmp=createPointObj(v_info.list_point_up_left[i]);
            cJSON_AddItemToArray(arrayitem_up,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_point_up_left",arrayitem_up);

        arrayitem_up=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_point_up_mid.size();i++){
            cJSON* item_tmp=createPointObj(v_info.list_point_up_mid[i]);
            cJSON_AddItemToArray(arrayitem_up,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_point_up_mid",arrayitem_up);

        arrayitem_up=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_point_up_right.size();i++){
            cJSON* item_tmp=createPointObj(v_info.list_point_up_right[i]);
            cJSON_AddItemToArray(arrayitem_up,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_point_up_right",arrayitem_up);

        cJSON * arrayitem_down=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_point_down_left.size();i++){
            cJSON* item_tmp=createPointObj(v_info.list_point_down_left[i]);
            cJSON_AddItemToArray(arrayitem_down,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_point_down_left",arrayitem_down);

        arrayitem_down=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_point_down_mid.size();i++){
            cJSON* item_tmp=createPointObj(v_info.list_point_down_mid[i]);
            cJSON_AddItemToArray(arrayitem_down,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_point_down_mid",arrayitem_down);

        arrayitem_down=cJSON_CreateArray();
        for(uint i=0;i<v_info.list_point_down_right.size();i++){
            cJSON* item_tmp=createPointObj(v_info.list_point_down_right[i]);
            cJSON_AddItemToArray(arrayitem_down,item_tmp);
        }
        cJSON_AddItemToObject(root,"list_point_down_right",arrayitem_down);
        cout<<__func__<<" end"<<endl;

        return root;
    }


    static InputBlockInfo loadFromJsonObj(cJSON* v_obj){
        InputBlockInfo v_datainfo;
        if(v_obj!= nullptr){

            cJSON* item_child=cJSON_GetObjectItem(v_obj,"group");
            v_datainfo.group=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"seq");
            v_datainfo.seq=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"flag_trust");
            v_datainfo.flag_trust=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"flag_pos");
            v_datainfo.flag_pos=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"flag_size");
            v_datainfo.flag_size=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"margin_up");
            v_datainfo.margin_up=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"margin_down");
            v_datainfo.margin_down=item_child->valueint;

            cJSON *array_up=cJSON_GetObjectItem(v_obj,"list_point_up_left");
            if(cJSON_IsArray(array_up)){
                int num_array = cJSON_GetArraySize(array_up);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array_up,i);
                    v_datainfo.list_point_up_left.push_back(loadFromPointObj(subitem));
                }
            }
            array_up=cJSON_GetObjectItem(v_obj,"list_point_up_mid");
            if(cJSON_IsArray(array_up)){
                int num_array = cJSON_GetArraySize(array_up);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array_up,i);
                    v_datainfo.list_point_up_mid.push_back(loadFromPointObj(subitem));
                }
            }
            array_up=cJSON_GetObjectItem(v_obj,"list_point_up_right");
            if(cJSON_IsArray(array_up)){
                int num_array = cJSON_GetArraySize(array_up);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array_up,i);
                    v_datainfo.list_point_up_right.push_back(loadFromPointObj(subitem));
                }
            }


            cJSON *array_down=cJSON_GetObjectItem(v_obj,"list_point_down_left");
            if(cJSON_IsArray(array_down)){
                int num_array = cJSON_GetArraySize(array_down);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array_down,i);
                    v_datainfo.list_point_down_left.push_back(loadFromPointObj(subitem));
                }
            }
            array_down=cJSON_GetObjectItem(v_obj,"list_point_down_mid");
            if(cJSON_IsArray(array_down)){
                int num_array = cJSON_GetArraySize(array_down);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array_down,i);
                    v_datainfo.list_point_down_mid.push_back(loadFromPointObj(subitem));
                }
            }
            array_down=cJSON_GetObjectItem(v_obj,"list_point_down_right");
            if(cJSON_IsArray(array_down)){
                int num_array = cJSON_GetArraySize(array_down);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array_down,i);
                    v_datainfo.list_point_down_right.push_back(loadFromPointObj(subitem));
                }
            }
        }

        return v_datainfo;

    }



    static string jsonString(const InputBlockInfo & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);

        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }

    string jsonString(){
        return jsonString(*this);
    }

    static int  saveJsonString(std::string path_file,const InputBlockInfo & v_datainfo){
        string jsonstring=jsonString(v_datainfo);
        char* buffer_json;
        buffer_json=const_cast<char*>(jsonstring.c_str());
        basic::saveFile(buffer_json,path_file);
        return 0;
    }

    static InputBlockInfo loadFromJsonString(std::string v_jsonstring){
        InputBlockInfo datainfo_src;
        char* buffer;
        buffer=const_cast<char*>(v_jsonstring.c_str());
        cJSON* root=cJSON_Parse(buffer);

        if(root!=nullptr){
            //            cout<<"is not null"<<endl;

            datainfo_src=loadFromJsonObj(root);
            cJSON_Delete(root);

        }
        else{
            //            cout<<"is  null"<<endl;
        }

        return datainfo_src;
    }

    static InputBlockInfo loadFromFile(std::string v_filename){
        char* buffer;
        basic::getStringFromFile(v_filename,&buffer);
        std::string string_json=buffer;
        return loadFromJsonString(string_json);
    }

}InputBlockInfo;


/**
 * @brief isInputDataComplete_strict:
 * 1. 各组数据包含的图片块是否是一致的，若有一组不同则报错
 * 3. 判断每个方块中图片是否完整，若有一块图片缺失或信息文件缺失，则报错
 * 2. 判断组间各行是否是对应的：根据图片的方格尺寸进行分堆，各组的堆的图片尺寸和图片块数量应对应相同
 *
 * @param dir_input
 * @return
 */
bool isInputDataComplete_strict(const std::string &dir_input);


/**
 * @brief isInputDataComplete_loose  宽松的判断数据完整性方法
 * 1. 4*4的方块数量相同
 * 2. 每个图片块文件夹下均需要有目录
 * @param dir_input
 * @return
 */
bool isInputDataComplete_loose(const std::string &dir_input);


typedef struct BlockID{
    int num_group;
    int num_seq;
    BlockID(int v_group,int v_seq){
        num_group=v_group;
        num_seq=v_seq;
    }
    BlockID(){
        num_group=-1;
        num_seq=-1;
    }


    static cJSON* createJsonObj(const BlockID& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "num_group", v_info.num_group);
        cJSON_AddNumberToObject(root, "num_seq", v_info.num_seq);
        return root;
    }

    static BlockID loadFromJsonObj(cJSON* v_obj){
        BlockID v_datainfo;
        if(v_obj!= nullptr){

            cJSON* item_child=cJSON_GetObjectItem(v_obj,"num_group");
            v_datainfo.num_group=item_child->valueint;

            item_child=cJSON_GetObjectItem(v_obj,"num_seq");
            v_datainfo.num_seq=item_child->valueint;
        }
        return v_datainfo;
    }

}BlockID;


typedef struct BlockList{
    std::vector<BlockID> _list;

    bool isNull(){
        if(_list.size()==0){
            return true;
        }
        else{
            return false;
        }
    }

    void pushBlock(int v_group,int v_seq){
        _list.push_back(BlockID(v_group,v_seq));
    }


    static BlockList getBlockList(const OutputData & v_ouput){
        BlockList lb;
        for(uint n_g=0;n_g<v_ouput.list_group.size();n_g++){
            for(uint n_s=0;n_s<v_ouput.list_group[n_g].list_block.size();n_s++){
                if(v_ouput.list_group[n_g].list_block[n_s].status != ProcessStatus_succeed){
                    int num_group_tmp=static_cast<int>(n_g)+v_ouput.group_start;
                    int num_seq_tmp=static_cast<int>(n_s)+v_ouput.list_group[n_g].col_start;
                    lb.pushBlock(num_group_tmp,num_seq_tmp);
                }
            }
        }

        return lb;
    }

    static cJSON* createJsonObj(const BlockList& v_info){
        cJSON *root = cJSON_CreateObject();

        cJSON * arrayitem=cJSON_CreateArray();
        for(uint i=0;i<v_info._list.size();i++){
            cJSON* item_tmp=BlockID::createJsonObj(v_info._list[i]);
            cJSON_AddItemToArray(arrayitem,item_tmp);
        }
        cJSON_AddItemToObject(root,"_list",arrayitem);
        return root;
    }

    static BlockList loadFromJsonObj(cJSON* v_obj){
        BlockList v_datainfo;
        if(v_obj!= nullptr){

            cJSON *array=cJSON_GetObjectItem(v_obj,"_list");
            if(cJSON_IsArray(array)){
                int num_array = cJSON_GetArraySize(array);
                cJSON* subitem;
                for(int i=0;i<num_array;i++){
                    subitem=cJSON_GetArrayItem(array,i);
                    v_datainfo._list.push_back(BlockID::loadFromJsonObj(subitem));
                }
            }
        }
        return v_datainfo;
    }


    static string jsonString(const BlockList & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }

    string jsonString(){
        return jsonString(*this);
    }

    static int  saveJsonString(std::string path_file,const BlockList & v_datainfo){
        string jsonstring=jsonString(v_datainfo);
        char* buffer_json;
        buffer_json=const_cast<char*>(jsonstring.c_str());
        basic::saveFile(buffer_json,path_file);
        return 0;
    }

    static BlockList loadFromJsonString(std::string v_jsonstring){
        BlockList datainfo_src;
        char* buffer;
        buffer=const_cast<char*>(v_jsonstring.c_str());
        cJSON* root=cJSON_Parse(buffer);

        if(root!=nullptr){

            datainfo_src=loadFromJsonObj(root);
            cJSON_Delete(root);

        }
        else{
        }

        return datainfo_src;
    }

    static BlockList loadFromFile(std::string v_filename){
        char* buffer;
        if(v_filename != ""){
            basic::getStringFromFile(v_filename,&buffer);
            std::string string_json=buffer;
            return loadFromJsonString(string_json);
        }
        else{
            BlockList output;
            return output;
        }

    }


}BlockList;

typedef struct StandardOutputInfo{
    int cols;
    int id;
    bool processed;
    int rows;
    string root;
    string format;

    StandardOutputInfo(int v_cols,
                       int v_id,
                       bool v_processed,
                       int v_rows,
                       const string & v_root,
                       const string & v_format){
        cols=v_cols;
        id=v_id;
        processed=v_processed;
        rows=v_rows;
        root=v_root;
        format=v_format;
    }


    static cJSON* createJsonObj(const StandardOutputInfo& v_info){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "cols", v_info.cols);
        cJSON_AddNumberToObject(root, "id", v_info.id);
        cJSON_AddNumberToObject(root, "rows", v_info.rows);
        cJSON_AddBoolToObject(root, "processed", v_info.processed);
        cJSON_AddStringToObject(root, "root", v_info.root.c_str());
        cJSON_AddStringToObject(root, "format", v_info.format.c_str());

        return root;
    }

    static string jsonString(const StandardOutputInfo & v_datainfo){
        string json;

        cJSON *root =createJsonObj(v_datainfo);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }

        return json;
    }

    string jsonString(){
        return jsonString(*this);
    }


    static int  saveJsonString(std::string path_file,const StandardOutputInfo & v_datainfo){
        string jsonstring=jsonString(v_datainfo);
        char* buffer_json;
        buffer_json=const_cast<char*>(jsonstring.c_str());
        basic::saveFile(buffer_json,path_file);
        return 0;
    }


}StandardOutputInfo;

typedef struct StandardOutputFrame{

    string dir_root;
    int num_start;
    int num_end;
    int num_start_boundry;
    int num_end_boundry;
    int cols;
    int rows;
    int id;
    int layer;

    StandardOutputFrame(const string v_dir_root,
                        int v_start,
                        int v_end,
                        int v_start_boundry,
                        int v_end_boundry,
                        int v_cols,int v_id){
        cout<<__func__<<endl;
        dir_root=v_dir_root;
        num_start=v_start;
        num_end=v_end;
        num_start_boundry=v_start_boundry;
        num_end_boundry=v_end_boundry;
        id=v_id;
        layer=1;
        cols=v_cols;

        rows=(num_start_boundry-num_start)
                +(num_end_boundry-num_start_boundry+1)*2
                +(num_end-num_end_boundry);


    }


    std::vector<string> getImageaPath(int v_group,int v_seq){
        std::vector<string> list_image;
        if(v_group>=cols||v_group<0){
            cout<<">>error: group v_group wrong"<<__func__<<endl;
        }
        if(v_seq>num_end||v_seq<num_start){
            cout<<">>error: group v_seq wrong"<<__func__<<endl;
        }

        int count_image=1;
        int number_start=0;
        if(v_seq<num_start_boundry){
            count_image=1;
            number_start=v_seq-num_start;
        }
        else if(v_seq>=num_start_boundry&&v_seq<=num_end_boundry){
            count_image=2;
            number_start=num_start_boundry-num_start+(v_seq-num_start_boundry)*2;
        }
        else if(v_seq>num_end_boundry){
            count_image=1;
            number_start=num_start_boundry-num_start
                    +(num_end_boundry-num_start_boundry)*2
                    +(v_seq-num_end_boundry)+1;
        }

        for(int c_r=0;c_r<count_image;c_r++){
            for(int c_c=0;c_c<2;c_c++){

                string path_tmp=dir_root
                        +basic::integerToString(id)+"-"
                        +basic::integerToString(layer)+"-"
                        +basic::integerToString(v_group*2+c_c)+"-"
                        +basic::integerToString(number_start+c_r)+".jpg";
                list_image.push_back(path_tmp);

            }

        }

        return list_image;
    }



    int saveStandardInfoFile(const string & v_path){
        StandardOutputInfo ouputinfo(cols*2,id,false,rows,"",".jpg");
        StandardOutputInfo::saveJsonString(v_path,ouputinfo);
        return 0;
    }


}StandardOutputFrame;










}

#endif
