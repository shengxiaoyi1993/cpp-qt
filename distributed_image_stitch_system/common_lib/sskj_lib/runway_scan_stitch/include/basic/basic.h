#ifndef BASIC_H
#define BASIC_H

#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "cJSON.h"
#include <float.h>
#include <fstream>
#include<ctime>

using namespace std;
using namespace cv;

#define LOGERRORPOS(x)  cout<<" Func:"<<__func__<<" Line:"<<__LINE__<<" Error:"<<(x)<<endl;



typedef struct TwoInt{
  int __a;
  int __b;
  TwoInt(){}
  TwoInt(int v_a,int v_b):
    __a(v_a),__b(v_b)
  {
  }
  TwoInt(const string &v_json){
    int ret=0;
    cJSON *root=cJSON_Parse(v_json.c_str());
    if (cJSON_IsNull(root)) {
      ret=-1;
    }


    if (ret == 0) {
      cJSON* item= cJSON_GetObjectItem(root,"__a");
      if (cJSON_IsNull(item)) {
        ret=-1;
      }
      else{
        if (cJSON_IsNumber(item)) {
          __a=item->valueint;
        }
        else{
          ret=-1;

        }
      }
    }

    if (ret == 0) {
      cJSON* item= cJSON_GetObjectItem(root,"__b");
      if (cJSON_IsNull(item)) {
        ret=-1;
      }
      else{
        if (cJSON_IsNumber(item)) {
          __b=item->valueint;
        }
        else{
          ret=-1;
        }
      }
    }
  }

  cJSON* jsonobj(){
    cJSON* root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"__a",__a);
    cJSON_AddNumberToObject(root,"__b",__b);
    return root;

  }


} TwoInt;

typedef struct FourInt{
  int __a;
  int __b;
  int __c;
  int __d;

  FourInt(){}
  FourInt(int v_a,int v_b,int v_c,int v_d):
    __a(v_a),__b(v_b),__c(v_c),__d(v_d)
  {
  }
  FourInt(const string &v_json){
    int ret=0;
    cJSON *root=cJSON_Parse(v_json.c_str());
    if (cJSON_IsNull(root)) {
      ret=-1;
    }


    if (ret == 0) {
      cJSON* item= cJSON_GetObjectItem(root,"__a");
      if (cJSON_IsNull(item)) {
        ret=-1;
      }
      else{
        if (cJSON_IsNumber(item)) {
          __a=item->valueint;
        }
        else{
          ret=-1;

        }
      }
    }

    if (ret == 0) {
      cJSON* item= cJSON_GetObjectItem(root,"__b");
      if (cJSON_IsNull(item)) {
        ret=-1;
      }
      else{
        if (cJSON_IsNumber(item)) {
          __b=item->valueint;
        }
        else{
          ret=-1;
        }
      }
    }

    if (ret == 0) {
      cJSON* item= cJSON_GetObjectItem(root,"__c");
      if (cJSON_IsNull(item)) {
        ret=-1;
      }
      else{
        if (cJSON_IsNumber(item)) {
          __c=item->valueint;
        }
        else{
          ret=-1;
        }
      }
    }

    if (ret == 0) {
      cJSON* item= cJSON_GetObjectItem(root,"__d");
      if (cJSON_IsNull(item)) {
        ret=-1;
      }
      else{
        if (cJSON_IsNumber(item)) {
          __d=item->valueint;
        }
        else{
          ret=-1;
        }
      }
    }
  }

  cJSON* jsonobj(){
    cJSON* root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"__a",__a);
    cJSON_AddNumberToObject(root,"__b",__b);
    cJSON_AddNumberToObject(root,"__c",__c);
    cJSON_AddNumberToObject(root,"__d",__d);
    return root;

  }


} FourInt;

inline
Point pointFromCjson(const cJSON* c_json){
  Point point_tmp;
  if (!cJSON_IsNull(c_json)) {
    cJSON* item=cJSON_GetObjectItem(c_json,"x");
    if ((!cJSON_IsNull(item)) && (cJSON_IsNumber(item))) {
      point_tmp.x=item->valueint;
    }

    item=cJSON_GetObjectItem(c_json,"y");
    if ((!cJSON_IsNull(item)) && (cJSON_IsNumber(item))) {
      point_tmp.y=item->valueint;
    }
  }
  return point_tmp;
}


inline
cJSON* jsonObjFromPoint(const Point &v_point){
  cJSON* root=cJSON_CreateObject();
  cJSON_AddNumberToObject(root,"x",v_point.x);
  cJSON_AddNumberToObject(root,"y",v_point.y);
  return root;
}


typedef  struct TwoPointList{
  vector<Point> __l_a;
  vector<Point> __l_b;

  TwoPointList(){}
  TwoPointList(const vector<Point> v_la,const vector<Point> v_lb):
    __l_a(v_la),__l_b(v_lb)
  {

  }

  TwoPointList(const string &v_json){
    int ret=0;
    cJSON *root=cJSON_Parse(v_json.c_str());
    if (cJSON_IsNull(root)) {
      ret=-1;
    }

    cJSON* item_array=cJSON_GetObjectItem(root,"__l_a");
    if (cJSON_IsNull(item_array)||(!cJSON_IsArray(item_array))) {
      ret=-1;
    }
    else{
      int size=cJSON_GetArraySize(item_array);
      for (int i=0;i<size;i++) {
        cJSON *item=cJSON_GetArrayItem(item_array,i);
        if (cJSON_IsNull(item)) {
          ret=-1;
        }
        else{
          __l_a.push_back(pointFromCjson(item));
        }
      }
    }

    item_array=cJSON_GetObjectItem(root,"__l_b");
    if (cJSON_IsNull(item_array)||(!cJSON_IsArray(item_array))) {
      ret=-1;
    }
    else{
      int size=cJSON_GetArraySize(item_array);
      for (int i=0;i<size;i++) {
        cJSON *item=cJSON_GetArrayItem(item_array,i);
        if (cJSON_IsNull(item)) {
          ret=-1;
        }
        else{
          __l_b.push_back(pointFromCjson(item));
        }
      }
    }

  }

  cJSON* jsonobj(){

    cJSON* root=cJSON_CreateObject();
    cJSON* item_array_a=cJSON_CreateArray();
    for (unsigned int i=0;i<__l_a.size();i++) {
      cJSON_AddItemToArray(item_array_a,jsonObjFromPoint(__l_a[i]));
    }
    cJSON_AddItemToObject(root,"__l_a",item_array_a);

    cJSON* item_array_b=cJSON_CreateArray();

    for (unsigned int i=0;i<__l_b.size();i++) {
      cJSON_AddItemToArray(item_array_b,jsonObjFromPoint(__l_b[i]));
    }
    cJSON_AddItemToObject(root,"__l_b",item_array_b);

    return root;

  }


} TwoPointList;

typedef struct TwoPointListAndTwoInt{

  TwoPointList __pl_two;
  TwoInt __twoint;
  TwoPointListAndTwoInt(){}

  TwoPointListAndTwoInt(const TwoPointList &v_pl_two,
                        const TwoInt &v_twoint):
    __pl_two(v_pl_two),__twoint(v_twoint)
  {
  }

  TwoPointListAndTwoInt(const string &v_json){
    cJSON* root=cJSON_Parse(v_json.c_str());
    if (!cJSON_IsNull(root)) {
      cJSON* item=cJSON_GetObjectItem(root,"__pl_two");
      if (!cJSON_IsNull(item)) {
        __pl_two=TwoPointList(cJSON_Print(item));
      }
      item=cJSON_GetObjectItem(root,"__twoint");
      if (!cJSON_IsNull(item)) {
        __twoint=TwoInt(cJSON_Print(item));
      }
    }
  }


  cJSON* jsonobj(){
    cJSON* root=cJSON_CreateObject();
    cJSON_AddItemToObject(root,"__pl_two",__pl_two.jsonobj());
    cJSON_AddItemToObject(root,"__twoint",__twoint.jsonobj());
    return root;
  }




}TwoPointListAndTwoInt;


inline
cv::Point getPointInLineX(const cv::Point &v_point0,
                          const cv::Point &v_point1,
                          int v_x
                          ){
  if (v_point0.y==v_point1.y) {
    return cv::Point(v_x,v_point0.y);
  }
  else{
    return cv::Point(v_x,(v_x-v_point0.x)*(v_point1.y-v_point0.y)/(v_point1.x-v_point0.x)+v_point0.y);
  }
}

typedef struct LinePara{
  double k;
  double b;
  LinePara(){
    k=0;
    b=0;
  }
  LinePara(double v_k,double v_b){
    k=v_k;
    b=v_b;
  }
  double getY(double v_x){
    return k*v_x+b;
  }
  double getX(double v_y){
    if(k>=-DBL_MIN&&k<=DBL_MIN){
      cout<<"DBL_MIN:"<<DBL_MIN<<endl;
      cout<<"k:"<<k<<endl;
      return 0;

      //            assert(0);
    }
    else{
      return (v_y-b)/k;
    }
  }
  static LinePara getLine(Point point0,Point point1){
    LinePara line_res;
    line_res.k=(point1.y-point0.y)*1.0/(point1.x-point0.x);
    line_res.b=point0.y-line_res.k*point0.x;
    return line_res;
  }

  static cJSON* createJsonObj(const LinePara& v_line){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "k", v_line.k);
    cJSON_AddNumberToObject(root, "b", v_line.b);
    return root;

  }


  static string jsonString(LinePara v_line){
    string json;

    cJSON *root =createJsonObj(v_line);
    char* str_res=cJSON_Print(root);
    if(str_res!=nullptr){
      json=str_res;
    }
    return json;
  }

  string jsonString(){
    return jsonString(*this);
  }

  static LinePara loadFromJsonObj(cJSON* v_obj){
    LinePara v_line;
    if(v_obj!= nullptr){
      cJSON* item_child=cJSON_GetObjectItem(v_obj,"k");
      v_line.k=item_child->valuedouble;
      item_child=cJSON_GetObjectItem(v_obj,"b");
      v_line.b=item_child->valuedouble;
    }
    return v_line;

  }

  static LinePara loadFromJsonString(const string & v_json){
    cJSON* v_obj = cJSON_Parse(v_json.c_str());
    return loadFromJsonObj(v_obj);
  }

  Point getPointGivenY(int v_y){
    return Point(static_cast<int>(getX(v_y)),v_y);
  }

  Point getPointGivenX(int v_x){
    return Point(v_x,static_cast<int>(getY(v_x)));
  }



}LinePara;


namespace basic {






inline
string getStringFromFile(const string& v_file){
  string ret;
  ifstream ifs(v_file);
  if (ifs.is_open()) {
    //    input>>ret;
    ifs.seekg(0,std::ios::end);
    int len = ifs.tellg();
    ifs.seekg(0,std::ios::beg);

    char *buff = new char[len];

    ifs.read(buff , len);
    ret=buff;
    delete []buff;
  }
  return ret;

}


/**
 * print mat as array of number
 * @param image image of source
 */
void printMatNum(const Mat image);

/**
 * get max value of every col of image
 * @param  array source image
 * @return       the value-list of max value of every col
 */
std::vector<int> getMaxValueList(const Mat &array);

int getMaxValue(const Mat &array);



void printMatInfo(Mat mat);

void showMatTransform(const Mat Image);
/**
 * calConvexHull caculate the convex hull of these points, result saved in list
 * @param list_point [description]
 */
std::vector<cv::Point2i> calConvexHull(const std::vector<cv::Point2i>& list_point);

void drawPointList(const std::vector<cv::Point2i>& list_point,cv::Mat& image);

void drawPointList(const std::vector<cv::Point2i>& list_point,cv::Mat& image,
                   const std::vector<int> list_num);




void drawMultiLineShape(const std::vector<cv::Point2i>& list_point,cv::Mat& image);


void displayMatWithSize(cv::Mat mat,std::string name,cv::Size size );


/**
 * this function is designed to get image after ritating a certain angle,
 * when the angle larger than certain value ,it may not work well
 * @param  mat_src [description]
 * @param  angle   [description]
 * @return         [description]
 */
cv::Mat rotateAndCutImage(const cv::Mat& mat_src,double angle);


int calMeanAndSD(const cv::Mat& mat_src,double &value_mean,double&value_sd);

/**
 * @brief drawLineCol draw line lengthwise
 * @param mat
 * @param c
 */
void drawLineCol( cv::Mat &mat,int c);

/**
 * @brief drawLineRow
 * @param mat
 * @param r
 */
void drawLineRow( cv::Mat &mat,int r);


/**
 * @brief drawLineRow draw line crosswise
 * @param mat
 * @param r
 * @param thickness
 */
void drawLineRow( cv::Mat &mat,int r,int thickness);



/**
 * @brief calcGrayHist
 * @param image
 * @return mat of grayhist ,which has [1*256] in size,1 in rows and 256 in cols.
 *         num_col:       gray value of pixes of source image,
 *         value of pixes:quentity of pixes in source image
 */
Mat calcGrayHist(const Mat & image) ;


/**
 * @brief calcGrayHistWithNormalization
 * @param mat_src
 * @return grayhist ,which has [1*256] in size,1 in rows and 256 in cols.
 *         num_col:       gray value of pixes of source image,
 *         value of pixes:percentage of pixes in source image
 */
Mat calcGrayHistWithNormalization(const Mat & mat_src) ;



/**
 * @brief printHistogram
 * @param array
 * @param size size of target image
 * @return mat to show the histgram, using the height of bar to repeat the
 *         quentity of pixes
 */
Mat printHistogram(const Mat& array,Size size);


Mat printGram(const Mat& mat_list);


Mat printGram(const Mat& mat_list,int height);


/**
 * @brief calcuate the mean crosswise
 * @param image
 * @return mat of mean crosswisem, which has [1*[image.rows]] in size
 */
cv::Mat calMeanForCol(const cv::Mat &image);



/**
 * @brief this function is designed to show the distribution of list_mean\
 *        it is usually used after [func]calMeanForCol
 * @param list_mean should has rows=1
 * @return
 */
cv::Mat drawDistributionInRow(const cv::Mat& list_mean);

double crossProduct(const cv::Vec2d& vec1,const cv::Vec2d& vec2);

cv::Point2i getMidPoint(const std::vector<cv::Point2i> & list);
Point2d getMidPoint(const std::vector<cv::Point2d> & list);


cv::Mat drawLine(const cv::Mat& mat_src,LinePara line);
int getMidValue(std::vector<int> list_value);

/**
 * @brief insert the number to list of number ,when the length of list larger than
 *        length ,remove the smallest
 * @param list_num
 * @param length
 * @return
 */
int insertOrderLy(vector<int> & list_num,int num,int length);

/**
 * @brief filterSmallBlock map in the image,set zero all the block of size_module
 *        whose mean is lower than thresold,set 255 all the block larger than thresold
 *        for grayscale
 * @param mat_src
 * @param size_module
 * @return
 */

cv::Mat filterSmallBlock(const cv::Mat& mat_src,Size2i size_module ,int thresold);

/**
 * @brief filterSmallBlockEx map in the image,set zero all the block of size_module
 *        whose mean is lower than thresold,set 255 all the block larger than thresold
 *        for grayscale，save the counts of block in cross in a list
 * @param mat_src
 * @param size_module
 * @param thresold
 * @param mat_distribution
 * @return
 */
cv::Mat filterSmallBlockEx(const cv::Mat& mat_src,Size2i size_module ,int thresold,
                           cv::Mat& mat_distribution);

cv::Mat filterSmallBlockLengthwise(const cv::Mat& mat_src,Size2i size_module ,int thresold,
                                   cv::Mat& mat_distribution);

cv::Mat filterSmallBlockCrosswise(const cv::Mat& mat_src,Size2i size_module ,int thresold,
                                  cv::Mat& mat_distribution);

enum Direction{
  direction_Crosswise=0,
  direction_Lengthwise
};

cv::Mat getDistributionMatOfBinaryMat(const cv::Mat &mat_binary,Direction type);

int calMean(const Mat &src);

int calMeanEx(const cv::Mat & mat_src,Rect2i rect_block,int num_rm);

int calMean(const cv::Mat & mat_src,Rect2i rect_block);


int setBrightBlock( cv::Mat & mat_src,Rect2i rect_block);

int drawRectToImage(cv::Mat &mat,std::vector<cv::Rect2i> list_rect);

int drawRectToImage(cv::Mat &mat,cv::Rect2i rect);


void copyFile(string file_src, string file_dst);

void mkDir(string dir);
void rmDir(string dir);


std::string integerToString(int num);

int countBrightPix(const cv::Mat & mat_src);


/**
 * @brief countBrightPixInConnectDomainIncludePoint 计算某点所在连通域的所有像素点数量
 * @param mat_src
 * @param point
 * @return
 */
int countBrightPixInConnectDomainIncludePoint(const cv::Mat & mat_src,
                                              cv::Point point,
                                              cv::Point& point_center);

/**
 * @brief separatePositiveAndNegtive 分离图片的正向和负向分量
 * @param mat_src
 * @param mat_positive
 * @param mat_negtive
 * @return
 */
int separatePositiveAndNegtive(const cv::Mat& mat_src,
                               cv::Mat &mat_positive,
                               cv::Mat &mat_negtive);

Point2i getBaryCenterOfBinaryMat(const cv::Mat& mat_src);


typedef struct Segment{
  Point2i point0;
  Point2i point1;
  Segment(Point2i v_point0,Point2i v_point1){
    point0=v_point0;
    point1=v_point1;
  }
  Segment(){
    point0=Point2i(0,0);
    point1=Point2i(0,0);
  }
  bool isNull(){
    if(point0==Point(0,0)&&
       point1==Point(0,0)
       ){
      return true;
    }
    else{
      return false;
    }
  }

  static bool isNull(const Segment &v_seg){
    if(v_seg.point0==Point(0,0)&&
       v_seg.point1==Point(0,0)
       ){
      return true;
    }
    else{
      return false;
    }
  }

  static  bool isCrossInX(const Segment & seg_0,const Segment& seg_1){
    int start_0;
    int end_0;
    int start_1;
    int end_1;
    if(seg_0.point0.x>seg_0.point1.x){
      start_0=seg_0.point1.x;
      end_0=seg_0.point0.x;
    }
    else{
      start_0=seg_0.point0.x;
      end_0=seg_0.point1.x;
    }

    if(seg_1.point0.x>seg_1.point1.x){
      start_1=seg_1.point1.x;
      end_1=seg_1.point0.x;
    }
    else{
      start_1=seg_1.point0.x;
      end_1=seg_1.point1.x;
    }

    if(end_1>=start_0&&end_0>=start_1){
      return true;
    }
    else{
      return false;
    }

  }

  Segment(const Segment &v_seg){
    point0=v_seg.point0;
    point1=v_seg.point1;
  }
  void get(const Segment &v_seg){
    point0=v_seg.point0;
    point1=v_seg.point1;
    return;
  }

  void print(){
    std::cout<<"point0:"<<point0<<std::endl;
    std::cout<<"point1:"<<point1<<std::endl;
    return;
  }
  /**
     * @brief calAvailableLength 计算某线段列表的有效长度
     * @param v_list_seg
     * @return
     */
  static int calAvailableLength( std::vector<Segment> &v_list_seg){
    int start_y=-1;
    int end_y=-1;
    for(uint i=0;i<v_list_seg.size();i++){
      if(!v_list_seg[i].isNull()){
        if(start_y==-1){
          start_y=v_list_seg[i].point0.y;
        }
        end_y=v_list_seg[i].point0.y;
      }
    }
    if(start_y==-1||end_y==-1){
      return -1;
    }
    else{
      return end_y-start_y;
    }
  }

  int distance(){
    if(point0.y==point1.y){
      return abs(point0.x-point1.x);
    }
    else if(point0.x==point1.x){
      return abs(point0.y-point1.y);
    }
    else{
      return static_cast<int>(sqrt((point0.y-point1.y)*(point0.y-point1.y)+
                                   (point0.x-point1.x)*(point0.x-point1.x)));
    }
  }

  static int distance(const Segment &v_segment){
    if(v_segment.point0.y==v_segment.point1.y){
      return abs(v_segment.point0.x-v_segment.point1.x);
    }
    else if(v_segment.point0.x==v_segment.point1.x){
      return abs(v_segment.point0.y-v_segment.point1.y);
    }
    else{
      return static_cast<int>(sqrt((v_segment.point0.y-v_segment.point1.y)*(v_segment.point0.y-v_segment.point1.y)+
                                   (v_segment.point0.x-v_segment.point1.x)*(v_segment.point0.x-v_segment.point1.x)));
    }
  }


  /**
     * @brief sortSegmentList 重新排列同一坐标的线段，若线段相互交叉，则融合两线段
     * @param v_list_seg
     * @return
     */
  static std::vector<Segment> sortAndCombineSegmentList(
      const std::vector<Segment> &v_list_seg){

    std::vector<Segment> list_sort=sortSegmentList(v_list_seg);
    std::vector<Segment> list_correct;
    for(uint i=0;i<list_sort.size();i++){
      for(uint j=i+1;j<list_sort.size();j++){
        if(isCrossInX(list_sort[i],list_sort[j])){
          int x_large=list_sort[j].point1.x>list_sort[i].point1.x?
                list_sort[j].point1.x:list_sort[i].point1.x;
          list_sort[i]=Segment(list_sort[i].point0,
                               Point(x_large,list_sort[i].point1.y));
          list_sort.erase(list_sort.begin()+j);
          j--;
        }
      }
      list_correct.push_back(list_sort[i]);
    }
    return list_correct;
  }

  static std::vector<Segment> sortSegmentList(
      const std::vector<Segment> &v_list_seg){
    std::vector<Segment> list_sort;
    for(uint i=0;i<v_list_seg.size();i++){
      uint pos_insert=static_cast<uint>(list_sort.size());
      for(uint j=0;j<list_sort.size();j++){
        if(v_list_seg[i].point0.x>list_sort[j].point0.x){
        }
        else{
          pos_insert=j;
          break;
        }
      }
      list_sort.insert(list_sort.begin()+pos_insert,v_list_seg[i]);
    }
    return list_sort;
  }

  /**
     * @brief getLargestRemoved 去除最长的线段后输出
     * @param v_list_seg_rm
     * @return
     */
  static std::vector<Segment> getLargestRemoved(const std::vector<Segment> &v_list_seg){

    std::vector<Segment> list_clone=v_list_seg;
    unsigned long lengthsize=v_list_seg.size();
    if(lengthsize<=1){
      return list_clone;
    }

    long seg_max;
    int distance_max=0;
    for(uint i=0;i<v_list_seg.size();i++){
      int distance_tmp=distance(v_list_seg[i]);
      if(distance_max<distance_tmp){
        distance_max=distance_tmp;
        seg_max=i;
      }
    }

    list_clone.erase(list_clone.begin()+seg_max);
    return list_clone;

  }
  /**
     * @brief calMeanLengthOfListRmLargest
     * @param v_list_seg
     * @return
     */
  static int calMeanLengthOfListRmLargest(const std::vector<Segment> &v_list_seg){

    unsigned long lengthsize=v_list_seg.size();
    if(lengthsize==1){
      return Segment::distance(v_list_seg[0]);
    }
    else if(lengthsize==0){
      return 0;
    }

    long seg_max;
    int distance_max=0;
    long sum_seg=0;
    for(uint i=0;i<v_list_seg.size();i++){
      int distance_tmp=distance(v_list_seg[i]);
      if(distance_max<distance_tmp){
        sum_seg+=distance_max;
        distance_max=distance_tmp;
        seg_max=i;
      }
      else{
        sum_seg+=distance_max;
      }
    }

    return static_cast<int>(static_cast<unsigned long>(sum_seg)/(lengthsize-1));

  }

  static double rateOfNullInSegmentlist(const std::vector<Segment> & v_list_seg){
    int listsize=static_cast<int>(v_list_seg.size());
    int count_null=0;
    for(uint i=0;i<v_list_seg.size();i++){
      if(Segment::isNull(v_list_seg[i])){
        count_null++;
      }
    }
    return count_null*1.0/listsize;

  }


  /**
     * @brief getSegmentListRemoved 假设给定要删除的线段在原线段内，且相互不交错
     * @param v_seg_src
     * @param v_list_seg_rm
     * @return
     */
  static std::vector<Segment> getSegmentListRemoved(const Segment & v_seg_src,
                                                    std::vector<Segment> v_list_seg_rm){

    //        //! test
    //        cout<<__func__<<endl;
    //        cout<<"v_seg_src: "<<v_seg_src.point0<<" "<<v_seg_src.point1<<endl;
    //        cout<<"v_list_seg_rm: "<<v_list_seg_rm.size()<<endl;
    //        for(uint i=0;i<v_list_seg_rm.size();i++){
    //            cout<<"i: ";
    //            v_list_seg_rm[i].print();
    //            cout<<endl;
    //        }


    std::vector<Segment> list_sort=sortAndCombineSegmentList(v_list_seg_rm);

    unsigned long listsize=list_sort.size();
    //!由此可知列表的尺寸
    std::vector<Segment> list_split;
    if(listsize ==0){
      list_split.push_back(v_seg_src);
      return list_split;
    }
    //输入参数需要满足的条件
    assert(listsize>0);
    for(uint i=0;i<listsize;i++){
      assert(list_sort[i].point1.x>list_sort[i].point0.x);
    }
    for(uint i=0;i<listsize-1;i++){
      assert(list_sort[i].point1.x<list_sort[i+1].point0.x);
    }
    assert(list_sort[0].point0.x>=v_seg_src.point0.x);
    assert(list_sort[listsize-1].point1.x<=v_seg_src.point1.x);


    for(uint i=0;i<listsize+1;i++){
      if(i==0){
        if(v_seg_src.point0.x!=list_sort[0].point0.x){
          list_split.push_back(Segment(
                                 v_seg_src.point0,
                                 list_sort[0].point0
                               ));
        }

      }
      else if(i == listsize){
        if(list_sort[listsize-1].point1.x!=v_seg_src.point1.x){
          list_split.push_back(Segment(
                                 list_sort[listsize-1].point1,
                               v_seg_src.point1
                               ));
        }

      }
      else{

        list_split.push_back(Segment(list_sort[i-1].point1,
                             list_sort[i].point0
                             ));
      }
    }
    return list_split;
  }

  static Mat drawSegment(const Mat mat_src,Segment v_segment){
    Mat mat_res=mat_src.clone();
    LinePara line_tmp=LinePara::getLine(v_segment.point0,v_segment.point1);
    std::vector<Point> list_point;
    int start_x;
    int end_x;
    if(v_segment.point0.x<v_segment.point1.x){
      start_x=v_segment.point0.x;
      end_x=v_segment.point1.x;
    }
    else{
      start_x=v_segment.point1.x;
      end_x=v_segment.point0.x;
    }

    for(int x=start_x;x<=end_x;x++){
      list_point.push_back(Point(x,line_tmp.getY(x)));
    }
    drawPointList(list_point,mat_res);
    return mat_res;
  }



  Segment& operator=(const Segment& v_seg){
    if (this != &v_seg){
      this->point0=v_seg.point0;
      this->point1=v_seg.point1;

    }
    return *this;

  }
  /**
         * @brief calHistMat mat_hist为一条横向分布累积曲线，规定size[N*1]，vsegment的宽度在该分布列表上累加
         * @param mat_hist
         * @param vsegment
         */
  static void calHistMatInX(cv::Mat& mat_hist,const Segment & v_segment){

    assert(mat_hist.type()==CV_32S);

    int start_x_tmp;
    int end_x_tmp;
    if(v_segment.point0.x>v_segment.point1.x){
      start_x_tmp=v_segment.point1.x;
      end_x_tmp=v_segment.point0.x;
    }
    else{
      start_x_tmp=v_segment.point0.x;
      end_x_tmp=v_segment.point1.x;
    }
    int* introwptr=mat_hist.ptr<int>(0);

    for(int i=start_x_tmp;i<=end_x_tmp;i++){
      introwptr[i]++;
    }
  }

}Segment;

std::vector<Segment> calSegmentInDistributionMat(const Mat &mat_distribution,
                                                 int v_threshold);

/**
 * @brief getMaxXInDistributionMat 获取分布图片的最大值的横坐标
 * @param mat_distribution   [width*1]
 * @return
 */
int getMaxXInDistributionMat(const Mat &mat_distribution);


/**
 * @brief searchMostBright 以给定的尺寸，在图片中寻找最亮的的区域；如果遇到区域内灰度和相同的情况，取中点灰度值较高的区域
 * @param mat_src
 * @param size_block
 * @return
 */
int searchMostBright(const cv::Mat&mat_src, Size2i size_block,Rect2i& rect_targrt);



/**
 * @brief calGraySum  计算图片中某一区域的灰度值的和.注意和不要超出int范围
 * @param mat_src
 * @param rect_area
 * @return
 *
 */
int calGraySum(const cv::Mat& mat_src, Rect2i rect_area);


/**
 * @brief getXFromStraightLineAtY 根据两点确定的直线，取某一纵坐标对应的横坐标
 * @param point0
 * @param point1
 * @param y
 * @return
 */
int getXFromStraightLineAtY(cv::Point2i point0,cv::Point2i point1,int y);


int  calValueWithProportion(int value_start,int value_end,int interval,int pos );

float  calValueWithProportion(float value_start,float value_end,int interval,int pos );

float  calValueWithCurve(float value_start,float value_end,int interval,int pos );

/**
 * @brief getVectorAtDistance 获取相同距离的向量，这里正方形边上的点距中点的距离是相同的
 * @param num
 * @return
 */
std::vector<Vec2i> getVectorAtDistance(int distance);

bool findNearBrightPoint(const cv::Mat& mat_src, Point2i point_given, Point& point_get);

/**
 * @brief saveFile 以文件形式保存某个字符串
 * @param content
 * @param file_dst
 */
void saveFile(char* content,std::string file_dst) ;

/**
 * @brief getStringFromFile 以字符串方式读出某个文件
 * @param dirpath
 * @param content
 * @return
 */
int getStringFromFile(std::string dirpath,char** content);

/**
 * @brief getRectInMat 根据已有的区域和图片尺寸计算出与图片相交部分的区域
 * @param size_mat_src
 * @param v_rect
 * @return
 */
int getRectInMat(const Size& size_mat,const Rect & rect_src,Rect & rect_target);

/**
 * @brief isRectInMat 判断某一区域是否在图片尺寸内
 * @return
 */
bool isRectInMat(const Size size_mat,const Rect& rect_block);

/**
 * @brief getRectInMat 计算某区域在图片中的部分
 * @param size_mat
 * @param rect_block
 * @return
 */
Rect getRectInMat(const Size size_mat,const Rect& rect_block);


int drawRectToImage(cv::Mat &mat,cv::Rect2i rect,cv::Scalar v_scalar);

/**
 * @brief calLineByHough 根据霍夫变化确定穿过点最密集的区域的直线
 * @param list_point
 * @return
 */
LinePara calLineByHough(const std::vector<Point2i> list_point);

/**
 * @brief calLineByHoughWithComfirmedPoints 已知确定的若干点，计算出这些点趋势的直线
 *  注意：点列中不能有横坐标为0的两个点
 * @param list_point
 * @return
 */
LinePara calLineByHoughWithComfirmedPoints(const std::vector<Point2i> list_point);



int getDistanceFromPoint(Point point_mid,Point point_edge);

/**
 * @brief calMeanOfPointInMat 计算图中某点一定范围内的点的平均值
 * @param point_mid
 * @param mat_src
 * @param range
 * @return
 */
int calMeanOfPointInMat(const Point& point_mid,const Mat &mat_src,int range);

/**
 * @brief isPointInMat 判断点是否位于图片中
 * @param point
 * @param mat_src
 * @return
 */
bool isPointInMat(const Point& point,const Mat &mat_src);

//std::vector<Vec2i> getVectorInCertainDirection(Vec2i vec_padding );

/**
 * @brief getPaddingListAlongLowGray 以给定的点作为起点，寻找一条沿着最暗处延伸的点阵,直至返回原点，或者参考方向上的点的数量低于一定值
 * @param mat_src
 * @param point_start
 * @return
 */
std::vector<Point> getPaddingListAlongLowGray(const Mat &mat_src,Point point_start,int v_range);


/**
 * @brief smoothPointListWithMedian ；利用中值对点列进行平滑处理，去除异常点
 * @param list_point
 * @param v_offset
 * @return
 */
std::vector<Point> smoothPointListWithMedian(const std::vector<Point> & list_point,
                                             int v_offset_x,
                                             int v_offset_y,
                                             int v_length);

/**
 * @brief smoothPointListWithMedian 利用平均值对点列进行平滑化处理
 * @param list_point
 * @return
 */
std::vector<Point>  smoothPointListWithMean(const std::vector<Point> & list_point, int v_length);




/**
 * @brief estimatePointWithComfirmPoints 非通用函数，仅用于smoothPointListWithMedian，已知后侧的一些点，求某一纵坐标的点
 *  离该点的距离越远，对该值的影响越小
 * @param list_point_comfirm
 * @return
 */
Point estimatePointWithComfirmPoints(const std::vector<Point> &list_point_comfirm,int point_y);


Point calPointInLineGivenY(Point point_0,Point point_1,int point_y);


bool isPointListInRect(const vector<Point> &v_list_point,const Rect v_rect);

Point GetPoint();


/**
 * @brief predictNextPointInList 根据已有点计算下一点的向量
 * @param v_list_point    参考的向量的列表，用point保存向量
 * @param v_count_refer   参考的点的数量
 * @param v_offset        当向量的模值大于该值时，该向量被过滤
 * @return
 */
Point predictNextPointInList(const std::vector<Point> v_list_point,
                             int v_count_refer,int v_offset);

bool isFileExist(const string &path_fire);



template<typename T>
inline
T getMidValue(const vector<T> &l_ele){
  //  assert(l_ele.size()>0);
  vector<T> l_tmp=l_ele;
  sort(l_tmp.begin(),l_tmp.end());
  return l_tmp[l_tmp.size()/2];
}


inline
long getTimeNs()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME,&ts);
  return ts.tv_sec*1000000000+ts.tv_nsec;
}


inline
int getRandomNum(){

  srand(static_cast<uint>(getTimeNs()));
  return rand() ;
}



inline
string generateRandomString(uint v_num,bool b_number=false,bool b_small=true,bool b_big=true ){
  string str_char;
  if (b_number) {
    str_char+="1234567890";
  }
  if (b_small) {
    str_char+="abcdefghijklmnopqrstuvwxyz";
  }
  if (b_big) {
    str_char+="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  }

  string str_ret;

  for (uint i=0;i<v_num;i++) {
    str_ret+=str_char[static_cast<uint>(getRandomNum()%str_char.size())];
  }

  return str_ret;

}


/// 用于跟踪一段代码的生命周期
class DebugHelp{
public:
  DebugHelp(const string &v_str):
    __info(v_str)
  {
    __time_record=getTimeNs();
    cout<<__info<<" ---start"<<endl;

  }
  ~DebugHelp(){
    long timecost=getTimeNs()-__time_record;
    cout<<__info<<" ---end timecost:"<<timecost<<endl;
  }



#define LIFETIMETRACE(x) basic::DebugHelp  DebugHelp_tmp(x);

private:
  string __info;
  long __time_record;


};

}



#endif // BASIbasicC_H
