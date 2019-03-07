#include "selfstockpanel.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

#include "suoluemin.h"
#include "klinecanvas.h"
#include "sltableview.h"
#include "httpdata.h"
#include "sltableview.h"
#include "zxstockopt.h"
SelfStockPanel::SelfStockPanel(BaseWidget *parent):
    BaseWidget(parent)
{
    createWg();

}

void SelfStockPanel::afterActive()
{
    m_zxTableView->afterActive();
    m_minKlineWnd->afterActive();
}

void SelfStockPanel::afterDeactive()
{
    m_zxTableView->afterDeactive();
    m_minKlineWnd->afterDeactive();
}

//QVector<HZData::AllStock> SelfStockPanel::stocks()
//{
////    return m_zxTableView->stocks();
//}

void SelfStockPanel::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0,1,0,0);

    m_zxTableView = new SLOptionTableView(SLOptionTableView::ZXStocks,this);
    m_zxTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    pMainLayout->addWidget(m_zxTableView);

    m_minKlineWnd =new MinTrendWiget();
    pMainLayout->addWidget(m_minKlineWnd);

    //选择股票，通知走势图
//    connect(m_zxTableView,&SLOptionTableView::dataChanged,this,&SelfStockPanel::stockChanged);
    connect(m_zxTableView,&SLOptionTableView::itemSelected,m_minKlineWnd,&MinTrendWiget::switchObj);
}

SelfStockWiget::SelfStockWiget(BaseWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void SelfStockWiget::createWg()
{

}

MinTrendWiget::MinTrendWiget(BaseWidget *parent):
    BaseWidget(parent),
    m_pTitleLabel(new QLabel(this)),
    m_bKline(false)
{
    createWg();
}

void MinTrendWiget::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);
    QWidget *widget = new QWidget;
    m_stackWnd = new QStackedWidget(this);
    m_pMinWidget = new MinCanvas(m_stackWnd);
    m_pMinWidget->setRecordSwitchPage(false);
    m_pKlineWidget = new KlineCanvas(m_stackWnd);
    m_stackWnd->addWidget(m_pMinWidget);
    m_stackWnd->addWidget(m_pKlineWidget);
    widget->setObjectName("title");
    widget->setFixedHeight(26);
    m_stackWnd->setFixedHeight(214);
    m_stackWnd->hide();
    CSubTab *subTab = addCSubTab(CSubTab::Root_Tab,"roottab");
    subTab->addButton(QStringLiteral("分时"), 0);
    subTab->addButton(QStringLiteral("日K"), 1);
    subTab->addButton(QStringLiteral("展开"), 2);

    subTab->getButton(0)->setChecked(true);


    m_pBtn = subTab->getButton(2);

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(14,0, 12,0);
    pLayout->addWidget(m_pTitleLabel);
    m_pTitleLabel->setText(QStringLiteral("上证指数"));
    pLayout->addStretch();
    pLayout->addWidget(subTab);
    widget->setLayout(pLayout);

    pMainLayout->addWidget(widget);
    pMainLayout->addWidget(m_stackWnd);

    m_obj = "sh000001";
    m_objName =QStringLiteral("上证指数");
    m_pKlineWidget->setBaseInfo(m_obj,HZData::KLINE_PERIOD_DAY);
    m_pKlineWidget->setChartsInfo(getKlineChartInfo());
    m_pKlineWidget->createWg();

    m_pMinWidget->setBaseInfo(m_obj);
    m_pMinWidget->setChartsInfo(getMinChartInfo());
    m_pMinWidget->createWg();
    m_pMinWidget->afterActive();

    connect(ZXStockOpt::instance(),&ZXStockOpt::stockCountChanged,this,&MinTrendWiget::slotsZxStockChanged);
}

void MinTrendWiget::afterActive()
{
    BaseWidget* pwnd = (BaseWidget*) m_stackWnd->currentWidget();
    pwnd->afterActive();
}

void MinTrendWiget::afterDeactive()
{
    BaseWidget* pwnd = (BaseWidget*) m_stackWnd->currentWidget();
    pwnd->afterDeactive();
}

void MinTrendWiget::switchObj(const QString &obj)
{
    if(m_bKline)
    {
        m_pKlineWidget->switchStock(obj,HZData::KLINE_PERIOD_DAY);
        m_pMinWidget->setBaseInfo(obj);
    }else
    {
        m_pKlineWidget->setBaseInfo(obj,HZData::KLINE_PERIOD_DAY);
        m_pMinWidget->switchStock(obj);
    }
    m_objName = Httpdata::instance()->getCodeNameByCode(obj);
    if(m_objName.isEmpty())
    {
        m_objName = QStringLiteral(Default_Item_String);
    }
    m_pTitleLabel->setText(m_objName);
}

void MinTrendWiget::slotsZxStockChanged(const int count)
{
    if(count == 0) //自选数据为空时，清理数据
    {
        m_pMinWidget->setBaseInfo("");
        m_pKlineWidget->setBaseInfo("",HZData::KLINE_PERIOD_DAY);
        m_objName = QStringLiteral(Default_Item_String);
        if(this->isVisible())
        {
            if(m_bKline)
            {
                m_pKlineWidget->switchStock("",HZData::KLINE_PERIOD_DAY);
            }else
            {
                m_pMinWidget->switchStock("");
            }
            m_pTitleLabel->setText(m_objName);
        }
    }
}

CSubTab *MinTrendWiget::addCSubTab(CSubTab::CSubTabType subTabType, QString instName)
{
    CSubTab *csubTab = new CSubTab(this,instName);
    csubTab->setObjectName(instName);
    csubTab->setCSubTabType(subTabType);
    connect(csubTab,SIGNAL(currentButtonChanged(int)),this,SLOT(tabCurrentBtnChanged(int)));
    return csubTab;
}

void MinTrendWiget::tabCurrentBtnChanged(int id)
{
    if(2 == id)
    {
        BaseWidget* pWnd = (BaseWidget*)m_stackWnd->currentWidget();
        if(m_stackWnd->isHidden())
        {
            m_stackWnd->show();
            m_pBtn->setText(QStringLiteral("收起"));
            pWnd->afterActive();
        }
        else
        {
            m_stackWnd->hide();
            m_pBtn->setText(QStringLiteral("展开"));
            pWnd->afterDeactive();

        }
    }else if(1 == id)
    {
        if(m_stackWnd->isHidden())
        {
            m_stackWnd->show();
            m_pBtn->setText(QStringLiteral("收起"));
        }
        m_bKline = true;
        m_pMinWidget->afterDeactive();
        m_stackWnd->setCurrentIndex(id);
        m_pKlineWidget->afterActive();

    }else if(0 == id)
    {
        if(m_stackWnd->isHidden())
        {
            m_stackWnd->show();
            m_pBtn->setText(QStringLiteral("收起"));
        }
        m_bKline = false;
        m_pKlineWidget->afterDeactive();
        m_stackWnd->setCurrentIndex(id);
        m_pMinWidget->afterActive();
    }
}

QVector<ChartItemInfo> MinTrendWiget::getMinChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "Minchart";
    item.margins.topMargin = 10;
    item.margins.rightMargin = 15;
    item.attr = ChartHideRightXaxis;
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.margins.rightMargin = 15;
    item.attr = ChartHideRightXaxis;
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    item.attr = ChartHideRightXaxis;
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}

QVector<ChartItemInfo> MinTrendWiget::getKlineChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "KlineChart";
    item.margins.topMargin = 10;
    item.margins.rightMargin = 10;
    item.attr = ChartHideRightXaxis;
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.margins.rightMargin = 10;
    item.attr = ChartHideRightXaxis;
    item.chartClass = "IndicatorChart";
    item.nHeight = 50;
    item.indiId =2;
    info.push_back(item);


    item = ChartItemInfo();
    item.margins.rightMargin = 10;
    item.attr = ChartHideRightXaxis;
    item.chartClass = "KlineTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    item.attr = ChartHideRightXaxis;
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}
