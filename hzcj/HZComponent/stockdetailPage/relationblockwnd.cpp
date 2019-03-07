#include "relationblockwnd.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "sltableview.h"
#include "stockdynaview.h"
#include "risefallflatbasewidget.h"
#include "pubmsgdef.h"
RelationBlockWnd::RelationBlockWnd(QWidget *parent, QString instName):
    BaseWidget(parent,instName),
    m_leftBkListView(nullptr),
    m_rightBkStocksView(nullptr)
{

}

void RelationBlockWnd::createWg()
{
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);


    QHBoxLayout* leftLayout =  new QHBoxLayout;
    leftLayout->setSpacing(0);
    leftLayout->setMargin(0);
    QWidget* pWnd = createLeftBkListWnd();;
    leftLayout->addWidget(pWnd,2);
    leftLayout->setSpacing(1);

    pWnd = createMidWnd();
    leftLayout->addWidget(pWnd,1);


    pLayout->addLayout(leftLayout,1);
    pLayout->setSpacing(1);
    pLayout->addWidget(createRightBkStocksWnd(),1);
    this->setLayout(pLayout);

}

void RelationBlockWnd::afterActive()
{
    m_leftBkListView->setStockCode(m_stockCode);
    m_leftBkListView->afterActive();
}

void RelationBlockWnd::afterDeactive()
{
    m_leftBkListView->afterDeactive();
    m_rightBkStocksView->afterDeactive();
}

void RelationBlockWnd::slotRightViewDbClicked(const QString &obj)
{
    emit signalDbClickObjChange(obj);
}

void RelationBlockWnd::slotLeftViewDbClicked(const QString& obj)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,obj);
    sendMessage(ROOT_FRAME,params);
}

void RelationBlockWnd::slotsItemSelected(const int &filterFlag, const QString &stockCode)
{
    m_rightBkStocksView->afterDeactive();
    m_rightBkStocksView->setFilterInfo(filterFlag,stockCode);
    m_rightBkStocksView->afterActive();
}

void RelationBlockWnd::OnStockCodeChange()
{
    if(m_leftBkListView)
        m_leftBkListView->setStockCode(m_stockCode);
}

QWidget *RelationBlockWnd::createLeftBkListWnd()
{
    m_leftBkListView = new RelationBlockListView(this);
    connect(m_leftBkListView,SIGNAL(itemEntered(const QString&)),
            this,SLOT(slotLeftViewDbClicked(const QString&)));
    return m_leftBkListView;
}

QWidget *RelationBlockWnd::createMidWnd()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    QLabel* pLabel = new QLabel;
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setText(QStringLiteral("涨平跌"));
    pLabel->setObjectName("RelationBlockWndLabel");
    pLabel->setFixedHeight(23);
    pLayout->addWidget(pLabel);
    pLayout->addSpacing(1);

    m_rffWnd = new RiseFallFlatRectChartWnd();
    connect(m_leftBkListView,&RelationBlockListView::signalsItemRiseFallFlatChanged,
            m_rffWnd,&RiseFallFlatBaseWidget::slotsItemRiseFallFlatChanged);
    pLayout->addWidget(m_rffWnd);
    //pLayout->addStretch();
    QWidget* pWnd = new QWidget(this);
    pWnd->setLayout(pLayout);
    return pWnd;




//    m_rffWnd = new RiseFallFlatRectChartWnd(this);
//    connect(m_leftBkListView,&RelationBlockListView::signalsItemRiseFallFlatChanged,
//            m_rffWnd,&RiseFallFlatBaseWidget::slotsItemRiseFallFlatChanged);
//    return m_rffWnd;
}

QWidget *RelationBlockWnd::createRightBkStocksWnd()
{
    m_rightBkStocksView = new  StockDynaView(StockDynaView::BKStockDyna,this);
    m_rightBkStocksView->setObjectName("relationBkChildStockView");
    connect(m_leftBkListView,&RelationBlockListView::signalsItemSelected,
            this,&RelationBlockWnd::slotsItemSelected);
    connect(m_rightBkStocksView,SIGNAL(itemEntered(const QString&)),
            this,SLOT(slotRightViewDbClicked(const QString&)));
    return m_rightBkStocksView;

    //QWidget* pWnd = new QWidget;
    //return pWnd;
}
