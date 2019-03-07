#include "lhbuysellmodel.h"
#include "quoteformat.h"


LhBuySellModel::LhBuySellModel(QObject *parent, int nType):
    QAbstractTableModel(parent),
    m_nType(nType)
{
    if(0 == nType)
    {
    m_lists << "序号"
            <<  "买入前5营业部"
            <<  "买入额"
            <<  "卖出额"
            <<  "跟买成功率";
    }
    else
    {
        m_lists << "序号"
                <<  "卖出前5营业部"
                <<  "买入额"
                <<  "卖出额"
                <<  "跟买成功率";
    }
}

int LhBuySellModel::rowCount(const QModelIndex &parent) const
{
    return m_modeData.size();
}

int LhBuySellModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant LhBuySellModel::data(const QModelIndex &index, int role) const
{
    int nColumu = index.column();
    int nRow = index.row();
    switch (role) {
    case Qt::DisplayRole:
    {
        if(nColumu == 0)
        {
            return nRow+1;
        }
        else if(nColumu == 1)
        {
            return m_modeData[nRow].name;
        }
        else if(nColumu == 2)
        {
            return HzQuoteFormat::StringUnitFormat::toFormatString(m_modeData[nRow].buy*10000);
        }
        else if(nColumu == 3)
        {
            return HzQuoteFormat::StringUnitFormat::toFormatString(m_modeData[nRow].sell*10000);
        }
        else if(nColumu == 4)
        {
            return QString::number(m_modeData[nRow].ratio_3d, 'f', 2) + "%";
        }
    }
        break;
    case Qt::TextColorRole:
    {
        if(index.column() == 2)
        {
            return m_styledWidget.riseColor();
        }
        else if(index.column() == 3)
        {
            return m_styledWidget.fallColor();
        }
        else
        {
            return m_styledWidget.flatColor();
        }
    }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::FontRole:
    {
        QFont font;
        if(1 == index.column())
        {
            font.setBold(false);
            font.setFamily("Arial");
            font.setPixelSize(14);
        }
        else
        {
            font.setBold(true);
            font.setFamily("Arial");
            font.setPixelSize(13);
        }
        return font;
    }
    case LHYYBIDROLE:
        return m_modeData[nRow].id;
    default:
        break;
    }
    return QVariant();
}

QVariant LhBuySellModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return m_lists[section];
        break;
    default:
        break;
    }
    return QVariant();
}

void LhBuySellModel::updateLhData(const StockLhbDetialListItem &data)
{
    if(data.reason.isEmpty())
        return;
    if(0 == m_nType)
    {
        m_modeData = data.top5_buy;
    }
    else
    {
        m_modeData = data.top5_sell;
    }

    refrushModel();
}

void LhBuySellModel::refrushModel()
{
    beginResetModel();
    endResetModel();
}
