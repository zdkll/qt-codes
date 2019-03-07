/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自BaseWidget   集合竞价图表窗体
*********************************************************************/
#ifndef SETBIDDINGWIDGET_H
#define SETBIDDINGWIDGET_H
#include "scrollwidget.h"
#include "fundchangechartwidget.h"
#include "datacontroller.h"
#include "hzpublic.hpp"
using namespace HZData;





struct SetBiddingDataUnit
{
    QString strName = "";  //下方显示文本
    QString strRate = "";  //涨幅
    float  rate = INVALID_FLOAT;   //涨速
//    QColor borderColor;   //矩形边框颜色
//    QColor brushColor;    //矩形填充颜色
//    QColor numColor;      //数字颜色
};

enum RateDir{
    RiseDir = 0,
    FallDir
};

typedef QVector<SetBiddingDataUnit> SetBiddingSet;

struct SetBiddingData
{
    SetBiddingSet biddingSet;
    float maxValue = INVALID_FLOAT;
    QString strMaxScale = ""; //最大坐标刻度
    QString strHalfScale = ""; //中间坐标刻度
};


/**
  * @brief 集合竞价绘图窗体
  */
class SetBiddingChart:public BaseWidget
{
    Q_OBJECT
public:
    explicit SetBiddingChart(QWidget *parent = nullptr);
    ~SetBiddingChart();

    void updateData();

    void updateData(const BiddingBlockInfo &info, RateDir rateDir = RiseDir);


protected:
    virtual void paintEvent(QPaintEvent *event);
//    virtual void resizeEvent(QResizeEvent *e);

private:
    void draw();
    void transformRiseData(const BiddingBlockInfo &info);
    void transformFallData(const BiddingBlockInfo &info);

private:
    QPixmap *m_pix;
    QFont m_textFont;
    QFont m_numFont;

    SetBiddingData m_riseData;
    SetBiddingData m_fallData;

    FundChartStyledWidget m_styledWidget;

};


/**
  * @brief 集合竞价窗体
  */
class SetBiddingWidget:public  ScrollWidget
{
    Q_OBJECT
public:
    explicit SetBiddingWidget(QWidget *parent = nullptr);

    //激活对象
    virtual void afterActive();
    //取消激活
    virtual void afterDeactive();

private:
    void createWg();

    void startSub();

    void cancelSub();


    void onDataArrived(ReqId id, const BiddingBlockInfo &info, ErrCode errCode);

private:
    ReqId  m_nRiseRed;

    ReqId m_nFallRed;

    SetBiddingChart *m_subwidget;
};

#endif // SETBIDDINGWIDGET_H
