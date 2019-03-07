#ifndef KLINECHART_H
#define KLINECHART_H
/********************************************************************
created:
author: min.cheng
purpose: K线主图绘图逻辑
*********************************************************************/
#include <map>
#include <string>
#include <qlist.h>

#include "datamodeldef.h"
#include "basechart.h"
#include "chartdef.h"
#include "indicatorchart.h"
#include "datakline.h"

#define LeftReq 0x0001
#define RightReq 0x0002
#define UpkeyReq 0x0004
#define DownkeyReq 0x0008
#define DrReq 0x0010

struct DataReqInfo
{
    int offset = 0;

    QString obj = "";
    int period = -1;
    qint64 startTime = -1;
    int dirc = 1;
    int count = 300;

    qint8 reqtype = 0 ;
    bool bEnd_;       //数据结束了
};

class IndicatorChart;
class KlineDataProvider;
class KlineChart : public BaseChart
{
    Q_OBJECT
public:
    KlineChart(ImageLayer* layer);

    ~KlineChart();

    //获取数据提供者
    DataProvider* getProvider();

    //响应数据回调
    void dataResponse(int id,int type);

    //响应错误回调
    void errorResponse(int id,int ncode,const QString& errReuslt);

    //获取最大最小值
    void getMaxMinRange(double& max,double& min);

    //获取缓存数据量
    int getCacheSize();

    //获取缓存数据
    const QVector<HZData::HZKline>& getKlineData();

    //获取周期
    int getPeriod();

    //不显示时，用于取消订阅
    virtual void afterDeactive();

    virtual void afterActive();

    //获取单根K线的状态 返回status等价于KlineStatus 如果返回false 表示越界
    virtual bool getStatusFromIndex(int index,int& status);

    //设置基本数据
    void setBaseInfo(const QString& obj,int period,int indi);

    //切换股票和周期
    void switchObj(const QString& obj,int period);

    //切换主图指标
    void switchIndi(int indi);

    //是否绘制十字交叉点
    bool isDrawCrossLintPt();

    //主图收费指标相关
    void createSrvIndi();
    void removSrvIndi();
    void switchSrvIndi(int id);
signals:

protected slots:
    virtual void soltButtonClose();
    virtual void soltButtonParamSet();
    void soltButtonPlus();
    void soltButtonSub();
private:
    void soltRestorationComBoxChanged(int index);
    //绘制蜡烛图
    void drawCandle(QPainter& dc,const DataKLine& data,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y);
    //计算和绘制网格
    double getGridHeight(const QRectF& rc,double& row);
    void drawGrid(QPainter& painter);
    //绘制Y轴文本
    void drawYaxis(QPainter& painter);
    //绘制最大最小值提示
    void drawMinMaxTip(QPainter& painter);
    //主图绘制
    virtual void redraw();
    //主图指标计算完成回调
    void OnIndiChange(bool bRefresh,int id,BaseChart* pChart);
    //清理数据
    void clearDrawData();

    //系统事件处理
    bool mouseMoveEx(QMouseEvent *event);
    bool mousePressEx(QMouseEvent *event);
    bool mouseReleaseEx(QMouseEvent *event);
    bool keyPressEx(QKeyEvent *event);
    bool keyReleaseEx(QKeyEvent *event);
    virtual void onCreate();
    virtual void onResize();
    virtual void switchParam(const QString& msg);
    int reqKline(const DataReqInfo& req);
    int subKline();
    void cancleEx();
private:
    QString getObjCmd();
    void resetDefaultValue();
    void upKey();
    void downKey();
    void dragLeft(int offset);
    void dragRight(int offset);
    void refreshXYaxis(bool bReCalc);
private:
    QString m_obj;
    int m_period;
    //主图指标Id
    int m_indiId;
    //鼠标按下位置
    int m_pressPosX;
    //K线请求
    DataReqInfo m_klineReqParam;
    //K线订阅
    DataReqInfo m_klinesubParam;
    //处理K线数据
    KlineDataProvider* m_provider;
    //附图
    IndicatorChart* m_indiChart;
    //服务器指标指标附图
    IndicatorChart* m_srvIndiChart = nullptr;
    //上次请求是否返回,避免频繁请求
    bool m_lastReqRet;
    //服务器通知ID
    int srvIndiNotifyId = -1;
    QVector<IndicatorChart*> m_childChartRedraw;
};

#endif // KLINECHART_H
