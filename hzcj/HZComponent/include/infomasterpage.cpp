#include "infomasterpage.h"
#include <QHBoxLayout>

InfoMasterPage::InfoMasterPage(QWidget *parent, QString instName)
    :BasePage(parent,instName)
    ,mInfoMarster(new InfoMaster)
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addWidget(mInfoMarster);
    setLayout(hLayout);
}

void InfoMasterPage::afterActive()
{
    mInfoMarster->afterActive();
}

void InfoMasterPage::afterDeactive()
{
    mInfoMarster->afterDeactive();
}

void InfoMasterPage::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    mInfoMarster->recvParam(param,bCurrPage);
}
