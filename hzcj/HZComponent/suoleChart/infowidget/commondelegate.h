/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自QStyledItemDelegate 自定义代理去除表格焦点
*********************************************************************/
#ifndef COMMONDELEGATE_H
#define COMMONDELEGATE_H
#include <QStyledItemDelegate>


class CommonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CommonDelegate(QObject *parent = nullptr);
    ~CommonDelegate();


private:

    void paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COMMONDELEGATE_H
