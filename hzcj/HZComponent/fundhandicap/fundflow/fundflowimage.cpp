#include "fundflowimage.h"

#include <QBoxLayout>
#include <QPalette>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include "fundflowpiechart.h"
#include "fundflowbarchart.h"

FundFlowImage::FundFlowImage(QWidget *parent)
    :QWidget(parent),m_fundInOutData(new FundInOutData)
{
    createWg();

    m_fundFlowPieChart->setData(m_fundInOutData);
    m_fundFlowBarChart->setData(m_fundInOutData);
}

FundFlowImage::~FundFlowImage()
{
    if(m_fundInOutData){
        delete m_fundInOutData;m_fundInOutData=NULL;
    }
}

void FundFlowImage::setDayMoneyField(const HZData::MoneyField &moneyField)
{
    if(!m_fundInOutData) m_fundInOutData = new FundInOutData;
    //抽取原始数据计算图像绘制数据结构
    long long totalInput(0),totalOutput(0),totalNetput(0),maxnetput(0);
    //主力净流入
    m_fundInOutData->leaderNetput = moneyField.leader_money.money_netput;
    //超大单
    totalInput += moneyField.maxord_money.money_input;
    m_fundInOutData->fundOrders[0].input = moneyField.maxord_money.money_input;
    totalOutput += moneyField.maxord_money.money_output;
    m_fundInOutData->fundOrders[0].output = moneyField.maxord_money.money_output;
    totalNetput += moneyField.maxord_money.money_netput;
    m_fundInOutData->fundOrders[0].netput = moneyField.maxord_money.money_netput;

    //最大净流入
    maxnetput = maxnetput<abs(moneyField.maxord_money.money_netput)?
                abs(moneyField.maxord_money.money_netput):maxnetput;

    //大单
    totalInput += moneyField.bigord_money.money_input;
    m_fundInOutData->fundOrders[1].input = moneyField.bigord_money.money_input;
    totalOutput += moneyField.bigord_money.money_output;
    m_fundInOutData->fundOrders[1].output = moneyField.bigord_money.money_output;
    totalNetput += moneyField.bigord_money.money_netput;
    m_fundInOutData->fundOrders[1].netput = moneyField.bigord_money.money_netput;

    //最大净流入
    maxnetput = maxnetput<abs(moneyField.bigord_money.money_netput)?
                abs(moneyField.bigord_money.money_netput):maxnetput;

    //中单
    totalInput += moneyField.midord_money.money_input;
    m_fundInOutData->fundOrders[2].input = moneyField.midord_money.money_input;
    totalOutput += moneyField.midord_money.money_output;
    m_fundInOutData->fundOrders[2].output = moneyField.midord_money.money_output;
    totalNetput += moneyField.midord_money.money_netput;
    m_fundInOutData->fundOrders[2].netput = moneyField.midord_money.money_netput;
    //最大净流入
    maxnetput = maxnetput<abs(moneyField.midord_money.money_netput)?
                abs(moneyField.midord_money.money_netput):maxnetput;
    //小单
    totalInput += moneyField.minord_money.money_input;
    m_fundInOutData->fundOrders[3].input = moneyField.minord_money.money_input;
    totalOutput += moneyField.minord_money.money_output;
    m_fundInOutData->fundOrders[3].output = moneyField.minord_money.money_output;
    totalNetput += moneyField.minord_money.money_netput;
    m_fundInOutData->fundOrders[3].netput = moneyField.minord_money.money_netput;
    //最大净流入
    maxnetput = maxnetput<abs(moneyField.minord_money.money_netput)?
                abs(moneyField.minord_money.money_netput):maxnetput;

    m_fundInOutData->totalInput        = totalInput;
    m_fundInOutData->maxTotalFund = totalInput;
    m_fundInOutData->totalOutput      = totalOutput;
    m_fundInOutData->maxTotalFund = m_fundInOutData->maxTotalFund>totalOutput?
                m_fundInOutData->maxTotalFund:totalOutput;
    m_fundInOutData->totalNetput     = totalNetput;
    m_fundInOutData->maxTotalFund = m_fundInOutData->maxTotalFund>abs(totalNetput)?
                m_fundInOutData->maxTotalFund:abs(totalNetput);
    m_fundInOutData->totalInOut   = totalInput + totalOutput;

    m_fundInOutData->maxNetput = maxnetput;

    m_fundFlowPieChart->setData(m_fundInOutData);
    m_fundFlowBarChart->setData(m_fundInOutData);
}

void FundFlowImage::clearData()
{
    if(m_fundInOutData){
        delete m_fundInOutData;
    }
    m_fundInOutData = new FundInOutData;
    m_fundFlowPieChart->setData(m_fundInOutData);
    m_fundFlowBarChart->setData(m_fundInOutData);
}

void FundFlowImage::createWg()
{
    QHBoxLayout *mainLayout = new  QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    //左侧
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(0);

    //饼状图
    m_fundFlowPieChart =  new FundFlowPieChart(this);
    m_fundFlowPieChart->setFixedHeight(160);
    m_fundFlowPieChart->setFixedWidth(110);
    leftLayout->addWidget(m_fundFlowPieChart);
    leftLayout->addStretch(1);

    mainLayout->addLayout(leftLayout);

    //右侧
    //    QVBoxLayout *rightLayout = new QVBoxLayout;
    m_fundFlowBarChart =  new FundFlowBarChart(this);
    //    rightLayout->addWidget(m_fundFlowBarChart);

    //    m_fundFlowTableWg =  new FundFlowTableWg(this);
    //    rightLayout->addWidget(m_fundFlowTableWg);

    mainLayout->addWidget(m_fundFlowBarChart);

    this->setLayout(mainLayout);
}
