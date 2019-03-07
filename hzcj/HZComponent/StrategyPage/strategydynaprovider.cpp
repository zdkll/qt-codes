#include "strategydynaprovider.h"
#include <QTime>
DynaDataProvider::DynaDataProvider(QObject *parent):
    QObject(parent)
{
    m_pTimer = new QTimer(this);
    QObject::connect(m_pTimer, SIGNAL(timeout()),this, SLOT(slotsTimer()));
    m_pTimer->setInterval(300);
}

void DynaDataProvider::addsubstock(const QString &stock)
{
    auto it =  m_dataMap.find(stock);
    if(it == m_dataMap.end())//未订阅过
    {
        subItem item;
        HZData::ReqOptionalDynaInfo reqDynaInfo;
        reqDynaInfo.stock_code.push_back(stock.toStdString());
        OnStockDynaArrived stockDynaArrived = std::bind(&DynaDataProvider::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
        item.reqId =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
        item.data = HZData::AllStock();
        m_dataMap[stock] =item;
    }else
    {
        it.value().reqTimes++;
    }
}

void DynaDataProvider::addsubstock(const QVector<QString> &stocks)
{
    for(int i=0;i < stocks.size();i++)
    {
        addsubstock(stocks[i]);
    }
}

void DynaDataProvider::removestock(const QString &stock)
{
    auto it =  m_dataMap.find(stock);
    if(it != m_dataMap.end())//订阅过
    {
        it.value().reqTimes--;
        if(it.value().reqTimes <= 0)
        {
            gHZDataController->cancelSub(it.value().reqId);
            m_dataMap.erase(it);
        }
    }
}

void DynaDataProvider::removestock(const QVector<QString> &stocks)
{
    for(int i=0;i < stocks.size();i++)
    {
        removestock(stocks[i]);
    }
}

void DynaDataProvider::removestockAll()
{
    for(auto it = m_dataMap.begin();it != m_dataMap.end();it++)
    {
        gHZDataController->cancelSub(it.value().reqId);
    }
    m_dataMap.clear();
}

HZData::AllStock DynaDataProvider::getStockData(const QString& stock)
{
    HZData::AllStock ret;
    auto it = m_dataMap.find(stock);
    if(it != m_dataMap.end())
        ret = it.value().data;
    else
        ret.stock_code = stock.toStdString();
    return ret;
}

void DynaDataProvider::startTimer()
{
    if(!m_pTimer->isActive())
        m_pTimer->start();
    m_bDataChanged =true;
}

void DynaDataProvider::stopTimer()
{
    if(m_pTimer->isActive())
        m_pTimer->stop();
    m_bDataChanged = false;
}

void DynaDataProvider::slotsTimer()
{
    if(m_dataMap.isEmpty() == false && m_bDataChanged)
    {
        emit signalUpdateData();
        m_bDataChanged = false;
    }
}

void DynaDataProvider::onStockOptionalDataArrived(ReqId reqId, char *data, int num, ErrCode errCode)
{
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        for(int i = 0;i < num;i++)
        {
            auto it = m_dataMap.find(stocks[i].stock_code.c_str());
            if(it != m_dataMap.end())
                m_dataMap[stocks[i].stock_code.c_str()].data = stocks[i];
        }
        m_bDataChanged = true;
    }
}
