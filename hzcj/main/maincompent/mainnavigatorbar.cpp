#include "mainnavigatorbar.h"
#include <QPushButton>
#include <QVector>
#include "pubmsgdef.h"
#include "hzauthenopt.h"
#include "hzline.h"

MainNavigatorBar:: MainNavigatorBar(QWidget *parent ,QString instName)
    :NavigatorBar(parent,instName)
{
    createWg();

    connect(this,SIGNAL(subTabSectionChanged(int,int)),this,SLOT(slotSubTabSectionChanged(int,int)));
}

enum UserPermissionType
{
    up_zbs = 0,    //直播室  zbs
    up_yk = 1,      //月刊      yk
    up_tzyf = 2,    //投资有方tzyf
    up_zk = 3,      //周刊      zk
    up_tzxt = 4,    //投资学堂tzxt
    up_dkx = 5,    //大咖秀    dkx
    up_dpzs = 6,  //盯盘助手 dpzs
    up_cjrl = 7,     //财经日历  cjrl
    up_dxjj = 8,    //短线掘金  dxjj
    up_nrzg = 9,  //牛人掌股  nrzg
    up_hzyj = 10, //汇正赢家  hzyj
    up_cynx = 11, //穿越牛熊  cynx
    up_cljq = 12,  //策略进取  cljq
    up_jgjj = 13,    //金股狙击  jgjj
    up_jhjj = 14,    //集合竞价  jhjj
    up_xskx = 15,  //相似K线   xskx
    up_lhxj   = 16, //龙虎先机   lhxj
    up_szkc = 17,//实战课程   szkc
    up_hztj = 18,//汇正投教   hztj
    up_zbhjjj = 19,//指标黄金狙击   zbhjjj
    up_zbdcxg = 20,//指标多彩选股   zbdcxg
    up_byzs = 21,//指标博弈致胜   byzs
    up_bdww = 22,//指标波段为王   bdww
    up_ztjc = 23//涨停监测   ztjc
};

void MainNavigatorBar::onNetIoReady(bool ready)
{
    if(!ready)
        return;
    QPushButton * activePage = m_rootTab->getButton(activity_center_page);
    QPushButton * productPage = m_rootTab->getButton(product_page);
    HZLine* activeLine = m_rootTab->getLine(activity_center_page);
    HZLine* productPageLine = m_rootTab->getLine(product_page);
    if(activePage == nullptr || productPage == nullptr || activeLine == nullptr || productPageLine == nullptr )
        return;
    if(HZAuthenOpt::instance()->hasPermission(HZAuthen::up_dxjj))
    {
        if(activePage->isHidden())
        {
            activePage->show();
            activeLine->show();
        }
        if(productPage->isHidden())
        {
            productPage->show();
            productPageLine->show();
        }
        productPage->setText(QStringLiteral("短线掘金"));
    }else if(HZAuthenOpt::instance()->hasPermission(HZAuthen::up_jgjj))
    {
        if(activePage->isHidden())
        {
            activePage->show();
            activeLine->show();
        }
        if(productPage->isHidden())
        {
            productPage->show();
            productPageLine->show();
        }
        productPage->setText(QStringLiteral("金股狙击"));

    }else if(HZAuthenOpt::instance()->hasPermission(HZAuthen::up_nrzg))
    {
        if(activePage->isHidden())
        {
            activePage->show();
            activeLine->show();
        }
        if(productPage->isHidden())
        {
            productPage->show();
            productPageLine->show();
        }
        productPage->setText(QStringLiteral("牛人掌股"));

    }else if(HZAuthenOpt::instance()->hasPermission(HZAuthen::up_hzyj))
    {
        if(activePage->isHidden())
        {
            activePage->show();
            activeLine->show();
        }
        if(productPage->isHidden())
        {
            productPage->show();
            productPageLine->show();
        }
        productPage->setText(QStringLiteral("汇正赢家"));

    }else
    {
        if(activePage->isVisible())
        {
            activePage->hide();
            activeLine->hide();
        }
        if(m_rootTab->currentButton() == activity_center_page)
        {
            slotSubTabSectionChanged(home_page,home_page);
        }
        if(productPage->isVisible())
        {
            productPage->hide();
            productPageLine->hide();
        }
        if(m_rootTab->currentButton() == product_page)
        {
            slotSubTabSectionChanged(home_page,home_page);
        }
    }
}

void MainNavigatorBar::createWg()
{
    m_rootTab = addCSubTab(CSubTab::Root_Tab,"roottab");
    m_rootTab->setObjectName("roottab");
    QVector<NavigatorBarItem> barList;
    barList.push_back(NavigatorBarItem(quote_list_page,QStringLiteral("沪深综合"),HszhInnerWndType));
    barList.push_back(NavigatorBarItem(option_stock_page,QStringLiteral("自选股"),OptionListWndType,0));
    barList.push_back(NavigatorBarItem(block_list_page,QStringLiteral("板块"),103));
    barList.push_back(NavigatorBarItem(data_center_page,QStringLiteral("集合竞价"),CallAuctionWnd));
    barList.push_back(NavigatorBarItem(info_center_page,QStringLiteral("热门资讯"),0));
    barList.push_back(NavigatorBarItem(live_video_page,QStringLiteral("视频直播")));
    barList.push_back(NavigatorBarItem(websit_page,QStringLiteral("官网")));
    barList.push_back(NavigatorBarItem(activity_center_page,QStringLiteral("活动中心")));
    //barList.push_back(NavigatorBarItem(test_page,QStringLiteral("页面测试")));


//    barList.push_back(NavigatorBarItem(strategy_center_page,QStringLiteral("策略中心")));
//    barList.push_back(NavigatorBarItem(short_msg_nuggets_page,QStringLiteral("短线掘金")));
    barList.push_back(NavigatorBarItem(product_page,QStringLiteral("")));
    m_rootTab->setPacing(0);
    m_rootTab->setContentsMargins(2, 0, 0, 0);
    for(int i =0;i < barList.size();i++)
    {
        HZLine* pLine = new HZLine(Qt::Vertical);
        pLine->setFixedSize(1,26);
        m_rootTab->addButton(barList[i].name,barList[i].type,pLine);
        m_tabMap[barList[i].type] = barList[i];
    }

    #if 1
    m_rootTab->getButton(activity_center_page)->hide();
    m_rootTab->getButton(product_page)->hide();
    m_rootTab->getLine(activity_center_page)->hide();
    m_rootTab->getLine(product_page)->hide();
    #endif

    m_rootTab->setStretchLastType(CSubTab::Button_Strecth);
    m_rootTab->getButton(activity_center_page)->setObjectName("actcenter");
    m_rootTab->getButton(product_page)->setObjectName("productItem");

    //m_rootTab->getButton(strategy_center_page)->setObjectName("pcycenter");
    //m_rootTab->getButton(short_msg_nuggets_page)->setObjectName("dxjj");


}

void MainNavigatorBar::onMessage(const QString &senderPath, const MessageParams &params)
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

void MainNavigatorBar::procePageChanged(const MessageParams &params)
{
    int childId = -1;
    auto cit = params.find(HZ_CHILD_CMD);
    if(cit != params.end())
        childId = cit->second.toInt();
    auto it = params.find(HZ_PAGE_ID);
    if(it != params.end())
    {
        int id = it->second.toInt();
        auto iter =m_tabMap.find(id);
        if(iter != m_tabMap.end() &&(childId == -1 || childId == iter.value().childId))
        {
            if(m_rootTab->currentButton() != id)
            {
                m_rootTab->setCurrentButton(id);
            }
        }else
        {
            m_rootTab->setAllUnCheck();
        }
    }
  this->update();
}

void MainNavigatorBar::slotSubTabSectionChanged(int tab,int button)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(button));
    if(m_tabMap.contains(button))
    {
        msg.emplace(CHILD_WND_ID,QString::number(m_tabMap[button].childId));
        if(m_tabMap[button].thridChildId != -1)
            msg.emplace(THIRD_CHILD_WND_ID,QString::number(m_tabMap[button].thridChildId));
    }

    sendMessage(ROOT_FRAME,msg,true);
    update();
}
