#ifndef INDICATORCHART_H
#define INDICATORCHART_H
/********************************************************************
created:
author: min.cheng
purpose: 指标数据类 用于K线和分时的指标计算
*********************************************************************/
#include "basechart.h"
#include "IndiInterface.h"
#include "Observer.hpp"

struct IndiChartInfo
{
    int indiId;
    QString obj;
    int period;
};

class IndicatorProvider;

class IndicatorChart: public BaseChart
{
    Q_OBJECT
public:
    IndicatorChart(ImageLayer* layer,bool bDrawBk=true,bool bDrawMainArea=false);

    ~IndicatorChart();

    //创建数据提供者
    void createProvider(const QString& provider);

    //用于指标切换
    void switchParam(const QString& msg);

    //设置指标数据 switchParam立即切换 setParam只是设置命令，不立即切换
    void setParam(const QString& msg);

    //指标重绘
    virtual void redraw();

    //接收主图变化通知
    void mainChartXaixsChnage(const QString& msg);

    //接收指标数据计算完成通知
    void dataResponse(int id,int type);

    //接收指标数据计算错误通知
    void errorResponse(int id,int ncode,const QString& errReuslt);

    //外部直接調用接口設置指標信息
    void setIndiInfo(const IndiChartInfo& info);

    //切换指标
    void switchIndi(const IndiChartInfo& info);

    //设置指标Id
    void setIndi(int id);

    //获取当前显示指标的id
    int getIndiId();

    //观察者 用于通知外部表示指标计算完成
    Events<IndiAxisRefreshCallBack>& getObserverMgr();

    //不显示时，用于取消订阅
    virtual void afterDeactive();

    //十字光标 左侧格式化方法
    virtual QString leftTipInfoFormat(double value);

    //十字光标右侧格式化方法
    virtual QString rightTipInfoFormat(double value);

    //获取显示指标的最大值和最小值的下标
    void getMaxMinRangeIndex(int& maxPos,int& minPos);

    //获取最大最小值
    void getMaxMinRange(double& max,double& min);

    //主图是获取
    void getMaxMinRangeByMainArea(double& max,double& min);

    //系统事件处理
    bool mouseMoveEx(QMouseEvent *event);
    bool keyPressEx(QKeyEvent *event);
    void onResize();
signals:
    //计算完成信号，指标计算再其他线程中完成，通过信号把其完成后的处理放在了主线程中
    void signalDataResponse(int id,int type);
protected slots:
    //计算完成处理
    void soltDataResponse(int id,int type);
public slots:
    //处理参数设置
    virtual void soltButtonParamSet();
    //指标出参数改变通知
    void soltValueChanged(bool isChange);
protected:
    void drawText(QPainter &painter);
    void drawLine(QPainter &painter);
    //清理数据
    void resetData();
    void clearDrawData();
    bool paraseCmd(const QString& msg);
    void createTitleTextInfo(std::vector<TitleTextItem>& itemList,int index);
    bool isShowCross();
    void cacIndicator();
    void releaseIndiData(IndiRspResult& data);
    virtual void onCreate();
protected:
    std::vector<TitleTextItem> m_titleItemList;
    Events<IndiAxisRefreshCallBack> m_observerMgr; //指标数据计算完成后通知外部
    IndiRspResult m_result;
    std::shared_ptr<IndicatorProvider> m_provider;
    int m_indiId;
    QString m_obj;
    int m_period;
    int m_reqId;
    bool m_bSwitchIndi;
    bool m_bDrawBk;
    bool m_bDrawMainArea = false;
};

#endif // INDICATORCHART_H
