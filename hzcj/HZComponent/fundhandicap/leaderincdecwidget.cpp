#include "leaderincdecwidget.h"

#include <QBoxLayout>

#include "introductionbutton.h"
#include "datacontroller.h"
#include "leaderincdecchart.h"
#include "leadernetputchart.h"
#include "hzline.h"


LeaderIncDecWidget::LeaderIncDecWidget(QWidget *parent, QString instName)
    :BaseFundWidget(parent,instName),m_actived(false)
    ,m_subReqId(INVALID_REQID)
{
    createWg();
}

void LeaderIncDecWidget::afterActive()
{
    m_actived = true;
    if(!m_currentStock.isEmpty()){
        startSub();
    }
}

void LeaderIncDecWidget::afterDeactive()
{
    m_actived = false;
    this->cancelSub();
    this->clearData();
}

void LeaderIncDecWidget::recvLeaderNetput(const QVector<long long> &netputs)
{
    m_leaderNetputChart->setData(netputs);
}

void LeaderIncDecWidget::stockChanged(const QString &stockCode)
{
    if(m_currentStock != stockCode){
        m_currentStock = stockCode;
        if(m_actived)
            startSub();
    }
}

void LeaderIncDecWidget::startSub()
{
    cancelSub();

    //开始订阅
    HZData::ReqLeaderIncDecPostionInfo req;
    req = m_currentStock.toStdString();
    OnIncrDecPostionInfoArrived func = std::bind(&LeaderIncDecWidget::onLeaderIncDecDataArrived
                                                 ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_subReqId = gHZDataController->subIncDecPositionInfo(req,func);

}

void LeaderIncDecWidget::cancelSub()
{
    if(m_subReqId != INVALID_REQID)
    {
        gHZDataController->cancelSub(m_subReqId);
    }
}

void LeaderIncDecWidget::clearData()
{
    m_leaderIncDecChart->clearData();
    m_leaderNetputChart->clearData();
}

void LeaderIncDecWidget::onLeaderIncDecDataArrived(ReqId,const HZData::IncrDecPostionInfo &info,ErrCode)
{
    QString unitString =  m_leaderIncDecChart->setData(info);

    m_introductionBtn->setHintString(QString(QStringLiteral("最近30日主力增减仓（%1）")).arg(unitString));
}

void LeaderIncDecWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    m_introductionBtn = new IntroductionButton(QStringLiteral("最近30日主力增减仓（元）"),this);
    m_introductionBtn->setFixedHeight(26);
    mainLayout->addWidget(m_introductionBtn);

    //30日主力增减仓
    m_leaderIncDecChart = new LeaderIncDecChart(this);
    m_leaderIncDecChart->setFixedHeight(118);
    mainLayout->addWidget(m_leaderIncDecChart);

    mainLayout->addWidget(new HZLine(Qt::Horizontal));

    //3，5，10，20日主力净流入
    m_leaderNetputChart = new LeaderNetputChart(this);
    m_leaderNetputChart->setFixedHeight(42);
    mainLayout->addWidget(m_leaderNetputChart);

    this->setLayout(mainLayout);
}
