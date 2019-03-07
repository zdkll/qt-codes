/********************************************************************
created:
author: dk.zhang
purpose: 资金主力净流入列表样式代理
*********************************************************************/
#ifndef SLLEADERNETPUTDELEGATES_H
#define SLLEADERNETPUTDELEGATES_H

#include "styleditemdelegate.h"


//成交时间 左侧 15px 对齐
class SLLeaderNetputTimeDelegate : public HandStyledItemDelegate
{
public:
    SLLeaderNetputTimeDelegate(QObject *parent = 0)
        :HandStyledItemDelegate(parent){m_margin = 15;m_alignment=Qt::AlignLeft|Qt::AlignVCenter;}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


//成交金额 右侧 15px 对齐
class SLLeaderNetputTurnoverDelegate : public HandStyledItemDelegate
{
public:
    SLLeaderNetputTurnoverDelegate(QObject *parent = 0)
        :HandStyledItemDelegate(parent){m_margin = 15;m_alignment=Qt::AlignRight|Qt::AlignVCenter;}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


//成交量 右侧 15px 对齐
class SLLeaderNetputVolumeDelegate : public HandStyledItemDelegate
{
public:
    SLLeaderNetputVolumeDelegate(QObject *parent = 0)
        :HandStyledItemDelegate(parent){m_margin = 15;m_alignment=Qt::AlignRight|Qt::AlignVCenter;}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


//成交均价 右侧 15px 对齐
class SLLeaderNetputAverPriceDelegate : public HandStyledItemDelegate
{
public:
    SLLeaderNetputAverPriceDelegate(QObject *parent = 0)
        :HandStyledItemDelegate(parent){m_margin = 15;m_alignment=Qt::AlignRight|Qt::AlignVCenter;}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // SLLEADERNETPUTDELEGATES_H
