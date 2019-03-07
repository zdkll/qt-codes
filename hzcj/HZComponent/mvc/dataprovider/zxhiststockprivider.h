/********************************************************************
created:
author: dk.zhang
purpose:自选股和历史浏览行情数据管理
*********************************************************************/
#ifndef ZXHISTSTOCKPRIVIDER_H
#define ZXHISTSTOCKPRIVIDER_H

#include "stockdynaprovider.h"

class HZAuthenOpt;
class LocalStockSource;
///自选股、历史浏览基类
class ZXHistStockDynaProvider : public QObject
{
    Q_OBJECT
public:
    ZXHistStockDynaProvider(StockBaseItemModel *model,QObject *parent = 0);
    virtual  ~ZXHistStockDynaProvider();

    void actived(const bool &active = true);

signals:
    void dataChanged();

    void rowCountChanged(int count);

protected:
    void onStockOptionalDataArrived(ReqId reqId,const QVector<AllStock > &stocks,ErrCode errCode);

    virtual void subOptionalStock() = 0;

    void cancelSub();

protected:
    StockBaseItemModel *m_model;
    ReqId                          m_subReqId;
    QVector<StockCode>    m_stockTable;

    int                               m_rowCount;
};


//自选，分为游客和用户
class ZXStockDynaProvider  : public ZXHistStockDynaProvider
{
public:
    ZXStockDynaProvider(StockBaseItemModel *model,QObject *parent = 0);

    void subOptionalStock();

protected:
    void onStockOptionalDataArrived(ReqId reqId,const QVector<AllStock > &stocks,ErrCode errCode);

private:
   QVector<QString> getZXStocks();

  HZAuthenOpt         *m_authenOpt;        //用户自选股
  LocalStockSource  *m_zxStockSource; //本地游客自选股
};

//历史浏览
class HistStockDynaProvider  : public ZXHistStockDynaProvider
{
public:
    HistStockDynaProvider(StockBaseItemModel *model,QObject *parent = 0);

    void subOptionalStock();
private:
      LocalStockSource  *m_histStockSource;
};

#endif // ZXHISTSTOCKPRIVIDER_H
