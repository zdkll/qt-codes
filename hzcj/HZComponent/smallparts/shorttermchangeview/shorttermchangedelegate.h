/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自QStyledItemDelegate 自定义短线异动表格样式代理
*********************************************************************/
#ifndef ShortTermChangeDelegate_H
#define ShortTermChangeDelegate_H
#include <QStyledItemDelegate>
#include "ShortTermChangestyledwidget.h"
#include "ShortTermChangemodel.h"


class ShortTermChangeDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ShortTermChangeDelegate(QObject *parent = nullptr);

private:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;


    QColor textColor(int type) const;

private:
    ShortTermChangeStyledWidget m_styledWidget;
};

#endif // ShortTermChangeDelegate_H
