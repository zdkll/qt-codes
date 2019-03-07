/********************************************************************
created:
author: dk.zhang
purpose:资金流向图像
*********************************************************************/
#ifndef FUNFLOWIMAGE_H
#define FUNFLOWIMAGE_H

#include <QWidget>

#include "datamodeldef.h"

class  FundFlowBarChart;
class  FundFlowPieChart;
struct FundInOutData;
class FundFlowImage : public QWidget
{
public:
    FundFlowImage(QWidget *parent = 0);
   ~FundFlowImage();

    void setDayMoneyField(const HZData::MoneyField&);
    void clearData();

private:
    void createWg();

    FundFlowPieChart  *m_fundFlowPieChart;
    FundFlowBarChart  *m_fundFlowBarChart;

    FundInOutData       *m_fundInOutData;
};


#endif // FUNFLOWIMAGE_H
