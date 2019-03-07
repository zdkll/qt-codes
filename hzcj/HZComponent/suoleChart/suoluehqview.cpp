#include "suoluehqview.h"

#include <QGridLayout>
#include <QLabel>

#include "hzline.h"

SuolueHqView::SuolueHqView(SuolueInfo info,QWidget *parent)
    :SuolueBase(info,parent)
{

}

void SuolueHqView::afterActive()
{
    m_RiseFallWg->afterActive();
}

void SuolueHqView::afterDeactive()
{
    m_RiseFallWg->afterDeactive();
}

QWidget *SuolueHqView::createContentWnd()
{
    m_RiseFallWg = new SLRiseFallWidget(this);
    connect(m_RiseFallWg,&SLRiseFallWidget::currentStockChanged,this,&SuolueHqView::currentStockChanged);
    return m_RiseFallWg;
}

void SuolueHqView::onFocusOut()
{
  m_RiseFallWg->onFocusOut();
}


SuolueTitleBase *SuolueHqView::createTitleWnd()
{
    return new HqSuolueTitle(m_slInfo);
}



HqSuolueTitle::HqSuolueTitle(SuolueInfo info, QWidget *parent, QString instName):
    SuolueTitleBase(info,parent,instName)
{

}

void HqSuolueTitle::createWg()
{
    if(m_slInfo.itemList.size() >1)
    {
        QLabel* leftLab = new QLabel;
        leftLab->setObjectName("leftLable");
        leftLab->setText(m_slInfo.itemList[0].name);
        QLabel* rightLab = new QLabel;
        rightLab->setText(m_slInfo.itemList[1].name);
        leftLab->setFixedHeight(20);
        rightLab->setFixedHeight(20);

        HZLine* pLine = new HZLine(Qt::Vertical);
        QHBoxLayout* pLayout = new QHBoxLayout;
        pLayout->setMargin(0);
        pLayout->setSpacing(0);
        pLayout->addWidget(leftLab);
        pLayout->addWidget(pLine);
        pLayout->addWidget(rightLab);
        this->setLayout(pLayout);
    }
}
