#include "slrisefallwidget.h"

#include <QSignalMapper>
#include <QBoxLayout>
#include "hzline.h"
#include "pubmsgdef.h"

SLRiseFallWidget::SLRiseFallWidget(QWidget *parent,QString instName)
    :BaseWidget(parent,instName),m_signalMapper(new QSignalMapper(this))
    ,m_currenIdx(-1)
{
    createWg();
}

void SLRiseFallWidget::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    //沪A第一列
    mainLayout->addLayout(createColumnLayout(HZData::MarketSH,0));

    mainLayout->addWidget(new HZLine(Qt::Vertical));

    //深A第二列
    mainLayout->addLayout(createColumnLayout(HZData::MarketSZ,1));

    for(int i=0;i<m_tableViews.size();i++){
        m_signalMapper->setMapping(m_tableViews[i],i);
        connect(m_tableViews[i],SIGNAL(itemSelected(QString)),m_signalMapper,SLOT(map()));
        connect(m_tableViews[i],SIGNAL(itemEntered(QString)),this,SLOT(slotItemSeleced(const QString&)));
    }
    connect(m_signalMapper,SIGNAL(mapped(int)),this,SLOT(slotCurrentViewChanged(int )));
    this->setLayout(mainLayout);
}

void SLRiseFallWidget::slotCurrentViewChanged(int idx)
{
    emit currentStockChanged();
    if(m_currenIdx>=0 && m_currenIdx != idx){
        m_tableViews[m_currenIdx]->onFocusOut();
        //qDebug()<<m_currenIdx<<" onFocusOut";
    }
    m_currenIdx = idx;
}

void SLRiseFallWidget::slotItemSeleced(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}

SLTableView *SLRiseFallWidget::createHSTableView(int flag,int sortColumn,Qt::SortOrder sortOrder)
{
    SLTableView *tableView = new SLTableView;
    tableView->setFilterFlag(flag);
    tableView->sortByColumn(sortColumn, sortOrder);
    return tableView;
}

QLayout *SLRiseFallWidget::createLineLayout()
{
    QHBoxLayout *hlineLayout = new QHBoxLayout;
    hlineLayout->setSpacing(0);
    hlineLayout->setContentsMargins(10,0,10,0);
    hlineLayout->addWidget(new HZLine(Qt::Horizontal));

    return hlineLayout;
}

QLayout *SLRiseFallWidget::createColumnLayout(int flag,int column)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    if(column == 0)
        vLayout->setContentsMargins(9,6,7,0);
    else
        vLayout->setContentsMargins(7,6,9,0);
    vLayout->setSpacing(0);

    SLTableView *tableview_first = createHSTableView(flag,2,Qt::DescendingOrder);
    m_tableViews.append(tableview_first);
    vLayout->addWidget(tableview_first,1);

    //水平分割线
    vLayout->addLayout(createLineLayout());

    SLTableView *tableview_second = createHSTableView(flag,2,Qt::AscendingOrder);
    m_tableViews.append(tableview_second);
    vLayout->addWidget(tableview_second,1);

    return vLayout;
}

void SLRiseFallWidget::afterActive()
{
    for(int i=0;i<m_tableViews.size();i++)
        m_tableViews[i]->afterActive();
}

void SLRiseFallWidget::afterDeactive()
{
    for(int i=0;i<m_tableViews.size();i++)
        m_tableViews[i]->afterDeactive();
}

void SLRiseFallWidget::onFocusOut()
{
    //qDebug()<<"SLRiseFallWidget focusOut";
    if(m_currenIdx>=0){
        m_tableViews[m_currenIdx]->onFocusOut();
    }
    m_focused = false;
    m_currenIdx = -1;
}

