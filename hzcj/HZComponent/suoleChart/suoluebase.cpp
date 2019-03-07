#include "suoluebase.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "subtab.h"

SuolueTitleBase::SuolueTitleBase(SuolueInfo info, QWidget *parent,QString instName):
    BaseWidget(parent,instName),
    m_slInfo(info)
{
}

void SuolueTitleBase::createWg()
{
    //导航栏
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->setSpacing(0);
    m_subTab = new CSubTab();
    m_subTab->setObjectName("roottab");
    m_subTab->setCSubTabType(CSubTab::Root_Tab);
    connect(m_subTab,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsTabCurrentBtnChanged(int)));
    for(int i=0;i < m_slInfo.itemList.size();i++)
    {
        m_idMap[m_slInfo.itemList[i].id] = m_slInfo.itemList[i];
        m_subTab->addButton(m_slInfo.itemList[i].name,m_slInfo.itemList[i].id);
    }
    m_subTab->setContentsMargins(1,0,1,1);
    m_subTab->setFixedHeight(20);
    m_subTab->setStretchLastType(CSubTab::Empty_Strecth);
    m_subTab->setCurrentButton(m_slInfo.defaultId);
    hlayout->addWidget(m_subTab);

    if(!(m_slInfo.attr & SuolueHideMoreBt))
    {
        QPushButton *btnMore = new QPushButton;
        btnMore->setObjectName("more");
        connect(btnMore, &QPushButton::clicked, this, &SuolueTitleBase::slotsBtClicked);
        hlayout->addWidget(btnMore);
    }
    this->setLayout(hlayout);
}

const QMap<int, SuolueItem> &SuolueTitleBase::getItemMap() const
{
    return m_idMap;
}

void SuolueTitleBase::slotsTabCurrentBtnChanged(int nId)
{
    emit signalTabCurrentBtnChanged(nId);
}

void SuolueTitleBase::slotsBtClicked()
{
    emit signalBtClicked();
}

SuolueBase::SuolueBase(SuolueInfo info, QWidget *parent):
    BaseWidget(parent),
    m_slInfo(info),
    m_title(nullptr)
{

}

SuolueBase::~SuolueBase()
{

}

void SuolueBase::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(1,1,1,1);
    pLayout->setSpacing(0);

    //导航栏
    m_title = createTitleWnd();
    if(m_title)
    {
        m_title->createWg();
        m_title->setFixedHeight(22);
        connect(m_title,&SuolueTitleBase::signalTabCurrentBtnChanged,this,&SuolueBase::slotsTabCurrentBtnChanged);
        connect(m_title, &SuolueTitleBase::signalBtClicked, this, &SuolueBase::slotsBtClicked);
        pLayout->addWidget(m_title);
    }
    //内容部分部件
    QWidget* pWnd = createContentWnd();
    if(pWnd)
    {
        pLayout->addWidget(pWnd);
    }
    this->setLayout(pLayout);

    //创建onCreate()
}

SuolueTitleBase *SuolueBase::createTitleWnd()
{
    SuolueTitleBase * pWnd = new SuolueTitleBase(m_slInfo);
    pWnd->setObjectName("normalTitle");
    return pWnd;
}

QWidget *SuolueBase::createContentWnd()
{
    return new QWidget();
}

void SuolueBase::onCreate()
{

}

void SuolueBase::onClickItem(const SuolueItem &item)
{
    QMessageBox::information(this, "test", item.name);
}

void SuolueBase::onClickMore()
{
    QMessageBox::information(this, "test", QStringLiteral("onClickMore"));
}

void SuolueBase::slotsTabCurrentBtnChanged(int nId)
{
    if(m_title)
    {
        const QMap<int,SuolueItem>& idmap =  m_title->getItemMap();
        auto it = idmap.find(nId);
        if(it != idmap.end())
        {
            onClickItem(it.value());
        }
    }
}

void SuolueBase::slotsBtClicked()
{
    onClickMore();
}
