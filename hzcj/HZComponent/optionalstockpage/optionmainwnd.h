#ifndef OPTIONMAINWND_H
#define OPTIONMAINWND_H
/********************************************************************
created:
author: min.cheng
purpose: 自选股页面中的左侧的主窗口，自选股列表，最近浏览，自选股资讯，智能盯盘，自选股周报
         页面布局，消息联动处理
*********************************************************************/
#include "basewidget.h"
#include "hzchildwndmgr.h"
#include <QDebug>

class DetailPageParam;
class OptionMainInnerWnd;
class CSubTab;

class OptionMainWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit OptionMainWnd(QWidget *parent = nullptr, QString instName = "");

    ~OptionMainWnd();

    //取消激活
    void afterDeactive();

    void afterActive();

    //窗口创建
    void createWg();

    void setRightPnaleWnd(BaseWidget* pPanel);

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
signals:
    //股票点击事件
    void signalsClickObjItem(const QString& obj);
    void signalsDbClickObjItem(const QString& obj);
private slots:
    //窗口切换
    void slotsSwtichWnd(int id);

    void onSetBtnClicked();
private:
    //创建标题导航 自选，最近浏览，自选股资讯
    QWidget* createTopTitle();
    //创建中间页面页面
    QWidget* createMidWnd();
    //创建底部导航栏
    QWidget* createBottomWnd();

    //自选股页面创建
    ChildWndItem createOptionWnd();
    //最近浏览页面创建
    ChildWndItem createRecentBrowseWnd();
    //自选股资讯页面创建
    ChildWndItem createOptionInfoWnd();
    //智能盯盘页面创建
    ChildWndItem createOptionSmartMonitorWnd();
    //创建自选股周报
    ChildWndItem createOptionWeeklyNewspaperWnd();

private:
    CSubTab* m_topSub;
    CSubTab* m_bottomSub;
    BaseWidget* m_rightPanel;
    //子页面管理器
    HzChildWndMgr<OptionMainWnd>* m_pMainChildWndMgr = nullptr;
};

#endif // OPTIONMAINWND_H
