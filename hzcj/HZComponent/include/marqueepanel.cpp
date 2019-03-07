#include "marqueepanel.h"
#include <QtWidgets>
#include <QDateTime>
#include "marqueeitem.h"
#include "marqueegraphicsview.h"
#include "hzinfocenter.h"
#include "hzinfoadapter.h"
#include "hzinfomationdlg.h"

class MarqueePanel;
class MarqueeReq
{
  public:

    MarqueeReq(MarqueePanel* pPanel):m_panel(pPanel)
    {
        m_newsId= -1;
    }

    ~MarqueeReq()
    {
        HzInfoCenter::getInstance()->cancle(m_newsId);
    }

    void reqFlashNew()
    {
        HzInfoCenter::getInstance()->cancle(m_newsId);
        m_newsId = HzInfoCenter::getInstance()->reqFlashNew(0, 2, std::bind(&MarqueeReq::flashNews,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    }

    void reqFlashNewDetail(const QString& id)
    {
       HzInfoCenter::getInstance()->reqFlashNewDetail(id,std::bind(&MarqueeReq::flashNewsDetail,this,std::placeholders::_1,std::placeholders::_2));
    }

    void flashNews(const InfoRet &ret, FlashNews &data, int npage)
    {
          qDebug() << "flashNews: ";
          mNextPage = npage;
          m_data    = data;
          m_panel->onDataCall(ret.code,ret.message);
    }

    void flashNewsDetail(const InfoRet &ret, const FlashNewDetail &data)
    {
        qDebug() << "flashNewsDetail: ";
        m_detail =data;
        m_panel->onDetailDataCall(ret.code,ret.message);
    }

    const FlashNews& getFlashNews() const
    {
        return m_data;
    }

    const FlashNewDetail& getFlashNewDetail() const
    {
        return m_detail;
    }

private:
    MarqueePanel* m_panel;
    FlashNews m_data;
    FlashNewDetail m_detail;
    int m_newsId;
    int mNextPage{0};
    friend class MarqueePanel;
};

MarqueePanel::MarqueePanel(QWidget *parent,QString instName) :
    BaseWidget(parent,instName)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_view =  new MarqueeGraphicsView(scene,this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(1000/30);

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget((QWidget*)m_view);
    setLayout(pLayout);
    m_req = new MarqueeReq(this);
    m_req->reqFlashNew();
    //定时刷新快讯 20s刷新一次
    QTimer* timer1 = new QTimer(this);
    QObject::connect(timer1, SIGNAL(timeout()), this, SLOT(onTimeRefreshNews()));
    timer1->start(120000);
}

MarqueePanel::~MarqueePanel()
{
    if(m_req)
    {
        delete m_req;
        m_req = nullptr;
    }
}

void MarqueePanel::resizeEvent(QResizeEvent *event)
{
    updateScene();
    BaseWidget::resizeEvent(event);
}

void MarqueePanel::onDataCall(int ret,const QString& msg)
{
    if(ret == 0)
    {
        const FlashNews &data =m_req->getFlashNews();
        MarqueeDataItem item;
        m_data.clear();
        for(int i=0;i < data.size();i++)
        {
            item.id = data[i].id;
            item.time = data[i].time;
            item.title = data[i].title;
            item.url = data[i].url;
            m_data.push_back(item);
        }
        updateScene();
    }
}

void MarqueePanel::onDetailDataCall(int ret,const QString& msg)
{
}

void MarqueePanel::onTimeRefreshNews()
{
    m_req->reqFlashNew();
}

void MarqueePanel::onMarqueeClick(const QString &id)
{
    //id 无效或数据列表为空 不弹出对话框
    if(!id.isEmpty()) {
        STInfoData tmpDatas;
        for(const FlashNewItem &item : m_req->getFlashNews()) {
            STVListData st;
            st.id = item.id;
            st.body = item.title;
            st.url = item.url;
            st.times = QDateTime::fromTime_t(item.time).toString("yyyy-MM-dd hh:mm:ss");
            tmpDatas.datas.append(st);
        }

        if(!tmpDatas.datas.isEmpty()){
            HZ7Multiply24Info info(id);
            info.setFirstDatas(tmpDatas);
            info.setNextpage(m_req->mNextPage);
//            HZInfomationDlg dlg(HZInfomationDlg::Flash_News, &info);
            gInfoDlg->setVisiter(HZInfomationDlg::Flash_News, &info);
//            dlg.exec();
        }
    }
}

void MarqueePanel::updateScene()
{
    MarqueeItem::stopScroll(true);
    scene->clear();
    int pos = 0;
    int interval = 50;
    MarqueeDataItem info;
    for (int i = 0; i < m_data.size(); ++i) {
        info = m_data[i];
        info.title = QDateTime::fromTime_t(m_data[i].time).toString("hh:mm ");
        info.title += m_data[i].title;
        MarqueeItem *item = new MarqueeItem(info,m_view);
        //item->setText(m_data[i].title);
        item->setElemInterval(interval);
        item->setPos(pos,0);
        scene->addItem(item);
        pos += (item->getWidth()+interval);
    }
    MarqueeItem::resetTailItem();
    MarqueeItem::stopScroll(false);
}
