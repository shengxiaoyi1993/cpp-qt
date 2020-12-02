#include "basecam.h"
#include "directcam_reg_def.h"

int DirectCam::initCam(){
    for (int r=0;r<5 ; r++) {
        for (int c=0;c<9 ; c++) {
            _array_addr[r][c]=0;
            _array_pad[r][c]=ns_tcs_ds_def::Pad(0,0);

        }
    }
    _array_addr[0][0]=EDGE_P00;
    _array_addr[0][1]=EDGE_P10;
    _array_addr[0][2]=EDGE_P20;
    _array_addr[0][3]=EDGE_P30;
    _array_addr[0][4]=EDGE_P40;
    _array_addr[0][5]=EDGE_P50;
    _array_addr[0][6]=EDGE_P60;
    _array_addr[0][7]=EDGE_P70;
    _array_addr[0][8]=EDGE_P80;
    _array_addr[1][0]=EDGE_P01;
    _array_addr[2][0]=EDGE_P02;
    _array_addr[3][0]=EDGE_P03;
    _array_addr[4][0]=EDGE_P04;
    _array_addr[4][1]=EDGE_P14;
    _array_addr[4][2]=EDGE_P24;
    _array_addr[4][3]=EDGE_P34;
    _array_addr[4][4]=EDGE_P44;
    _array_addr[4][5]=EDGE_P54;
    _array_addr[4][6]=EDGE_P64;
    _array_addr[4][7]=EDGE_P74;
    _array_addr[4][8]=EDGE_P84;
    _array_addr[1][8]=EDGE_P81;
    _array_addr[2][8]=EDGE_P82;
    _array_addr[3][8]=EDGE_P83;


    getRegValue();

    for (uint r=0;r<5 ;r++ ) {
        for (uint c=0;c<9 ;c++ ) {
            if (_array_addr[r][c] == 0) {
                continue;
            }
            op(ns_tcs_ds_def::PointAndPad(ns_tcs_ds_def::Point(c,r),
                                          ns_tcs_ds_def::Pad(0,0)));
        }
    }



    return 0;



}

void DirectCam::getRegValue(){
    for (uint r=0;r<5 ;r++ ) {
        for (uint c=0;c<9 ;c++ ) {
            if (_array_addr[r][c] == 0) {
                continue;
            }
            unsigned char crc_tmp;
            unsigned int tmpaddr;
            int flag=uart_convert::addrFromPoint(ns_tcs_ds_def::Point(c,r),tmpaddr);
            if (flag != 0) {
                cout<<"get addr fail"<<endl;
                return ;

            }

            string s_r=uart_convert::string_readReg(tmpaddr ,crc_tmp);

            //            _logstreamm<<"readstr:"<<s_r<<endl;
//            _logstreamm<<"r:"<<r<<" c:"<<c<<endl;

//            _logstreamm<<"write:";
//            for (auto it=s_r.begin();it!=s_r.end() ; it++) {
//                _logstreamm<<hex<<(uint)*it<<" ";
//            }
//            _logstreamm<<endl;

            _serial_port.write(s_r.c_str(),s_r.size());
            _serial_port.waitForReadyRead(100);
            QByteArray data_return=_serial_port.readAll();
            //            _logstreamm<<"return_read:"<<data_return.data()<<endl;

//            _logstreamm<<"return_read:";
//            for (auto it=data_return.begin();it!=data_return.end() ; it++) {
//                _logstreamm<<hex<<(uint)*it<<" ";
//            }
//            _logstreamm<<endl;

            int tmp_x;
            int tmp_y;
            unsigned char crc_return;
            if (uart_convert::ret_readResult(data_return,crc_return,tmp_y,tmp_x) == 0) {
                if (crc_return == crc_tmp) {
                    //                    cout<<"****** read succeed !"<<endl;
                    //                    cout<<"tmp_x:"<<tmp_x<<endl;
                    //                    cout<<"tmp_y:"<<tmp_y<<endl;

                    _array_pad[r][c]=ns_tcs_ds_def::Pad(tmp_x,tmp_y);


                }
                else{
                    cout<<"****** crc_is_wrong"<<endl;
                    return ;

                }
            }
            else{
                cout<<"****** return_is_wrong"<<endl;
                return ;
            }
//            cout<<__func__<<" ----end----"<<endl;

        }
    }

    for (int r=0;r<5 ; r++) {
        for (int c=0;c<9 ; c++) {
            _logstreamm<<dec<<"("<<_array_pad[r][c]._x<<" "<<_array_pad[r][c]._y<<")   ";

        }
        _logstreamm<<endl;
    }
    _logstreamm<<endl;
}


int DirectCam::op_pad(const ns_tcs_ds_def::PointAndPad &v_pad){
    ns_tcs_ds_def::PointAndPad  v_data=v_pad;
    v_data._pad=_array_pad[v_pad._point._y][v_pad._point._x]+v_pad._pad;
    _logstreamm<<"_array_pad:"<<_array_pad[v_pad._point._y][v_pad._point._x].jsonobj().ToFormattedString()<<endl;
    return op(v_data);
}



int DirectCam::op(const ns_tcs_ds_def::PointAndPad &v_data){

    _logstreamm<<"v_data:"<<v_data.jsonobj().ToFormattedString()<<endl;

    if (uart_convert::isInRange(v_data)) {
        unsigned int v_addr;
        if (uart_convert::addrFromPoint(v_data._point,v_addr) == 0) {
            unsigned char crc_tmp;
            string s_w=uart_convert::string_writeReg(
                        v_addr,v_data._pad._x,v_data._pad._y,crc_tmp);
            _logstreamm<<"write:";
            for (auto it=s_w.begin();it!=s_w.end() ; it++) {
                _logstreamm<<hex<<(uint)*it<<" ";
            }
            _logstreamm<<endl;

            _serial_port.write(s_w.c_str(),s_w.size());
            _serial_port.waitForReadyRead(100);
            QByteArray data_return=_serial_port.readAll();
            cout<<"data_return.size():"<<data_return.size()<<endl;
            _logstreamm<<"return:";

            for (auto it=data_return.begin();it!=data_return.end() ; it++) {
                _logstreamm<<hex<<(uint)*it<<" ";
            }
            _logstreamm<<endl;

            unsigned char crc_return;
            if (uart_convert::ret_writeResult(data_return,crc_return) == 0) {
                if (crc_return == crc_tmp) {
                    /// 操作成功
                    /// read reg and check
                    ///
                    string s_r=uart_convert::string_readReg(
                                v_addr ,crc_tmp);

                    _serial_port.write(s_r.c_str(),s_r.size());
                    _serial_port.waitForReadyRead(100);
                    QByteArray data_return=_serial_port.readAll();
                    int tmp_x;
                    int tmp_y;
                    unsigned char crc_return;
                    if (uart_convert::ret_readResult(data_return,crc_return,tmp_y,tmp_x) == 0) {
                        if (crc_return == crc_tmp) {
                            cout<<hex;
                            cout<<"****** read succeed !"<<endl;
                            cout<<"tmp_x:"<<tmp_x<<endl;
                            cout<<"tmp_y:"<<tmp_y<<endl;
                            cout<<"v_data._pad._x:"<<v_data._pad._x<<endl;
                            cout<<"v_data._pad._y:"<<v_data._pad._y<<endl;
                            cout<<dec;

                            /// if input==ouput ,operate succeed
                            if(tmp_x == v_data._pad._x&&tmp_y == v_data._pad._y){
                                getRegValue();

                                return 0;
                            }


                        }
                        else{
                            cout<<"****** crc_is_wrong"<<endl;
                        }

                    }
                    else{
                        cout<<"****** return_is_wrong"<<endl;

                    }
                    return 0;

                }
                else{
                    cout<<"****** crc_is_wrong"<<endl;
                }
                return 0;

            }
            else{
                cout<<"****** return_is_null"<<endl;
            }

        }
        else{
            cout<<"****** can_not_find_addr"<<endl;
        }
    }
    else{
        cout<<"****** out_of_range"<<endl;
    }


    return -1;
}
