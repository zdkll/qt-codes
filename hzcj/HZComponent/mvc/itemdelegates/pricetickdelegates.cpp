#include "pricetickdelegates.h"

#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QProgressBar>
#include "datamodeldef.h"

///PriceColDelegate----
void PriceColDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    HandStyledItemDelegate::paint(painter,option,index);

    double  price = index.data(Stock_Price_Role).toDouble();
    double pre_close_price = index.data(Stock_Pre_Close_Price_Role).toDouble();

    QString text = Default_Item_String;
    QColor color= m_plainColor;
    if(price != INVALID_DOUBLE)
    {
        text = QString::number(price,'f',2);
        if(price>pre_close_price)
            color = m_riseColor;
        else if(price<pre_close_price)
            color = m_fallColor;
    }
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QApplication::style()->drawItemText (painter, option.rect.adjusted(8,0,0,0) , Qt::AlignLeft|Qt::AlignVCenter,option.palette, true,text);
}


///VolumeColDelegate----
void VolumeColDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    HandStyledItemDelegate::paint(painter,option,index);

    UINT64 max_volume= index.data(Stock_Max_Volume_Role).toULongLong();
    UINT64 total_volume= index.data(Stock_Total_Volume_Role).toULongLong();

    UINT64 volume =  index.data(Stock_Volume_Role).toULongLong();
    UINT64 buy_volume = index.data(Stock_Buy_Volume_Role).toULongLong();
    UINT64 sell_volume = index.data(Stock_Sell_Volume_Role).toULongLong();

    double  sell_ratio = double(sell_volume)/double(volume);
    double  buy_ratio =  double(buy_volume)/double(volume);
    double  max_volume_ratio =  double(volume)/double(max_volume);
    double  total_volume_ratio = 100.f*double(volume)/double(total_volume);

    QRect rect = option.rect.adjusted(m_margin,0,-m_margin,0);
    int width = rect.width();

    width  = std::round(width *max_volume_ratio);
    QPixmap pix(width, rect.height()-4);
    pix.fill(m_riseColor);
    QApplication::style()->drawItemPixmap(painter, rect,Qt::AlignLeft|Qt::AlignVCenter,pix);

    width = std::round(width*sell_ratio);
    QPixmap pix1(width, rect.height()-4);
    pix1.fill(m_fallColor);
    QApplication::style()->drawItemPixmap(painter, rect,Qt::AlignLeft|Qt::AlignVCenter,pix1);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    QString text = QString::number(total_volume_ratio,'f',2)+"%";
    QApplication::style()->drawItemText (painter, rect , Qt::AlignRight|Qt::AlignVCenter,option.palette, true,text);
}
