#include "FundChangeModel.h"
#include <QVariant>

FundChangeModel::FundChangeModel(QObject *parent):
    QAbstractTableModel(parent)
{
//    m_fundData.strCode = "000001";
//    m_fundData.strName = "贵州茅台";
//    m_fundData.strTime = "11:00:00";
//    m_fundData.strType = "大笔买入";
}

int FundChangeModel::rowCount(const QModelIndex &parent) const
{
    return m_fundDataVec.size();
}

int FundChangeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant FundChangeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case MYDATAROLE:
        return QVariant::fromValue(m_fundDataVec[row]);
    default:
        break;
    }
    return QVariant();
}

void FundChangeModel::upDateData(const QVector<ShortMotion> &fundVec)
{
    translateData(fundVec);
}

void FundChangeModel::clearData()
{
    m_fundDataVec.clear();
    refrushModel();
}


void FundChangeModel::refrushModel()
{
    beginResetModel();
    endResetModel();
}

void FundChangeModel::translateData(const QVector<ShortMotion> &fundVec)
{

    m_fundDataVec.clear();
    for(int i = 0; i < fundVec.size(); i++)
    {
        FundData fundData;

        fundData.strCode = getText(fundVec[i], FieldCode);
        fundData.strName = getText(fundVec[i], FieldName);
        fundData.strTime = getText(fundVec[i], FieldTime);
        fundData.strType = textType(fundVec[i].short_type);
        if(fundData.strType == "--")
        {
           fundData.short_type = -1;
        }
        else
        {
            fundData.short_type = fundVec[i].short_type;
        }
        m_fundDataVec.append(fundData);
    }
    refrushModel();
}

QString FundChangeModel::textType(ShortMotionType type)
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

QString FundChangeModel::getText(const ShortMotion &Motion, Field field)
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

