#include "fundprovider.h"

#include <QObject>
#include <QDebug>

#include "datacontroller.h"
#include "httpdata.h"
#include "hzuserdatamgr.h"

#include "zxstockopt.h"

BaseFundProvider::BaseFundProvider(FundBaseModel *model,QObject *parent)
    :QObject(parent), m_model(model)
    ,m_initialed(false),m_actived(false)
{
    m_t.setInterval(100);
    m_t.setSingleShot(true);

    //拉取股票码
    m_reqBatchInfo.mask    = HZData::MoneyStockCode_Mask;
    //订阅当日资金
    m_subReqInfo.money_mask = HZData::MoneyField_Mask;

    m_outSortField<<HZData::leader_money_output
                 <<HZData::maxord_money_output
                <<HZData::bigord_money_output
               <<HZData::midord_money_output
              <<HZData::minord_money_output;
    connect(&m_t,&QTimer::timeout,[this]{if(m_actived) subBatchMoneyData();});
}

BaseFundProvider::~BaseFundProvider()
{
    cancelSub();
}

void BaseFundProvider::actived(const bool &active)
{
    m_actived = active;
    m_initialed = false;
    cancel();
    if(active)
        reqBatchMoneyData();
    else
        m_model->clearData();
}

void BaseFundProvider::setMoneyFieldMask(HZData::MoneyFieldMask mask)
{
    m_subReqInfo.money_mask =mask;
}

void BaseFundProvider::startSub()
{
    cancelSub();

    if(m_initialed)
        m_t.start();
}

void BaseFundProvider::cancel()
{
    cancelSub();
    cancelReq();
}

void BaseFundProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
}

void BaseFundProvider::cancelReq()
{
    if(m_reqBatchqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_reqBatchqId);
        m_reqBatchqId = INVALID_REQID;
    }
}

void BaseFundProvider::setFilterFlag(const int &filterFlag)
{
    m_reqBatchInfo.filter = filterFlag;
    m_reqBatchInfo.count_range = Httpdata::instance()->getCountByFiter(filterFlag);
    m_reqBatchInfo.count_range = 10000;
}

void BaseFundProvider::setFilterDetail(const QString &filterDetial)
{
    m_reqBatchInfo.filter_detail = filterDetial.toStdString();
    m_reqBatchInfo.count_range = Httpdata::instance()->getCountByFiter(m_reqBatchInfo.filter,filterDetial);
}

void BaseFundProvider::setFilterInfo(const int &filterFlag,const QString &filterDetail)
{
    m_reqBatchInfo.filter = filterFlag;
    m_reqBatchInfo.filter_detail = filterDetail.toStdString();
    m_reqBatchInfo.count_range = Httpdata::instance()->getCountByFiter(filterFlag,filterDetail);
}

void BaseFundProvider::currentRangeChanged(int start , int count)
{
    if(!m_initialed) return;
    start = (start-count/2)<0?0:start-count/2;
    int end  = start + count*2;
    end = end>(m_model->rowCount()-1)?(m_model->rowCount()-1):end;
    m_subReqInfo.start = start;
    m_subReqInfo.count = end - start +1;

    startSub();
}

void BaseFundProvider::sortIndicatorChanged(const SortField &sortField,const bool &desc)
{
    m_reqBatchInfo.sort_field = sortField;
    m_reqBatchInfo.is_desc = desc;

    if(m_actived)
        actived(true);
}

void BaseFundProvider::reqBatchMoneyData()
{
    OnMoneyArrived  moneyArrived = std::bind(&BaseFundProvider::onBatchMoneyArrived
                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);

    m_reqBatchqId = gHZDataController->requestBatchMoney(m_reqBatchInfo,moneyArrived);
}

void BaseFundProvider::subBatchMoneyData()
{
    cancelSub();

    OnMoneyArrived  moneyArrived = std::bind(&BaseFundProvider::onBatchMoneyArrived
                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    HZData::ReqBatchFundInfo req = m_reqBatchInfo;

    req.mask            = m_subReqInfo.money_mask;
    req.index            = m_subReqInfo.start;
    req.count_range = m_subReqInfo.count;

    m_subReqInfo.reqId = DataController::instance()->subBatchMoney(req,moneyArrived);
    m_subReqInfo.start  = req.index ;
    m_subReqInfo.count = req.count_range;
}

void BaseFundProvider::onBatchMoneyArrived(ReqId  reqId,const char *data,int num,ErrCode errCode)
{
    if(errCode !=0) return;

    if(!m_initialed){
        m_initialed = true;
        m_stockCodes.clear();
        m_stockCodes.resize(num);
        HZData::StockCode *stockCodes = (HZData::StockCode *)data;
        for(int i=0;i<num;i++){
            m_stockCodes[i].stock_code =stockCodes[i].stock_code;
            m_stockCodes[i].stock_name=stockCodes[i].stock_name;
        }
        m_model->initData(m_stockCodes);
    }else{
        m_model->updateData(data,m_subReqInfo.start,num);
    }
}


///SLStockFundProvider----
SLStockFundProvider::SLStockFundProvider(FundBaseModel *model,QObject *parent)
    :StockFundProvider(model,parent)
{
    //拉取资金流向字段
    m_reqBatchInfo.mask = HZData::MoneyNetput_Mask;
}

void  SLStockFundProvider::actived(const bool &active)
{
    m_actived = active;
    m_initialed = false;
    cancelSub();
    if(active)
        subBatchMoneyData();
    else
        m_model->clearData();
}

void SLStockFundProvider::setStockCount(int count)
{
    m_reqBatchInfo.count_range = count;
    if(m_initialed)
        subBatchMoneyData();
}

void SLStockFundProvider::setFilterFlag(const int &filterFlag)
{
    //个股根据显示范围调整
    m_reqBatchInfo.filter = filterFlag;
}

void SLStockFundProvider::sortIndicatorChanged(const SortField &sortField,const bool &desc)
{
    m_reqBatchInfo.sort_field = sortField;
    m_reqBatchInfo.is_desc = desc;

    if(!m_initialed) return;
    subBatchMoneyData();
}

void SLStockFundProvider::subBatchMoneyData()
{
    cancelSub();
    m_initialed = false;

    OnMoneyArrived  moneyArrived = std::bind(&SLStockFundProvider::onBatchMoneyArrived
                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);

    m_subReqInfo.reqId = DataController::instance()->subBatchMoney(m_reqBatchInfo,moneyArrived);
    m_subReqInfo.start  = 0;
    m_subReqInfo.count = m_reqBatchInfo.count_range;
}

void SLStockFundProvider::onBatchMoneyArrived(ReqId  ,const char *data,int num,ErrCode errCode)
{
    if(errCode !=0) return;

    if(!m_initialed){
        m_initialed = true;
        m_model->initData(data,0,num);
    }else{
        m_model->updateData(data,m_subReqInfo.start,num);
    }
}


///OptionFundProvider----
OptionFundProvider::OptionFundProvider(FundBaseModel *model,QObject *parent)
    :StockFundProvider(model,parent)
{
    m_subReqInfo.money_mask = HZData::MoneyField_Mask;
}

void OptionFundProvider::actived(const bool &active)
{
    m_actived = active;
    m_initialed = false;
    cancelSub();
    if(m_actived)
        startSubOptionalFund();
    else
        m_model->clearData();
}

void OptionFundProvider::startSubOptionalFund()
{
    cancelSub();
    QVector<QString> stocks = getStocks();

    if(stocks.size()<1) return;

    reqOptionalFund(stocks);
}

void OptionFundProvider::reqOptionalFund(const QVector<QString> &stock_codes)
{
    HZData::ReqOptionalFundInfo req;
    for(int i=0;i<stock_codes.size();i++)
        req.stock_code.push_back( stock_codes[i].toStdString());

    req.mask  = HZData::MoneyField_Mask;
    m_reqBatchqId =  gHZDataController->requestOptionalMoney(req,std::bind(&OptionFundProvider::onOptionalFundArrived
                                                                           ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
}

void OptionFundProvider::subOptionalFund(const std::list<std::string> &stock_codes)
{
    HZData::ReqOptionalFundInfo req;
    req.stock_code = std::move(stock_codes);

    req.mask  = HZData::MoneyField_Mask;
    m_subReqInfo.reqId = gHZDataController->subOptionalMoney(req,std::bind(&OptionFundProvider::onOptionalFundArrived
                                                                           ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
}

void OptionFundProvider::onOptionalFundArrived(ReqId , char *data,int num, ErrCode errCode)
{
    if(errCode != 0) return;

    if(!m_initialed){
        m_initialed = true;
        m_model->initData(data,0,num);

        //开始订阅
        HZData::StockMoney* stockMoneys = (HZData::StockMoney*)data;
        std::list<std::string>     stock_codes;
        for(int i=0;i<num;i++){
            stock_codes.push_back(stockMoneys[i].stock_code);
        }
        subOptionalFund(stock_codes);
    }else;
    m_model->updateData(data,0,num);
}


///ImpIndexFundProvider----
ImpIndexFundProvider::ImpIndexFundProvider(FundBaseModel *model, QObject *parent)
    :OptionFundProvider(model,parent)
{
    //2018-11-20 去掉 "sh000011"
    m_stocks =  \
    {"sh000001","sz399001","sz399005"
     ,"sz399006","sh000300","sh000016"
     ,"sh000132","sh000003","sh000012"
     ,"sh000013","sz399106"
     ,"sz399004","sz399007","sz399008"
     ,"sz399100","sz399550","sh000903"};
}

QVector<QString> ImpIndexFundProvider::getStocks()
{
    return m_stocks;
}


///ZXStockFundProvider---
ZXStockFundProvider::ZXStockFundProvider(FundBaseModel *model, QObject *parent )
    :OptionFundProvider(model,parent)
{
    connect(ZXStockOpt::instance(),&ZXStockOpt::stockCountChanged,this,&ZXStockFundProvider::slotStkCountChanged);
}

void ZXStockFundProvider::slotStkCountChanged(int  )
{
    //其他地方导致自选股数量改变,重新订阅
    if(m_actived){
        startSubOptionalFund();
    }
}

QVector<QString> ZXStockFundProvider::getStocks()
{
    //获取自选股
    QVector<QString> stockCodes = ZXStockOpt::instance()->getZXStock();

    return  stockCodes;
}

void ZXStockFundProvider::deleteStock(const QString &stockCode)
{
    if(m_currentDeletingStock  == stockCode)
        return;
    m_currentDeletingStock = stockCode;
    //暂不激活，避免自身删除导致重新刷新
    m_actived = false;
    bool ok =ZXStockOpt::instance()->delZXStock(stockCode);
    m_actived = true;
    if(ok){
        m_currentDeletingStock.clear();
        //移除数据
        m_model->removeStock(stockCode);
        //重新订阅
        startSubOptionalFund();
    }
}


///HistStockFundProvider---
QVector<QString> HistStockFundProvider::getStocks()
{
    return  LocalStockSource::globalHistInstance()->getStocksList();
}
