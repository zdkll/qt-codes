/********************************************************************
created: 2018/06
author: dk.zhang
purpose: 行情列表主窗口，固定前几列
*********************************************************************/
#ifndef STOCKDYNAVIEW_H
#define STOCKDYNAVIEW_H

#include "stockdynabaseview.h"

#include "hzcomponent_global.h"

//个股\板块\指数 显示主窗口-统一的行为
class   StockDynaView : public StockDynaBaseView
{
    Q_OBJECT
public:
    enum StockDynaType
    {
        StockDyna = 0, //个股
        BlockDyna = 1,//板块
        IndexDyna = 2,//指数
        BKStockDyna = 3, //只显示板块成分股
        IndexStockDyna  = 4  //指数成分股
    };
    StockDynaView(const StockDynaType &type, QWidget *parent = 0);

protected:
    virtual void modelReset();
    int stockDynaType(){return m_stkDynaType;}
protected slots:
    void slotEnteredItem(const QModelIndex &index);

private:
    AbstractBaseStockModel  *createModel(const StockDynaType &type);
    BaseStockDynaProvider   *createStockDynaProvider(AbstractBaseStockModel  *,const StockDynaType &type);

    QHash<StockDynaType,HZData::StockFieldMask>   m_fieldHash;
    StockDynaType   m_stkDynaType;
};


class  BKAndStockView : public StockDynaBaseView
{
public:
    BKAndStockView(QWidget *parent = 0);

protected:
    virtual void modelReset();
    int stockDynaType(){return 6;}
};

#endif // STOCKDYNAVIEW_H
