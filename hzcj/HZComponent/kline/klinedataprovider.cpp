#include "klinedataprovider.h"
#include <qfile.h>
#include <QApplication>
#include <qdatetime.h>
#include <algorithm>
#include <qdebug.h>
#include <hzlog.h>
#include <hzobject.h>
#include <qdatetime.h>
#include "httpdata.h"



KlineDataProvider::KlineDataProvider(BaseChart* pchart):DataProvider(pchart)
{
    m_bLastSendedError = false;
    reqKlineInfo_.stock_name = "";
    m_bSubFirstPrush = false;
    m_reType = RestorationNone;
    m_rdReqId = -1;
}

KlineDataProvider::~KlineDataProvider()
{

}

qint64 strTimeToSeconds(const QString& strTime,QString format = "yyyy-MM-dd")
{
    qint64 nTime = 0;
    //2001-04-23 "yyyy:MM:dd hh:mm"
    QDateTime nt = QDateTime::fromString(strTime, format);
    if(nt.isValid())
       nTime =  nt.toTime_t();
    return nTime;
}

QString formatTime(quint32 t)
{
    QDateTime nt;
    nt.setTime_t(t);
    return nt.toString("yyyy-MM-dd hh:mm");
}

int KlineDataProvider::query(const string &req)
{
    int id = INT_MAX;
    QString key,msg;
    msg = req.c_str();
    MessageParams params;
    if(HZObject::parseMsg(msg,key,params))
    {
        if(key == KlineReqInfo || key == KlineSubInfo)
        {
            ReqKlineInfoEx reqInfo;
            int reType = RestorationNone;
            for(auto it = params.begin();it != params.end();it++)
            {
                if(it->first == "obj")
                {
                    reqInfo.stock_name =it->second.toStdString();
                }
                if(it->first == "period")
                {
                    reqInfo.kline_type = it->second.toInt();
                }
                if(it->first == "startTime")
                {
                    reqInfo.start_time = it->second.toInt();
                }

                if(it->first == "count")
                {
                    reqInfo.count = it->second.toInt();
                }
                if(it->first == "dirc")
                {
                    reqInfo.is_desc = it->second.toInt();
                }
                if(it->first == "reType")
                {
                    reType = it->second.toInt();
                }
            }
            if(reqInfo.stock_name != "")
            {
                if(false == reqKlineInfo_.stock_name.empty() && KlineSubInfo != key)
                {
                    //切换周期和合约 需要初始化相关变量
                    if(reqKlineInfo_.stock_name != reqInfo.stock_name || reqKlineInfo_.kline_type != reqInfo.kline_type)
                    {
                        resetId();
                    }else if(reqKlineInfo_.is_desc != reqInfo.is_desc)
                    {
                        reqKlineInfo_.endPoint = false;
                    }
                }
                m_drData = ChartModle::getInstance()->getDrData(reqInfo.stock_name.c_str());
                if(m_drData.isEmpty() && Httpdata::instance()->isHasObjFromFilter(reqInfo.stock_name.c_str(),HZData::MarketAll))
                {
                    StockDrListCallBack fun = std::bind(&KlineDataProvider::OnDirCallBack,this,std::placeholders::_1,std::placeholders::_2);
                    m_rdReqId = HzInfoCenter::getInstance()->reqStockDrList(reqInfo.stock_name.c_str(),fun);
                }
                m_reType =(RestorationType)reType;
                id = procesKlineReq(reqInfo,key);
            }
        }
    }
    return id;
}

//void KlineDataProvider::cancle(int id)
//{
//    DataController::instance()->cancelSub(id);
//    auto it  = reqMap_.find(id);
//    if(it !=  reqMap_.end())
//    {
//        reqMap_.erase(it);
//    }
//}

void KlineDataProvider::cancleEx()
{
    for(auto it = reqMap_.begin();it != reqMap_.end();it++)
    {
        DataController::instance()->cancelSub(it->first);
    }
    reqMap_.clear();
    HzInfoCenter::getInstance()->cancle(m_rdReqId);
}

int KlineDataProvider::getData(int offset,int count,DataKLine& vKlineData, double &max, double &min,int& maxIndex,int& minIndex)
{
    int nOffset = offset;
    DataKLine* data = getDataCache(m_reType)->pData;
    if(data && data->m_data.isEmpty() == false)
    {
        vKlineData.clearSig = data->clearSig;
        int end = data->m_data.size() + offset;
        if(end <= 0)
            return nOffset ;
        int begin = end - count;
        if(begin < 0)
            begin = 0;
        data->mid(begin,count,vKlineData);
        QVector<double>::iterator biggest = std::max_element(std::begin(vKlineData.m_highPrice), std::end(vKlineData.m_highPrice));
        QVector<double>::iterator littlegest = std::min_element(std::begin(vKlineData.m_lowPrice), std::end(vKlineData.m_lowPrice));
        max = *biggest;
        min = *littlegest;
        if(begin == 0)
        {
            nOffset =vKlineData.m_updateTime.size() - data->m_updateTime.size();
        }
        if(abs(max-min) < 0.001)
        {
            max = max+ max*0.018;
            min = min - min*0.018;
            minIndex = maxIndex = -1;
        }else
        {
            maxIndex = vKlineData.m_highPrice.indexOf(max);
            minIndex = vKlineData.m_lowPrice.indexOf(min);
        }
    }
    return nOffset;
}

void KlineDataProvider::getFlexCountDataBySub(DataKLine &vKlineData, double &max, double &min,int& maxIndex,int& minIndex)
{
    DataKLine* data = getDataCache(m_reType)->pData;
    int totalSize = data->m_updateTime.size();
    vKlineData.clearSig = data->clearSig;
    if(vKlineData.m_updateTime.size()>0 && totalSize > 0)
    {
        int vSize =vKlineData.m_updateTime.size();
        if(data->m_updateTime[totalSize-1] != vKlineData.m_updateTime[vSize-1]) //推送的数据不相等
        {
            int count = vKlineData.m_updateTime.size();
            vKlineData.clear();
            getData(0,count,vKlineData,max,min,maxIndex,minIndex);
        }else
        {
            vKlineData.m_data[vSize-1] = data->m_data[totalSize-1];
            vKlineData.m_tradingDay[vSize-1] = data->m_tradingDay[totalSize-1];
            vKlineData.m_updateTime[vSize-1] = data->m_updateTime[totalSize-1];
            vKlineData.m_upsertTime[vSize-1] = data->m_upsertTime[totalSize-1];
            vKlineData.m_highPrice[vSize-1] = data->m_highPrice[totalSize-1];
            vKlineData.m_openPrice[vSize-1] = data->m_openPrice[totalSize-1];
            vKlineData.m_lowPrice[vSize-1] = data->m_lowPrice[totalSize-1];
            vKlineData.m_closePrice[vSize-1] = data->m_closePrice[totalSize-1];
            vKlineData.m_volume[vSize-1] = data->m_volume[totalSize-1];
            vKlineData.m_turnover[vSize-1] = data->m_turnover[totalSize-1];
            vKlineData.m_preClosePrice[vSize-1] = data->m_preClosePrice[totalSize-1];
            vKlineData.m_averPrice[vSize-1] = data->m_averPrice[totalSize-1];
            if(vKlineData.m_lowPrice[vSize-1] < min)
            {
                min = vKlineData.m_lowPrice[vSize-1];
                minIndex = vSize-1;

            }
            if(vKlineData.m_highPrice[vSize-1] > max)
            {
                max = vKlineData.m_highPrice[vSize-1];
                maxIndex = -1;
            }
            if(abs(max-min) < 0.001)
            {
                max = max+ max*0.018;
                min = min - min*0.018;
                minIndex = maxIndex = -1;
            }
        }
    }else if(totalSize > 0)
    {
        vKlineData.append(*data);
        QVector<double>::iterator biggest = std::max_element(std::begin(vKlineData.m_highPrice), std::end(vKlineData.m_highPrice));
        QVector<double>::iterator littlegest = std::min_element(std::begin(vKlineData.m_lowPrice), std::end(vKlineData.m_lowPrice));
        max = *biggest;
        min = *littlegest;
        if(abs(max-min) < 0.001)
        {
            max = max+ max*0.018;
            min = min - min*0.018;
            minIndex = maxIndex = -1;
        }else
        {
            maxIndex = vKlineData.m_highPrice.indexOf(max);
            minIndex = vKlineData.m_lowPrice.indexOf(min);
        }
    }
}

int KlineDataProvider::getCacheSize()
{
    int ret = 0;
    DataKLine* data = getDataCache(m_reType)->pData;
    if(data != nullptr)
    {
        ret = data->m_data.size();
    }
    return ret;
}

int KlineDataProvider::dataEndPoint(bool dirc,const std::string& obj,int period)
{
    bool bRet = false;
    if(dirc == reqKlineInfo_.is_desc && reqKlineInfo_.stock_name == obj && reqKlineInfo_.kline_type == period)
        bRet = reqKlineInfo_.endPoint;
    return bRet;
}

void KlineDataProvider::OnDirCallBack(const InfoRet &ret, const StockDrList &data)
{
    if(ret.code == 0)
        ChartModle::getInstance()->setDrData(reqKlineInfo_.stock_name.c_str(),data);
}
void KlineDataProvider::OnKline(ReqId id, DataKLine *data, ErrCode code)
{
    qDebug() << "///////////////////////////////////////////////////////////" ;
    qDebug() << "OnKline::kline";
    qDebug() << "count: " << data->m_updateTime.size();
    qDebug() << "id :" << id;
    qDebug() << "///////////////////////////////////////////////////////////" ;
    auto it = reqMap_.find(id);
    if(it != reqMap_.end() || id == 0)
    {
        if(code == 0)
        {
            if(id != 0) //请求
            {
                qDebug() << "OnKline::kline req";
                if(data == nullptr || data->m_updateTime.empty())
                {
                    reqKlineInfo_.endPoint = true;
                    if(m_bLastSendedError == false)
                    {
                        sendDataResponse(id,KlineReqType);
                        m_bLastSendedError = true;
                    }
                    Log("KlineMinProvider","INFO","onKline req_line_id data is empty");
                }
                else
                {
                    mergeKlineWithDr(data,reqKlineInfo_.stock_name.c_str(),reqKlineInfo_.kline_type,false);
                    reqKlineInfo_.rspCount += data->m_updateTime.size();
                    if(data->m_updateTime.size() < it->second.count)//服务器数据没有了
                    {
                        reqKlineInfo_.endPoint = true;
                        qDebug() << "reqKlineInfo_.endPoint: " << reqKlineInfo_.endPoint;
                    }
                    reqMap_.erase(it);
                    if(reqKlineInfo_.rspCount == reqKlineInfo_.count || reqKlineInfo_.endPoint)
                    {
                        sendDataResponse(id,KlineReqType);
                        qDebug() << "sendDataResponse(id,REQ_KLINE_ID)";
                    }
                    else
                    {
                        qDebug() << "reqNextKline(data)";
                        reqNextKline(data);
                    }
                }
                m_bLastSendedError = false;
            }else
            {
                qDebug() << "OnKline::kline sub";
                if(!data->m_data.empty())
                {
                    DataKLine*  pCache = getDataCache(m_reType)->pData;
                    if(pCache && pCache->m_data.empty() == false && m_bSubFirstPrush == false)
                    {
                        int size = data->m_data.size();
                        if(data->m_data[size-1] == pCache->m_data[pCache->m_data.size()-1] ||
                           data->m_data[size-1].upsert_time < pCache->m_data[pCache->m_data.size()-1].upsert_time) //最后一根数据完全相等或之前数据就不通知刷新界面
                        {
                            return;
                        }
                    }
                    mergeKlineWithDr(data,reqKlineInfo_.stock_name.c_str(),reqKlineInfo_.kline_type,true);
                    sendDataResponse(id,KlineSubType);
                    m_bLastSendedError = false;
                    if(m_bSubFirstPrush)
                        m_bSubFirstPrush = false;
                }
            }
        }else
        {
            int c  = DataController::instance()->errCode();
            QString st = DataController::instance()->errString().c_str();
            qDebug() <<  "OnKline errCode:" << c;
            qDebug() << "OnKline errString:" << st;
            Log("KlineMinProvider","INFO","onKline error code :%d",code);
            if(m_bLastSendedError == false)
            {
                sendErrorResponse(id,code,DataController::instance()->errString().c_str());
                m_bLastSendedError = true;
            }
        }
    }
}

int KlineDataProvider::procesKlineReq(ReqKlineInfoEx info,const QString &key)
{
    int id = INT_MAX;
    OnKLineArrived  klineArrived = std::bind(&KlineDataProvider::OnKline,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    if(key == KlineReqInfo)
    {
        if(dataEndPoint(info.is_desc,info.stock_name,info.kline_type))
        {
            sendDataResponse(INT_MAX,KlineReqType);
            return INT_MAX;
        }
        DataKLine* data = getDataCache(m_reType)->pData;
        int index = -1;
        if(info.start_time != -1 && data && data->m_upsertTime.isEmpty() == false)
        {
            index = data->m_upsertTime.lastIndexOf(info.start_time);
        }
        info.count += IndiDependNum;
        if(index != -1)
        {
            if(!info.is_desc) //界面向左拖时 正方向请求
            {
                sendDataResponse(INT_MAX,KlineReqType);
                return INT_MAX;
                //如果当前数据够绘制界面 则不发生真正请求
                if(index + info.count  < data->m_upsertTime.size())
                {
                    qDebug() << "当前数据够绘制界面 则不发生真正请求 kline req time: " << formatTime(info.start_time);
                    sendDataResponse(INT_MAX,KlineReqType);
                    return INT_MAX;
                }else
                {
                    info.start_time = data->m_upsertTime[data->m_upsertTime.size()-1];
                }
            }else
            {
                //反方向请求（缩放/右拖/第一次请求）
                //本地缓存数据够计算K线和绘制或则如果服务器数据已经到头了不发生真正请求
                if(index + 1 - info.count >= 0)
                {
                    qDebug() << "本地缓存数据够计算K线和绘制或则如果服务器数据已经到头了不发生真正请求 kline req time: " << formatTime(info.start_time);
                    Log("KlineMinProvider","INFO","本地缓存数据够计算K线和绘制或则如果服务器数据已经到头了不发生真正请求 kline req time:  %d",index);
                    sendDataResponse(INT_MAX,KlineReqType);
                    return INT_MAX;
                }else
                {
                    info.start_time = data->m_upsertTime[0];
                }
            }
        }
        m_bLastSendedError = false;
        info.count += 200;//每次发生实际请求时，都多要2000根数据
        reqKlineInfo_ = info;
        if(info.count > KlineReqMaxCount)
            info.count = KlineReqMaxCount;
        id = DataController::instance()->requestKline(info ,klineArrived);
        qDebug() << "///////////////////////////////////////////////////////////" ;
        qDebug() << "procesKlineReq kline query: " ;
        qDebug() << "count: " << info.count;
        qDebug() << "dirc: " << info.is_desc;
        qDebug() << "kline_type: " << info.kline_type;
        qDebug() << "kline req time: " << formatTime(info.start_time);
        qDebug() << "id: " << id;
        qDebug() << "///////////////////////////////////////////////////////////" ;
        reqMap_[id] = info;
    }else
    {
        m_bSubFirstPrush = true;
        m_bLastSendedError = false;
        subKlineInfo_ = info;
        id = DataController::instance()->subKline(info,klineArrived);
        reqMap_[id] = info;
    }
    return id;
}

int KlineDataProvider::reqNextKline(DataKLine *data)
{
    ReqKlineInfoEx info = reqKlineInfo_;
    info.count = reqKlineInfo_.count-reqKlineInfo_.rspCount; //剩余还没有请求的根数
    if(info.count > KlineReqMaxCount)
        info.count = KlineReqMaxCount;
    if(!reqKlineInfo_.is_desc) //界面向左拖时 正方向请求
    {
        info.start_time = data->m_upsertTime[data->m_upsertTime.size()-1];
    }else
    {
        //反方向请求（缩放/右拖/第一次请求）
        info.start_time = data->m_upsertTime[0];
    }
    OnKLineArrived  klineArrived = std::bind(&KlineDataProvider::OnKline,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    int id = DataController::instance()->requestKline(info ,klineArrived);

    qDebug() << "///////////////////////////////////////////////////////////" ;
    qDebug() << "kline query next: " ;
    qDebug() << "count: " << info.count;
    qDebug() << "dirc: " << info.is_desc;
    qDebug() << "kline_type: " << info.kline_type;
    qDebug() << "kline req time: " << formatTime(info.start_time);
    qDebug() << "kline req id: " << id;
    qDebug() << "///////////////////////////////////////////////////////////" ;

    reqMap_[id] = info;
    return id;
}

void KlineDataProvider::resetId()
{
    reqKlineInfo_ = ReqKlineInfoEx();
    subKlineInfo_ = ReqKlineInfoEx();
    reqMap_.clear();
    clearCache();
}


