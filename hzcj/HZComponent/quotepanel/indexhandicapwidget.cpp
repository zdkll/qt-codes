#include "indexhandicapwidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "stockinfowidget.h"
#include "indexpartswidget.h"
#include "zhubihandicapview.h"
#include "timedealwidget.h"
#include "pubmsgdef.h"
#include "httpdata.h"

IndexHandicapWidget::IndexHandicapWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_period(HZData::KLINE_PERIOD_MIN)
{
    createWg();
}

IndexHandicapWidget::~IndexHandicapWidget()
{

}

void IndexHandicapWidget::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);
    pMainLayout->addWidget(createQuoteTitleWnd());
    pMainLayout->addWidget(createQuoteBodyWnd());
    //    pMainLayout->addWidget(createRiseFallWnd());
    pMainLayout->addWidget(createMinWnd());
    pMainLayout->addWidget(createKlineWnd());
    pMainLayout->addWidget(createFenbiWnd());


    //    pMainLayout->addStretch();
}

void IndexHandicapWidget::afterActive()
{

    QVector<HZData::AllStock> vec;
    HZData::AllStock allStock;
    allStock.stock_code = m_stockCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCodeFromIndex(m_stockCode).toStdString();

    vec.push_back(allStock);
    m_handicapTitleWidget->updateData(vec);
    m_indexQuoteWidget->updateData(vec);

    subDyna();
    m_minWnd->afterActive();
    m_klineWnd->afterActive();

    m_zhubiHandicapView->afterActive();

}

void IndexHandicapWidget::afterDeactive()
{
    cancelSub();
    m_minWnd->afterDeactive();
    m_klineWnd->afterDeactive();
    m_zhubiHandicapView->afterDeactive();
    MessageParams params;
    params.emplace(HZ_CMD, HIDE_ALL_TIME_DEAL_DLG);
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params);
}

void IndexHandicapWidget::OnStockCodeChange()
{
    m_handicapTitleWidget->setStockCode(m_stockCode);
    m_minWnd->setBaseInfo(m_stockCode);
    m_klineWnd->setBaseInfo(m_stockCode,m_period);
    m_zhubiHandicapView->setStockCode(m_stockCode);
}

QWidget *IndexHandicapWidget::createQuoteTitleWnd()
{
    m_handicapTitleWidget = new  HandicapTitleWidget;
    return m_handicapTitleWidget;
}

QWidget *IndexHandicapWidget::createQuoteBodyWnd()
{
    m_indexQuoteWidget = new  IndexQuoteWidget;
    return m_indexQuoteWidget;
}

QWidget *IndexHandicapWidget::createRiseFallWnd()
{
    m_indexRiseFallWidget = new IndexRiseFallWidget;
    return m_indexRiseFallWidget;
}

QWidget *IndexHandicapWidget::createMinWnd()
{
    QWidget *mainwidget = new QWidget;
    QVBoxLayout *mainlayout = new QVBoxLayout(mainwidget);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    QWidget *widget = new QWidget;
    widget->setFixedHeight(26);
    widget->setObjectName("mintitle");

    QHBoxLayout *pHlayout = new QHBoxLayout(widget);
    pHlayout->setSpacing(0);
    pHlayout->setMargin(0);
    pHlayout->addSpacing(12);
    pHlayout->addWidget(new QLabel("分时图"));
    widget->setLayout(pHlayout);

    m_minWnd = new MinCanvas(this);

    m_minWnd->setFixedHeight(190);

    m_minWnd->setBaseInfo(m_stockCode);
    m_minWnd->setChartsInfo(getMinChartInfo());
    m_minWnd->createWg();

    mainlayout->addWidget(widget);
    mainlayout->addWidget(m_minWnd);
    mainwidget->setLayout(mainlayout);
    return mainwidget;
}

QWidget *IndexHandicapWidget::createKlineWnd()
{
    QWidget *mainwidget = new QWidget;
    QVBoxLayout *mainlayout = new QVBoxLayout(mainwidget);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    QWidget *widget = new QWidget;
    widget->setFixedHeight(26);
    widget->setObjectName("klinetitle");

    QHBoxLayout *pHlayout = new QHBoxLayout;
    pHlayout->setSpacing(0);
    pHlayout->setMargin(0);
    pHlayout->addSpacing(12);
    pHlayout->addWidget(new QLabel("k线图"));
    widget->setLayout(pHlayout);

    m_klineWnd = new KlineCanvas(this);
    m_klineWnd->setFixedHeight(190);
    m_klineWnd->setBaseInfo(m_stockCode,m_period);
    m_klineWnd->setChartsInfo(getKlineChartInfo());
    m_klineWnd->createWg();

    mainlayout->addWidget(widget);
    mainlayout->addWidget(m_klineWnd);
    mainwidget->setLayout(mainlayout);
    return mainwidget;
}

QWidget *IndexHandicapWidget::createFenbiWnd()
{
    QWidget *mainwidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    //逐笔，分价表标题
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    QWidget *pTitle = new QWidget;
    pTitle->setObjectName("fbtitle");

    QLabel *fblabel = new QLabel;
    fblabel->setText("分笔成交");
    titleLayout->addWidget(fblabel);
    titleLayout->addSpacing(20);
    QLabel *fjlabel = new QLabel;
    fjlabel->setText("分价表");
    fjlabel->setObjectName("tickPrice");
    titleLayout->addWidget(fjlabel);


    QPushButton* pButton = new QPushButton;
    pButton->setFixedSize(18,18);
    pButton->setObjectName("clickTick");
    connect(pButton,&QPushButton::clicked,this,&IndexHandicapWidget::soltButtonSwitcTick);
    titleLayout->addWidget(pButton);
    titleLayout->addSpacing(12);

    pTitle->setLayout(titleLayout);
    pTitle->setFixedHeight(26);
    mainLayout->addWidget(pTitle);

    //添加逐笔
    m_zhubiHandicapView = new ZhubiHandicapView(ZhubiHandicapView::ZB_Index,this);
    mainLayout->addWidget(m_zhubiHandicapView);

    mainwidget->setLayout(mainLayout);

    return mainwidget;
}


void IndexHandicapWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);
        m_handicapTitleWidget->updateData(vec);
        m_indexQuoteWidget->updateData(vec);
        //        m_indexRiseFallWidget->updateData(vec);
    }
}

void IndexHandicapWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&IndexHandicapWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);

}

void IndexHandicapWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

QVector<ChartItemInfo> IndexHandicapWidget::getMinChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "Minchart";
    item.type = ChartMainType;
    item.attr = MinChartShowObjCode|MinTitleShowCanvasName|MinTitleShowAvgData|MinChartDrawRealLine;
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithPlusBt|ChartTitleWithSubBt;
    info.push_back(item);

    item = ChartItemInfo();
    item.padding = 1;
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);

    return info;
}

QVector<ChartItemInfo> IndexHandicapWidget::getKlineChartInfo()
{
    QVector<ChartItemInfo> info;
    ChartItemInfo item;
    item.chartClass = "KlineChart";
    //    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithPlusBt|ChartTitleWithSubBt;
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.padding = 3;
    item.chartClass = "KlineTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}

void IndexHandicapWidget::soltButtonSwitcTick()
{
    MessageParams params;
    params.emplace(HZ_CMD, SHOW_ALL_TIME_DEAL_DLG);
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params);
}
