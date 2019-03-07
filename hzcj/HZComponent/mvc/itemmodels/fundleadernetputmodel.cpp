#include "fundleadernetputmodel.h"

FundLeaderNetputModel::FundLeaderNetputModel(const QString &modelName,QObject *parent)
    :AbstractBaseModel(modelName,parent)
{

}

void FundLeaderNetputModel::initData(const QVector<HZData::HZTick> &data)
{
    this->beginResetModel();
    m_ticks = data;
    this->endResetModel();
}

void FundLeaderNetputModel::clearData()
{
    if(m_ticks.size()<1) return;
    this->beginResetModel();
    m_ticks.clear();
    this->endResetModel();
}

int FundLeaderNetputModel::rowCount(const QModelIndex &) const
{
    return m_ticks.size();
}


QVariant FundLeaderNetputModel::data(const QModelIndex &index, int role) const
{
    int row       = index.row();
    int column = index.column();

    if(role == LeaderNetput_Data_Role ){
        switch (column) {
        case 0:
            return  m_ticks[row].update_time;
            break;
        case 1:
            return  m_ticks[row].turnover;
            break;
        case 2:
            return  m_ticks[row].volume;
            break;
        case 3:
            return  m_ticks[row].price;
            break;
        default:
            break;
        }
    }else if(role == LeaderNetput_Direct_Role)
        return  m_ticks[row].access_way;

    return QVariant();
}
