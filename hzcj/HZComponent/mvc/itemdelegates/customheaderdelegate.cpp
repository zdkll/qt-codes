#include "customheaderdelegate.h"
#include <QPainter>
#include <QStyle>
#include <QApplication>
#include "mvcpublicdef.h"
#include <QDebug>

CustomHeaderDelegate::CustomHeaderDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}



///FundHeaderDelegate
FundHeaderDelegate::FundHeaderDelegate(QObject *parent)
    :CustomHeaderDelegate(parent)
{

}

void FundHeaderDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    if(option.rect.width()<0)
        return;

    painter->setFont(m_font);
    painter->setPen(m_fontColor);

    QString  text = index.data(Money_Header_Data_Role).toString();

    Qt::Alignment  alignment = (Qt::Alignment)index.data(Money_Header_Alignment_Role).toInt();
    QRect ret_rect =  adjustedRect(alignment,option.rect);

    bool isSortIndex =  index.data(Money_Header_IsSortIndex_Role).toBool();
    if(isSortIndex && (alignment&Qt::AlignRight))
        ret_rect =  ret_rect.adjusted(0,0,-8,0);
    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , alignment,option.palette, true,text );

    //绘制排序箭头
    if(isSortIndex && index.column()>0){
        int sortOrder = index.data(Money_Header_SortOrder_Role).toInt();
        int textWidth = QFontMetrics(m_font).width(text);
        //先实现右对齐排序标识
        if(alignment & Qt::AlignRight)
            ret_rect = QRect(ret_rect.right()+1,ret_rect.top(),7,ret_rect.height());
        else if(alignment & Qt::AlignLeft)
            ret_rect = QRect(ret_rect.left()+textWidth+1,ret_rect.top(),7,ret_rect.height());
        else
            ret_rect  = QRect(ret_rect.center().x()+textWidth/2+1,ret_rect.top(),7,ret_rect.height());

        if(sortOrder == Qt::AscendingOrder)
            QApplication::style()->drawItemPixmap(painter, ret_rect , alignment, m_upPixmap);
        else
            QApplication::style()->drawItemPixmap(painter, ret_rect , alignment, m_downPixmap);
    }
}


///SimiKHeaderDelegate-----
void SimiKHeaderDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    painter->setFont(m_font);
    painter->setPen(m_fontColor);

    if(index.row() == 0 || index.column() == 4){
        QStyleOptionViewItem option1 = option;
        option1.backgroundBrush = QBrush(QColor("#222222"));
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&option1,painter);
    }
    QString  text = index.data(SimK_Header_Data_Role).toString();

    bool isSortIndex =  index.data(SimK_Header_IsSortIndex_Role).toBool();

    Qt::Alignment  alignment = (Qt::Alignment)index.data(SimK_Header_Alignment_Role).toInt();
    QRect ret_rect =  adjustedRect(alignment,option.rect);
    if(isSortIndex && (alignment&Qt::AlignRight))
        ret_rect =  ret_rect.adjusted(0,0,-8,0);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , alignment,option.palette, true,text );
    //绘制排序箭头
    if(isSortIndex && index.column()>0){
        int sortOrder = index.data(SimK_Header_SortOrder_Role).toInt();
        int textWidth = QFontMetrics(m_font).width(text);
        //先实现右对齐排序标识
        if(alignment & Qt::AlignRight)
            ret_rect = QRect(ret_rect.right()+1,ret_rect.top(),7,ret_rect.height());
        else if(alignment & Qt::AlignLeft)
            ret_rect = QRect(ret_rect.left()+textWidth+1,ret_rect.top(),7,ret_rect.height());
        else
            ret_rect  = QRect(ret_rect.center().x()+textWidth/2+1,ret_rect.top(),7,ret_rect.height());

        if(sortOrder == Qt::AscendingOrder)
            QApplication::style()->drawItemPixmap(painter, ret_rect , alignment, m_upPixmap);
        else
            QApplication::style()->drawItemPixmap(painter, ret_rect , alignment, m_downPixmap);
    }
}

