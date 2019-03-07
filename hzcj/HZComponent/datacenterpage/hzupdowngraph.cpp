#include "hzupdowngraph.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QtMath>

#include <datamodeldefs/stockinfodef.h>
#include <datacontroller.h>

#define xOffset     12
#define yOffset     12
#define itemSpace   20

static int offsetX = 40;
static int offsetY = 28;
#define PI 3.1415926535


///
/// \brief HZUpDownGraph::HZUpDownGraph
/// \param parent
///
HZUpDownGraph::HZUpDownGraph(const STUpDownGraphData &data,QWidget *parent) :
    QLabel(parent),mDatas(data)
{
//    setFixedHeight(229);
    setContentsMargins(0,0,0,0);
    setMargin(0);

    mTitleFont.setFamily("SimSun");
    mTitleFont.setPixelSize(16);
    mTitleFont.setBold(true);

    mTiltValFont.setFamily("Arial-Bold");
    mTiltValFont.setPixelSize(24);
    mTiltValFont.setBold(true);
    //mTiltValFont.setStyleStrategy(QFont::NoAntialias);

    mItemTextFont.setFamily("SimSun");
    mItemTextFont.setPixelSize(14);

    mItemValFont.setFamily("Arial-Bold");
    mItemValFont.setPixelSize(13);
    mItemValFont.setStyleStrategy(QFont::NoAntialias);

    QPen mFirstItemPn{QColor("#4f9bff")}, mSecondItemPn{QColor("#ff9600")}, mThreeItemPn{QColor("#00a2ff")};
    QBrush mFirstItemBr{QColor("#0b284b")}, mSecondItemBr{QColor("#a45607")}, mThreeItemBr{QColor("#145797")};
    mItemPns.push_back(mFirstItemPn);
    mItemPns.push_back(mSecondItemPn);
    mItemPns.push_back(mThreeItemPn);
    mItemBrs.push_back(mFirstItemBr);
    mItemBrs.push_back(mSecondItemBr);
    mItemBrs.push_back(mThreeItemBr);
}

HZUpDownGraph::~HZUpDownGraph()
{

}

void HZUpDownGraph::setGraphData(const STUpDownGraphData &data)
{
    mDatas = data;
    update();
}

static int getSumValue(const QList<STUpDownGraphData::PrivateData> &vals)
{
    int sum = 0;
    for(int i=0; i<vals.count(); ++i) sum += qAbs(vals[i].itemVal);
    return sum;
}

static int getMax(const QList<STUpDownGraphData::PrivateData> &vals)
{
    int max = INT_MIN;
    for(int i=0; i<vals.count(); ++i)
        max = qMax((int)vals[i].itemVal,max);
    return max;
}

static int getMin(const QList<STUpDownGraphData::PrivateData> &vals)
{
    int min = INT_MAX;
    for(int i=0; i<vals.count(); ++i)
        min = qMin((int)vals[i].itemVal,min);
    return min;
}

void HZUpDownGraph::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
#if 0
    drawArcGraph(&p);
#else
    drawHorizonRectGraph(&p);
#endif
}

void HZUpDownGraph::drawArcGraph(QPainter *p)
{
    p->fillRect(rect().adjusted(10,20,-20,-20),QColor("#222222"));

    //draw title (先计算最大的)
    p->setFont(mTitleFont);
    int titleH = p->fontMetrics().height();
    QRect titleRt(rect().x()+offsetX, rect().y()+offsetY, width(), titleH), tmpRt(titleRt);
    int valPos = p->fontMetrics().width(mDatas.titleName)+titleRt.x()+10;
    titleRt.setX(valPos);
    p->setFont(mTiltValFont);
    titleH = p->fontMetrics().height();
    titleRt.setHeight(titleH);
    p->setPen(mDatas.titleValue>0.?mUpPen:mDatas.titleValue<0.?mDownPen:mNoUpDown);
    QString tmpVal="";
    int tmpTy = mDatas.type;
    if(tmpTy == Sealing_Plate || tmpTy == Yeaterday_Continuous_Plate || tmpTy == Yeaterday_Limit_Plate)
    {
        tmpVal.append(QString::number(mDatas.titleValue,'f',2)+"%");
    }/* else if()
    {
        tmpVal.prepend(QString::number(mDatas.titleValue,'f',2));
    } */else
    {
        tmpVal = QString::number(mDatas.titleValue);
    }

    p->drawText(titleRt, Qt::AlignLeft|Qt::AlignVCenter, tmpVal);

    tmpRt.setHeight(titleRt.height());
    p->setFont(mTitleFont);
    p->setPen(mTitlePen);
    p->drawText(tmpRt, Qt::AlignLeft|Qt::AlignVCenter, mDatas.titleName);

    if(!mDatas.datas.isEmpty()){
        qreal xPos = 142., yPos = 60., radius = 75., dis=7., endAngle = 0., forward = 90., startPos = 90;
        int tw =width()-xPos-40, th=height()-yPos-40;
        int minRadius = qMin(tw,th);
        QRect ellipseRt(xPos, yPos, minRadius, minRadius);
        int sum = getSumValue(mDatas.datas);
        qreal val = qAbs(mDatas.datas[0].itemVal) * 1.0;
        endAngle = val/sum *360. + startPos;

        p->setPen(mItemPns[0]);
        p->setBrush(mItemBrs[0]);
        for(int i=1; i<mDatas.datas.count(); ++i) {
            qreal averAngle=0.;
            QRectF newRect;
            if(val != 0) {
                averAngle = (forward + endAngle)/2.;
                newRect = ellipseRt.adjusted(dis*cos(averAngle*PI/180.), -dis*sin(averAngle*PI/180.),
                                             dis*cos(averAngle*PI/180.),-dis*sin(averAngle*PI/180.));
                p->drawPie(newRect, forward*16, (endAngle-forward)*16);
                forward = endAngle;
            }

            val = qAbs(mDatas.datas[i].itemVal);
            if(val != 0) {
                endAngle += val/sum *360.;
                p->setPen(mItemPns[i]);
                p->setBrush(mItemBrs[i]);
                averAngle = (forward + endAngle)/2.;
                newRect = ellipseRt.adjusted(dis*cos(averAngle*PI/180.), -dis*sin(averAngle*PI/180.),
                                             dis*cos(averAngle*PI/180.),-dis*sin(averAngle*PI/180.));
                p->drawPie(newRect, forward*16, (endAngle-forward)*16);
            }
        }
    }

    //left bar
    int itemH = 50;
    p->setFont(mItemTextFont);
    for(int i=0; i<mDatas.datas.count(); ++i){
        int xPos = offsetX;
        QRect rootItem(xPos, 60+i*itemH, width(), itemH);
        QRect rt(rootItem.x(), rootItem.y()+(rootItem.height()-8)/2,  8, 8);
        p->setPen(mItemPns[i]);
        p->setBrush(mItemBrs[i]);
        p->drawRect(rt);

        p->setFont(mItemTextFont);
        p->setPen(mTitlePen);
        xPos += 18;

        QRect itemTx(rootItem);
        itemTx.setX(xPos);
        p->drawText(itemTx, Qt::AlignLeft|Qt::AlignVCenter, mDatas.datas[i].itemName);

        p->setFont(mItemValFont);
        int itemTxW = p->fontMetrics().width(mDatas.datas[i].itemName);
        xPos += (itemTxW + 11);
        itemTx.setX(xPos);
        if(mDatas.type == Yeaterday_Limit_Plate || mDatas.type == Yeaterday_Continuous_Plate)
            if(i==0)
                p->drawText(itemTx, Qt::AlignLeft|Qt::AlignVCenter, QString::number(mDatas.datas[i].itemVal,'f',2)+"%");
            else
                p->drawText(itemTx, Qt::AlignLeft|Qt::AlignVCenter, QString::number(mDatas.datas[i].itemVal,'f',2));
        else
            p->drawText(itemTx, Qt::AlignLeft|Qt::AlignVCenter, QString::number(mDatas.datas[i].itemVal));
    }
}

void HZUpDownGraph::drawHorizonRectGraph(QPainter *p)
{
    QRect rlRt(rect().adjusted(xOffset, yOffset, -xOffset/4, -yOffset));
    p->fillRect(rlRt,QColor("#222222"));
    p->setFont(mTitleFont);

    QPoint ltPt(rlRt.x()+20,27+p->fontMetrics().height());
    p->setPen(mTitlePen);
    p->drawText(ltPt, mDatas.titleName);

    int flagPos = p->fontMetrics().width(mDatas.titleName);
    flagPos += ltPt.x();

    //绘制title
    int tmpTy = mDatas.type;
    QString tmpVal="";
    if(tmpTy == Sealing_Plate || tmpTy == Yeaterday_Continuous_Plate || tmpTy == Yeaterday_Limit_Plate)
    {
        tmpVal.append(QString::number(mDatas.titleValue,'f',2)+"%");
    }/* else if()
    {
        tmpVal.prepend(QString::number(mDatas.titleValue,'f',2));
    } */else
    {
        tmpVal = QString::number(mDatas.titleValue);
    }

    p->setPen(mDatas.titleValue>0.? mUpPen:mDatas.titleValue<0.? mDownPen:mNoUpDown);
    p->drawText(QPoint(flagPos+10, ltPt.y()), tmpVal);
    ltPt += QPoint(0,p->fontMetrics().height()+itemSpace);

    //文本项和水平柱状图一起画
    for(int k=0; k<mDatas.datas.count(); ++k)
    {
        p->setFont(mItemTextFont);
        p->setPen(mTitlePen);
        p->drawText(ltPt, mDatas.datas[k].itemName);

        p->setFont(mItemValFont);
        flagPos = p->fontMetrics().width(mDatas.datas[k].itemName)+ltPt.x();

        if(mDatas.type == Yeaterday_Limit_Plate || mDatas.type == Yeaterday_Continuous_Plate)
            if(k==0)
                p->drawText(QPoint(flagPos+10, ltPt.y()), QString::number(mDatas.datas[k].itemVal,'f',2)+"%");
            else
                p->drawText(QPoint(flagPos+10, ltPt.y()), QString::number(mDatas.datas[k].itemVal,'f',2));
        else
            p->drawText(QPoint(flagPos+10, ltPt.y()), QString::number(mDatas.datas[k].itemVal));

        ltPt += QPoint(0,10);

        //画横向矩形
        int itemMax = getMax(mDatas.datas);
        if(itemMax == 0)
        {
            ltPt += QPoint(0, itemSpace*2+10);
            continue;
        }

        double pixwdt(rlRt.width()-xOffset*3);
        double radio = pixwdt / itemMax;
        p->setPen(mItemPns[k]);
        p->setBrush(mItemBrs[k]);
        p->drawRect(QRect(ltPt.x(), ltPt.y(), radio*mDatas.datas[k].itemVal, itemSpace));
        ltPt += QPoint(0, itemSpace*2+10);
    }
}

///
/// \brief HZUpDownGraphMarster::HZUpDownGraphMarster
/// \param parent
///
HZUpDownGraphMarster::HZUpDownGraphMarster(QWidget *parent)
    :BaseWidget(parent)
{
    QHBoxLayout *hl = new QHBoxLayout;
    hl->setContentsMargins(0,0,6,0);
    hl->setSpacing(0);
    setLayout(hl);

    mGraphs[0] = new HZUpDownGraph();
    hl->addWidget(mGraphs[0]);

    mGraphs[1] = new HZUpDownGraph();
    hl->addWidget(mGraphs[1]);

    mGraphs[2] = new HZUpDownGraph();
    hl->addWidget(mGraphs[2]);

    mGraphs[3] = new HZUpDownGraph();
    hl->addWidget(mGraphs[3]);

    HZData::RiseFallMonitorNumInfo monitor;
    monitor.real_time_num = -1;
    receiverRfmInfo(0, monitor);
}

void HZUpDownGraphMarster::afterActive()
{
    qDebug() << Q_FUNC_INFO;
    startReadMonitorInfo();
}

void HZUpDownGraphMarster::afterDeactive()
{
    qDebug() << Q_FUNC_INFO;
    if(-1 == mReqId) return;
    DataController::instance()->cancelSub(mReqId);
}

void HZUpDownGraphMarster::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(rect(),QColor("#191919"));
}

void HZUpDownGraphMarster::startReadMonitorInfo()
{
    auto fun = std::bind(&HZUpDownGraphMarster::receiverRfmInfo, this, std::placeholders::_1, std::placeholders::_2);
    mReqId = DataController::instance()->subRfmNumInfo(fun);
}

/*!
 * \brief HZUpDownGraphMarster::receiverRfmInfo
 * \param id
 * \param info
 *
 * Real_Time_Plate 自然 一字 连板
 * Sealing_Plate 涨停 咋板
 * Yeaterday_Continuous_Plate 离开率 获利率
 * Yeaterday_Limit_Plate 离开率 获利率
 *
 */
void HZUpDownGraphMarster::receiverRfmInfo(ReqId id, HZData::RiseFallMonitorNumInfo monitor)
{
    HZData::RiseFallMonitorNumInfo info;
    if(monitor.real_time_num != -1) {
        info = monitor;
    }

#if 1
    qDebug() << Q_FUNC_INFO << "id:" << id << info.aword_plate
             << info.real_time_num
             <<  info.natural_num
              <<  info.continuous_num
               << info.aword_plate
                <<  info.sealing_plate_rate
                 <<  info.fried_plate
                  <<  info.yeaterday_limit_rate
                   <<  info.yeaterday_high_open_rate
                    << info.yeaterday_profit_rate
                      <<  info.yeaterday_continuous_limit_rate
                       << info.yeaterday_continuous_high_open_rate
                       <<  info.yeaterday_continuous_profit_rate
                        <<  info.update_time_sec
                         << info.trading_day  ;
#endif


    STUpDownGraphData dt;
    QList<STUpDownGraphData::PrivateData> items;
    STUpDownGraphData::PrivateData item;
    item.itemName = QStringLiteral("自然");
    item.itemVal = info.natural_num==INVALID_UINT32?0:info.natural_num;
    items.push_back(item);
    item.itemName = QStringLiteral("一字");
    item.itemVal = info.aword_plate==INVALID_UINT32?0:info.aword_plate;
    items.push_back(item);
    item.itemName = QStringLiteral("连板");
    item.itemVal = info.continuous_num==INVALID_UINT32?0:info.continuous_num;
    items.push_back(item);
    dt.datas = items;
    dt.type = Real_Time_Plate;
    dt.titleName = QStringLiteral("实时涨停");
    dt.titleValue = info.real_time_num==INVALID_INT32?0:info.real_time_num;
    mGraphs[0]->setGraphData(dt);

    items.clear();
    item.itemName = QStringLiteral("涨停");
    item.itemVal = info.real_time_num==INVALID_INT32?0:info.real_time_num;
    items.push_back(item);
    item.itemName = QStringLiteral("炸板");
    item.itemVal = info.fried_plate==INVALID_UINT32?0:info.fried_plate;
    items.push_back(item);
    dt.datas = items;
    dt.type = Sealing_Plate;
    dt.titleName = QStringLiteral("封板率");
    dt.titleValue = info.sealing_plate_rate==INVALID_FLOAT?0:info.sealing_plate_rate*100;
    mGraphs[1]->setGraphData(dt);

    items.clear();
    item.itemName = QStringLiteral("高开率");
    item.itemVal = info.yeaterday_continuous_high_open_rate==INVALID_FLOAT?0:info.yeaterday_continuous_high_open_rate*100;
    items.push_back(item);
    item.itemName = QStringLiteral("获利额(元/股) ");
    item.itemVal = info.yeaterday_continuous_profit_rate==INVALID_FLOAT?0:info.yeaterday_continuous_profit_rate;
    items.push_back(item);
    dt.datas = items;
    dt.type = Yeaterday_Continuous_Plate;
    dt.titleName = QStringLiteral("昨日连板");
    dt.titleValue = info.yeaterday_continuous_limit_rate==INVALID_FLOAT?0:info.yeaterday_continuous_limit_rate*100;
    mGraphs[2]->setGraphData(dt);

    items.clear();
    item.itemName = QStringLiteral("高开率");
    item.itemVal = info.yeaterday_high_open_rate==INVALID_FLOAT?0:info.yeaterday_high_open_rate*100;
    items.push_back(item);
    item.itemName = QStringLiteral("获利额(元/股) ");
    item.itemVal = info.yeaterday_profit_rate==INVALID_FLOAT?0:info.yeaterday_profit_rate;
    items.push_back(item);
    dt.datas = items;
    dt.type = Yeaterday_Limit_Plate;
    dt.titleName = QStringLiteral("昨日涨停");
    dt.titleValue = info.yeaterday_limit_rate==INVALID_FLOAT?0:info.yeaterday_limit_rate*100;//yeaterday_limit_rate
    mGraphs[3]->setGraphData(dt);
}
