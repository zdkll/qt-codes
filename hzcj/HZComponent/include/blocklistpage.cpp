#include "blocklistpage.h"
#include <QVBoxLayout>

#include "blocklistmainwnd.h"

BlockListPage::BlockListPage(QWidget *parent, QString instName)
    :BasePage(parent,instName)
{
    createWg();
}

void BlockListPage::afterActive()
{
    m_pMainWnd->afterActive();
}

void BlockListPage::afterDeactive()
{
    m_pMainWnd->afterDeactive();
}

void BlockListPage::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    m_pMainWnd->recvParam(param,bCurrPage);
}

void BlockListPage::createWg()
{
    m_pMainWnd = new BlockListMainWnd;
    m_pMainWnd->createWg();

    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(m_pMainWnd);
    this->setLayout(pLayout);
}

