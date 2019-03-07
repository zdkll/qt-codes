#include "indexdetailhandicapwidget.h"
#include <QBoxLayout>
#include <QLabel>
#include "indexpartswidget.h"
#include "stockinfowidget.h"
#include "fundhandicap.h"
#include "httpdata.h"


IndexDetailHandicapWidget::IndexDetailHandicapWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_handicapTitleWidget(new HandicapTitleWidget),
    m_pIndexDetailQuoteWidget(new IndexDetailQuoteWidget),
//    m_indexRiseFallWidget(new IndexRiseFallWidget),
    m_fundHandicap(new FundHandicap)
{
    createWg();
}

void IndexDetailHandicapWidget::afterActive()
{
    QVector<HZData::AllStock> vec;
    HZData::AllStock allStock;
    allStock.stock_code = m_stockCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();
    vec.push_back(allStock);
    m_pIndexDetailQuoteWidget->updateData(vec);
    m_handicapTitleWidget->updateData(vec);
    subDyna();
    m_fundHandicap->afterActive();
}

void IndexDetailHandicapWidget::afterDeactive()
{
    cancelSub();
    m_fundHandicap->afterDeactive();
}

void IndexDetailHandicapWidget::OnStockCodeChange()
{
    m_handicapTitleWidget->setStockCode(m_stockCode);
    m_fundHandicap->setStock(m_stockCode);
}

void IndexDetailHandicapWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    mainLayout->addWidget(m_handicapTitleWidget);
    mainLayout->addWidget(m_pIndexDetailQuoteWidget);
//    mainLayout->addWidget(m_indexRiseFallWidget);

    mainLayout->addWidget(createFundPanel());

//    mainLayout->addStretch();
}

QWidget *IndexDetailHandicapWidget::createFundPanel()
{
    QWidget *mainwidget = new QWidget;
    QWidget *widget = new QWidget;
    widget->setFixedHeight(26);
    widget->setObjectName("fundtitle");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(12);
    layout->addWidget(new QLabel("资金"));
    widget->setLayout(layout);
    mainlayout->addWidget(widget);
    mainlayout->addWidget(m_fundHandicap);
    mainwidget->setLayout(mainlayout);
    return mainwidget;
}

void IndexDetailHandicapWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&IndexDetailHandicapWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
}

void IndexDetailHandicapWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}


void IndexDetailHandicapWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);
        m_pIndexDetailQuoteWidget->updateData(vec);
//        m_indexRiseFallWidget->updateData(vec);
        m_handicapTitleWidget->updateData(vec);
    }
}
