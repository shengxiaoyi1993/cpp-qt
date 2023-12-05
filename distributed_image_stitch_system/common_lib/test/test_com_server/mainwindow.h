
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../sskj_lib/com_server/include/com_server_export.h"
#include <iostream>
#include <QImage>
#include <QPixmap>
#include <fstream>


using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
};

inline
 int cb_CallbackOnNewHttpRequest(long long int fd, char *username, char *req_data,
                                        void *ptr){
   cout<<__func__<<" fd:"<<fd<<endl;
//   QImage image_tmp("/home/sxy/Desktop/0_0_4.jpg");

//   cout<<"width:"<<image_tmp.size().width();
//   cout<<"height:"<<image_tmp.size().height();
//   QPixmap map_tmp;
//   map_tmp.

   printf("req_data:%s\n",req_data);

   string file_image="/home/sxy/Desktop/0_0_4.jpg";
   ifstream ifs(file_image);
   string tmp;

   ifs.seekg(0,std::ios::end);
   int len = ifs.tellg();
   ifs.seekg(0,std::ios::beg);

   char *buff = new char[len];

   ifs.read(buff , len);
   char* type="jpeg";
   cout<<"len:"<<len<<endl;
   SendHttpResponse(fd,type,(unsigned char*)buff,len);
   CloseHttpConnection(fd);

   fflush(stdout);

 }


// 当 HTTP 监听发生错误时的回调

inline
 int cb_CallbackOnHttpError(int error_code, void*ptr){
   cout<<__func__<<" error_code:"<<error_code<<endl;

 }


#endif // MAINWINDOW_H
