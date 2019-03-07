#include <QDebug>
#include "datasubcenter.h"

DataSubCenter::DataSubCenter(QObject *parent) : QObject(parent),
    m_nReq(INVALID_REQID)
{

}

void DataSubCenter::startSub(std::list<string> lists)
{
    qDebug() << "lists.size() = " << lists.size();
    cancelSub();

    if(lists.empty())
        return;

    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code = lists;
    reqDynaInfo.mask = HZData::SLStock_Mask;

    OnStockDynaArrived stockDynaArrived = std::bind(&DataSubCenter::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3, std::placeholders::_4);

    m_nReq = gHZDataController->subOptionalDyna(reqDynaInfo, stockDynaArrived);

}

void DataSubCenter::cancelSub()
{
    if(m_nReq != INVALID_REQID){
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}


void DataSubCenter::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    qDebug()<<"ReqId:"<<id;
    Q_UNUSED(id)
    if(0 == errCode)
    {
        emit subStockArrived(data, num);
    }
}
