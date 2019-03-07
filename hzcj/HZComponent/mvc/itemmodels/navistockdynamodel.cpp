#include "navistockdynamodel.h"
#include <QMessageBox>

///NaviStockDynaModel----

void NaviStockDynaModel::initData(const char *data,int num)
{
    HZData::StockCode *stockCodes = (HZData::StockCode *)data;
    beginResetModel();
    m_slStocks.clear();
    m_slStocks.resize(num);
    for(int i=0;i<num;i++){
        m_slStocks[i].stock_code = stockCodes[i].stock_code;
        m_slStocks[i].stock_name = stockCodes[i].stock_name;
    }
    endResetModel();
}

void NaviStockDynaModel::initData(const QVector<HZData::StockCode>& stockCodes)
{
    beginResetModel();
    m_stkMap.clear();
    m_slStocks.clear();
    m_slStocks.resize(stockCodes.size());
    for(int i=0;i<stockCodes.size();i++){
        m_slStocks[i].stock_code = stockCodes[i].stock_code;
        m_slStocks[i].stock_name = stockCodes[i].stock_name;
    }
    endResetModel();
}

void NaviStockDynaModel::updateData(const char *data,int start,int count,bool first)
{
    HZData::SLStock *slStocks = (HZData::SLStock *)data;
    if(first){
        for(int i=0;i<count;i++){
            m_slStocks.replace(start+i,slStocks[i]);
            m_stkMap.emplace(slStocks[i].stock_code,start+i);
        }
    }else{
        for(int i=0;i<count;i++){
            auto it = m_stkMap.find(slStocks[i].stock_code);
            if(it != m_stkMap.end())
                m_slStocks.replace(it->second,slStocks[i]);
        }
    }
    if(count>0)
        emit dataChanged(index(start,0),index(start+count-1,0));
}

int NaviStockDynaModel::rowCount(const QModelIndex &parent ) const
{
    return m_slStocks.size();
}

QVariant NaviStockDynaModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case Stock_No_Role:
        return row;
        break;
    case Stock_Name_Role:
        return QString::fromStdString(m_slStocks[row].stock_name);
        break;
    case Stock_Code_Role:
        return QString::fromStdString(m_slStocks[row].stock_code);
        break;
    case Stock_LastPrice_Role:
        return m_slStocks[row].last_price;
        break;
    case Stock_UpDownPrice_Role:
        return m_slStocks[row].up_down_price;
        break;
    case Stock_Rate_Role:
        return m_slStocks[row].rate;
        break;
    default:
        break;
    }
    return QVariant();
}


///OptionNaviStockDynaModel--
void OptionNaviStockDynaModel::initData(const QVector<HZData::StockCode>& stockCodes)
{
    beginResetModel();
    m_stkMap.clear();
    m_slStocks.clear();
    m_slStocks.resize(stockCodes.size());
    for(int i=0;i<stockCodes.size();i++){
        m_slStocks[i].stock_code = stockCodes[i].stock_code;
        m_slStocks[i].stock_name = stockCodes[i].stock_name;
        m_stkMap.emplace(m_slStocks[i].stock_code,i);
    }
    endResetModel();
}

void OptionNaviStockDynaModel::updateData(const char *data,int start,int count,bool first)
{
    HZData::SLStock *slStocks = (HZData::SLStock *)data;

    for(int i=0;i<count;i++){
        auto it = m_stkMap.find(slStocks[i].stock_code);
        if(it != m_stkMap.end())
            m_slStocks.replace(it->second,slStocks[i]);
    }

    if(count>0)
        emit dataChanged(index(start,0),index(start+count-1,0));
}

