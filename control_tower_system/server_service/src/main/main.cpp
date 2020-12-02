#include <QCoreApplication>
#include <iostream>
#include <QCryptographicHash>
#include "../datamanage/datamanage.h"
#include "../controler/controler.h"
#include "../devproxy/uartconvert.h"
#include "../devproxy/directcam_reg_def.h"



using namespace std;

void testCom();

/// 测试uart传输string_convert
void test_uartconvert();
void test_directCam();

void test_DirectCamOp();

int main(int argc, char *argv[])
{


    QCoreApplication a(argc, argv);
//    test_uartconvert();
//test_DirectCamOp();
//    test_directCam();

          Controler *ctlr;
          try {
            ctlr=new Controler("default.config");

          } catch (std::exception v_ex) {
            cout<<"exception:"<<v_ex.what()<<endl;
          }


    return a.exec();
}

void test_uartconvert(){

    /// test readreg
    unsigned char v_crc;
    string str=uart_convert::string_readReg(EDGE_P00,v_crc);
    for (int i=0;i<str.size() ;i++ ) {
        cout<<hex<<(int)str[i]<<" ";
    }
    cout<<endl;
    cout<<"v_crc:"<<hex<<(int)v_crc<<endl;


    unsigned char v_crc_w;
    string str_w=uart_convert::string_writeReg(EDGE_P00,0x12,0x13,v_crc_w);
    for (int i=0;i<str_w.size() ;i++ ) {
        cout<<hex<<(int)str_w[i]<<" ";
    }
    cout<<endl;
    cout<<"v_crc_w:"<<hex<<(int)v_crc_w<<endl;

}

void test_DirectCamOp(){
    ns_tcs_ds_def::DirectCamOp op("cam",
                               ns_tcs_ds_def::PointAndPad(
                ns_tcs_ds_def::Point(1,0),
                                  ns_tcs_ds_def::Pad(3,4))
                                  );
    cout<<op.jsonobj().ToFormattedString()<<endl;
    neb::CJsonObject root=op.jsonobj();

    cout<<"recv:"<<root.ToFormattedString();
    ns_tcs_ds_def::DirectCamOp data_tmp(root);
    cout<<"data_tmp:"<<data_tmp.jsonobj().ToFormattedString()<<endl;


}


void test_directCam(){
    DirectCam cam;
    cam.setTTY("DIR_CAM","COM11");
   if( cam.connect() == 0){
       cout<<"connect succeed !"<<endl;
//       cam.initCam();
       cout<<"after init"<<endl;
       ns_tcs_ds_def::PointAndPad tmpop(ns_tcs_ds_def::Point(0,4),
                                        ns_tcs_ds_def::Pad(0,0));
       for (int i=-50;i<50;i++ ) {
           tmpop._pad._y=0;
           if(cam.op(tmpop) == 0) {
               cout<<"op succeed"<<endl;
           }
           else{
               cout<<"op fail"<<endl;
           }

       }

//       cam.initCam();


   }
   else{
       cout<<"connect failed !"<<endl;
   }


}
//{
//	"_point":	{
//		"_x":	4,
//		"_y":	0
//	},
//	"_pad":	{
//		"_x":	0,
//		"_y":	1
//	}
//}
//fffffffa 19 79 6 0 20 c 1 0 0 0



//{
//	"_point":	{
//		"_x":	4,
//		"_y":	0
//	},
//	"_pad":	{
//		"_x":	0,
//		"_y":	1
//	}
//}
//fffffffa 19 79 6 0 20 c 1 0 0 0
