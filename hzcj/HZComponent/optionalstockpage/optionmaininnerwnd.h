#ifndef OPTIONMAININNERWND_H
#define OPTIONMAININNERWND_H
/********************************************************************
created: 自选股列表，最近浏览，自选股资讯，智能盯盘，自选股周报具体页面实现
author: min.cheng
purpose:
*********************************************************************/
#include <QStackedWidget>
#include <QMap>
#include "drawmaster.h"
#include "pubmsgdef.h"
#include "hzchildwndmgr.h"

class CSubTab;
class weeklys;

struct ListInnerWndItem
{
    QWidget* pWnd = nullptr;
    HZObject* pObj = nullptr;
};

//内部窗口基类
class OptionMainInnerWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit OptionMainInnerWnd(QWidget *parent = nullptr, QString instName = "");

    virtual ~OptionMainInnerWnd(){}

    //窗口创建
    virtual void createWg(){}

    //是否有 BtChildWndType类型的子窗口
    virtual bool bShowBtChildWnd(){return true;}

    //是否显示右侧盘口
    virtual bool bShowRightPanelWnd(){return true;}

    virtual int currType(){return 0;}

    virtual void switchChildWnd(int type,bool bActive){}
signals:
    //股票点击事件
    void signalsClickObjItem(const QString& obj);
    void signalsDbClickObjItem(const QString& obj);
public slots:
    virtual void slotsSwtichChildWnd(int type){}
};

//列表样式展示
class OptionListInnerWnd:public OptionMainInnerWnd
{
  Q_OBJECT
public:
    explicit OptionListInnerWnd(QWidget *parent = nullptr, QString instName = "");

    //窗口创建
    void createWg();

    //取消激活
    void afterDeactive();

    void afterActive();

    int currType();

    void switchChildWnd(int type,bool bActive);
signals:

public slots:
    virtual void slotsSwtichChildWnd(int type);
protected:
    //创建基础行情列表
    virtual ListInnerWndItem createBaseQuoteList();
    //创建资金流向列表
    virtual ListInnerWndItem createFundFlowList();
    //创建DDE决策
    virtual ListInnerWndItem createDDEList();

    //创建基础行情列表
    ChildWndItem createBaseQuoteListEx();
    //创建资金流向列表
    ChildWndItem createFundFlowListEx();
    //创建DDE决策
    ChildWndItem createDDEListEx();
private:
    //子窗口统一管理
    HzChildWndMgr<OptionListInnerWnd>* m_pMainChildWndMgr = nullptr;
};

//自选股列表
class OptionListWnd:public OptionListInnerWnd
{
  Q_OBJECT
public:
    explicit OptionListWnd(QWidget *parent = nullptr, QString instName = "");

protected:
    //创建基础行情列表 绑定基类的点击和双击事件
    ListInnerWndItem createBaseQuoteList();
    //创建资金流向列表 绑定基类的点击和双击事件
    ListInnerWndItem createFundFlowList();
    //创建DDE决策 绑定基类的点击和双击事件
    ListInnerWndItem createDDEList();
};

//最近浏览
class OptionRecentBrowseWnd:public OptionListInnerWnd
{
  Q_OBJECT
public:
    explicit OptionRecentBrowseWnd(QWidget *parent = nullptr, QString instName = "");
protected:
    //创建基础行情列表 绑定基类的点击和双击事件
    ListInnerWndItem createBaseQuoteList();
    //创建资金流向列表 绑定基类的点击和双击事件
    ListInnerWndItem createFundFlowList();
    //创建DDE决策 绑定基类的点击和双击事件
    ListInnerWndItem createDDEList();
};

//自选股资讯
class NaviZXListView;
class NewsMarster;
class OptionInfoWnd: public OptionMainInnerWnd
{
    Q_OBJECT
public:
    explicit OptionInfoWnd(QWidget *parent = nullptr, QString instName = "");

    //窗口创建
    void createWg();

    //是否有 BtChildWndType类型的子窗口
    bool bShowBtChildWnd(){return false;}

    //是否显示右侧盘口
    bool bShowRightPanelWnd(){return false;}
    //~
    void afterActive();
    void afterDeactive();

private:
    NaviZXListView *mZxView;
    NewsMarster *mNewWnd;
};

#define CompanyEventWndId 0
#define AbnormalChangesWndId 1

//智能盯盘
class OptionSmartMonitorWnd: public OptionMainInnerWnd
{
    Q_OBJECT
public:
    explicit OptionSmartMonitorWnd(QWidget *parent = nullptr, QString instName = "");

    //窗口创建
    void createWg();

    //是否有 BtChildWndType类型的子窗口
    bool bShowBtChildWnd(){return false;}

    //是否显示右侧盘口
    //bool bShowRightPanelWnd(){return false;}

    void afterActive();

    void afterDeactive();

private slots:
    //窗口盘中异动和公司大事
    void slotsSwtichWnd(int id);
private:
    void OnStockCodeChange();
    //创建右侧自选列表
    ListInnerWndItem createLeftOptionListWnd();
    //创建盘中异动窗口
    ChildWndItem createAbnormalChangesWndEx();
    //创建公司大事窗口
    ChildWndItem createCompanyEventWndEx();
private:
    CSubTab* m_topSub;
    NaviZXListView *mZxView;
    DrawMaster *mSmartPanel{nullptr}, *mCompanyEvent{nullptr};
    //盘中异动，公司大事窗口管理
    HzChildWndMgr<OptionSmartMonitorWnd>* m_pMainChildWndMgr = nullptr;
};

//自选股周报 weekly newspaper
class OptionWeeklyNewspaperWnd: public OptionMainInnerWnd
{
    Q_OBJECT
public:
    explicit OptionWeeklyNewspaperWnd(QWidget *parent = nullptr, QString instName = "");

    //窗口创建
    void createWg();

    void afterActive();

    void afterDeactive();

    //是否有 BtChildWndType类型的子窗口
    bool bShowBtChildWnd(){return false;}

    //code changed
    void OnStockCodeChange();

    //是否显示右侧盘口
    //bool bShowRightPanelWnd(){return false;}

private:
    weeklys *m_weeklys;
};

#endif // OPTIONMAININNERWND_H
