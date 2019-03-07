/********************************************************************
created:2018/06/14
author: dk.zhang
purpose:分时成交 样式代理 -针对时间,成交价,成交量三个字段,分为总的逐笔列表和盘口右下角缩略逐笔列表
*********************************************************************/
#ifndef TIMEDEALITEMDELEGATES_H
#define TIMEDEALITEMDELEGATES_H

#include <QStyledItemDelegate>
#include "mvcpublicdef.h"

/**
  * @brief 逐笔代理基类，保存样式属性值
  */
class TickItemDelegate : public QStyledItemDelegate
{
public:
    TickItemDelegate(QObject *parent = Q_NULLPTR):QStyledItemDelegate(parent){}
    ~TickItemDelegate(){}

    //设置该字段颜色
    void  setColor(const QColor &color){m_color = color;}
    //设置该字段字体
    void setFont(const QFont &font){
        m_font = font;
        m_font.setStyleStrategy(QFont::NoAntialias);
    }
    //如果自绘网格线的话，需要设置网格线的颜色
    void setGridLineColor(const QColor &color){m_gridLineColor = color;}

protected:
    QColor m_color;

    QColor m_gridLineColor;

    QFont  m_font;
};

/**
  * @brief 时间字段显示代理，逐笔全图
  */
class AllTickColTimeDelegate : public TickItemDelegate
{
public:
    AllTickColTimeDelegate(QObject *parent = Q_NULLPTR):TickItemDelegate(parent){}
    ~AllTickColTimeDelegate(){}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

/**
  * @brief 成交价 double，逐笔全图
  */
class AllTickColPriceDelegate : public TickItemDelegate
{
public:
    AllTickColPriceDelegate(QObject *parent = Q_NULLPTR);
    ~AllTickColPriceDelegate(){}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

/**
  * @brief 手数 int，逐笔全图
  */
class AllTickColVolumeDelegate : public TickItemDelegate
{
public:
    AllTickColVolumeDelegate(QObject *parent = Q_NULLPTR);
    ~AllTickColVolumeDelegate(){}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //设置买入颜色
    void setUpColor(const QColor &color){m_upColor = color;}
    //设置卖出颜色
    void setDownColor(const QColor &color){m_downColor = color;}
    //设置买入箭头
    void setUpArrowPix(const QPixmap &pix){m_upArrowPix = pix; }
    //设置卖出箭头
    void setDownArrowPix(const QPixmap &pix){m_downArrowPix = pix;}

private:
    QPixmap m_upArrowPix,m_downArrowPix;
    QColor    m_upColor,m_downColor;
};




///缩略图 样式-----------------------------------------------------
/**
* @brief 时间 QString，逐笔缩略图
*/
class SLTickColTimeDelegate : public TickItemDelegate
{
public:
    SLTickColTimeDelegate(QObject *parent = Q_NULLPTR):TickItemDelegate(parent){}
    ~SLTickColTimeDelegate(){}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};


/**
* @brief 成交价 double，逐笔缩略图
*/
class SLTickColPriceDelegate : public TickItemDelegate
{
public:
    SLTickColPriceDelegate(QObject *parent = Q_NULLPTR);
    ~SLTickColPriceDelegate(){}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QPixmap   m_gridLinePix;
};

/**
* @brief 手数 uint64，逐笔缩略图
*/
class SLTickColVolumeDelegate : public TickItemDelegate
{
public:
    SLTickColVolumeDelegate(QObject *parent = Q_NULLPTR);
    ~SLTickColVolumeDelegate(){}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //设置买入颜色
    void setUpColor(const QColor &color){m_upColor = color;}
    //设置卖出颜色
    void setDownColor(const QColor &color){m_downColor = color;}
    //设置买入箭头
    void setUpArrowPix(const QPixmap &pix){m_upArrowPix = pix; }
    //设置卖出箭头
    void setDownArrowPix(const QPixmap &pix){m_downArrowPix = pix;}

private:
    QPixmap m_upArrowPix,m_downArrowPix;
    QColor     m_upColor,m_downColor;
};



#endif // TIMEDEALITEMDELEGATES_H
