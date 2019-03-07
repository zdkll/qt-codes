/********************************************************************
created:
author: dk.zhang
purpose:逐笔和分价表窗口
*********************************************************************/
#ifndef ZHUBIHANDICAPVIEW_H
#define ZHUBIHANDICAPVIEW_H

#include <basewidget.h>

class  SLTimeDealWidget;
class  PriceTickView;
class ZhubiHandicapView : public BaseWidget
{
public:
    enum ZbHandType
    {
        ZB_Stock = 0,
        ZB_Index = 1,
    };
    ZhubiHandicapView(ZbHandType type = ZB_Stock,QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void setStockCode(const QString &stockCode);

private:
    void createWg();
    SLTimeDealWidget  *m_timeDealWg;
    PriceTickView           *m_priceTickView;

    ZbHandType             m_zbhdType;
};

#endif // ZHUBIHANDICAPVIEW_H
