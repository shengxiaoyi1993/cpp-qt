#ifndef UARTCONVERT_H
#define UARTCONVERT_H

#include <QString>
#include "../../../common_component/ds_def/tcs_ds_def.h"





namespace uart_convert {


/// 根据ke户端传输的点
string string_readReg(unsigned int v_addr, unsigned char &v_crc);

string string_writeReg(unsigned int v_addr, int v_x,
                         int v_y,unsigned char& v_crc );

int ret_writeResult(const QByteArray &v_data,unsigned char& v_crc);

int ret_readResult(const QByteArray &v_data, unsigned char& v_crc,
                   int &v_x, int& v_y);

/// judge if point after move is in map
/// \brief isInRange
/// \param v_data
/// \return
///
bool isInRange(const ns_tcs_ds_def::PointAndPad &v_data);


/// 将v_src字符串中若干word进行高低位转换
/// \brief reverseWord
/// \param v_src
/// \param v_dst         targrt string
/// \param v_start       start pos must in string
/// \param v_length      end pos must in string
/// \return              if succeed return 0
///
int reverseWord(string v_dst,const string &v_src,int v_start,int v_length );

/// 计算CRC
/// \brief calCRC
/// \param v_crc
/// \param v_src
/// \param v_start
/// \param v_length
/// \return           if succeed return 0
///
int calCRC(unsigned char& v_crc,const string &v_src,int v_start,int v_length);

///  根据一定的规则，convert point from client to addr defined in device
/// \brief addrFromPoint
/// \param v_point
/// \param v_addr
/// \return  if succeed return 0
///
int addrFromPoint(const ns_tcs_ds_def::Point & v_point ,unsigned int &v_addr);

unsigned char hex_to_char(char *src) ;

///
/// \brief charToValue generate string_dst from string_src,two char to on char
/// \param v_dst
/// \param v_src
/// \return
///
int charToValue(string &v_dst,const string &v_src);


}

#endif // UARTCONVERT_H
