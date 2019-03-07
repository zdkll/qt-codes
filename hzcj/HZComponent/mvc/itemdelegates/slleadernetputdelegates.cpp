#include "slleadernetputdelegates.h"

#include <QPainter>
#include <QDateTime>
#include <QStyle>
#include <QApplication>

#include "mvcpublicdef.h"
#include "delegatefunctions.h"


///SLLeaderNetputTimeDelegate
void SLLeaderNetputTimeDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ColStyledItemDelegate::paint(painter,option,index);

    QRect rect=  ColStyledItemDelegate::adjustRect(option.rect,m_margin,m_alignment);

    painter->setFont(m_digitalFont);
    painter->setPen(m_normalColor);

    uint time_t = index.data(LeaderNetput_Data_Role).toUInt();
    QString text = QDateTime::fromTime_t(time_t).toString("HH:mm:ss");

    QApplication::style()->drawItemText(painter,rect,m_alignment,option.palette,true,text);

    //绘制底部分割线
    painter->setPen(m_gridLineColor);

    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
}


///SLLeaderNetputTurnoverDelegate
void SLLeaderNetputTurnoverDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ColStyledItemDelegate::paint(painter,option,index);

//    QRect rect = ColStyledItemDelegate::adjustRect(option.rect,m_margin,m_alignment);

    uint direct   = index.data(LeaderNetput_Direct_Role).toUInt();
    QString text =  DelegateFunctions::toFormatString(index.data(LeaderNetput_Data_Role).toDouble());

    QColor color = direct == 2?m_bpColorFirst:m_fallColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);
    QApplication::style()->drawItemText(painter,option.rect,m_alignment,option.palette,true,text);

    //绘制底部分割线
    painter->setPen(m_gridLineColor);

    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
}



///SLLeaderNetputVolumeDelegate
void SLLeaderNetputVolumeDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ColStyledItemDelegate::paint(painter,option,index);

//    QRect rect=  ColStyledItemDelegate::adjustRect(option.rect,m_margin,m_alignment);

    uint direct   = index.data(LeaderNetput_Direct_Role).toUInt();
    QString text =  DelegateFunctions::stockToFormatString(index.data(LeaderNetput_Data_Role).toULongLong())+QStringLiteral("手");

    QColor color = direct == 2?m_bpColorFirst:m_fallColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);
    QApplication::style()->drawItemText(painter,option.rect,m_alignment,option.palette,true,text);

    //绘制底部分割线
    painter->setPen(m_gridLineColor);

    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
}


///SLLeaderNetputAverPriceDelegate
void SLLeaderNetputAverPriceDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ColStyledItemDelegate::paint(painter,option,index);

    QRect rect=  ColStyledItemDelegate::adjustRect(option.rect,m_margin,m_alignment);

    uint direct   = index.data(LeaderNetput_Direct_Role).toUInt();
    QString text =  QString::number(index.data(LeaderNetput_Data_Role).toDouble(),'f',2);

    QColor color = direct == 2?m_riseColor:m_fallColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);
    QApplication::style()->drawItemText(painter,rect,m_alignment,option.palette,true,text);

    //绘制底部分割线
    painter->setPen(m_gridLineColor);

    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
}
