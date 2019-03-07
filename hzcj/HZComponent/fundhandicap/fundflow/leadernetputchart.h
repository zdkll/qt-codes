#ifndef LEADERNETPUTCHART_H
#define LEADERNETPUTCHART_H

#include "fundbasechart.h"
#include "fundstyleproperties.h"

class LeaderNetputChart :public FundBaseChart, public FundIncDecStyleProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

public:
    LeaderNetputChart(QWidget *parent = 0);

    void setData(const QVector<long long> &netputs);

    void clearData();

protected:
    void drawPixmap(QPixmap *pix);

    void paintVertLine(QPainter *pt);

private:
   NeputData   m_data[4];

   QFont          m_charFont,m_digitalFont;
};

#endif // LEADERNETPUTCHART_H
