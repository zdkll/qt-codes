#include "subtab.h"
#include "hzline.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QSpacerItem>

CSubTab::CSubTab(QWidget *parent,QString instName)
    : BaseWidget(parent,instName),m_tabType(Root_Tab)
    ,m_currentBtnIdx(-1)
    ,m_stretchLastType(None_Strecth)
{
    m_pButtonGroup = new QButtonGroup(this) ;

    createWg();
    connect(m_pButtonGroup,SIGNAL(buttonToggled(int ,bool)),this,SLOT(buttonToggled(int,bool)));
    connect(m_pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotsButtonClicked(int)));
}

CSubTab::~CSubTab()
{

}

void CSubTab::setCSubTabType(CSubTabType tabType)
{
    m_tabType = tabType;

    emit csubTypeChanged(m_tabType);
}

void  CSubTab::createWg()
{
    topHLayout = new QHBoxLayout;
    topHLayout->setSpacing(0);
    topHLayout->setContentsMargins(0,0,0,0);

    m_pMainLayout = new QHBoxLayout();
    m_pMainLayout->setContentsMargins(2, 0, 2, 0);
    m_pMainLayout->setSpacing(1);

    topHLayout->addLayout(m_pMainLayout);

    this->setLayout(topHLayout);
}

void CSubTab::setPacing(const int &space)
{
    m_pMainLayout->setSpacing(space);
}

void CSubTab::setContentsMargins(int left, int top, int right, int bottom)
{
    m_pMainLayout->setContentsMargins(left,top,right,bottom);
}

void CSubTab::addLine(int w,int h)
{
    HZLine* pLine = new HZLine(Qt::Vertical);
    pLine->setFixedSize(w,h);
    m_pMainLayout->addWidget(pLine);
    m_lineVec.push_back(pLine);
}

void CSubTab::addButton(const QString & name, const int  &id,HZLine* pLine)
{
    QPushButton* spButton =  new QPushButton(this);
    spButton->setObjectName(name);
    spButton->setText(name);
    spButton->setCheckable(true);

    m_pMainLayout->addWidget(spButton);

    int mId = id;
    if(mId<0){
        mId = m_buttonMap.size();
    }

    m_pButtonGroup->addButton(spButton, mId);
    m_buttonMap.insert(mId, spButton);
    if(pLine)
    {
        m_pMainLayout->addWidget(pLine);
        m_lineVec.push_back(pLine);
        m_lineMap.insert(mId,pLine);
    }
}

void CSubTab::addButtons(const QStringList &textList)
{
    int  mId = m_buttonMap.size();
    for(int i=0;i<textList.size();i++){
        QPushButton* spButton =  new QPushButton();
        spButton->setObjectName(textList[i]);
        spButton->setText(textList[i]);
        spButton->setCheckable(true);
        m_pMainLayout->addWidget(spButton);

        m_pButtonGroup->addButton(spButton, mId);
        m_buttonMap.insert(mId, spButton);
        mId++;
    }
}



void CSubTab::setButtons(const QStringList &textList)
{
    clear();
    addButtons(textList);
}



void CSubTab::setStretchLastType(StretchLastType stretchType)
{
    m_stretchLastType = stretchType;

    //1 先清除原来的填充
    QLayoutItem *layoutItem = topHLayout->itemAt(1);
    if(layoutItem)
        topHLayout->removeItem(layoutItem);

    //2 增加填充
    if(m_stretchLastType == Empty_Strecth){
        topHLayout->addStretch();
    }
    if(m_stretchLastType == Button_Strecth){
        QPushButton *stretchButton = new QPushButton(this);
        stretchButton->setChecked(false);
        stretchButton->setEnabled(false);
        stretchButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
        topHLayout->addWidget(stretchButton,1);
    }
}

void CSubTab::setCurrentButton(const int &id)
{
    auto it = m_buttonMap.find(id);
    if(it != m_buttonMap.end())
        it.value()->setChecked(true);
//    if(0<=id && id<  m_pButtonGroup->buttons().size())
//        m_pButtonGroup->button(id)->setChecked(true);
}

void CSubTab::clear()
{
    QList<int> keys = m_buttonMap.keys();
    foreach(int key,keys){
        m_pMainLayout->removeWidget(m_buttonMap.value(key));
        m_pButtonGroup->removeButton(m_buttonMap.value(key));
        delete m_buttonMap.value(key);
    }
    for(int i = 0;i  <m_lineVec.size();i++)
    {
        m_pMainLayout->removeWidget(m_lineVec[i]);
        delete  m_lineVec[i];
    }
    m_lineVec.clear();
    m_buttonMap.clear();
}

QPushButton *CSubTab::getButton(int nIndex)
{
    if(m_buttonMap.size())
    {
        return m_buttonMap[nIndex];
    }
    return NULL;
}

HZLine *CSubTab::getLine(int nIndex)
{
    if(m_lineMap.contains(nIndex))
    {
        return m_lineMap[nIndex];
    }
    return NULL;
}

void CSubTab::setAllUnCheck()
{
    m_currentBtnIdx = -1;
    int id =  m_pButtonGroup->checkedId();
    auto pButton = m_pButtonGroup->button(id);
    if(pButton)
    {
        m_pButtonGroup->setExclusive(false);
        pButton->setChecked(false);
        m_pButtonGroup->setExclusive(true);
    }
}

void CSubTab::setExclusiveEx(bool bFlg)
{
    m_pButtonGroup->setExclusive(bFlg);
}

void CSubTab::buttonToggled(int id,bool checked)
{
    if(checked){
        m_currentBtnIdx = id;
        //emit  currentButtonChanged(id);
    }
}

void CSubTab::slotsButtonClicked(int id)
{
    m_currentBtnIdx = id;
    emit currentButtonChanged(id);
}

void CSubTab::switchBack()
{
    //back
}

void CSubTab::switchForward()
{
    //forward
}




