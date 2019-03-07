#include "zxhiststockprivider.h"

#include "datacontroller.h"

#include "hzuserdatamgr.h"
#include "localstocksource.h"
#include "hzauthenopt.h"

#include <QObject>
#include <QDebug>



///ZXStockDynaProvider-----
ZXHistStockDynaProvider::ZXHistStockDynaProvider(StockBaseItemModel *model,QObject *parent)
    :QObject(parent),m_model(model),m_subReqId(INVALID_REQID)
    ,m_rowCount(0)
{

}

ZXHistStockDynaProvider::~ZXHistStockDynaProvider()
{
    cancelSub();
}

void ZXHistStockDynaProvider::actived(const bool &active)
{
    if(active)
        subOptionalStock();
    else{
        cancelSub();
    }
}

void ZXHistStockDynaProvider::cancelSub()
{
    if(m_subReqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void ZXHistStockDynaProvider::onStockOptionalDataArrived(ReqId  ,const QVector<AllStock > &stocks,ErrCode errCode)
{
    m_model->updateData(stocks,0,stocks.size());
}


///ZXStockDynaProvider-----
ZXStockDynaProvider::ZXStockDynaProvider(StockBaseItemModel *model,QObject *parent)
    :ZXHistStockDynaProvider(model,parent)
{
    m_authenOpt       = HZAuthenOpt::instance();
    m_zxStockSource = LocalStockSource::globalZXInstance();
}


QVector<QString> ZXStockDynaProvider::getZXStocks()
{
    QVector<QString> zxstocks;
    //判断当前用户类型
    if(m_authenOpt->isUserLogged()){
        m_authenOpt->queryZXStock();
        m_authenOpt->waitForFinshed();
        zxstocks = m_authenOpt->getZXStock();
    }else
        zxstocks = m_zxStockSource->getStocksList();
    return zxstocks;
}
void ZXStockDynaProvider::subOptionalStock()
{
    cancelSub();

    QVector<QString> zxstocks = getZXStocks();

    //1 初始化列表
    QVector<AllStock>  stocks(zxstocks.size());
    for(int i=0;i<zxstocks.size();i++)
        stocks[i].stock_code =zxstocks[i].toStdString();
    m_model->initData(stocks);

    if(m_rowCount != stocks.size()){
        m_rowCount = stocks.size();
        emit rowCountChanged(m_rowCount);
    }
    if(zxstocks.size()<1)
        return;

    //2 开始订阅
    ReqOptionalDynaInfo req;
    for(int i=0;i<zxstocks.size();i++){
        req.stock_code.push_back(zxstocks[i].toStdString());
    }
    OnStockDataArrived  stockOptionalDataArrived = std::bind(&ZXStockDynaProvider::onStockOptionalDataArrived
                                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_subReqId= DataController::instance()->subOptionalDyna(req,stockOptionalDataArrived);
}

void ZXStockDynaProvider::onStockOptionalDataArrived(ReqId  ,const QVector<AllStock > &stocks,ErrCode errCode)
{
    m_model->updateData(stocks,0,stocks.size());
    emit dataChanged();
}

///HistStockDynaProvider-----
HistStockDynaProvider::HistStockDynaProvider(StockBaseItemModel *model,QObject *parent)
    :ZXHistStockDynaProvider(model,parent)
{
    m_histStockSource = LocalStockSource::globalHistInstance();
}

void HistStockDynaProvider::subOptionalStock()
{
    cancelSub();

    QVector<QString> histstocks = m_histStockSource->getStocksList();
    QVector<AllStock>   stocks(histstocks.size());
    for(int i=0;i<histstocks.size();i++)
        stocks[i].stock_code =histstocks[i].toStdString();
    m_model->initData(stocks);

    if(m_rowCount != stocks.size()){
        m_rowCount = stocks.size();
        emit rowCountChanged(m_rowCount);
    }

    if(histstocks.size()<1)
        return;

    ReqOptionalDynaInfo req;
    for(int i=0;i<histstocks.size();i++){
        req.stock_code.push_back(histstocks[i].toStdString());
    }

    OnStockDataArrived  stockOptionalDataArrived = std::bind(&HistStockDynaProvider::onStockOptionalDataArrived
                                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_subReqId= DataController::instance()->subOptionalDyna(req,stockOptionalDataArrived);
}

