#include "callmarkethandicapwidget.h"
#include <QBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include "stockinfowidget.h"
#include "quotetabwidget.h"
#include "minchartwidget.h"
#include "biddingcanvas.h"
#include "httpdata.h"

CallMarketHandicapWidget::CallMarketHandicapWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_handicapTitleWidget(new HandicapTitleWidget),
    m_handicapCentralWidget(new HandicapCentralWidget),
    m_quoteTabWidget(new QuoteTabWidget),
    m_minChartWidget(new MinChartWidget),
    m_biddingWidget(new BiddingCanvas)
{
    createWg();
}

void CallMarketHandicapWidget::afterActive()
{
    QVector<HZData::AllStock> vec;
    HZData::AllStock allStock;
    allStock.stock_code = m_stockCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();
    vec.push_back(allStock);
    m_quoteTabWidget->updateStock(vec);
    m_handicapTitleWidget->updateData(vec);
    m_handicapCentralWidget->updateData(vec);

    subDyna();
    if(m_quoteTabWidget)
    {
        m_quoteTabWidget->afterActive();
    }
    if(m_minChartWidget)
    {
        m_minChartWidget->afterActive();
    }
    m_biddingWidget->afterActive();
}

void CallMarketHandicapWidget::afterDeactive()
{
    cancelSub();
    if(m_quoteTabWidget)
    {
        m_quoteTabWidget->afterDeactive();
    }
    if(m_minChartWidget)
    {
        m_minChartWidget->afterDeactive();
    }
    m_biddingWidget->afterDeactive();
}

void CallMarketHandicapWidget::createWg()
{
    ScrollWidget *srcollwidget = new ScrollWidget(this);

    QWidget *maintwidget = new QWidget(this);
    maintwidget->setObjectName("mainwidget");
    maintwidget->setFixedWidth(350);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_handicapTitleWidget);
    mainLayout->addWidget(m_handicapCentralWidget);
    mainLayout->addWidget(srcollwidget);


    QVBoxLayout *subLayout = new QVBoxLayout;
    subLayout->setSpacing(0);
    subLayout->setMargin(0);

    subLayout->addWidget(m_minChartWidget);
    QWidget *widget = new QWidget;
    widget->setObjectName("bidding");
    widget->setFixedHeight(22);
    QLabel *label = new QLabel(QStringLiteral("集合竞价"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(16, 0, 0, 0);
    layout->addWidget(label);
    widget->setLayout(layout);
    subLayout->addWidget(widget);
//    m_biddingWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    subLayout->addWidget(m_biddingWidget);
    subLayout->addWidget(m_quoteTabWidget);
    m_quoteTabWidget->setFixedHeight(600);
    m_minChartWidget->setFixedHeight(240);
    m_biddingWidget->setFixedHeight(240);

    maintwidget->setLayout(subLayout);

    srcollwidget->scrollArea()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srcollwidget->scrollArea()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srcollwidget->setWidget(maintwidget);
}

void CallMarketHandicapWidget::OnStockCodeChange()
{
    m_handicapTitleWidget->setStockCode(m_stockCode);
    m_quoteTabWidget->setStockCode(m_stockCode);
    m_minChartWidget->setStockCode(m_stockCode);
    m_biddingWidget->setStockCode(m_stockCode);
}

void CallMarketHandicapWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
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
    }
}

void CallMarketHandicapWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&CallMarketHandicapWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);

}

void CallMarketHandicapWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}


