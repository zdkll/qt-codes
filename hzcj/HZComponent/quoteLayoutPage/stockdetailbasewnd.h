#ifndef STOCKDETAILBASEWND_H
#define STOCKDETAILBASEWND_H
/********************************************************************
created:
author: min.cheng
purpose: 个股详情页面小窗口基类
*********************************************************************/
#include <QMap>
#include "basewidget.h"
#include "basestackwidget.h"
#include "subtab.h"
#define QuoteLeftWndWidth 210
#define QuoteRightWndWidth 350
#define QuoteBottomWndHight 186
#define QuoteBottomTabWndHight 26

struct StockDetailSubItem
{
    int id;
    QString name;
};

class StockDetailBaseWnd: public BaseStackWidget
{
  Q_OBJECT
public:
    explicit StockDetailBaseWnd(QWidget *parent = 0,QString instName="");

    virtual void hideWnd(bool bHide);
signals:
    //内部主动改变合约，通知外部
    void signalObjChange(const QString& obj);
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    virtual void slotsObjChange(const QString& obj);
};


enum InfoWndTabType
{
    NewsTab = 0,  //新闻
    AnncTab,      //公告
    ReportTab,    //研报
    BlockTab,     //成分股
};

//详情页底部窗口
class StockBaseDetailInfoWnd: public StockDetailBaseWnd
{
  Q_OBJECT
public:
    explicit StockBaseDetailInfoWnd(QWidget *parent = 0,QString instName="");

    //窗口创建
    void createWg();

    int getCurrTabId();

    //返回TAB数据项
    virtual void getStockDetailSubItemList(QVector<StockDetailSubItem>& list){}
public slots:
    //切换子页面
    virtual void slotsSwitchWnd(int id);
    //处理成分股双击事件
    void slotsChildStockListDbClick(const QString &stockCode);
protected:
    //创建body
    virtual QWidget* createBody(){return nullptr;}
protected:
    QMap<InfoWndTabType,int> m_indexMap;
    int m_currTabId;

};

//详情页左边窗口
class StockBaseDetailListWnd: public StockDetailBaseWnd
{
  Q_OBJECT
public:
    explicit StockBaseDetailListWnd(QWidget *parent = 0,QString instName="");

    //窗口创建
    void createWg();
public slots:
    //tab切换
    virtual void slotsSwitchWnd(int id){}
    virtual void slotsButtonClicked(int nId){}

    virtual void slotsSwitchUpDownStock(bool  ,int  ){}
protected:
    //创建标题
    virtual QWidget *createTitle();
    //table
    virtual CSubTab* createSubAndItems(){return nullptr;}
    //创建body
    virtual QWidget* createBody(){return nullptr;}
protected:
    //标题tab
    CSubTab* m_table;
};

#endif // STOCKDETAILBASEWND_H
