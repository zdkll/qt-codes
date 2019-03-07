#include "naviitemdelegate.h"

#include <QPainter>
#include <QApplication>
#include "sortfilterproxymodel.h"

QFont  NaviItemDelegate::m_charFont  = QFont("SimSun");
QFont  NaviItemDelegate::m_digitalFont1 = QFont("Arial");
QFont  NaviItemDelegate::m_digitalFont2 = QFont("Arial");
QColor NaviItemDelegate::m_riseColor = QColor("#e62727");
QColor NaviItemDelegate::m_fallColor  = QColor("#0fc351");
QColor NaviItemDelegate::m_plainColor= QColor("#cad4dc");
QColor NaviItemDelegate::m_numberColor= QColor("#adadad");
QColor NaviItemDelegate::m_textColor = QColor("#ffffff");
bool NaviItemDelegate::m_isInitedStyleProperties = false;

///NaviItemDelegate-------
NaviItemDelegate::NaviItemDelegate(QObject *parent)
    :NoFocusStyledItemDelegate(parent)
    ,m_marginLeft(14),m_marginRight(11)
{

}


void NaviItemDelegate::initStyleProperties(const StyleProperties &styledProperties)
{
    m_riseColor = styledProperties.riseColor;
    m_fallColor  = styledProperties.fallColor;
    m_plainColor= styledProperties.plainColor;
    m_numberColor= styledProperties.numberColor;
    m_textColor = styledProperties.textColor;

    m_digitalFont1.setPixelSize(12);
    m_digitalFont1.setBold(true);
    m_digitalFont1.setStyleStrategy(QFont::NoAntialias);
    m_digitalFont2.setPixelSize(12);

    m_isInitedStyleProperties = true;
}

void NaviItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoFocusStyledItemDelegate::paint(painter,option,index);

    QRect rect0 = option.rect.adjusted(m_marginLeft,7,-m_marginRight,-5);

    QString stock_name = index.model()->data(index,Stock_Name_Role).toString();
    QString stock_code = index.model()->data(index,Stock_Code_Role).toString();
    double last_price = index.model()->data(index,Stock_LastPrice_Role).toDouble();
    double up_down_price = index.model()->data(index,Stock_UpDownPrice_Role).toDouble();
    double rate = index.model()->data(index,Stock_Rate_Role).toDouble();

    //绘制股票名称代码
    painter->setPen(m_textColor);
    painter->setFont(m_charFont);
    QApplication::style()->drawItemText (painter, rect0 , Qt::AlignLeft|Qt::AlignTop,option.palette, true,stock_name);
    painter->setPen(m_numberColor);
    painter->setFont(m_digitalFont2);
    QApplication::style()->drawItemText (painter, rect0 , Qt::AlignLeft|Qt::AlignBottom,option.palette, true
                                         ,QString("%1.%2").arg(stock_code.mid(2)).arg(stock_code.left(2).toUpper()));

    //现价
    painter->setFont(m_digitalFont1);
    QColor color= m_plainColor;
    QString text = Default_Item_String;
    if(last_price != INVALID_DOUBLE && last_price != 0)
    {
        text    = QString::number(last_price,'f',2);
        if(up_down_price>0)
            color = m_riseColor;
        else if(up_down_price<0)
            color = m_fallColor;
    }
    painter->setPen(color);
    int up_down_margin = QFontMetrics(m_digitalFont1).width(text);
    QApplication::style()->drawItemText (painter, rect0 , Qt::AlignRight|Qt::AlignTop,option.palette, true,text);

    //涨跌幅
    painter->setFont(m_digitalFont2);
    text = rate == INVALID_DOUBLE?Default_Item_String:QString::number(rate,'f',2)+"%";
    up_down_margin = std::max(up_down_margin,QFontMetrics(m_digitalFont2).width(text));
    QApplication::style()->drawItemText (painter, rect0 , Qt::AlignRight|Qt::AlignBottom,option.palette, true,text);

    //涨跌
    text = up_down_price == INVALID_DOUBLE?Default_Item_String:QString::number(up_down_price,'f',2);
    QApplication::style()->drawItemText (painter, rect0.adjusted(0,0,-up_down_margin-11,0) , Qt::AlignRight|Qt::AlignBottom,option.palette, true,text);
}
