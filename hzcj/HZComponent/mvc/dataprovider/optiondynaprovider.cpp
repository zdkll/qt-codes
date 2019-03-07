#include "optiondynaprovider.h"

#include "datacontroller.h"

#include "hzuserdatamgr.h"
#include "zxstockopt.h"
#include "httpdata.h"

#include <QObject>
#include <QDebug>


///ZXStockDynaProvider-----
OptionDynaProvider::OptionDynaProvider(AbstractBaseStockModel *model,QObject *parent)
    :BaseStockDynaProvider(model,parent) ,m_rowCount(0)
{

}

OptionDynaProvider::~OptionDynaProvider()
{
    cancelOptSub();
}

void OptionDynaProvider::actived(const bool &active)
{
    m_actived = active;
    cancelOptSub();
    if(active)
        startSubOptionStocks();
    else
        m_model->clearData();
}

void OptionDynaProvider::startSubOptionStocks()
{
    QVector<QString> stocks = getStocks();

    //1 初始化列表
    int num = stocks.size();
    if(m_rowCount != num){
        m_rowCount = num;
        emit rowCountChanged(m_rowCount);
    }
    QVector<HZData::StockCode>  stockCodes(num);
    for(int i=0;i<num;i++)
        stockCodes[i].stock_code = stocks[i].toStdString();

    m_model->initData(stockCodes.data(),num);

    if(stocks.size()<1)  return;

    subOptionalData(stockCodes.data(),num);
}

void OptionDynaProvider::reSubOptionStocks()
{
    QVector<QString> stocks = getStocks();

    int num = stocks.size();

    if(m_rowCount != num){
        m_rowCount = num;
        emit rowCountChanged(m_rowCount);
    }
    if(num<1)  return;

    HZData::ReqOptionalDynaInfo req;
    for(int i=0;i<num;i++){
        req.stock_code.push_back(stocks[i].toStdString());
    }
    //订阅字段类型
    req.mask = m_subOptInfo.stock_mask;
    OnStockDynaArrived  stockOptionalDataArrived = std::bind(&OptionDynaProvider::onStockOptionalDataArrived
                                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);

    m_subOptInfo.reqId = gHZDataController->subOptionalDyna(req,stockOptionalDataArrived);
}

void OptionDynaProvider::onStockOptionalDataArrived(ReqId reqId,char *data,int num,ErrCode )
{
    if(num >0)
        //判断是第一次订阅返回
        m_model->updateData(data,0,num,m_subOptInfo.reqId == reqId);
}


///ZXStockDynaProvider-----
ZXStockDynaProvider::ZXStockDynaProvider(AbstractBaseStockModel *model,QObject *parent)
    :OptionDynaProvider(model,parent)
{
    connect(ZXStockOpt::instance(),&ZXStockOpt::stockCountChanged,this,&ZXStockDynaProvider::slotStkCountChanged);
}

void ZXStockDynaProvider::deleteStock(const QString &stockCode)
{
    if(m_currentDeletingStock  == stockCode)
        return;
    m_currentDeletingStock = stockCode;

    //暂时不激活，避免重复更新
    m_actived = false;
    bool ok = ZXStockOpt::instance()->delZXStock(stockCode);
    m_actived = true;

    if(ok){
        m_currentDeletingStock.clear();
        cancelOptSub();
        //移除数据
        m_model->removeStock(stockCode);
        //重新订阅
        reSubOptionStocks();
    }
}

void ZXStockDynaProvider::slotStkCountChanged(int count)
{
    //只有激活的窗口才执行
    if(m_actived){
        //发送股票数改变信号
        if(m_rowCount != count){
            m_rowCount = count;
            emit rowCountChanged(m_rowCount);
        }
        //重新订阅数据
        cancelOptSub();
        startSubOptionStocks();
    }
}

QVector<QString> ZXStockDynaProvider::getStocks()
{
    QVector<QString> zxstocks = ZXStockOpt::instance()->getZXStock();

    return zxstocks;
}



///HistStockDynaProvider-----
HistStockDynaProvider::HistStockDynaProvider(AbstractBaseStockModel *model,QObject *parent)
    :OptionDynaProvider(model,parent)
{

}

QVector<QString> HistStockDynaProvider::getStocks()
{
    return LocalStockSource::globalHistInstance()->getStocksList();
}



///ImpIndexDynaProvider----
ImpIndexDynaProvider::ImpIndexDynaProvider(AbstractBaseStockModel *model,QObject *parent)
    :OptionDynaProvider(model,parent)
{
    m_indexs =   \
    {"sh000001","sz399001","sz399005"
     ,"sz399006","sh000300","sh000016"
     ,"sh000132","sh000003","sh000012"
     ,"sh000013","sh000011","sz399106"
     ,"sz399004","sz399007","sz399008"
     ,"sz399100","sz399550","sh000903"};
}


QVector<QString> ImpIndexDynaProvider::getStocks()
{
    return m_indexs;
}


RelationBlockListProvider::RelationBlockListProvider(AbstractBaseStockModel *model, QObject *parent)
    :OptionDynaProvider(model,parent)
{

}

void RelationBlockListProvider::setCurrStock(const QString &stock)
{
    m_currStock = stock;
}


QVector<QString> RelationBlockListProvider::getStocks()
{
    QVector<QString> stocks;
    if(!m_currStock.isEmpty())
    {
        QVector<FilterDetail> out;
        Httpdata::instance()->getBkListByStock(out,m_currStock);
        stocks.resize(out.size());
        for(int i=0;i<out.size();i++)
            stocks[i]  = out[i].stock_code;
    }

    return stocks;
}
