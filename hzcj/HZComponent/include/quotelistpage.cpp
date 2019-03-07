#include "quotelistpage.h"
#include <QHBoxLayout>
#include <QMap>
#include <QMessageBox>
#include "pubmsgdef.h"
#include "quotelistpagemainwnd.h"
#include "quotepanelwnd.h"

QuoteListPage::QuoteListPage(QWidget *parent, QString instName)
    :BasePage(parent,instName),m_leftWnd(nullptr)
{
    createWg();
}

void QuoteListPage::afterActive()
{
    if(m_leftWnd)
        m_leftWnd->afterActive();
}

void QuoteListPage::afterDeactive()
{
    if(m_leftWnd)
        m_leftWnd->afterDeactive();
    m_rightWnd->afterDeactive();
}

void QuoteListPage::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    m_leftWnd->recvParam(param,bCurrPage);
}

void QuoteListPage::slotClickObjItem(const QString &obj)
{
    qDebug() << "QuoteListPage::slotClickObjItem" << obj;
    m_rightWnd->slotsObjChange(obj);
}

void QuoteListPage::slotDbClickObjItem(const QString &obj)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    msg.emplace(STOCK_ID,obj);    //股票和板块ID 如果是板块跳转，这个字段也要求赋值
    sendMessage(ROOT_FRAME,msg,true);
}

void QuoteListPage::createWg()
{
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(createLeftWnd());
    pLayout->setSpacing(1);
    pLayout->addWidget(createRightWnd());
    this->setLayout(pLayout);
}

QWidget *QuoteListPage::createLeftWnd()
{
    m_leftWnd = new QuoteListPageMainWnd;
    connect(m_leftWnd,SIGNAL(signalsClickObjItem(const QString&)),
            this,SLOT(slotClickObjItem(const QString&)));
    connect(m_leftWnd,SIGNAL(signalsDbClickObjItem(const QString&)),
            this,SLOT(slotDbClickObjItem(const QString&)));
    return m_leftWnd;
}

QWidget *QuoteListPage::createRightWnd()
{
   m_rightWnd= new QuoteListPanelWnd;
   m_leftWnd->setRightWnd(m_rightWnd);
   m_rightWnd->setFixedWidth(350);
   m_rightWnd->createWg();
   return m_rightWnd;
}

