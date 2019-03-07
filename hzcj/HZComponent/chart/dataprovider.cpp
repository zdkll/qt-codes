#include "dataprovider.h"
#include <thread>
#include "basechart.h"


DataProvider::DataProvider(BaseChart* pchart):m_chart(pchart)
{
}

void DataProvider::cancle(int id)
{

}

int DataProvider::query(const std::string &req)
{
    return -1;
}

KlineData *DataProvider::getDataCache(RestorationType type)
{
    KlineData* pData;
    if(type == RestorationNone)
        pData = &m_klineData;
    else if(type == RestorationBeforType)
        pData = &m_drForwardKlineData;
    else if(type == RestorationBackType)
        pData = &m_drBackwardklineData;
    return pData;
}

void DataProvider::sendDataResponse(int id,int type)
{
    if(m_chart)
        m_chart->dataResponse(id,type);
}

void DataProvider::sendErrorResponse(int id, int ncode, const QString &errReuslt)
{
    if(m_chart)
        m_chart->errorResponse(id,ncode,errReuslt);
}

IndiCalcData *DataProvider::toIndiChache(DataKLine *pData)
{
    IndiCalcData* pIndiData = nullptr;
    if(pData)
    {
        pIndiData = new IndiCalcData();
        pIndiData->Resize(pData->m_updateTime.size());
        MemBlock::fillBlock(pIndiData->pTime,0,(char*)pData->m_updateTime.data(),sizeof(double)*pData->m_updateTime.size());
        MemBlock::fillBlock(pIndiData->pHigh,0,(char*)pData->m_highPrice.data(),sizeof(double)*pData->m_highPrice.size());
        MemBlock::fillBlock(pIndiData->pLow,0,(char*)pData->m_lowPrice.data(),sizeof(double)*pData->m_lowPrice.size());
        MemBlock::fillBlock(pIndiData->pClose,0,(char*)pData->m_closePrice.data(),sizeof(double)*pData->m_closePrice.size());
        MemBlock::fillBlock(pIndiData->pOpen,0,(char*)pData->m_openPrice.data(),sizeof(double)*pData->m_openPrice.size());
        MemBlock::fillBlock(pIndiData->pVolume,0,(char*)pData->m_volume.data(),sizeof(double)*pData->m_volume.size());
        MemBlock::fillBlock(pIndiData->pAmount,0,(char*)pData->m_turnover.data(),sizeof(double)*pData->m_turnover.size());
    }
    return pIndiData;
}

void DataProvider::mergeKline(DataKLine *src, const QString &obj, int period, bool bPush)
{
    mergeKline(src,m_klineData,obj,period,bPush);
}

void DataProvider::mergeKlineWithDr(DataKLine *src, const QString &obj, int period, bool bPush)
{
    mergeKline(src,m_klineData,obj,period,bPush);
    DataKLine drForward = *src,drBackward=*src;
    drCalc(drForward,drBackward,obj);
//    forwardDrCalc(drForward,obj);
//    backwardDrCalc(drBackward,obj);
    mergeKline(&drForward,m_drForwardKlineData,obj,period,bPush);
    mergeKline(&drBackward,m_drBackwardklineData,obj,period,bPush);
}

void DataProvider::mergeKline(DataKLine *src,KlineData& klineData, const QString &obj, int period,bool bPush)
{
    if(src && src->m_upsertTime.isEmpty() == false)
    {
        if(klineData.pData == nullptr)
        {

            klineData.pData = new DataKLine();
            *klineData.pData = *src;
            klineData.pInidCalcData = toIndiChache(klineData.pData);
            return;
        }

        int srcSize = src->m_upsertTime.size();
        int dataSize = klineData.pData->m_upsertTime.size();
        if(bPush)
        {
            if(src->m_data[srcSize-1] == klineData.pData->m_data[dataSize-1] ||
               src->m_data[srcSize-1].upsert_time < klineData.pData->m_data[dataSize-1].upsert_time)
            {
                //推送，且最后一根数据相等，退出
                return;
            }else if(src->m_upsertTime[0] > klineData.pData->m_upsertTime[dataSize-1])
            {
                klineData.pData->append(*src);
            }else
            {
                int pos = -1;
                for(int i=0;i < srcSize;i++)
                {
                    pos =  klineData.pData->m_upsertTime.lastIndexOf(src->m_upsertTime[i]);\
                    if(pos != -1)
                    {
                        klineData.pData->m_data[pos] = src->m_data[i];
                        klineData.pData->m_tradingDay[pos] = src->m_tradingDay[i];
                        klineData.pData->m_updateTime[pos] = src->m_updateTime[i];
                        klineData.pData->m_upsertTime[pos] = src->m_upsertTime[i];
                        klineData.pData->m_highPrice[pos] = src->m_highPrice[i];
                        klineData.pData->m_openPrice[pos] = src->m_openPrice[i];
                        klineData.pData->m_lowPrice[pos] = src->m_lowPrice[i];
                        klineData.pData->m_closePrice[pos] = src->m_closePrice[i];
                        klineData.pData->m_volume[pos] = src->m_volume[i];
                        klineData.pData->m_turnover[pos] = src->m_turnover[i];
                        klineData.pData->m_preClosePrice[pos] = src->m_preClosePrice[i];
                        klineData.pData->m_averPrice[pos] = src->m_averPrice[i];
                    }else if(src->m_upsertTime[i] > klineData.pData->m_upsertTime[dataSize-1])
                    {
                        klineData.pData->m_data.append(src->m_data[i]);
                        klineData.pData->m_tradingDay.append(src->m_tradingDay[i]);
                        klineData.pData->m_updateTime.append(src->m_updateTime[i]);
                        klineData.pData->m_upsertTime.append(src->m_upsertTime[i]);
                        klineData.pData->m_highPrice.append(src->m_highPrice[i]);
                        klineData.pData->m_openPrice.append(src->m_openPrice[i]);
                        klineData.pData->m_lowPrice.append(src->m_lowPrice[i]);
                        klineData.pData->m_closePrice.append(src->m_closePrice[i]);
                        klineData.pData->m_volume.append(src->m_volume[i]);
                        klineData.pData->m_turnover.append(src->m_turnover[i]);
                        klineData.pData->m_preClosePrice.append(src->m_preClosePrice[i]);
                        klineData.pData->m_averPrice.append(src->m_averPrice[i]);
                    }
                }
            }
            klineData.pData->clearSig = src->clearSig;
            if(klineData.pInidCalcData)
            {
                delete klineData.pInidCalcData;
                klineData.pInidCalcData =nullptr;
            }
            klineData.pInidCalcData = toIndiChache(klineData.pData);
            return;
        }

        // 新请求的数据完全包含现有缓存中的数据  使用新请求的数据替换现有缓存中的数据
        if(src->m_upsertTime[0] < klineData.pData->m_upsertTime[0] && src->m_upsertTime[srcSize-1] > klineData.pData->m_upsertTime[dataSize-1])
        {
            //Log("KlineMinProvider","INFO","mergeKline 新请求的数据完全包含现有缓存中的数据  使用新请求的数据替换现有缓存中的数据");
            klineData.pInidCalcData->Release();
            delete klineData.pData;
            delete klineData.pInidCalcData;
            klineData.pData = nullptr;
            klineData.pInidCalcData = nullptr;
            klineData.pData = new DataKLine();
            *klineData.pData = *src;
            klineData.pInidCalcData = toIndiChache(klineData.pData);
            return;
        }else if(src->m_upsertTime[0] < klineData.pData->m_upsertTime[0])
        {
            //推送的的数据时间晚于缓存中的数据   向前请求数据
            //Log("KlineMinProvider","INFO","mergeKline 推送的的数据时间晚于缓存中的数据   向前请求数据");
            int nIndex = src->m_upsertTime.lastIndexOf(klineData.pData->m_upsertTime[0]);
//            if(nIndex == -1)
//                return;
            QVector<HZData::HZKline> data;
            QVector<string>  tradingDay;
            QVector<double> updateTime;
            QVector<double> upsertTime;
            QVector<double>  highPrice;
            QVector<double>  openPrice ;
            QVector<double>	 lowPrice;
            QVector<double>  closePrice	;
            QVector<double>   volume ;
            QVector<double>	 turnover ;
            QVector<double>   preClosePrice ;
            QVector<double>  averPrice ;

            data.reserve(srcSize+dataSize);
            tradingDay.reserve(srcSize+dataSize);
            updateTime.reserve(srcSize+dataSize);
            upsertTime.reserve(srcSize+dataSize);
            highPrice.reserve(srcSize+dataSize);
            openPrice.reserve(srcSize+dataSize);
            lowPrice.reserve(srcSize+dataSize);
            closePrice.reserve(srcSize+dataSize);
            volume.reserve(srcSize+dataSize);
            turnover.reserve(srcSize+dataSize);
            preClosePrice.reserve(srcSize+dataSize);
            averPrice.reserve(srcSize+dataSize);

            data.append(src->m_data.mid(0,nIndex));
            data.append(klineData.pData->m_data);
            klineData.pData->m_data.swap(data);

            tradingDay.append(src->m_tradingDay.mid(0,nIndex));
            tradingDay.append(klineData.pData->m_tradingDay);
            klineData.pData->m_tradingDay.swap(tradingDay);

            upsertTime.append(src->m_upsertTime.mid(0,nIndex));
            upsertTime.append(klineData.pData->m_upsertTime);
            klineData.pData->m_upsertTime.swap(upsertTime);

            updateTime.append(src->m_updateTime.mid(0,nIndex));
            updateTime.append(klineData.pData->m_updateTime);
            klineData.pData->m_updateTime.swap(updateTime);


            highPrice.append(src->m_highPrice.mid(0,nIndex));
            highPrice.append(klineData.pData->m_highPrice);
            klineData.pData->m_highPrice.swap(highPrice);

            openPrice.append(src->m_openPrice.mid(0,nIndex));
            openPrice.append(klineData.pData->m_openPrice);
            klineData.pData->m_openPrice.swap(openPrice);

            lowPrice.append(src->m_lowPrice.mid(0,nIndex));
            lowPrice.append(klineData.pData->m_lowPrice);
            klineData.pData->m_lowPrice.swap(lowPrice);

            closePrice.append(src->m_closePrice.mid(0,nIndex));
            closePrice.append(klineData.pData->m_closePrice);
            klineData.pData->m_closePrice.swap(closePrice);

            volume.append(src->m_volume.mid(0,nIndex));
            volume.append(klineData.pData->m_volume);
            klineData.pData->m_volume.swap(volume);

            turnover.append(src->m_turnover.mid(0,nIndex));
            turnover.append(klineData.pData->m_turnover);
            klineData.pData->m_turnover.swap(turnover);

            preClosePrice.append(src->m_preClosePrice.mid(0,nIndex));
            preClosePrice.append(klineData.pData->m_preClosePrice);
            klineData.pData->m_preClosePrice.swap(preClosePrice);

            averPrice.append(src->m_averPrice.mid(0,nIndex));
            averPrice.append(klineData.pData->m_averPrice);
            klineData.pData->m_averPrice.swap(averPrice);
            klineData.pInidCalcData->Release();
            delete klineData.pInidCalcData;
            klineData.pInidCalcData = nullptr;
            klineData.pInidCalcData = toIndiChache(klineData.pData);
            klineData.pData->clearSig = src->clearSig;
        }
        else if(src->m_upsertTime[srcSize-1] > klineData.pData->m_upsertTime[dataSize-1])
        {
            //推送的数据时间部分早于缓存中的数据
            //Log("KlineMinProvider","INFO","mergeKline 推送的数据时间部分早于缓存中的数据");
            for(int i=0;i < srcSize;i++)
            {
                dataSize = klineData.pData->m_upsertTime.size();
                if(src->m_upsertTime[i] <= klineData.pData->m_upsertTime[dataSize-1])
                    break;
                klineData.pData->m_data.append(src->m_data[i]);
                klineData.pData->m_tradingDay.append(src->m_tradingDay[i]);
                klineData.pData->m_updateTime.append(src->m_updateTime[i]);
                klineData.pData->m_upsertTime.append(src->m_upsertTime[i]);
                klineData.pData->m_highPrice.append(src->m_highPrice[i]);
                klineData.pData->m_openPrice.append(src->m_openPrice[i]);
                klineData.pData->m_lowPrice.append(src->m_lowPrice[i]);
                klineData.pData->m_closePrice.append(src->m_closePrice[i]);
                klineData.pData->m_volume.append(src->m_volume[i]);
                klineData.pData->m_turnover.append(src->m_turnover[i]);
                klineData.pData->m_preClosePrice.append(src->m_preClosePrice[i]);
                klineData.pData->m_averPrice.append(src->m_averPrice[i]);
            }
            klineData.pData->clearSig = src->clearSig;
            if(klineData.pInidCalcData)
            {
                delete klineData.pInidCalcData;
                klineData.pInidCalcData =nullptr;
            }
            klineData.pInidCalcData = toIndiChache(klineData.pData);
        }
    }
}

void DataProvider::appendKline(DataKLine *src)
{
    append(src,m_klineData);
}

void DataProvider::appendDrforwardKline(DataKLine *src)
{
    append(src,m_drForwardKlineData);
}

void DataProvider::appendDrBackKline(DataKLine *src)
{
    append(src,m_drBackwardklineData);
}

void DataProvider::clearCache()
{
    clear(m_klineData);
    clear(m_drForwardKlineData);
    clear(m_drBackwardklineData);
}

void DataProvider::clear(KlineData &data)
{
    if(data.pData)
    {
        delete data.pData;
        data.pData = nullptr;
    }
    if(data.pInidCalcData)
    {
        delete data.pInidCalcData;
        data.pInidCalcData = nullptr;
    }
}

void DataProvider::append(DataKLine *src, KlineData &data)
{
    if(src && src->m_upsertTime.isEmpty() == false)
    {
        if(data.pData == nullptr)
        {

            data.pData = new DataKLine();
            *data.pData = *src;
            data.pInidCalcData = toIndiChache(data.pData);
            return;
        }
        data.pData->append(*src);
        data.pData->clearSig = src->clearSig;
        if(data.pInidCalcData)
        {
            delete data.pInidCalcData;
            data.pInidCalcData =nullptr;
        }
        data.pInidCalcData = toIndiChache(data.pData);
    }
}
#include <QDateTime>
QString formatTime(long long time)
{
    QDateTime nt;
    nt.setTime_t(time);
    return nt.toString("yyyy-MM-dd hh:mm");
}

struct DivisorItem
{
    int index; //数据所在下标
    double divisor;  //当前复权因子
};

void convertDrTimeToPeriod(StockDrList& drData,int period)
{
    QString strType = "";
    switch (period) {
    case HZData::KLINE_PERIOD_MIN:
        strType = QStringLiteral("1分钟");
        break;
    case HZData::KLINE_PERIOD_5MIN:
        strType = QStringLiteral("5分钟");
        break;
    case HZData::KLINE_PERIOD_15MIN:
        strType = QStringLiteral("15分钟");
        break;
    case HZData::KLINE_PERIOD_HOUR:
        strType = QStringLiteral("60分钟");
        break;
    case HZData::KLINE_PERIOD_DAY:
        strType = QStringLiteral("日K");
        break;
    case HZData::KLINE_PERIOD_WEEK:
        strType = QStringLiteral("周K");
        break;
    case HZData::KLINE_PERIOD_MONTH:
        strType = QStringLiteral("月K");
        break;
    }
}

void DataProvider::drCalc(DataKLine &fdata, DataKLine &bdata, const QString &obj)
{
    StockDrList drData = ChartModle::getInstance()->getDrData(obj);
    if(drData.isEmpty())
        return;
    int size = fdata.m_upsertTime.size();
    int drIndex = drData.size();
    double mul=0,add=0;
    bool bFowardCalc = false;
    bool bBackCalc = false;
    //复权数据： 下标0表示历史时间除权
    //K线数据： 下标0表示历史数据
    for(int i=0;i < size;i++)
    {
        bFowardCalc = false;
        bBackCalc = false;
        int fStep=0;
        int bStep=drIndex-1;
        for(int j =0;j < drIndex;j++,fStep++,bStep--)
        {
            if(bFowardCalc == false && fdata.m_upsertTime[i] < drData[fStep].cqcxr) //最新除权点前的数据不除权
            {
                bFowardCalc =true;
                mul = drData[fStep].multiply_forward;
                add = drData[fStep].add_forward;
                if(mul != 0)
                {
                    fdata.m_data[i].close_price = fdata.m_closePrice[i] =  fdata.m_closePrice[i]*mul+add;
                    fdata.m_data[i].open_price = fdata.m_openPrice[i] =  fdata.m_openPrice[i] *mul+add;
                    fdata.m_data[i].high_price = fdata.m_highPrice[i] =  fdata.m_highPrice[i]*mul+add;
                    fdata.m_data[i].low_price = fdata.m_lowPrice[i] = fdata.m_lowPrice[i]*mul+add;
                }
            }
            if(bBackCalc == false && bdata.m_upsertTime[i] >= drData[bStep].cqcxr) //历史数据处于第一次除权之前的数据不除权
            {
                bBackCalc = true;
                mul = drData[bStep].multiply_backward;
                add = drData[bStep].add_backward;
                if(mul != 0)
                {
                    bdata.m_data[i].close_price = bdata.m_closePrice[i] = bdata.m_closePrice[i]  *mul+add;
                    bdata.m_data[i].open_price = bdata.m_openPrice[i] = bdata.m_openPrice[i] *mul+add;
                    bdata.m_data[i].high_price = bdata.m_highPrice[i] = bdata.m_highPrice[i] *mul+add;
                    bdata.m_data[i].low_price = bdata.m_lowPrice[i] =bdata.m_lowPrice[i] *mul+add;
                }
            }
        }
    }



    /*
       前后复权规则：
       基础公式：
       前复权：复权后价格=[(复权前价格-现金红利[给过来])+股价配(新)格[当前价格]×流通股份变动比例[确定计算方式  ]]÷(1+流通股份变动比例)
       后复权：复权后价格=复权前价格×(1+流通股份变动比例)-配(新)股价格×流通股份变动比例+现金红利

       现金红利 : 10派5 红利就是0.5


       流变通股份动比例公式：流动股份变动比例=(现在流动股份—原流动股份)/原流动股份
       例如 10送5 10转2 10配1  流通比例 0.8

       叠加计算规则：
       1 根据除权时间点： 叠加计算每个除权点的数据。
       2 前复权：从当前复权点的前一个数据开始 当前复权时间-----到历史数据的叠加计算 （后一条特别重要，否则你无法和东财数据保持一致）
       3 后复权：从当前复权点开始。 历史时间点------到最近数据的叠加计算 （后一条特别重要，否则你无法和东财数据保持一致）

       学习网站：
          基础知识：https://baijiahao.baidu.com/s?id=1592887943251348333&wfr=spider&for=pc
          算法：https://wenku.baidu.com/view/4238ae41a8956bec0975e3c0.html
    */
//    StockDrList drData = ChartModle::getInstance()->getDrData(obj);
//    if(drData.isEmpty())
//        return;
//    int size = fdata.m_upsertTime.size();
//    int drIndex = drData.size()-1;
//    double fclose,fopen,fhigh,flow;
//    double bclose,bopen,bhigh,blow;
//    double cashDiv ,prechange;
//    double pxgj = drData[0].pxgj;
//    int fStep=drIndex;
//    int bStep=0;
//    //复权数据： 下标0表示最近时间除权
//    //K线数据： 下标0表示历史数据
//    for(int i=0;i < size;i++)
//    {
//        //qDebug() << "forwardDrCalc: kline time: " << formatTime(data.m_upsertTime[i]);
//        fclose = fdata.m_closePrice[i];
//        fopen = fdata.m_openPrice[i];
//        fhigh = fdata.m_highPrice[i];
//        flow = fdata.m_lowPrice[i];

//        bclose = bdata.m_closePrice[i];
//        bopen = bdata.m_openPrice[i];
//        bhigh = bdata.m_highPrice[i];
//        blow = bdata.m_lowPrice[i];
//        //特别注意，为了和东财保持一致，后复权计算必须从历史除权点往最新除权点方向递归计算 否则有误差
//        fStep=drIndex;
//        bStep=0;
//        for(int j =0;j <= drIndex;j++,fStep--,bStep++)
//        {
//            if(fdata.m_upsertTime[i] < drData[fStep].cqcxr && fdata.m_upsertTime[i] <= drData[0].cqcxr) //最新除权点前的数据不除权
//            {
//                cashDiv = drData[fStep].cashDividends;
//                prechange = drData[fStep].percentageChange;
//                pxgj = drData[fStep].pxgj;
//                fclose  = ((fclose-cashDiv)+pxgj*prechange)/(1+prechange);
//                fopen = ((fopen-cashDiv)+pxgj*prechange)/(1+prechange);
//                fhigh = ((fhigh-cashDiv)+pxgj*prechange)/(1+prechange);
//                flow  = ((flow-cashDiv)+pxgj*prechange)/(1+prechange);
//            }
//            if(bdata.m_upsertTime[i] > drData[bStep].cqcxr && bdata.m_upsertTime[i] >= drData[drIndex].cqcxr) //历史数据处于第一次除权之前的数据不除权
//            {
//                cashDiv = drData[bStep].cashDividends;
//                prechange = drData[bStep].percentageChange;
//                pxgj = drData[bStep].pxgj;
//                bclose = bclose*(1+prechange)-pxgj*prechange+cashDiv;
//                bopen = bopen*(1+prechange)-pxgj*prechange+cashDiv;
//                bhigh = bhigh*(1+prechange)-pxgj*prechange+cashDiv;
//                blow = blow*(1+prechange)-pxgj*prechange+cashDiv;
//            }
//        }
//        fdata.m_data[i].close_price = fdata.m_closePrice[i] = fclose;
//        fdata.m_data[i].open_price = fdata.m_openPrice[i] = fopen;
//        fdata.m_data[i].high_price = fdata.m_highPrice[i] = fhigh;
//        fdata.m_data[i].low_price = fdata.m_lowPrice[i] = flow;

//        bdata.m_data[i].close_price = bdata.m_closePrice[i] = bclose;
//        bdata.m_data[i].open_price = bdata.m_openPrice[i] = bopen;
//        bdata.m_data[i].high_price = bdata.m_highPrice[i] = bhigh;
//        bdata.m_data[i].low_price = bdata.m_lowPrice[i] = blow;
//    }

}
