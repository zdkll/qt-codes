#include "quotetabwidget.h"
#include <QBoxLayout>
#include <QStackedWidget>
#include "subtab.h"
#include "quotepanel.h"
#include "fundhandicap.h"
#include "financemarster.h"
#include <QPushButton>

#define quote_panel_type 0
#define fund_handicap_type 1
#define finance_graph_type 2

QuoteTabWidget::QuoteTabWidget(QWidget *parent, QString instName):
    BaseWidget(parent, instName),
    m_childWndMgr(new HzChildWndMgr<QuoteTabWidget>(this,new QStackedWidget(this)))
{
    createWg();
}

QuoteTabWidget::~QuoteTabWidget()
{
    delete m_childWndMgr;
    m_childWndMgr = nullptr;
}

void QuoteTabWidget::afterActive()
{
    ChildWndItem item;
    m_childWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

void QuoteTabWidget::afterDeactive()
{
    ChildWndItem item;
    m_childWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void QuoteTabWidget::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    QWidget* pWnd = createTabWnd();
    pWnd->setFixedHeight(26);
    pLayout->addWidget(pWnd);

    pWnd = createStackWnd();
    pLayout->addWidget(pWnd);
}

void QuoteTabWidget::updateStock(const QVector<HZData::AllStock> &data)
{
    if(m_quotePanel && !data.isEmpty())
    {
        m_quotePanel->updateStock(data);
    }
}

void QuoteTabWidget::slotsSwitchWnd(int id)
{
    m_childWndMgr->setCurrWndByPageType(id,true);
}

void QuoteTabWidget::OnStockCodeChange()
{
    m_childWndMgr->setCurrStock(m_stockCode,true);
}

QWidget *QuoteTabWidget::createTabWnd()
{
    //tab
    m_table = new CSubTab();
    m_table->setContentsMargins(0,0,0,0);
    m_table->setPacing(1);
    m_table->setObjectName("sletTab");
    m_table->addButton(QStringLiteral("盘口"),quote_panel_type);
    m_table->addButton(QStringLiteral("资金"),fund_handicap_type);
    m_table->addButton(QStringLiteral("财务"),finance_graph_type);
    m_table->setCurrentButton(0);
    connect(m_table,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchWnd(int)));

    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(m_table);
    QWidget* pWnd = new QWidget;
    pWnd->setLayout(pLayout);
    return pWnd;
}

QWidget *QuoteTabWidget::createStackWnd()
{
    m_childWndMgr->registChildCreater(&QuoteTabWidget::createQuotePanel,quote_panel_type,true);
    m_childWndMgr->registChildCreater(&QuoteTabWidget::createFundPanel,fund_handicap_type);
    m_childWndMgr->registChildCreater(&QuoteTabWidget::createFinancePanel,finance_graph_type);
    return m_childWndMgr->getCurrStackWnd();
}

ChildWndItem QuoteTabWidget::createQuotePanel()
{
    ChildWndItem item;
    m_quotePanel = new QuotePanel(this);
    item.pObj = m_quotePanel;
    item.pWnd = m_quotePanel;
    return item;
}

ChildWndItem QuoteTabWidget::createFundPanel()
{
    ChildWndItem item;
    m_fundHandicap = new FundHandicap(this);
    item.pObj = m_fundHandicap;
    item.pWnd = m_fundHandicap;
    return item;
}

ChildWndItem QuoteTabWidget::createFinancePanel()
{
    ChildWndItem item;
    m_financeGraph = new HZFinanceMarster(this);
    item.pObj = m_financeGraph;
    item.pWnd = m_financeGraph;
    return item;
}
