/********************************************************************
created:
author: dk.zhang
purpose:资金30 日主力增减仓，中部
*********************************************************************/
#ifndef LEADERINCDECWIDGET_H
#define LEADERINCDECWIDGET_H

#include "basefundwidget.h"

#include "datamodeldef.h"

class IntroductionButton;
class LeaderIncDecChart;
class LeaderNetputChart;
class LeaderIncDecWidget : public BaseFundWidget
{
    Q_OBJECT
public:
    LeaderIncDecWidget(QWidget *parent = 0,QString instName="");
    void afterActive();

    void afterDeactive();

public slots:
   void recvLeaderNetput(const QVector<long long> &);

protected:
    void stockChanged(const QString &stockCode);

private:
    void createWg();

    void onLeaderIncDecDataArrived(ReqId,const HZData::IncrDecPostionInfo &info,ErrCode);

    void startSub();
    void cancelSub();
    void clearData();

    IntroductionButton *m_introductionBtn;

    QString m_currentStock;
    bool      m_actived;

    ReqId   m_subReqId;

    LeaderIncDecChart  *m_leaderIncDecChart;
    LeaderNetputChart  *m_leaderNetputChart;
};

#endif // LEADERINCDECWIDGET_H
