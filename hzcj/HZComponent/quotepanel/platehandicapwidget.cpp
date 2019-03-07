#include "platehandicapwidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include "stockinfowidget.h"
#include "minchartwidget.h"
#include "platepartswidget.h"
#include "indexpartswidget.h"
#include "quotetabwidget.h"
#include "httpdata.h"


PlateHandicapWidget::PlateHandicapWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_nStockReq(INVALID_REQID),
    m_plateTitleWidget(new HandicapTitleWidget),
    m_plateChartWidget(new MinChartWidget),
    m_plateQuoteWidget(new PlateDetailQuoteWidget),
    m_plateRiseFallWidget(new IndexRiseFallWidget),
    m_stockTitleWidget(new HandicapTitleWidget),
    m_stockChartWidget(new MinChartWidget),
    m_stockTabWidget(new QuoteTabWidget)
{
//    m_plateChartWidget->setFixedHeight(213);
//    m_stockChartWidget->setFixedHeight(213);
    m_stockTabWidget->setFixedHeight(155);
    createWg();
}

void PlateHandicapWidget::afterActive()
{

    if(Httpdata::instance()->isHasObjFromFilter(m_stockCode,HZData::BlockWhole))
    {
        QVector<HZData::AllStock> vec;
        HZData::AllStock allStock;
        allStock.stock_code = m_stockCode.toStdString();
        allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();
        vec.push_back(allStock);
        m_plateTitleWidget->updateData(vec);
        m_plateQuoteWidget->updateData(vec);
        m_plateRiseFallWidget->updateData(vec);

        subPlateDyna();
        m_plateChartWidget->afterActive();
    }
    else if(Httpdata::instance()->isHasObjFromFilter(m_stockCode,HZData::MarketAll))
    {
        QVector<HZData::AllStock> vec;
        HZData::AllStock allStock;
        allStock.stock_code = m_stockCode.toStdString();
        allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();
        vec.push_back(allStock);
        m_stockTitleWidget->updateData(vec);
        m_stockTabWidget->updateStock(vec);

        subStockDyna();
        m_stockChartWidget->afterActive();
        m_stockTabWidget->afterActive();
    }
//    m_plateChartWidget->afterActive();
//    m_stockChartWidget->afterActive();
//    m_stockTabWidget->afterActive();
}

void PlateHandicapWidget::afterDeactive()
{
    cancelPlateSub();
    cancelStockSub();
    m_plateChartWidget->afterDeactive();
    m_stockChartWidget->afterDeactive();
    m_stockTabWidget->afterDeactive();
}

void PlateHandicapWidget::OnStockCodeChange()
{

    if(Httpdata::instance()->isHasObjFromFilter(m_stockCode,HZData::BlockWhole))
    {
        m_plateTitleWidget->setStockCode(m_stockCode);
        m_plateChartWidget->setStockCode(m_stockCode);

    }
    else if(Httpdata::instance()->isHasObjFromFilter(m_stockCode,HZData::MarketAll))
    {
        m_stockTitleWidget->setStockCode(m_stockCode);
        m_stockChartWidget->setStockCode(m_stockCode);
        m_stockTabWidget->setStockCode(m_stockCode);
    }
}

void PlateHandicapWidget::createWg()
{
    ScrollWidget *srcollwidget = new ScrollWidget(this);

    QWidget *maintwidget = new QWidget;
    maintwidget->setObjectName("mainwidget");
    maintwidget->setFixedWidth(350);
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    mainlayout->addWidget(m_plateTitleWidget);
    mainlayout->addWidget(srcollwidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_plateChartWidget);
    layout->addWidget(m_plateQuoteWidget);
    layout->addWidget(m_plateRiseFallWidget);
    layout->addWidget(createStockTitle());
    layout->addWidget(m_stockTitleWidget);
    layout->addWidget(m_stockChartWidget);
    layout->addWidget(m_stockTabWidget);

    m_stockTabWidget->setFixedHeight(600);
    m_plateChartWidget->setFixedHeight(220);
    m_stockChartWidget->setFixedHeight(220);

    maintwidget->setLayout(layout);

    srcollwidget->scrollArea()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srcollwidget->scrollArea()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srcollwidget->setWidget(maintwidget);

}

QWidget *PlateHandicapWidget::createStockTitle()
{
    QWidget *widget = new QWidget;
    widget->setFixedHeight(26);
    widget->setObjectName("title");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(12);
    layout->addWidget(new QLabel("所选成分股"));
    widget->setLayout(layout);
    return widget;
}

void PlateHandicapWidget::subPlateDyna()
{
    cancelPlateSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&PlateHandicapWidget::onPlateDynaArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
}

void PlateHandicapWidget::cancelPlateSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

void PlateHandicapWidget::subStockDyna()
{
    cancelStockSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&PlateHandicapWidget::onStockDynaArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nStockReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);

}

void PlateHandicapWidget::cancelStockSub()
{
    if(INVALID_REQID != m_nStockReq)
    {
        gHZDataController->cancelSub(m_nStockReq);
        m_nStockReq = INVALID_REQID;
    }
}


void PlateHandicapWidget::onPlateDynaArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);
        m_plateTitleWidget->updateData(vec);
        m_plateQuoteWidget->updateData(vec);
        m_plateRiseFallWidget->updateData(vec);
    }
}

void PlateHandicapWidget::onStockDynaArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);
        m_stockTitleWidget->updateData(vec);
        m_stockTabWidget->updateStock(vec);
    }
}
