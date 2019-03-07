#include "datasimulator.h"

#define STOCK_ABNORMAL_TYPE_NUM   6

#include <string>
#include <unordered_map>
#include <map>
#include <QDateTime>
#include <QTime>
#include <QDebug>

using namespace HZData;

const QTime gtime0(0,0,0);
const QTime gclear_time(8,45,0);
const QTime gbegin_time(9,24,0);
const QTime gend_time(14,56,0);

const int gclear_time_t   = gtime0.secsTo(gclear_time);
const int gbegin_time_t  = gtime0.secsTo(gbegin_time);
const int gend_time_t    = gtime0.secsTo(gend_time);

const QDate gdate0 = QDate(2000,1,1);

struct TestStock
{
    std::string  stock_code;
    std::string  stock_name;
};
//测试股票
const TestStock gstocks_arr[]
={{"sz300216","千山药机"},{"sh603557","起步股份"},
  {"sh603297","永新光学"},{"sz300603","立昂技术"},
  {"sh601068","中铝国际"},{"sz300489","中飞股份"}};
const int gstocks_size = 6;

DataSimulator::DataSimulator()
{

}


void DataSimulator::setRecvCbFunction(OnRecvFunction recvFunc)
{
    m_recvFunc = recvFunc;
}

DataSimulator *DataSimulator::instance()
{
    static DataSimulator dataSimulator;
    return  &dataSimulator;
}


void DataSimulator::createStockAbnormalData(int time_t)
{
    if(m_d.stockAbnormalData.time_t<gbegin_time_t)
        m_d.stockAbnormalData.time_t=gbegin_time_t;
    //十秒生成一次
    if(time_t-m_d.stockAbnormalData.time_t<10)
        return;
//    qDebug()<<"time _t:"<<time_t<<"data time_t:"<<m_d.stockAbnormalData.time_t;
    int time = m_d.stockAbnormalData.time_t+10;
    int rand_val,stock_idx;
    for(;time<time_t;time+=10){
        StockAbnormal stockAbnormal;
        stockAbnormal.update_time = time;
        qsrand(time);
        rand_val = qrand();
        stock_idx = rand_val%gstocks_size;
        stockAbnormal.stock_code= gstocks_arr[stock_idx].stock_code;
        stockAbnormal.stock_name=gstocks_arr[stock_idx].stock_name;
        stockAbnormal.abnormal_type = AbnormalType(rand_val%STOCK_ABNORMAL_TYPE_NUM);
        getStockAbnormalValue(stockAbnormal.abnormal_type,stockAbnormal.value);
        m_d.stockAbnormalData.data.append(stockAbnormal);
        m_d.stockAbnormalData.time_t = time;
    }
    m_d.stockAbnormalData.totalCount = m_d.stockAbnormalData.data.size();
}

void DataSimulator::getStockAbnormalValue(AbnormalType type,double& value)
{
    switch (type) {
    case speed_up:
        value = 0.032456;
        break;
    case speed_down:
        value = -0.041326;
        break;
    case big_sell:
        value = 526400;
        break;
    case big_buy:
        value = 160000;
        break;
    case rise_stop:
        value = 57.50;
        break;
    case fall_stop:
        value = 4.97;
        break;
    default:
        break;
    }
}

void DataSimulator::requestIncDecPosInfo(ReqId reqId,const  HZData::ReqLeaderIncDecPostionInfo &req)
{
    //模拟
    QDate date = QDate::currentDate();

    int end_days = gdate0.daysTo(date);

    int start_days = end_days - 30 + 1;

    QVector<HZData::IncrDecPostion> incrDecPostionInfo(30);
    std::srand(end_days);
    for(int i=0;i<30;i++){
        incrDecPostionInfo[i].date     = start_days + i;
        incrDecPostionInfo[i].money = std::rand()-10000;
    }
    m_recvFunc.recvIncDecPosInfo(reqId,incrDecPostionInfo);
}

