#include "hszhleftwnd.h"
#include <QBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>
#include "subtab.h"
#include "updownwidget.h"
#include "lzpaltepanel.h"
#include "fundflowpanel.h"
#include "pubmsgdef.h"

HszhMinWidget::HszhMinWidget(QWidget *parent):
    BaseWidget(parent),
    m_pMainLayout(new QVBoxLayout(this))
{
    m_stockCode = "sh000001";
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setMargin(0);

    createTitleBar();
    createWg();
}

void HszhMinWidget::afterActive()
{
    m_minChart->afterActive();
}

void HszhMinWidget::afterDeactive()
{
    m_minChart->afterDeactive();
}

void HszhMinWidget::setTitle(const QString &strName)
{
    QString strTmp = strName;
    strTmp += "走势图";
    m_pLabelName->setText(strTmp);
}

void HszhMinWidget::switchStockEx(const QString &strCode)
{
   m_stockCode = strCode;
   m_minChart->switchStockEx(strCode);
}

void HszhMinWidget::createTitleBar()
{
    m_pLabelName = new QLabel("上证指数趋势图");
    QWidget *widget = new QWidget(this);
    widget->setObjectName("mintitle");
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(18, 0, 0, 0);
    pLayout->addWidget(m_pLabelName);

    widget->setLayout(pLayout);
    m_pMainLayout->addWidget(widget, 0, Qt::AlignTop);

}

void HszhMinWidget::createWg()
{
    m_minChart = new MinCanvas(this);
    m_minChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if(!m_stockCode.isEmpty())
    {
        m_minChart->setBaseInfo(m_stockCode);
        m_minChart->setChartsInfo(getChartInfo());
        m_minChart->createWg();
    }
    m_pMainLayout->addWidget(m_minChart);
}


QVector<ChartItemInfo> HszhMinWidget::getChartInfo()
{
    QVector<ChartItemInfo> info;
    ChartItemInfo item;
    item.margins.topMargin = 10;
    item.chartClass = "Minchart";
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}

void HszhMinWidget::OnStockCodeChange()
{
    m_minChart->switchStockEx(m_stockCode);
}


HszhLeftTopWnd::HszhLeftTopWnd(QWidget *parent):
    BaseWidget(parent),
    m_pMainLaytout(new QVBoxLayout(this))
{
    createWg();
}

void HszhLeftTopWnd::afterActive()
{
    m_pleftMinWidget->afterActive();
    m_prightMinWidget->afterActive();
    m_pUpDownWidget->afterActive();
}

void HszhLeftTopWnd::afterDeactive()
{
    m_pleftMinWidget->afterDeactive();
    m_prightMinWidget->afterDeactive();
    m_pUpDownWidget->afterDeactive();
}

void HszhLeftTopWnd::createWg()
{
    m_pMainLaytout->setSpacing(0);
    m_pMainLaytout->setContentsMargins(0, 2, 0, 1);
    createNavBar();
    createUpDownPanel();
    createMinWidget();
}

void HszhLeftTopWnd::createNavBar()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("title");
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);

    m_pSubTab = new CSubTab;
    m_pSubTab->setFixedHeight(22);
    m_pSubTab->setObjectName("root");
    m_pSubTab->setExclusiveEx(false);

    QPushButton *leftBtn = new QPushButton;
    QPushButton *rightBtn = new QPushButton;

    leftBtn->setObjectName("left");
    rightBtn->setObjectName("right");

    connect(leftBtn ,&QPushButton::clicked, this, &HszhLeftTopWnd::onLeftBtnClicked);
    connect(rightBtn ,&QPushButton::clicked, this, &HszhLeftTopWnd::onRightBtnClicked);

    QStringList list;
    list << "上证指数" <<"深证成指" << "中小板指"
         <<"创业板指" << "沪深300" << "上证50";

    m_pSubTab->addButtons(list);
    m_pSubTab->setStretchLastType(CSubTab::Empty_Strecth);
    getNavBtn();
    pLayout->addWidget(m_pSubTab);
    pLayout->addWidget(leftBtn);
    pLayout->addSpacing(4);
    pLayout->addWidget(rightBtn);
    pLayout->addSpacing(10);

    widget->setLayout(pLayout);
    m_pMainLaytout->addWidget(widget, 0, Qt::AlignTop);


    connect(m_pSubTab, &CSubTab::currentButtonChanged, this, &HszhLeftTopWnd::onButtonChanged);
}

void HszhLeftTopWnd::createUpDownPanel()
{
    m_pUpDownWidget = new UpDownWidget;
    m_pUpDownWidget->setStockCode("000000");
    m_pMainLaytout->addWidget(m_pUpDownWidget, 0, Qt::AlignTop);

}

void HszhLeftTopWnd::createMinWidget()
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(1, 0, 1, 0);

    m_pleftMinWidget = new HszhMinWidget;

    m_pleftMinWidget->setObjectName("leftmin");

    m_prightMinWidget = new HszhMinWidget;

    m_prightMinWidget->setObjectName("rightmin");

    m_pleftMinWidget->setStockCode("sh000001");

    m_pleftMinWidget->setTitle("上证指数");

    m_prightMinWidget->setStockCode("sz399001");

    m_prightMinWidget->setTitle("深证成指");

    layout->addWidget(m_pleftMinWidget, 1);

    layout->addWidget(m_prightMinWidget, 1);

    m_pMainLaytout->addLayout(layout);

}

void HszhLeftTopWnd::getNavBtn()
{
    for(int i = 0; i < 6; i++)
    {
        QPushButton *btn = m_pSubTab->getButton(i);
        m_btnVec.append(btn);
    }
    m_btnVec[0]->setChecked(true);
    m_btnVec[1]->setChecked(true);
}

void HszhLeftTopWnd::switchStockEx(int id)
{
    if(0 == id || 1 == id)
    {
        m_pleftMinWidget->setTitle("上证指数");
        m_pleftMinWidget->switchStockEx("sh000001");
        m_prightMinWidget->setTitle("深证成指");
        m_prightMinWidget->switchStockEx("sz399001");
    }
    else if(2 == id || 3 == id)
    {
        m_pleftMinWidget->setTitle("中小板指");
        m_pleftMinWidget->switchStockEx("sz399005");
        m_prightMinWidget->setTitle("创业板指");
        m_prightMinWidget->switchStockEx("sz399006");
    }
    else if(4 == id || 5 == id)
    {
        m_pleftMinWidget->setTitle("沪深300");
        m_pleftMinWidget->switchStockEx("sh000300");
        m_prightMinWidget->setTitle("上证50");
        m_prightMinWidget->switchStockEx("sh000016");
    }

}

void HszhLeftTopWnd::onButtonChanged(int id)
{
    if(0 == id || 1 == id)
    {
        m_btnVec[0]->setChecked(true);
        m_btnVec[1]->setChecked(true);
        m_btnVec[2]->setChecked(false);
        m_btnVec[3]->setChecked(false);
        m_btnVec[4]->setChecked(false);
        m_btnVec[5]->setChecked(false);
    }
    else if(2 == id || 3 == id)
    {
        m_btnVec[0]->setChecked(false);
        m_btnVec[1]->setChecked(false);
        m_btnVec[2]->setChecked(true);
        m_btnVec[3]->setChecked(true);
        m_btnVec[4]->setChecked(false);
        m_btnVec[5]->setChecked(false);
    }
    else if(4 == id || 5 == id)
    {
        m_btnVec[0]->setChecked(false);
        m_btnVec[1]->setChecked(false);
        m_btnVec[2]->setChecked(false);
        m_btnVec[3]->setChecked(false);
        m_btnVec[4]->setChecked(true);
        m_btnVec[5]->setChecked(true);
    }
    switchStockEx(id);
}

void HszhLeftTopWnd::onLeftBtnClicked()
{
    for(int i = 0; i < m_btnVec.size(); i++)
    {
        if(m_btnVec[i]->isChecked())
        {
            if(0 == i || 1 == i)
            {
                break;
            }
            else if(2 == i || 3 == i)
            {
                m_btnVec[2]->setChecked(false);
                m_btnVec[3]->setChecked(false);
                m_btnVec[0]->setChecked(true);
                m_btnVec[1]->setChecked(true);
                switchStockEx(1);
                break;
            }
            else if(4 == i || 5 == i)
            {
                m_btnVec[4]->setChecked(false);
                m_btnVec[5]->setChecked(false);
                m_btnVec[2]->setChecked(true);
                m_btnVec[3]->setChecked(true);
                switchStockEx(2);
                break;
            }
        }
    }
}

void HszhLeftTopWnd::onRightBtnClicked()
{
    for(int i = 0; i < m_btnVec.size(); i++)
    {
        if(m_btnVec[i]->isChecked())
        {
            if(4 == i || 5 == i)
            {
                break;
            }
            else if(0 == i || 1 == i)
            {
                m_btnVec[0]->setChecked(false);
                m_btnVec[1]->setChecked(false);
                m_btnVec[2]->setChecked(true);
                m_btnVec[3]->setChecked(true);
                switchStockEx(2);
                break;
            }
            else if(2 == i || 3 == i)
            {
                m_btnVec[2]->setChecked(false);
                m_btnVec[3]->setChecked(false);
                m_btnVec[4]->setChecked(true);
                m_btnVec[5]->setChecked(true);
                switchStockEx(4);
                break;
            }
        }
    }
}


HszhLeftCenterWnd::HszhLeftCenterWnd(QWidget *parent):
    BaseWidget(parent),
    m_pMainLayout(new QVBoxLayout(this)),
    m_reqId(INVALID_REQID)
{
    this->setFixedHeight(208);
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);
    createWg();

    m_reqBatchInfo.filter = HZData::BlockWhole;
    m_reqBatchInfo.sort_field =  HZData::block_rate;
    m_reqBatchInfo.is_desc = false;
    m_reqBatchInfo.count_ange = 8;
    m_reqBatchInfo.mask = HZData::Block_Mask;
}

void HszhLeftCenterWnd::afterActive()
{
    subBatchDyna();
}

void HszhLeftCenterWnd::afterDeactive()
{
    cancelSub();
}

void HszhLeftCenterWnd::subBatchDyna()
{
    cancelSub();

    OnStockDynaArrived fun = std::bind(&HszhLeftCenterWnd::onBatchDynaArrived, this, std::placeholders::_1
                                       , std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    m_reqId = gHZDataController->subBatchDyna(m_reqBatchInfo, fun);
}

void HszhLeftCenterWnd::cancelSub()
{
    if(INVALID_REQID != m_reqId)
    {
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}

void HszhLeftCenterWnd::onBatchDynaArrived(ReqId ,const char * data, int num, ErrCode errCode)
{
    if(0 != errCode)  return;

    qDebug()<<"HszhLeftCenterWnd left center wind recv data 404 =======";
    HZData::Block *blocks = (HZData::Block *)data;
    for(int i = 0; i < num; i++)
        m_lzPlateVec[i]->updateData(blocks[i]);
}






///HszhLeftBottomWnd--
HszhLeftBottomWnd::HszhLeftBottomWnd(QWidget *parent):
    BaseWidget(parent),
    m_pMainLayout(new QVBoxLayout(this))
  ,m_reqId(INVALID_REQID)
{
    this->setFixedHeight(162);
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);
    createWg();

    m_reqBatchFunInfo.filter = HZData::MarketAll;
    m_reqBatchFunInfo.sort_field =  HZData::leader_money_netput;
    m_reqBatchFunInfo.is_desc = false;
    m_reqBatchFunInfo.index = 0;
    m_reqBatchFunInfo.count_range = 4;
    m_reqBatchFunInfo.mask  = HZData::MoneyField_Mask;
}

void HszhLeftBottomWnd::afterActive()
{
    subBatchDyna();
}

void HszhLeftBottomWnd::afterDeactive()
{
    cancelSub();
}

void HszhLeftBottomWnd::createWg()
{
    createTitleBar();
    createBody();
}


void HszhLeftBottomWnd::subBatchDyna()
{
    cancelSub();

    OnMoneyArrived func = std::bind(&HszhLeftBottomWnd::onBatchFundArrived, this, std::placeholders::_1
                                    , std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

    m_reqId = gHZDataController->subBatchMoney(m_reqBatchFunInfo,func);
}


void HszhLeftBottomWnd::cancelSub()
{
    if(INVALID_REQID != m_reqId)
    {
        gHZDataController->cancelSub(m_reqId);
        m_reqId = INVALID_REQID;
    }
}


void HszhLeftBottomWnd::onBatchFundArrived(ReqId ,const char *data,int num,ErrCode errCode)
{
    if(0 != errCode)  return;

    HZData::StockMoney *stockMoneys = (HZData::StockMoney *)data;
    for(int i = 0; i < num; i++)
        m_pFundPanelVec[i]->updateData(stockMoneys[i]);
}






///HszhLeftWnd------
HszhLeftWnd::HszhLeftWnd(QWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void HszhLeftWnd::afterActive()
{
    m_pLeftTopWidget->afterActive();
    m_pLeftCenterWidget->afterActive();
    m_pLeftBottomWidget->afterActive();
}

void HszhLeftWnd::afterDeactive()
{
    m_pLeftTopWidget->afterDeactive();
    m_pLeftCenterWidget->afterDeactive();
    m_pLeftBottomWidget->afterDeactive();
}

void HszhLeftWnd::createWg()
{
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(3);
    pLayout->setMargin(3);

    m_pLeftTopWidget = new HszhLeftTopWnd;
    m_pLeftCenterWidget = new HszhLeftCenterWnd;
    m_pLeftBottomWidget = new HszhLeftBottomWnd;

    pLayout->addWidget(m_pLeftTopWidget);
    pLayout->addWidget(m_pLeftCenterWidget);
    pLayout->addWidget(m_pLeftBottomWidget);
}




void HszhLeftCenterWnd::createWg()
{
    createTitleBar();
    createBody();
}

void HszhLeftCenterWnd::createTitleBar()
{
    QWidget *widget = new QWidget(this);
    widget->setObjectName("title");
    QPushButton *btn = new QPushButton;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(17, 0, 6, 0);
    layout->addWidget(new QLabel("领涨板块"));
    layout->addStretch();
    layout->addWidget(btn);
    widget->setLayout(layout);
    m_pMainLayout->addWidget(widget, 0, Qt::AlignTop);

    connect(btn, &QPushButton::clicked, [&]{
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(CHILD_WND_ID,QString::number(103));
        params.emplace(HZ_PAGE_ID,QString::number(block_list_page));
        sendMessage(ROOT_FRAME,params,true);
    });
}

void HszhLeftCenterWnd::createBody()
{
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    LzPaltePanel *panel1 = new  LzPaltePanel;
    LzPaltePanel *panel2 = new  LzPaltePanel;
    LzPaltePanel *panel3 = new  LzPaltePanel;
    LzPaltePanel *panel4 = new  LzPaltePanel;
    LzPaltePanel *panel5 = new  LzPaltePanel;
    LzPaltePanel *panel6 = new  LzPaltePanel;
    LzPaltePanel *panel7 = new  LzPaltePanel;
    LzPaltePanel *panel8 = new  LzPaltePanel;
    m_lzPlateVec.append(panel1);
    m_lzPlateVec.append(panel2);
    m_lzPlateVec.append(panel3);
    m_lzPlateVec.append(panel4);
    m_lzPlateVec.append(panel5);
    m_lzPlateVec.append(panel6);
    m_lzPlateVec.append(panel7);
    m_lzPlateVec.append(panel8);

    panel1->setObjectName("first");
    panel2->setObjectName("second");
    panel3->setObjectName("third");
    panel4->setObjectName("forth");
    panel5->setObjectName("fifth");
    panel6->setObjectName("sixth");
    panel7->setObjectName("seventh");
    panel8->setObjectName("eighth");


    layout->addWidget(panel1, 0, 0);
    layout->addWidget(panel2, 0, 1);
    layout->addWidget(panel3, 0, 2);
    layout->addWidget(panel4, 0, 3);
    layout->addWidget(panel5, 1, 0);
    layout->addWidget(panel6, 1, 1);
    layout->addWidget(panel7, 1, 2);
    layout->addWidget(panel8, 1, 3);


    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 1);

    m_pMainLayout->addLayout(layout);
}


void HszhLeftBottomWnd::createTitleBar()
{
    QWidget *widget = new QWidget(this);
    widget->setObjectName("title");
    QPushButton *btn = new QPushButton;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(17, 0, 6, 0);
    layout->addWidget(new QLabel("资金流向"));
    layout->addStretch();
    layout->addWidget(btn);
    widget->setLayout(layout);
    m_pMainLayout->addWidget(widget, 0, Qt::AlignTop);

    connect(btn, &QPushButton::clicked, [&]{
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(quote_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(HsagListWndType));
        msg.emplace(THIRD_CHILD_WND_ID,QString::number(BtFundFlowerFlowType));
        sendMessage(ROOT_FRAME,msg,true);

    });
}

void HszhLeftBottomWnd::createBody()
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    FundFlowPanel *p1 = new FundFlowPanel;
    FundFlowPanel *p2 = new FundFlowPanel;
    FundFlowPanel *p3 = new FundFlowPanel;
    FundFlowPanel *p4 = new FundFlowPanel;

    m_pFundPanelVec.append(p1);
    m_pFundPanelVec.append(p2);
    m_pFundPanelVec.append(p3);
    m_pFundPanelVec.append(p4);

    p1->setObjectName("first");
    p2->setObjectName("second");
    p3->setObjectName("third");
    p4->setObjectName("forth");

    layout->addWidget(p1);
    layout->addWidget(p2);
    layout->addWidget(p3);
    layout->addWidget(p4);


    m_pMainLayout->addLayout(layout);
}

