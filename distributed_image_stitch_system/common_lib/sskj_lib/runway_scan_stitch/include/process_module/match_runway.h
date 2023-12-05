#ifndef IMAGE_MATCH_H
#define IMAGE_MATCH_H

#include <iostream>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "../basic/basic.h"
#include "../config_module/commonpara.h"
#include "../opencv_basic/match.h"

using namespace std;
using namespace cv;


typedef struct MatchPointRefer{
    //set before match
    cv::Point2i point_base;
    cv::Point2i point_match_refer;
    cv::Rect2i  rect_base;
    cv::Rect2i  rect_match_refer;


    //set match result
    cv::Point2i point_match_result;
    cv::Rect2i  rect_match;
    double arracy_match;
    MatchPointRefer(){
        point_base=cv::Point2i(0,0);
        point_match_refer=cv::Point2i(0,0);
        rect_base=cv::Rect2i(0,0,0,0);
        rect_match_refer=cv::Rect2i(0,0,0,0);
        point_match_result=cv::Point2i(0,0);
        rect_match=cv::Rect2i(0,0,0,0);

    }

    MatchPointRefer(cv::Point2i v_point_base){
        point_base=v_point_base;
    }
}MatchPointRefer;



namespace match_runway {

/**
 * @brief calReferRectForList
 * @param list
 * @param offset_x refer_x -base_x
 * @param offset_y refer_y-base_y
 * @return
 */
int calReferRectForList(std::vector<MatchPointRefer>& list,int offset_x,int offset_y);


/**
 * @brief calBasePointList 计算某区域内的中心线的一列点
 * @param rect
 * @param interval
 * @param padding        起始点
 * @param list_basepoint
 * @return
 */
int calBasePointList(Rect rect, int interval, int padding,
                     vector<Point> & list_basepoint);

/**
 * @brief colMostIntenseRect 输入一列区域，以尺寸为size_module进行查找，
 * 得到区域的左上点密集程度最高的的区域，返回包含的区域数量
 * @param list_rect
 * @param height_range
 * @param width_range
 * @param size_module
 * @param rect_target
 * @return
 */
int colMostIntenseRect(const std::vector<Rect>& list_rect,
                       int height_range,
                       int width_range,
                       Size size_module,
                       Rect& rect_target);

enum MatchDirection{
    MatchDirection_Left=0,
    MatchDirection_Right
};



/**
 * @brief calMatchLine 计算两张图片的基准线与匹配线
 * @param mat_base
 * @param mat_match
 * @param list_baseline
 * @param list_matchline
 * @return
 */
int calMatchLine(const cv::Mat& mat_base,const cv::Mat & mat_match,
                 int spliline_base_start_y,int spliline_match_start_y,
                 std::vector<Point> &list_baseline,
                 std::vector<Point> &list_matchline,
                 MatchDirection matchtype);



}


#endif
