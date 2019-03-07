/********************************************************************
created:2018/08
author: dk.zhang
purpose:分时成交窗口，包括全景窗口和缩略窗口
*********************************************************************/
#ifndef TIMEDEALWIDGET_H
#define TIMEDEALWIDGET_H

#include "framewidget.h"
#include "hzcomponent_global.h"
#include "basetimedealwidget.h"


class QLabel;
//分时成交窗口-全景页面
class  AllTimeDealWidget : public BaseTimeDealWidget
{
    Q_OBJECT
public:
    AllTimeDealWidget(QWidget *parent = 0);
    ~AllTimeDealWidget();
    // @brief  初始化动态样式属性
    void initStylePropoerty();

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);

    void init();
    void startSub();

protected slots:
    void scrollPressed();
    void scrollActionTriggered();
    void scrollValueChanged(int value);

private:
    void initWg();
    void reqTickData();

    //倒拉请求数据
    ReqId posTickRequest(const int &index,const uint &count,const bool &is_desc);

    //请求数据
    void onTickArrived(const  ReqId  &reqId,const  HZData::Zhubi &,const  ErrCode &errCode);

    //订阅数据
    void onSubTickArrived(const  ReqId  &reqId,const  HZData::Zhubi &,const  ErrCode &errCode );

    //value 不变，更新page 和range
    void updateScrollBarPageAndRange();

    //更改value,对齐
    void alignScrollBar();

    void updateScrollBar(const int &value);

    int saveZhubiData(const ReqRecord &recrd,const HZData::Zhubi &zhubi);

private:
    QPair<int,int> m_dataScope;
};


//分时成交窗口-缩略页面HZCOMPONENTSHARED_EXPORT
class   SLTimeDealWidget : public BaseTimeDealWidget
{
    Q_OBJECT
public:
    SLTimeDealWidget(QWidget *parent = 0);
    ~SLTimeDealWidget();

    void initStylePropoerty();

    void setStockCode(const string &stockcode, const QString &stockName = QString());

protected:
    void init();
    void startSub();

    void resizeEvent(QResizeEvent *e);

protected slots:
    void reqTickData();

private:
    void initWg();

    ReqId posTickRequest(const int &index,const uint &count,const bool &is_desc);

    void onTickArrived(const  ReqId  &reqId,const  HZData::Zhubi & ,const  ErrCode &errCode);

    void onSubTickArrived(const  ReqId  &reqId,const  HZData::Zhubi &,const  ErrCode &errCode);
};


#endif // TIMEDEALWIDGET_H
