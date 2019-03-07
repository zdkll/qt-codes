/********************************************************************
created:2018.10.30
author: juntang.xie
purpose:派生自QStyledItemDelegate 自定义资金异动表格样式代理
*********************************************************************/
#ifndef FundChangeDelegate_H
#define FundChangeDelegate_H
#include <QStyledItemDelegate>
#include "fundchangestyledwidget.h"
#include "fundchangemodel.h"


class FundChangeDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FundChangeDelegate(QObject *parent = nullptr);

private:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;


    QColor textColor(int type) const;

private:
    FundChangeStyledWidget m_styledWidget;
};

#endif // FundChangeDelegate_H
