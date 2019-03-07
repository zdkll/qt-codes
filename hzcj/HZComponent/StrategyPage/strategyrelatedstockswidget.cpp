#include "strategyrelatedstockswidget.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include "relatedstockpanel.h"
StrategyRelatedStocksWidget::StrategyRelatedStocksWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_nRedFund(INVALID_REQID)
{
    createWg();
}

void StrategyRelatedStocksWidget::updatedata(const TopicInvestDetail &data)
{
    m_deTailData = data;
    subDyna();
    subOptionalFund();
}

void StrategyRelatedStocksWidget::afterDeactive()
{
    cancelSub();
    cancelSubFund();
}

void StrategyRelatedStocksWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    QWidget *titleWidget = new QWidget;
    titleWidget->setObjectName("title");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(22,0,0,0);
    layout->addWidget(new QLabel(QStringLiteral("相关个股")));
    layout->addStretch();
    QPushButton *btnMore = new QPushButton(QStringLiteral("查看更多>"));
    layout->addWidget(btnMore);
    layout->addSpacing(15);

    titleWidget->setLayout(layout);

    mainLayout->addWidget(titleWidget);

    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setSpacing(0);
    gridlayout->setMargin(0);

    RelatedStockPanel *panel = new RelatedStockPanel;
    panel->setObjectName("first");
    m_panelVec.append(panel);
    gridlayout->addWidget(panel, 0, 0);
    panel = new RelatedStockPanel;
    panel->setObjectName("second");
    m_panelVec.append(panel);
    gridlayout->addWidget(panel, 0, 1);
    panel = new RelatedStockPanel;
    panel->setObjectName("third");
    m_panelVec.append(panel);
    gridlayout->addWidget(panel, 0, 2);
    panel = new RelatedStockPanel;
    panel->setObjectName("forth");
    m_panelVec.append(panel);
    gridlayout->addWidget(panel, 1, 0);
    panel = new RelatedStockPanel;
    panel->setObjectName("fifth");
    m_panelVec.append(panel);
    gridlayout->addWidget(panel, 1, 1);
    panel = new RelatedStockPanel;
    panel->setObjectName("sixth");
    m_panelVec.append(panel);
    gridlayout->addWidget(panel, 1, 2);

    mainLayout->addLayout(gridlayout);
}

void StrategyRelatedStocksWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;

        for(int i = 0; i < num; i++)
        {
            HZData::AllStock allStock = stocks[i];

            m_panelMap[allStock.stock_code]->updateData(allStock);
        }

    }
}

void StrategyRelatedStocksWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;

    int nSize = 6;
    if(m_deTailData.stock.size() < nSize)
        nSize = m_deTailData.stock.size();

    m_panelMap.clear();
    for(int i = 0; i < nSize; i++)
    {
        reqDynaInfo.stock_code.push_back(m_deTailData.stock[i].stockcode.toStdString());
        m_panelMap[m_deTailData.stock[i].stockcode.toStdString()] = m_panelVec[i];
    }
    OnStockDynaArrived stockDynaArrived = std::bind(&StrategyRelatedStocksWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);

}

void StrategyRelatedStocksWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

void StrategyRelatedStocksWidget::subOptionalFund()
{
    cancelSubFund();
    HZData::ReqOptionalFundInfo req;
    req.mask  = HZData::MoneyField_Mask;
    int nSize = 6;
    if(m_deTailData.stock.size() < nSize)
        nSize = m_deTailData.stock.size();
    for(int i = 0; i < nSize; i++)
    {
        req.stock_code.push_back(m_deTailData.stock[i].stockcode.toStdString());
    }

    OnMoneyArrived fun = std::bind(&StrategyRelatedStocksWidget::optionalFundArrived
                                   ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);

    m_nRedFund = gHZDataController->subOptionalMoney(req, fun);
}

void StrategyRelatedStocksWidget::optionalFundArrived(ReqId id, const char *data, int num, ErrCode errCode)
{
    if(errCode != 0 || num<1)
        return;

    //设置流向图数据
    HZData::StockMoney *allStockMoney = (HZData::StockMoney*)data;

    for(int i = 0; i < num; i++)
    {
        HZData::StockMoney stockMoney= allStockMoney[i];

        m_panelMap[stockMoney.stock_code]->updateData(stockMoney);
    }

}

void StrategyRelatedStocksWidget::cancelSubFund()
{
    if(INVALID_REQID != m_nRedFund)
    {
        gHZDataController->cancelSub(m_nRedFund);
        m_nRedFund = INVALID_REQID;
    }
}
