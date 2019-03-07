#include "buysellfivedelegate.h"
#include <QPainter>
#include "datacontroller.h"
#include "buysellfivemodel.h"

BuySellFiveDelegate::BuySellFiveDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void BuySellFiveDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //去掉Focus
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    DATABUYSELL data = index.data(MyDataRole).value<DATABUYSELL>();

    QRect rc(option.rect);

    rc.adjust(3, 2, 0, 0);
    rc.setWidth(29);
    rc.setHeight(15);
    if(0 == data.nType)
    {
        painter->setPen(m_styledWidget.riseColor());
        painter->setBrush(m_styledWidget.riseColor());

    }
    else
    {
        painter->setPen(m_styledWidget.downColor());
        painter->setBrush(m_styledWidget.downColor());
    }
    //画圆角矩形
    painter->drawRoundedRect(rc, 2, 2);

    rc.adjust(2, 1, 0, 0);
    QFont font;
    font.setFamily("SimSun");
    font.setPixelSize(12);
    painter->setFont(font);
    painter->setPen(QColor("#ffffff"));
    //画买卖
    painter->drawText(rc,  data.strTitle);


    font.setFamily("Arial");
    font.setPixelSize(13);
    painter->setFont(font);
    if(0 == data.nType)
    {
        painter->setPen(m_styledWidget.risePriceColor());
    }
    else
    {
        painter->setPen(m_styledWidget.downPriceColor());
    }

    QRect rect(option.rect);

    rect.adjust(36, 2, 0, 0);
    //画价格
    painter->drawText(rect, Qt::AlignCenter,data.strPrice);

    rect.adjust(55, 0,-5,0);
    font.setFamily("Arial");
    font.setPixelSize(12);
    painter->setFont(font);
    painter->setPen(m_styledWidget.volumeColor());
    //画数量
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter,data.strVolume);
}
