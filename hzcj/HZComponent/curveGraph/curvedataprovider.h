#ifndef CURVEDATAPROVIDER_H
#define CURVEDATAPROVIDER_H
#include "curvedef.h"

class CurveDataProvider
{
public:
    CurveDataProvider();

    void setObserver(CurveUpdateCallBack fun);

    virtual int req(const CurveReqParam& param);

    virtual void cancle(int id);
protected:
    CurveUpdateCallBack m_fun;
};

#endif // CURVEDATAPROVIDER_H
