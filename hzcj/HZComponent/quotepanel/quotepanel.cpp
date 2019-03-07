#include "quotepanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include "timedealwidget.h"
#include "buysellfivewidget.h"
#include "zhubihandicapview.h"
#include "pubmsgdef.h"

QuotePanel::QuotePanel(QWidget *parent, QString instName):
    //    BaseWidget(parent,instName)
    ScrollWidget(parent,instName)
{
    createWg();
}

QuotePanel::~QuotePanel()
{
}

void QuotePanel::createWg()
{
    QWidget *mainwidget = new QWidget(this);
    mainwidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addSpacing(1);


    //五档买卖标题栏
    QHBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    QWidget* pTitle = new QWidget;
    pTitle->setObjectName("title");
    QLabel* label = new QLabel;
    label->setText("五档买卖");
    titleLayout->addWidget(label);
    pTitle->setLayout(titleLayout);
    pTitle->setFixedHeight(26);
    pLayout->addWidget(pTitle);
    pLayout->addSpacing(1);

    //五档买卖
    m_buySellFiveWidget = new BuySellFiveWidget;
    pLayout->addWidget(m_buySellFiveWidget, 0, Qt::AlignTop);
    pLayout->addSpacing(1);

    //逐笔，分价表标题
    titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    pTitle = new QWidget;
    pTitle->setObjectName("title");

    label = new QLabel;
    label->setText("分笔成交");
    titleLayout->addWidget(label);
    titleLayout->addSpacing(20);
    label = new QLabel;
    label->setText("分价表");
    label->setObjectName("tickPrice");
    titleLayout->addWidget(label);


    QPushButton* pButton = new QPushButton;
    pButton->setFixedSize(18,18);
    pButton->setObjectName("clickTick");
    connect(pButton,&QPushButton::clicked,this,&QuotePanel::soltButtonSwitcTick);
    titleLayout->addWidget(pButton);
    titleLayout->addSpacing(12);

    pTitle->setLayout(titleLayout);
    pTitle->setFixedHeight(26);
    pLayout->addWidget(pTitle);
    //    pLayout->addSpacing(1);

    //添加逐笔
    m_zhubiHandicapView = new ZhubiHandicapView;
    pLayout->addWidget(m_zhubiHandicapView);
    //   this->setLayout(pLayout);
    mainwidget->setLayout(pLayout);
    mainwidget->setObjectName("mainwidget");

    mainwidget->setFixedWidth(350);
    this->scrollArea()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->scrollArea()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setWidget(mainwidget);
}

void QuotePanel::updateStock(const QVector<HZData::AllStock> &data)
{
    m_data = data;
    if(m_buySellFiveWidget && m_data.isEmpty() == false)
        m_buySellFiveWidget->updateData(data);
}

void QuotePanel::afterActive()
{
    if(m_buySellFiveWidget && m_data.isEmpty() == false)
        m_buySellFiveWidget->updateData(m_data);
    m_zhubiHandicapView->afterActive();
}

void QuotePanel::afterDeactive()
{
    m_zhubiHandicapView->afterDeactive();
    MessageParams params;
    params.emplace(HZ_CMD, HIDE_ALL_TIME_DEAL_DLG);
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params);
}

void QuotePanel::soltButtonSwitcTick()
{
    MessageParams params;
    params.emplace(HZ_CMD, SHOW_ALL_TIME_DEAL_DLG);
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params);
}

void QuotePanel::OnStockCodeChange()
{
    m_zhubiHandicapView->setStockCode(m_stockCode);
}
