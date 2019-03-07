/********************************************************************
created:2018.9
author: juntang.xie
purpose: 买卖五档代理类
*********************************************************************/
#ifndef BUYSELLFIVEDELEGATE_H
#define BUYSELLFIVEDELEGATE_H
#include <QStyledItemDelegate>
#include "handicapstyledwidget.h"
#pragma execution_character_set("utf-8")

class BuySellFiveDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BuySellFiveDelegate(QObject *parent = nullptr);


private:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

private:
    HandicapStyledWidget m_styledWidget;
};

#endif // BUYSELLFIVEDELEGATE_H
