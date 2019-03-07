/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自QStyledItemDelegate 自定义公告资讯表格样式代理
*********************************************************************/
#ifndef NEWSDELEGATE_H
#define NEWSDELEGATE_H
#include <QStyledItemDelegate>
#include "newsstyledwidget.h"


class NewsDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NewsDelegate(QObject *parent = nullptr);

private:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

private:
    NewsStyledWidget m_styledWidget;
};

#endif // NEWSDELEGATE_H
