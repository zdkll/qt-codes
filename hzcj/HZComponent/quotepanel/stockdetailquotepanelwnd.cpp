#include "stockdetailquotepanelwnd.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include "subtab.h"
#include "stockinfowidget.h"
#include "fundhandicap.h"
#include "financemarster.h"
#include "quotetabwidget.h"
#include "httpdata.h"

StockDetailQuotePanelWnd::StockDetailQuotePanelWnd(QWidget *parent, QString instName)
        :StockDetailBaseWnd(parent,instName),
          m_nReq(INVALID_REQID)
{

}

void StockDetailQuotePanelWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    QWidget* pWnd = createQuoteTitleWnd();
    pWnd->setFixedHeight(76);
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

void StockDetailQuotePanelWnd::afterActive()
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
}

void StockDetailQuotePanelWnd::afterDeactive()
{
    DataController::instance()->cancelSub(m_nReq);
    if(m_quoteTabWidget)
    {
        m_quoteTabWidget->afterDeactive();
    }
}

void StockDetailQuotePanelWnd::slotsObjChange(const QString &obj)
{
    m_stockCode = obj;
    m_quoteTabWidget->setStockCode(m_stockCode);
    m_handicapTitleWidget->setStockCode(m_stockCode);
    if(this->isVisible())
    {
        afterDeactive();
        afterActive();
    }
}


void StockDetailQuotePanelWnd::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
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

void StockDetailQuotePanelWnd::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&StockDetailQuotePanelWnd::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
}

void StockDetailQuotePanelWnd::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

void StockDetailQuotePanelWnd::OnStockCodeChange()
{
    m_quoteTabWidget->setStockCode(m_stockCode);
    m_handicapTitleWidget->setStockCode(m_stockCode);
}

QWidget *StockDetailQuotePanelWnd::createQuoteTitleWnd()
{
    m_handicapTitleWidget = new HandicapTitleWidget;
    return m_handicapTitleWidget;
}

QWidget *StockDetailQuotePanelWnd::createQuoteBaseDataWnd()
{
    m_handicapCentralWidget = new HandicapCentralWidget;
    return m_handicapCentralWidget;
}

QWidget *StockDetailQuotePanelWnd::createQuoteExtraDataWnd()
{
    m_handicapBottomWidget = new HandicapBottomWidget;
    return m_handicapBottomWidget;
}

QWidget *StockDetailQuotePanelWnd::createQuoteTabWidget()
{
    m_quoteTabWidget = new QuoteTabWidget;
    return m_quoteTabWidget;
}
