#ifndef RISEFALLFLATBASEWIDGET_H
#define RISEFALLFLATBASEWIDGET_H
/********************************************************************
created:
author: min.cheng
purpose: 涨跌平家数图形绘制窗口
*********************************************************************/
#include "basewidget.h"
class RiseFallFlatProvider;
class RiseFallFlatBaseWidget: public BaseWidget
{
    Q_PROPERTY(QColor m_upClr READ getUpClr WRITE setUpClr DESIGNABLE true)
    Q_PROPERTY(QColor m_downClr READ getDownClr WRITE setDownClr DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClr READ getEqualClr WRITE setEqualClr DESIGNABLE true)
    Q_PROPERTY(QColor m_upClrText READ getUpClrText WRITE setUpClrText DESIGNABLE true)
    Q_PROPERTY(QColor m_downClrText READ getDownClrText WRITE setDownClrText DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClrText READ getEqualClrText WRITE setEqualClrText DESIGNABLE true)
    Q_PROPERTY(QColor m_clrText READ getClrText WRITE setClrText DESIGNABLE true)
    Q_OBJECT
public:
    explicit RiseFallFlatBaseWidget(QWidget *parent = 0,QString instName="");
public slots:
    void slotsItemRiseFallFlatChanged(const QString& bkStock,int rise,int fall,int flat);
protected:
    void paintEvent(QPaintEvent *e);
    virtual void draw();
protected:
    //上涨颜色
    QColor getUpClr() const{return m_upClr;}
    void setUpClr(QColor color){m_upClr=color;}
    QColor m_upClr = QColor(Qt::red);

    //下跌颜色
    QColor getDownClr() const{return m_downClr;}
    void setDownClr(QColor color){m_downClr=color;}
    QColor m_downClr = QColor(Qt::red);

    //平局颜色  （不涨不跌）
    QColor getEqualClr() const{return m_equalClr;}
    void setEqualClr(QColor color){m_equalClr=color;}
    QColor m_equalClr = QColor(Qt::red);

    //上涨文本颜色
    QColor getUpClrText() const{return m_upClrText;}
    void setUpClrText(QColor color){m_upClrText=color;}
    QColor m_upClrText = QColor(Qt::red);

    //下跌文本颜色
    QColor getDownClrText() const{return m_downClrText;}
    void setDownClrText(QColor color){m_downClrText=color;}
    QColor m_downClrText = QColor(Qt::red);

    //平局文本颜色  （不涨不跌）
    QColor getEqualClrText() const{return m_equalClrText;}
    void setEqualClrText(QColor color){m_equalClrText=color;}
    QColor m_equalClrText = QColor(Qt::red);

    //普通文本颜色
    QColor getClrText() const{return m_clrText;}
    void setClrText(QColor color){m_clrText=color;}
    QColor m_clrText = QColor(Qt::red);
protected:
    int m_rise;
    int m_fall;
    int m_flat;
    double m_riseRate;
    double m_fallRate;
    double m_flatRate;
    int m_total;
    int m_max;
    bool m_dataChanged;
};


//垂直矩形绘图
class RiseFallFlatRectChartWnd:public RiseFallFlatBaseWidget
{
     Q_PROPERTY(QColor m_clrLine READ getClrLine WRITE setClrLine DESIGNABLE true)
    Q_OBJECT
public:
    explicit RiseFallFlatRectChartWnd(QWidget *parent = 0,QString instName="");
protected:
    void draw();
private:
    //线条颜色
    QColor getClrLine() const{return m_clrLine;}
    void setClrLine(QColor color){m_clrLine=color;}
    QColor m_clrLine = QColor(Qt::red);
    void resizeEvent(QResizeEvent *event);
    QRect getRc();
private:
    QRectF m_chartRc;
    int m_width;
    QRectF m_rcRiseText;
    QRectF m_rcFallText;
    QRectF m_rcFlatText;
    QPointF m_linePt1;
    QPointF m_linePt2;
};
#endif // RISEFALLFLATBASEWIDGET_H
