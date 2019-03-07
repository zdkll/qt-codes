#include "hzpagehelper.h"
#include <QBoxLayout>

#include "pagefactory.h"
#include "pubmsgdef.h"
#include "webmainpage.h"

void HzPageHelper::readPageCfg()
{
    WebMainPage::initialization();
    m_idInfoMap[home_page] = PageInfo("HomePage","home_page");
    m_idInfoMap[option_stock_page] = PageInfo("OptionStockPage","option_stock_page");
    m_idInfoMap[quote_list_page]= PageInfo("QuoteListPage","quote_list_page");
    m_idInfoMap[stock_detail_page]= PageInfo("StockDetailPage","stock_detail_page");
    m_idInfoMap[block_list_page]= PageInfo("BlockListPage","block_list_page");

    m_idInfoMap[data_center_page] = PageInfo("DataCenterPage","data_center_page");
    m_idInfoMap[strategy_center_page] = PageInfo("StrategyCenterPage","strategy_center_page");
    m_idInfoMap[info_center_page] = PageInfo("InfoMasterPage","info_center_page");

    m_idInfoMap[finance_news_page] = PageInfo("WebMainPage","finance_news_page");
    m_idInfoMap[live_video_page] = PageInfo("WebMainPage","live_video_page");
    m_idInfoMap[websit_page] = PageInfo("WebMainPage","websit_page");
    m_idInfoMap[activity_center_page] = PageInfo("WebMainPage","activity_center_page");
    m_idInfoMap[product_page] = PageInfo("WebMainPage","product_page");
    m_idInfoMap[short_msg_nuggets_page] = PageInfo("WebMainPage","short_msg_nuggets_page");
    m_idInfoMap[go_buy_page] = PageInfo("WebMainPage","go_buy_page");

    //m_idInfoMap[test_page] = PageInfo("test_page","test_page");
}

HzPageHelper::HzPageHelper(QWidget *parent,QString instName) :BasePage(parent,instName)
{
    m_stackWnd = new QStackedWidget();
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(m_stackWnd);
    this->setLayout(pLayout);
    readPageCfg();
}

BasePage* HzPageHelper::createPage(const QString &className, const QString &instanceName)
{
    //std::shared_ptr<T> ResolveShared
    BasePage * pPage = nullptr;
    if(className.isEmpty() == false)
    {
        std::string name = className.toStdString();
        QWidget* pThisWnd = (QWidget*)this;
        qDebug() << "HzPageHelper::createPage:" << className << ": " <<  instanceName;
        pPage = PageFactory::getIoc()->Resolve<BasePage>(name,pThisWnd,instanceName);
    }
    return pPage;
}

void HzPageHelper::notifyPageChange(int id,int childId)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,PAGE_CHANGED);
    msg.emplace(HZ_PAGE_ID,QString::number(id));
    msg.emplace(HZ_CHILD_CMD,QString::number(childId));
    broadcastMessage(msg);
}

void HzPageHelper::setCurrPageId(int id,const DetailPageParam& info)
{
    //判断页面是否已经创建
    auto it = m_idIndexMap.find(id);
    if(it != m_idIndexMap.end())
    {
        setGlobalPageId(id);
        setGlobalChildPageId(info.childWndId);
        //判断当前页面是否等于需要切换的页面
        int currId = getCurrPageId();
        if(currId != id) //不等于当前页面才进行切换
        {
            //取消上次页面激活功能
            BasePage* wnd = (BasePage*)m_stackWnd->currentWidget();
            wnd->afterDeactive();
            //设置当前页面并激活
            m_stackWnd->setCurrentIndex(it.value());
            wnd = (BasePage*)m_stackWnd->currentWidget();
            wnd->recvParam(info);
            wnd->afterActive();
        }else //等于当前页面 只传递数据
        {
            BasePage* wnd = (BasePage*)m_stackWnd->currentWidget();
            wnd->recvParam(info,true);
        }
        notifyPageChange(id,info.childWndId);
    }else
    {
        //创建页面 首先判断是否有配置
        auto it = m_idInfoMap.find(id);
        if(it != m_idInfoMap.end())
        {
            setGlobalPageId(id);
            setGlobalChildPageId(info.childWndId);
            BasePage* page = (BasePage*)m_stackWnd->currentWidget();
            //取消当前页面激活
            if(page)
                page->afterDeactive();
            page = createPage(it.value().className,it.value().instanceName);
            if(page)
            {
                page->setOwner(this);
                int currIndex = m_stackWnd->addWidget((QWidget*)page);
                m_indexIdMap[currIndex] = id;
                m_idIndexMap[id] = currIndex;
                m_stackWnd->setCurrentIndex(currIndex);
                page->recvParam(info);
                page->afterActive();
                notifyPageChange(id,info.childWndId);
            }
        }
    }
}

int HzPageHelper::getCurrPageId()
{
    int ret = m_stackWnd->currentIndex();
    if(ret != InvalidPage)
    {
        auto it = m_indexIdMap.find(ret);
        if(it != m_indexIdMap.end())
            ret = it.value();
    }
    return ret;
}

void HzPageHelper::afterActive()
{
    BaseWidget* wnd = (BaseWidget*)m_stackWnd->currentWidget();
    //当前页面激活
    if(wnd)
        wnd->afterActive();
}

void HzPageHelper::afterDeactive()
{
    BaseWidget* wnd = (BaseWidget*)m_stackWnd->currentWidget();
    //取消当前页面激活
    if(wnd)
        wnd->afterDeactive();
}

void HzPageHelper::onNetIoReady(bool ready)
{
    BaseWidget* wnd = (BaseWidget*)m_stackWnd->currentWidget();
    if(wnd)
        wnd->onNetIoReady(ready);
}

void HzPageHelper::defaultCreaePage(const QVector<int> &pages)
{
    for(int i=0;i < pages.size();i++)
    {
        auto it = m_idInfoMap.find(pages[i]);
        if(it != m_idInfoMap.end())
        {
            BasePage* page = createPage(it.value().className,it.value().instanceName);
            if(page)
            {
                page->setOwner(this);
                int index = m_stackWnd->addWidget((QWidget*)page);
                m_indexIdMap[index] = pages[i];
                m_idIndexMap[pages[i]] = index;
            }
        }
    }
}
