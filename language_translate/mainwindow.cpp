#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    label=new QLabel(this);
    pushbutton= new QPushButton(this);
    qtrans=new QTranslator();
    label->setText(QObject::tr("hello"));
    label->show();
    QGridLayout* layout_main=new QGridLayout(this);
    layout_main->addWidget(label);
    layout_main->addWidget(pushbutton);

    QWidget* widget_main=new QWidget();
    this->setCentralWidget(widget_main);
    widget_main->setLayout(layout_main);
    pushbutton->setText(tr("language switch"));
    //    label->setText(QObject::tr("hello"));
    connect(pushbutton,SIGNAL(clicked()),
            this,SLOT(on_pushbutton_click()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::changeEvent(QEvent * event){
    if(event->type() == QEvent::LanguageChange) label->setText(QObject::tr("hello"));
    else  QMainWindow::changeEvent(event);
    qDebug()<<"changeEvent conclude"<<endl;
}


void MainWindow::languageChanged(int flag) {

    if(flag ==0){
        if(qtrans->load("../language_translate/chinese.qm")){
            qDebug()<<"load success"<<endl;
        }
        else{
            qDebug()<<"load failed"<<endl;
        }
    }
    else if(flag ==1){
        if(qtrans->load("../language_translate/english.qm")){
            qDebug()<<"load success"<<endl;
        }
        else{
            qDebug()<<"load failed"<<endl;
        }
    }


    QFile tmp("../language_translate/chinese.qm");
    if(tmp.exists()){
        qDebug()<<"chinese.qm exist"<<endl;
    }
}

void MainWindow::on_pushbutton_click(){
    if(_flag_language == 0){
        _flag_language =1;
    }
    else{
        _flag_language =0;
    }
    languageChanged(_flag_language);

}

