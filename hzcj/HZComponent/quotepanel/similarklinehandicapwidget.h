#ifndef SimilarKlineHandicapWidget_H
#define SimilarKlineHandicapWidget_H
/********************************************************************
created: 209.2.25
author: juntang.xie
purpose: 相似K线盘口
*********************************************************************/
#include "stockdetailbasewnd.h"
#include "datacontroller.h"



class QStackedWidget;
class HandicapTitleWidget;
class HandicapCentralWidget;
class HandicapBottomWidget;
class SimilarKlineHandicapTitleWidget;
class HandicapNewsWidget;
class HZAnncsInfo;
class HZNewsInfo;
class HZResearchReport;
class SimilarKlineHandicapWidget: public StockDetailBaseWnd
{
    Q_OBJECT
public:
    explicit SimilarKlineHandicapWidget(QWidget *parent = 0,QString instName="");

    //窗口创建
    void createWg();

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();


    void switchStocks(const QString& stock1,const QString& stock2);

signals:

public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
private:
    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);
    //请求股票
    void subDyna();
    //取消订阅
    void cancelSub();
    //股票合约变化
    void OnStockCodeChange();

private slots:
    void onBtnClicked(int id);
private:
    //合约
    QString m_obj;
    //定义ID
    ReqId m_nReqSelect;
    ReqId m_nReqSimilar;

    SimilarKlineHandicapTitleWidget *m_selectedTitleWidget;
    SimilarKlineHandicapTitleWidget *m_similarTitleWidget;
    HandicapCentralWidget *m_selectedCentralWidget;
    HandicapCentralWidget *m_similarCentralWidget;
    QString m_strSelectedCode;
    QString m_strSimilarCode;

    QStackedWidget *m_stackWidget;
    HandicapNewsWidget *m_newswidget;
    HZAnncsInfo* m_anncInfo;
    HZNewsInfo* m_newsInfo;
    HZResearchReport* m_researchReport;
};

#endif // SimilarKlineHandicapWidget_H
