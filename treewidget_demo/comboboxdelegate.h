#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QComboBox>
#include <QItemDelegate>

class ComboBoxDelegate : public QItemDelegate

{

public:

  ComboBoxDelegate();

public:

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};


#endif // COMBOBOXDELEGATE_H
