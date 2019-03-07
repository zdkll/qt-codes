#include "datacenterpage.h"
#include <QBoxLayout>
#include "datacenterpagemainwnd.h"

DataCenterPage::DataCenterPage(QWidget *parent, QString instName)
    :BasePage(parent,instName)
{
    m_mainChildWnd =  new DataCenterPageMainWnd;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_mainChildWnd);
    setLayout(layout);
}

void DataCenterPage::afterActive()
{
    m_mainChildWnd->afterActive();
}

void DataCenterPage::afterDeactive()
{
    m_mainChildWnd->afterDeactive();
}

void DataCenterPage::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    m_mainChildWnd->recvParam(param,bCurrPage);
}

void DataCenterPage::onNetIoReady(bool ready)
{
    m_mainChildWnd->onNetIoReady(ready);
}
