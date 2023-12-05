#ifndef DETECT_RUNWAY_H
#define DETECT_RUNWAY_H

#include <iostream>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "../basic/basic.h"
#include "../config_module/commonpara.h"

#define DEBUG

using namespace std;
using namespace cv;

namespace detect_runway{

LinePara getSplitLineWithGrayMean();

LinePara getSplitLineWithGrayGrad(Mat mat_src);

/**
 * @brief calFirstLineForBlockCut: 已知一个张图片，上有
 * @param mat
 * @param offset
 * @param v_width
 * @return
 */
LinePara calCenterLineForBlockCutCrosswise(const cv::Mat &mat,int offset,int v_width );

typedef struct Para_calCenterLineForBlockCutForBinaryCrosswise {
    int offset;
    int width;
    Para_calCenterLineForBlockCutForBinaryCrosswise(){
        offset=0;
        width=0;
    }
    Para_calCenterLineForBlockCutForBinaryCrosswise(int v_offset,int v_width){
        offset=v_offset;
        width=v_width;
    }

    static cJSON* createJsonObj(
            const Para_calCenterLineForBlockCutForBinaryCrosswise& para){
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "offset", para.offset);
        cJSON_AddNumberToObject(root, "width", para.width);
        return root;
    }

    static Para_calCenterLineForBlockCutForBinaryCrosswise
    loadFromJsonObj(cJSON* v_obj){
        Para_calCenterLineForBlockCutForBinaryCrosswise v_para;
        if(v_obj!= nullptr){
            cJSON* item_child=cJSON_GetObjectItem(v_obj,"offset");
            v_para.offset=item_child->valueint;
            item_child=cJSON_GetObjectItem(v_obj,"width");
            v_para.width=item_child->valueint;
        }
        return v_para;
    }

    static Para_calCenterLineForBlockCutForBinaryCrosswise
    loadFromJsonString(std::string v_jsonstring){
        Para_calCenterLineForBlockCutForBinaryCrosswise datainfo_src;
        char* buffer;
        buffer=const_cast<char*>(v_jsonstring.c_str());
        cJSON* root=cJSON_Parse(buffer);

        if(root!=nullptr){
            datainfo_src=loadFromJsonObj(root);
            cJSON_Delete(root);
        }

        return datainfo_src;
    }

    static string jsonString(Para_calCenterLineForBlockCutForBinaryCrosswise v_info){
        string json;
        cJSON *root =createJsonObj(v_info);
        char* str_res=cJSON_Print(root);
        if(str_res!=nullptr){
            json=str_res;
        }
        return json;
    }


    static int  saveJsonString(
            std::string path_file,
            const Para_calCenterLineForBlockCutForBinaryCrosswise & v_datainfo){
        string jsonstring=jsonString(v_datainfo);
        char* buffer_json;
        buffer_json=const_cast<char*>(jsonstring.c_str());
        basic::saveFile(buffer_json,path_file);
        return 0;
    }

    static Para_calCenterLineForBlockCutForBinaryCrosswise
    loadFromFile(std::string v_filename){
        char* buffer;
        basic::getStringFromFile(v_filename,&buffer);
        std::string string_json=buffer;
        return loadFromJsonString(string_json);
    }

}Para_calCenterLineForBlockCutForBinaryCrosswise;



LinePara calCenterLineForBlockCutForBinaryCrosswise(const cv::Mat &mat,int offset,
                                                    int v_width );

/**
 * @brief calFirstLineForBlockCutForBinary：二值化后的图片上有一条直线，已知直线通过图片的中心
 * 和图片的宽度，且直线的倾斜角在一定范围内，求图片的倾斜角
 * @param mat
 * @param offset
 * @param v_width
 * @return
 */

LinePara calFirstSplitLine(
        const cv::Mat &mat_src
        #ifdef DEBUG
        , string dir_test=""
        #endif
        );

LinePara calLastSplitLine(const cv::Mat &mat_src);


/**
 * @brief calCenterLineForBlockCutLengthwise :假设图片中仅有一条纵向的连续曲线，
 * 检测这条直线，输出直线上的点列
 * @param mat
 * @return
 */
vector<Point> calCenterLineForBlockCutLengthwise(const cv::Mat &mat );

/**
 * @brief calCenterLineForBlockCutLengthwiseV1 :假设图片中仅有一条纵向的连续曲线，
 * 检测这条直线，输出直线上的点列
 * @param mat
 * @param flag_func_rmguideline 是否剔除引导线
 * @return
 */
vector<Point>  calCenterLineForBlockCutLengthwiseV1(const cv::Mat &mat,
                                                    bool flag_func_rmguideline=true);




int getSplitlineWidthList(const Mat&mat_src,
                          int num,
                          LinePara splitline,
                          std::vector<int> &list_width,
                          std::vector<Point> &list_braycenter
                          #ifdef DEBUG
                          , string path_debugfile
                          #endif
                          );

typedef  struct InterMediateBlock{
    uint start_seg;
    uint end_seg;
    uint length;
    std::vector<bool> list_used;
    std::vector<basic::Segment> list_segment;
    std::vector<int> list_height_segment;
    std::vector<std::vector<basic::Segment>> list_guideline;
    int resize(unsigned long v_list,unsigned long v_seg){
        list_guideline.clear();
        list_guideline.resize(v_list);
        list_used.resize(v_list);

        for(uint i=0;i<v_list;i++){

            list_guideline[i].resize(v_seg);
            list_used[i]=false;
        }
        return 0;
    }


} InterMediateBlock;


typedef struct GuideLine{
    std::vector<basic::Segment> list_segment;
    basic::Segment segment_range_end;
    bool flag_used;

    GuideLine(){
        flag_used=false;
        list_segment.clear();
        segment_range_end=basic::Segment();
    }
    GuideLine(basic::Segment  v_segment_range_end,std::vector<basic::Segment> v_list){
        flag_used=false;
        segment_range_end=v_segment_range_end;
        list_segment=v_list;
    }

    void addSegmentList(const std::vector<basic::Segment> v_list_segment_add,basic::Segment v_segment_range_end){
        list_segment.insert(list_segment.end(),v_list_segment_add.begin(),v_list_segment_add.end());
        segment_range_end=v_segment_range_end;
    }

    /**
     * @brief isEndInEdge 判断引导线是否与某个区域的边缘相较，用于判断是否是引导线的依据
     * @param rect_block
     * @param v_interval_y
     * @param v_interval_x
     * @return
     */
    bool isEndInEdge(Rect rect_block,int v_interval_y,int v_interval_x){
        unsigned long listsize=list_segment.size();
        if(listsize==0){
            return false;
        }

        int left_edge_x=rect_block.x;
        int up_edge_y=rect_block.y;
        int right_edge_x=rect_block.x+rect_block.width;
        int down_edge_y=rect_block.y+rect_block.height;

        unsigned long num_segment=5;

        int up_y=list_segment[0].point0.y;
        int down_y=list_segment[listsize-1].point0.y;


        if((abs(up_y-up_edge_y)<v_interval_y&&!basic::Segment::isNull(list_segment[0]))
                ||((abs(down_y-down_edge_y)<v_interval_y)&&!basic::Segment::isNull(list_segment[listsize-1]))){
            return true;
        }

        num_segment=num_segment<(listsize)?
                    num_segment:(listsize);

        int min_start_x=right_edge_x;
        int max_start_x=left_edge_x;
        int min_end_x=right_edge_x;
        int max_end_x=left_edge_x;

        cout<<"left_edge_x: "<<left_edge_x<<endl;
        cout<<"right_edge_x: "<<right_edge_x<<endl;


        for(uint i=0;i<num_segment;i++){
            if(!list_segment[i].isNull()){
                cout<<"list_segment[i]"<<endl;
                list_segment[i].print();
                min_start_x=min_start_x<list_segment[i].point0.x?
                            min_start_x:list_segment[i].point0.x;
                max_start_x=max_start_x>list_segment[i].point1.x?
                            max_start_x:list_segment[i].point1.x;
            }
            if(!list_segment[listsize-i-1].isNull()){
                cout<<"list_segment[listsize-i-1]."<<endl;
                list_segment[listsize-i-1].print();
                min_end_x=min_end_x<list_segment[listsize-i-1].point0.x?
                            min_end_x:list_segment[listsize-i-1].point0.x;
                max_end_x=max_end_x>list_segment[listsize-i-1].point1.x?
                            max_end_x:list_segment[listsize-i-1].point1.x;
            }
        }

        cout<<"min_start_x: "<<min_start_x<<endl;
        cout<<"max_start_x: "<<max_start_x<<endl;
        cout<<"min_end_x: "<<min_end_x<<endl;
        cout<<"max_end_x: "<<max_end_x<<endl;


        if(abs(min_start_x-left_edge_x)<v_interval_x
                ||abs(min_end_x-left_edge_x)<v_interval_x
                ||abs(max_start_x-right_edge_x)<v_interval_x
                ||abs(max_end_x-right_edge_x)<v_interval_x
                ){
            cout<<"x meet"<<endl;
            return true;
        }

        return false;
    }

}GuideLine;

typedef  struct AreaSign{
    enum SignFlag{
        SignFlag_normal=0,
        SignFlag_positive,
        SignFlag_negtive,
        SignFlag_nocomfirm
    };
    Vec2i range;
    SignFlag flag_sign;
    int up_x;
    int down_x;

    AreaSign(){
        range =Vec2i(-1,-1);
        flag_sign=SignFlag_nocomfirm;
    }

    AreaSign(Vec2i v_range,SignFlag v_sign){
        range=v_range;
        flag_sign=v_sign;
    }

    AreaSign(Vec2i v_range){
        range =v_range;
        flag_sign=SignFlag_nocomfirm;
    }



} AreaSign;



/**
 * @brief getNewSegment 更改线段列表的横坐标
 * @param list_segment
 * @return
 */
std::vector<basic::Segment> getNewSegmentList(const std::vector<basic::Segment> & list_segment,int y_offset,int v_blocklength);


}






#endif
