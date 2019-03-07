/********************************************************************
created:
author: dk.zhang
purpose:分价表样式代理
*********************************************************************/
#ifndef PRICETICKDELEGATES_H
#define PRICETICKDELEGATES_H

#include <QStyledItemDelegate>
#include "mvcpublicdef.h"
#include  "styleditemdelegate.h"

//第一列 价格
class PriceColDelegate : public HandStyledItemDelegate
{
public:
    PriceColDelegate(QObject *parent = 0)
        :HandStyledItemDelegate(parent){}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

};


//第二列列 成交量占比
class VolumeColDelegate : public HandStyledItemDelegate
{
public:
    VolumeColDelegate(QObject *parent = 0)
        :HandStyledItemDelegate(parent){}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // PRICETICKDELEGATES_H
