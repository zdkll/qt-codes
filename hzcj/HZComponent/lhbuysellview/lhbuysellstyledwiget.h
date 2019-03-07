/********************************************************************
created:2018.10.31
author: juntang.xie
purpose:派生自BaseWidget 自定义样式属性窗体
*********************************************************************/
#ifndef LHBUYSELLSTYLEDWIGET_H
#define LHBUYSELLSTYLEDWIGET_H
#include<QWidget>

class LhBuySellStyledWiget:public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor DESIGNABLE true)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor DESIGNABLE true)
    Q_PROPERTY(QColor flatColor READ flatColor WRITE setFlatColor DESIGNABLE true)
public:
    explicit LhBuySellStyledWiget(QWidget *parent = nullptr);

    //上涨颜色
    QColor riseColor() const{ return m_riseColor;}
    void setRiseColor(const QColor &color){m_riseColor = color;}

    //下跌颜色
    QColor fallColor() const{return m_fallColor;}
    void setFallColor(const QColor &color){m_fallColor = color;}


    //平盘颜色
    QColor flatColor() const{return m_flatColor;}
    void setFlatColor(const QColor &color){m_flatColor = color;}

private:
    QColor m_riseColor;
    QColor m_fallColor;
    QColor m_flatColor;

};

#endif // LHBUYSELLSTYLEDWIGET_H
