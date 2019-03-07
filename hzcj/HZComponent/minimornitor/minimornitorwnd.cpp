#include "minimornitorwnd.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QStackedWidget>
#include "minititlebar.h"
#include "selfstockrollpanel.h"
#include "mininavebar.h"
#include "selfstockpanel.h"
#include "marketchangepanel.h"
#include "stockrollstatusbar.h"
#include "hzauthenopt.h"
#include "localstocksource.h"
#include "zxstockopt.h"


MiniMornitorWnd::MiniMornitorWnd(BaseWidget *parent, QString instName):
    BaseWidget(parent, instName),
    m_pRollPanel(new SelfStockRollPanel),
    m_pNavbar(new MiniNaveBar),
    m_pSelfStockPanel(new SelfStockPanel),
    m_pMarketPanel(new MarketChangePanel),
    m_pRollStatusBar(new StockRollStatusBar),
    m_pStackedWidget(new QStackedWidget),
    m_isActive(false),
    m_pHelper(new FramelessHelper(this))
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | windowFlags());

    m_pHelper->setWidgetResizable(false);
    m_pHelper->setTitleHeight(40);
    m_pHelper->activateOn(this);
    this->setWindowOpacity(0.9);
    createWg();
    buildConnect();
//    setPositon();
}

void MiniMornitorWnd::afterActive()
{
    m_isActive = true;
    m_pRollStatusBar->afterActive();
    //    m_pRollPanel->setStockCode("sh603369");

    m_pRollPanel->setStockCode(getStockLists());
    m_pRollPanel->afterActive();

    if(m_pStackedWidget->currentIndex() == 0)
        m_pSelfStockPanel->afterActive();
    else
        m_pMarketPanel->afterActive();
}

void MiniMornitorWnd::afterDeactive()
{
    m_isActive = false;
    m_pRollStatusBar->afterDeactive();
    m_pRollPanel->afterDeactive();

    if(m_pStackedWidget->currentIndex() == 0)
        m_pSelfStockPanel->afterDeactive();
    else
        m_pMarketPanel->afterDeactive();
}

void MiniMornitorWnd::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setMargin(1);
    pMainLayout->setSpacing(0);
//    pMainLayout->setContentsMargins(0,0,0,0);

    MiniTitleBar *pMiniTitle = new MiniTitleBar;


    connect(pMiniTitle, &MiniTitleBar::minMizeClicked, [this]{this->showMinimized();});

    connect(pMiniTitle, &MiniTitleBar::clickClose, [this]{this->close();this->afterDeactive();});

    connect(pMiniTitle, &MiniTitleBar::maxiMize, [this]{emit maxiMize();});

    connect(pMiniTitle, &MiniTitleBar::playBtnClicked, m_pRollPanel, &SelfStockRollPanel::onPlayBtnClicked);

    pMiniTitle->setObjectName("minititle");

    pMainLayout->addWidget(pMiniTitle);
    pMainLayout->addWidget(m_pRollPanel);
    pMainLayout->addWidget(m_pNavbar);
    pMainLayout->addWidget(m_pStackedWidget);
    pMainLayout->addWidget(m_pRollStatusBar);

    m_pStackedWidget->addWidget(m_pSelfStockPanel);
    m_pStackedWidget->addWidget(m_pMarketPanel);


    connect(ZXStockOpt::instance(), &ZXStockOpt::stockCountChanged, [&](const int count){

        if(m_isActive)
        {
            m_pRollPanel->setStockCode(getStockLists());
            m_pRollPanel->afterActive();
        }
    });
}

void MiniMornitorWnd::setPositon()
{
    QRect screenRt = QApplication::desktop()->screenGeometry();
//    this->setFixedSize(230, screenRt.height()*3 / 4);
//    this->move(screenRt.width() -(screenRt.width()*1/4) / 2 + 2, (screenRt.height() - height())/2);



    this->move(screenRt.width() - (screenRt.width() - 1302) / 2, (screenRt.height() - 820) / 2);
}

void MiniMornitorWnd::buildConnect()
{

    connect(m_pNavbar, &MiniNaveBar::clicked,
            [this](int id){
        m_pStackedWidget->setCurrentIndex(id);
        if(id ==0){
            m_pMarketPanel->afterDeactive();
            m_pSelfStockPanel->afterActive();
        }else{
            m_pSelfStockPanel->afterDeactive();
            m_pMarketPanel->afterActive();
        }
    });
}

std::list<string> MiniMornitorWnd::getStockLists()
{

    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();


    QVector<QString> vec;


    vec = zxStockOpt->getZXStock();

    std::list<string> lists;
    foreach (QString str, vec) {
        lists.push_back(str.toStdString());
    }


    return lists;
}


