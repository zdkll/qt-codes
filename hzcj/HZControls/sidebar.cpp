#include "sidebar.h"

#include <QButtonGroup>
#include <QToolButton>
#include <QVBoxLayout>
#include <QPushButton>

#include "hzline.h"
#include "micro.h"

SideBar::SideBar(QWidget *parent ,QString instName)
    :BaseWidget(parent,instName)
{
    createWg();
}

bool SideBar::isCheck(int id)
{
    bool bRet = false;
    if(m_btnGroup->checkedId() == id)
        bRet = true;
    return bRet;
}

int SideBar::getCurrCheckId()
{
    return m_btnGroup->checkedId();
}

void SideBar::setCheckId(int id)
{
    auto it = m_idMap.find(id);
    if(it != m_idMap.end())
    {
        auto pButton = m_btnGroup->button(id);
        if(pButton)
        {
            pButton->setChecked(true);
        }
    }
}

void SideBar::setAllUnCheck()
{
    int id =  m_btnGroup->checkedId();
    auto pButton = m_btnGroup->button(id);
    if(pButton)
    {
        m_btnGroup->setExclusive(false);
        pButton->setChecked(false);
        m_btnGroup->setExclusive(true);
    }
}

void SideBar::createWg()
{
    m_btnGroup = new QButtonGroup(this);
    m_vLayout = new QVBoxLayout;
    m_vLayout->setContentsMargins(0,9,0,0);
    this->setLayout(m_vLayout);
    m_vLayout->setSpacing(0);
    connect(m_btnGroup, SIGNAL(buttonClicked(int)),this,SLOT(slotsButtonClick(int)));
    m_btnGroup->setExclusive(true);
}

QToolButton * SideBar::addToolButton(QToolButton *toolButton,int id,const bool &addToGrp)
{
    if(addToGrp)
    {
        m_btnGroup->addButton(toolButton,id);
        m_idMap[id] = toolButton;
    }
    m_vLayout->addWidget(toolButton);
    return toolButton;
}

QToolButton *  SideBar::addToolButton(const QString &text,int id,const bool &addToGrp ,Qt::ToolButtonStyle toolButtonStyle)
{
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setText(text);
    toolButton->setToolButtonStyle(toolButtonStyle);
    if(addToGrp){
        m_btnGroup->addButton(toolButton,id);
        m_idMap[id] = toolButton;
    }
    m_vLayout->addWidget(toolButton, 0, Qt::AlignHCenter);
    return toolButton;
}

QToolButton *SideBar::addToolButton(const QIcon &icon,int id,const QString &text,const bool &addToGrp,Qt::ToolButtonStyle  toolButtonStyle)
{
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setIcon(icon);
    toolButton->setText(text);
    toolButton->setToolButtonStyle(toolButtonStyle);
    if(addToGrp)
    {
        m_btnGroup->addButton(toolButton,id);
        m_idMap[id] = toolButton;
    }
    m_vLayout->addWidget(toolButton);
    return toolButton;
}

HZLine *SideBar::addSeparator()
{
    HZLine *line =  new HZLine(Qt::Horizontal,this);
    //添加水平分割线
    m_vLayout->addWidget(line);
    return line;
}

QPushButton *SideBar::addLogo()
{
    QPushButton *btn =  new QPushButton(this);
    //添加程序logo
    m_vLayout->addWidget(btn, 0, Qt::AlignCenter);
    m_vLayout->addSpacing(11);
    return btn;
}

void SideBar::addStretch()
{
    m_vLayout->addStretch();
}

void SideBar::slotsButtonClick(int id)
{
    emit signalClickButton(id);
}

