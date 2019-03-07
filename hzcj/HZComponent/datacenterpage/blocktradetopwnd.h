#ifndef BLOCKTRADETOPWND_H
#define BLOCKTRADETOPWND_H
/********************************************************************
created:
author: min.cheng
purpose: 融资融券详情顶部窗口
*********************************************************************/
#include "basewidget.h"
#include "hzinfocenter.h"
#include "calandarbar.h"

class HZCalanderCtrl;
class CustomCldBtn;
class NetBuyWidget;
class QLabel;
class QMenu;
class QWidgetAction;
class QLabel;
class SmtCurveGraphCanvas;
class BlockTradeTopWnd:public BaseWidget
{
   Q_OBJECT
public:
    explicit BlockTradeTopWnd(QWidget *parent = nullptr);
    //激活
    virtual void afterActive();
    //取消激活
    virtual void afterDeactive();
    //获取数据
    StockRzrqDetailList getDatas();
    //获取到当前时间
    qint64 getTime();
    //外部设置当前显示的日期
    void setCurrShowTime(qint64 time);
signals:
    void signalDataChanged();
private:
    void OnStockCodeChange();
    void createWg();
    QWidget* createCalandarBar();
    QWidget* createNavigateBar();
    void reqStockDatas();
    void reqDateList();
    void cancle();
    void onStocDatasCallBack(const InfoRet& ret,const StockRzrqDetailList& datas);
    void onStockDateListCall(const InfoRet& ret,const StockRzrqDateList& datas);
private:
    QLabel* m_pRzye = nullptr;
    QLabel* m_pRqye = nullptr;
    QLabel* m_pStockName = nullptr;
    StockRzrqDateList m_dateList;
    StockRzrqDetailList m_datas;
    qint64 m_listReqId=-1;
    qint64 m_timeListId=-1;
    qint64 m_dateTime =-1;
    qint64 m_externalTime=-1;//外部跳转设置
    CustomCalandarBtnMarster *mCalandarBtn;
    SmtCurveGraphCanvas* m_smtCurveWnd;
};

#endif // BLOCKTRADETOPWND_H
