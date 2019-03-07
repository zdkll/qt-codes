#ifndef MINBLOCKABNORMAL_H
#define MINBLOCKABNORMAL_H
/********************************************************************
created:
author: min.cheng
purpose: 分时 板块异动分时页面
*********************************************************************/
#include "minchart.h"
#include "datacontroller.h"

class MinBlockAbnormal: public Minchart
{
    Q_OBJECT
public:
    MinBlockAbnormal(ImageLayer* layer);

    ~MinBlockAbnormal();

    void afterActive();

    //不显示时，用于取消订阅
    void afterDeactive();

    //重写最大值和最小值
    virtual void getMaxMinRange(double& max,double& min);

    virtual void errorResponse(int id,int ncode,const QString& errReuslt);
protected:
    void onResize();
    void onCreate();
    void OnBlockAbnormal(ReqId id, const HZData::BlockAbnormalInfo& data, ErrCode err);
    void drawBlock();
private:
    HZData::BlockAbnormalInfo m_data;
    int m_bkAbnormalLayer = -1;
    ReqId m_subId;
};

#endif // MINBLOCKABNORMAL_H
