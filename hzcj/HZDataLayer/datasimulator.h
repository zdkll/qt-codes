#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include "datamodeldef.h"
#include "datactrldef.h"


//本地股票短线异动数据
struct StockAbnormalData
{
    int time_t;
    int totalCount = 0;
    QVector<HZData::StockAbnormal>   data;
};

//数据模拟专用类
class DataSimulator
{
public:
    static DataSimulator *instance();
    void setRecvCbFunction(OnRecvFunction recvFunc);

    void requestIncDecPosInfo(ReqId reqId,const  HZData::ReqLeaderIncDecPostionInfo &req);


private:
    void createStockAbnormalData(int time_t);
    void getStockAbnormalValue(HZData::AbnormalType type,double& value);

    DataSimulator();

    struct PrivateData
    {
        StockAbnormalData stockAbnormalData;
    };

    PrivateData m_d;

    OnRecvFunction  m_recvFunc;
};

#endif // DATASIMULATOR_H
