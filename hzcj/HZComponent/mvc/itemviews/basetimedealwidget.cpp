#include "basetimedealwidget.h"
#include "timedealtableview.h"

#include "../itemmodels/timedealmodel.h"
#include "../itemdelegates/timedealitemdelegates.h"

#include <QScrollBar>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QLabel>

///BaseTimeDealWidget
BaseTimeDealWidget::BaseTimeDealWidget(QWidget *parent,const QString &instName)
    :BaseWidget(parent,instName),isInitedStyle(false)
    ,m_titleWg(0),m_isInitialed(false)
    ,m_isActived(false)
{
    initWg();
}

BaseTimeDealWidget::~BaseTimeDealWidget()
{

}


void BaseTimeDealWidget::showEvent(QShowEvent *e)
{
    if(!isInitedStyle)
        initStylePropoerty();
    isInitedStyle = true;
}

void BaseTimeDealWidget::setData(const QVector<HZData::HZTick> &ticks)
{
    int index = 0;
    for(int i=0;i<m_tableViews.size();i++){
        m_tableViews[i]->setData(ticks.mid(index,m_tableViews[i]->maxRowCount()));
        index+=m_tableViews[i]->maxRowCount();
    }
}

void BaseTimeDealWidget::clearData()
{
    m_zhubi.totalCount = 0;
    m_zhubi.ticks.clear();

    for(int i=0;i<m_tableViews.size();i++)
        m_tableViews[i]->clearData();
}

void BaseTimeDealWidget::setStockCode(const string &stockcode,const QString &stockName)
{
    if(m_titleWg)
        m_titleWg->setStockName(stockName);

    m_stockCode = stockcode;
}

void BaseTimeDealWidget::afterActive()
{
    m_isActived = true;
    m_isInitialed = false;
    if(!m_stockCode.empty())
        init();
}

void BaseTimeDealWidget::afterDeactive()
{
    cancelSub();
    m_isInitialed = false;
    m_isActived = false;
    clearData();
}

void BaseTimeDealWidget::addTableView(TimeDealTableView *tableView)
{
    m_tableViews.push_back(tableView);
    m_tableViewLayout->addWidget(tableView);
}

void BaseTimeDealWidget::setShowVerticalScrollBar(const bool &show)
{
    m_scrollBar->setHidden(!show);
}

void BaseTimeDealWidget::setTitleWg(TimeDealTitleWg *wg)
{
    if(m_titleWg){
        mainLayout->removeWidget(m_titleWg);
        delete m_titleWg;
    }
    m_titleWg = wg;
    mainLayout->insertWidget(0,m_titleWg);
}

void BaseTimeDealWidget::setShowTitle(const bool &show)
{
    if(m_titleWg)
        m_titleWg->setHidden(!show);
}



int   BaseTimeDealWidget::currentMaxRowCount() const
{
    int rowCount = 0;
    foreach(TimeDealTableView *tableViews,m_tableViews)
        rowCount += tableViews->maxRowCount();
    return rowCount;
}

int  BaseTimeDealWidget::currentRowCount() const
{
    int rowCount = 0;
    foreach(TimeDealTableView *tableViews,m_tableViews)
        rowCount += tableViews->rowCount();
    return rowCount;
}


void BaseTimeDealWidget::cancelSub()
{
    if(m_subReqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_subReqId);
        m_subReqId= INVALID_REQID;
    }
    if(m_reqRecrd.reqId != INVALID_REQID){
        DataController::instance()->cancelSub(m_reqRecrd.reqId);
        m_reqRecrd.reqId= INVALID_REQID;
    }
}


void BaseTimeDealWidget::initWg()
{
    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    m_tableViewLayout = new QHBoxLayout;
    m_tableViewLayout->setSpacing(0);
    m_tableViewLayout->setContentsMargins(0,0,0,0);

    contentLayout->addLayout(m_tableViewLayout);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);

    m_scrollBar = new QScrollBar(Qt::Vertical,this);
    contentLayout->addWidget(m_scrollBar);

    mainLayout->addLayout(contentLayout);
    this->setLayout(mainLayout);
}


///TimeDealTitleWg----
TimeDealTitleWg::TimeDealTitleWg(QWidget *parent )
    :BaseWidget(parent)
{
    QLabel *leftLabel = new QLabel(this);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *legendLabel = new QLabel(this);
    legendLabel->setObjectName("legendLabel");
    legendLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    legendLabel->setText(QStringLiteral("青色和紫色的现手分别表示成交额大于20万的内外盘分时成交"));

    mainLayout->setContentsMargins(16,2,16,2);
    mainLayout->setSpacing(1);
    mainLayout->addWidget(leftLabel,1);
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(legendLabel,1);
    this->setLayout(mainLayout);
}

void TimeDealTitleWg::setStockName(const QString &stockName)
{
    m_stockName = stockName;

    m_titleLabel->setText(m_stockName+QStringLiteral("分时成交"));
}

