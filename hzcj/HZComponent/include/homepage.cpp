#include "homepage.h"

#include <QVector>
#include <QGridLayout>
#include <QMap>
#include <QSignalMapper>

#include "suoluefactory.h"

//测试资讯数据
#include "hzinfocenter.h"
class testReq
{
  public:
    void req()
    {
        //HzInfoCenter::getInstance()->reqHostNews(0,std::bind(&testReq::hostNews,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        //HzInfoCenter::getInstance()->reqHostDetail("60",std::bind(&testReq::hostNewsDetail,this,std::placeholders::_1,std::placeholders::_2));
        //
        //HzInfoCenter::getInstance()->reqFlashNew(0,std::bind(&testReq::flashNews,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        //HzInfoCenter::getInstance()->reqFlashNewDetail("60",std::bind(&testReq::flashNewsDetail,this,std::placeholders::_1,std::placeholders::_2));
        //HzInfoCenter::getInstance()->reqFinanceInfo("sz002196",std::bind(&testReq::financeInfoEx,this,std::placeholders::_1,std::placeholders::_2));
        //HzInfoCenter::getInstance()->reqResearchReport("sz002196",std::bind(&testReq::researchReportEx,this,std::placeholders::_1,std::placeholders::_2));
        //HzInfoCenter::getInstance()->reqResearchReportDetail("33926",std::bind(&testReq::researchReportDetailEx,this,std::placeholders::_1,std::placeholders::_2));

        QVector<QString> list;
        list.push_back("sz000001");
        list.push_back("sz002723");
       // HzInfoCenter::getInstance()->reqNewsInfo(list,0,std::bind(&testReq::newsInfo,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        HzInfoCenter::getInstance()->reqNewInfoDetail("222157",std::bind(&testReq::newInfoDetail,this,std::placeholders::_1,std::placeholders::_2));

        HzInfoCenter::getInstance()->reqAnncsInfo(list,0,std::bind(&testReq::anncsInfo,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        HzInfoCenter::getInstance()->reqAnncInfoDetail("366889",std::bind(&testReq::anncInfoDetail,this,std::placeholders::_1,std::placeholders::_2));
    }

    void hostNews(const InfoRet &ret, HostNews &data, int npage)
    {
        HzInfoCenter::getInstance()->reqHostNews(npage,std::bind(&testReq::hostNews,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        qDebug() << "hostNews: " ;
    }

    void hostNewsDetail(const InfoRet &ret, const HostNewDetail &data)
    {
        qDebug() << "hostNewsDetail: ";
    }

    void flashNews(const InfoRet &ret, FlashNews &data, int npage)
    {
          qDebug() << "flashNews: ";
    }

    void flashNewsDetail(const InfoRet &ret, const FlashNewDetail &data)
    {
        qDebug() << "flashNewsDetail: ";
    }

    void financeInfoEx(const InfoRet &ret,const FinanceInfo& info)
    {

    }

    void researchReportEx(const InfoRet &ret,const ResearchReports& info)
    {

    }

    void researchReportDetailEx(const InfoRet &ret,const ResearchReportDetail& info)
    {

    }

    void newsInfo(const InfoRet &ret,const NewsInfo &data, int npage)
    {
    }

    void newInfoDetail(const InfoRet &ret, const NewInfoDetail &data)
    {
    }

    void anncsInfo(const InfoRet &ret,const AnncsInfo &data, int npage)
    {
    }

    void anncInfoDetail(const InfoRet &ret, const AnncInfoDetail &data)
    {
    }
};
HomePage::HomePage(QWidget *parent, QString instName):
    BasePage(parent, instName),m_signalMapper(new QSignalMapper(this))
  ,m_currentViewIdx(-1)
{
    testReq tes;
    tes.req();
    createWg();
}

HomePage::~HomePage()
{

}

void HomePage::afterActive()
{
    for(int i =0;i < m_slWndList.size();i++)
    {
        m_slWndList[i]->afterActive();
    }
}

void HomePage::afterDeactive()
{
    for(int i =0;i < m_slWndList.size();i++)
    {
        m_slWndList[i]->afterDeactive();
    }
}

void HomePage::createWg()
{
    QVector<SuolueInfo> list;

    //缩略分时图1
    SuolueInfo info;
    info.className = "SuolueMin";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("上证指数")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("沪深300")));
    info.itemList.push_back(SuolueItem(2,QStringLiteral("上证50")));
    info.attr |= SuolueHideMoreBt;
    QMap<int,QString> codeMap;
    codeMap[0] = "sh000001";
    codeMap[1] = "sh000300";
    codeMap[2] = "sh000016";
    info.data = codeMap;
    list.push_back(info);

    //缩略分时图2
    info = SuolueInfo();
    info.className = "SuolueMin";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("深证成指")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("中小板指")));
    info.itemList.push_back(SuolueItem(2,QStringLiteral("创业板指")));
    info.attr |= SuolueHideMoreBt;
    info.row = 0;
    info.column = 1;
    codeMap.clear();
    codeMap[0] = "sz399001";
    codeMap[1] = "sz399005";
    codeMap[2] = "sz399006";
    info.data = codeMap;
    list.push_back(info);

    //图3
    info = SuolueInfo();
    info.className = "SuolueRiseFall";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("涨跌分布")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("上证指数")));
    info.itemList.push_back(SuolueItem(2,QStringLiteral("深证成指")));
    info.itemList.push_back(SuolueItem(3,QStringLiteral("中小板指")));
    info.itemList.push_back(SuolueItem(4,QStringLiteral("创业板指")));
    info.attr |= SuolueHideMoreBt;
    info.row = 0;
    info.column = 2;
    list.push_back(info);


    //图4 沪深A涨跌
    info = SuolueInfo();
    info.className = "SuolueHqView";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("沪A涨跌")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("深A涨跌")));
    info.attr |= SuolueHideMoreBt;
    info.row = 1;
    info.column = 0;
    list.push_back(info);

    //图5 板块
    info = SuolueInfo();
    info.className = "SuolueBlock";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("行业板块")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("概念板块")));
    info.attr |= SuolueHideMoreBt;
    info.row = 1;
    info.column = 1;
    list.push_back(info);


    //图6 资金
    info = SuolueInfo();
    info.className = "SuolueFund";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("资金流入")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("资金流出")));
    info.attr |= SuolueHideMoreBt;
    info.row = 1;
    info.column = 2;
    list.push_back(info);

    //图7
    info = SuolueInfo();
    info.className = "SuolueZxHist";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("自选股")));
    info.itemList.push_back(SuolueItem(1,QStringLiteral("最近浏览")));
    info.attr |= SuolueHideMoreBt;
    info.row = 2;
    info.column = 0;
    info.columnSapn = 2;
    list.push_back(info);

    //图8
    info = SuolueInfo();
    info.className = "SuoLueHotInfo";
    info.itemList.push_back(SuolueItem(0,QStringLiteral("热门资讯")));
    //info.attr |= SuolueHideMoreBt;
    info.row = 2;
    info.column =2;
    list.push_back(info);

    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(10);
    layout->setSpacing(3);
    QWidget* pThis = this;
    SuolueBase* pWnd = nullptr;
    for(int i = 0; i < list.size(); i++)
    {
        pWnd = SuolueFactory::getIoc()->Resolve<SuolueBase>(list[i].className.toStdString(),list[i],pThis);
        pWnd->createWg();
        layout->addWidget(pWnd,list[i].row,list[i].column,list[i].rowSpan,list[i].columnSapn,list[i].align);
        m_slWndList.push_back(pWnd);
        m_signalMapper->setMapping(pWnd,i);
        connect(pWnd,SIGNAL(currentStockChanged()),m_signalMapper,SLOT(map()));
    }
    connect(m_signalMapper, SIGNAL(mapped(int)),
                  this, SLOT(slotCurrentStockViewChanged(int)));

    layout->setRowStretch(0,1);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,1);
    setLayout(layout);
}

void  HomePage::slotCurrentStockViewChanged(int idx)
{
    //qDebug()<<"current stock view changed:"<<idx;
    if(m_currentViewIdx>=0 && m_currentViewIdx != idx)
     {
        //qDebug()<<"view"<<m_currentViewIdx<<" focus out";
        m_slWndList[m_currentViewIdx]->onFocusOut();
    }
    m_currentViewIdx = idx;
}

