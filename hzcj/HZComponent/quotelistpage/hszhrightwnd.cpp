#include "hszhrightwnd.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>

#include "optionstockview.h"
#include "sltableview.h"
#include "pubmsgdef.h"


HszhRightWnd::HszhRightWnd(QWidget *parent)
    :BaseWidget(parent,"HszhRightWnd")
{
    createWg();
}

void HszhRightWnd::afterActive()
{
    m_zxTableView->afterActive();
    ((HszhRankPage*)m_stackedWidget->currentWidget())->afterActive();
}

void HszhRightWnd::afterDeactive()
{
    m_zxTableView->afterDeactive();
    ((HszhRankPage*)m_stackedWidget->currentWidget())->afterDeactive();
}

void HszhRightWnd::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);

    mainLayout->addWidget(createTopWg(),560);

    mainLayout->addWidget(createBottomWg(),373);

    this->setLayout(mainLayout);
}

QWidget *HszhRightWnd::createTopWg()
{
    SuolueWidget *wg = new SuolueWidget(this);

    QVBoxLayout *tpLayout = new QVBoxLayout;
    tpLayout->setContentsMargins(1,1,1,1);
    tpLayout->setSpacing(0);

    CSubTab *cstab = new CSubTab(this);
    cstab->setContentsMargins(0,0,0,0);
    cstab->setPacing(0);

    m_stackedWidget = new QStackedWidget(this);

    m_hszhRankPages.resize(3);
    m_hszhRankPages[0].rankType = Total_Rank;
    m_hszhRankPages[0].tiltle = QStringLiteral("涨幅排名");
    m_hszhRankPages[1].rankType = ThreeMin_RiseFall;
    m_hszhRankPages[1].tiltle = QStringLiteral("3分钟涨跌");
    m_hszhRankPages[2].rankType = FiveDay_RiseFall;
    m_hszhRankPages[2].tiltle = QStringLiteral("5日涨跌");
    //m_hszhRankPages[3].rankType = FromYear_RiseFall;
    //m_hszhRankPages[3].tiltle = QStringLiteral("年初至今涨跌");

    for(int i=0;i<m_hszhRankPages.size();i++){
        cstab->addButton(m_hszhRankPages[i].tiltle,i);
        HszhRankPage *page = new HszhRankPage(m_hszhRankPages[i].rankType,this);
        m_stackedWidget->addWidget(page);

        connect(page,&HszhRankPage::signalDbClickObjItem,this,&HszhRightWnd::slotDbClickItem);
    }

    cstab->setStretchLastType(CSubTab::Empty_Strecth);
    cstab->setFixedHeight(22);
    cstab->setCurrentButton(0);

    tpLayout->addWidget(cstab);
    tpLayout->addWidget(m_stackedWidget);

    wg->setLayout(tpLayout);
    connect(cstab,&CSubTab::currentButtonChanged,this,&HszhRightWnd::currentTabChanged);
    return wg;
}

QWidget *HszhRightWnd::createBottomWg()
{
    SuolueWidget *wg = new SuolueWidget(this);

    QVBoxLayout *btLayout = new QVBoxLayout;
    btLayout->setContentsMargins(1,1,1,1);
    btLayout->setSpacing(0);
    CSubTab *cstab = new CSubTab(this);
    cstab->setContentsMargins(0,0,0,0);
    cstab->setPacing(0);
    cstab->setStretchLastType(CSubTab::Empty_Strecth);
    btLayout->addWidget(cstab);

    cstab->addButton(QStringLiteral("自选股"),0);
    cstab->setCurrentButton(0);
    cstab->setFixedHeight(22);

    m_zxTableView = new ZXStockDynaView(OverallView);

    connect(m_zxTableView,&ZXStockDynaView::itemEntered,this,&HszhRightWnd::slotDbClickItem);
    btLayout->addWidget(m_zxTableView);
    wg->setLayout(btLayout);

    return wg;
}

void  HszhRightWnd::currentTabChanged(int id)
{
    ((HszhRankPage*)m_stackedWidget->currentWidget())->afterDeactive();

    m_stackedWidget->setCurrentIndex(id);
    ((HszhRankPage*)m_stackedWidget->widget(id))->afterActive();
}

void HszhRightWnd::slotDbClickItem(const QString &stockCode)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    msg.emplace(STOCK_ID,stockCode);    //股票和板块ID 如果是板块跳转，这个字段也要求赋值
    sendMessage(ROOT_FRAME,msg,true);
}



///HszhRankPage-----
HszhRankPage::HszhRankPage(const HszhRankType &type, QWidget *parent)
    :BaseWidget(parent),m_rankType(type)
{
    m_riseFallViews =  createViews(m_rankType);
}

void HszhRankPage::afterActive()
{
    for(int i=0;i<m_riseFallViews.size();i++)
        m_riseFallViews[i]->afterActive();
}

void HszhRankPage::afterDeactive()
{
    for(int i=0;i<m_riseFallViews.size();i++)
        m_riseFallViews[i]->afterDeactive();
}

QVector<RiseFallRankInfo> HszhRankPage::getRankInfoByType(const HszhRankType &type)
{
    QVector<RiseFallRankInfo>   rfRankInfo;
    switch (type) {
    case Total_Rank:
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketAll,ColRate,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketAll,ColRate,true));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketAll,ColTurnover,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketAll,ColTurnoverRate,false));
        break;
        //3分钟涨跌幅
    case ThreeMin_RiseFall:
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSH,ColSpeedRate,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSH,ColSpeedRate,true));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSZ,ColSpeedRate,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSZ,ColSpeedRate,true));
        break;
        //5 日涨跌幅
    case FiveDay_RiseFall:
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSH,ColFiveRate,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSH,ColFiveRate,true));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSZ,ColFiveRate,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSZ,ColFiveRate,true));
        break;
        //年初至今涨幅
    case FromYear_RiseFall:
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSH,ColRateYearYet,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSH,ColRateYearYet,true));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSZ,ColRateYearYet,false));
        rfRankInfo.append(RiseFallRankInfo(HZData::MarketSZ,ColRateYearYet,true));
        break;
    default:
        break;
    }
    return rfRankInfo;
}

QList<HszhRiseFallView *> HszhRankPage::createViews(const HszhRankType &type)
{
    //获取页面信息
    QVector<RiseFallRankInfo> riseFallRankInfo = getRankInfoByType(type);

    QList<HszhRiseFallView *> views;

    QGridLayout  *gridLayout = new  QGridLayout;
    gridLayout->setSpacing(1);
    gridLayout->setContentsMargins(1,1,1,1);
    for(int i=0;i<4;i++){
        HszhRiseFallView *view = new HszhRiseFallView(riseFallRankInfo[i],this);

        connect(view,&HszhRiseFallView::signalDbClickObjItem,this,&HszhRankPage::signalDbClickObjItem);

        gridLayout->addWidget(view,i/2,i%2);
        views.append(view);
    }
    this->setLayout(gridLayout);
    return views;
}



///HszhRiseFallView----
HszhRiseFallView::HszhRiseFallView(RiseFallRankInfo rankInfo, QWidget *parent)
    :BaseWidget(parent),m_rankInfo(rankInfo)
{
    createWg();
}

void HszhRiseFallView::afterActive()
{
    m_tableView->afterActive();
}

void HszhRiseFallView::afterDeactive()
{
    m_tableView->afterDeactive();
}

void HszhRiseFallView::createWg()
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(1);

    CSubTab *csTab = new CSubTab(this);
    csTab->setContentsMargins(0,0,0,0);
    csTab->addButton(getTitle(m_rankInfo));
    csTab->setFixedHeight(22);
    vLayout->addWidget(csTab,0);

    csTab->setStretchLastType(CSubTab::Empty_Strecth);

    csTab->getButton(0)->setCheckable(false);
    //具体表格
    m_tableView =  createTableView(m_rankInfo.colIdx);
    //初始设置排序
    m_tableView->initSortIndicator(sortField(m_rankInfo.colIdx),m_rankInfo.is_desc);

    //设置filter
    m_tableView->setFilterFlag(m_rankInfo.filterFlag);

    vLayout->addWidget(m_tableView);

    connect(m_tableView,&HSZHTableView::itemEntered,this,&HszhRiseFallView::signalDbClickObjItem);

    this->setLayout(vLayout);
}

HSZHTableView  *HszhRiseFallView::createTableView(ColumnIdx idx)
{
    switch (idx) {
    case ColRate:
        return new HSZHTableView(HSZHTableView::RiseFallRate,this);
        break;
    case ColTurnover:
        return new HSZHTableView(HSZHTableView::Turnover,this);
        break;
    case ColTurnoverRate:
        return new HSZHTableView(HSZHTableView::TuvRate,this);
        break;
    case ColSpeedRate:
        return new HSZHTableView(HSZHTableView::ThreeMinRate,this);
        break;
    case ColFiveRate:
        return new HSZHTableView(HSZHTableView::FiveDayRate,this);
        break;
    case ColRateYearYet:
        return new HSZHTableView(HSZHTableView::FromYearYet,this);
        break;
    default:
        break;
    }
    return 0;
}

SortField  HszhRiseFallView::sortField(ColumnIdx idx)
{
    switch (idx) {
    case ColRate:
        return HZData::rise_fall_rate;
        break;
    case ColTurnover:
        return HZData::turnover;
        break;
    case ColTurnoverRate:
        return   HZData::turnover_rate;
        break;
    case ColSpeedRate:
        return HZData::speed_rate;
        break;
    case ColFiveRate:
        return  HZData::five_rate;
        break;
    case ColRateYearYet:
        return  HZData::rise_fall_rate;
        break;
    default:
        break;
    }
    return HZData::none_sort;
}

QString HszhRiseFallView::getTitle(const RiseFallRankInfo& rankInfo)
{
    QString title;
    //filter
    if(rankInfo.filterFlag == HZData::MarketSH)
        title += QStringLiteral("沪");
    else if(rankInfo.filterFlag == HZData::MarketSZ)
        title += QStringLiteral("深");

    //ColumnIdx
    switch (rankInfo.colIdx) {
    case ColRate:
        if(!rankInfo.is_desc)
            title += QStringLiteral("涨幅");
        else
            title += QStringLiteral("跌幅");
        break;
    case ColSpeedRate:
        if(!rankInfo.is_desc)
            title += QStringLiteral("3分钟涨幅");
        else
            title += QStringLiteral("3分钟跌幅");
        break;
    case ColFiveRate:
        if(!rankInfo.is_desc)
            title += QStringLiteral("5日涨幅");
        else
            title += QStringLiteral("5日跌幅");
        break;
    case ColRateYearYet:
        if(!rankInfo.is_desc)
            title += QStringLiteral("年初至今涨幅");
        else
            title += QStringLiteral("年初至今跌幅");
        break;
    case ColTurnover:
        title += QStringLiteral("成交额");
        break;
    case ColTurnoverRate:
        title += QStringLiteral("换手率");
        break;
    default:
        break;
    }
    return title;
}
