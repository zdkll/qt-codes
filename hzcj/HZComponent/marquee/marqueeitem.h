#ifndef MARQUEEITEM_H
#define MARQUEEITEM_H
/********************************************************************
created:
author: min.cheng
purpose: 跑马灯中的每个子项
*********************************************************************/
#include <QGraphicsItem>
#include "marqueegraphicsview.h"
class MarqueeItem : public QGraphicsItem
{
public:
    MarqueeItem(const MarqueeDataItem& info,MarqueeGraphicsView* view,QGraphicsItem *parent = Q_NULLPTR);

    //设置跑马灯文本
    void setText(const QString& strText);

    //获取文本
    QString getText();

    //获取当前项宽度 不包括项于项之间的间隔
    int getWidth();

    //设置项目之间间隔
    void setElemInterval(int value);

    //获取项的区域
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    QPainterPath shape() const Q_DECL_OVERRIDE;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) Q_DECL_OVERRIDE;

    //记录所有项的尾部
    static void resetTailItem();

    //是否滚动标记
    static void stopScroll(bool bScroll);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
protected:
    void advance(int step) Q_DECL_OVERRIDE;
private:
    static MarqueeItem * gTailItem;
    static bool gStopScroll;
    MarqueeGraphicsView* m_view;
    MarqueeDataItem m_info;
    QString m_text;
    int m_width;
    int m_interval;
    bool m_bEntered;
    bool m_mouseHover;
};

#endif // MARQUEEITEM_H
