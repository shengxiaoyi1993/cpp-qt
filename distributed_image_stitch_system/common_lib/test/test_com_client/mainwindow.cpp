#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../sskj_lib/com_client/include/com_client_export.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  InitHTTPEnv();
  ui->setupUi(this);
  char output_type[32];
  unsigned char *output=nullptr;
  int buffer_len;
  SendHTTPRequestToPort("192.168.1.126","hello",output_type,&output,&buffer_len,40400);
  cout<<"output_type:"<<output_type<<endl;
  cout<<"buffer_len:"<<buffer_len<<endl;
  printf("output_type:%s\n",output_type);

//  ofstream o_strewam("tmp.jpg");

//  string tmpdate=(char*)output;

//  o_strewam<<tmpdate;
  FILE *fp=fopen("tmp.jpg","w");
  if (fp!=NULL) {
   fwrite(output, buffer_len, 1, fp );
  }

}

MainWindow::~MainWindow()
{
  delete ui;
  DisableHTTPEnv();
}

