#include "mininavebar.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QDebug>
MiniNaveBar::MiniNaveBar(BaseWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void MiniNaveBar::createWg()
{
    QButtonGroup *pBtnGroup = new QButtonGroup(this);
    connect(pBtnGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [this](int nIndex){emit clicked(nIndex); qDebug() << nIndex;});
    QPushButton *pSelfStockBtn = new QPushButton(QStringLiteral("自选股"));
    QPushButton *pMarketBtn = new QPushButton(QStringLiteral("板块异动"));
    pBtnGroup->addButton(pSelfStockBtn, 0);
    pBtnGroup->addButton(pMarketBtn, 1);
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setSpacing(1);
    pLayout->setMargin(0);
    pLayout->addWidget(pSelfStockBtn);
    pLayout->addWidget(pMarketBtn);

    pSelfStockBtn->setCheckable(true);
    pMarketBtn->setCheckable(true);
    pSelfStockBtn->setChecked(true);
}
