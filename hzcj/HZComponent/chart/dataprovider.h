#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H
/********************************************************************
created: 2018/4/9
author: min.cheng
purpose: 数据请求处理类
*********************************************************************/
#include <QObject>
#include "chartdef.h"
#include "chartmodle.h"
class BaseChart;
class DataProvider:public QObject
{
        Q_OBJECT
public:
    DataProvider(BaseChart* pchart);
public:
    //需求请求
    virtual void cancle(int id);

    //查询数据 内容通过命令格式拼接
    virtual int query(const std::string& req);

    //获取缓存数据
    KlineData*  getDataCache(RestorationType type= RestorationNone);
signals:

public slots:

protected:
    //原来的信号改成直接回调是为了效率
    void sendDataResponse(int id,int type);
    void sendErrorResponse(int id,int ncode,const QString& errReuslt);
    //用于计算的指标数据
    IndiCalcData* toIndiChache(DataKLine* pData);
    //合并缓存数据
    void mergeKline(DataKLine *src,const QString& obj,int period,bool bPush);
    //需要复权数据的存储
    void mergeKlineWithDr(DataKLine *src,const QString& obj,int period,bool bPush);
    //简单追加
    void appendKline(DataKLine *src);
    //追加前复权数据
    void appendDrforwardKline(DataKLine *src);
    //追加后复权数据
    void appendDrBackKline(DataKLine *src);
    //清理数据
    void clearCache();
    //前后复权优化计算
    void drCalc(DataKLine& fdata,DataKLine& bdata,const QString& obj);
private:
    void clear(KlineData& data);
    void append(DataKLine *src,KlineData& data);
    void mergeKline(DataKLine *src,KlineData& klineData,const QString& obj,int period,bool bPush);
protected:
    BaseChart* m_chart;
    //缓存数据
    KlineData m_klineData;
    //前复权数据
    KlineData m_drForwardKlineData;
    //后复权数据
    KlineData m_drBackwardklineData;
};






#endif // DATAPROVIDER_H
