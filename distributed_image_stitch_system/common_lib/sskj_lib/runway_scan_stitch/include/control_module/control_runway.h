#ifndef CONTROL_RUNWAY_H
#define CONTROL_RUNWAY_H


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
namespace control_runway {
/**
   * @brief stitchCompleteInputData
   * 1. 拼接分块后的数据，输入的数据应该被正确分块
   * 2. 将拼接完成的数据保存到指定路径，创建分块图片集合的完整文件目录
   * @param dir_datapath 输入数据路径，
   * @param dir_output   输出数据路径
   */
  void stitchCompleteInputData(const string &dir_datapath,
                               const string &dir_output);

  /**
   * @brief stitchBlockList
   * 1. 根据某一列表拼接指定的图片块
   * 2. 将拼接结果保存相应路径。虽然是拼接指定的数据，但是仍然会完整地创建对应整个分块图片集合的文件路径
   * @param dir_datapath   分块后的数据路径
   * @param path_blocklist 待拼接的图片块列表的路径
   * @param dir_ouput      输出路径
   *
   */
  void stitchBlockList(const string &dir_datapath,
                       const string &path_blocklist,
                       const string &dir_ouput);


  /**
   * @brief outputStandardBlock
   * 1. 将拼接后的数据输出为Viewer能够查看的图片组织形式
   * @param dir_input  输入数据的路径，指分块后的图片数据集合
   * @param dir_out    输出数据路径，指拼接完成后的图片保存路径
   * @param dir_out_standard  标准输出路径，为Viewer能够导入的图片数据的路径
   */
  void outputStandardBlock(const string & dir_input,
                           const string &dir_out,
                           const string &dir_out_standard);


}






#endif
