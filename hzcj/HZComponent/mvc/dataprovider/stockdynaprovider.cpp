#include "stockdynaprovider.h"

#include "datacontroller.h"
#include "httpdata.h"

#include "hzuserdatamgr.h"

#include <QObject>
#include <QDebug>

///SLStockDynaProvider-----
void SLStockDynaProvider::actived(const bool &active)
{
    m_actived = active;

    if(active)
        subBatchStockData();
    else{
        cancelSub();
        m_isInitialed = false;
    }
}

void SLStockDynaProvider::setFilterFlag(const int &filterFlag)
{
    m_reqBatchInfo.filter = filterFlag;
}

void SLStockDynaProvider::setFilterDetail(const QString &filterDetial)
{
    m_reqBatchInfo.filter_detail = filterDetial.toStdString();
}

void SLStockDynaProvider::setFilterInfo(const int &filterFlag,const QString &filterDetial)
{
    m_reqBatchInfo.filter = filterFlag;
    m_reqBatchInfo.filter_detail = filterDetial.toStdString();
}

void SLStockDynaProvider::sortIndicatorChanged(const SortField &sortField,const bool &desc)
{
    m_reqBatchInfo.sort_field = sortField;
    m_reqBatchInfo.is_desc   = desc;
}

void SLStockDynaProvider::setStockCount(int  count)
{
    m_reqBatchInfo.count_ange = count;
    m_subBatchInfo.count = count;
    m_isInitialed = false;
    if(m_actived)
        subBatchStockData();
}

void SLStockDynaProvider::subBatchStockData()
{
    cancelSub();

    if(m_subBatchInfo.count<1)  return;

    //拉取显示范围条目
    HZData::ReqBatchDynaInfo req = m_reqBatchInfo;
    req.index = 0;
    req.count_ange = m_subBatchInfo.count;

    m_subBatchInfo.reqId = gHZDataController->subBatchDyna(req
                                                           ,std::bind(&SLStockDynaProvider::onStockBatchArrived
                                                                      ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                                                                      ,std::placeholders::_4));
}

void SLStockDynaProvider::onStockBatchArrived(ReqId,const char *data,int num,ErrCode errCode)
{
    if(errCode !=0) return;
    //第一次拉取全部排行初始化
    if(!m_isInitialed){
        m_isInitialed = true;
        m_model->initData(data,num);
    }
    else
        m_model->updateData(data,num);
}




///BatchDynaProvider---
BatchDynaProvider::BatchDynaProvider(AbstractBaseStockModel *model,QObject *parent)
    :BaseStockDynaProvider(model,parent)
{
    m_subBatchInfo.stock_mask = HZData::Block_Mask;
}

void BatchDynaProvider::actived(const bool &active)
{
    m_actived = active;
    m_isInitialed = false;
    cancelSub();
    if(active)
        reqBatchStockData();
    else
        m_model->clearData();

}

void BatchDynaProvider::setStockFieldMask(HZData::StockFieldMask mask)
{
    m_subBatchInfo.stock_mask = mask;
}

void BatchDynaProvider::onStockBatchDataArrived(ReqId reqId,const char * data,int num,ErrCode errCode)
{
    if(errCode !=0) return;

    //第一次拉取全部排行初始化
    if(reqId == m_reqBatchId){
        HZData::StockCode *stockCodes = (HZData::StockCode *)data;
        m_isInitialed = true;
        m_model->initData(stockCodes,num);
    }
    else if(reqId == m_subBatchInfo.reqId){
        m_model->updateData(data,m_subBatchInfo.start,num,true);
    }
}


///BKStockDynaProvider------
void BKStockDynaProvider::actived(const bool &active)
{
    m_actived = active;
    m_isInitialed = false;
    cancelSub();
    if(active && !m_reqBatchInfo.filter_detail.empty())
        reqBatchStockData();
    else
        m_model->clearData();
}


void BKStockDynaProvider::onStockBatchDataArrived(ReqId reqId,const char *data, int num, ErrCode errCode)
{
    if(errCode !=0 ||num < 1)  return;
    HZData::StockCode *stockCodes = (HZData::StockCode*)data;
    stockCodes ++;

    //去掉第一个
    if(m_reqBatchId ==reqId){
        m_isInitialed = true;
        m_model->initData(stockCodes,num-1);
    }else{
        subOptionalData(stockCodes,num-1);
    }
}

///IndexStockDynaProvider----
void  IndexStockDynaProvider::actived(const bool &active)
{
    m_actived = active;
    m_isInitialed = false;
    cancelSub();
    if(active && !m_reqBatchInfo.filter_detail.empty())
        reqBatchStockData();
    else{
        m_model->clearData();
    }
}

void IndexStockDynaProvider::onStockBatchDataArrived(ReqId  ,const char *data,int num,ErrCode errCode)
{
    if(errCode !=0 ||num < 1)  return;

    HZData::StockCode *stockCodes = (HZData::StockCode*)data;
    //去掉第一个
    if(!m_isInitialed){
        m_isInitialed = true;
        m_model->initData(stockCodes,num);
    }else
        subOptionalData(stockCodes,num);
}



///BKAndStockDynaProvider-----
void  BKAndStockDynaProvider::actived(const bool &active)
{
    m_actived = active;
    m_isInitialed = false;
    cancelSub();
    if(active && !m_reqBatchInfo.filter_detail.empty() && m_reqBatchInfo.filter != INVALID_INT32)
        reqBatchStockData();
    else
        m_model->clearData();
}

void BKAndStockDynaProvider::currentRangeChanged(int start , int count)
{
    start--;
    start=start<0?0:start;
    count--;
    BaseStockDynaProvider::currentRangeChanged(start,count);
}

void BKAndStockDynaProvider::onStockOptionalDataArrived(ReqId reqId,char *data,int num,ErrCode  )
{
    if(num >0)
        //第一个为板块
        m_model->updateData(data, m_subOptInfo.start,num,reqId == m_subOptInfo.reqId);
}




///AblBlockDynaProvider------------------
void  AblBlockDynaProvider::actived(const bool &active)
{
    m_actived = active;
    if(active){
        subAblBlockInfo();
    }else{
        m_isInitialed = false;
        cancelSub();
        m_model->clearData();
    }
}

void AblBlockDynaProvider::setFilterFlag(const int &filterFlag)
{
    m_filter = filterFlag;
}

void AblBlockDynaProvider::subAblBlockInfo()
{
    requestAblBlockInfo();

    //定时拉取
    m_timerId = this->startTimer(10000);
}

void AblBlockDynaProvider::requestAblBlockInfo()
{
    HZData::ReqAbnormalBlockInfo req;
    req.filter = m_filter;
    OnAbnormalBlockArrived func = std::bind(&AblBlockDynaProvider::onAblBlockInfoArrived,this
                                            ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_reqId = gHZDataController->requestAbnormalBlock(req, func);
}

void AblBlockDynaProvider::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_timerId){
        requestAblBlockInfo();
    }
    return QObject::timerEvent(e);
}

void AblBlockDynaProvider::onAblBlockInfoArrived(ReqId reqId, const QVector<HZData::Block>& blocks, ErrCode  errCode)
{
    if(!m_isInitialed ){
        ((AblBlockItemModel*)m_model)->initData(blocks);
        m_isInitialed  =true;
    }else if(blocks.size() != m_model->rowCount())
        ((AblBlockItemModel*)m_model)->updateData(blocks);
}

void AblBlockDynaProvider::cancelSub()
{
    if(m_timerId >0){
        this->killTimer(m_timerId);
        m_timerId = -1;
    }
    //取消请求
    if(m_reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}




