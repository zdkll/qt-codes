#include "navigatorbar.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <assert.h>

#include "subtab.h"

NavigatorBar::NavigatorBar(QWidget *parent,QString instName):
    BaseWidget(parent,instName)
{
    createWg();
}

void NavigatorBar::createWg()
{
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(2);

    this->setLayout(m_mainLayout);
}

void  NavigatorBar::addCSubTab(CSubTab *csubTab)
{
    m_mainLayout->addWidget(csubTab);
    int mId  =  m_csubTabs.size();
    m_csubTabs.insert(mId,csubTab);
    connect(csubTab,SIGNAL(currentButtonChanged(id)),this,SLOT(tabCurrentBtnChanged(int)));
}

 CSubTab *NavigatorBar::addCSubTab(CSubTab::CSubTabType subTabType, QString instName)
{
    CSubTab *csubTab = new CSubTab(this,instName);
    csubTab->setObjectName(instName);
    csubTab->setCSubTabType(subTabType);
    m_mainLayout->addWidget(csubTab);
    int mId =   m_csubTabs.size();
    m_csubTabs.insert(mId,csubTab);

    connect(csubTab,SIGNAL(currentButtonChanged(int)),this,SLOT(tabCurrentBtnChanged(int)));

    return csubTab;
}

void NavigatorBar::addButtonToTab(const int &id,const QString &text)
{
    CSubTab *csubTab = m_csubTabs.value(id);
    if(csubTab){
        csubTab->addButton(text);
    }
}

void NavigatorBar::setSubTabButtons(const int &id,const QStringList &textList)
{
    CSubTab *csubTab = m_csubTabs.value(id);
    if(csubTab) {
        csubTab->setButtons(textList);
    }
}
void NavigatorBar::tabCurrentBtnChanged(const int &btnId)
{
    assert(m_csubTabs.size()>0);

    CSubTab *sendSubTab = static_cast<CSubTab *>(sender());

    //主导航栏
    int priButton = m_csubTabs.value(0)->currentButton();
    int secButon= -1;

    if(m_csubTabs.size()>1)
        secButon = m_csubTabs.value(1)->currentButton();

    if(sendSubTab == m_csubTabs.value(0))
        emit subTabSectionChanged(0,btnId);
    else if(sendSubTab == m_csubTabs.value(1))
        emit subTabSectionChanged(1,btnId);

    emit navigatorSectionChanged(priButton, secButon);
}

void NavigatorBar::OnClickBack()
{
    MsgBack();
}
