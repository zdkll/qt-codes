#ifndef QUOTELISTPAGEMAINWND_H
#define QUOTELISTPAGEMAINWND_H
/********************************************************************
created:
author: min.cheng
purpose: 行情页面左侧列表展示窗口
*********************************************************************/
#include "basewidget.h"
#include <QDebug>
#include <QMap>
#include "hzchildwndmgr.h"

class CSubTab;
class DetailPageParam;
class QuoteListPanelWnd;
class QuoteListPageMainWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit QuoteListPageMainWnd(QWidget *parent = nullptr, QString instName = "");

    ~QuoteListPageMainWnd();

    //取消激活
    void afterDeactive();

    void afterActive();

    //窗口创建
    void createWg();

    //设置右侧盘口
    void setRightWnd(QuoteListPanelWnd* rightWnd);

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
signals:
    //股票点击事件
    void signalsClickObjItem(const QString& obj);
    void signalsDbClickObjItem(const QString& obj);
private slots:
    //窗口切换
    void slotsSwtichWnd(int id);
private:
    //创建标题导航 自选，最近浏览，自选股资讯
    QWidget* createTopTitle();
    //创建中间页面页面
    QWidget* createMidWnd();
    //创建底部导航栏
    QWidget* createBottomWnd();
    //显示和隐藏盘口
    void hideRightPanel(bool bHide);

    //创建沪深综合页面
    ChildWndItem createHszhInnerWnd();
    //创建沪深A股 上证A股 深证A股 沪深风险警示 中小板 创业板
    ChildWndItem createQuoteListWnd();
    //创建重要指数
    ChildWndItem createImpIndexListWnd();
    //创建全部指数
    ChildWndItem createIndexListWnd();
private:
    QMap<int,int> m_typeIndexMap; //key 页面类型  value stackWidget下标
    QMap<int,int> m_typeFilterMap; //key 页面类型  value filter
    CSubTab* m_topSub;
    CSubTab* m_bottomSub;
    QuoteListPanelWnd* m_rightPwd;
    //子页面管理器
    HzChildWndMgr<QuoteListPageMainWnd>* m_pMainChildWndMgr = nullptr;
};



#endif // QUOTELISTPAGEMAINWND_H
