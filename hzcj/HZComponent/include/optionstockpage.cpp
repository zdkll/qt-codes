#include "optionstockpage.h"
#include <QPainter>
#include <QBoxLayout>
#include <QMap>
#include "pubmsgdef.h"
#include "newswidget.h"
#include "hzinfoadapter.h"
#include "buysellfivewidget.h"
#include "buysellfiveview.h"
#include "buysellfivewidget.h"
#include "quotepanelwnd.h"
#include "optionmainwnd.h"

OptionStockPage::OptionStockPage(QWidget *parent, QString instName)
    :BasePage(parent,instName),
     m_rightWidget(new QuoteListPanelWnd(this)),
     m_leftWidget(new OptionMainWnd(this))
{
    createWg();
    //先默认赋值一个合约
    m_leftWidget->setStockCode("sh600004");
    m_rightWidget->setStockCode("sh600004");
}

void OptionStockPage::afterActive()
{
    if(m_rightWidget->isVisible())
        m_rightWidget->afterActive();
    if(m_leftWidget)
        m_leftWidget->afterActive();
}

void OptionStockPage::afterDeactive()
{
    if(m_rightWidget->isVisible())
        m_rightWidget->afterDeactive();
    if(m_leftWidget)
        m_leftWidget->afterDeactive();
}

void OptionStockPage::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    m_leftWidget->recvParam(param,bCurrPage);
}

void OptionStockPage::slotsSwitchObj(const QString &obj)
{
    m_rightWidget->setStockCode(obj);
    m_rightWidget->afterActive();
}

void OptionStockPage::slotsSwtichDetailPage(const QString &obj)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    msg.emplace(STOCK_ID,obj);    //股票和板块ID 如果是板块跳转，这个字段也要求赋值
    sendMessage(ROOT_FRAME,msg,true);
}

void OptionStockPage::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    m_leftWidget->createWg();
    mainLayout->addWidget(m_leftWidget);
    mainLayout->setSpacing(1);
    m_rightWidget->setFixedWidth(350);
    m_rightWidget->createWg();
    mainLayout->addWidget(m_rightWidget);
    m_leftWidget->setRightPnaleWnd(m_rightWidget);

    connect(m_leftWidget,SIGNAL(signalsClickObjItem(const QString&)),this,SLOT(slotsSwitchObj(const QString&)));
    connect(m_leftWidget,SIGNAL(signalsDbClickObjItem(const QString&)),this,SLOT(slotsSwtichDetailPage(const QString&)));
}

