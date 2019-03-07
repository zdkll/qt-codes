#ifndef INDICATORPROVIDER_H
#define INDICATORPROVIDER_H
/********************************************************************
created:
author: min.cheng
purpose: 指标计算类，用于指标数据提供
*********************************************************************/
#include <memory>
#include <mutex>

#include "dataprovider.h"
#include "IndiInterface.h"
#include "datamodeldef.h"

class QTimer;
class IndicatorProvider : public DataProvider
{
     Q_OBJECT
public:
    IndicatorProvider(BaseChart* pchart):DataProvider(pchart){}

    virtual ~IndicatorProvider(){}

    //指标请求查询接口
    virtual int query(const std::string& req){return -1;}

    //取消计算
    virtual void cancle(int id){}

    //对外提供数据拷贝
    virtual void copyData(IndiRspResult& data,int offset,int count){}

    //获取指标基本信息
    virtual void getIndiBaseInfo(IndiItemInfo& base_info,int indiId){}
};


class IndiLocalProvider: public IndicatorProvider
{
     Q_OBJECT
public:
    IndiLocalProvider(BaseChart* pchart);

    ~IndiLocalProvider();

    //指标请求查询接口
    int query(const std::string& req);

    //取消计算
    void cancle(int id);

    //对外提供数据拷贝
    void copyData(IndiRspResult& data,int offset,int count);

    //获取指标基本信息
    virtual void getIndiBaseInfo(IndiItemInfo& base_info,int indiId);
signals:

protected slots:

private:
    //指标计算完成回调函数
    void OnIndiCalc(IndiRspResult& result);
    //执行指标计算
    int calc(int indiId,int offset,int count,const QString& obj,int period);
    std::shared_ptr<IndiCalcData> getData(IndiCalcData* pKlineData,int count,int offset);
private:
    //指标类型ID
    int m_indiId;
    //指标计算ID
    int64_t m_id;
    QString m_market;
    QString m_obj;
    int m_period;
    IndiRspResult m_result;
    std::mutex m_mutex;

};


class IndicatorSrvProvider : public IndicatorProvider
{
    Q_OBJECT
public:
    IndicatorSrvProvider(BaseChart* pchart);

    ~IndicatorSrvProvider();

    //指标请求查询接口
    int query(const std::string& req);

    //取消计算
    void cancle(int id=-1);

    //对外提供数据拷贝
    void copyData(IndiRspResult& data,int offset,int count);

    //获取指标基本信息
    virtual void getIndiBaseInfo(IndiItemInfo& base_info,int indiId);
private:
    void onIndiCallBack(ReqId id, const HZData::ZhibiaoInfo& info, ErrCode error);
    int indexOfZbDotVec(double time,int type=-1,int from=0);
    int indexOfZbColumnVec(double time,int from=0);
    void reqData(int beginTime,int endTime,const QVector<double>& timeList,HZData::ReqZhibiaoInfo req);
private:
    //数据处理
    void convertIndiData(bool bSub);
    void proceJdcj(bool bSub);
    void proceCmfb(bool bSub);
    void proceCptx(bool bSub);
    void proceHlqj(bool bSub);
    void proceHpm(bool bSub);
    void proceXqqx(bool bSub);

    //绘图样式处理
    void getJdcjBaseInfo(IndiItemInfo& base_info);
    void getCmfbBaseInfo(IndiItemInfo& base_info);
    void getCptxBaseInfo(IndiItemInfo& base_info);
    void getHlqjBaseInfo(IndiItemInfo& base_info);
    void getHpmBaseInfo(IndiItemInfo& base_info);
    void getXqqxBaseInfo(IndiItemInfo& base_info);
private:
    HZData::ZhibiaoInfo m_data;
    HZData::ReqZhibiaoInfo m_reqInfo;
    ReqId m_subId = -1;
    QMap<ReqId,ReqId> m_reqIdMap;
    QMap<ReqId,HZData::ZhibiaoInfo> m_tempData;
    IndiRspResult m_result;
    RestorationType m_drType=RestorationNone;
};
#endif // INDICATORPROVIDER_H
