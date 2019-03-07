#ifndef DATAKLINE_H
#define DATAKLINE_H

#include <QVector>

#include "hzdatalayer_global.h"
#include "datamodeldefs/klinedef.h"

//K数据类--------------------------------------------------------
class HZDATALAYERSHARED_EXPORT DataKLine
{
public:
    DataKLine(const QVector<HZData::HZKline>& data,const bool &clearSig = false);
    DataKLine();
    ~DataKLine();

    //从集合竞价数据1分钟拼接-旧的接口obs
    void initFromBidKline(const QVector<HZData::HZKline>& data);

    void mid(int pos,int nLen,DataKLine& out);
    void clear();
    void resize(int size);
    void append(const DataKLine& input);
    void remove(int index,int n);
    void set(int src,int dest,const DataKLine& other);

    QVector<HZData::HZKline>    m_data;  //原始数据
    QVector<std::string>     m_tradingDay;		//交易日
    QVector<double>     m_updateTime;//时间
    QVector<double>     m_upsertTime;
    QVector<double>     m_highPrice; //最高价
    QVector<double>     m_openPrice ; //开盘价
    QVector<double>	    m_lowPrice; //最低价
    QVector<double>     m_closePrice	;//

    QVector<double>      m_volume ;		//成交量
    QVector<double>	   m_turnover ;		//成交额
    QVector<double>      m_preClosePrice ;		// 昨收价
    QVector<double>      m_averPrice ;// 均价 成交额/成交量

    //清盘消息
    bool                            clearSig;
private:
    void resolveData(); //解析数据
};


#endif // DATAKLINE_H
