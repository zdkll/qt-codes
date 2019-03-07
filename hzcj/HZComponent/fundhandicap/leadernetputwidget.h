/********************************************************************
created:
author: dk.zhang
purpose:资金净流入，底部
*********************************************************************/
#ifndef LEADERNETPUTWIDGET_H
#define LEADERNETPUTWIDGET_H

#include "basefundwidget.h"

class IntroductionButton;
class SLFundLeaderNetputView;
class LeaderNetputWidget : public BaseFundWidget
{
    Q_OBJECT
public:
    LeaderNetputWidget(QWidget *parent = 0,QString instName="");
    void afterActive();

    void afterDeactive();

protected:
    void stockChanged(const QString &stockCode);

private:
    void createWg();

    IntroductionButton *m_introductionBtn;
    SLFundLeaderNetputView *m_view;
};

#endif // LEADERNETPUTWIDGET_H
