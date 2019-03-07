#include "itemdelegates.h"

#include "../itemmodels/sortfilterproxymodel.h"
#include "delegatefunctions.h"

#include <QPainter>
#include <QStyle>
#include <QApplication>
#include <QDebug>
#include <QFont>

const   QColor    gGreenColor    =    Qt::green;
const   QColor    gRedColor       =    Qt::red;
const   QColor    gWhiteColor    =    Qt::white;

///NoItemDelegate----
void NoItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QString text = QString::number(index.row()+1);
    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    if(ret_rect.width()<1) return;
    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///ZeroNoItemDelegate--
void ZeroNoItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QString text = QString::number(index.row());
    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    if(ret_rect.width()<1) return;
    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///PlainTextDelegate----------
void PlainTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QString text = index.data(Stock_Data_Role).toString();

    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///ULongLongItemDelegate---
void ULongLongItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qulonglong value = index.data(Stock_Data_Role).toULongLong();

    QString text = value ==INVALID_UINT64?Default_Item_String:QString::number(value);

    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}


///PlainIntItemDelegate---
void PlainIntItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    int value = index.data(Stock_Data_Role).toInt();

    QString text = value ==INVALID_INT32?Default_Item_String:QString::number(value);

    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///CodeTextItemDelegate----
void CodeTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    painter->setRenderHint(QPainter::TextAntialiasing, false);
    QString text = index.data(Stock_Data_Role).toString();
    if(!text.startsWith("BK"))
        text = text.mid(2);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///StockCodeTextItemDelegate----
void StockCodeTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    painter->setRenderHint(QPainter::TextAntialiasing, false);
    QString text = index.data(Stock_Data_Role).toString().mid(2);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///BKCodeTextItemDelegate----
void BKCodeTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    painter->setRenderHint(QPainter::TextAntialiasing, false);
    QString text = index.data(Stock_Data_Role).toString();

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///NameTextItemDelegate----
void NameTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QString text = index.data(Stock_Data_Role).toString();

    text = text.isEmpty()?Default_Item_String:text;

    QRect ret_rect =  adjustedRect(Qt::AlignVCenter| Qt::AlignLeft,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_charFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , Qt::AlignVCenter| Qt::AlignLeft,option.palette, true,text );
}

///DateItemDelegate---
void DateItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    long long timet = index.data(Stock_Data_Role).toLongLong();
    QString text = timet == INVALID_INT64?Default_Item_String:QDateTime::fromTime_t(timet).toString("MM-dd");

    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///FullDateItemDelegate
void FullDateItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    long long timet = index.data(Stock_Data_Role).toLongLong();
    QString text = (timet == INVALID_INT64)?Default_Item_String:QDateTime::fromTime_t(timet).toString("yyyy-MM-dd");

    QRect ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///RiseFallItemDelegate--------
void RiseFallItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    //获取涨跌
    double rise_fall  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = Default_Item_String;
    QColor color = m_plainColor;
    if(rise_fall != INVALID_DOUBLE)
    {
        text = QString::number(rise_fall,'f',2);
        rise_fall = text.toDouble();
        if(rise_fall>0)
            color = m_riseColor;
        else if(rise_fall<0)
            color = m_fallColor;
    }

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setFont(m_digitalFont);

    painter->setPen(color);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}


///PercentItemDelegate--------
void PercentItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    //获取涨跌
    double rise_fall  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = Default_Item_String;
    QColor color = m_plainColor;

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setFont(m_digitalFont);

    if(rise_fall != INVALID_DOUBLE)
    {
        text = QString::number(rise_fall,'f',2);
        rise_fall = text.toDouble();
        if(rise_fall>0)
            color = m_riseColor;
        else if(rise_fall<0)
            color = m_fallColor;


        // QFont perFont = m_digitalFont;
        // perFont.setStyleStrategy(QFont::PreferDefault);
        int per_width = QFontMetrics(m_digitalFont).width("%");
        // painter->setFont(perFont);
        painter->setPen(color);

        //绘制"%"
        QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,"%");
        ret_rect.adjust(0,0,-per_width,0);
    }

    painter->setPen(color);

    //绘制数值
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}


///LonglongFormatItemDelegate-----
void LonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = (value == INVALID_INT64)?Default_Item_String:DelegateFunctions::toFormatString(value);

    painter->setPen(m_normalColor);

    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///ULonglongFormatItemDelegate-----
void ULonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    qulonglong value  = index.data(Stock_Data_Role ).toULongLong() ;

    QString text = (value == INVALID_UINT64)?Default_Item_String:DelegateFunctions::toFormatString(value);

    painter->setPen(m_normalColor);

    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///DvalLongLongFormatItemDelegate---
void DvalLongLongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = Default_Item_String;
    QColor  color=m_plainColor;
    if(value != INVALID_INT64)
    {
        text = DelegateFunctions::toFormatString(value);
        if(value>0)
            color = m_riseColor;
        else if(value <0){
            color = m_fallColor;
        }
    }

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(color);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect,m_alignment,option.palette, true,text );

}


///DvalDoubleFormatItemDelegate---
void DvalDoubleFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = Default_Item_String;
    QColor  color=m_plainColor;
    if(value != INVALID_DOUBLE)
    {
        text = DelegateFunctions::toFormatString(value);
        if(value>0)
            color = m_riseColor;
        else if(value <0){
            color = m_fallColor;
        }
    }

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(color);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect,m_alignment,option.palette, true,text );
}


///BSBLongLongFormatItemDelegate---
void BSBLongLongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = Default_Item_String;
    QColor  color=m_plainColor;
    if(value != INVALID_INT64)
    {
        value /=100;
        text = DelegateFunctions::toFormatString(value);
        if(value>0)
            color = m_riseColor;
        else if(value <0){
            color = m_fallColor;
        }
    }

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(color);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect,m_alignment,option.palette, true,text );

}

///RiseDayItemDelegate---
void RiseDayItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    int value  = index.data(Stock_Data_Role ).toInt() ;

    QString text = Default_Item_String;
    QColor  color=m_plainColor;
    if(value != INVALID_INT32)
    {
        text = QString("%1%2").arg(value).arg(QStringLiteral("天"));
        if(value>0){
            color = m_riseColor;
        }else if(value<0)
            color = m_fallColor;
    }
    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    painter->setPen(color);
    painter->setFont(m_digitalFont);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect,m_alignment,option.palette, true,text );
}

///NpLonglongFormatItemDelegate----
void NpLonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = (value == INVALID_INT64)?Default_Item_String:DelegateFunctions::stockToFormatString(value);

    QColor color = (value == INVALID_INT64)?m_plainColor:m_fallColor;
    painter->setPen(color);

    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}

///FallLonglongFormatItemDelegate----
void FallLonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = (value == INVALID_INT64)?Default_Item_String:DelegateFunctions::toFormatString(value);

    QColor color = (value == INVALID_INT64)?m_plainColor:m_fallColor;
    painter->setPen(color);

    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}



///NegFallLonglongFormatItemDelegate----
void NegFallLonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = (value == INVALID_INT64)?Default_Item_String:"-"+DelegateFunctions::toFormatString(value);

    QColor color = (value == INVALID_INT64)?m_plainColor:m_fallColor;
    painter->setPen(color);

    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}



///NegFallDoubleFormatItemDelegate----
void NegFallDoubleFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = (value == INVALID_DOUBLE)?Default_Item_String:"-"+DelegateFunctions::toFormatString(value);

    QColor color = (value == INVALID_DOUBLE)?m_plainColor:m_fallColor;
    painter->setPen(color);

    painter->setFont(m_digitalFont);

    QRect   ret_rect = adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}


///RiseLonglongFormatItemDelegate----
void RiseLonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = (value == INVALID_INT64)?Default_Item_String:DelegateFunctions::toFormatString(value);

    QColor color = (value == INVALID_INT64)?m_plainColor:m_riseColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}

///RiseDoubleFormatItemDelegate----
void RiseDoubleFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = (value == INVALID_DOUBLE)?Default_Item_String:DelegateFunctions::toFormatString(value);

    QColor color = (value == INVALID_DOUBLE)?m_plainColor:m_riseColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}

///FallDoubleFormatItemDelegate
void FallDoubleFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = (value == INVALID_DOUBLE)?Default_Item_String:DelegateFunctions::toFormatString(value);

    QColor color = (value == INVALID_DOUBLE)?m_plainColor:m_fallColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}

///WpLonglongFormatItemDelegate----
void WpLonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role ).toLongLong() ;

    QString text = (value == INVALID_INT64)?Default_Item_String:DelegateFunctions::stockToFormatString(value);

    QColor color = (value == INVALID_INT64)?m_plainColor:m_riseColor;
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}

///NwpRateItemDelegate----
void NwpRateItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);
    double value  = index.data(Stock_Data_Role ).toDouble();

    painter->setFont(m_digitalFont);
    QColor color = m_normalColor;
    QString text =Default_Item_String;
    if(value != INVALID_DOUBLE){
        text = QString::number(value,'f',2);
        if(fabs(value)<10e-2){
            color = m_plainColor;
        }else
            color = m_riseColor;
    }
    painter->setPen(color);
    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///VolULonglongFormatItemDelegate-----
void VolULonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qulonglong value  = index.data(Stock_Data_Role).toULongLong();

    QString text = value == INVALID_UINT64?Default_Item_String: DelegateFunctions::stockToFormatString(value);
    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///VolLonglongFormatItemDelegate-----
void VolLonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qlonglong value  = index.data(Stock_Data_Role).toULongLong();

    QString text = value == INVALID_INT64?Default_Item_String: DelegateFunctions::stockToFormatString(value);
    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///CurULonglongFormatItemDelegate---
void CurULonglongFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    qulonglong value = index.data(Stock_Data_Role).toULongLong();

    const SortFilterProxyModel *model =  static_cast<const SortFilterProxyModel *>(index.model());

    //交易方向
    uint direct = model->colIdxData(index.row(),ColCurVolume).toUInt();
    QString text = Default_Item_String;
    QColor color = m_plainColor;
    if(value != INVALID_UINT64){
        text = DelegateFunctions::stockToFormatString(value);
        if(direct ==2)
            color = m_riseColor;
        else if(direct ==1)
            color = m_fallColor;
    }
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}


///DoublePercentItemDelegate----
void DoubleSimpleItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble() ;

    painter->setFont(m_digitalFont);

    QString text = (value == INVALID_DOUBLE)?(Default_Item_String):QString::number(value,'f',2);

    painter->setPen(m_normalColor);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}

//YuguDoubleSimpleItemDelegate
QPixmap* YuguDoubleSimpleItemDelegate::m_pixmap = 0;
void YuguDoubleSimpleItemDelegate::setPixmap(const QPixmap& pix)
{
    if(m_pixmap) delete m_pixmap;
    m_pixmap = new QPixmap(pix);
}
void YuguDoubleSimpleItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role).toDouble() ;

    bool isYc =false;
    if( value == INVALID_DOUBLE){
        value  = index.data(Yugu_Data_Role).toDouble();
        isYc = (value != INVALID_DOUBLE);
    }
    painter->setFont(m_digitalFont);
    QString text = (value == INVALID_DOUBLE)?(Default_Item_String):QString::number(value,'f',2);
    painter->setPen(m_normalColor);
    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);
    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text);

    //绘制预测标签
    if(isYc){
        int txtWidth = QFontMetrics(m_digitalFont).width(text);
        QRect yc_rect = QRect(ret_rect.right()-txtWidth - 22-4,ret_rect.top()+(ret_rect.height()-14)/2,22,14);

        QApplication::style()->drawItemPixmap(painter, yc_rect, m_alignment,*m_pixmap);
        painter->setPen(Qt::white);
        QFont font = m_charFont;
        font.setPixelSize(10);
        painter->setFont(font);
        QApplication::style()->drawItemText (painter, yc_rect, Qt::AlignCenter,option.palette, false,QStringLiteral("预估"));
    }
}


///DoubleFormatItemDelegate-----
void DoubleFormatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble() ;

    QString text = value == INVALID_DOUBLE?Default_Item_String:DelegateFunctions::toFormatString(value);

    painter->setPen(m_normalColor);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///LastPrice
void RatePriceItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role).toDouble();

    //根据涨幅判断颜色
    const SortFilterProxyModel *model =  static_cast<const SortFilterProxyModel *>(index.model());
    double rate  =  model->indexData(index.row(),ColRate).toDouble();
    rate = QString::number(rate,'f',2).toDouble();
    QString text = Default_Item_String;
    QColor color = m_plainColor;
    if(value != INVALID_DOUBLE && value != 0)
    {
        text = QString::number(value,'f',2);
        if(rate>0)
            color = m_riseColor;
        else if(rate<0)
            color = m_fallColor;
    }
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);
    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}


///PriceItemDelegate---
void PriceItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    double value  = index.data(Stock_Data_Role ).toDouble();
    //昨收价
    const SortFilterProxyModel *model =  static_cast<const SortFilterProxyModel *>(index.model());

    double preclosePrice =  model->colIdxData(index.row(),ColPreClosePrice).toDouble();

    QString text = Default_Item_String;
    QColor color = m_plainColor;
    if(value != INVALID_DOUBLE && value != 0)
    {
        text    = QString::number(value,'f',2);
        preclosePrice = QString::number(preclosePrice,'f',2).toDouble();
        value = text.toDouble();
        if(value>preclosePrice)
            color = m_riseColor;
        else if(value<preclosePrice)
            color = m_fallColor;
    }
    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);
    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
}


///RiseFallNumItemDelegate----
void RiseFallNumItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QString text = Default_Item_String;

    const SortFilterProxyModel *model =  static_cast<const SortFilterProxyModel *>(index.model());

    //涨家
    UINT32  rise_num =model->colIdxData(index.row(),ColRiseNum).toUInt();
    //跌家
    UINT32 fall_num   = model->colIdxData(index.row(),ColFallNum).toUInt();

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);
    painter->setFont(m_digitalFont);
    if(rise_num != INVALID_UINT32)
    {
        QFontMetrics fontMetrics(m_digitalFont);
        //绘制跌
        painter->setPen(m_fallColor);
        text = QString::number(fall_num);
        QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text);

        painter->setPen(m_normalColor);
        ret_rect.adjust(0,0,-fontMetrics.width(text),0);
        text = "/";
        QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text);

        painter->setPen(m_riseColor);
        ret_rect.adjust(0,0,-fontMetrics.width(text),0);
        QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,QString::number(rise_num));
    }else{
        painter->setPen(m_normalColor);
        //绘制文本
        QApplication::style()->drawItemText (painter, ret_rect, m_alignment,option.palette, true,text );
    }
}

///RiseFallRatioItemDelegate----
void RiseFallRatioItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    QString text = Default_Item_String;
    QList<QVariant> list= index.data(Stock_Data_Role).toList();
    UINT32 rise_num  = list[0].toUInt();
    UINT32 fall_num   = list[1].toUInt();
    double ratio =  list[2].toDouble();
    QColor color = m_plainColor;
    if(ratio != INVALID_DOUBLE)
    {
        if(rise_num >0 && fall_num == 0){
            text = QStringLiteral("全涨");
            color = m_riseColor;
        }
        else if(rise_num == 0 && fall_num > 0){
            text = QStringLiteral("全跌");
            color = m_fallColor;
        }
        else{
            text = QString::number(ratio,'f',2);

            ratio = QString::number(ratio,'f',2).toDouble();
            if(ratio>1.0)
                color = m_riseColor;
            else if(ratio<1.0)
                color = m_fallColor;
        }
    }

    painter->setPen(color);
    painter->setFont(m_digitalFont);

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);

    //绘制文本
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}


///TimeItemDelegate----
void TimeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);
    painter->setPen(m_plainColor);
    painter->setFont(m_digitalFont);

    UINT64 time_t = index.data(Stock_Data_Role).toULongLong();

    QString text = (time_t == 0 || time_t == INVALID_UINT64)?Default_Item_String:QDateTime::fromTime_t(time_t).toString("HH:mm");

    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}

///TradeDircItemDelegate
void TradeDircItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);

    painter->setFont(m_digitalFont);

    int dirc = index.data(Stock_Data_Role).toInt();

    QColor color = dirc==0?m_riseColor:m_fallColor;
    QString text = dirc==0?QStringLiteral("买入"):QStringLiteral("卖出");

    painter->setPen(color);
    QRect   ret_rect =  adjustedRect(m_alignment,option.rect);
    QApplication::style()->drawItemText (painter, ret_rect , m_alignment,option.palette, true,text );
}




///FileItemDelegate----
///
QPixmap*  FileItemDelegate::m_filePixmap = 0;
QPixmap*  FileItemDelegate::m_fileHoverPixmap = 0;
void FileItemDelegate::setPixmap(const QPixmap& pix1,const QPixmap& pix2)
{
    if(m_filePixmap) delete m_filePixmap;
    m_filePixmap = new QPixmap(pix1);
    if(m_fileHoverPixmap) delete m_fileHoverPixmap;
    m_fileHoverPixmap = new QPixmap(pix2);
}

void FileItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StockStyledItemDelegate::paint(painter,option,index);
    if(option.state & QStyle::State_HasFocus){
        QApplication::style()->drawItemPixmap(painter,option.rect,Qt::AlignCenter,*m_fileHoverPixmap);
    }
    else{
        QApplication::style()->drawItemPixmap(painter,option.rect,Qt::AlignCenter,*m_filePixmap);
    }
}

