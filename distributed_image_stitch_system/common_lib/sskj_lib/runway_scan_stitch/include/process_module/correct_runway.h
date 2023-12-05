#ifndef CORRECT_RUNWAY_H
#define CORRECT_RUNWAY_H


#include <iostream>
#include <vector>
#include <string>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "../basic/basic.h"



using namespace std;
using namespace cv;

namespace correct_runway {



cv::Mat correctImageLengthwise(const cv::Mat & mat_src,
                     LinePara line_up,
                     LinePara line_down ,
                               int height_block,
                               int &start_y,
                               int &end_y);



/**
 * @brief stitchImage 已有三张图片和左右两张图片与中间图片的基准线与匹配线的对应坐标
 * @param mat_left
 * @param mat_mid
 * @param mat_right
 * @param list_baseline_left
 * @param list_matchline_left
 * @param list_baseline_right
 * @param list_matchline_right
 * @return
 */
cv::Mat stitchImage(const cv::Mat &mat_left,
                    const cv::Mat &mat_mid,
                    const cv::Mat &mat_right,
                    const std::vector<Point> &list_baseline_left,
                    const std::vector<Point> &list_matchline_left,
                    const std::vector<Point> &list_baseline_right,
                    const std::vector<Point> &list_matchline_right,
                    int & length_mid_start,
                    int & length_mid_end
                    );



/**
 * @brief correctImageCrosswise 已有一张方格图和方格图上的两条等间距对应的纵向分割线上的点，
 * 以将该分割线校准成直线矫正图片
 * @param mat_src   源图片
 * @param list_point_left_splitline  左侧纵向分割线
 * @param list_point_right_splitline 右侧纵向分割线
 * @param pos_splitline_x_left   矫正后左侧纵向分割线的横坐标
 * @param pos_splitline_x_right  矫正后右侧纵向分割线的横坐标
 * @param start_y_rm             矫正后图片上侧损失的高度
 * @param end_r_rm               矫正后图片下侧损失的高度
 * @return
 */
cv::Mat correctImageCrosswise(const cv::Mat&mat_src,
                              std::vector<Point> list_point_left_splitline,
                              std::vector<Point> list_point_right_splitline,
                              int& pos_splitline_x_left,
                              int& pos_splitline_x_right,
                              int &start_y_rm,
                              int &end_r_rm
                              );











}




#endif
