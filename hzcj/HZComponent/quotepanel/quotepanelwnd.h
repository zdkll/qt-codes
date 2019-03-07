#ifndef QUOTEPANELWND_H
#define QUOTEPANELWND_H
/********************************************************************
created:
author: min.cheng
purpose: 右侧盘口窗口 根据合约类型自动切换到不同的盘口，统一管理 股票详情盘口 板块详情盘口
         指数详情盘口  股票列表右侧盘口 板块列表右侧盘口 指数列表右侧盘口 。。。。
*********************************************************************/
#include <QMap>
#include "hzchildwndmgr.h"
#include "basewidget.h"
enum QuoteRightPanelType
{
    QuotepanelTypeUnknown = 0,
    stock_detail_right_panel, //股票详情盘口
    block_detail_right_panel, //板块详情盘口
    index_detail_right_panel, //指数详情盘口
    index_nomal_detail_right_panel, //非重要指数详情盘口
    stock_list_right_panel,   //股票列表右侧盘口
    //block_list_right_panel,   //板块列表右侧盘
    index_list_right_panel    //指数列表右侧盘口
};

//右侧盘口基类
class QuotePanelWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit QuotePanelWnd(QWidget *parent = nullptr, QString instName = "");

    ~QuotePanelWnd();

    virtual void createWg();

    //设置默认显示的盘口类型
    void setDefaultType(QuoteRightPanelType defaultType);

    void afterActive();

    void afterDeactive();

    virtual void hideWnd(bool bHide);

    //创建股票详情盘口
    ChildWndItem createStockDetailPanel();
    //创建板块详情盘口
    ChildWndItem createPlateDetailPanel();
    //创建指数详情盘口
    ChildWndItem createIndexDetailPanel();
    //股票列表右侧盘口
    ChildWndItem createStockListPanel();
    //指数列表右侧盘口
    ChildWndItem createIndexListPanel();
    //非重要指数右侧盘口
    ChildWndItem createNonImpIndexListPanel();
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
protected:
    void OnStockCodeChange();
    //根据合约代码返回不同的类型
    virtual QuoteRightPanelType convertTypeByObjCode(const QString& objCode){return QuotepanelTypeUnknown;}
protected:
    HzChildWndMgr<QuotePanelWnd>* m_childWndMgr;
    QuoteRightPanelType m_defaultType =  QuotepanelTypeUnknown;
    QMap<QuoteRightPanelType,int> m_typeIndexMap;
};


//详情页右侧盘口类型
class QuoteDetailPanelWnd:public QuotePanelWnd
{
    Q_OBJECT
public:
    explicit QuoteDetailPanelWnd(QWidget *parent = nullptr, QString instName = "");

    void createWg();

    void showTickWnd();
protected:
    //根据合约代码返回不同的类型
    QuoteRightPanelType convertTypeByObjCode(const QString& objCode);
    QMap<QString,QString> m_impStockMap;
};

//列表页右侧盘口
class QuoteListPanelWnd:public QuotePanelWnd
{
    Q_OBJECT
public:
    explicit QuoteListPanelWnd(QWidget *parent = nullptr, QString instName = "");

    void createWg();
protected:
    //根据合约代码返回不同的类型
    QuoteRightPanelType convertTypeByObjCode(const QString& objCode);
};




#endif // QUOTEPANELWND_H
