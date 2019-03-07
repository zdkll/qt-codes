#ifndef HZUPDOWNGRAPH_H
#define HZUPDOWNGRAPH_H

#include <QWidget>
#include <QLabel>
#include <QPair>
#include <QFont>
#include <QPen>

#include <datactrldef.h>
#include "basewidget.h"

enum {
    Real_Time_Plate,            //实时涨停
    Sealing_Plate,              //封板率
    Yeaterday_Continuous_Plate, //昨日连板
    Yeaterday_Limit_Plate,      //昨日涨停
};

struct STUpDownGraphData
{
    struct PrivateData{
        QString itemName;
        qreal itemVal = 0.0;
    };
    int type;
    QString titleName;
    qreal titleValue = 0.0;
    QList<PrivateData> datas;
};

class HZUpDownGraph : public QLabel
{
    Q_OBJECT

public:
    explicit HZUpDownGraph(const STUpDownGraphData &data=STUpDownGraphData{},QWidget *parent = 0);
    ~HZUpDownGraph();

    void setGraphData(const STUpDownGraphData &data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    STUpDownGraphData mDatas;
    QFont mTitleFont, mTiltValFont, mItemTextFont, mItemValFont;
    //up down no (文本样式)
    QPen mUpPen{QColor("#e62727")};
    QPen mDownPen{QColor("#0fc351")};
    QPen mNoUpDown{QColor("#cad4dc")};
    QPen mTitlePen{QColor("#cad4dc")};
    QBrush mBkBr{QColor("#222222")};

    //最大存三条样式
    QVector<QPen> mItemPns;
    QVector<QBrush> mItemBrs;

    void drawArcGraph(QPainter *p);
    void drawHorizonRectGraph(QPainter *p);
};

class HZUpDownGraphMarster : public BaseWidget
{
    Q_OBJECT
public:
    HZUpDownGraphMarster(QWidget *parent=nullptr);
    ~HZUpDownGraphMarster(){}


    void afterActive();
    void afterDeactive();

protected:
    void paintEvent(QPaintEvent *event);

private:
    HZUpDownGraph *mGraphs[4];
    ReqId mReqId{-1};

    void startReadMonitorInfo();

    //涨停数据回调
    void receiverRfmInfo(ReqId id,HZData::RiseFallMonitorNumInfo info);
    void init();
};

#endif // HZUPDOWNGRAPH_H
