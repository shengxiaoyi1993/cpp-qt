#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>


class CloseButton : public QStyledItemDelegate {
    Q_OBJECT
public:

    explicit CloseButton(QObject *parent = 0,
                         const QPixmap &closeIcon = QPixmap())
        : QStyledItemDelegate(parent)
        , m_closeIcon(closeIcon)
    {
        if(m_closeIcon.isNull())
        {
            m_closeIcon = qApp->style()
                ->standardPixmap(QStyle::SP_DialogCloseButton);
        }
    }

    QPoint closeIconPos(const QStyleOptionViewItem &option) const {
        return QPoint(option.rect.right() - m_closeIcon.width() - margin,
                      option.rect.center().y() - m_closeIcon.height()/2);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const {
        QStyledItemDelegate::paint(painter, option, index);
        // Only display the close icon for top level items...
        if(!index.parent().isValid()
                // ...and when the mouse is hovering the item
                // (mouseTracking must be enabled on the view)
                && (option.state & QStyle::State_MouseOver))
        {
            painter->drawPixmap(closeIconPos(option), m_closeIcon);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);

        // Make some room for the close icon
        if(!index.parent().isValid()) {
            size.rwidth() += m_closeIcon.width() + margin * 2;
            size.setHeight(qMax(size.height(),
                                m_closeIcon.height() + margin * 2));
        }
        return size;
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index)
    {
        // Emit a signal when the icon is clicked
        if(!index.parent().isValid() &&
                event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            QRect closeButtonRect = m_closeIcon.rect()
                    .translated(closeIconPos(option));

            if(closeButtonRect.contains(mouseEvent->pos()))
            {
                emit closeIndexClicked(index);
            }
        }
        return false;
    }

signals:
    void closeIndexClicked(const QModelIndex &);
private:
    QPixmap m_closeIcon;
    static const int margin = 2; // pixels to keep arount the icon

    Q_DISABLE_COPY(CloseButton)
};

#endif // CLOSEBUTTON_H
