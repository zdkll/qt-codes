#include "optionalstockquotewidget.h"
#include <QVBoxLayout>
#include "stockinfowidget.h"
#include "quotetabwidget.h"
#include "minchartwidget.h"
#include "httpdata.h"

OptionalStockQuoteWidget::OptionalStockQuoteWidget(QWidget *parent, QString instName)
        :BaseWidget(parent,instName),
          m_quoteTabWidget(nullptr),
          m_nReq(INVALID_REQID)
{
}

void OptionalStockQuoteWidget::createWg()
{
    this->setFixedWidth(350);
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    QWidget* pWnd = createQuoteTitleWnd();
    pWnd->setFixedHeight(76);
    pLayout->addWidget(pWnd);

    pWnd = createMinLineWidget();
    pWnd->setFixedHeight(213);

    pLayout->addWidget(pWnd);

    pWnd = createQuoteBaseDataWnd();
    pWnd->setFixedHeight(102);
    pLayout->addWidget(pWnd);

    pWnd = createQuoteExtraDataWnd();
    pWnd->setFixedHeight(66);
    pLayout->addWidget(pWnd);

    pWnd = createQuoteTabWidget();

    pLayout->addWidget(pWnd);
    this->setLayout(pLayout);

}

void OptionalStockQuoteWidget::OnStockCodeChange()
{
    m_handicapTitleWidget->setStockCode(m_stockCode);
    m_quoteTabWidget->setStockCode(m_stockCode);
    m_minChartWidget->setStockCode(m_stockCode);
}

void OptionalStockQuoteWidget::afterActive()
{

    QVector<HZData::AllStock> vec;
    HZData::AllStock allStock;
    allStock.stock_code = m_stockCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();
    vec.push_back(allStock);
    m_quoteTabWidget->updateStock(vec);
    m_handicapTitleWidget->updateData(vec);
    m_handicapCentralWidget->updateData(vec);
    m_handicapBottomWidget->updateData(vec);

    subDyna();
    if(m_quoteTabWidget)
    {
        m_quoteTabWidget->afterActive();
    }
    if(m_minChartWidget)
    {
        m_minChartWidget->afterActive();
    }
}

void OptionalStockQuoteWidget::afterDeactive()
{
    cancelSub();
    DataController::instance()->cancelSub(m_nReq);
    if(m_quoteTabWidget)
    {
        m_quoteTabWidget->afterDeactive();
    }
    if(m_minChartWidget)
    {
        m_minChartWidget->afterDeactive();
    }
}

void OptionalStockQuoteWidget::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
    if(this->isVisible())
    {
        afterDeactive();
        afterActive();
    }
}


void OptionalStockQuoteWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);
        m_quoteTabWidget->updateStock(vec);
        m_handicapTitleWidget->updateData(vec);
        m_handicapCentralWidget->updateData(vec);
        m_handicapBottomWidget->updateData(vec);
    }
}

void OptionalStockQuoteWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&OptionalStockQuoteWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
}

void OptionalStockQuoteWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

QWidget *OptionalStockQuoteWidget::createQuoteTitleWnd()
{
    m_handicapTitleWidget = new HandicapTitleWidget;
    return m_handicapTitleWidget;
}

QWidget *OptionalStockQuoteWidget::createMinLineWidget()
{
    m_minChartWidget = new MinChartWidget;
    return m_minChartWidget;
}

QWidget *OptionalStockQuoteWidget::createQuoteBaseDataWnd()
{
    m_handicapCentralWidget = new HandicapCentralWidget;
    return m_handicapCentralWidget;
}

QWidget *OptionalStockQuoteWidget::createQuoteExtraDataWnd()
{
    m_handicapBottomWidget = new HandicapBottomWidget;
    return m_handicapBottomWidget;
}

QWidget *OptionalStockQuoteWidget::createQuoteTabWidget()
{
    m_quoteTabWidget = new QuoteTabWidget;
    return m_quoteTabWidget;
}



