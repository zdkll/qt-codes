#ifndef MINCHART_H
#define MINCHART_H
/********************************************************************
created:
author: min.cheng
purpose: 分时绘图逻辑
*********************************************************************/
#include <qvector.h>
#include "basechart.h"
#include "chartdef.h"
#include "quoteformat.h"

class MinVolChart;
class IndicatorChart;
class MinProvider;
class Minchart: public BaseChart
{
    Q_OBJECT
public:
    Minchart(ImageLayer* layer);

    ~Minchart();

    //数据响应
    void dataResponse(int id,int type);

    //数据请求错误响应
    virtual void errorResponse(int id,int ncode,const QString& errReuslt);

    //重写最大值和最小值
    virtual void getMaxMinRange(double& max,double& min);

    //十字光标Y轴设置调整 true表示设置成功，否则失败
    bool adjustCrossLineYAxis(std::shared_ptr<BaseChart> crossChart,const QPoint& pos);

    virtual void afterActive();

    //不显示时，用于取消订阅
    virtual  void afterDeactive();

    //获取单根K线的状态 返回status等价于KlineStatus 如果返回false 表示越界
    bool getStatusFromIndex(int index,int& status);

    //对外提供数据
    const QVector<HZData::HZKline>& getKlineData();

    //十字光标右侧格式化方法
    QString rightTipInfoFormat(double value);

    //返回数据提供者
    DataProvider* getProvider();

    //返回当前股票
    const QString& getObj() const;

    //返回当前显示分时数据
    const DataKLine& getMinData() const;

    //返回多日分时的分割下标
    const QVector<int>& getDayIndexList() const;

    //获取歷史分時天數
    int getDayCount();

    //切换股票
    void switchStock(const QString& obj,int day);

    //获取chart主要绘图区域
    QRect getChartMainArea();

    //获取X轴右边坐标区域
    QRect getRightAxisArea();

    //获取X轴右边坐标区域
    QRect getLeftAxisArea();

    bool isDrawAvgLine();
signals:

public slots:
    void soltButtonPlus();
    void soltButtonSub();
protected:
    QRect getVolArea();
    void calcRc(double pos,double preClose,double gridHeight,QColor& clr,HzQuoteFormat::QuoteItem& itemleft,QRectF& leftRc,HzQuoteFormat::QuoteItem& itemright,QRectF& rightRc);
    void drawMin(QPainter& dc,QPainter& gradDc,const DataKLine& data,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y);
    double getGridHeight(const QRectF& rc,double& row);
    virtual void redraw();
    void drawGrid(QPainter& painter);
    void drawYaxis(QPainter& painter);
    bool mouseMoveEx(QMouseEvent *event);
    virtual bool keyPressEx(QKeyEvent *event);
    virtual void onCreate();
    virtual void onResize();
    virtual void switchParam(const QString& msg);
    void resetDefaultValue();
    int reqKline();
    int subKline();
    void cancle(int& id);
    void upKey();
    void downKey();
    void refreshXYaxis(bool bReCalc);
    QString getObjCmd();
    virtual void createTitleTextInfo(std::vector<TitleTextItem>& itemList,int index);
    //清理数据
    void clearDrawData();
    void changeAvgDrawFlag(const QString& stock);
protected:
    //成交量绘制
    MinVolChart* m_volChart;
    //標題欄文本項
    std::vector<TitleTextItem> m_titleItemList;
    QVector<int> m_dayIndexList; //当前天对应的数据下标
    //K线请求
    int m_klineReqId;
    //K线订阅
    int m_klineSubId;
    //当前市场和合约
    QString m_obj;
    //分时数据提供者
    MinProvider* m_provider;
    //当前显示的历史分时天数 0 代表 当前最新的一天，一次后推
    int m_dayIndex;
    int m_historyTime; //历史数据
    //渐变图层
    int m_gradientLayer;
    //昨天收价
    double m_preClose;
    //是否绘制均线
    bool m_drawAvgPrice;
    //网格对应点
    QVector<double> m_gridPosVec;
};

#endif // MINCHART_H
