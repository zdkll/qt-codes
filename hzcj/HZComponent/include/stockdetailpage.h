#ifndef STOCKDETAILPAGE_H
#define STOCKDETAILPAGE_H

/********************************************************************
created:
author: min.cheng
purpose: 个股详情页面
*********************************************************************/
#include "hzcomponent_global.h"
#include "stockbasedetailpage.h"
class QuotePageLayout;
class StockDetailListWnd;
class StockDetailKlineMinWnd;
class StockDetailInfoWnd;
class QuoteDetailPanelWnd;
class HZCOMPONENTSHARED_EXPORT  StockDetailPage : public StockBaseDetailPage
{
Q_OBJECT
public:
    explicit StockDetailPage(QWidget *parent = 0,QString instName="");
protected:
    StockBaseDetailListWnd* createLeftListWnd();
    StockBaseDetailInfoWnd* createBottomWnd();
};



#endif // STOCKDETAILPAGE_H
