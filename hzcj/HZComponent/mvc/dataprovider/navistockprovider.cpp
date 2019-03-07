#include "navistockprovider.h"

#include <QDebug>

#include "datacontroller.h"

#include "hzuserdatamgr.h"
#include "zxstockopt.h"

#include "httpdata.h"

NaviStockProvider::NaviStockProvider(NaviStockDynaModel *model,QObject *parent)
    :QObject(parent), m_dataModel(model)
    ,m_actived(false),m_initialed(false)
{
    m_reqBatchInfo.mask = HZData::SLStock_Mask;

    m_t.setInterval(200);
    m_t.setSingleShot(true);
    connect(&m_t,&QTimer::timeout,this,&NaviStockProvider::slotSubTimeOut);
}

void NaviStockProvider::actived(bool enable)
{
    m_actived = enable;
    m_initialed  = false;
    cancelSub();
    if(m_actived)
        reqBatchStock();

}

void NaviStockProvider::setStockCode(const QString &stockCode)
{
    m_currentStock = stockCode;
}

void  NaviStockProvider::setFilterFlag(int filterFlag)
{
    m_reqBatchInfo.filter = filterFlag;
    //获取行情分类下股票个数
    m_reqBatchInfo.count_ange = Httpdata::instance()->getCountByFiter(filterFlag);
}

void  NaviStockProvider::sortIndicatorChanged(SortField sortField,bool is_desc)
{
    m_reqBatchInfo.sort_field = sortField;
    m_reqBatchInfo.is_desc   = is_desc;

    //qDebug()<<"NaviStockProvider sort field:"<<sortField<<"desc:"<<is_desc;
    if(m_actived){
        actived(true);
    }
}

void NaviStockProvider::currentRangeChanged(int start,int count)
{
    m_curRange.first = start;
    m_curRange.second = count;

    if(m_initialed)
        startSub();
}

void NaviStockProvider::cancelSub()
{
    if(m_subReqInfo.reqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqInfo.reqId);
        m_subReqInfo.reqId = INVALID_REQID;
    }
    if(m_reqBatchId != INVALID_REQID){
        gHZDataController->cancelSub(m_reqBatchId);
        m_reqBatchId = INVALID_REQID;
    }
}



void NaviStockProvider::startSub()
{
    cancelSub();
    m_t.start();
}

void NaviStockProvider::slotSubTimeOut()
{
    if(!m_actived) return;
    subData();
}

void NaviStockProvider::reqBatchStock()
{
    //先拉股票码
    m_reqBatchInfo.mask = HZData::StockCode_Mask;
    OnStockDynaArrived  stockDynaArrived = std::bind(&NaviStockProvider::onBatchDynaArrived
                                                     ,this,std::placeholders::_1,std::placeholders::_2
                                                     ,std::placeholders::_3,std::placeholders::_4);

    //    qDebug()<<m_reqBatchInfo.filter<<m_reqBatchInfo.sort_field<<m_reqBatchInfo.is_desc
    //           <<"req batch index:"<< m_reqBatchInfo.index<<"count:"<<m_reqBatchInfo.count_ange;
    m_reqBatchId =  gHZDataController->requestBatchDyna(m_reqBatchInfo,stockDynaArrived);
}

void NaviStockProvider::subData()
{
    int start = m_curRange.first - m_curRange.second/2;
    start = start<0?0:start;
    int end = start + m_curRange.second*2;
    end = end >(m_dataModel->rowCount()-1)?(m_dataModel->rowCount()-1):end;

    HZData::ReqBatchDynaInfo req = m_reqBatchInfo;
    req.mask = HZData::SLStock_Mask;
    req.index = start;
    req.count_ange = end - start +1;

    OnStockDynaArrived  stockDynaArrived = std::bind(&NaviStockProvider::onBatchDynaArrived
                                                     ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                                                     ,std::placeholders::_4);
    m_subReqInfo.reqId = gHZDataController->subBatchDyna(req,stockDynaArrived);
    m_subReqInfo.start   = start;
    m_subReqInfo.count = req.count_ange;
}

void NaviStockProvider::onBatchDynaArrived(ReqId,const char  *data,int num, ErrCode  errCode)
{
    if(errCode !=0)  return;

    if(!m_initialed){
        m_initialed = true;
        m_dataModel->initData(data,num);
    }else
        m_dataModel->updateData(data,m_subReqInfo.start,num);
}




////NaviOptionStockProvider----
void NaviOptionStockProvider::actived(bool enable)
{
    m_actived = enable;
    if(enable)
        subOptionalStock();
    else{
        cancelSub();
    }
}

void NaviOptionStockProvider::subOptionalStock()
{
    cancelSub();

    //获取股票
    QVector<QString> stocks =std::move(getStockVector());
    int size = stocks.size();

    //初始化数据
    HZData::ReqOptionalDynaInfo req;
    QVector<HZData::StockCode> stockCodes(size);
    for(int i=0;i<size;i++){
        stockCodes[i].stock_code = stocks[i].toStdString();
        req.stock_code.push_back(stockCodes[i].stock_code);
    }
    if(m_rowCount != size){
        m_rowCount = size;
        emit rowCountChanged(m_rowCount);
    }
    m_dataModel->initData(stockCodes);

    if(size<1) return;

    req.mask = HZData::SLStock_Mask;
    //开始订阅
    OnStockDynaArrived fun = std::bind(&NaviZXStockProvider::onOptionalStockArrived,this
                                       ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_subReqInfo.reqId = gHZDataController->subOptionalDyna(req,fun);
}

void NaviOptionStockProvider::onOptionalStockArrived(ReqId reqId,const char *data,int num,ErrCode errCode)
{
    if(errCode != 0) return;
    m_dataModel->updateData(data,0,num,reqId == m_subReqInfo.reqId);
}


////NaviZXStockProvider
NaviZXStockProvider::NaviZXStockProvider(NaviStockDynaModel *model,QObject *parent)
    :NaviOptionStockProvider(model,parent)
{
    connect(ZXStockOpt::instance(),&ZXStockOpt::stockCountChanged,this,&NaviZXStockProvider::slotStkCountChanged);
}

void NaviZXStockProvider::slotStkCountChanged(int count)
{
    if(m_actived){
        //发送股票数改变信号
        if(m_rowCount != count){
            m_rowCount = count;
            emit rowCountChanged(m_rowCount);
        }
        //重新订阅数据
         subOptionalStock();
    }
}

QVector<QString> NaviZXStockProvider::getStockVector()
{
    //获取自选股
    QVector<QString> stockCodes = ZXStockOpt::instance()->getZXStock();

    return  stockCodes;
}



////NaviHistStockProvider
QVector<QString> NaviHistStockProvider::getStockVector()
{
    return  LocalStockSource::globalHistInstance()->getStocksList();
}








///NaviIndexStockProvider
NaviIndexStockProvider::NaviIndexStockProvider(NaviStockDynaModel *model,QObject *parent)
    :NaviStockProvider(model, parent),m_isOptionalStock(false)
{
    //固定显示指数
    m_indexStocks =   \
    {"sh000001","sz399001","sz399005"
     ,"sz399006","sh000300","sh000016"
     ,"sh000132","sh000003","sh000012"
     ,"sh000013","sh000011","sz399106"
     ,"sz399004","sz399007","sz399008"
     ,"sz399100","sz399550","sh000903"};

    //577所有指数
    m_reqBatchInfo.filter = HZData::IndexInner;
}

void NaviIndexStockProvider::actived(bool enable)
{
    m_actived = enable;
    m_initialed  = false;

    cancelSub();
    if(m_actived){
        //        if(m_isOptionalStock)
        //            subOptionalIndex(); //订阅指定的指数
        //        else
        reqBatchStock(); //拉取所有指数排行
    }
}


void NaviIndexStockProvider::setStockCode(const QString &stockCode)
{
    //当前指数在固定列表中
    int index = m_indexStocks.indexOf(stockCode);

    //    m_isOptionalStock = index>=0?true:false;
}


void NaviIndexStockProvider::currentRangeChanged(int start,int count)
{
    //    if(!m_isOptionalStock){
    m_curRange.first = start;
    m_curRange.second = count;

    if(m_initialed)
        startSub();
    //    }
}


void NaviIndexStockProvider::subOptionalIndex()
{
    cancelSub();

    int size = m_indexStocks.size();
    //初始化数据
    HZData::ReqOptionalDynaInfo req;
    QVector<HZData::StockCode> stockCodes(size);
    for(int i=0;i<size;i++){
        stockCodes[i].stock_code = m_indexStocks[i].toStdString();
        req.stock_code.push_back(stockCodes[i].stock_code);
    }

    m_dataModel->initData(stockCodes);

    m_subReqInfo.start=0;
    m_subReqInfo.count=size;

    req.mask = HZData::SLStock_Mask;
    //开始订阅
    OnStockDynaArrived fun = std::bind(&NaviIndexStockProvider::onOptionalStockArrived,this
                                       ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                                       ,std::placeholders::_4);

    m_subReqInfo.reqId = gHZDataController->subOptionalDyna(req,fun);
}

void NaviIndexStockProvider::onOptionalStockArrived(ReqId reqId,const char *data,int num,ErrCode errCode)
{
    if(errCode != 0) return;

    m_dataModel->updateData(data,0,num,reqId == m_subReqInfo.reqId);
}



