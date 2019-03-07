#include "suoluehotinfo.h"
#include "infowidget.h"
#include "pubmsgdef.h"

SuoLueHotInfo::SuoLueHotInfo(SuolueInfo info, QWidget *parent):
    SuolueBase(info,parent)
{
}

void SuoLueHotInfo::afterActive()
{
    if(m_infoWidget)
        m_infoWidget->afterActive();
}

void SuoLueHotInfo::afterDeactive()
{
    if(m_infoWidget)
        m_infoWidget->afterDeactive();
}

QWidget *SuoLueHotInfo::createContentWnd()
{
    m_infoWidget = new InfoWidget(this);
    return m_infoWidget;
}

void SuoLueHotInfo::onClickMore()
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(info_center_page));
    sendMessage(ROOT_FRAME,msg,true);
}

void SuoLueHotInfo::onClickItem(const SuolueItem &item)
{

}

SuolueTitleBase *SuoLueHotInfo::createTitleWnd()
{
    SuolueTitleBase * pWnd = new SuolueTitleBase(m_slInfo);
    pWnd->setObjectName("hostInfo");
    return pWnd;
}
