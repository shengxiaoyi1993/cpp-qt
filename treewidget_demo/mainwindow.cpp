#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidgetItem>

/// 测试QTreeWidget修改item中显示的控件
///  - 单选框
///  - combox
///  - lineedit
///  - pushbutton

/// 测试了使用委托方式更换item的显示方式和修改方式
///  - 需要将item 设置为editable ,委托才会生效
///  - 具体的委托实现见附带的类中



void brashWidgetItems(const QTreeWidgetItem* p_item,const QColor & v_color){
  int count = p_item->childCount();

  for(int i=0;i<count;i++){
    p_item->child(i)->setBackgroundColor(0,v_color);
    p_item->child(i)->setBackgroundColor(1,v_color);
    brashWidgetItems(p_item->child(i),v_color);
  }
  return;
}


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  __menu_right_click.hide();

  __action_rm.setText(tr("rm"));
  __action_rm.setStatusTip(tr("rm this entry"));
  connect(&__action_rm,SIGNAL(triggered(bool)),
          this,SLOT(slt_on___action_rm_trigger()));


  ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  int count_topitem=ui->treeWidget->topLevelItemCount();
  QList<QColor> list_color={
    QColor(255, 255, 153),
    QColor(255, 204, 153),
    QColor(51, 255, 102),
    QColor(51, 255, 255)
  };

  for (int j=0;j<count_topitem;j++) {

    for (int i=0;i<2;i++) {
      ui->treeWidget->topLevelItem(j)->setBackgroundColor(i,QColor(176, 173, 172));
      brashWidgetItems(ui->treeWidget->topLevelItem(j),list_color[j]);

    }
  }


  // 设置可选框
  ui->treeWidget->topLevelItem(1)->child(0)->setCheckState(1,Qt::CheckState::Unchecked);

  // 设置多选框
  QComboBox * combox_layoutddirect= new QComboBox();
  combox_layoutddirect->insertItems(0,{"LeftToRight","RightToLeft","Auto"});
  ui->treeWidget->setItemWidget(ui->treeWidget->topLevelItem(1)->child(2),
                                1,combox_layoutddirect);


  // 添加一个编辑栏
  QLineEdit * lineedit_test=new QLineEdit();
  ui->treeWidget->setItemWidget(ui->treeWidget->topLevelItem(3)->child(0),
                                1,lineedit_test);


  // 添加一个对话栏
  QPushButton * pushbutton_test=new QPushButton();
  connect(pushbutton_test,SIGNAL(clicked()),&__dialog_test,SLOT(show()));
  //    QTreeWidgetItem * widgetitem_test=new QTreeWidgetItem();
  ui->treeWidget->setItemWidget(ui->treeWidget->topLevelItem(3)->child(1),
                                1,pushbutton_test);

  // 使能
  ui->treeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);


  //  ui->treeWidget->setItemDelegateForColumn(1,new ComboBoxDelegate);
//  ui->treeWidget->setItemDelegateForColumn(0,&__boxdelegate);
      ui->treeWidget_2->setItemDelegateForColumn(2,&__boxdelegate);


  ui->tableWidget->setItemDelegateForColumn( 1, new CloseButton( this ) );


  //设置委托，使用另一空间显示和处理处理数据
  ui->tableWidget->setItemDelegateForColumn(2,new ComboBoxDelegate);
//      ui->tableWidget->setItemDelegateForColumn(2,&__boxdelegate);


  QSizePolicy sp;

}

void MainWindow::slt_on___action_rm_trigger(){
  QMessageBox msgBox;
  msgBox.setText(__string_item+" 触发右键动作");
  msgBox.exec();
}


MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
  QTreeWidgetItem *item_select=ui->treeWidget->itemAt(pos);
  if (item_select != nullptr) {
    item_select->setSelected(true);
    __string_item=item_select->text(0)+":"+item_select->text(1);
  }
  else{
    __string_item="global";
  }


  __menu_right_click.clear();
  __menu_right_click.addAction(&__action_rm);
  __menu_right_click.addAction("action1");
  __menu_right_click.addAction("action2");
  __menu_right_click.addAction("action3");
  QMenu menu0("menu0");
  menu0.addAction("action0");
  menu0.addAction("action1");
  __menu_right_click.addMenu(&menu0);

  __menu_right_click.exec(QCursor::pos());



}
