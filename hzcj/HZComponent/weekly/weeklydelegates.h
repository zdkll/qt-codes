/********************************************************************
created:2019.1
author: jdxchina
purpose: 自选股周报代理样式
*********************************************************************/
#ifndef WEEKLYDELEGATES_H
#define WEEKLYDELEGATES_H

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QHeaderView>
#include <QTableView>
#include <QPainter>
#include "customheaderdelegate.h"

/**
 * @brief The WeeklyChgDelegate class
 */
class WeeklyChgDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    WeeklyChgDelegate(int type=0, QObject *parent = 0)
        :QStyledItemDelegate(parent),m_type(type){}
    ~WeeklyChgDelegate(){}

    /**
     * @brief changeType 共享类型
     * @return
     */
    int changeType() const { return m_type;}

protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;

private:
    int m_type;
};

class WeeklyChgDelegate2 : public QItemDelegate
{
    Q_OBJECT
public:
    WeeklyChgDelegate2(int type=0, QObject *parent = Q_NULLPTR)
        :QItemDelegate(parent),m_type(type){}
    ~WeeklyChgDelegate2(){}

    /**
     * @brief changeType 共享类型
     * @return
     */
    int changeType() const { return m_type;}

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    int m_type;
};

/**
 * @brief The WeeklyHorizonHeader class 一维水平表头
 */
class WeeklyHorizonHeader : public QHeaderView
{
    Q_OBJECT
public:
    WeeklyHorizonHeader(Qt::Orientation orientate,QWidget *parent = nullptr)
        :QHeaderView(orientate, parent)
    {
        setObjectName("WeeklyHorizonHeader");
         setSectionsClickable(true);
    }

    ~WeeklyHorizonHeader()
    {}

    /**
     * @brief getValidRect 获取绘制区域
     * @param rect
     * @param alignment
     * @return
     */
    QRect getValidRect(const QRect &rect,Qt::Alignment alignment) const;

protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private:
    int m_leftMargin{10};
    int m_rightMargin{8};
};

class WeeklyTabelView : public QTableView
{
    Q_OBJECT
public:
    WeeklyTabelView(QWidget *parent=nullptr):QTableView(parent)
    {

    }

protected:
    void paintEvent(QPaintEvent *e)
    {
//        QTableView::paintEvent(e);
        QPainter p(this);
//        p.setBrush(QColor("#222222"));
        p.fillRect(rect(),QColor("#222222"));
    }
};

#endif // WEEKLYDELEGATES_H
