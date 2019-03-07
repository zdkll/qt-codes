#include "handdataprovider.h"

#include "fundleadernetputmodel.h"
#include "pricetickmodel.h"

HandDataProvider::HandDataProvider()
    :m_actived(false),m_subReqId(INVALID_REQID)
    ,m_showCount(0),m_initialed(false)
{
    m_dataScope.first = m_dataScope.second = -1;
}

void HandDataProvider::actived(bool active)
{
    m_actived = active;
    m_dataScope.first = m_dataScope.second = -1;
    m_initialed = false;
    cancelSub();
    if(m_actived)
        reqData();
    else
        clearData();
}

void  HandDataProvider::setStockCode(const QString &stockCode)
{
    m_stockCode = stockCode;
}

void HandDataProvider::setShowCount(const int &count)
{
    m_showCount = count;
    m_dataScope.first = m_dataScope.second = -1;
    m_initialed = false;

    if(m_actived)
        reqData();
}

void HandDataProvider::cancelSub()
{
    if(m_subReqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}


///LeaderNetputProvider---
LeaderNetputProvider::LeaderNetputProvider(FundLeaderNetputModel *model)
    :HandDataProvider(),m_model(model)
{

}

void LeaderNetputProvider::reqData()
{
    HZData::ReqTickInfo req;
    req.stock_code = m_stockCode.toStdString();
    req.update_time = -1;
    req.count    = m_showCount;
    req.is_desc = true;
    req.zb_type = HZData::zhubi_type_money;
    gHZDataController->requestTick(req,std::bind(&LeaderNetputProvider::leaderNetputArrived
                                                 ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}

void LeaderNetputProvider::clearData()
{
    m_model->clearData();
}

void LeaderNetputProvider::reqPrevData()
{
    //数据足够，设置数据显示返回
    //    if((m_dataScope.second - m_dataScope.first+1)>m_showCount)
    //        return updateData();
}

void LeaderNetputProvider::subLeaderNetput()
{
    cancelSub();
    HZData::ReqTickInfo req;

    req.stock_code = m_stockCode.toStdString();
    req.update_time = -1;
    req.count    =  2;
    req.is_desc = true;
    req.zb_type = HZData::zhubi_type_money;
    m_subReqId = gHZDataController->subTick(req,std::bind(&LeaderNetputProvider::subLeaderNetputArrived
                                                          ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}

void LeaderNetputProvider::leaderNetputArrived(ReqId ,const  HZData::Zhubi& zhubi, ErrCode errCode)
{
    if(errCode != 0) return;

    if(!m_initialed){
        m_zhubi.totalCount = zhubi.totalCount;
        m_zhubi.ticks = std::move(zhubi.ticks);
        //第一次数据到达,设置数据范围
        m_dataScope.first       = m_zhubi.totalCount-m_zhubi.ticks.size();
        m_dataScope.second = m_zhubi.totalCount-1;

        //开始订阅
        subLeaderNetput();
    }else{
        m_zhubi.totalCount = zhubi.totalCount;
        m_zhubi.ticks =  zhubi.ticks +m_zhubi.ticks;

        m_dataScope.first    = m_zhubi.totalCount-m_zhubi.ticks.size();
    }

    updateData();

    //尝试继续向前拉取数据
    //    reqPrevData();
}

void LeaderNetputProvider::subLeaderNetputArrived(ReqId  ,const  HZData::Zhubi& zhubi, ErrCode errCode)
{
    if(errCode != 0) return;
    //判断是否有新增(或者清盘变少)条目-最一条
    if(zhubi.totalCount != m_zhubi.totalCount){
        int incNum = zhubi.totalCount -  m_zhubi.totalCount;
        if(incNum > zhubi.ticks.size()){
            qDebug()<<"Miss sub LeaderNetput=====";
        }
        if(incNum>0){
            int idx = zhubi.ticks.size() - incNum;
            idx = idx <0?0:idx;
            for(;idx<zhubi.ticks.size();idx++){
                m_zhubi.ticks.append(zhubi.ticks[idx]);
            }
            m_zhubi.totalCount = zhubi.totalCount;
            updateData();
        }else{ //变少，需要重新拉取
            actived(false);
            actived(true);
        }
    }
}

void LeaderNetputProvider::updateData()
{
    if(m_zhubi.ticks.size()>0){
        int index  = m_zhubi.ticks.size() - m_showCount;
        index = index<0?0:index;
        m_model->initData(m_zhubi.ticks.mid(index));
    }
}




///PriceTickProvider----
PriceTickProvider::PriceTickProvider(PriceTickModel *model)
    :m_model(model)
{
    m_curRange.first = m_curRange.second = 0;
    m_showCount = 0;
}

void PriceTickProvider::setShowCount(const int &count)
{
    m_showCount =  count;
    if(m_initialed)
        reqData();
}

void PriceTickProvider::currentRangeChange(int start ,int count)
{
    if(!m_initialed) return;
    m_curRange.first      = start;
    m_curRange.second = count;
    qDebug()<<"current Range changed:"<<start<<count;
    reqData();
}

void PriceTickProvider::reqData()
{
    cancelSub();

    m_reqPriceTickInfo.stock_code = m_stockCode.toStdString();
    m_reqPriceTickInfo.index  = m_curRange.first ;
    m_reqPriceTickInfo.count =  m_showCount+1; //按照显示范围条数去拉取，目的是能保证增加条数及时获取
    m_subReqId = gHZDataController->subPriceTickInfo(m_reqPriceTickInfo,std::bind(&PriceTickProvider::priceTickInfoArrived,this
                                                                                  ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}

void PriceTickProvider::clearData()
{
    m_model->clearData();
}

void PriceTickProvider::priceTickInfoArrived(ReqId , const HZData::PriceTickInfo& priceTickInfo, ErrCode errCode)
{
    if(errCode != 0) return;
    if(!m_initialed){
        m_initialed = true;
        m_model->initData(priceTickInfo,m_reqPriceTickInfo.index);
    }else
        m_model->updateData(priceTickInfo,m_reqPriceTickInfo.index);
}

