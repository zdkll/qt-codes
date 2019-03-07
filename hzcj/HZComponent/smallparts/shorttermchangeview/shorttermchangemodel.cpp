#include "ShortTermChangeModel.h"
#include <QVariant>
#include "quoteformat.h"

ShortTermChangeModel::ShortTermChangeModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

int ShortTermChangeModel::rowCount(const QModelIndex &parent) const
{
    return m_shortTermDataVec.size();
}

int ShortTermChangeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ShortTermChangeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case MYDATAROLE:
        return QVariant::fromValue(m_shortTermDataVec[row]);
    default:
        break;
    }
    return QVariant();
}

void ShortTermChangeModel::upDateData(const QVector<ShortMotion> &fundVec)
{
    translateData(fundVec);
}

void ShortTermChangeModel::clearData()
{
    m_shortTermDataVec.clear();
    refrushModel();
}


void ShortTermChangeModel::refrushModel()
{
    beginResetModel();
    endResetModel();
}

void ShortTermChangeModel::translateData(const QVector<ShortMotion> &fundVec)
{

    m_shortTermDataVec.clear();
    for(int i = 0; i < fundVec.size(); i++)
    {
        ShortTermData shortTermData;

        shortTermData.strCode = getText(fundVec[i], FieldCode);
        shortTermData.strName = getText(fundVec[i], FieldName);
        shortTermData.strTime = getText(fundVec[i], FieldTime);
        shortTermData.strType = textType(fundVec[i].short_type);
        if(shortTermData.strType == "--")
        {
           shortTermData.short_type = -1;
        }
        else
        {
            shortTermData.short_type = fundVec[i].short_type;
        }

        shortTermData.strVal = getStrVal(fundVec[i]);

        m_shortTermDataVec.append(shortTermData);
    }
    refrushModel();
}

QString ShortTermChangeModel::textType(ShortMotionType type)
{
    switch (type) {
    case OrganizationEat:
        return "机构吃货";
        break;
    case LargeBid:
        return "有大买盘";
        break;
    case RocketLaunch:
        return "火箭发射";
        break;
    case RapidRebound:
        return "快速反弹";
        break;
    case HighDiving:
        return "高台跳水";
        break;
    case AccelerateDecline:
        return "加速下跌";
        break;
    case LargeBuy:
        return "大笔买入";
        break;
    case LargeSell:
        return "大笔卖出";
        break;
    case LargeAsk:
        return "有大卖盘";
        break;
    case UpperLimit:
        return "封涨停板";
        break;
    case DownLimit:
        return "封跌停板";
        break;
    case OpenUpperLimit:
        return "打开涨停";
        break;
    case OpenDownLimit:
        return "打开跌停";
        break;
    case OrganizationBuy:
        return "机构买单";
        break;
    case OrganizationSell:
        return "机构卖单";
        break;
    case OrganizationSpit:
        return "机构吐货";
        break;
    case BuyNew:
        return "买入新单";
        break;
    case SellNew:
        return "卖出新单";
        break;
    default:
        break;
    }
    return "--";
}

QString ShortTermChangeModel::getText(const ShortMotion &Motion, Field field)
{
    QString str = "--";
    switch (field) {
    case FieldCode:
    {
        if(QString::fromStdString(Motion.stock_code).isEmpty())
        {
            return str;
        }
        else
        {
            return QString::fromStdString(Motion.stock_code);
        }

    }
        break;
    case FieldName:
        if(QString::fromStdString(Motion.stock_name).isEmpty())
        {
            return str;
        }
        else
        {
            return QString::fromStdString(Motion.stock_name);
        }
        break;
    case FieldTime:
        if(Motion.update_time_sec == INVALID_INT64)
        {
            return str;
        }
        else
        {
            return QDateTime::fromTime_t(Motion.update_time_sec).toString("hh:mm:ss");
        }
        break;
    default:
        break;
    }
    return str;
}

QString ShortTermChangeModel::getStrVal(const ShortMotion &shortMotion)
{
    QString str = "--";
    switch (shortMotion.short_type) {
    case OrganizationEat:       //机构吃货
    case LargeBid:      //有大买盘
    case OrganizationBuy:   //机构买单
    case BuyNew:    //买入新单
    {
        if(INVALID_INT64 == shortMotion.buy_volume)
            break;

        str = HzQuoteFormat::StringUnitFormat::toFormatString(shortMotion.buy_volume / 100) + QStringLiteral("手") ;
//        str = QString::number(shortMotion.buy_volume / 100) + QStringLiteral("手") ;
    }
    break;

    case LargeAsk:      //有大卖盘
    case OrganizationSell:  //机构卖单
    case OrganizationSpit:  //机构吐货
    case SellNew:   //卖出新单
    {
        if(INVALID_INT64 == shortMotion.sell_volume)
            break;
        str = HzQuoteFormat::StringUnitFormat::toFormatString(shortMotion.sell_volume / 100) + QStringLiteral("手") ;
//        str = QString::number(shortMotion.sell_volume / 100) + QStringLiteral("手") ;
    }
        break;

    case LargeBuy:      //大笔买入
    case LargeSell:     //大笔卖出
    {
        if(INVALID_INT64 == shortMotion.detail_vol)
            break;

        str = HzQuoteFormat::StringUnitFormat::toFormatString(shortMotion.detail_vol / 100) + QStringLiteral("手") ;
//        str = QString::number(shortMotion.detail_vol / 100) + QStringLiteral("手") ;
    }
    break;

    case RocketLaunch:  //火箭发射
    case RapidRebound:  //快速反弹
    case HighDiving:    //高台跳水
    case AccelerateDecline: //加速下跌
    {
        if(INVALID_DOUBLE == shortMotion.rate)
            break;
        str = QString::number(shortMotion.rate *100, 'f', 2) + "%" ;
    }
    break;

    case UpperLimit:    //打开涨停
    case DownLimit:     //封跌停板
    case OpenUpperLimit:    //打开涨停
    case OpenDownLimit:     //打开跌停
    {
        if(INVALID_DOUBLE == shortMotion.last_price)
            break;
        str = QString::number(shortMotion.last_price, 'f', 2) + QStringLiteral("元");
    }
    break;
    default:
        break;
    }
    return str;
}

