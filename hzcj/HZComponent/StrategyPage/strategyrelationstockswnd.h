/********************************************************************
created:
author: min.cheng
purpose: 策略相关股票窗口
*********************************************************************/
#ifndef STRATEGYRELATIONSTOCKSWND_H
#define STRATEGYRELATIONSTOCKSWND_H
#include <QPainter>

#include "imagelayer.h"
#include "strategydynaprovider.h"
#include "basewidget.h"
#include "datamodeldef.h"

class StrategyRelationStocksInnerWnd;
class ViewTipsPanel;

class StocksItemDrawer
{
public:
    StocksItemDrawer(StrategyRelationStocksInnerWnd* pWnd=nullptr);

    ~StocksItemDrawer();

    void update(const HZData::AllStock& stock,const HZData::StockMoney& fund,const QRect rc,QPainter& dc);
    void update(const HZData::AllStock& stock,const HZData::StockMoney& fund,QPainter& dc);
private:
    void redraw(QPainter& dc);
private:
    StrategyRelationStocksInnerWnd* m_pMainWnd;
    QRect m_rc;
    HZData::AllStock m_stock;
    HZData::StockMoney m_fund;
};

class StrategyRelationStocksInnerWnd :public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor m_bkClr READ getBkClr WRITE setBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_itemBkClr READ getItemBkClr WRITE setItemBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_upClr READ getUpClr WRITE setUpClr DESIGNABLE true)
    Q_PROPERTY(QColor m_downClr READ getDownClr WRITE setDownClr DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClr READ getEqualClr WRITE setEqualClr DESIGNABLE true)
    Q_PROPERTY(QColor m_stockCodeAndNameClr READ getStockCodeAndNameClr WRITE setStockCodeAndNameClr DESIGNABLE true)
    Q_PROPERTY(QColor m_stockFiledClr READ getStockFiledClr WRITE setStockFiledClr DESIGNABLE true)
public:
    explicit StrategyRelationStocksInnerWnd(QMap<QString,HZData::StockMoney>& fundMap,QWidget *parent = 0);

    void setDynaProvider(DynaDataProvider* pProvider);

    void setStockList(const QVector<QString>& stockList);

    void refresh();

    void createWg();

    //图层背景颜色
    QColor getBkClr() const{return m_bkClr;}
    void setBkClr(QColor color){m_bkClr=color;}
    QColor m_bkClr = QColor("#191919");

    //股票绘制项背景颜色
    QColor getItemBkClr() const{return m_itemBkClr;}
    void setItemBkClr(QColor color){m_itemBkClr=color;}
    QColor m_itemBkClr = QColor(Qt::red);

    //上涨颜色
    QColor getUpClr() const{return m_upClr;}
    void setUpClr(QColor color){m_upClr=color;}
    QColor m_upClr = QColor(Qt::red);

    //下跌颜色
    QColor getDownClr() const{return m_downClr;}
    void setDownClr(QColor color){m_downClr=color;}
    QColor m_downClr = QColor(Qt::red);

    //平局颜色  （不涨不跌）
    QColor getEqualClr() const{return m_equalClr;}
    void setEqualClr(QColor color){m_equalClr=color;}
    QColor m_equalClr = QColor(Qt::red);

    //股票代码颜色
    QColor getStockCodeAndNameClr() const{return m_stockCodeAndNameClr;}
    void setStockCodeAndNameClr(QColor color){m_stockCodeAndNameClr=color;}
    QColor m_stockCodeAndNameClr = QColor(Qt::red);

    //股票字段颜色
    QColor getStockFiledClr() const{return m_stockFiledClr;}
    void setStockFiledClr(QColor color){m_stockFiledClr=color;}
    QColor m_stockFiledClr = QColor(Qt::red);
private:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *event);
    void redraw();
    int getrow();
    QRect getRc();
private:
    DynaDataProvider* m_pProvider = nullptr;
    QVector<QString> m_stockList;
    ImageLayer* m_pImageLayer = nullptr;
    int m_layerType = 0;
    int m_stockAreaType = 0;
    QVector<StocksItemDrawer> m_stockItems;
    BaseWidget* m_pStocksWnd = nullptr;
    QMap<QString,HZData::StockMoney>& m_fundMap;
};

class StrategyRelationStocksWnd :public BaseWidget
{
    Q_OBJECT
public:
    explicit StrategyRelationStocksWnd(QWidget *parent = 0);

    void setDynaProvider(DynaDataProvider* pProvider);

    void afterActive();

    void afterDeactive();

    void setStockList(int code,const QVector<QString>& stockList);
signals:

public slots:
    void slotsTimer(); //定时刷新
private:
    void createWg();
    void onOptionalFundArrived(ReqId , char *data,int num, ErrCode errCode);
private:
    int m_subFundReq = -1;
    StrategyRelationStocksInnerWnd* m_pConentWnd = nullptr;
    ViewTipsPanel* m_pInfoTipsPanel = nullptr;
    DynaDataProvider* m_pProvider = nullptr;
    QVector<QString> m_stockList;
    QMap<QString,HZData::StockMoney> m_funMap;
};

#endif // STRATEGYRELATIONSTOCKSWND_H
