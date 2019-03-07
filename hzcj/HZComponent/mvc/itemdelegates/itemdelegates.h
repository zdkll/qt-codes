/********************************************************************
created:2018/06/20
author: dk.zhang
purpose: 行情列表相关的同一类处理方式 列样式代理
*********************************************************************/
#ifndef ITEMDELEGATES_H
#define ITEMDELEGATES_H

#include "styleditemdelegate.h"

//从1开始序号
class NoItemDelegate: public StockStyledItemDelegate
{
public:
    NoItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignCenter;}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

//从零开始序号
class ZeroNoItemDelegate: public StockStyledItemDelegate
{
public:
    ZeroNoItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignCenter;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



/**
 * @brief  1 HZData::StockCode  2StockName
 * 普通文本样式:纯色  直接绘制文本
 */
class PlainTextItemDelegate: public StockStyledItemDelegate
{
public:
    PlainTextItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief 普通数字，长整数
 */
class ULongLongItemDelegate: public StockStyledItemDelegate
{
public:
    ULongLongItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief 普通数字，整数
 */
class PlainIntItemDelegate: public StockStyledItemDelegate
{
public:
    PlainIntItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief 股票和板块混合显示  HZData::StockCode ，判断是否以BK开头
 */
class CodeTextItemDelegate: public StockStyledItemDelegate
{
public:
    CodeTextItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignVCenter|Qt::AlignLeft;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  股票码,个股、指数等
 */
class StockCodeTextItemDelegate: public StockStyledItemDelegate
{
public:
    StockCodeTextItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignVCenter|Qt::AlignLeft;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  板块股票码
 */
class BKCodeTextItemDelegate: public StockStyledItemDelegate
{
public:
    BKCodeTextItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignVCenter|Qt::AlignLeft;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  2 StockName 股票名称
 */
class NameTextItemDelegate: public StockStyledItemDelegate
{
public:
    NameTextItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignCenter;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  5 Date 日期：格式：MM-dd
 */
class DateItemDelegate: public StockStyledItemDelegate
{
public:
    DateItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignCenter;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief    Date 日期：格式：yyyy-MM-dd
 */
class FullDateItemDelegate: public StockStyledItemDelegate
{
public:
    FullDateItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){m_alignment = Qt::AlignCenter;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief 4 UpDownPrice 5 Rate 16 Committee 34 ColSpeedRate
 * 涨跌  出入等 委比 判断正负 <=100% double 样式: 根据 值正负 判断颜色
 */
class RiseFallItemDelegate: public StockStyledItemDelegate
{
public:
    RiseFallItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * 绘制 % 符号
 * @brief 4 UpDownPrice 5 Rate 16 Committee 34 ColSpeedRate
 * 涨跌  出入等 委比 判断正负 <=100% double 样式: 根据 值正负 判断颜色
 */
class PercentItemDelegate: public StockStyledItemDelegate
{
public:
    PercentItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief 21 TotStock 23 CirStock
 * INT64 long long 量 >=0 样式: 纯色
 * 需要根据量大小规整 到万/亿、万亿单位,宽度为最大4 包含小数点
 */
class LonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    LonglongFormatItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class ULonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    ULonglongFormatItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief 103 ColLeaderNetput ColMaxNetput... 差值计算
 * LONGLONG 颜色区分正负（涨跌或方向流入流出）,且需要规范,显示整数
 */
class DvalLongLongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    DvalLongLongFormatItemDelegate(QObject *parent = Q_NULLPTR) :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief 103 ColLeaderNetput ColMaxNetput... 差值计算
 * Double 型 颜色区分正负（涨跌或方向流入流出）,且需要规范,显示整数
 */
class DvalDoubleFormatItemDelegate: public StockStyledItemDelegate
{
public:
    DvalDoubleFormatItemDelegate(QObject *parent = Q_NULLPTR) :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief 27  ColBuySellBuy 手数差
 * LONGLONG
 */
class BSBLongLongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    BSBLongLongFormatItemDelegate(QObject *parent = Q_NULLPTR) :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * 27  ColRiseDay 连涨
 *  INT
 */
class RiseDayItemDelegate: public StockStyledItemDelegate
{
public:
    RiseDayItemDelegate(QObject *parent = Q_NULLPTR) :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief 18 NpVolume内盘
 */
class NpLonglongFormatItemDelegate:public StockStyledItemDelegate
{
public:
    NpLonglongFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

/**
 * @brief  跌,流出 fallColor
 */
class FallLonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    FallLonglongFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  特殊 Output 资金流出 数字前添加负号
 */
class NegFallLonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    NegFallLonglongFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  特殊 Output 资金流出 数字前添加负号 - double 型
 */
class NegFallDoubleFormatItemDelegate: public StockStyledItemDelegate
{
public:
    NegFallDoubleFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief  riseColor  Input 资金流入,涨 Long long 型
 */
class RiseLonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    RiseLonglongFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
/**
 * @brief  riseColor  Input 资金流入,涨 Double   型
 */
class RiseDoubleFormatItemDelegate: public StockStyledItemDelegate
{
public:
    RiseDoubleFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
/**
 * @brief  fallColor  资金流出,跌 Double   型
 */
class FallDoubleFormatItemDelegate: public StockStyledItemDelegate
{
public:
    FallDoubleFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  19 WpVolume
 */
class WpLonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    WpLonglongFormatItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  NwpRate 内外比
 */
class NwpRateItemDelegate: public StockStyledItemDelegate
{
public:
    NwpRateItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  NwpRate 内外比 6 Volume
 * UINT64 long long 量 >=0 样式: 纯色
 * 需要根据量大小规整 到万/亿、万亿单位,宽度为最大4 包含小数点
 */
class VolULonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    VolULonglongFormatItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief  NwpRate 内外比 6 Volume
 * INT64 long long 量 >=0 样式: 纯色
 * 需要根据量大小规整 到万/亿、万亿单位,宽度为最大4 包含小数点
 */
class VolLonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    VolLonglongFormatItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  7 CurVolume 现手
 * 根据交易方向确定 fall / rise 颜色
 *
 */
class CurULonglongFormatItemDelegate: public StockStyledItemDelegate
{
public:
    CurULonglongFormatItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief  8   TurnoverRate  9 Pes  14 PreClosePrice  15 Ratio  20 Pbv 32 Ped
 * double  普通 样式: 单一颜色 精确到两位小数
 *
 */
class DoubleSimpleItemDelegate: public StockStyledItemDelegate
{
public:
    DoubleSimpleItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

//包含预估字样
class YuguDoubleSimpleItemDelegate: public StockStyledItemDelegate
{
public:
    YuguDoubleSimpleItemDelegate(QObject *parent = Q_NULLPTR):StockStyledItemDelegate(parent){}

   static void setPixmap(const QPixmap& pix);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
   static QPixmap *m_pixmap;
};



/**
 * @brief  10 Turnover  22 TotValue  24 CirValue
 *额 double   >=0 样式: 纯色
 * 需要根据量大小规整 到万/亿、万亿单位,宽度为最大4 包含小数点
 */
class DoubleFormatItemDelegate: public StockStyledItemDelegate
{
public:
    DoubleFormatItemDelegate(QObject *parent = Q_NULLPTR) :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief  3 LastPrice最新价，根据昨收价来设置
 */
class  RatePriceItemDelegate: public StockStyledItemDelegate
{
public:
    RatePriceItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



/**
 * @brief  11 HighestPrice 12 LowestPrice  13 OpenPrice  17 AvgPrice 25 BuyPrice 26 SellPrice
 * double 价格 样式: 跟 昨收价对比 判断颜色 double 精度小数点两
 */
class  PriceItemDelegate: public StockStyledItemDelegate
{
public:
    PriceItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/**
 * @brief  28 RiseFall
 * UINT32 涨跌家数  显示: 涨个数/跌个数, 涨跌颜色不一样
 */
class  RiseFallNumItemDelegate: public StockStyledItemDelegate
{
public:
    RiseFallNumItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  29  RiseFallRatio
 * double 涨跌比 涨/跌 %, 显示全涨和全跌 >1: rise <1: fall
 */
class  RiseFallRatioItemDelegate: public StockStyledItemDelegate
{
public:
    RiseFallRatioItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * @brief  29  Time UINT64 - 转 小时：分钟
 * double 涨跌比 涨/跌 %, 显示全涨和全跌 >1: rise <1: fall
 */
class  TimeItemDelegate: public StockStyledItemDelegate
{
public:
    TimeItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/**
 * 买入或者卖出，根据方向判断文字和方向
 */
class  TradeDircItemDelegate: public StockStyledItemDelegate
{
public:
    TradeDircItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



///特殊的文件打开列表
class  FileItemDelegate : public StockStyledItemDelegate
{
public:
    FileItemDelegate(QObject *parent = Q_NULLPTR)
        :StockStyledItemDelegate(parent){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    static void setPixmap(const QPixmap& pix1,const QPixmap& pix2);

private:
   static QPixmap *m_filePixmap,*m_fileHoverPixmap;
};

#endif // ITEMDELEGATES_H
