#ifndef DATACENTERBASEWND_H
#define DATACENTERBASEWND_H
/********************************************************************
created:
author: min.cheng
purpose: 数据页面左侧基础类
*********************************************************************/
#include <QMap>
#include "basewidget.h"
#include "subtab.h"
#include "hzinfocenter.h"
#include "hzupdowngraph.h"
#include "hzinfocenter.h"
#include "calandarbar.h"
#include "hzchildwndmgr.h"

class SmtLsGraphTitle;
class SmtCurveGraphCanvas;
class LhbStockView;
class QStackedWidget;
class QLabel;
class BlockTradeTopWnd;
//龙虎榜
class LhbYybHoView;
class LhbYybSbView;
class LhbYybDrbView;
class LhbYybSlbView;
class LhbYybHybView;

//融资融券
class StockRzrqRankTableView; //个股融资融券净买入卖出前10
class StockRzrqView;//个股买入榜
class BlockRzrqView;//板块融资融券榜
class BlockStockRzrqView;
class StockRzrqDetailView;

//大宗交易
class StockDzjyView;

//新股日历
class XgrlTableView;

class DatacenterBaseWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit DatacenterBaseWnd(QWidget *parent = nullptr, QString instName = "");
signals:
    //股票改变
    void signalStockObjChanged(const QString& stock);
    //双击跳转到详情页面
    void signalSWitchDetailPage(const QString& stock);
};

//上中下结构页类的创建
struct NormalTabStatckItem
{
    int tabId = 0;
    QString name;
    QWidget* pWnd = nullptr;
    HZObject* pObj = nullptr;
    NormalTabStatckItem(){}
    NormalTabStatckItem(int id,const QString& strName,QWidget* wnd,HZObject* obj):
        tabId(id),
        name(strName),
        pWnd(wnd),
        pObj(obj)
    {

    }
};

class NormalChildWnd: public DatacenterBaseWnd
{
    Q_OBJECT
public:
    explicit NormalChildWnd(QWidget *parent = nullptr, QString instName = "");

    void afterActive();

    void afterDeactive();

    //创建流程
    virtual void createWg();
protected:
    //获取当前窗口创建数据
    virtual void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);
    //创建顶部窗口
    virtual BaseWidget* createTopWnd();
    //创建TAB
    CSubTab* createTab();
    //创建底部视图列表相关
    QWidget* createBottomWnd();
protected slots:
    //tab切换事件
    virtual void slotsTabSwtichWnd(int id);
protected:
    BaseWidget* m_topWnd = nullptr;
    QStackedWidget* m_pstackWnd = nullptr;
    CSubTab* m_tabSub = nullptr;
    //列表和TAB映射表
    QMap<int,NormalTabStatckItem> m_listTabMap; //key=TAB  value=堆栈窗口下标
    QMap<int,NormalTabStatckItem> m_listIndexMap; //key=index value=堆栈窗口
    QVector<NormalTabStatckItem> m_childWndItems;
};

//集合竞价
class CallAuctionChildWnd: public NormalChildWnd
{
    Q_OBJECT
public:
    explicit CallAuctionChildWnd(QWidget *parent = nullptr, QString instName = "");
protected:
    //获取当前窗口创建数据
    virtual void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);
    //创建顶部窗口
    virtual BaseWidget* createTopWnd();
};

//涨停监测
class LimitUpMonitorChildWnd: public NormalChildWnd
{
    Q_OBJECT
public:
    explicit LimitUpMonitorChildWnd(QWidget *parent = nullptr, QString instName = "");
protected:
    //获取当前窗口创建数据
    virtual void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);
    //创建顶部窗口
    virtual BaseWidget* createTopWnd();
};

//龙虎榜单
class WinnersListChildWnd: public NormalChildWnd
{
    Q_OBJECT
public:
    explicit WinnersListChildWnd(QWidget *parent = nullptr, QString instName = "");

    void afterActive();

    //void afterDeactive();

    qint64 getTime();
protected:
    //获取当前窗口创建数据
    virtual void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);

    //创建顶部窗口
    virtual BaseWidget* createTopWnd();
protected slots:
    //tab切换事件
    virtual void slotsTabSwtichWnd(int id);
    void slotsTimeSelected(qint64 time);
    void slotsSwitchYybDetail(const QString& id);
private:
    qint64 m_currTime;
    LhbStockView * m_lhbStockView;
    LhbYybDrbView *m_lhbYybDrbView;
};

//资金异动页面
class FundAbnormalChildWnd: public NormalChildWnd
{
    Q_OBJECT
public:
    explicit FundAbnormalChildWnd(QWidget *parent = nullptr, QString instName = "");
protected:
    //获取当前窗口创建数据
    virtual void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);
    //创建顶部窗口
    virtual BaseWidget* createTopWnd();
private:
    BaseWidget* m_topWnd;
};

//龙虎榜营业部详情页面
class WinnersListSalesOfficeChildWnd:public DatacenterBaseWnd
{
    Q_OBJECT
public:
    explicit WinnersListSalesOfficeChildWnd(QWidget *parent = nullptr, QString instName = "");

    void setSalesOfficeId(const QString& id);

    void afterActive();

    void afterDeactive();

    void createWg();
private:
    void onDataCallBack(const InfoRet& id,const StockYybDetail& data);
private:
    QLabel* m_pLabelSoName;
    InfoReqId m_id;
    QString m_currSalesOfficeId;
    LhbYybHoView* m_yybHoView;
    LhbYybSbView *m_yybSbView;
};


//大宗交易
class SecuritiesMarginTradingChildWnd:public DatacenterBaseWnd
{
    Q_OBJECT
public:
    explicit SecuritiesMarginTradingChildWnd(QWidget *parent = nullptr, QString instName = "");

    void afterActive();

    void afterDeactive();

    void createWg();
protected slots:
    void slotsCalandarBarClicked();
private:
    qint64 m_reqId;
    QWidget* createTimeTableWnd();
    QWidget* createDzjyListWnd();
    //时间变化处理，通知列表
    void proceTimeChange();
    //大宗交易日历回调
    void onStockDzjyDateListCall(const InfoRet& ret,const StockDzjyDateList& data);
private:
    StockDzjyDateList m_stockDzjyDateList;
    qint64 m_currTime;
    CalandarBarMaster *mCalandar;
    StockDzjyView  *m_stkDzjyView;
};

//融资融券
class BlockTradeChildWnd: public NormalChildWnd
{
    Q_OBJECT
public:
    explicit BlockTradeChildWnd(QWidget *parent = nullptr, QString instName = "");

    void afterActive();

    void afterDeactive();

    qint64 getTime();
protected slots:
    //双击跳转到融资融券详情
    void slotsSwitchRzrqDetail(const QString& id);
    //双击跳转到板块成分股
    void slotsSwitchRzrqBkStockList(const QString& bkCode);
protected:
    //获取当前窗口创建数据
    void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);
    //创建顶部窗口
    BaseWidget* createTopWnd();
    //创建日历控件
    QWidget* createCalandarBar();
    //创建两市融资融券曲线图
    BaseWidget* createRzrqHistoryChartWnd();
    //创建融资融券净买入额前10
    QWidget* createTheNetBuyingList();
    //创建融券融券净卖出额前10
    QWidget* createTheNetSellingList();
    //请求融资融券历史数据
    void reqStockDatas();
private:
    //融资融券详情也
    void onStocDatasCallBack(const InfoRet& ret,const StockMarketRzrqList& datas);
    //融资融券日历回调
    void onStockRzrqDateListCall(const InfoRet& ret,const StockRzrqDateList& data);
    //时间变化处理，通知列表
    void proceTimeChange();
private:
    SmtCurveGraphCanvas* m_smtCurveWnd;
    StockMarketRzrqList m_datas;
    StockRzrqDateList m_stockDateList;
    qint64 m_reqId;
    qint64 m_listReqId=-1;
    qint64 m_currTime;
    CustomCalandarBtnMarster *mCalandarBtn;
    StockRzrqRankTableView *m_stockRzrqNetInputView,*m_stockRzrqNetOutputView;
    StockRzrqView *m_stkRzrqview;
    BlockRzrqView *m_bkRzrqView;
    SmtLsGraphTitle* m_smtTitle;
};


//融资融券详情页
class BlockTradeDetailChildWnd:public DatacenterBaseWnd
{
    Q_OBJECT
public:
    explicit BlockTradeDetailChildWnd(QWidget *parent = nullptr, QString instName = "");

    void afterActive();

    void afterDeactive();

    void createWg();

    void setCurrTime(qint64 currTime);
protected slots:
    void slotsDataListChanged();
private:
    void OnStockCodeChange();
    //创建数据列表
    QWidget* createStockDataList();

private:
    BlockTradeTopWnd   *m_topWnd;
    StockRzrqDetailView *m_stkRzrqDetailView;
};



//融资融券板块成分股展示
class BlockTradeBkAndStocksChildWnd:public DatacenterBaseWnd
{
    Q_OBJECT
public:
    explicit BlockTradeBkAndStocksChildWnd(QWidget *parent = nullptr, QString instName = "");

    void createWg();

    void setCurrTime(qint64 currTime);

    void afterActive();

    void afterDeactive();
signals:
    //板块数据列表单击事件
    void signalListCategoryCodeChange(const QString& bkCode);
    void signalBkChildListChanged(const QString& stock);
protected slots:
    void slotsHideBottomWnd(bool bHide);
    //单击板块列表表事件
    void slotsCategoryWndItemSelected(const QString &stockCode);  //选中某股票

    //设置板块成分股个数
    void bkChilCountChanged(int count);
private:
    //创建板块列表
    QWidget* createBkList();
    //创建成分股列表
    QWidget* createChildList();
    //创建按钮按钮和成分股描述文本标题栏
    QWidget* createBottomBar();

private:
    QLabel* m_childListDesc;
    int m_childCount;
    QString m_bkname;
    qint64 m_currTime=-1;

    BlockRzrqView          *m_rzrqBlockView;
    BlockStockRzrqView *m_rzrqBkStockView;
};

//新股日历
class NewStockCalendarChildWnd: public NormalChildWnd
{
    Q_OBJECT
public:
    explicit NewStockCalendarChildWnd(QWidget *parent = nullptr, QString instName = "");

    void afterActive();

    void afterDeactive();

    //创建流程
    void createWg();
protected:
    //获取当前窗口创建数据
    void getChildWndItems(QVector<NormalTabStatckItem>& list,int& currTab);
    //创建个股详细列表
    void createTopWndEx();
    //创建日历控件
    QWidget* createCalandarBar();
    //日历控件时间发生切换处理
    void proceTimeChange();
protected slots:
    //日历选择控件
    void slotsCalandarBarClicked();
private:
    //日历回调
    void onStockDateListCall(const InfoRet ret,const NewStockCalandars& data);
private:
    qint64 m_currTime;
    ChildWndItem m_topItem;
    //日历控件
    CalandarBarMaster *mCalandar;

    //新股申购-顶部
    XgrlTableView  *m_xgrlXgsg;
    //日历数据
    NewStockCalandars m_dataList;
    //日历请求id
    int m_reqDateListId = -1;
};

//今日申购页面
class  XgrlJrsgWidget : public BaseWidget
{
    Q_OBJECT
public:
    XgrlJrsgWidget(QWidget *parent = 0,QString instName="XgrlJrsgWidget");

    void afterActive();
    void afterDeactive();

signals:
  void signalSWitchDetailPage(const QString &);

private:
    void createChildWndItems();
    CSubTab *createTab(const QString& title);

    QVector<NormalTabStatckItem> m_NormalItems;
};

#endif // DATACENTERBASEWND_H
