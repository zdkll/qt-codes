#include "mainsidebar.h"

#include <QToolButton>
#include <QPushButton>
#include <QBoxLayout>

#include "pubmsgdef.h"

MainSideBar::MainSideBar(QWidget *parent ,QString instName)
    :SideBar(parent,instName)
{
    createWg();
}

void MainSideBar::createWg()
{

    QVector<NavigatorBarItem> barList;
    barList.push_back(NavigatorBarItem(home_page,QStringLiteral("首页"),"home"));
    barList.push_back(NavigatorBarItem(option_stock_page,QStringLiteral("自选"),"selfstock"));
    barList.push_back(NavigatorBarItem(stock_detail_page,QStringLiteral("个股"),"hsstock"));
    barList.push_back(NavigatorBarItem(quote_list_page,QStringLiteral("行情"),"Quotes"));
    barList.push_back(NavigatorBarItem(block_list_page,QStringLiteral("板块"),"hsbk"));
    barList.push_back(NavigatorBarItem(info_center_page,QStringLiteral("资讯"),"hszx"));
    barList.push_back(NavigatorBarItem(data_center_page,QStringLiteral("数据"),"Data"));
    barList.push_back(NavigatorBarItem(strategy_center_page,QStringLiteral("策略"),"strategy"));

    QPushButton *btn = addLogo();
    btn->setObjectName("logo");

    QToolButton *toolButton;
    for(int i =0;i < barList.size();i++)
    {
        toolButton = addToolButton(barList[i].name,barList[i].type);
        toolButton->setObjectName(barList[i].style);
        toolButton->setCheckable(true);
        m_tabMap[barList[i].type] = barList[i];
    }
    addStretch();
}

void MainSideBar::onMessage(const QString &senderPath, const MessageParams &params)
{
    if(senderPath == BROAD_CASET)
    {
        auto it = params.find(HZ_CMD);
        if(it != params.end())
        {
            if(it->second == PAGE_CHANGED)
            {
                procePageChanged(params);
            }
        }
    }
}

void MainSideBar::slotsButtonClick(int id)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(id));
    msg.emplace(STOCK_DETAIL_TYPE,QString::number(stock_detail_id));
    sendMessage(ROOT_FRAME,msg,true);
    this->update();
}

void MainSideBar::procePageChanged(const MessageParams &params)
{
    auto it = params.find(HZ_PAGE_ID);
    if(it != params.end())
    {
        int id = it->second.toInt();
        auto iter = m_tabMap.find(id);
        if(iter != m_tabMap.end())
        {
            if(!isCheck(id))
            {
                setCheckId(id);
            }
        }else
        {
            setAllUnCheck();
        }
    }
}
