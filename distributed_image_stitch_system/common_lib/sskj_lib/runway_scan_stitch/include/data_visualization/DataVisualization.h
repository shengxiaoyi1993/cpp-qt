#ifndef DATAVISUALIZATION_H
#define DATAVISUALIZATION_H

#include <iostream>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
using namespace std;
using namespace cv;

namespace datavisualization{

inline Mat getBasicBlock(Size size, Scalar scalar){
    assert(size.width>0&&size.height>0);
    Mat mat= Mat(size, CV_8UC3, scalar);
    for(int r=0;r<size.height;r++){
        mat.at<Vec3b>(r,0)=Vec3b(255,0,0);
        mat.at<Vec3b>(r,size.width-1)=Vec3b(255,0,0);
    }

    for(int c=0;c<size.width;c++){
        mat.at<Vec3b>(0,c)=Vec3b(255,0,0);
        mat.at<Vec3b>(size.height-1,c)=Vec3b(255,0,0);
    }
    return mat;
}

inline void setMatEdge(cv::Mat& mat_src,Scalar scalar){
    Vec3b vec3b_edge=Vec3b(
                static_cast<uchar>(scalar[0]),
            static_cast<uchar>(scalar[1]),
            static_cast<uchar>(scalar[2])

            );
    for(int r=0;r<mat_src.rows;r++){
        mat_src.at<Vec3b>(r,0)=vec3b_edge;
        mat_src.at<Vec3b>(r,mat_src.cols-1)=vec3b_edge;
    }

    for(int c=0;c<mat_src.cols;c++){
        mat_src.at<Vec3b>(0,c)=vec3b_edge;
        mat_src.at<Vec3b>(mat_src.rows-1,c)=vec3b_edge;
    }
}

/**
  * @brief getScalar 根据比例获取颜色值
  * @param scale 要求在0~1之间
  * @return
  */
inline Scalar getScalar(float scale){
    assert(scale>=0&&scale<=1);
    int sum=256*256*256;
    int value=static_cast<int>(scale*sum);
    int num0=value/(256*256);
    int remain0=value%(256*256);
    int num1=remain0/256;
    int num2=remain0%256;
    return Scalar(num0,num1,num2);
}


inline  bool isPointInPointList(const std::vector<Point2i> &list,
                                const Point2i v_point){
    for(uint i=0;i<list.size();i++){
        if(v_point==list[i]){
            return true;
        }
    }
    return false;
}

//    datavisualization::VisualBasic  block=datavisualization::VisualBasic(1,1,20,20);
//    vector<Point2i> list_error;
//    list_error.push_back(Point2i(0,0));
//    block.setErrorBlock(list_error);
//    Mat mat=block.getMatBlock();


typedef struct VisualBasic{
    int cols;
    int rows;
    int width_block;
    int height_block;

    std::vector<Point2i> list_block;

    VisualBasic(){
        cols=0;
        rows=0;
        width_block=0;
        height_block=0;
    }

    void setErrorBlock(std::vector<Point2i> list_errorblock){
        list_block=list_errorblock;
    }

    VisualBasic(int v_cols,int v_rows,int v_w,int v_h){
        cols=v_cols;
        rows=v_rows;
        width_block=v_w;
        height_block=v_h;
    }

    cv::Mat getMatBlock(){
        Mat mat_res;
        if(cols<=0||rows<=0||width_block<=0||height_block<=0){
            assert(0);
        }

        for(int r=0;r<rows;r++){
            Mat mat_crosswise_tmp;
            for(int c=0;c<cols;c++){
                Mat mat_tmp;
                if(isPointInPointList(list_block,Point2i(c,r))){
                    mat_tmp= getBasicBlock(
                                Size(width_block,height_block),
                                Scalar(0,0,0)
                                );
                }
                else{
                    mat_tmp= getBasicBlock(
                                Size(width_block,height_block),
                                getScalar(static_cast<float>((r*cols*1.0+c+1)/(cols*rows)))
                                );
                }

                if(mat_crosswise_tmp.rows==0){
                    mat_crosswise_tmp=mat_tmp;
                }
                else{
                    hconcat(mat_crosswise_tmp,mat_tmp,mat_crosswise_tmp);
                }

            }
            if(mat_res.rows==0){
                mat_res=mat_crosswise_tmp;
            }
            else{
                vconcat(mat_res,mat_crosswise_tmp,mat_res);
            }
        }
        setMatEdge(mat_res,Scalar(0,0,0));
        return mat_res;
    }

} VisualBasic;


typedef struct VisualField{
    uint cols;
    uint rows;
    std::vector<std::vector<VisualBasic>> list;
    VisualField(){
        cols=0;
        rows=0;
    }
    VisualField(uint v_cols,uint v_rows){
        cols=v_cols;
        rows=v_rows;
        list.resize(cols);
        for(uint i=0;i<static_cast<uint>(cols);i++){
            list[i].resize(rows);
        }
        cout<<"list.size:"<<list.size()<<endl;
        for(uint i=0;i<list.size();i++){
            cout<<"i: "<<i<<" "<<list[i].size()<<endl;
        }
    }

    cv::Mat getMatField(){
        cv::Mat mat_res;
        for(uint c=0;c<list.size();c++){
            Mat mat_lengthwise_tmp;
            for(uint r=0;r<list[c].size();r++){
                Mat mat_tmp=list[c][r].getMatBlock();
                if(mat_lengthwise_tmp.rows==0){
                    mat_lengthwise_tmp=mat_tmp;
                }
                else{
                    vconcat(mat_lengthwise_tmp,mat_tmp,mat_lengthwise_tmp);
                }
            }
            if(mat_res.rows==0){
                mat_res=mat_lengthwise_tmp;
            }
            else if(mat_res.rows==mat_lengthwise_tmp.rows){
                hconcat(mat_res,mat_lengthwise_tmp,mat_res);
            }
            else{
                assert(0);
            }
        }
        return mat_res;
    }


} VisualField;







}


#endif
