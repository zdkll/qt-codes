/********************************************************************
created:2018/06
author: dk.zhang
purpose:行情列表样式属性类
*********************************************************************/
#ifndef STOCKDYNAVIEWPROPERTIES_H
#define STOCKDYNAVIEWPROPERTIES_H

#include <QtCore>
#include <QPixmap>
#include "mvcpublicdef.h"

///行情列表属性
class ViewStyleProperties
{
public:
    ViewStyleProperties(){}
    //涨-颜色
    QColor riseColor()const {return m_riseColor;}
    void     setRiseColor(const QColor &color){m_riseColor = color;}

    //无效值,无变化值颜色
    QColor plainColor()const {return m_plainColor;}
    void     setPlainColor(const QColor &color){m_plainColor = color;}

    //跌颜色
    QColor fallColor()const {return m_fallColor;}
    void     setFallColor(const QColor &color){m_fallColor = color;}

    //普通颜色
    QColor normalColor()const {return m_normalColor;}
    void     setNormalColor(const QColor &color){m_normalColor = color;}

    QColor gridLineColor()const{return m_gridLineColor;}
    void     setGridLineColor(const QColor &color){m_gridLineColor = color;}

    QPixmap filePixmap()const{return m_filePixmap;}
    void setFilePixmap(const QPixmap &pix){m_filePixmap = pix;}

    QPixmap fileHoverPixmap()const{return m_fileHoverPixmap;}
    void setFileHoverPixmap(const QPixmap &pix){m_fileHoverPixmap = pix;}

    QPixmap ygPixmap()const{return m_ygPixmap;}
    void setYgPixmap(const QPixmap &pix){m_ygPixmap = pix;}


    StyleProperties styleProperties();

protected:
    QColor m_riseColor,m_plainColor,m_fallColor;
    QColor m_normalColor,m_gridLineColor;

    QPixmap  m_filePixmap,m_fileHoverPixmap,m_ygPixmap;
};


///分时成交样式
class TimeDealProperties
{
public:
    //style propertys----
    //成交时间颜色
    QColor colTimeColor()const { return m_colTimeColor;}
    void      setColTimeColor(const QColor &color){m_colTimeColor = color;}
    //成交价颜色
    QColor colPriceColor()const { return m_colPriceColor;}
    void      setColPriceColor(const QColor &color){m_colPriceColor = color;}
    //买入颜色
    QColor colVolUpColor()const { return m_colVolUpColor;}
    void      setColVolUpColor(const QColor &color){m_colVolUpColor = color;}
    //卖出颜色
    QColor colVolDownColor()const { return m_colVolDownColor;}
    void      setColVolDownColor(const QColor &color){m_colVolDownColor = color;}
    //网格颜色
    QColor gridLineColor(){return m_gridLineColor;}
    void      setGridLineColor(const QColor &color){m_gridLineColor = color;}
    //字体名称
    QString  fontFamily()const { return m_tickFontFamily;}
    void       setFontFamily(const QString &fontFamily){m_tickFontFamily = fontFamily;}
    //字体大小
    int         fontPointSize()const { return m_tickFontPointSize;}
    void      setFontPointSize(const int &pointSize){m_tickFontPointSize = pointSize;}

    //up-down arrow
    //买入箭头图片
    QPixmap upArrowPix(){return m_upArrowPix;}
    void         setUpArrowPix(const QPixmap &pix){m_upArrowPix = pix;}
    //卖出箭头图片
    QPixmap downArrowPix(){return m_downArrowPix;}
    void         setDownArrowPix(const QPixmap &pix){m_downArrowPix = pix;}

private:
    //style propertys-----
    QColor  m_colTimeColor;
    QColor  m_colPriceColor;
    //笔数两种颜色
    QColor  m_colVolUpColor;
    QColor  m_colVolDownColor;
    QColor  m_gridLineColor;

    QString   m_tickFontFamily;
    int           m_tickFontPointSize;

    QPixmap m_upArrowPix,m_downArrowPix;
};


///盘口样式
class  HandStyledProperities
{
public:
    HandStyledProperities(){}
    //涨-颜色
    QColor riseColor()const {return m_styledProperties.riseColor;}
    void     setRiseColor(const QColor &color){m_styledProperties.riseColor = color;}

    //持平颜色
    QColor plainColor()const {return m_styledProperties.plainColor;}
    void     setPlainColor(const QColor &color){m_styledProperties.plainColor = color;}

    //跌颜色
    QColor fallColor()const {return m_styledProperties.fallColor;}
    void     setFallColor(const QColor &color){m_styledProperties.fallColor = color;}

    //普通颜色
    QColor normalColor()const {return m_styledProperties.normalColor;}
    void     setNormalColor(const QColor &color){m_styledProperties.normalColor = color;}


    //网格线颜色
    QColor gridLineColor()const {return m_styledProperties.gridLineColor;}
    void     setGridLineColor(const QColor &color){m_styledProperties.gridLineColor = color;}


    //其他备用颜色
    QColor bpColorFirst()const {return m_styledProperties.bpColorFirst;}
    void     setBpColorFirst(const QColor &color){m_styledProperties.bpColorFirst = color;}


    StyleProperties   styledProperties(){return m_styledProperties;}

private:
    StyleProperties  m_styledProperties;
};

//导航左侧导航列表样式
class  NaviStyledProperities
{
public:
    NaviStyledProperities(){}
    //涨-颜色
    QColor riseColor()const {return m_styledProperties.riseColor;}
    void     setRiseColor(const QColor &color){m_styledProperties.riseColor = color;}

    //跌颜色
    QColor fallColor()const {return m_styledProperties.fallColor;}
    void     setFallColor(const QColor &color){m_styledProperties.fallColor = color;}

    //持平颜色
    QColor plainColor()const {return m_styledProperties.plainColor;}
    void     setPlainColor(const QColor &color){m_styledProperties.plainColor = color;}

    //普通数字颜色
    QColor numberColor()const {return m_styledProperties.numberColor;}
    void     setNumberColor(const QColor &color){m_styledProperties.numberColor = color;}

    //文本颜色
    QColor textColor()const {return m_styledProperties.textColor;}
    void     setTextColor(const QColor &color){ m_styledProperties.textColor = color;}

    //网格线颜色
    QColor gridLineColor()const {return m_styledProperties.gridLineColor;}
    void     setGridLineColor(const QColor &color){m_styledProperties.gridLineColor = color;}

    StyleProperties   styledProperties(){return m_styledProperties;}

private:
    StyleProperties  m_styledProperties;
};

//自定义表头（资金表头/相似K线等）样式
class CustomHeaderStyledProperities
{
public:
    CustomHeaderStyledProperities(){}
    //向上排序箭头
    void         setUpPixmap(const QPixmap &pix){m_upPixmap = pix;}
    QPixmap  upPixmap() const {return m_upPixmap;}
    //向下排序箭头
    void          setDownPixmap(const QPixmap &pix){m_downPixmap = pix;}
    QPixmap   downPixmap() const {return m_downPixmap;}
    //字体颜色
    void          setFontColor(const QColor &color){m_fontColor = color;}
    QColor      fontColor()const {return m_fontColor;}
protected:
    //样式属性
    QPixmap        m_upPixmap,m_downPixmap;
    QColor           m_fontColor;

    QFont            m_font;
};

#endif // STOCKDYNAVIEWPROPERTIES_H
