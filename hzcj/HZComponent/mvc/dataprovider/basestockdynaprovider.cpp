#include "basestockdynaprovider.h"

#include "datacontroller.h"
#include "httpdata.h"

#include "hzuserdatamgr.h"

#include <QObject>
#include <QDebug>

BaseStockDynaProvider::BaseStockDynaProvider(AbstractBaseStockModel *model,QObject *parent)
    :QObject(parent), m_model(model)
    ,m_actived(false), m_isInitialed(false)
    ,m_reqBatchId(INVALID_REQID)
{
    m_subTimer.setInterval(200);
    m_subTimer.setSingleShot(true);

    m_reqBatchInfo.mask           = HZData::StockCode_Mask;
    m_subBatchInfo.stock_mask = HZData::StockCode_Mask;
    //当显示范围变化时，定时拉取排行
    connect(&m_subTimer,&QTimer::timeout, this,&BaseStockDynaProvider::subBatchStockData);
}

BaseStockDynaProvider::~BaseStockDynaProvider()
{
    cancelSub();
}

void BaseStockDynaProvider::setStockFieldMask(HZData::StockFieldMask mask)
{
    m_subOptInfo.stock_mask = mask;
}

void BaseStockDynaProvider::actived(const bool &active)
{
    m_actived = active;
    m_isInitialed = false;
    cancelSub();
    if(active)
        reqBatchStockData();
    else
        m_model->clearData();
}

void BaseStockDynaProvider::startSub()
{
    cancelSub();
    m_subTimer.start();
}

void BaseStockDynaProvider::cancelSub()
{
    cancelReqBatch();
    cancelBatchSub();
    cancelOptSub();
}

void BaseStockDynaProvider::cancelReqBatch()
{
    if(m_reqBatchId != INVALID_REQID){
        DataController::instance()->cancelSub(m_reqBatchId);
        qDebug()<<"cancel reqBatch:"<<m_reqBatchId;
        m_reqBatchId = INVALID_REQID;

    }
}

void BaseStockDynaProvider::cancelBatchSub()
{
    if(m_subBatchInfo.reqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_subBatchInfo.reqId);
        m_subBatchInfo.reqId = INVALID_REQID;
    }
}

void BaseStockDynaProvider::cancelOptSub()
{
    if(m_subOptInfo.reqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_subOptInfo.reqId);
        m_subOptInfo.reqId = INVALID_REQID;
    }
}

void BaseStockDynaProvider::slotStartSubBatchDyna()
{
    subBatchStockData();
}

void BaseStockDynaProvider::setFilterFlag(const int &filterFlag)
{
    m_reqBatchInfo.filter = filterFlag;
   qDebug()<<"93=============setFilter:"<<filterFlag;
    int stockCount = Httpdata::instance()->getCountByFiter(filterFlag);
    m_reqBatchInfo.count_ange = stockCount;
    m_reqBatchInfo.count_ange = 10000;
}

void BaseStockDynaProvider::setFilterDetail(const QString &filterDetial)
{
    m_reqBatchInfo.filter_detail = filterDetial.toStdString();

    int stockCount = Httpdata::instance()->getCountByFiter(m_reqBatchInfo.filter,filterDetial);
    m_reqBatchInfo.count_ange = stockCount;
    m_reqBatchInfo.count_ange = 10000;
}

void BaseStockDynaProvider::setFilterInfo(const int &filterFlag,const QString &filterDetial)
{
    m_reqBatchInfo.filter = filterFlag;
    m_reqBatchInfo.filter_detail = filterDetial.toStdString();
    int stockCount = Httpdata::instance()->getCountByFiter(filterFlag,filterDetial);
    m_reqBatchInfo.count_ange = stockCount;
    m_reqBatchInfo.count_ange = 10000;
}

void BaseStockDynaProvider::currentRangeChanged(int start , int count)
{
    //计算拉取排行范围，上下各半屏
    m_subBatchInfo.start = (start - count/2)<0?0:(start - count/2);
    int end = start+count +count/2;
    end = end> (m_reqBatchInfo.count_ange -1)?m_reqBatchInfo.count_ange-1: end;
    m_subBatchInfo.count =  end-m_subBatchInfo.start+1;

    if(!m_isInitialed ) return;
    startSub();
}

void BaseStockDynaProvider::sortIndicatorChanged(const SortField &sortField,const bool &desc)
{
    m_reqBatchInfo.sort_field = sortField;
    m_reqBatchInfo.is_desc   = desc;

    if(m_actived)
        actived(true);
}

void BaseStockDynaProvider::reqBatchStockData()
{

    m_reqBatchId = DataController::instance()->requestBatchDyna(m_reqBatchInfo
                                                                ,std::bind(&BaseStockDynaProvider::onStockBatchDataArrived
                                                                           ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                                                                    ,std::placeholders::_4));
   qDebug() << "=================m_reqBatchId req=====:" << m_reqBatchId;
}

//订阅当前屏幕数据
void BaseStockDynaProvider::subBatchStockData()
{
    if(!m_actived) return;

    if(m_subBatchInfo.count<1)  return;

    HZData::ReqBatchDynaInfo req = m_reqBatchInfo;
    req.mask           = m_subBatchInfo.stock_mask;
    req.index           = m_subBatchInfo.start;
    req.count_ange = m_subBatchInfo.count;

    m_subBatchInfo.reqId = gHZDataController->subBatchDyna(req
                                                           ,std::bind(&BaseStockDynaProvider::onStockBatchDataArrived
                                                                      ,this,std::placeholders::_1,std::placeholders::_2
                                                                      ,std::placeholders::_3,std::placeholders::_4));
}

void BaseStockDynaProvider::subOptionalData(const HZData::StockCode *stockCodes,int num)
{
    //取消上一次快照订阅
    cancelOptSub();
    if(m_model->rowCount()<1) return;

    HZData::ReqOptionalDynaInfo req;
    //订阅拉取的排行范围内股票
    int start = m_subBatchInfo.start;
    for(int i=0;i<num;i++){
        req.stock_code.push_back(stockCodes[i].stock_code);
        //qDebug()<<QString::fromStdString(stockCodes[i].stock_code);
    }
    //订阅字段类型
    req.mask = m_subOptInfo.stock_mask;
    OnStockDynaArrived  stockOptionalDataArrived = std::bind(&BaseStockDynaProvider::onStockOptionalDataArrived
                                                             ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);

    m_subOptInfo.reqId = gHZDataController->subOptionalDyna(req,stockOptionalDataArrived);

    //订阅起始位置和数目
    m_subOptInfo.start = start;
    m_subOptInfo.count= num;
}


void BaseStockDynaProvider::onStockBatchDataArrived(ReqId reqId,const char * data,int num,ErrCode errCode)
{
    qDebug() << "=========m_reqBatchId onCallBack=====: " <<"reqId:" <<  reqId << " m_reqBatchId: " << m_reqBatchId <<"m_isInitialed: " << m_isInitialed;
    if(errCode !=0) return;
    //第一次拉取全部排行初始化
    HZData::StockCode *stockCodes = (HZData::StockCode *)data;
    if(!m_isInitialed){
        m_isInitialed = true;
        m_model->initData(stockCodes,num);
    }
    else
        //开始订阅快照
        subOptionalData(stockCodes,num);
}

void BaseStockDynaProvider::onStockOptionalDataArrived(ReqId reqId,char *data,int num,ErrCode )
{
    if(num>0)
    //判断是第一次订阅返回
    m_model->updateData(data,m_subOptInfo.start,num, reqId ==m_subOptInfo.reqId);
}

