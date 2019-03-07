#ifndef SMTLSGRAPHTITLE_H
#define SMTLSGRAPHTITLE_H
/********************************************************************
created:
author: min.cheng
purpose:两市融资融券标题部分
*********************************************************************/
#include "curvegraphstyle.h"
#include "imagelayer.h"
#include "quoteformat.h"
#include <QPainter>

struct SmtTiletDataItem
{
    double lsye = DBL_MAX;
    double preLsye = DBL_MAX;
    double rzye =DBL_MAX;
    double preRzye =DBL_MAX;
    double preRqye=DBL_MAX;
    double rqye=DBL_MAX;
};

class SmtLsGraphTitle:public CurveGraphStyle
{
    Q_OBJECT
public:
    SmtLsGraphTitle(QWidget *parent = 0);

    ~SmtLsGraphTitle();

    void updateData(SmtTiletDataItem data);
protected:
    void setDcClr(HzQuoteFormat::QuoteItem item,QPainter& dc);
    QRect getRc();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void drawData();
    ImageLayer* m_imageLayer = nullptr;
    int m_layerId = 0;
    SmtTiletDataItem m_data;
};

#endif // SMTLSGRAPHTITLE_H
