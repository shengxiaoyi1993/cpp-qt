#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>


using namespace cv;

namespace transformation {

cv::Mat rotateAndCutImage(const cv::Mat& mat_src,double angle,Rect2i &rect_in);

cv::Mat cutImageByLine(const int startline,
                       const int endline,
                       const Mat& mat_src);

cv::Mat cutImageByCol(const int startcol,
                      const int endcol,
                      const Mat& mat_src);

/**
 * @brief cutMatIntoSeveralChunk
 * @param mat_src
 * @param num_chunk
 * @return
 */
std::vector<cv::Mat>  cutMatIntoSeveralChunkCrosswise(const cv::Mat& mat_src,int num_chunk);

/**
 * @brief cutMatIntoSeveralChunkLengthwise
 * @param mat_src
 * @param num_chunk
 * @return
 */
std::vector<cv::Mat>  cutMatIntoSeveralChunkLengthwise(const cv::Mat& mat_src,int num_chunk);


enum ImageReverse{
    ImageReverse_right=0,
    ImageReverse_left
};
enum Direction{
    Direction_Left=0,
    Direction_Right
};

/**
 * @brief getImageCorrectCrosswise 将图片根据点数组进行矫正，使点数组的线在矫正之后为一条直线
 * @param mat_src
 * @param list_point_correct
 * @param interval_lengthwise 点数组的原纵向间距，横向间距为0
 * @param width_target  保留部分的图片将被矫正成该宽度
 * @param flag          保留左侧还是右侧
 * @return
 */
cv::Mat getImageCorrectCrosswise(const cv::Mat& mat_src,
                                 std::vector<Point2i> list_point_correct,
                                 int interval_lengthwise,
                                 int width_target,
                                 ImageReverse flag);

int adjustGrayAroundLengthwiseLine(cv::Mat &mat_src,
                                   int interval_length,
                                   const std::vector<float> list_gray_offset,
                                   int line_x,
                                   int width_adjust,
                                   Direction direction
                                   );
/**
 * @brief getImageDivide 将某一图片分成若干份
 * @param mat_src
 * @param v_num
 * @param v_list_path
 * @return
 */
int getImageDivide(const cv::Mat & mat_src,cv::Size v_size,
                   int v_num,const std::vector<std::string> v_list_path);

int getMakeUp(cv::Size v_target_size,const std::vector<std::string> v_list_path);



}




#endif // IMAGETRANSFORM_
