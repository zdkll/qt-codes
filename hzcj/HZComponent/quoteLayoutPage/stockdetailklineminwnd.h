#ifndef STOCKDETAILKLINEMINWND_H
#define STOCKDETAILKLINEMINWND_H
/********************************************************************
created:
author: min.cheng
purpose:个股详情页分时和K线页面 中部顶部区域
*********************************************************************/
#include <QMap>
#include <QStackedWidget>
#include "stockdetailbasewnd.h"
#include "chartdef.h"
class MinCanvas;
class KlineCanvas;
class QLabel;
class CSubTab;
class StockDetailKlineMinWnd: public StockDetailBaseWnd
{
    Q_OBJECT
public:
    explicit StockDetailKlineMinWnd(QWidget *parent = 0,QString instName="");

    //窗口创建
    void createWg();

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

    //分时,K线各个周期，多日分时切换
    void setCurrType(int type);
    int getCurrType();

    //设置主图指标
    void setMainIndi(int indi);

    //设置副图指标
    void setAttachIndi(int indi);

    //切换K线或分时
    void switchKlineAndMin();

    //复权切换
    void switchDrType();

    void onNetIoReady(bool ready);
signals:
    //隐藏坐标列表
    void signalHideLeftWnd(bool bHide);
    //隐藏显示右边窗口
    void signalHideRightWnd(bool bHide);
    //滚轮滚动/上下翻页实现切换合约
    void signalWheelChangeStock(const QString& obj,bool upDirc,int step);
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
    //周期切换
    void slotsSwitchPeriod(int id);
    //主图指标选择消息
    void slotsSwitchMainIndicator(int id);
    //附图指标选择消息
    void slotsSwitchAttachIndicator(int id);
    //模板指标选择消息
    void slotsSwitchTemplateIndicator(int id);
    //隐藏/显示按钮点击
    void slotsLeftClick(bool bClick);
    void slotsRightClick(bool bClick);
    //内部复权发生变化
    void slotDrTypeChanged(int type,bool bCtrl);
private:
    //股票合约变化
    void OnStockCodeChange();
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *e);
private:
    //创建周期选择标题
    QWidget* createTitle();
    //获取中间分时/K线窗口
    QWidget* createMidWnd();
    //获取底部指标选择导航兰
    QWidget* createBottomWnd();

    //标题区域获取周期切换面板
    QWidget* createPeriodPanel();
    //底部区域 底部指标选择区域
    QWidget* createMidBottomInidWnd();

    //K线分时相关配置
    QVector<ChartItemInfo> getMinChartInfo();
    QVector<ChartItemInfo> getKlineChartInfo();
    //绘制股票代码
    void setObjCodeLabel();
    //根据周期 和指标权限来调整指标模板
    void adjustSrvIndiByPeriodAndStock(const QString& stock,int period,int drType);
    //创建绘制特色指标、
    void createHztsIndicator();
private:
    //合约代码
    QLabel* m_objLabel;
    //周期
    int m_period;
    //指标ID
    int m_mainId;
    int m_attachId;
    //K线分时
    QStackedWidget* m_pStackWnd;
    MinCanvas* m_minWnd;
    KlineCanvas* m_klineWnd;
    //周期TAB
    CSubTab* m_periodTable;
    //周期下标和类型映射 key=下标 value=类型
    QMap<int,int> m_periodIndexTypeMap; //key == DetialKlineMinType value==服务器上传周期
    QMap<int,int> m_periodToTypeMap;  //key== 服务器对应的周期  value== DetialKlineMinType
    //主图TAB
    CSubTab* m_mainIndiTable;
    //附图TAB
    CSubTab* m_attachIndiTable;
    //模板指标 --多个指标同时呈现
    CSubTab* m_templateTable;
    struct HzSpecialIndicator
    {
        QVector<ChartItemInfo> list;
        int mainIndiId = -1;
    };
    QMap<int,HzSpecialIndicator> m_templateMap;
    int m_srvIndiId = -1;
    //
};

#endif // STOCKDETAILKLINEMINWND_H
