#include "timedealitemdelegates.h"

#include <QPainter>
#include <QApplication>
#include <QDateTime>

#include "datamodeldef.h"

//青色
const QColor cyanColor = QColor("#13ffff");
//紫色
const QColor purpleColor = QColor("#e970d0");

void  AllTickColTimeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    QStyleOptionViewItem  myOption = option;
    myOption.displayAlignment = Qt::AlignCenter;

    int time = index.data(Tick_Data_Role).toInt();

    QString text = (time == INVALID_INT32)?Default_Item_String:QDateTime::fromTime_t(time).toString("HH:mm:ss");

    painter->setPen(m_color);
    painter->setFont(m_font);
    QApplication::style()->drawItemText (painter, myOption.rect , myOption.displayAlignment,option.palette, true,text );
}

///AllTickColPriceDelegate
AllTickColPriceDelegate::AllTickColPriceDelegate(QObject *parent)
    :TickItemDelegate(parent)
{

}

void  AllTickColPriceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    Qt::Alignment alignment = Qt::AlignCenter;

    double price =  index.data(Tick_Data_Role).toDouble();

    QString text = price == INVALID_DOUBLE?Default_Item_String:QString::number(price,'f',2);

    painter->setPen(m_color);

    painter->setFont(m_font);

    QApplication::style()->drawItemText (painter, option.rect , alignment,option.palette, true,text );
}


///AllTickColVolumeDelegate
AllTickColVolumeDelegate::AllTickColVolumeDelegate(QObject *parent)
    :TickItemDelegate(parent)
{

}

void  AllTickColVolumeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    QStyleOptionViewItem  myOption = option;

    myOption.displayAlignment = Qt::AlignVCenter|Qt::AlignRight;

    quint64 volume= index.data(Tick_Data_Role).toULongLong();

    int       direct = index.model()->data(index,Tick_Data_Direct_Role).toInt();

    double price =  index.model()->data(index.sibling(index.row(),1),Tick_Data_Role).toDouble();

    QPixmap pix ;
    QString text  = Default_Item_String;
    QColor color = m_color;
    if(volume != INVALID_UINT64){
        text   = QString::number(double(volume)/100.f,'f',0);
        pix    = (direct == 1)?m_downArrowPix:m_upArrowPix;
        double turnover = price * volume;
        if(turnover<2e5)
        {
            color = (direct == 1)?m_downColor:m_upColor;
        }else{
            color = (direct == 1)?cyanColor:purpleColor;
        }
    }

    painter->setPen(color);
    painter->setFont(m_font);

    QApplication::style()->drawItemText (painter, myOption.rect.adjusted(0,0,-myOption.rect.width()/2,0) , myOption.displayAlignment,option.palette, true,text );

    if(volume != INVALID_UINT64)
        QApplication::style()->drawItemPixmap(painter,  myOption.rect.adjusted(myOption.rect.width()/2,0,0,0),Qt::AlignVCenter|Qt::AlignLeft,pix);
}

///SLTickColTimeDelegate
void  SLTickColTimeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    int time = index.data(Tick_Data_Role).toInt();
    QString text = (time == INVALID_INT32)?Default_Item_String:QDateTime::fromTime_t(time).toString("HH:mm:ss");

    painter->setPen(m_color);
    painter->setFont(m_font);

    QApplication::style()->drawItemText (painter, option.rect.adjusted(8,0,0,0), Qt::AlignVCenter|Qt::AlignLeft,option.palette, true,text );

    painter->setPen(m_gridLineColor);
    painter->drawLine(option.rect.topLeft(),option.rect.topRight());
}

///SLTickColPriceDelegate
SLTickColPriceDelegate::SLTickColPriceDelegate(QObject *parent)
    :TickItemDelegate(parent)
{

}

void  SLTickColPriceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    double price =  index.data(Tick_Data_Role).toDouble();

    QString text = price == INVALID_DOUBLE?Default_Item_String:QString::number(price,'f',2);

    painter->setPen(m_color);
    painter->setFont(m_font);

    QApplication::style()->drawItemText (painter, option.rect.adjusted(8,0,0,0) , Qt::AlignVCenter|Qt::AlignLeft,option.palette, true,text );

    painter->setPen(m_gridLineColor);
    painter->drawLine(option.rect.topLeft(),option.rect.topRight());
}


///SLTickColVolumeDelegate
SLTickColVolumeDelegate::SLTickColVolumeDelegate(QObject *parent )
{

}

void  SLTickColVolumeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    quint64 volume= index.data(Tick_Data_Role).toULongLong();

    int      direct = index.model()->data(index,Tick_Data_Direct_Role).toInt();

    QColor color = (direct == 1)?m_downColor:m_upColor;
    QPixmap pix = (direct == 1)?m_downArrowPix:m_upArrowPix;

    painter->setPen(color);
    painter->setFont(m_font);

    QString text = volume == INVALID_UINT64?Default_Item_String:QString::number(double(volume)/100.f,'f',0);

    QRect textRect = option.rect;
    textRect.adjust(0,0,-18,0);
    QRect pixRect =option.rect;
    pixRect.adjust(option.rect.width()-18,0,-8,0);

    QApplication::style()->drawItemText (painter, textRect , Qt::AlignVCenter|Qt::AlignRight,option.palette, true,text );
    if(volume != INVALID_UINT64)
        QApplication::style()->drawItemPixmap(painter, pixRect,Qt::AlignCenter,pix);

    // 绘制网格线
    painter->setPen(m_gridLineColor);
    painter->drawLine(option.rect.topLeft(),option.rect.topRight());
}


