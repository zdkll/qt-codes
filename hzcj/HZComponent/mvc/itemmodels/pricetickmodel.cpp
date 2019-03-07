#include "pricetickmodel.h"

#include "mvcpublicdef.h"

PriceTickModel::PriceTickModel(const QString &modelName,QObject *parent)
    :AbstractBaseModel(modelName,parent)
{

}


void PriceTickModel::initData(const HZData::PriceTickInfo& priceTickInfo,int index)
{
    this->beginResetModel();
    int totalCount = priceTickInfo.totalCount;
    m_priceTickInfo.totalCount = totalCount;
    m_priceTickInfo.volume = priceTickInfo.volume;
    m_priceTickInfo.max_volume = priceTickInfo.max_volume;
    m_priceTickInfo.pre_close_price = priceTickInfo.pre_close_price;

    m_priceTickInfo.priceTicks.resize(totalCount);
    int num = priceTickInfo.priceTicks.size();
    for(int i=0;i<num;i++){
        m_priceTickInfo.priceTicks[index+i] = priceTickInfo.priceTicks[i];
    }
    this->endResetModel();
}

void PriceTickModel::updateData(const HZData::PriceTickInfo& priceTickInfo,int index)
{
    int num = priceTickInfo.priceTicks.size();
    int totalCount = priceTickInfo.totalCount;
    m_priceTickInfo.volume = priceTickInfo.volume;
    m_priceTickInfo.max_volume = priceTickInfo.max_volume;
    m_priceTickInfo.pre_close_price = priceTickInfo.pre_close_price;

    //条目数改变
    if(m_priceTickInfo.totalCount != totalCount){

        int incNum = totalCount- m_priceTickInfo.totalCount;
        //增加
        if(incNum>0){
            this->beginInsertRows(QModelIndex(),m_priceTickInfo.totalCount,totalCount-1);
            m_priceTickInfo.totalCount = totalCount;
            m_priceTickInfo.priceTicks.resize(totalCount);
            for(int i=0;i<num;i++){
                m_priceTickInfo.priceTicks[index+i] = priceTickInfo.priceTicks[i];
            }
            this->endInsertRows();
        }else{ //变少一般是清盘
            this->beginRemoveRows(QModelIndex(),totalCount,m_priceTickInfo.totalCount-1);
            m_priceTickInfo.totalCount = totalCount;
            m_priceTickInfo.priceTicks.resize(totalCount);
            for(int i=0;i<num;i++){
                m_priceTickInfo.priceTicks[index+i] = priceTickInfo.priceTicks[i];
            }
            this->endRemoveRows();
        }
    }else{
        for(int i=0;i<num;i++){
            m_priceTickInfo.priceTicks[index+i] = priceTickInfo.priceTicks[i];
        }
    }
    //更新数据
    emit dataChanged(this->index(index,0),this->index(index+num-1,this->columnCount()-1));
}

void  PriceTickModel::clearData()
{
    if(m_priceTickInfo.totalCount<1) return;
    this->beginResetModel();
    m_priceTickInfo = HZData::PriceTickInfo();
    this->endResetModel();
}

int  PriceTickModel::rowCount(const QModelIndex & ) const
{
    return m_priceTickInfo.totalCount;
}


QVariant PriceTickModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    //三个统一值
    case Stock_Total_Volume_Role:
        return m_priceTickInfo.volume;
        break;
    case Stock_Max_Volume_Role:
        return m_priceTickInfo.max_volume;
        break;
    case Stock_Pre_Close_Price_Role:
        return m_priceTickInfo.pre_close_price;
        break;

    case Stock_Price_Role:
        return m_priceTickInfo.priceTicks[row].price;
        break;
    case Stock_Sell_Volume_Role:
        return m_priceTickInfo.priceTicks[row].sell_volume;
        break;
    case Stock_Buy_Volume_Role:
        return m_priceTickInfo.priceTicks[row].buy_volume;
        break;
    case Stock_Volume_Role:
        return m_priceTickInfo.priceTicks[row].volume;
        break;
    default:
        break;
    }
    return QVariant();
}

