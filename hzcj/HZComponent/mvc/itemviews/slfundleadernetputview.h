/********************************************************************
created:
author: dk.zhang
purpose:主力资金净流入盘口列表
*********************************************************************/
#ifndef SLFUNDLEADERNETPUTVIEW_H
#define SLFUNDLEADERNETPUTVIEW_H

#include <QTableView>

#include "fundleadernetputmodel.h"
#include "viewstyleproperties.h"
#include "handtableview.h"

#include "hzcomponent_global.h"

class LeaderNetputProvider;

class  SLFundLeaderNetputView :public HandTableView
{
    Q_OBJECT
public:
    SLFundLeaderNetputView(QWidget *parent = 0);

    void setStockCode(const QString &stock);

protected:
    virtual void initItemDelegates();

private:
    LeaderNetputProvider *m_lnpProvider;
};

#endif // SLFUNDLEADERNETPUTVIEW_H
