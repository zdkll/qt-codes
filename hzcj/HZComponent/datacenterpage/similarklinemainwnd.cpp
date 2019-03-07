#include "similarklinemainwnd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QDebug>
#include "simikview.h"
#include "SimilarKlineHandicapWidget.h"

SimilarKlineRightPanel::SimilarKlineRightPanel(QWidget *parent, QString instName):
    BaseWidget(parent,instName),
    m_rightHandicap(new SimilarKlineHandicapWidget(this))
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(1);
    CSubTab* m_topSub;
    m_topSub = new CSubTab;
    m_topSub->setObjectName("topTab");
    m_topSub->addButton(QStringLiteral("相似K线"),SimilarKlineType);
    m_topSub->addButton(QStringLiteral("战绩回顾"),RecordReviewType);
    pLayout->addWidget(m_topSub);
    m_rightHandicap->createWg();
//    m_rightHandicap->setStockCode("sh600000");
    m_rightHandicap->switchStocks("sh600000", "sh600332");
    pLayout->addWidget(m_rightHandicap);
//    pLayout->addStretch(1);
    connect(m_topSub,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwtichWnd(int)));
}

void SimilarKlineRightPanel::afterDeactive()
{
    m_rightHandicap->afterDeactive();
}

void SimilarKlineRightPanel::afterActive()
{
    //请求时间列表
    qDebug() << "SimilarKlineRightPanel::afterActive()";
    m_rightHandicap->afterActive();
}

void SimilarKlineRightPanel::switchStocks(const QString &stock1, const QString &stock2)
{
    qDebug() << " SimilarKlineRightPanel::switchStocks: stock1 " << stock1 << " stock2: " << stock2;
}

void SimilarKlineRightPanel::slotsSwtichWnd(int id)
{
    if(id  == SimilarKlineType)
    {
       emit signalSwitchSimilarKlineWnd(1549987200);
    }else if(id  == RecordReviewType)
    {
        emit signalSwitchRecordReviewWnd();
    }
}

SimilarKlineMainWnd::SimilarKlineMainWnd(QWidget *parent, QString instName)
    : NormalChildWnd(parent,instName),
      m_pMainChildWndMgr(new HzChildWndMgr<SimilarKlineMainWnd>(this,new QStackedWidget(this)))
{

}

void SimilarKlineMainWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
    m_pRightWnd->afterDeactive();
    m_similarKView->afterDeactive();
    m_currTime = -1;
}

void SimilarKlineMainWnd::afterActive()
{
    m_pRightWnd->afterActive();
    m_similarKView->afterActive();
}

void SimilarKlineMainWnd::createWg()
{
    m_pMainChildWndMgr->registChildCreater(&SimilarKlineMainWnd::createSimilarKlineWnd,SimilarKlineType,true);
    m_pMainChildWndMgr->registChildCreater(&SimilarKlineMainWnd::createRecordReviewWnd,RecordReviewType,false);
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd());
    pLayout->addSpacing(1);

    m_pRightWnd = new SimilarKlineRightPanel();
    m_pRightWnd->setFixedWidth(342);
    connect(m_pRightWnd, SIGNAL(signalSwitchSimilarKlineWnd(int64_t)), SLOT(slotSwitchSimilarKlineWnd(int64_t)));
    connect(m_pRightWnd, SIGNAL(signalSwitchRecordReviewWnd()), SLOT(slotSwitchRecordReviewWnd()));
    pLayout->addWidget(m_pRightWnd);
}

void SimilarKlineMainWnd::slotSwitchSimilarKlineWnd(int64_t time)
{
     m_currTime = time;
     //设置时间
     m_pMainChildWndMgr->setCurrWndByPageType(SimilarKlineType,true);
}

void SimilarKlineMainWnd::slotSwitchRecordReviewWnd()
{
    m_pMainChildWndMgr->setCurrWndByPageType(RecordReviewType,true);
}

ChildWndItem SimilarKlineMainWnd::createSimilarKlineWnd()
{
    m_similarKView = new SimiKView(this);
    m_similarKView->afterActive();
    ChildWndItem item;
    item.pObj   = m_similarKView;
    item.pWnd = m_similarKView;
    return item;
}

ChildWndItem SimilarKlineMainWnd::createRecordReviewWnd()
{
    BaseWidget * pWnd = new BaseWidget();
    pWnd->setObjectName("RecordReviewWnd");
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

