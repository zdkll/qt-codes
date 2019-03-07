#ifndef BIDDINGCANVAS_H
#define BIDDINGCANVAS_H
/********************************************************************
created:
author: min.cheng
purpose: 集合竞价
*********************************************************************/
#include <QVector>
#include <QLineF>
#include "basewidget.h"
#include "biddingprovider.h"
#include "quoteformat.h"
class ImageLayer;
class BiddingStyle: public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor m_bkClr READ getBkClr WRITE setBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_minLineClr READ getMinLineClr WRITE setMinLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_gridClr READ getGridClr WRITE setGridClr DESIGNABLE true)
    Q_PROPERTY(QColor m_upClr READ getUpClr WRITE setUpClr DESIGNABLE true)
    Q_PROPERTY(QColor m_downClr READ getDownClr WRITE setDownClr DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClr READ getEqualClr WRITE setEqualClr DESIGNABLE true)
    Q_PROPERTY(QColor m_titleTextClr READ getTitleTextClr WRITE setTitleTextClr DESIGNABLE true)
public:
    explicit BiddingStyle(QWidget *parent = 0);

    //背景色
    QColor getBkClr() const{return m_bkClr;}
    void setBkClr(QColor color){m_bkClr=color;}
    QColor m_bkClr = QColor("#0c1215");

    //曲线颜色
    QColor getMinLineClr() const{return m_minLineClr;}
    void setMinLineClr(QColor color){m_minLineClr=color;}
    QColor m_minLineClr = QColor(Qt::red);// QColor(54,129,227);

    //网格颜色
    QColor getGridClr() const{return m_gridClr;}
    void setGridClr(QColor color){m_gridClr=color;}
    QColor m_gridClr = QColor(Qt::blue);

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

    //文本颜色
    QColor getTitleTextClr() const{return m_titleTextClr;}
    void setTitleTextClr(QColor color){m_titleTextClr=color;}
    QColor m_titleTextClr=QColor(102,139,139);
};

class BiddingTitle : public BiddingStyle
{
    Q_OBJECT
public:
    explicit BiddingTitle(QWidget *parent = 0);

    void updateData(const BiddingItem& data);

    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
private:
    QColor getClrByDirc(HzQuoteFormat::QuoteDirc dirc);
    void draw();
    BiddingItem m_data;
};

class BiddingChart : public BiddingStyle
{
    Q_OBJECT
public:
    explicit BiddingChart(QWidget *parent = 0);

    void updateData(BiddingData* pData);

    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
private:
    void drawGrid();
    void drawLine();
    void CalcRc(QRect rc);
    void volStick(QPainter &dc,double ratio,double singleAreaWidth);
    void calcRc(double pos,double preClose,QColor& clr,HzQuoteFormat::QuoteItem& itemleft,
                QRectF& leftRc,HzQuoteFormat::QuoteItem& itemright,QRectF& rightRc,double ratio);
private:
    QRectF m_chartRc;
    QRectF m_volumeRc;
    QRectF m_timeRc;
    BiddingData m_data;
    QVector<QLineF> m_chartGridLines; //网格水平线
    QVector<int> m_verPtxs; //垂直点
    ImageLayer* m_imageLayer;
    int m_chartLayerId; //主图图层ID
    int m_gradientLayerId; //渐变层图层ID
    int m_gridAndAxisLayerId;
};

class BiddingCanvas: public BaseWidget
{
    Q_OBJECT
public:
    explicit  BiddingCanvas(QWidget *parent = 0);

    ~BiddingCanvas();

    void afterActive();

    void afterDeactive();

    void onDataRefresh(BiddingData* pData);
private:
    BiddingTitle* m_pTitle;
    BiddingChart* m_pChart;
    BiddingProvider* m_pProvider;
};

#endif // BIDDINGCANVAS_H
