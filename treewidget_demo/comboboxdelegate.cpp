#include "comboboxdelegate.h"
#include <QDebug>

ComboBoxDelegate::ComboBoxDelegate(){

}



QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const

{

  qDebug()<<__func__<<endl;

  QComboBox *editor = new QComboBox(parent);

  if(index.column() == 2)

  {

    qDebug()<<"index.column() == 2"<<endl;

    editor->addItem(QString::fromLocal8Bit("17"));

    editor->addItem(QString::fromLocal8Bit("18"));

    editor->setCurrentIndex(0);

    return editor;

  }

  else

  {

    return QItemDelegate::createEditor(parent,option,index);

  }

}
