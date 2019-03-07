/********************************************************************
created:2018/6/20
author: dk.zhang
purpose:行情列表样式代理公共基类,实现动态样式属性赋值
*********************************************************************/
#ifndef STYLEDITEMDELEGATE_H
#define STYLEDITEMDELEGATE_H

#define  Col_Margin         10

#include <QStyledItemDelegate>
#include "mvcpublicdef.h"
#include <QPixmap>

//No Focus
class  NoFocusStyledItemDelegate : public QStyledItemDelegate
{
public:
    NoFocusStyledItemDelegate(QObject *parent = 0)
        :QStyledItemDelegate(parent){}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


//实现列左右对齐
class ColStyledItemDelegate:public NoFocusStyledItemDelegate
{
public:
    ColStyledItemDelegate(QObject *parent = 0)
        :NoFocusStyledItemDelegate(parent){}

    static QRect adjustRect(const QRect &rect,int margin,Qt::Alignment alignment){
        return (alignment&Qt::AlignLeft)?rect.adjusted(margin,0,0,0):
                                         (alignment&Qt::AlignRight)?rect.adjusted(0,0,-margin,0):rect;
    }

    void setAlignment(Qt::Alignment &alignment){m_alignment = alignment;}

protected:
    //左右对齐和Margin
    int                    m_margin{8};
    Qt::Alignment  m_alignment{Qt::AlignCenter};
};



//行情列表字段相关样式基类，包括一般的静态属性
class  StockStyledItemDelegate : public NoFocusStyledItemDelegate
{
public:
    //格式化数据类型
    enum StyleDataType
    {

    };
    StockStyledItemDelegate(QObject *parent = 0);
    //设置对齐
    void setAlignment(const Qt::Alignment &alignment){m_alignment = alignment;}
    //设置颜色
    void setColor(const QColor &color){m_color = color;}
    //设置字体
    void setFont(const QFont &font){ m_font = font;}

    static bool isInitedStyleProperties(){return m_isInitedStyleProperties;}
    static void initStyleProperties(const StyleProperties &properties);

    //设置Rise Color 数值表示升
    static void setRiseColor(const QColor &color){m_riseColor = color;}
    //设置Plain Color 数值表示平
    static void setPlainColor(const QColor &color){m_plainColor = color;}
    //数值表示降 Fall Color
    static void setFallColor(const QColor &color){m_fallColor = color;}
    //设置普通颜色，如文本代码等
    static void setNormalColor(const QColor &color){m_normalColor = color;}

    //设置数字的字体
    static void setDigitalFont(const QFont &font){
        m_digitalFont = font;
        m_digitalFont.setStyleStrategy(QFont::NoAntialias);
    }
    //设置文字的字体
    static void setCharFont(const QFont &font){
        m_charFont = font;
        m_charFont.setStyleStrategy(QFont::NoAntialias);
    }
    //设置右对齐Margin
    void setRightMargin(const int &margin){m_rightMargin = margin;}
    //设置左对齐Margin
    void setLeftMargin(const int &margin){m_leftMargin = margin;}


protected:
    //根据对齐方式和margin调整绘制矩形
    QRect adjustedRect(Qt::Alignment alignment,const QRect &rect) const
    {
        return (alignment & Qt::AlignRight)?rect.adjusted(0,0,-m_rightMargin,0):
                                            (alignment & Qt::AlignLeft)?rect.adjusted(m_leftMargin,0,0,0):rect;
    }

protected:
    Qt::Alignment   m_alignment;
    QColor              m_color;
    QFont               m_font;

    static QColor    m_riseColor,m_plainColor,m_fallColor,m_normalColor;
    static QFont     m_digitalFont,m_charFont;

    int                  m_rightMargin,m_leftMargin;

    static bool     m_isInitedStyleProperties;
};


//盘口相关列表的样式
class HandStyledItemDelegate : public ColStyledItemDelegate
{
public:
    HandStyledItemDelegate(QObject *parent = 0):ColStyledItemDelegate(parent){}

    static bool isInitedStyleProperties(){return m_isInitedStyleProperties;}
    static void initStyleProperties(const StyleProperties &properties);

protected:
    static QColor m_riseColor,  m_plainColor,m_fallColor,m_normalColor,m_gridLineColor,m_bpColorFirst;
    static QFont  m_digitalFont,m_charFont;

private:
    static bool m_isInitedStyleProperties;
};


//一般的列表样式
class  NormalStyledItemDelegate : public NoFocusStyledItemDelegate
{
public:
    NormalStyledItemDelegate(QObject *parent = 0):NoFocusStyledItemDelegate(parent){}

    static bool isInitedStyleProperties(){return m_isInitedStyleProperties;}
    static void initStyleProperties(const StyleProperties &properties);

protected:
    static QColor  m_riseColor,m_plainColor,m_fallColor,m_normalColor;
    static QFont   m_digitalFont,m_charFont;
    static bool      m_isInitedStyleProperties;
};




#endif // STYLEDITEMDELEGATE_H
