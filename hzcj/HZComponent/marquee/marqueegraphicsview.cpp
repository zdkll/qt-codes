#include "marqueegraphicsview.h"
#include <QPainter>
#include <QStyleOption>
#include "marqueeitem.h"
#include "marqueepanel.h"

MarqueeGraphicsView::MarqueeGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene,parent),
    m_panel((MarqueePanel*)parent)
{
}

void MarqueeGraphicsView::clickItem(const QString &id)
{
    m_panel->onMarqueeClick(id);
}

void MarqueeGraphicsView::enterEvent(QEvent *event)
{
    MarqueeItem::stopScroll(true);
    QGraphicsView::enterEvent(event);
}

void MarqueeGraphicsView::leaveEvent(QEvent *event)
{
    MarqueeItem::stopScroll(false);
    QGraphicsView::leaveEvent(event);
}

//void MarqueeGraphicsView::paintEvent(QPaintEvent *e)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
//    QGraphicsView::paintEvent(e);
//}

