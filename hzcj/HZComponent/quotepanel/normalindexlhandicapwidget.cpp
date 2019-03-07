#include "normalindexlhandicapwidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "indexpartswidget.h"
#include "stockinfowidget.h"
#include "zhubihandicapview.h"
#include "timedealwidget.h"
#include "pubmsgdef.h"
#include "httpdata.h"


NormalIndexHandicapWidget::NormalIndexHandicapWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_handicapTitleWidget(new HandicapTitleWidget),
    m_pIndexDetailQuoteWidget(new IndexDetailQuoteWidget)
{
    createWg();
}

void NormalIndexHandicapWidget::afterActive()
{
    QVector<HZData::AllStock> vec;
    HZData::AllStock allStock;
    allStock.stock_code = m_stockCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();
    vec.push_back(allStock);
    m_pIndexDetailQuoteWidget->updateData(vec);
    m_handicapTitleWidget->updateData(vec);

    subDyna();
    m_zhubiHandicapView->afterActive();
}

void NormalIndexHandicapWidget::afterDeactive()
{
    cancelSub();
    m_zhubiHandicapView->afterDeactive();
    MessageParams params;
    params.emplace(HZ_CMD, HIDE_ALL_TIME_DEAL_DLG);
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params);
}

void NormalIndexHandicapWidget::OnStockCodeChange()
{
    m_handicapTitleWidget->setStockCode(m_stockCode);
    m_zhubiHandicapView->setStockCode(m_stockCode);
}

void NormalIndexHandicapWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    mainLayout->addWidget(m_handicapTitleWidget);
    mainLayout->addWidget(m_pIndexDetailQuoteWidget);

    mainLayout->addWidget(createZhuBiPanel());
}

QWidget *NormalIndexHandicapWidget::createZhuBiPanel()
{
    QWidget *mainwidget = new QWidget;
    QWidget *pTitle = new QWidget;
    pTitle->setFixedHeight(26);
    pTitle->setObjectName("title");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(12);

    pTitle->setObjectName("title");

    QLabel *label = new QLabel;
    label->setText("分笔成交");
    layout->addWidget(label);
    layout->addSpacing(20);
    label = new QLabel;
    label->setText("分价表");
    label->setObjectName("tickPrice");
    layout->addWidget(label);

    QPushButton* pButton = new QPushButton;
    pButton->setFixedSize(18,18);
    pButton->setObjectName("clickTick");
    connect(pButton,&QPushButton::clicked,this,&NormalIndexHandicapWidget::soltButtonSwitcTick);
    layout->addWidget(pButton);
    layout->addSpacing(12);

    pTitle->setLayout(layout);

    //添加逐笔
    m_zhubiHandicapView = new ZhubiHandicapView(ZhubiHandicapView::ZB_Index,this);

    pTitle->setLayout(layout);
    mainlayout->addWidget(pTitle);
    mainlayout->addWidget(m_zhubiHandicapView);
    mainwidget->setLayout(mainlayout);
    return mainwidget;
}

void NormalIndexHandicapWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&NormalIndexHandicapWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
}

void NormalIndexHandicapWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}


void NormalIndexHandicapWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);
        m_pIndexDetailQuoteWidget->updateData(vec);
        //m_indexRiseFallWidget->updateData(vec);
        m_handicapTitleWidget->updateData(vec);
    }
}

void NormalIndexHandicapWidget::soltButtonSwitcTick()
{
    MessageParams params;
    params.emplace(HZ_CMD, SHOW_ALL_TIME_DEAL_DLG);
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params);
}
