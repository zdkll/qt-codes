#include "marqueeitem.h"
#include <QPainter>
#include <QGraphicsScene>
#include <qfontmetrics.h>
#include <qdebug.h>
#include <QList>
#include <QGraphicsView>
#include <QEvent>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include "marqueegraphicsview.h"

MarqueeItem* MarqueeItem::gTailItem = nullptr;
bool MarqueeItem::gStopScroll = false;
MarqueeItem::MarqueeItem(const MarqueeDataItem& info,MarqueeGraphicsView* view,QGraphicsItem *parent):
    QGraphicsItem(parent),
    m_info(info),
    m_view(view)
{

    setText(info.title);
    setAcceptHoverEvents(true);
    m_bEntered = false;
    m_mouseHover = false;
}

void MarqueeItem::setText(const QString &strText)
{
    m_text = strText;
    if(strText.length()> 29)
    {
        m_text = strText.mid(0,30);
        m_text += "...";
    }
    QFont font;
    font.setFamily("宋体");
    font.setPixelSize(12);
    QFontMetrics fm(font);
    m_width = fm.width(m_text);
}

QString MarqueeItem::getText()
{
   return m_text;
}

int MarqueeItem::getWidth()
{
    return m_width;
}

void MarqueeItem::setElemInterval(int value)
{
    m_interval = value;
}

QRectF MarqueeItem::boundingRect() const
{
    qreal adjust =0;
    return QRectF(-m_width/2 - adjust, -12 - adjust,
                  m_width + adjust, 12 + adjust);
}

QPainterPath MarqueeItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void MarqueeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_view && m_text.isEmpty() == false)
    {
        QRectF rc = boundingRect();
        if(!rc.isNull())
        {
            QFont font;
            font.setFamily(m_view->getFontFamily());
            font.setPixelSize(m_view->getFontSize());
            font.setStyleStrategy(QFont::NoAntialias);
            QPen pen;
            if(m_mouseHover)
                pen.setColor(m_view->getSelectedTextClr());
            else
                pen.setColor(m_view->getNormalTextClr());
            painter->setPen(pen);
            painter->setFont(font);
            painter->drawText(rc,Qt::AlignLeft|Qt::AlignVCenter,m_text);
        }
    }
}

void MarqueeItem::resetTailItem()
{
    gTailItem = nullptr;
}

void MarqueeItem::stopScroll(bool bScroll)
{
    gStopScroll = bScroll;
}

void MarqueeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_view->setCursor(Qt::PointingHandCursor);
    stopScroll(true);
    m_mouseHover = true;
    update();
}

void MarqueeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_view->setCursor(Qt::ArrowCursor);
    stopScroll(false);
    m_mouseHover = false;
    update();
}

void MarqueeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_view->clickItem(m_info.id);
    qDebug() << Q_FUNC_INFO << event->type();
}


void MarqueeItem::advance(int step)
{
    if (!step)
        return;
    QList<QGraphicsView *> views = scene()->views();
    QGraphicsView * pView = views[views.size()-1];
    QList<QGraphicsItem *>  viewPortItems = pView->items(pView->rect());
    QList<QGraphicsItem *> items = scene()->items();
    if(!gTailItem)
    {
        gTailItem = (MarqueeItem *)items[items.size()-1];
    }
    if(!gStopScroll)
    {
        if(viewPortItems.indexOf(this) == -1 && m_bEntered)
        {
            //表示刚刚出视口
            this->setPos(gTailItem->pos().rx()+m_width+m_interval,0);
            gTailItem = this;
        }else
        {
            setPos(mapToParent(-1,0));
        }
    }
    if(viewPortItems.indexOf(this) != -1) //进入视口
    {
        m_bEntered = true;
    }else
    {
        m_bEntered = false;
    }
}
