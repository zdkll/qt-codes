/********************************************************************
created:2018.10.17
author: juntang.xie
purpose: 沪深综合上方张平跌组件
*********************************************************************/
#ifndef UPDOWNWIDGET_H
#define UPDOWNWIDGET_H
#include "basewidget.h"
#include "datamodeldefs/stockinfodef.h"

#define  LEGEND_DISTANCE                  78
#define  LEGEND_RECT_WIDTH                5
#define  LEGEND_TEXT_OFFSE                9
#define  LEGEND_TEXT_HEIGHT               11
#pragma execution_character_set("utf-8")

class ColorWidget:public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor upColor READ upColor WRITE setUpColor DESIGNABLE true)
    Q_PROPERTY(QColor equalColor READ equalColor WRITE setEqualColor DESIGNABLE true)
    Q_PROPERTY(QColor downColor READ downColor WRITE setDownColor DESIGNABLE true)
    Q_PROPERTY(QColor contexColor READ contexColor WRITE setContexColor DESIGNABLE true)
    Q_PROPERTY(QColor numColor READ numColor WRITE setNumColor DESIGNABLE true)
    Q_PROPERTY(QColor upRectColor READ upRectColor WRITE setUpRectColor DESIGNABLE true)
    Q_PROPERTY(QColor downRectColor READ downRectColor WRITE setDownRectColor DESIGNABLE true)
    Q_PROPERTY(QColor equalRectColor READ equalRectColor WRITE setEqualRectColor DESIGNABLE true)
    Q_PROPERTY(QColor upBorderColor READ upBorderColor WRITE setUpBorderColor DESIGNABLE true)
    Q_PROPERTY(QColor equalBorderColor READ equalBorderColor WRITE setEqualBorderColor DESIGNABLE true)
    Q_PROPERTY(QColor downBorderColor READ downBorderColor WRITE setDownBorderColor DESIGNABLE true)
public:
    explicit ColorWidget(QWidget *parent = nullptr):QWidget(parent){}

    QColor upColor() const{ return m_upColor;}
    void  setUpColor(const QColor &color){m_upColor = color;}

    QColor equalColor() const{ return m_equalColor;}
    void  setEqualColor(const QColor &color){m_equalColor = color;}

    QColor downColor() const{ return m_downColor;}
    void  setDownColor(const QColor &color){m_downColor = color;}

    QColor contexColor() const{ return m_contexColor;}
    void  setContexColor(const QColor &color){m_contexColor = color;}

    QColor numColor() const{ return m_numColor;}
    void  setNumColor(const QColor &color){m_numColor = color;}

    QColor upRectColor() const{ return m_upRectColor;}
    void  setUpRectColor(const QColor &color){m_upRectColor = color;}

    QColor downRectColor() const{ return m_downRectColor;}
    void  setDownRectColor(const QColor &color){m_downRectColor = color;}

    QColor equalRectColor() const{ return m_equalRectColor;}
    void  setEqualRectColor(const QColor &color){m_equalRectColor = color;}

    QColor upBorderColor() const{ return m_upBorderColor;}
    void  setUpBorderColor(const QColor &color){m_upBorderColor = color;}


    QColor equalBorderColor() const{ return m_equalBorderColor;}
    void  setEqualBorderColor(const QColor &color){m_equalBorderColor = color;}


    QColor downBorderColor() const{ return m_downBorderColor;}
    void  setDownBorderColor(const QColor &color){m_downBorderColor = color;}

private:
    QColor m_upColor;
    QColor m_equalColor;
    QColor m_downColor;
    QColor m_contexColor;
    QColor m_numColor;
    QColor m_upRectColor;
    QColor m_downRectColor;
    QColor m_equalRectColor;
    QColor m_upBorderColor;
    QColor m_equalBorderColor;
    QColor m_downBorderColor;
};

class UpDownWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit UpDownWidget(QWidget *parent = nullptr);

    void setData(const HZData::StockRiseFallInfo &risefallInfo);

    void afterActive();

    void afterDeactive();

    void setStockCode(const QString &stockCode);

protected:
    virtual void paintEvent(QPaintEvent *e);


private:
    void initData();
    void subRiseFallInfo();
    void cancelSub();
    void onSubRiseFallInfoArrived(ReqId reqId, const HZData::StockRiseFallInfo &riseFallInfo, ErrCode errCode);

private:
    struct UpDownWidgetRiseFallUint
    {
        int         value = 0;
        QString label;
    };
    QVector<UpDownWidgetRiseFallUint>    m_riseFallLegendSet;
    bool     m_actived;
    ReqId    m_subReqId;

    ColorWidget m_colorWidget;
};

#endif // UPDOWNWIDGET_H
