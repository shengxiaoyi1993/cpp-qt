#include "uartconvert.h"
#include <vector>
#include "directcam_reg_def.h"
using namespace std;


namespace uart_convert {





//通讯规范： 读取寄存器X"0C10", 读取数据X"00120013"
// FA  19  79  02  01  10  0C   total 7
string string_readReg(unsigned int v_addr, unsigned char &v_crc){
    char l_ret[7];
    l_ret[0]=0xFA;
    l_ret[1]=0x19;
    l_ret[2]=0x79;

    l_ret[3]=0x02;
    l_ret[4]=0x01;

    l_ret[6]=(char)(v_addr>>8);
    l_ret[5]=(char)(v_addr);

    string ret;
    ret.append(l_ret,7);

//    std::cout<<"ret:"<<ret<<endl;

    if (calCRC(v_crc,ret,4,3) == 0) {

        return ret;
    }

    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"
                          +string(__FUNCTION__)+":Error!");

}
// 写入寄存器X"0C10", 数据X"00120013"
// FA  19  79+  06  00+  10  0C + 13  00  12  00   共11
string string_writeReg(unsigned int v_addr, int v_x,
                       int v_y,unsigned char& v_crc ){

    char l_ret[11];
    l_ret[0]=0xFA;
    l_ret[1]=0x19;
    l_ret[2]=0x79;

    l_ret[3]=0x06;
    l_ret[4]=0x00;

    l_ret[6]=(char)(v_addr>>8);
    l_ret[5]=(char)(v_addr);





    uint tmpx=(uint)v_x;
    tmpx&=0x3ff;

    if (v_x<0) {
        tmpx|=0x200;
    }
    else{
        tmpx&=0x1ff;
    }
    l_ret[8]=(char)(tmpx>>8);
    l_ret[7]=(char)(tmpx);

    uint tmpy=(uint)v_y;
    tmpy&=0x3ff;

    if (v_y<0) {
        tmpy|=0x200;
    }
    else{
        tmpy&=0x1ff;
    }
    l_ret[10]=( char)(tmpy>>8);
    l_ret[9]=( char)(tmpy);



    string ret;
    ret.append(l_ret,11);


    if (calCRC(v_crc,ret,4,7) == 0) {
        return ret;
    }

    throw StringException(string(__FILE__)+":"+to_string(__LINE__)+":"
                          +string(__FUNCTION__)+":Error!");

}

int ret_writeResult(const QByteArray &v_data,unsigned char& v_crc){
    if (v_data.size()>0) {
        v_crc=v_data.data()[0];
        return 0;
    }
    return -1;

}

int ret_readResult(const QByteArray &v_data,unsigned char& v_crc,int &v_x, int& v_y){
    if (v_data.size() >= 5) {
        v_crc=v_data.data()[0];
        uint tmp_x;
        uint tmp_y;
        tmp_y=(uint)(v_data.data()[1])|(uint)(v_data.data()[2]<<8);
        tmp_y<<=(sizeof (uint)*8-10);
        v_y=(int)tmp_y;
        v_y>>=(sizeof (uint)*8-10);

        tmp_x=(uint)(v_data.data()[3])+(uint)(v_data.data()[4]<<8);

        tmp_x<<=(sizeof (uint)*8-10);
        v_x=(int)tmp_x;
        v_x>>=(sizeof (uint)*8-10);

        return 0;
    }
    return -1;
}

int calCRC(unsigned char& v_crc,const string &v_src,int v_start,int v_length){

    if ((int)(v_src.size())>=(v_start+v_length)) {
        unsigned char cmd_crc = 0;
         for (int i=0; i<v_length; i++) {
              cmd_crc += v_src[i+v_start];
         }
         v_crc=cmd_crc;
         return 0;
    }
    cout.flush();
    return -1;

}



/// persume that row and column is correspond between client and device
/// y: row 0~4,x:col 0~8
int addrFromPoint(const ns_tcs_ds_def::Point & v_point ,unsigned int &v_addr){
    const int tr=5;
    const int tc=9;

    uint array_num[tr][tc];
    for (int r=0;r<tr ; r++) {
        for (int c=0;c<tc ; c++) {
            array_num[r][c]=0;
        }
    }

    array_num[0][0]=EDGE_P00;
    array_num[0][1]=EDGE_P10;
    array_num[0][2]=EDGE_P20;
    array_num[0][3]=EDGE_P30;
    array_num[0][4]=EDGE_P40;
    array_num[0][5]=EDGE_P50;
    array_num[0][6]=EDGE_P60;
    array_num[0][7]=EDGE_P70;
    array_num[0][8]=EDGE_P80;
    array_num[1][0]=EDGE_P01;
    array_num[2][0]=EDGE_P02;
    array_num[3][0]=EDGE_P03;
    array_num[4][0]=EDGE_P04;
    array_num[4][1]=EDGE_P14;
    array_num[4][2]=EDGE_P24;
    array_num[4][3]=EDGE_P34;
    array_num[4][4]=EDGE_P44;
    array_num[4][5]=EDGE_P54;
    array_num[4][6]=EDGE_P64;
    array_num[4][7]=EDGE_P74;
    array_num[4][8]=EDGE_P84;
    array_num[1][8]=EDGE_P81;
    array_num[2][8]=EDGE_P82;
    array_num[3][8]=EDGE_P83;

    v_addr=array_num[v_point._y][v_point._x];
    if (v_addr != 0) {
        return 0;
    }
    return -1;


}


bool isInRange(const ns_tcs_ds_def::PointAndPad &v_data){
    const int rows=2160;
    const int cols=3940;
    int x_tmp= cols/8*v_data._point._x+v_data._pad._x;
    int y_tmp=rows/4*v_data._point._y+v_data._pad._y;
//    cout<<"x_tmp:"<<x_tmp<<endl;
//    cout<<"y_tmp:"<<y_tmp<<endl;
    if (x_tmp>=0&&x_tmp<=cols&&y_tmp>=0&&y_tmp<=rows) {
        return true;
    }
    else{
        return false;
    }
}





unsigned char hex_to_char(char *src) {
    //    --{{{
    unsigned char value;

    if (src[1]<='9' && src[1]>='0') {
        value = (int)(src[1]-'0');
    } else if (src[1]<='f' && src[1]>='a') {
        value = (int)(src[1]-'a'+10);
    } else if (src[1]<='F' && src[1]>='A') {
        value = (int)(src[1]-'A'+10);
    } else {
        printf("Hex to Char convert error!!!\n");
    }

    if (src[0]<='9' && src[0]>='0') {
        value = value + 16*(int)(src[0]-'0');
    } else if (src[0]<='f' && src[0]>='a') {
        value = value + 16*(int)(src[0]-'a'+10);
    } else if (src[0]<='F' && src[0]>='A') {
        value = value + 16*(int)(src[0]-'A'+10);
    } else {
        printf("Hex to Char convert error!!!\n");
    }

    return value;
}

}
