/********************************************************************
created:
author: dk.zhang
purpose:资金流向窗口，顶部
*********************************************************************/
#ifndef FUNDFLOWWIDGET_H
#define FUNDFLOWWIDGET_H

#include "basefundwidget.h"
#include "datamodeldef.h"

class FundFlowImage;
class FundFlowWidget : public BaseFundWidget
{
    Q_OBJECT
public:
    FundFlowWidget(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

signals:
    void recvLeaderNetput(const QVector<long long> &);

protected:
    void stockChanged(const QString &stockCode);

private:
    void subOptionalFund();
    void cancelSub();

    void optionalFundArrived(ReqId , const char *data,int num, ErrCode );

    void createWidget();

    FundFlowImage *m_fundFlowImage;
    ReqId                m_subReqId;
    bool                  m_isActived;
};

#endif // FUNDFLOWWIDGET_H
