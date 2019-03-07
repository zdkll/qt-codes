/********************************************************************
created:
author: dk.zhang
purpose:资金流向条形图
*********************************************************************/
#ifndef FUNDFLOWBARCHART_H
#define FUNDFLOWBARCHART_H

#include <QWidget>
#include "quoteformat.h"
#include "fundbasechart.h"

#include "fundstyleproperties.h"

class FundFlowBarChart : public FundBaseChart,public FundBarStyleProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor inColor READ inColor WRITE setInColor)
    Q_PROPERTY(QColor outColor READ outColor WRITE setOutColor)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)

public:
    FundFlowBarChart(QWidget *parent = 0);

    void setData(FundInOutData *data);

protected:
    virtual void drawPixmap(QPixmap *pix);

    //绘制条形图
    void drawBar(QPainter *pt,int width,int height ,int space);

    //绘制表格
    void drawTableView(QPainter *pt,int width,int height);

private:
    void initData();
    void updateData(FundInOutData *data);

    void calFunItem(FundItem &item,long long value,long long max,HzQuoteFormat::UnitType type);

    FundInOutData *m_data;
    //条形图数据
    FundItem            m_barItems[3];
    //表格数据
    FundItem            m_tableItems[4];


    QFont                m_charFont,m_digitalFont;
};

#endif // FUNDFLOWBARCHART_H
