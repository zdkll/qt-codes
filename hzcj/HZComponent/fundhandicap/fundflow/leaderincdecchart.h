/********************************************************************
created:
author: dk.zhang
purpose:30 日主力增减仓
*********************************************************************/
#ifndef LEADERINCDECCHART_H
#define LEADERINCDECCHART_H

#include <QWidget>

#include "datamodeldef.h"
#include "fundbasechart.h"

#include "fundstyleproperties.h"

class LeaderIncDecChart : public FundBaseChart,public FundIncDecStyleProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)
    Q_PROPERTY(QColor riseMaxColor READ riseMaxColor WRITE setRiseMaxColor)
    Q_PROPERTY(QColor fallMaxColor READ fallMaxColor WRITE setFallMaxColor)
    Q_PROPERTY(QColor midColor READ midColor WRITE setMidColor)

public:
    LeaderIncDecChart(QWidget *parent = 0);
   ~LeaderIncDecChart();

    //范围单位
    QString setData(const HZData::IncrDecPostionInfo& data);
    void       clearData();

protected:
    virtual void drawPixmap(QPixmap *pixmap);

private:
    void paintGrap(QPainter *pt,int width,int height);
    QPixmap createLinePix(int width,int height);

    void paintDate(QPainter *pt,int width,int height);

private:
    IncrDecPosData   *m_d;
    QFont          m_font;
    //差值数据
    double        *m_interData;
};

#endif // LEADERINCDECCHART_H
