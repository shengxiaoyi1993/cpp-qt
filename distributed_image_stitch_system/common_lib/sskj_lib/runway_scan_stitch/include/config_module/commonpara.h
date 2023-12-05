#ifndef COMMONPARA_H
#define COMMONPARA_H
#include <iostream>

namespace commonpara {

typedef struct CommonPara{


 /**
 * @brief __min_angle_rotate：angle of image rotate in direction of anticlockwise,
 * the value is negative
 */
double __min_angle_rotate;


/**
 * @brief __max_angle_rotate: angle of image rotate in direction of clockwise, the value is positive
 */
double __max_angle_rotate;

/**
 * @brief __pi:value of pi
 */
double __pi;

/**
 * @brief __offset_cut:width of edge reserved when cut the image alongside the splitline
 */
int __offset_cut;


/**
 * @brief __range_deformation_x:value of deformation per 1024 pixes in direction of x
 */
int __range_deformation_x;

/**
 * @brief __range_deformation_y:value of deformation per 1024 pixes in direction of y
 */
int __range_deformation_y;

int __min_width_stripeline_widthwise;
int __max_width_stripeline_widthwise;

int __min_width_splitline_widthwise;
int __max_width_splitline_widthwise;

int __max_width_splitline_lengthwise;
int __min_width_splitline_lengthwise;


int __min_distance_splitline_splitline;
int __max_distance_splitline_splitline;

int __min_distance_splitline_stripeline;
int __max_distance_splitline_stripeline;

int __min_distance_stripeline_stripeline;
int __max_distance_stripeline_stripeline;

int __min_overlap_x_left;
int __max_overlap_x_left;

int __min_overlap_x_right;
int __max_overlap_x_right;

int __x_left_ledge_splitline_length;
int __x_left_redge_splitline_length;

int __x_right_ledge_splitline_length;
int __x_right_redge_splitline_length;





CommonPara(){
    __min_angle_rotate=-1.5;
    __max_angle_rotate=1.5;
    __pi=3.1415926;
    __offset_cut=256;
    __range_deformation_x=50;
    __range_deformation_y=100;
    __min_distance_stripeline_stripeline=45;
    __max_distance_stripeline_stripeline=60;
    __min_distance_splitline_splitline=3900;
    __max_distance_splitline_splitline=4500;
    __min_distance_splitline_stripeline=140;
    __max_distance_splitline_stripeline=300;
    __min_width_stripeline_widthwise=8;
    __max_width_stripeline_widthwise=11;
    __min_width_splitline_widthwise=14;
    __max_width_splitline_widthwise=60;

    __min_overlap_x_left=900;
    __max_overlap_x_left=1600;
    __min_overlap_x_right=800;
    __max_overlap_x_right=1400;

    __max_width_splitline_lengthwise=30 ;
    __min_width_splitline_lengthwise=15;

     __x_left_ledge_splitline_length=0;
     __x_left_redge_splitline_length=2000;

    __x_right_ledge_splitline_length=7500;
    __x_right_redge_splitline_length=9000;

}


}CommonPara;

static CommonPara commonpara_global;





}









#endif
