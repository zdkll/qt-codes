#include "minprovider.h"
#include <QVector>

#include "hzobject.h"
#include "chartmodle.h"
#include "hzlog.h"
#include "basechart.h"

MinProvider::MinProvider(BaseChart* pchart):DataProvider(pchart)
{
    m_bLastSendedError = false;
    m_bSubFirstPrush = false;
    //注册清盘
    OnRecvTradeDay fun = std::bind(&MinProvider::OnTradeChanged,this,std::placeholders::_1);
    DataController::instance()->subTradeDayMsg(fun);
}

MinProvider::~MinProvider()
{

}

int MinProvider::query(const std::string &req)
{
    MinProviderReqInfo minReq;
    QString key,msg;
    msg = req.c_str();
    MessageParams params;
    if(HZObject::parseMsg(msg,key,params))
    {
        if(key == KlineReqInfo || key == KlineSubInfo)
        {
            OnTimeKLineArrived fun = std::bind(&MinProvider::OnKline,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
            for(auto it = params.begin();it != params.end();it++)
            {
                if(it->first == "obj")
                {
                    minReq.info.stock_name =it->second.toStdString();
                }
                if(it->first == "day")
                {
                    minReq.info.start_time = it->second.toInt();
                }else if(it->first == "date")
                {
                    minReq.info.start_time = it->second.toInt();
                }
            }
            if(minReq.info.stock_name.empty() || minReq.info.start_time == INVALID_INT32)
                return minReq.id;
            if(key == KlineReqInfo)
            {
                qDebug() << "min KLINE_REQ_INF";
                reset();
                if(minReq.info.start_time < 0)
                {
                    int size = abs(minReq.info.start_time);
                    int id = INT_MAX;
                    HZData::ReqTimeKlineInfo req = minReq.info;
                    for(int i = size;i > 0;i--)
                    {
                        req.start_time = -i;
                        id = DataController::instance()->requestTimeKline(req,fun);
                        m_reqIdMap[id] = req;
                        if(i == 1)
                            minReq.id = id;
                    }
                }else
                {
                    minReq.id = DataController::instance()->requestTimeKline(minReq.info,fun);
                    m_reqIdMap[minReq.id] = minReq.info;
                }
                m_reqInfo = minReq;
            }else
            {
                qDebug() << "min KlineSubInfo";
                minReq.id = DataController::instance()->subTimeKline(minReq.info,fun);
                m_subInfo  = minReq;
            }
        }
    }
    qDebug() << minReq.info.stock_name.c_str();
    return minReq.id;
}

void MinProvider::cancle(int id)
{
    if(id == INT_MAX)
        return;
    if(m_reqInfo.id == id)
    {
        for(auto it = m_reqIdMap.begin();it != m_reqIdMap.end();it++)
        {
            DataController::instance()->cancelSub(it.key());
        }
        m_reqIdMap.clear();
        m_reqInfo = MinProviderReqInfo();
    }else if(m_subInfo.id == id)
    {
        DataController::instance()->cancelSub(id);
        m_subInfo = MinProviderReqInfo();
    }
}

QString formatTimeEx(quint32 t)
{
    QDateTime nt;
    nt.setTime_t(t);
    return nt.toString("yyyy-MM-dd hh:mm");
}

int MinProvider::getData(DataKLine &vKlineData, QVector<int> &dayIndexList, double &max, double &min,int nDay,bool bAvg)
{
    int retDay = 0;
    int reqTime = nDay;
    if(nDay > 0)
    {
        nDay = -1;
    }
    for(int i=nDay;i < 0;i++)
    {
        auto it = m_daysMap.find(reqTime > 0?reqTime:i);
        if(it == m_daysMap.end())
        {
            continue;
        }
        vKlineData.append(it->second);
        QVector<double>::iterator biggest = std::max_element(std::begin(vKlineData.m_highPrice), std::end(vKlineData.m_highPrice));
        QVector<double>::iterator littlegest = std::min_element(std::begin(vKlineData.m_lowPrice), std::end(vKlineData.m_lowPrice));
        if(i==nDay)
        {
            max = *biggest;
            min = *littlegest;
        }
        if(*littlegest < min)
        {
            min = *littlegest;
        }
        if(*biggest > max)
        {
            max = *biggest;
        }
        if(bAvg)
        {
            QVector<double>::iterator avgBiggest = std::max_element(std::begin(vKlineData.m_averPrice), std::end(vKlineData.m_averPrice));
            QVector<double>::iterator avgLittlegest = std::min_element(std::begin(vKlineData.m_averPrice), std::end(vKlineData.m_averPrice));
            if(*avgLittlegest < min)
            {
                min = *avgLittlegest;
            }
            if(*avgBiggest > max)
            {
                max = *avgBiggest;
            }
        }
        dayIndexList.push_back(vKlineData.m_updateTime.size()-1);
        retDay++;
    }
    return retDay==0?-1:-retDay;
}

void MinProvider::OnKline(ReqId id, DataKLine *data, ErrCode code)
{
    auto iter = m_reqIdMap.find(id);
    if(iter != m_reqIdMap.end() && id != 0) //请求
    {
        qDebug() << "OnKline::min req";
        if(code == 0)
        {
            if(data == nullptr || data->m_updateTime.empty())
            {
                Log("MinProvider","INFO","onKline req_line_id data is empty or reqIdMap do not finded");
                m_reqIdMap.erase(iter);
                if(m_reqIdMap.isEmpty())//请求都回来了后，发送界面更新推送（ID 对应之前请求返回给界面的ID）
                    sendDataResponse(m_reqInfo.id,KlineReqType);
            }
            else
            {
                int i = data->m_data.size()-1;
//                Log("MinProvider","INFO","onkine req id  %d",id);
//                Log("MinProvider","INFO","onkine begin pt");
//                Log("MinProvider","INFO","onkine trading_day:   %d",data->m_data[0].trading_day);
//                Log("MinProvider","INFO","onkine update_time: %s",formatTimeEx(data->m_data[0].update_time).data());
//                Log("MinProvider","INFO","onkine upsert_time: %s",formatTimeEx(data->m_data[0].upsert_time).data());
//                Log("MinProvider","INFO","onkine high_price:   %f",data->m_data[0].high_price);
//                Log("MinProvider","INFO","onkine open_price:   %f",data->m_data[0].open_price);
//                Log("MinProvider","INFO","onkine low_price:    %f",data->m_data[0].low_price);
//                Log("MinProvider","INFO","onkine close_price:  %f",data->m_data[0].close_price);
//                Log("MinProvider","INFO","onkine trading_day:  %f",data->m_data[0].trading_day);
//                Log("MinProvider","INFO","onkine pre_close_price:  %f",data->m_data[0].pre_close_price);

//                Log("MinProvider","INFO","onkine end pt: ");
//                Log("MinProvider","INFO","onkine trading_day:   %d",data->m_data[i].trading_day);
//                Log("MinProvider","INFO","onkine update_time: %s",formatTimeEx(data->m_data[i].update_time).data());
//                Log("MinProvider","INFO","onkine upsert_time: %s",formatTimeEx(data->m_data[i].upsert_time).data());
//                Log("MinProvider","INFO","onkine high_price:   %f",data->m_data[i].high_price);
//                Log("MinProvider","INFO","onkine open_price:   %f",data->m_data[i].open_price);
//                Log("MinProvider","INFO","onkine low_price:    %f",data->m_data[i].low_price);
//                Log("MinProvider","INFO","onkine close_price:  %f",data->m_data[i].close_price);
//                Log("MinProvider","INFO","onkine trading_day:  %f",data->m_data[i].trading_day);
//                Log("MinProvider","INFO","onkine pre_close_price:  %f",data->m_data[i].pre_close_price);
//                Log("MinProvider","INFO","onkline total size:   %d",data->m_data.size());
                m_bLastSendedError = false;
                m_daysMap[iter.value().start_time] = *data;
                m_reqIdMap.erase(iter);
                appendKline(data);
                if(m_reqIdMap.isEmpty())//请求都回来了后，发送界面更新推送（ID 对应之前请求返回给界面的ID）
                {

                    //mergeKline(data,m_reqInfo.info.stock_name.c_str(),HZData::KLINE_PERIOD_MIN,false);
                    sendDataResponse(m_reqInfo.id,KlineReqType);
                }
            }
        }else
        {
            Log("KlineMinProvider","INFO","onKline error code :%d",code);
            if(m_bLastSendedError == false)
            {
                sendErrorResponse(m_reqInfo.id,code,"msg"); //发送错误后推送客户端，Id对应之前请求返回给界面的ID
                m_bLastSendedError = true;
            }
        }
    }else if(0 == id) //订阅
    {
        if(data->clearSig)
        {
            m_daysMap.clear();
            clearCache();
            sendErrorResponse(m_subInfo.id,code,"清盘");
        }else if(data != nullptr  && data->m_data.empty() == false)
        {
            DataKLine*  pCache = NULL;
            auto it = m_daysMap.find(-1);//当天数据
            if(it != m_daysMap.end())
            {
                pCache = &it->second;
            }
            if(pCache && pCache->m_upsertTime.empty() == false)
            {
                int dataSize = pCache->m_data.size();
                int size = data->m_upsertTime.size();
                if(data->m_data[size-1] == pCache->m_data[dataSize-1] ||
                   data->m_data[size-1].upsert_time < pCache->m_data[dataSize-1].upsert_time) //最后一根数据完全相等或之前数据就不通知刷新界面
                {
                    if(m_bSubFirstPrush == true)
                    {
                        sendDataResponse(m_subInfo.id,KlineSubType);
                        m_bSubFirstPrush = false;
                    }
                    return;
                }else if(data->m_upsertTime[0] > pCache->m_upsertTime[dataSize-1])
                {
                    pCache->append(*data);
                }else
                {
                    int pos = -1;
                    for(int i=0;i < size ;i++)
                    {
                        pos =  pCache->m_upsertTime.lastIndexOf(data->m_upsertTime[i]);\
                        if(pos != -1)
                        {
                            pCache->m_data[pos] = data->m_data[i];
                            pCache->m_tradingDay[pos] = data->m_tradingDay[i];
                            pCache->m_updateTime[pos] = data->m_updateTime[i];
                            pCache->m_upsertTime[pos] = data->m_upsertTime[i];
                            pCache->m_highPrice[pos] = data->m_highPrice[i];
                            pCache->m_openPrice[pos] = data->m_openPrice[i];
                            pCache->m_lowPrice[pos] = data->m_lowPrice[i];
                            pCache->m_closePrice[pos] = data->m_closePrice[i];
                            pCache->m_volume[pos] = data->m_volume[i];
                            pCache->m_turnover[pos] = data->m_turnover[i];
                            pCache->m_preClosePrice[pos] = data->m_preClosePrice[i];
                            pCache->m_averPrice[pos] = data->m_averPrice[i];
                        }else if(data->m_upsertTime[i] > pCache->m_upsertTime[dataSize-1])
                        {
                            pCache->m_data.append(data->m_data[i]);
                            pCache->m_tradingDay.append(data->m_tradingDay[i]);
                            pCache->m_updateTime.append(data->m_updateTime[i]);
                            pCache->m_upsertTime.append(data->m_upsertTime[i]);
                            pCache->m_highPrice.append(data->m_highPrice[i]);
                            pCache->m_openPrice.append(data->m_openPrice[i]);
                            pCache->m_lowPrice.append(data->m_lowPrice[i]);
                            pCache->m_closePrice.append(data->m_closePrice[i]);
                            pCache->m_volume.append(data->m_volume[i]);
                            pCache->m_turnover.append(data->m_turnover[i]);
                            pCache->m_preClosePrice.append(data->m_preClosePrice[i]);
                            pCache->m_averPrice.append(data->m_averPrice[i]);
                        }
                    }
                }
                mergeKline(data,m_subInfo.info.stock_name.c_str(),HZData::KLINE_PERIOD_MIN,true);
            }else
            {
                m_daysMap[-1] = *data;
            }
            sendDataResponse(m_subInfo.id,KlineSubType);
            m_bLastSendedError = false;
            if(m_bSubFirstPrush)
                m_bSubFirstPrush = false;
        }
    }
}

void MinProvider::OnTradeChanged(const HZData::TradeDayMsg &data)
{
    if(m_chart && m_chart->getCanvasWnd()->isVisible())
    {
        qDebug() << "MinProvider::OnTradeChanged";
        Log("KlineMinProvider","INFO","MinProvider::OnTradeChanged");
        m_daysMap.clear();
        clearCache();
        sendErrorResponse(m_subInfo.id,0,"清盘");
    }
}

void MinProvider::reset()
{
    m_reqIdMap.clear();
    m_daysMap.clear();
    m_subInfo = MinProviderReqInfo();
    m_reqInfo = MinProviderReqInfo();
    m_bLastSendedError = false;
    m_bSubFirstPrush = true;
    clearCache();
}

//double MinProvider::getDateValue(double times)
//{
//    double trade = DBL_MAX;
//    QDateTime dateTime;
//    dateTime.setTime_t(times);
//    QString strTime = dateTime.toString("yyyy-MM-dd");
//    dateTime = QDateTime::fromString(strTime,"yyyy-MM-dd");
//    trade = dateTime.toTime_t();
//    return trade;
//}
