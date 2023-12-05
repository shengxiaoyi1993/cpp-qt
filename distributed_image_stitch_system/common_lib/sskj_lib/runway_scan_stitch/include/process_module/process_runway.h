#ifndef PROCESS_RUNWAY_H
#define PROCESS_RUNWAY_H

#include <iostream>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "../basic/basic.h"
#include "../config_module/commonpara.h"
#include "../opencv_basic/match.h"
#include "../runwayio/runwayio.h"


using namespace std;
using namespace cv;


namespace process_runway {

enum CorrectType{
    CorrectType_right_only=0,
    CorrectType_left_only,
    CorrectType_right_and_left

};


/**
 * @brief processToCorrectCrosswise图片完成拼接后的横向矫正过程，先进行图片纵向分割线的
 * 识别，再进行图片的横向矫正。
 * @param mat_src   源图片
 * @param mat_dst   目标图片
 * @param pos_splitline_x_left  矫正后左侧纵向分割线的横坐标
 * @param pos_splitline_x_right 矫正后右侧纵向分割线的横坐标
 * @param start_y_rm       矫正过程中图片上侧损失的高度
 * @param end_r_rm         矫正过程中图片下侧损失的高度
 * @param flag_func_rmguideline  是否在检测分割线时剔除引导线，4*4建议选true，2*2建议选false
 * @param v_correcttype          矫正类型，矫正左侧的分割线或这右侧的分割线，或者两分割线都需要矫正
 * @return
 */
int processToCorrectCrosswise(const Mat& mat_src,
                              Mat& mat_dst,
                              int& pos_splitline_x_left,
                              int& pos_splitline_x_right,
                              int &start_y_rm,
                              int &end_r_rm,
                              bool flag_func_rmguideline=true,
                              CorrectType v_correcttype=CorrectType_right_and_left
                              );

/**
 * @brief processToStitch
 * @param mat_left  左图
 * @param mat_mid   中图
 * @param mat_right 右图
 * @param mat_dst   结果图
 * @param pos_splitline_y_start  结果图中上分割线的纵坐标
 * @param pos_splitline_y_end    结果图中下分割线的纵坐标
 * @param size_target            预先给定的结果图中方格的目标尺寸
 * @return          若拼接成功则输出0；若异常，则输出非0
 */
int processToStitch(const Mat& mat_left,
                    const Mat& mat_mid,
                    const Mat& mat_right,
                    Mat& mat_dst,
                    int& pos_splitline_y_start,
                    int& pos_splitline_y_end,
                    Size size_target,
                    runwayio::InputBlockInfo v_blkinfo,
                    bool hasPreInfo
                    );


typedef struct BlockStitchPara{


    int num_group;
    int num_seq;
    runwayio::InputFrame frame_in;
    runwayio::OutputFrame frame_out;

    BlockStitchPara(){
        num_group=-1;
        num_seq=-1;
    }

    BlockStitchPara(int v_group,int v_seq,const string &dir_in,const string &dir_out){
        num_group=v_group;
        num_seq=v_seq;
        frame_in.dir_root=dir_in;
        frame_out.dir_root=dir_out;
    }

    bool isNull(){
        if((num_group>=0)){
            return false;
        }
        else{
            return true;
        }
    }

    string blockDir_in(){
        return frame_in.getBlockDir(num_group,num_seq);
    }
    string blockDir_out(){
        return frame_out.getBlockDir(num_group,num_seq);
    }

    string blockInfoPath_in(){
        return frame_in.getBlockInfo(num_group,num_seq);
    }

    string blockInfoPath_out(){
        return frame_out.getBlockInfo(num_group,num_seq);
    }



}BlockStitchPara;


void stitchImageBlock(BlockStitchPara v_para);



/**
 * @brief stitchImageInDir 拼接文件中的图片,返回拼接好后的图片和分割线的中心位置
 * @param dir_src
 * @return
 */

/**
 * @brief stitchImageInDir 拼接某个文件夹中的
 * @param dir_src
 * @param mat_target
 * @param pos_splitline
 * @param v_blokdata
 * @param path_debug
 * @param v_correcttype 用来判断需要矫正的纵向分割线，左边缘处不矫正左边缘
 * @return
 */
int stitchImageInDir(const string & dir_src,
                     Mat & mat_target,
                     runwayio::SplitLinePos& pos_splitline,
                     runwayio::BlockData & v_blokdata,
                     string path_debug=""
                     , CorrectType v_correcttype=CorrectType_right_and_left);




}

#endif
