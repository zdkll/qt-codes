/********************************************************************
created:2018.10.31
author: juntang.xie
purpose:派生自QStyledItemDelegate 自定义龙虎榜买卖表格代理
*********************************************************************/
#ifndef LHBUYSELLDELEGATE_H
#define LHBUYSELLDELEGATE_H
#include <QStyledItemDelegate>

class LhBuySellDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LhBuySellDelegate(QObject *parent = nullptr);

private:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LHBUYSELLDELEGATE_H
