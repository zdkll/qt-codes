#include "strategycenterpage.h"
#include <QGridLayout>
#include <QVBoxLayout>

#include "strategytitlewnd.h"
#include "strategyrelationstockswnd.h"
#include "strategyrelationinfownd.h"
#include "strategydynaprovider.h"
#include "strategyrelatedstockswidget.h"
#include "strategyrelatedinfowidget.h"
StrategyCenterPage::StrategyCenterPage(QWidget *parent, QString instName):
    BasePage(parent, instName)
{
    m_pProvider = new DynaDataProvider(this);
    m_pLongTermProvider = new DynaDataProvider(this);
    createWg();
}

StrategyCenterPage::~StrategyCenterPage()
{
}

void StrategyCenterPage::afterActive()
{
    m_bFirstActive = true;
    m_pProvider->startTimer();
    m_pLongTermProvider->startTimer();
    m_ptodyChanceWnd->afterActive();
    m_pLongTermChanceWnd->afterActive();

}

void StrategyCenterPage::afterDeactive()
{
    m_pProvider->stopTimer();
    m_pProvider->removestockAll();
    m_pLongTermProvider->stopTimer();
    m_pLongTermProvider->removestockAll();
    m_ptodyChanceWnd->afterDeactive();
    m_pLongTermChanceWnd->afterDeactive();
//    m_relationStocks->afterDeactive();
    m_bFirstActive = false;
}

void StrategyCenterPage::slotClickId(QString topic_id, QString codeName)
{
    qDebug() << "StrategyCenterPage::slotClickId: " << codeName;
    cancleInfoReq();
    TopicInvestDetailCallBack fun  = std::bind(&StrategyCenterPage::onTopicInvestDetail, this, std::placeholders::_1, std::placeholders::_2);
    m_reqId  = HzInfoCenter::getInstance()->reqTopicInvestDetail(topic_id,fun);
//    if(m_reqId == -1) //网络请求错误了
//        m_relationStocks->afterDeactive();

    m_pInfoWnd->afterDeactive();
    m_pInfoWnd->setNextPage(0);
    m_pInfoWnd->requestTopicNews(topic_id.toInt());
}

void StrategyCenterPage::createWg()
{
    QGridLayout* pLayout = new QGridLayout(this);
    pLayout->setVerticalSpacing(0);
    pLayout->setHorizontalSpacing(1);
    pLayout->setMargin(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(1);
    layout->setMargin(0);

    //今日机会
    m_ptodyChanceWnd = new StrategyTodayChanceWnd(this, TodayChanceType);
    m_ptodyChanceWnd->setDynaProvider(m_pProvider);
    m_ptodyChanceWnd->setFixedWidth(500);

    connect(m_ptodyChanceWnd, &StrategyTodayChanceWnd::updateReady, [&]{
        if(m_bFirstActive)
        {
            m_bFirstActive = false;
            m_ptodyChanceWnd->selectFirstRow();
        }
    });

    m_pLongTermChanceWnd =  new StrategyTodayChanceWnd(this, LongTermChanceType);
    m_pLongTermChanceWnd->setTitleText(QStringLiteral("主题投资-中长线机会"));
    m_pLongTermChanceWnd->setDynaProvider(m_pLongTermProvider);
    m_pLongTermChanceWnd->setFixedWidth(500);

    layout->addWidget(m_ptodyChanceWnd, 1);
    layout->addWidget(m_pLongTermChanceWnd, 1);

    pLayout->addLayout(layout,0,0,3,1);

    connect(m_ptodyChanceWnd, &StrategyTodayChanceWnd::signalsClickId, [&]{
        m_pLongTermChanceWnd->clearSelect();
    });

    connect(m_pLongTermChanceWnd, &StrategyTodayChanceWnd::signalsClickId, [&]{
        m_ptodyChanceWnd->clearSelect();
    });

    connect(m_ptodyChanceWnd, SIGNAL(signalsClickId(QString,QString)),this, SLOT(slotClickId(QString,QString)));
    connect(m_pLongTermChanceWnd, SIGNAL(signalsClickId(QString,QString)),this, SLOT(slotClickId(QString,QString)));

    //详情标题
    m_pTitleWnd = new StrategyTitleWnd();
    m_pTitleWnd->setDynaProvider(m_pProvider);
    m_pTitleWnd->setFixedHeight(203);
    pLayout->addWidget(m_pTitleWnd,0,1);

    //相关个股
//    m_relationStocks = new StrategyRelationStocksWnd();
//    m_relationStocks->setDynaProvider(m_pProvider);
//    m_relationStocks->setFixedHeight(300);
    //相关个股
    m_relatedStockWidget = new StrategyRelatedStocksWidget;
    m_relatedStockWidget->setFixedHeight(300);
    pLayout->addWidget(m_relatedStockWidget,1,1);

    //相关资讯
    m_pInfoWnd = new StrategyRelatedInfowidget();

    pLayout->addWidget(m_pInfoWnd,2,1);
}

void StrategyCenterPage::onTopicInvestDetail(const InfoRet &ret, const TopicInvestDetail &data)
{

    if(ret.code != 0){
        return;
    }

    m_pTitleWnd->updatedata(data);
    m_relatedStockWidget->updatedata(data);

#if 0
    m_relationStocks->afterDeactive();
    QVector<QString> list =data.stock;
    if(list.size() > 6)
        list = list.mid(0,6);//只保留六个
    m_relationStocks->setStockList(ret.code,list);
    m_relationStocks->afterActive();
#endif
}


void StrategyCenterPage::cancleInfoReq()
{
    if(m_reqId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_reqId);
        m_reqId = -1;
    }
}

