#include "datakline.h"

DataKLine::DataKLine(const QVector<HZData::HZKline>& data,const bool &clear)
    :m_data(data),clearSig(clear)
{
    resolveData();
}

DataKLine::~DataKLine()
{
}

void DataKLine::initFromBidKline(const QVector<HZData::HZKline>& klines)
{
    if(klines.size()<1)
        return;
    //昨收价
    double pre_close_price = klines[0].pre_close_price;
    QVector<HZData::HZKline> new_klines;
    //分钟
    uint  cur_min =0,min;
    UINT64 volume = 0;
    int i=0;
    for(;i<klines.size();i++){
        min = klines[i].update_time/60;
        //开始新的一分钟
        if(min != cur_min){
            if(cur_min>0){//如果有，则保存上一分钟数据
                HZData::HZKline kline;
                kline.close_price   =  klines[i-1].close_price;
                kline.update_time =  klines[i-1].update_time;
                kline.upsert_time = cur_min*60;
                kline.pre_close_price = pre_close_price;
                kline.volume  = volume;
                new_klines.append(kline);
                volume = 0;
            }
            cur_min = min;
        }
        volume += klines[i].volume;
    }
    //保存最后一分钟Kline
    HZData::HZKline kline;
    kline.close_price   =  klines[i-1].close_price;
    kline.update_time =  klines[i-1].update_time;
    kline.upsert_time = min*60;
    kline.pre_close_price = pre_close_price;
    kline.volume = volume;
    new_klines.append(kline);

    m_data = std::move(new_klines);
    resolveData();
}

void DataKLine::mid(int pos, int nLen, DataKLine &out)
{
    out.m_tradingDay=m_tradingDay.mid(pos,nLen);
    out.m_updateTime=m_updateTime.mid(pos,nLen);
    out.m_upsertTime = m_upsertTime.mid(pos,nLen);
    out.m_highPrice=m_highPrice.mid(pos,nLen);
    out.m_openPrice=m_openPrice.mid(pos,nLen);
    out.m_lowPrice=m_lowPrice.mid(pos,nLen);
    out.m_closePrice=m_closePrice.mid(pos,nLen);

    out.m_volume = m_volume.mid(pos,nLen);
    out.m_turnover = m_turnover.mid(pos,nLen);
    out.m_preClosePrice = m_preClosePrice.mid(pos,nLen);
    out.m_averPrice = m_averPrice.mid(pos,nLen);
    out.m_data = m_data.mid(pos,nLen);
    out.clearSig = clearSig;
}

void DataKLine::clear()
{
    m_tradingDay.clear();
    m_updateTime.clear();
    m_upsertTime.clear();
    m_highPrice.clear();
    m_openPrice .clear();
    m_lowPrice.clear();
    m_closePrice.clear();

    m_volume.clear();
    m_turnover.clear();
    m_preClosePrice.clear();
    m_averPrice.clear();
    m_data.clear();
}

void DataKLine::resize(int size)
{
    m_tradingDay.resize(size);
    m_updateTime.resize(size);
    m_upsertTime.resize(size);
    m_highPrice.resize(size);
    m_openPrice .resize(size);
    m_lowPrice.resize(size);
    m_closePrice.resize(size)	;

    m_volume.resize(size);
    m_turnover.resize(size);
    m_preClosePrice.resize(size ) ;
    m_averPrice.resize(size) ;
    m_data.resize(size);
}

void DataKLine::append(const DataKLine &input)
{
    m_tradingDay.append(input.m_tradingDay);
    m_updateTime.append(input.m_updateTime);
    m_upsertTime.append(input.m_upsertTime);
    m_highPrice.append(input.m_highPrice);
    m_openPrice.append(input.m_openPrice);
    m_lowPrice.append(input.m_lowPrice);
    m_closePrice.append(input.m_closePrice);

    m_volume.append(input.m_volume);
    m_turnover.append(input.m_turnover);
    m_preClosePrice.append(input.m_preClosePrice);
    m_averPrice.append(input.m_averPrice);
    m_data.append(input.m_data);
}

void DataKLine::remove(int index,int n)
{
    m_tradingDay.remove(index,n);
    m_updateTime.remove(index,n);
    m_highPrice.remove(index,n);
    m_openPrice.remove(index,n);
    m_lowPrice.remove(index,n);
    m_closePrice.remove(index,n);

    m_volume.remove(index,n);
    m_turnover.remove(index,n);
    m_preClosePrice.remove(index,n);
    m_averPrice.remove(index,n);
    m_data.remove(index,n);
}

void DataKLine::set(int src, int dest, const DataKLine& other)
{
    if(src < m_data.size() && dest < other.m_data.size())
    {
        m_tradingDay[src] = other.m_tradingDay[dest];
        m_updateTime[src] = other.m_updateTime[dest];
        m_upsertTime[src] = other.m_upsertTime[dest];
        m_highPrice[src] = other.m_highPrice[dest];
        m_openPrice[src] = other.m_openPrice[dest];
        m_lowPrice[src] = other.m_lowPrice[dest];
        m_closePrice[src] = other.m_closePrice[dest];

        m_volume[src] = other.m_volume[dest];
        m_turnover[src]= other.m_turnover[dest];
        m_preClosePrice[src] = other.m_preClosePrice[dest];
        m_averPrice[src] = other.m_averPrice[dest];
        m_data[src]= other.m_data[dest];
    }
}

DataKLine::DataKLine()
{

}

void DataKLine::resolveData()
{
    int size = m_data.size();

    m_tradingDay.resize(size);
    m_updateTime.resize(size);
    m_upsertTime.resize(size);
    m_highPrice.resize(size);
    m_openPrice .resize(size);
    m_lowPrice.resize(size);
    m_closePrice.resize(size);

    m_volume.resize(size);
    m_turnover.resize(size);
    m_preClosePrice.resize(size) ;
    m_averPrice.resize(size);

    for(int i=0;i<size;i++){
        m_tradingDay[i] = m_data[i].trading_day;
        m_updateTime[i] = m_data[i].update_time;
        m_upsertTime[i]  = m_data[i].upsert_time;


        m_highPrice[i] = m_data[i].high_price;
        m_openPrice [i] = m_data[i].open_price;
        m_lowPrice[i] = m_data[i].low_price;
        m_closePrice[i] = m_data[i].close_price;

        m_volume[i] = m_data[i].volume;
        m_turnover [i] = m_data[i].turnover;
        m_preClosePrice[i] = m_data[i].pre_close_price;
        m_averPrice[i] = m_data[i].avg_price;
    }
}
