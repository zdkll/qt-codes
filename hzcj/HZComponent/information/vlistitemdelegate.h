/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QAbstractItemDelegate>

class HZVListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HZVListItemDelegate(QObject *parent = 0);
    ~HZVListItemDelegate(){}

    /**
     * @brief setWidth 设置宽度
     * @param w
     */
    void setWidth(int w);

protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    mutable int m_width;
};

#endif // ITEMDELEGATE_H
