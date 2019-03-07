/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自BaseWidget 自定义样式属性窗体
*********************************************************************/
#ifndef ShortTermChangeStyledWidget_H
#define ShortTermChangeStyledWidget_H
#include "basewidget.h"


class ShortTermChangeStyledWidget:public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor DESIGNABLE true)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor DESIGNABLE true)
    Q_PROPERTY(QColor flatColor READ flatColor WRITE setFlatColor DESIGNABLE true)
public:
    explicit ShortTermChangeStyledWidget(QWidget *parent = nullptr);
    //设置上涨颜色
    QColor riseColor() const{return m_riseColor;}
    void setRiseColor(const QColor &color){m_riseColor = color;}

    //设置下跌颜色
    QColor fallColor() const{return m_fallColor;}
    void setFallColor(const QColor &color){m_fallColor = color;}

    //设置平盘颜色
    QColor flatColor() const{return m_flatColor;}
    void setFlatColor(const QColor &color){m_flatColor = color;}

private:
    QColor m_riseColor;
    QColor m_fallColor;
    QColor m_flatColor;
};

#endif // ShortTermChangeStyledWidget_H
