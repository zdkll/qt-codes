#ifndef DATACENTERPAGEMAINWND_H
#define DATACENTERPAGEMAINWND_H
/********************************************************************
created:
author: min.cheng
purpose: 数据页面主窗口布局
*********************************************************************/
#include "datacenterbasewnd.h"
#include "hzchildwndmgr.h"
#include <QMap>
class CSubTab;
class DetailPageParam;
class WinnersListSalesOfficeChildWnd;
class DataCenterPageMainWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit DataCenterPageMainWnd(QWidget *parent = nullptr, QString instName = "");

    ~DataCenterPageMainWnd();

    //取消激活
    void afterDeactive();

    void afterActive();

    //窗口创建
    void createWg();

    //代替页面处理接收到的消息
    void recvParam(const DetailPageParam &param,bool bCurrPage = false);

    void onNetIoReady(bool ready);
private slots:
    //窗口切换
    void slotsSwtichWnd(int id);
    //股票改变
    void slotsStockObjChanged(const QString& stock);
    //双击跳转到详情页面
    void slotsSWitchDetailPage(const QString& stock);
private:
    //根据权限返回不同页面
    int getPageFromPrmission(int page);
    //顶部TAB
    QWidget* createSubTab();
    //创建子页面
    QWidget* createMidWnd();

    //集合竞价
    ChildWndItem createCallAuctionWnd();
    //相似K线
    ChildWndItem createSimilarKlineWnd();
    //涨停监测
    ChildWndItem createLimitUpMonitorWnd();
    //龙虎榜
    ChildWndItem createWinnersListWndWnd();
    //资金异动
    ChildWndItem createFundAbnormalWnd();
    //新股日历
    ChildWndItem createNewStockCalendarWnd();
    //融资融券
    ChildWndItem createBlockTradeWnd();
    //大宗交易
    ChildWndItem createSecuritiesMarginTradingWnd();
    //龙虎榜详情页面
    ChildWndItem createWinnersListSalesOfficeDetailWnd();
    //融资融券详情
    ChildWndItem createBlockTradeDetailWnd();
    //融资融券板块成分股
    ChildWndItem createBlockTradeBkAndStocksWnd();

    //集合竞价无权限
    ChildWndItem createCallAuctionNoPermissionWnd();
    //涨停监测无权限
    ChildWndItem createLimitUpMonitorNoPermissionWnd();
    //相似K线无权限
    ChildWndItem createSimilarKlineNoPermissionWnd();
private:
    DetailPageParam* m_pLastCmdParam = nullptr; //记录上次节目请求数据。
    QMap<int,int> m_typeTabIdMap; //key 页面类型 value tab值  页面类型和tab对应表
    CSubTab* m_topSub;
    //左侧窗口映射表
    struct RightPanel
    {
        int index;
        int width;
    };
    QMap<int,RightPanel> m_rightPanelMap;
    QStackedWidget* m_pRightStackWnd = nullptr;
    //营业部详情
    WinnersListSalesOfficeChildWnd* m_salesOfficeDetailWnd = nullptr;
    //龙虎榜
    WinnersListChildWnd * m_winnerListWnd = nullptr;
    //融资融券主页
    BlockTradeChildWnd* m_blockTradeChildWnd = nullptr;
    //融资融券详细页面
    BlockTradeDetailChildWnd* m_blockTradeDetailChildWnd = nullptr;
    //融券融券板块成分股列表
    BlockTradeBkAndStocksChildWnd * m_blockTradeBkAndStockChildWnd = nullptr;
    //子页面管理器
    HzChildWndMgr<DataCenterPageMainWnd>* m_pMainChildWndMgr = nullptr;
};

#endif // DATACENTERPAGEMAINWND_H
