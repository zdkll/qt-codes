#include "basedataprovider.h"

#define Default_ReqBatch_Count  50

#define Default_Count_PerPage    20
#define Rzrq_Count_PerPage       10

#include <QTimer>

#include "hzauthenopt.h"
#include "zxstockopt.h"
#include "datacontroller.h"
#include "hzinfocenter.h"

BaseDataProvider::BaseDataProvider(AbstractDataBaseModel *model,QObject *parent)
    :QObject(parent),m_model(model)
    ,m_isInitialed(false),m_actived(false)
{
    m_timer.setSingleShot(true);
    m_timer.setInterval(200);

    connect(&m_timer,&QTimer::timeout,this,&BaseDataProvider::slotSubTimeOut);
}

void BaseDataProvider::actived(bool active)
{
    m_actived    = active;
    m_isInitialed = false;
    cancelSub();
    if(active)
        reqData();
    else{
        m_model->clearData();
    }
}

void BaseDataProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
}

void BaseDataProvider::startSub()
{
    cancelSub();
    m_timer.start();
}

void BaseDataProvider::slotSubTimeOut()
{
    subData();
}

QVector<QString> BaseDataProvider::getZXStocks()
{
    return ZXStockOpt::instance()->getZXStock();
}


///BiddingDataProvider----
BiddingDataProvider::BiddingDataProvider(BiddingItemModel *model,QObject *parent)
    :BaseDataProvider(model,parent)
{
    m_dataModel = model;

    m_reqBatchBidInfo.index = 0;
    m_reqBatchBidInfo.count_range = Default_ReqBatch_Count;
}

void BiddingDataProvider::currentRangeChanged(int start,int count)
{
    if(!m_actived)  return;
    start  = start - count/2;
    start  =start<0?0:start;

    int end = start+2*count;
    end = end>(m_dataModel->rowCount()-1)?(m_dataModel->rowCount()-1):end;
    m_reqBatchBidInfo.index            = start;
    m_reqBatchBidInfo.count_range = end-start+1;

    if(m_isInitialed)
        startSub();
}

void BiddingDataProvider::setSortIndicator(SortField sortField,bool desc)
{
    m_reqBatchBidInfo.sort_field = sortField;
    m_reqBatchBidInfo.is_desc   = desc;

    //重新订阅当前范围排行
    if(m_actived)
        reqData();
}

void BiddingDataProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
    if(m_reqId  != INVALID_REQID){
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}


void BiddingDataProvider::reqData()
{
    cancelSub();
    m_isInitialed = false;

    m_reqBatchBidInfo.count_range = Default_ReqBatch_Count;

    OnBiddingArrived func = std::bind(&BiddingDataProvider::OnBatchBiddingArrived,this
                                      ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_reqId = gHZDataController->requestBatchBidding(m_reqBatchBidInfo,func);
}

void BiddingDataProvider::subData()
{
    OnBiddingArrived func = std::bind(&BiddingDataProvider::OnBatchBiddingArrived,this
                                      ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_subReqInfo.start   = m_reqBatchBidInfo.index;
    m_subReqInfo.count = m_reqBatchBidInfo.count_range;
    m_subReqInfo.reqId = gHZDataController->subBatchBidding(m_reqBatchBidInfo,func);
}

void BiddingDataProvider::OnBatchBiddingArrived(ReqId ,const HZData::BiddingInfo &bidInfo, ErrCode errCode)
{
    if(errCode != 0) return;

    if(!m_isInitialed){
        m_isInitialed = true;
        m_totalCount = bidInfo.total_cnt;
        m_dataModel->initData(bidInfo);
    }else{
        if(bidInfo.total_cnt != m_totalCount){
            //数量变化，重新请求数据
            m_totalCount = bidInfo.total_cnt;
            reqData();
        }
        m_dataModel->updateData(bidInfo,m_subReqInfo.start);
    }
}


///BiddingOpotionalProvider----
BiddingOpotionalProvider::BiddingOpotionalProvider(BiddingItemModel *model,QObject *parent)
    :BaseDataProvider(model,parent)
{
    m_dataModel = model;

    connect(ZXStockOpt::instance(),&ZXStockOpt::stockCountChanged,this,&BiddingOpotionalProvider::slotStkCountChanged);
}

void BiddingOpotionalProvider::actived(bool active)
{
    m_actived = active;
    m_isInitialed  = false;
    cancelSub();
    if(m_actived)
        subOptionalBidding();
    else{
        m_dataModel->clearData();
    }
}

void BiddingOpotionalProvider::slotStkCountChanged(int )
{
    if(m_actived){
        m_dataModel->clearData();
        actived(true);
    }
}

void BiddingOpotionalProvider::subOptionalBidding()
{
    QVector<QString> stocks = getZXStocks();

    //1 初始化列表
    int num = stocks.size();
    if(num <1)  return;

    std::list<std::string> stock_codes;
    for(int i=0;i<num;i++)
        stock_codes.push_back(stocks[i].toStdString());

    //第一次请求数据,然后筛选
    OnBiddingArrived func = std::bind(&BiddingOpotionalProvider::OnOptionalBiddingArrived,this
                                      ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    gHZDataController->requestOptionalBidding(stock_codes,func);
}

void BiddingOpotionalProvider::OnOptionalBiddingArrived(ReqId,const HZData::BiddingInfo &biddingInfo, ErrCode errCode)
{
    if(errCode!=0) return;
    if(!m_isInitialed){
        m_dataModel->initData(biddingInfo);
        if(m_rowCount != biddingInfo.total_cnt)
        {
            m_rowCount = biddingInfo.total_cnt;
            emit rowCountChanged(m_rowCount);
        }
        m_isInitialed = true;

        //开始订阅
        if(m_rowCount < 1) return;
        std::list<std::string> stock_codes;
        for(int i=0;i<m_rowCount;i++)
            stock_codes.push_back(biddingInfo.stk[i].stock_code);
        subBidding(stock_codes);
    }else
        m_dataModel->updateData(biddingInfo,0);
}

void BiddingOpotionalProvider::subBidding(const std::list<std::string> &stockCodes)
{
    //先拉取一次集合竞价，然后再订阅
    OnBiddingArrived func = std::bind(&BiddingOpotionalProvider::OnOptionalBiddingArrived,this
                                      ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    m_subReqInfo.reqId = gHZDataController->subOptionalBidding(stockCodes,func);
}


///RiseFallMoniProvider----
RiseFallMoniProvider::RiseFallMoniProvider(RFMonitorItemModel *model,QObject *parent)
    :BaseDataProvider(model,parent)
{
    m_dataModel = model;

    m_reqBatchRfmInfo.index = 0;
    m_reqBatchRfmInfo.count_range = Default_ReqBatch_Count;
}

void RiseFallMoniProvider::setRFMonitorType(HZData::RFMonitorType type)
{
    m_reqBatchRfmInfo.m_type = type;
}

void RiseFallMoniProvider::currentRangeChanged(int start,int count)
{
    if(!m_actived)  return;
    start  = start - count/2;
    start  =start<0?0:start;
    int end = start+2*count;
    end = end>(m_dataModel->rowCount()-1)?(m_dataModel->rowCount()-1):end;

    m_reqBatchRfmInfo.index = start;
    m_reqBatchRfmInfo.count_range =  end-start+1;

    if(m_isInitialed)
        startSub();
}

void RiseFallMoniProvider::setSortIndicator(SortField sortField,bool desc)
{
    m_reqBatchRfmInfo.sort_type = sortField;
    m_reqBatchRfmInfo.is_desc   = desc;

    if(m_actived)
        reqData();
}

void RiseFallMoniProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
    if(m_reqId  != INVALID_REQID){
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}

void RiseFallMoniProvider::reqData()
{
    cancelSub();
    m_isInitialed = false;
    m_reqBatchRfmInfo.count_range = Default_ReqBatch_Count;
    OnRFMonitorInfoArrived func = std::bind(&RiseFallMoniProvider::OnBatchRfmArrived,this
                                            ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_reqId =  gHZDataController->requestRFMonitorInfo(m_reqBatchRfmInfo,func);
}

void RiseFallMoniProvider::subData()
{
    OnRFMonitorInfoArrived func = std::bind(&RiseFallMoniProvider::OnBatchRfmArrived,this
                                            ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_subReqInfo.start  = m_reqBatchRfmInfo.index;
    m_subReqInfo.count= m_reqBatchRfmInfo.count_range;
    m_subReqInfo.reqId = gHZDataController->subRFMonitorInfo(m_reqBatchRfmInfo,func);
}

void RiseFallMoniProvider::OnBatchRfmArrived(ReqId, const HZData::RiseFallMonitorInfo &rfmInfo, ErrCode errCode)
{
    if(errCode != 0) return;
    if(!m_isInitialed){
        m_isInitialed  =true;
        m_totalCount = rfmInfo.totalCount;
        m_dataModel->initData(rfmInfo);
    }else{
        if(rfmInfo.totalCount != m_totalCount){
            //数量变化，重新请求数据
            m_totalCount = rfmInfo.totalCount;
            reqData();
        }
        m_dataModel->updateData(rfmInfo,m_subReqInfo.start);
    }
}




///FunAbnormalProvider----
FunAbnormalProvider::FunAbnormalProvider(FundAbnormalItemModel *model,QObject *parent)
    :BaseDataProvider(model,parent)
{
    m_dataModel = model;

    m_reqBatchFalInfo.index = 0;
    m_reqBatchFalInfo.count_range = Default_ReqBatch_Count;
}

void FunAbnormalProvider::setFunAbnormalType(HZData::FundAbnormalType type)
{
    m_reqBatchFalInfo.type = type;
}

void FunAbnormalProvider::currentRangeChanged(int start,int count)
{
    if(!m_actived)  return;
    start  = start - count/2;
    start  =start<0?0:start;
    int end = start+2*count;
    end = end>(m_dataModel->rowCount()-1)?(m_dataModel->rowCount()-1):end;

    m_reqBatchFalInfo.index = start;
    m_reqBatchFalInfo.count_range =  end-start+1;

    if(m_isInitialed)
        startSub();
}

void FunAbnormalProvider::setSortIndicator(SortField sortField,bool desc)
{
    m_reqBatchFalInfo.sort_field = sortField;
    m_reqBatchFalInfo.is_desc = desc;

    if(m_actived)
        reqData();
}

void FunAbnormalProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
    if(m_reqId  != INVALID_REQID){
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}

void FunAbnormalProvider::reqData()
{
    m_isInitialed = false;
    cancelSub();
    m_reqBatchFalInfo.count_range = Default_ReqBatch_Count;
    OnFundAbnormalInfoArrived func = std::bind(&FunAbnormalProvider::OnBatchFalArrived,this
                                               ,std::placeholders::_1 ,std::placeholders::_2 ,std::placeholders::_3);
    m_reqId = gHZDataController->requestBatchFundAbnormal(m_reqBatchFalInfo,func);
}

void FunAbnormalProvider::subData()
{
    OnFundAbnormalInfoArrived func = std::bind(&FunAbnormalProvider::OnBatchFalArrived,this
                                               ,std::placeholders::_1 ,std::placeholders::_2 ,std::placeholders::_3);
    m_subReqInfo.reqId = gHZDataController->requestBatchFundAbnormal(m_reqBatchFalInfo,func);
    m_subReqInfo.start = m_reqBatchFalInfo.index;
    m_subReqInfo.count=m_reqBatchFalInfo.count_range;
}

void FunAbnormalProvider::OnBatchFalArrived(ReqId, const HZData::FundAbnormalInfo &fundAbnormalInfo, ErrCode errCode)
{
    if(errCode != errCode) return;

    if(!m_isInitialed){
        m_isInitialed = true;
        m_totalCount = fundAbnormalInfo.totalCount;
        m_dataModel->initData(fundAbnormalInfo);
    }else{
        if(fundAbnormalInfo.totalCount != m_totalCount){
            //数量变化，重新请求数据
            m_totalCount = fundAbnormalInfo.totalCount;
            reqData();
        }
        m_dataModel->updateData(fundAbnormalInfo,m_subReqInfo.start);
    }
}


///FamlOptionalProvider----
FamlOptionalProvider::FamlOptionalProvider(FundAbnormalItemModel *model,QObject *parent)
    :BaseDataProvider(model,parent)
{
    m_dataModel  = model;

    connect(ZXStockOpt::instance(),&ZXStockOpt::stockCountChanged,this,&FamlOptionalProvider::slotStkCountChanged);
}

void FamlOptionalProvider::slotStkCountChanged(int )
{
    if(m_actived){
        m_dataModel->clearData();
        actived(true);
    }
}

void FamlOptionalProvider::setFunAbnormalType(HZData::FundAbnormalType type)
{
    m_reqBatchFalInfo.type = type;
}

void FamlOptionalProvider::actived(bool active)
{
    m_actived  = active;
    m_isInitialed = false;
    cancelSub();
    if(m_actived){
        subFundAbnormal();
    }else{
        m_dataModel->clearData();
    }
}

void FamlOptionalProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
    if(m_reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}

void FamlOptionalProvider::subFundAbnormal()
{
    QVector<QString> stocks = getZXStocks();

    //1 初始化列表
    int num = stocks.size();
    if(num <1)  return;

    std::list<std::string> stock_codes;
    for(int i=0;i<num;i++)
        stock_codes.push_back(stocks[i].toStdString());

    //第一次请求数据,然后筛选
    HZData::ReqOptionalFundAbnormal req;
    req.stock_code = stock_codes;
    OnFundAbnormalInfoArrived func = std::bind(&FamlOptionalProvider::OnOptionalFalArrived,this
                                               ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    m_reqId = gHZDataController->requestOptionalFundAbnormal(req,func);
}

void FamlOptionalProvider::OnOptionalFalArrived(ReqId, const HZData::FundAbnormalInfo &famlInfo, ErrCode errCode)
{
    if(errCode != 0) return;
    if(!m_isInitialed){
        m_isInitialed = true;
        m_dataModel->initData(famlInfo);

        const int num = famlInfo.totalCount;
        if(num<0) return;

        HZData::ReqOptionalFundAbnormal req;
        for(int i=0;i<num;i++)
            req.stock_code.push_back(famlInfo.fundAbmls[i].stock_code);

        OnFundAbnormalInfoArrived func = std::bind(&FamlOptionalProvider::OnOptionalFalArrived,this
                                                   ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        m_subReqInfo.reqId = gHZDataController->subOptionalFundAbnormal(req,func);
    }else{
        m_dataModel->updateData(famlInfo,0);
    }
}




///BasePageProvider--
BasePageProvider::BasePageProvider(PageReqType type,AbstractDataBaseModel *model,QObject *parent)
    :BaseDataProvider(model,parent),m_prtType(type)
    ,m_reqId(RewInfoInvalidId),m_curPage(0)
{
    m_minExceCount = Default_Count_PerPage;
}

void BasePageProvider::setPageReqType(const PageReqType &type)
{
    m_prtType = type;
}

void BasePageProvider::actived(bool active)
{
    m_actived   = active;
    m_curPage = 0;
    m_isInitialed = false;
    cancelSub();
    if(m_actived){
        reqData();
    }else
        m_model->clearData();
}

//都是请求
void BasePageProvider::cancelSub()
{
    if(m_reqId != RewInfoInvalidId){
        gHzInfoCenter->cancle(m_reqId);
        m_reqId= RewInfoInvalidId;
    }
}

void BasePageProvider::currentRangeChanged(int start ,int count)
{
    if(m_prtType == PRTOnce) return;

    m_curRange.first = start;
    m_curRange.second = count;

    if(m_actived)
        reqNextPage();
}

void BasePageProvider::reqNextPage()
{
    if(m_prtType == PRTOnce) return;

    //数据拉到底 或者  正在请求数据
    if(m_curPage == -1 || m_reqId != RewInfoInvalidId) return;

    //数据范围
    int decCount = m_model->rowCount() - (m_curRange.first+m_curRange.second);
    if((decCount< m_minExceCount) || (m_model->rowCount() <m_showCount))
        reqData();
}



///LhbStockProvider---
LhbStockProvider::LhbStockProvider(LhbStockItemModel *model,QObject *parent)
    :BasePageProvider(PRTOnce,model,parent)
{
    m_dataModel= model;
}

void LhbStockProvider::setStockLhbType(StockLhbType  type)
{
    m_stkLhbType = type;
}


void LhbStockProvider::reqData()
{
    StockLhbDatasCallBack func = std::bind(&LhbStockProvider::stockLhbDatasArrived,this
                                           ,std::placeholders::_1,std::placeholders::_2);

    m_reqId = gHzInfoCenter->reqStockLhbDatas(m_time,m_stkLhbType,func);
}

void LhbStockProvider::stockLhbDatasArrived(const InfoRet& ret,const StockLhbDatas& stkLhbDatas)
{
    if(ret.code != 0) return;

    m_dataModel->initData(stkLhbDatas);
}



///LhbYybDrbProvider--
LhbYybDrbProvider::LhbYybDrbProvider(LhbYybDrbStockItemModel *model,QObject *parent)
    :BasePageProvider(PRTAllPage,model,parent)
{
    m_dataModel = model;
}



void LhbYybDrbProvider::reqData()
{
    m_reqId = gHzInfoCenter->reqStockYybDrb(m_time,m_curPage,std::bind(&LhbYybDrbProvider::onLhbYybDrbArrived,this
                                                                       ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}

void LhbYybDrbProvider::onLhbYybDrbArrived(const InfoRet& ret,const StockYybDrbList& stkYybDrbList,int npage)
{
    if(ret.code != 0){
        m_reqId = RewInfoInvalidId;
        m_curPage = npage;
        return;
    }

    if(!m_isInitialed){
        m_isInitialed = true;
        m_dataModel->initData(stkYybDrbList);
    }else{
        m_dataModel->appendData(stkYybDrbList);
    }
    m_reqId = RewInfoInvalidId;
    m_curPage = npage;

    reqNextPage();
}


///LhbYybSlbProvider----
LhbYybSlbProvider::LhbYybSlbProvider(LhbYybSlbStockItemModel *model,QObject *parent)
    :BasePageProvider(PRTAllPage,model,parent)
{
    m_dataModel = model;
}

void LhbYybSlbProvider::reqData()
{
    m_reqId = gHzInfoCenter->reqStockYybSlb(m_curPage,std::bind(&LhbYybSlbProvider::onLhbYybSlbArrived,this
                                                                ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}

void LhbYybSlbProvider::onLhbYybSlbArrived(const InfoRet& ret,const StockYybSlbList&stkYybSlbList,int npage)
{
    if(ret.code != 0){
        m_reqId = RewInfoInvalidId;
        m_curPage = npage;
        return;
    }

    if(m_curPage == 0){
        m_isInitialed = true;
        m_dataModel->initData(stkYybSlbList);
    }else{
        m_dataModel->appendData(stkYybSlbList);
    }
    m_reqId = RewInfoInvalidId;
    m_curPage = npage;

    reqNextPage();
}




///LhbYybHybProvider----
LhbYybHybProvider::LhbYybHybProvider(LhbYybHybStockItemModel *model,QObject *parent)
    :BasePageProvider(PRTAllPage,model,parent)
{
    m_dataModel = model;
}

void LhbYybHybProvider::reqData()
{
    m_reqId = gHzInfoCenter->reqStockYybHyb(m_curPage,std::bind(&LhbYybHybProvider::onLhbYybHybArrived,this
                                                                ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

}

void LhbYybHybProvider::onLhbYybHybArrived(const InfoRet& ret,const StockYybHybList& stkYybHybList,int npage)
{
    if(ret.code != 0){
        m_reqId = RewInfoInvalidId;
        m_curPage = npage;
        return;
    }

    if(!m_isInitialed){
        m_isInitialed = true;
        m_dataModel->initData(stkYybHybList);
    }else{
        m_dataModel->appendData(stkYybHybList);
    }
    m_reqId = RewInfoInvalidId;
    m_curPage = npage;

    reqNextPage();
}


///StockDzjyProvider---
StockDzjyProvider::StockDzjyProvider(StockDzjyItemModel *model,QObject *parent)
    : BasePageProvider(PRTOnce,model,parent)
{
    m_dataModel = model;
}

void StockDzjyProvider::reqData()
{
    StockDzjyListCallBack func = std::bind(&StockDzjyProvider::stkDzjyArrived
                                           ,this,std::placeholders::_1,std::placeholders::_2);
    gHzInfoCenter->reqStockDzjyList(m_time,func);
}

void StockDzjyProvider::stkDzjyArrived(const InfoRet& ret,const StockDzjyList& stkDzjyList)
{
    if(ret.code!=0) return;

    m_dataModel->initData(stkDzjyList);
}


///StockRzrqRankProvider----
StockRzrqRankProvider::StockRzrqRankProvider(PageReqType type,StockRzrqRankItemModel *model,QObject *parent)
    :BasePageProvider(type,model,parent)
{
    m_curRange.first       = 0;
    m_curRange.second = 20;
    m_minExceCount = Rzrq_Count_PerPage;
    m_dataModel  = model;
}

void StockRzrqRankProvider::reqData()
{
    StockRzrqTopListCallBack  func = std::bind(&StockRzrqRankProvider::stockRzrqTopListArrived,this,
                                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    m_reqId = gHzInfoCenter->reqStockRzrqTopList(m_time,m_sort,m_curPage,func);
}

void StockRzrqRankProvider::stockRzrqTopListArrived(const InfoRet& ret,const StockRzrqTopList& stkRzrqTopList,int npage)
{
    if(ret.code != 0){
        m_reqId = RewInfoInvalidId;
        m_curPage = npage;
        return;
    }

    if(!m_isInitialed){
        m_isInitialed = true;
        m_dataModel->initData(stkRzrqTopList);
    }else{
        m_dataModel->appendData(stkRzrqTopList);
    }
    m_curPage = npage;
    m_reqId = RewInfoInvalidId;

    if(m_prtType  == PRTAllPage){

        reqNextPage();
    }
}



///StockRzrqDetailProvider---
StockRzrqDetailProvider::StockRzrqDetailProvider(StockRzrqDetailItemModel *model,QObject *parent)
    :BasePageProvider(PRTOnce,model,parent)
{
    m_dataModel = model;
}


void StockRzrqDetailProvider::setStockCode(const QString &stockCode)
{
    m_stockCode = stockCode;
}

void StockRzrqDetailProvider::reqData()
{
    StockRzrqDetailListCallBack func = std::bind(&StockRzrqDetailProvider::stockRzrqDetailArrived,this
                                                 ,std::placeholders::_1 ,std::placeholders::_2 );
    m_reqId = gHzInfoCenter->reqStockRzrqDetailList(m_time,m_stockCode,func);
}

void StockRzrqDetailProvider::stockRzrqDetailArrived(const InfoRet& ret,const StockRzrqDetailList& stkRzrqDetailList)
{
    if(ret.code != 0) return;

    m_dataModel->initData(stkRzrqDetailList.dataList);
}



///BlockRzrqProvider---
BlockRzrqProvider::BlockRzrqProvider(BlockRzrqItemModel *model,QObject *parent)
    :BasePageProvider(PRTOnce,model,parent)
{
    m_dataModel = model;
}


void BlockRzrqProvider::reqData()
{
    StockBoardRzrqTopListCallBack func = std::bind(&BlockRzrqProvider::blockRzrqArrived,this,std::placeholders::_1
                                                   ,std::placeholders::_2);

    m_reqId = gHzInfoCenter->reqStockBoardRzrqTopList(m_time,func);
}

void BlockRzrqProvider::blockRzrqArrived(const InfoRet& ret,const StockBoardRzrqTopList &blockRzrqList)
{
    m_reqId = RewInfoInvalidId;
    if(ret.code !=0 ) return;

    m_dataModel->initData(blockRzrqList);

    std::list<std::string> stock_codes;
    for(int i=0;i<blockRzrqList.size();i++){
        stock_codes.push_back(blockRzrqList[i].boardcode.toStdString());

    }
    //拉取快照
    reqStockDyna(stock_codes);
}

void BlockRzrqProvider::reqStockDyna(const std::list<std::string> &stockcodes)
{
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stockcodes;
    req.mask = HZData::Stock_Mask;
    gHZDataController->requestOptionalDyna(req,std::bind(&BlockRzrqProvider::onStockDynaArrived
                                                         ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
}

void BlockRzrqProvider::onStockDynaArrived(ReqId  , char *data, int num, ErrCode errCode)
{
    if(errCode != 0) return;
    m_dataModel->setStockDyna((HZData::Stock*)data,num);
}




///BlockStockRzrqProvider----
BlockStockRzrqProvider::BlockStockRzrqProvider(BlockStockRzrqItemModel *model,QObject *parent)
    :BasePageProvider(PRTAllPage,model,parent)
{
    m_dataModel = model;
}



void BlockStockRzrqProvider::setBlockCode(const QString& blockCode)
{
    m_blockCode = blockCode;
}

void BlockStockRzrqProvider::reqData()
{
    StockBoardRzrqListCallBack func = std::bind(&BlockStockRzrqProvider::blockStockRzrqArrived
                                                ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_reqId = gHzInfoCenter->reqStockBoardRzrqList(m_time,m_blockCode,m_curPage,func);
}

void BlockStockRzrqProvider::blockStockRzrqArrived(const InfoRet& ret,const StockBoardRzrqList& bkStockRzrqList,int npage)
{
    if(ret.code != 0){
        m_reqId = RewInfoInvalidId;
        m_curPage = npage;
        return;
    }
    if(!m_isInitialed){
        m_isInitialed = true;
        m_dataModel->initData(bkStockRzrqList.dataList);
        emit bkStockCountChanged(bkStockRzrqList.count);
    }else
        m_dataModel->appendData(bkStockRzrqList.dataList);

    m_curPage = npage;
    m_reqId = RewInfoInvalidId;

    reqNextPage();
}




///XgrlStockProvider
XgrlStockProvider::XgrlStockProvider(XgrlType type, XgrlStockItemModel *model,QObject *parent)
    :BasePageProvider(PRTOnce,model,parent),m_xgrlType(type)
{
    m_dataModel = model;
}

void XgrlStockProvider::reqData()
{
    NewStockPreViewCallBack func = std::bind(&XgrlStockProvider::xgrlNewStockArrived
                                             ,this,std::placeholders::_1,std::placeholders::_2);

    switch (m_xgrlType) {
    case Xgrl_Xgsg:
        gHzInfoCenter->reqNewStockTodaysBids(m_time,func);
        break;
    case Xgrl_Xgyg:
        gHzInfoCenter->reqNewStockPreViews(func);
        break;
    case Xgrl_Jrsg:
        gHzInfoCenter->reqNewStockTodaysBids(-1,func);
        break;
    case Xgrl_Jrss:
        gHzInfoCenter->reqNewStockMarketTodays(func);
        break;
    case Xgrl_Yfxdss:
        gHzInfoCenter->reqNewStockMarketIssueds(func);
        break;
    case Xgrl_Xgssbx:
        gHzInfoCenter->reqNewStockIPOPerformances(func);
        break;
    default:
        break;
    }
}

void XgrlStockProvider::xgrlNewStockArrived(const InfoRet& ret,const QVector<StockXgrlItem> &stockXgrlItems)
{
    if(ret.code != 0) return;

    m_dataModel->initData(stockXgrlItems);

    qDebug()<<"xgrlNewStockArrived count:"<<stockXgrlItems.size();

    //新股上市表现请求快照填充
    if(stockXgrlItems.size()>0){
        if(m_xgrlType == Xgrl_Xgssbx || m_xgrlType == Xgrl_Jrss) { //请求快照
            HZData::ReqOptionalDynaInfo req;
            for(int i=0;i<stockXgrlItems.size();i++){
                req.stock_code.push_back(stockXgrlItems[i].stockcode.toStdString());
            }
            req.mask = HZData::SLStock_Mask;
            gHZDataController->requestOptionalDyna(req,std::bind(&XgrlStockProvider::optionalDynaArrived,this
                                                                 ,std::placeholders::_1 ,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        }
    }
}

void XgrlStockProvider::optionalDynaArrived(ReqId  , char *data, int num, ErrCode )
{
    m_dataModel->updateDynaData(data,num);
}



