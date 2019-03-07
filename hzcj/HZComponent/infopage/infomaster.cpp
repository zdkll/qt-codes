#include "infomaster.h"
#include <QtWidgets>
#include <QDebug>
#include <QFile>
#include <QDialog>
#include <hzcfg.h>
#include <hzauthenopt.h>
#include <authenpublicdef.h>
#include <localstocksource.h>
#include "infodataprovidor.h"
#include "pubmsgdef.h"
#include "subtab.h"
#include "zxstockopt.h"
#include "hzinfomationdlg.h"

const static QString gModuleNames[] = {
    QStringLiteral("热门"),
    QStringLiteral("7x24快讯"),
    QStringLiteral("经济日历"),
    QStringLiteral("股市日历"),
    QStringLiteral("自选新闻"),
    QStringLiteral("自选公告")
};

QVector<QString> InfoMaster::mZXStocks;
InfoMaster::InfoMaster(QWidget *parent)
    : BasePage(parent)
{
    setObjectName("InfoMaster");
    m_pTab = new CSubTab;
    m_pTab->setFixedHeight(26);
    m_pTab->setObjectName("topTab");
    connect(HZAuthenOpt::instance(), &HZAuthenOpt::loginStateChanged, this, &InfoMaster::getCustomStock);
    connect(gInfoStyle, &InfoStyle::signalStockClicked, this, &InfoMaster::slotStockClicked);
    connect(gInfoStyle, &InfoStyle::signalUpdateCustomStockCache, this, &InfoMaster::getCustomStock);
    connect(m_pTab,&CSubTab::currentButtonChanged,this,&InfoMaster::slotBtnGroupClicked);
    for(int i=0; i<6; i++) {
        m_pTab->addButton(gModuleNames[i],i);
    }
    m_pTab->setStretchLastType(CSubTab::Empty_Strecth);
    m_pTab->setCurrentButton(0);
    m_stackWnd = new QStackedWidget;
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addWidget(m_pTab);
    vLayout->addSpacing(1);
    vLayout->addWidget(m_stackWnd);
    //vLayout->setStretch(1,2);
    setLayout(vLayout);
    getCustomStock();

    m_infoHotData    = new InfoHotProvidor;
    m_multiHourData  = new InfoNultiHourProvidor;
    m_econonicData   = new InfoEconomicCalandarProvidor;
    m_marketCalandarData = new InfoMarketCalandarProvidor;
    m_infoNewsData   = new InfoNewsProvidor;
    m_infoAnncData   = new InfoAnncProvidor;

    m_hotPage        = new HotMarster(m_infoHotData);
    m_multiHourPage  = new MultiHourMarster(m_multiHourData);
    m_economicPage   = new EconomicMaster(m_econonicData);
    m_marketPage     = new MarketMaster(m_marketCalandarData);
    m_newsPage       = new NewsMarster(m_infoNewsData);
    m_anncPage       = new AnncMarster(m_infoAnncData);

    m_stackWnd->insertWidget(0, m_hotPage);
    m_stackWnd->insertWidget(1, m_multiHourPage);
    m_stackWnd->insertWidget(2, m_economicPage);
    m_stackWnd->insertWidget(3, m_marketPage);
    m_stackWnd->insertWidget(4, m_newsPage);
    m_stackWnd->insertWidget(5, m_anncPage);

    //自选股信号曹
    connect(ZXStockOpt::instance(), &ZXStockOpt::stockCountChanged, [=](int count){
        mZXStocks = ZXStockOpt::instance()->getZXStock();
//        gInfoStyle->signalUpdateCustomStockCache();
        if(isVisible()) {
            gInfoStyle->signalUpdateCustomStockCache();
        }
    });

    //跳出详情页
    connect(gInfoStyle, &InfoStyle::signalFinaoReportDetail, [=](const QString &title, const QString &url){
        qDebug() << Q_FUNC_INFO << "url:" <<url;
        QUrl tmpUrl(QUrl::fromUserInput(url));
        if(tmpUrl.isValid()) {
            gInfoDlgEx->setPageInfo(title,url);
        }
    });
}

void InfoMaster::updateOfOptionalStocks()
{
    mZXStocks = ZXStockOpt::instance()->getZXStock();
}

void InfoMaster::afterActive()
{
    BasePage *page = static_cast<BasePage*>(m_stackWnd->currentWidget());
    if(page) page->afterActive();
}

void InfoMaster::afterDeactive()
{
    BasePage *page = static_cast<BasePage*>(m_stackWnd->currentWidget());
    if(page) page->afterDeactive();
}

void InfoMaster::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    int id  = 0;
    if(param.childWndId != -1)
        id = param.childWndId;
    if(m_pTab->currentButton() != id)
        m_pTab->setCurrentButton(id);
    BasePage *page = static_cast<BasePage*>(m_stackWnd->currentWidget());
    if(page) page->afterDeactive();
    m_stackWnd->setCurrentIndex(id);
    page = static_cast<BasePage*>(m_stackWnd->currentWidget());
    if(page && bCurrPage) page->afterActive();
}

void InfoMaster::slotBtnGroupClicked(int id)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(CHILD_WND_ID,QString::number(id));
    params.emplace(HZ_PAGE_ID,QString::number(info_center_page));
    sendMessage(ROOT_FRAME,params,true);
}

void InfoMaster::slotStockClicked(const QString &code)
{
    int childId = m_pTab->currentButton();
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(CHILD_WND_ID, QString::number(childId));
    params.emplace(STOCK_ID,code);
    sendMessage(ROOT_FRAME,params,true);
}

void InfoMaster::getCustomStock()
{
    InfoMaster::mZXStocks = ZXStockOpt::instance()->getZXStock();
}
