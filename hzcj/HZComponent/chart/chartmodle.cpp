#include "chartmodle.h"
#include <qvector.h>
#include <qdatetime.h>
#include <hzlog.h>

#include "httpdata.h"

ChartModle *ChartModle::getInstance()
{
    static ChartModle instance;
    return &instance;
}

ChartModle::~ChartModle()
{
}

QString ChartModle::getStrPeriod(int period)
{
    QString strType = "";
    switch (period) {
    case HZData::KLINE_PERIOD_MIN:
        strType = QStringLiteral("1分钟");
        break;
    case HZData::KLINE_PERIOD_5MIN:
        strType = QStringLiteral("5分钟");
        break;
    case HZData::KLINE_PERIOD_15MIN:
        strType = QStringLiteral("15分钟");
        break;
    case HZData::KLINE_PERIOD_HOUR:
        strType = QStringLiteral("60分钟");
        break;
    case HZData::KLINE_PERIOD_DAY:
        strType = QStringLiteral("日K");
        break;
    case HZData::KLINE_PERIOD_WEEK:
        strType = QStringLiteral("周K");
        break;
    case HZData::KLINE_PERIOD_MONTH:
        strType = QStringLiteral("月K");
        break;
//    case eSeason:
//        strType = QStringLiteral("季度K");
//        break;
//    case eHalfYear:
//        strType = QStringLiteral("半年K");
//        break;
//    case eYear:
//        strType = QStringLiteral("年K");
//        break;
    default:
        break;
    }
     return strType;
}

QString ChartModle::getObjNameById(const QString &obj)
{
    //return QStringLiteral("浦发银行");
    return obj;
}

QVector<QString> ChartModle::getMinXaxisStrTimeList(const QString &obj)
{
    QVector<QString> tradeNode;
    tradeNode.push_back("10:30");
    tradeNode.push_back("11:30");
    tradeNode.push_back("14:00");
    tradeNode.push_back("15:00");
    return tradeNode;
}

int ChartModle::getMinDayCount(const QString &obj)
{
    return 241;
}

void ChartModle::setDrData(const QString &stock, const StockDrList &data)
{
    m_drDataMap[stock] = data;
}

StockDrList ChartModle::getDrData(const QString &stock)
{
    StockDrList ret;
    if(m_drDataMap.contains(stock))
        ret = m_drDataMap[stock];
    return ret;
}

QVector<double> ChartModle::getTradeNode(const QString& obj)
{
    QVector<double> tradeNode;
    tradeNode.push_back(34200);
    tradeNode.push_back(41400);
    tradeNode.push_back(46800);
    tradeNode.push_back(54000);
    return tradeNode;
}

QString ChartModle::getKlinekey(const QString &obj, int period)
{
    return obj +  "_" + QString::number(period);
}

ChartModle::ChartModle()
{
}

QString formatTime1(quint32 t)
{
    QDateTime nt;
    nt.setTime_t(t);
    return nt.toString("yyyy-MM-dd hh:mm");
}
