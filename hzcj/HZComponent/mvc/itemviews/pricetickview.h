/********************************************************************
created:
author: dk.zhang
purpose:分价表
*********************************************************************/
#ifndef PRICETICKVIEW_H
#define PRICETICKVIEW_H
#include "handtableview.h"

#include "hzcomponent_global.h"
#include "hzobject.h"

class PriceTickModel;
class PriceTickProvider;
class   PriceTickView : public  HandTableView
{
    Q_OBJECT

public:
    PriceTickView(QWidget *parent = 0);

    //设置每一列的宽度百分比
    void setColStretchFactor(int idx,float factor);

    void setStockCode(const QString &stockCode);

    void currentRangeChanged(int start, int count);

protected:
    void initItemDelegates() Q_DECL_OVERRIDE;

private:
    PriceTickModel    *m_model;

    PriceTickProvider *m_priceTickProvider;
};

#endif // PRICETICKVIEW_H
