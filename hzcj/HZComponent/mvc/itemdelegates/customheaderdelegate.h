/********************************************************************
created:2019/02/18
author: dk.zhang
purpose:自定义表头样式基类
*********************************************************************/
#ifndef CUSTOMHEADERDELEGATE_H
#define CUSTOMHEADERDELEGATE_H

#include <QStyledItemDelegate>

class CustomHeaderDelegate : public QStyledItemDelegate
{
public:
    CustomHeaderDelegate(QObject *parent = 0);
    //设置字体
    void setFont(const QFont &font){m_font = font;}
    //设置颜色
    void setColor(const QColor &color){m_fontColor = color;}
    //设置排序升序降序箭头图片
    void setPixmap(const QPixmap &upPix,const QPixmap &downPix){
        m_upPixmap = upPix;m_downPixmap = downPix;
    }

protected:
    QRect adjustedRect(const Qt::Alignment &alignment,const QRect &rect) const
    {
        return (alignment & Qt::AlignRight)?rect.adjusted(0,0,-m_rightMargin,0):
                                            (alignment & Qt::AlignLeft)?rect.adjusted(m_leftMargin,0,0,0):rect;
    }

    int                   m_sortIndex;
    Qt::SortOrder  m_sortOrder;

    QFont         m_font;
    QColor        m_fontColor;
    QPixmap     m_upPixmap,m_downPixmap;
    int               m_leftMargin   = 10;
    int               m_rightMargin = 10;
};

//资金自定义表头样式
class FundHeaderDelegate : public CustomHeaderDelegate
{
public:
    FundHeaderDelegate(QObject *parent = 0);

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

//相似K线自定义表头样式
class SimiKHeaderDelegate : public CustomHeaderDelegate
{
public:
    SimiKHeaderDelegate(QObject *parent = 0)
        :CustomHeaderDelegate(parent){}

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


#endif // CUSTOMHEADERDELEGATE_H
