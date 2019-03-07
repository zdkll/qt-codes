/********************************************************************
created:
author: dk.zhang
purpose:资金流向饼状图
*********************************************************************/
#ifndef FUNDFLOWPIECHART_H
#define FUNDFLOWPIECHART_H

#include <QWidget>

#include "fundbasechart.h"
#include "fundstyleproperties.h"

class QLabel;
class FundFlowPieChart : public FundBaseChart, public FundPieStyleProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor)
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)
    Q_PROPERTY(QColor inColor READ inColor WRITE setInColor)
    Q_PROPERTY(QColor outColor READ outColor WRITE setOutColor)
    Q_PROPERTY(QColor percentColor READ percentColor WRITE setPercentColor)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)

public:
    FundFlowPieChart(QWidget *parent = 0);

    void setData(FundInOutData *data);

protected:
    virtual void drawPixmap(QPixmap *pix);

    //绘制主力净流入
    void drawLeaderNetput(QPainter *pt);

    void drawInOut(QPainter *pt);

    void showTip(const QPoint &pt);

    void mouseMoveEvent(QMouseEvent *e);

private:
    void updateData(FundInOutData *data);

    FundInOutData *m_data;

    FundItem           m_pieItems[8];
    QFont                m_titleFont;
    QFont                m_titleNumFont;
    QFont                m_pieCharFont;

    QPoint               m_pieCenter;
    bool                   m_showTip;
};

#endif // FUNDFLOWPIECHART_H
