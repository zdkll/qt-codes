#include "curvedataprovider.h"

CurveDataProvider::CurveDataProvider()
{

}

void CurveDataProvider::setObserver(CurveUpdateCallBack fun)
{
    m_fun = fun;
}

int CurveDataProvider::req(const CurveReqParam &param)
{
    return -1;
}

void CurveDataProvider::cancle(int id)
{

}
