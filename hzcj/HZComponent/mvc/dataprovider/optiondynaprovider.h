/********************************************************************
created:
author: dk.zhang
purpose:自选股和历史浏览行情数据管理
*********************************************************************/
#ifndef OPTIONDYNAPRIVIDER_H
#define OPTIONDYNAPRIVIDER_H

#include "stockdynaprovider.h"


//分类查询股票数据管理类，比如自选股，历史浏览和相关板块等
class OptionDynaProvider : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    OptionDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0);
    virtual  ~OptionDynaProvider();

    void actived(const bool &active = true);

signals:
    void dataChanged();
    void rowCountChanged(int count);

protected:
    void startSubOptionStocks();
    void reSubOptionStocks();

    virtual QVector<QString> getStocks() = 0;

    void  onStockOptionalDataArrived(ReqId reqId,char *data,int num,ErrCode );
protected:
    //当前股票数
    int                               m_rowCount;
};

//自选，分为游客和用户
class ZXStockDynaProvider  : public OptionDynaProvider
{
    Q_OBJECT
public:
    ZXStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0);

    void deleteStock(const QString &stockCode);

protected slots:
    void  slotStkCountChanged(int count);

protected:
    QVector<QString> getStocks();

    QString  m_currentDeletingStock;
};

//历史浏览
class HistStockDynaProvider  : public OptionDynaProvider
{
public:
    HistStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0);

protected:
    QVector<QString> getStocks();

};

//重要指数
class ImpIndexDynaProvider : public OptionDynaProvider
{
public:
    ImpIndexDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0);

protected:
    QVector<QString> getStocks();

private:
    QVector<QString>  m_indexs;
};

//相关板块列表
class RelationBlockListProvider: public OptionDynaProvider
{
    Q_OBJECT
public:
    RelationBlockListProvider(AbstractBaseStockModel *model,QObject *parent = 0);

    void setCurrStock(const QString& stock);

protected:
  QVector<QString> getStocks();

protected:
    QString         m_currStock;
};

#endif // OPTIONDYNAPRIVIDER_H
