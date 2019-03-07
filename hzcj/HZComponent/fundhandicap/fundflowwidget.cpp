#include "fundflowwidget.h"

#include <QDebug>
#include <QBoxLayout>
#include "fundflowimage.h"
#include "datacontroller.h"

FundFlowWidget::FundFlowWidget(QWidget *parent,QString instName)
    :BaseFundWidget(parent,instName),m_subReqId(INVALID_REQID)
    ,m_isActived(false)
{
    createWidget();
}

void FundFlowWidget::afterActive()
{
    m_isActived = true;
    if(!m_stockCode.isEmpty())
        subOptionalFund();
}

void FundFlowWidget::afterDeactive()
{
    m_isActived = false;
    cancelSub();
    m_fundFlowImage->clearData();
}

void FundFlowWidget::stockChanged(const QString &stockCode)
{
    if(m_stockCode != stockCode){
        m_stockCode = stockCode;
        if(m_isActived)
            subOptionalFund();
    }
}

void FundFlowWidget::subOptionalFund()
{
    cancelSub();

    HZData::ReqOptionalFundInfo req;
    req.mask  = HZData::MoneyField_Mask;
    req.stock_code.push_back(m_stockCode.toStdString());
    m_subReqId = gHZDataController->subOptionalMoney(req,std::bind(&FundFlowWidget::optionalFundArrived
                                                                   ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));

}

void FundFlowWidget::cancelSub()
{
    if(m_subReqId != INVALID_REQID)
    {
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void FundFlowWidget::optionalFundArrived(ReqId ,const char *data, int num, ErrCode erroCode)
{
    if(erroCode != 0 || num<1)
        return;

    //设置流向图数据
    HZData::StockMoney allStockMoney = ((HZData::StockMoney*)data)[0];
    //停牌的股票
    if(allStockMoney.money_one_day.last_price == 0){
        return;
    }

    m_fundFlowImage->setDayMoneyField(allStockMoney.money_one_day);


    //设置3,5,10,20 日主力净流入------------------------------
    QVector<long long> m_leaderNetputs(4);
    m_leaderNetputs[0] = allStockMoney.one_money_netput;
    m_leaderNetputs[1] = allStockMoney.three_money_netput;
    m_leaderNetputs[2] = allStockMoney.five_money_netput;
    m_leaderNetputs[3] = allStockMoney.ten_money_netput;
    emit recvLeaderNetput(m_leaderNetputs);
}

void FundFlowWidget::createWidget()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    m_fundFlowImage = new FundFlowImage;
    mainLayout->addWidget(m_fundFlowImage);

    this->setLayout(mainLayout);
}
