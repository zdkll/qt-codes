#include "stockbasedetailpage.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QPushButton>

#include "localstocksource.h"
#include "stockdetailklineminwnd.h"
#include "quotepanelwnd.h"
#include "quotepagelayout.h"
#include "stockdetailbasewnd.h"
#include "pubmsgdef.h"
#include "httpdata.h"
#include "blockdetailinfownd.h"
#include "blockdetaillistwnd.h"
#include "indexdetaillistwnd.h"
#include "stockdetailinfownd.h"
#include "stockdetaillistwnd.h"
#include "subtab.h"
#include "hzline.h"
#include "hzftenmarster.h"
#include "stockadddialog.h"

#define LEFT_WND 0
#define BOTTOM_WND 1

StockBaseDetailPage::StockBaseDetailPage(QWidget *parent, QString instName)
    :BasePage(parent,instName)
{
    if(m_stockCode.isEmpty())
        m_stockCode = "sh600004";
    m_pageLayout = new QuotePageLayout(this,this);
    m_midTopWnd = new StockDetailKlineMinWnd(this);
    m_rightPanel = new QuoteDetailPanelWnd(this);
    connect(m_midTopWnd,&StockDetailKlineMinWnd::signalWheelChangeStock,
            this,&StockBaseDetailPage::slotsWheelAndUpDownPageChangeStock);
}

void StockBaseDetailPage::afterActive()
{
//    if(m_leftWnd && m_leftWnd->isVisible())
//         m_leftWnd->afterActive();
//    if(m_midTopWnd && m_midTopWnd->isVisible())
//        m_midTopWnd->afterActive();
//    if(m_midBottomWnd && m_midBottomWnd->isVisible())
//        m_midBottomWnd->afterActive();
//    if(m_rightPanel && m_rightPanel->isVisible())
//        m_rightPanel->afterActive();

    if(m_leftWndStackWnd && m_leftWndStackWnd->isVisible())
         m_leftWndStackWnd->afterActive();
    if(m_midTopWnd && m_midTopWnd->isVisible())
        m_midTopWnd->afterActive();
    if(m_BottomWndStackWnd && m_BottomWndStackWnd->isVisible())
        m_BottomWndStackWnd->afterActive();
    if(m_rightPanel && m_rightPanel->isVisible())
        m_rightPanel->afterActive();
}

void StockBaseDetailPage::afterDeactive()
{
//    if(m_leftWnd && m_leftWnd->isVisible())
//         m_leftWnd->afterDeactive();
//    if(m_midTopWnd && m_midTopWnd->isVisible())
//        m_midTopWnd->afterDeactive();
//    if(m_midBottomWnd && m_midBottomWnd->isVisible())
//        m_midBottomWnd->afterDeactive();
//    if(m_rightPanel && m_rightPanel->isVisible())
//        m_rightPanel->afterDeactive();

    if(m_leftWndStackWnd && m_leftWndStackWnd->isVisible())
         m_leftWndStackWnd->afterDeactive();
    if(m_midTopWnd && m_midTopWnd->isVisible())
        m_midTopWnd->afterDeactive();
    if(m_BottomWndStackWnd && m_BottomWndStackWnd->isVisible())
        m_BottomWndStackWnd->afterDeactive();
    if(m_rightPanel && m_rightPanel->isVisible())
        m_rightPanel->afterDeactive();
}

void StockBaseDetailPage::slotsLeftObjChange(const QString &obj)
{
    m_stockCode = obj;
    LocalStockSource::globalHistInstance()->addStock(obj);
    changeBottomInfoByStock(obj,true);
    proceF10Visual(obj);
    m_midTopWnd->slotsObjChange(obj);
    m_rightPanel->slotsObjChange(obj);
}

void StockBaseDetailPage::slotsBottomObjChange(const QString &obj)
{
    m_stockCode = obj;
    LocalStockSource::globalHistInstance()->addStock(obj);
//    m_midTopWnd->slotsObjChange(obj);
//    m_rightPanel->slotsObjChange(obj);
    changeLeftListWndByStock(obj,true);
    proceF10Visual(obj);
}

void StockBaseDetailPage::slotsTablSwitch(int id)
{
    if(m_BottomWndStackWnd->isHidden())
    {
        m_titleHideBt->setChecked(false);
        m_BottomWndStackWnd->show();
    }
    m_midBottomWnd->slotsSwitchWnd(id);
}

void StockBaseDetailPage::slotsHideLeftWnd(bool bHide)
{
    if(m_leftWndStackWnd->isHidden() && bHide == false)
    {
        m_leftWndStackWnd->show();
        m_leftWndStackWnd->afterActive();
    }else if(m_leftWndStackWnd->isVisible() && bHide)
    {
        m_leftWndStackWnd->hide();
        m_leftWndStackWnd->afterDeactive();
    }
}

void StockBaseDetailPage::slotsHideMidBottomWnd(bool bHide)
{
    if(m_BottomWndStackWnd->isHidden() && bHide == false)
    {
        m_BottomWndStackWnd->show();
        m_BottomWndStackWnd->afterActive();
    }else if(m_BottomWndStackWnd->isVisible() && bHide)
    {
        m_BottomWndStackWnd->hide();
        m_BottomWndStackWnd->afterDeactive();
    }
}

void StockBaseDetailPage::slotsHideRightWnd(bool bHide)
{
    if(m_rightPanel->isHidden() && bHide == false)
    {
        m_rightPanel->show();
        m_rightPanel->afterActive();
    }else if(m_rightPanel->isVisible() && bHide)
    {
        m_rightPanel->hide();
        m_rightPanel->afterDeactive();
    }
}

void StockBaseDetailPage::slotsWheelAndUpDownPageChangeStock(const QString &obj, bool bUp, int step)
{
    qDebug() << "stock: " << obj << " bUp" << bUp << " step: " << step;
    m_leftWnd->slotsSwitchUpDownStock(bUp,step);
}

void StockBaseDetailPage::slotsF10Click()
{
    qDebug() << "StockBaseDetailPage::slotsF10Click";
    HZFTenMarster fTenDlg(m_stockCode);
    fTenDlg.exec();

//    fTenDlg.showExec(m_stockCode);
}

StockBaseDetailListWnd *StockBaseDetailPage::createLeftListWnd()
{
    return nullptr;
}

StockBaseDetailInfoWnd *StockBaseDetailPage::createBottomWnd()
{
    return nullptr;
}

void StockBaseDetailPage::OnStockCodeChange()
{
    LocalStockSource::globalHistInstance()->addStock(m_stockCode);
    m_leftWnd->setStockCode(m_stockCode);
    m_midTopWnd->setStockCode(m_stockCode);
    m_midBottomWnd->setStockCode(m_stockCode);
    m_rightPanel->setStockCode(m_stockCode);
}

void StockBaseDetailPage::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    auto it  = param.cmd.find(HZ_CHILD_CMD);
    if(it != param.cmd.end() && it->second == SWITCH_KLINE_MIN && bCurrPage)
    {
        m_midTopWnd->switchKlineAndMin();
    }else
    {
        QString stock = param.stock;
        if(stock.isEmpty())
            stock = m_stockCode;
        LocalStockSource::globalHistInstance()->addStock(stock);
        if(param.period != TabKlineMinUnknown && param.period != -1)
            m_midTopWnd->setCurrType(param.period);
        setStockCode(stock);
        auto stockType = param.cmd.find(STOCK_DETAIL_TYPE);
        if(stockType == param.cmd.end())
        {
            changeLeftListWndByStock(stock,bCurrPage);
        }
        else
        {
            int id = stockType->second.toInt();
            m_leftWndStackWnd->clearCurrWidget();
            m_leftWnd = m_pageIdMapToChildWnd[id].leftWnd;
            m_leftWnd->slotsObjChange(stock); //通知窗口切换到最近浏览界面
            m_leftWndStackWnd->setCurrWidget(m_pageIdMapToChildWnd[id].index,bCurrPage);
        }
        proceF10Visual(stock);
        if(bCurrPage)
        {
            m_midTopWnd->slotsObjChange(stock);
            m_rightPanel->slotsObjChange(stock);
        }
    }
}

void StockBaseDetailPage::onMessage(const QString &senderPath, const MessageParams &params)
{
    auto it = params.find(HZ_CMD);
    if(it != params.end())
    {
        if(it->second == SWITCH_DR_FB && this->isVisible())//显示短线异动， 已经显示则不要重复显示
        {
            if(m_midTopWnd)
            {
                m_midTopWnd->switchDrType();
            }
        }
    }
}

void StockBaseDetailPage::onNetIoReady(bool ready)
{
    if(ready)
    {
        if(m_midTopWnd)
             m_midTopWnd->onNetIoReady(ready);
        this->afterActive();
    }
}

void StockBaseDetailPage::createWg()
{
    m_bottomTitleBar = createTitle();
    createListAndInfoWnd();

    m_midTopWnd->createWg();
    m_rightPanel->createWg();

    //窗口隐藏显示事件
    connect(m_midTopWnd,SIGNAL(signalHideLeftWnd(bool)),this,SLOT(slotsHideLeftWnd(bool)));
    connect(m_midTopWnd,SIGNAL(signalHideRightWnd(bool)),this,SLOT(slotsHideRightWnd(bool)));


    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    m_leftWndStackWnd->setFixedWidth(QuoteLeftWndWidth);
    pLayout->addWidget(m_leftWndStackWnd);

    QVBoxLayout* pMidLayout = new QVBoxLayout;
    pMidLayout->setSpacing(0);
    pMidLayout->setMargin(0);
    pMidLayout->addWidget(m_midTopWnd);
    m_midTopWnd->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    pMidLayout->addSpacing(1);
    pMidLayout->addWidget(m_bottomTitleBar);
    pMidLayout->addSpacing(1);
    m_BottomWndStackWnd->setFixedHeight(QuoteBottomWndHight);
//    m_BottomWndStackWnd->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    pMidLayout->addWidget(m_BottomWndStackWnd);
    QWidget* pMid = new QWidget;
    pMid->setLayout(pMidLayout);
    pLayout->addSpacing(1);
    pLayout->addWidget(pMid);
    pLayout->addSpacing(1);

    m_rightPanel->setFixedWidth(QuoteRightWndWidth);
    pLayout->addWidget(m_rightPanel);
    this->setLayout(pLayout);
}

QWidget *StockBaseDetailPage::createTitle()
{
    //左隐藏和显示按钮
    QToolButton* pButton = new QToolButton(this);
    m_titleHideBt = pButton;
    pButton->setCheckable(true);
    pButton->setObjectName("bottomExpandButton");
    connect(pButton,SIGNAL(clicked(bool)),this,SLOT(slotsHideMidBottomWnd(bool)));

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pButton->setFixedWidth(26);
    pLayout->addWidget(pButton);
    HZLine* pLine = new HZLine(Qt::Vertical);
    pLine->setFixedSize(1,26);
    pLayout->addWidget(pLine);

    //tab
    m_table = new CSubTab(this);
    m_table->setStretchLastType(CSubTab::Empty_Strecth);
    m_table->setObjectName("seltTab");
    pLayout->addWidget(m_table);
    connect(m_table,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsTablSwitch(int)));

    //添加F10按钮
    m_f10Bt  = new QPushButton(this);
    m_f10Bt->setObjectName("bottomF10");
    m_f10Bt->setText(QStringLiteral("F10数据"));
    pLayout->addStretch();
    pLayout->addWidget(m_f10Bt);
    pLayout->addSpacing(10);
    connect(m_f10Bt,SIGNAL(clicked()),this,SLOT(slotsF10Click()));
    QWidget* pWnd = new QWidget;
    pWnd->setLayout(pLayout);
    pWnd->setObjectName("titleWnd");
    m_bottomTitleBar = pWnd;
    return pWnd;
}

void StockBaseDetailPage::createListAndInfoWnd()
{
    m_BottomWndStackWnd = new BaseStackWidget(this);
    m_leftWndStackWnd = new BaseStackWidget(this);
    //创建个股左侧和底部窗口
    DetailWndItem item;
    item.leftWnd = new StockDetailListWnd;
    item.infoBottomWnd = new StockDetailInfoWnd;
    item.index = 0;
    item.leftWnd->createWg();
    item.infoBottomWnd->createWg();
    m_pageIdMapToChildWnd[stock_detail_id] = item;
    m_leftWndStackWnd->addWidget(item.leftWnd,item.leftWnd);
    m_BottomWndStackWnd->addWidget(item.infoBottomWnd,item.infoBottomWnd);
    connect(item.leftWnd,SIGNAL(signalObjChange(const QString&)),this,SLOT(slotsLeftObjChange(const QString&)));
    connect(item.infoBottomWnd,SIGNAL(signalObjChange(const QString&)),this,SLOT(slotsBottomObjChange(const QString&)));

    //创建板块左侧和底部窗口
    item.leftWnd = new BlockDetaillistWnd;
    item.infoBottomWnd = new BlockDetailInfoWnd;
    item.index = 1;
    item.leftWnd->createWg();
    item.infoBottomWnd->createWg();
    m_pageIdMapToChildWnd[block_detail_id] = item;
    m_leftWndStackWnd->addWidget(item.leftWnd,item.leftWnd);
    m_BottomWndStackWnd->addWidget(item.infoBottomWnd,item.infoBottomWnd);
    connect(item.leftWnd,SIGNAL(signalObjChange(const QString&)),this,SLOT(slotsLeftObjChange(const QString&)));
    connect(item.infoBottomWnd,SIGNAL(signalObjChange(const QString&)),this,SLOT(slotsBottomObjChange(const QString&)));

    //创建指数左侧和底部窗口
    item.leftWnd = new IndexDetailListWnd;
    item.infoBottomWnd = new IndexDetailInfoWnd;
    item.index = 2;
    item.leftWnd->createWg();
    item.infoBottomWnd->createWg();
    m_pageIdMapToChildWnd[index_detail_id] = item;
    m_leftWndStackWnd->addWidget(item.leftWnd,item.leftWnd);
    m_BottomWndStackWnd->addWidget(item.infoBottomWnd,item.infoBottomWnd);
    connect(item.leftWnd,SIGNAL(signalObjChange(const QString&)),this,SLOT(slotsLeftObjChange(const QString&)));
    connect(item.infoBottomWnd,SIGNAL(signalObjChange(const QString&)),this,SLOT(slotsBottomObjChange(const QString&)));

   //第一次默认值设置
    m_leftWnd = m_pageIdMapToChildWnd[stock_detail_id].leftWnd;
    m_midBottomWnd = m_pageIdMapToChildWnd[stock_detail_id].infoBottomWnd;
    changeTabItemByWnd(m_midBottomWnd);
    m_BottomWndStackWnd->layoutStackWnd();
    m_leftWndStackWnd->layoutStackWnd();
    m_leftWndStackWnd->setCurrWidget(0,false);
    m_BottomWndStackWnd->setCurrWidget(0,false);
}

bool StockBaseDetailPage::changeLeftListWndByStock(const QString &stock,bool bActive)
{
    int id  = stock_detail_id;
    if(Httpdata::instance()->isHasObjFromFilter(stock,HZData::BlockWhole))
    {
        id = block_detail_id;
    }else if(Httpdata::instance()->isHasObjFromFilter(stock,HZData::IndexInner))
    {
        id = index_detail_id;
    }
    if(m_pageIdMapToChildWnd[id].index != m_leftWndStackWnd->currWidgetIndex())
    {
        m_leftWndStackWnd->clearCurrWidget();
        m_leftWnd = m_pageIdMapToChildWnd[id].leftWnd;
        m_leftWnd->slotsObjChange(stock); //通知窗口切换到最近浏览界面
        m_leftWndStackWnd->setCurrWidget(m_pageIdMapToChildWnd[id].index,bActive);
        return true;
    }else
    {
        m_leftWnd->slotsObjChange(stock);
        if(bActive)
        {
            m_leftWndStackWnd->afterDeactive();
            m_leftWndStackWnd->afterActive();
        }
    }
    return false;
}

bool StockBaseDetailPage::changeBottomInfoByStock(const QString &stock,bool bActive)
{
    int id  = stock_detail_id;
    if(Httpdata::instance()->isHasObjFromFilter(stock,HZData::BlockWhole))
    {
        id = block_detail_id;
    }else if(Httpdata::instance()->isHasObjFromFilter(stock,HZData::IndexInner))
    {
        id = index_detail_id;
    }
    //if(m_pageIdMapToChildWnd[id].index != m_BottomWndStackWnd->currWidgetIndex())
    //    {
    //        m_BottomWndStackWnd->clearCurrWidget();
    //        m_midBottomWnd = m_pageIdMapToChildWnd[id].infoBottomWnd;
    //        changeTabItemByWnd(m_midBottomWnd);
    //        m_midBottomWnd->slotsObjChange(stock);
    //        m_BottomWndStackWnd->setCurrWidget(m_pageIdMapToChildWnd[id].index,bActive);
    //        return true;
    //    }
    m_BottomWndStackWnd->clearCurrWidget();
    m_midBottomWnd = m_pageIdMapToChildWnd[id].infoBottomWnd;
    changeTabItemByWnd(m_midBottomWnd);
    m_midBottomWnd->slotsObjChange(stock);
    m_BottomWndStackWnd->setCurrWidget(m_pageIdMapToChildWnd[id].index,bActive);
    return true;
}

void StockBaseDetailPage::changeTabItemByWnd(StockBaseDetailInfoWnd *midBottomWnd)
{
    m_table->clear();
    QVector<StockDetailSubItem> list;
    m_midBottomWnd->getStockDetailSubItemList(list);
    int currId = m_midBottomWnd->getCurrTabId();
    for(int i=0;i < list.size();i++)
    {
        m_table->addButton(list[i].name,list[i].id);
        m_table->addLine(1,12);
    }
    m_table->setCurrentButton(currId);
}

void StockBaseDetailPage::proceF10Visual(const QString &stock)
{
    bool bStock =  Httpdata::instance()->isHasObjFromFilter(stock,HZData::MarketAll);
    if(bStock && m_f10Bt->isHidden())
    {
        m_f10Bt->show();
    }else if(bStock == false && m_f10Bt->isVisible())
    {
        m_f10Bt->hide();
    }
}

void StockBaseDetailPage::keyPressEvent(QKeyEvent *event)
{
    BasePage::keyPressEvent(event);
    if(Qt::Key_F10 == event->key() && m_f10Bt->isVisible())
    {
        slotsF10Click();
    }else if(Qt::Key_Insert == event->key())
    {
        StockAddDialog dlg;
        dlg.exec();
    }else if(Qt::Key_F1 == event->key())
    {
        MessageParams params;
        params.emplace(HZ_CMD, SHOW_ALL_TIME_DEAL_DLG);
        params.emplace(STOCK_ID,m_stockCode);
        sendMessage(ROOT_FRAME,params);
    }
}
