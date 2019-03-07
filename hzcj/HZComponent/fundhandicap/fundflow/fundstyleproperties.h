/********************************************************************
created:
author: dk.zhang
purpose:资金样式属性类
*********************************************************************/
#ifndef FUNDSTYLEPROPERTIES_H
#define FUNDSTYLEPROPERTIES_H

#include <QColor>

//饼状图颜色
class FundPieStyleProperties
{
public:
    FundPieStyleProperties(){}

    QColor titleColor() const{return m_colors[0];}
    void setTitleColor(const QColor &color) {m_colors[0] = color;}

    QColor riseColor() const{return m_colors[1];}
    void setRiseColor(const QColor &color) {m_colors[1] = color;}

    QColor fallColor() const{return m_colors[2];}
    void setFallColor(const QColor &color) {m_colors[2] = color;}

    QColor inColor() const{return m_colors[3];}
    void setInColor(const QColor &color) {m_colors[3] = color;}

    QColor outColor() const{return m_colors[4];}
    void setOutColor(const QColor &color) {m_colors[4] = color;}

    QColor percentColor() const{return m_colors[5];}
    void setPercentColor(const QColor &color) {m_colors[5] = color;}

    QColor lineColor() const{return m_colors[6];}
    void setLineColor(const QColor &color) {m_colors[6] = color;}

protected:
    /*0 标题文本颜色
     *1 rise
     *2 fall
     *3 流入
     *4 流出
     *5 百分比
     *6 线条
     */
    QColor m_colors[7];
};


//条形图和表格颜色
class FundBarStyleProperties
{
public:
    FundBarStyleProperties(){}

    QColor textColor() const{return m_colors[0];}
    void setTextColor(const QColor &color) {m_colors[0] = color;}

    QColor inColor() const{return m_colors[1];}
    void setInColor(const QColor &color) {m_colors[1] = color;}

    QColor outColor() const{return m_colors[2];}
    void setOutColor(const QColor &color) {m_colors[2] = color;}

    QColor lineColor() const{return m_colors[3];}
    void setLineColor(const QColor &color) {m_colors[3] = color;}

protected:
    /*0 文本颜色
     *1 流入
     *2 流出
     *3 线条
     */
    QColor m_colors[4];
};


//30日主力增减仓
class FundIncDecStyleProperties
{
public:
    FundIncDecStyleProperties(){}

    QColor textColor() const{return m_colors[0];}
    void setTextColor(const QColor &color) {m_colors[0] = color;}

    QColor riseColor() const{return m_colors[1];}
    void setRiseColor(const QColor &color) {m_colors[1] = color;}

    QColor fallColor() const{return m_colors[2];}
    void setFallColor(const QColor &color) {m_colors[2] = color;}

    QColor riseMaxColor() const{return m_colors[3];}
    void setRiseMaxColor(const QColor &color) {m_colors[3] = color;}

    QColor fallMaxColor() const{return m_colors[4];}
    void setFallMaxColor(const QColor &color) {m_colors[4] = color;}

    QColor midColor() const{return m_colors[5];}
    void setMidColor(const QColor &color) {m_colors[5] = color;}

protected:
    /*0 文本颜色
     *1 增加
     *2 减少
     *3 增加Max
     *4 减少Max
     *5 中间颜色 Mid
     */
    QColor m_colors[6];
};



#endif // FUNDSTYLEPROPERTIES_H
