#include "databasemodel.h"

AbstractDataBaseModel::AbstractDataBaseModel(const QString &modelName,QObject *parent)
    :AbstractBaseModel(modelName,parent)
{

}



///BiddingItemModel---
void BiddingItemModel::initData(const HZData::BiddingInfo &biddingInfo)
{
    this->beginResetModel();
    m_totalCount = biddingInfo.total_cnt;
    m_stockBids.clear();
    m_stockBids = std::move(biddingInfo.stk);
    //先保存后扩展长度
    m_stockBids.resize(m_totalCount);
    this->endResetModel();
}

void BiddingItemModel::updateData(const HZData::BiddingInfo &biddingInfo,int start)
{
    const int num = biddingInfo.stk.size();

    for(int i=0;i<num;i++)
        m_stockBids[start+i]  = biddingInfo.stk[i] ;

    emit dataChanged(index(start,1),index(start+num-1,this->columnCount()-1));
}

void BiddingItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_stockBids.clear();
    this->endResetModel();
}

QVariant BiddingItemModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    ColumnIdx idx = this->columnIdx(index.column());
    if(role == Stock_Data_Role)
    {
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColStockCode:
            return QString::fromStdString(m_stockBids[row].stock_code);
            break;
        case ColStockName:
            return QString::fromStdString(m_stockBids[row].stock_name);
            break;
        case ColBlockName:
            return QString::fromStdString(m_stockBids[row].plate);
            break;
        case ColRatePrice:
            return m_stockBids[row].last_price;
            break;
        case ColRate:
            return m_stockBids[row].rate;
            break;
        case ColAuctionRate:
            return m_stockBids[row].bidding_rise;
            break;
        case ColLeaderFund:
            return m_stockBids[row].main_funds;
            break;
        case ColAutionQaunRatio:
            return m_stockBids[row].bidding_quantity_than;
            break;
        case ColAuctionTurnover:
            return m_stockBids[row].bidding_turnover;
            break;
        case ColCirValue:
            return m_stockBids[row].current_value;
            break;
        case ColPreRate:
            return  m_stockBids[row].yesterday_rate;
            break;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant BiddingItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColPlainPrice)  return  m_stockBids[row].pre_close_price;
    else  if(idx == ColStockCode) return QString::fromStdString(m_stockBids[row].stock_code);

    return QVariant();
}



///RFMonitorItemModel---
void RFMonitorItemModel::initData(const HZData::RiseFallMonitorInfo &rfmInfo)
{
    this->beginResetModel();
    m_riseFallData.clear();
    m_totalCount = rfmInfo.totalCount;
    m_riseFallData = std::move(rfmInfo.rfmData);
    m_riseFallData.resize(m_totalCount);
    this->endResetModel();
}

void RFMonitorItemModel::updateData(const HZData::RiseFallMonitorInfo &rfmInfo,int start)
{
    const int num = rfmInfo.rfmData.size();

    for(int i=0;i<num;i++)
        m_riseFallData[start+i]  = rfmInfo.rfmData[i] ;

    emit dataChanged(index(start,1),index(start+num-1,this->columnCount()-1));
}

void RFMonitorItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_riseFallData.clear();
    this->endResetModel();
}

QVariant RFMonitorItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx idx = this->columnIdx(index.column());

    if(role == Stock_Data_Role)
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColStockCode:
            return QString::fromStdString(m_riseFallData[row].stock_code);
            break;
        case ColStockName:
            return QString::fromStdString(m_riseFallData[row].stock_name);
            break;
        case ColPrice:
            return 0;
            break;
        case ColRate:
            return m_riseFallData[row].rise;
            break;

        case ColRfmUpLimitOpenTime:
            return  m_riseFallData[row].to_up_limit_time;
            break;
        case ColRfmDownLimitOpenTime:
            return  m_riseFallData[row].to_lower_limit_time;
            break;
        case ColRfmUpLimitTime:
            return  m_riseFallData[row].up_limit_rate_time;
            break;
        case ColRfmDownLimitTime:
            return  m_riseFallData[row].lower_limit_rate_time;
            break;
        case ColContUpLimitRate:
            return  QString::fromStdString(m_riseFallData[row].continuous_up_limit_rate);
            break;
        case ColContDownLimitRate:
            return  QString::fromStdString(m_riseFallData[row].continuous_up_limit_rate);
            break;
        case ColUpLimitVolume:
            return  m_riseFallData[row].up_limit_volume;
            break;
        case ColDownLimitVolume:
            return  m_riseFallData[row].lower_limit_volume;
            break;
        case ColFriedTime:
            return  m_riseFallData[row].fried_time;
            break;
        case ColFriedNum:
            return  m_riseFallData[row].fried_num;
            break;
        case ColRfmHistCont:
            return  QString::fromStdString(m_riseFallData[row].history_continuous);
            break;
        case ColLeaderNetput:
            return  m_riseFallData[row].main_funds;
            break;
        case ColBlockName:
            return QString::fromStdString(m_riseFallData[row].plate);
            break;
        default:
            break;
        }

    return QVariant();
}

QVariant RFMonitorItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    //if(idx == ColPlainPrice)  return  m_riseFallData[row].pre_close_price;
    if(idx == ColStockCode) return QString::fromStdString(m_riseFallData[row].stock_code);

    return QVariant();
}


///FundAbnormalItemModel----
void FundAbnormalItemModel::initData(const HZData::FundAbnormalInfo &falInfo)
{
    this->beginResetModel();
    m_totalCount = falInfo.totalCount;
    m_fundAbnormal.clear();
    m_fundAbnormal = falInfo.fundAbmls;
    m_fundAbnormal.resize(m_totalCount);
    this->endResetModel();
}

void FundAbnormalItemModel::updateData(const HZData::FundAbnormalInfo &falInfo,int start)
{
    const int num = falInfo.fundAbmls.size();

    for(int i=0;i<num;i++)
        m_fundAbnormal[start+i] = falInfo.fundAbmls[i] ;

    emit dataChanged(index(start,1),index(start+num-1,this->columnCount()-1));
}

void FundAbnormalItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_fundAbnormal.clear();
    this->endResetModel();
}

QVariant FundAbnormalItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    ColumnIdx idx = this->columnIdx(index.column());
    if(role == Stock_Data_Role){
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColStockCode:
            return QString::fromStdString(m_fundAbnormal[row].stock_code);
            break;
        case ColStockName:
            return QString::fromStdString(m_fundAbnormal[row].stock_name);
            break;
        case ColBlockName:
            return QString::fromStdString(m_fundAbnormal[row].block_name);
            break;
        case ColRatePrice:
            return m_fundAbnormal[row].last_price;
            break;
        case ColRate:
            return  m_fundAbnormal[row].rate;
            break;
        case ColThirtyRate:
            return m_fundAbnormal[row].thirty_rate;
            break;
        case ColActiveNetput:
            return  m_fundAbnormal[row].active_netput;
            break;
        case ColThirtyActiveNetput:
            return  m_fundAbnormal[row].thirty_active_netput;
            break;
        case ColLeaderNetput:
            return  m_fundAbnormal[row].leader_netput;
            break;
        case ColActiveRate:
            return  m_fundAbnormal[row].avtive_rate;
            break;
        case ColLeaderRate:
            return  m_fundAbnormal[row].leader_rate;
            break;
        case ColTurnoverRate:
            return  m_fundAbnormal[row].turnover_rate;
            break;
        case ColRatio:
            return  m_fundAbnormal[row].ratio;
            break;
        case ColCirValue:
            return  m_fundAbnormal[row].cir_value;
            break;
        case ColPed:
            return  m_fundAbnormal[row].ped;
            break;
        case ColThreeActiveNetput:
            return  m_fundAbnormal[row].three_avtive_netput;
            break;
        case ColThreeRate:
            return  m_fundAbnormal[row].three_rate;
            break;
        case ColFiveActiveNetput:
            return  m_fundAbnormal[row].five_avtive_netput;
            break;
        case ColFiveRate:
            return  m_fundAbnormal[row].five_rate;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant FundAbnormalItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColPlainPrice)  return  m_fundAbnormal[row].pre_close_price;
    else  if(idx == ColStockCode) return QString::fromStdString(m_fundAbnormal[row].stock_code);

    return QVariant();
}




///LhbStockItemModel---
LhbStockItemModel::LhbStockItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent),m_totalCount(0)
{

}

void LhbStockItemModel::initData(const StockLhbDatas &stockLhbDatas)
{
    this->beginResetModel();
    m_totalCount = stockLhbDatas.nShCOunt + stockLhbDatas.nSzCount;
    m_stkLhbItem = std::move(stockLhbDatas.datas);
    this->endResetModel();
}

void LhbStockItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_stkLhbItem.clear();
    this->endResetModel();
}

QVariant LhbStockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx idx = this->columnIdx(index.column());

    if(role == Stock_Data_Role){
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColStockCode:
            return m_stkLhbItem[row].stockcode;
            break;
        case ColStockName:
            return m_stkLhbItem[row].stockname;
            break;
        case ColRatePrice:
            return m_stkLhbItem[row].close_price;
            break;
        case ColRate:
            return m_stkLhbItem[row].change_ratio;
            break;
        case ColNetput:
            return m_stkLhbItem[row].net_buy;
            break;
        case ColTotalBuyIn:
            return m_stkLhbItem[row].buy;
            break;
        case ColTotalSellOut:
            return m_stkLhbItem[row].sell;
            break;
        case ColBlockName:
            return m_stkLhbItem[row].block;
            break;
        default:
            break;
        }
    }
    return QVariant();
}


QVariant LhbStockItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColStockCode) return m_stkLhbItem[row].stockcode;
    return QVariant();
}



////LhbYybDrbStockItemModel---
LhbYybDrbStockItemModel::LhbYybDrbStockItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void LhbYybDrbStockItemModel::initData(const StockYybDrbList &stkYybDrbList)
{
    this->beginResetModel();
    m_stkLhbItem = std::move(stkYybDrbList);
    this->endResetModel();
}

void LhbYybDrbStockItemModel::appendData(const StockYybDrbList &stkYybDrbList)
{
    if(stkYybDrbList.size()<1) return;
    int first = this->rowCount() ;

    this->beginInsertRows(QModelIndex(),first,first+stkYybDrbList.size()-1);
    m_stkLhbItem.append(stkYybDrbList);
    this->endInsertRows();
}

void LhbYybDrbStockItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_stkLhbItem.clear();
    this->endResetModel();
}

int LhbYybDrbStockItemModel::rowCount(const QModelIndex & ) const
{
    return m_stkLhbItem.size();
}

QVariant LhbYybDrbStockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    ColumnIdx idx = this->columnIdx(index.column());
    if(role == Stock_Data_Role){
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColYybName:
            return m_stkLhbItem[row].name;
            break;
        case ColStockNum:
            return m_stkLhbItem[row].stockCount;
            break;
        case ColNetput:
            return m_stkLhbItem[row].net_buy;
            break;
        case ColThreeSucRate:
            return m_stkLhbItem[row].ratio_3d;
            break;
        case ColCfStock:
            return m_stkLhbItem[row].stockName;
            break;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant LhbYybDrbStockItemModel::colIdxData(const int &row, const ColumnIdx & idx) const
{
    if(ColStockCode == idx) return m_stkLhbItem[row].stockCode;
    else if(idx ==  ColYybId) return m_stkLhbItem[row].id;
    return QVariant();
}





///LhbYybSlbStockItemModel----
LhbYybSlbStockItemModel::LhbYybSlbStockItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void LhbYybSlbStockItemModel::initData(const StockYybSlbList &stkYybSlbList)
{
    this->beginResetModel();
    m_stkLhbItem = std::move(stkYybSlbList);
    this->endResetModel();
}

void LhbYybSlbStockItemModel::appendData(const StockYybSlbList &stkYybSlbList)
{
    if(stkYybSlbList.size()<1) return;
    int first = this->rowCount() ;

    this->beginInsertRows(QModelIndex(),first,first+stkYybSlbList.size()-1);
    m_stkLhbItem.append(stkYybSlbList);
    this->endInsertRows();
}

void LhbYybSlbStockItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_stkLhbItem.clear();
    this->endResetModel();
}

int LhbYybSlbStockItemModel::rowCount(const QModelIndex &) const
{
    return m_stkLhbItem.size();
}

QVariant LhbYybSlbStockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx idx = columnIdx(index.column());
    if(role == Stock_Data_Role){
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColYybName:
            return m_stkLhbItem[row].name;
            break;
        case ColSlFund:
            return m_stkLhbItem[row].max_amount;
            break;
        case ColLastNetput:
            return m_stkLhbItem[row].net_buy;
            break;
        case ColLastOperate:
            return m_stkLhbItem[row].stockname;
            break;
        case ColFullDate:
            return m_stkLhbItem[row].stock_date;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant LhbYybSlbStockItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColStockCode) return m_stkLhbItem[row].stockcode;
    else if(idx ==  ColYybId) return m_stkLhbItem[row].id;
    return QVariant();
}

///LhbYybHybStockItemModel----
LhbYybHybStockItemModel::LhbYybHybStockItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void LhbYybHybStockItemModel::initData(const StockYybHybList &stkYybHybList)
{
    this->beginResetModel();
    m_stkLhbItem = std::move(stkYybHybList);
    this->endResetModel();
}

void LhbYybHybStockItemModel::appendData(const StockYybHybList &stkYybHybList)
{
    if(stkYybHybList.size()<1) return;
    int first = this->rowCount() ;

    this->beginInsertRows(QModelIndex(),first,first+stkYybHybList.size()-1);
    m_stkLhbItem.append(stkYybHybList);
    this->endInsertRows();
}

void LhbYybHybStockItemModel::clearData()
{
    if(m_totalCount<1) return;
    this->beginResetModel();
    m_totalCount = 0;
    m_stkLhbItem.clear();
    this->endResetModel();
}

int LhbYybHybStockItemModel::rowCount(const QModelIndex & ) const
{
    return m_stkLhbItem.size();
}

QVariant LhbYybHybStockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(role == Stock_Data_Role){
        ColumnIdx idx = columnIdx(index.column());
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColYybName:
            return m_stkLhbItem[row].name;
            break;
        case ColRankNum:
            return m_stkLhbItem[row].count_3m;
            break;
        case ColBuyTurnover:
            return m_stkLhbItem[row].buy;
            break;
        case ColSellTurnover:
            return m_stkLhbItem[row].sell;
            break;
        case ColLastOperate:
            return m_stkLhbItem[row].stockname;
            break;
        case ColFullDate:
            return m_stkLhbItem[row].stock_date;
            break;
        case ColRate:
            return m_stkLhbItem[row].stock_rate;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant LhbYybHybStockItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColStockCode) return m_stkLhbItem[row].stockcode;
    else if(idx ==  ColYybId)  return m_stkLhbItem[row].id;
    return QVariant();
}



///LhbYybSbItemModel---
LhbYybSbItemModel::LhbYybSbItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{
    m_histSbList = {QStringLiteral("近一月"),QStringLiteral("近三月")
                    ,QStringLiteral("近半年"),QStringLiteral("近一年")};
}

void LhbYybSbItemModel::initData(const QVector<StockYybDetailStatistics> &stkYybSbList)
{
    this->beginResetModel();
    m_stkLhbItem = std::move(stkYybSbList);
    this->endResetModel();
}

void LhbYybSbItemModel::clearData()
{
    if(m_stkLhbItem.size()<0) return;
    this->beginResetModel();
    m_stkLhbItem.clear();
    this->endResetModel();
}

int LhbYybSbItemModel::rowCount(const QModelIndex &) const
{
    return m_stkLhbItem.size();
}

QVariant LhbYybSbItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    ColumnIdx idx=  columnIdx(index.column());
    if(role == Stock_Data_Role){
        switch (idx) {
        case ColHistSb:
            return m_histSbList[row];
            break;
        case ColRankNum:
            return m_stkLhbItem[row].count;
            break;
        case ColTotalBuyIn:
            return m_stkLhbItem[row].buy;
            break;
        case ColTotalSellOut:
            return m_stkLhbItem[row].sell;
            break;
        case ColGBuySucRate:
            return m_stkLhbItem[row].ratio;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant LhbYybSbItemModel::colIdxData(const int &, const ColumnIdx &) const
{
    return QVariant();
}


///LhbYybHoItemModel---
LhbYybHoItemModel::LhbYybHoItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void LhbYybHoItemModel::initData(const QVector<StockYybDetaiHistory> &stkYybHoList)
{
    this->beginResetModel();
    m_stkLhbItem = std::move(stkYybHoList);
    this->endResetModel();
}

void LhbYybHoItemModel::clearData()
{
    if(m_stkLhbItem.size()<0) return;
    this->beginResetModel();
    m_stkLhbItem.clear();
    this->endResetModel();
}

int LhbYybHoItemModel::rowCount(const QModelIndex &) const
{
    return m_stkLhbItem.size();
}

QVariant LhbYybHoItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    ColumnIdx idx=  columnIdx(index.column());
    if(role == Stock_Data_Role){
        switch (idx) {
        case ColFullDate:
            return m_stkLhbItem[row].date;
            break;
        case ColStockCode:
            return m_stkLhbItem[row].stockcode;
            break;
        case ColStockName:
            return m_stkLhbItem[row].stockname;
            break;
        case ColTradeDirc:
            return m_stkLhbItem[row].dirc;
            break;
        case ColTotalBuyIn:
            return m_stkLhbItem[row].buy*10000; //单位：万元
            break;
        case ColTotalSellOut:
            return m_stkLhbItem[row].sell*10000; //单位：万元
            break;
        case ColDrRate:
            return m_stkLhbItem[row].change_ratio;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant LhbYybHoItemModel::colIdxData(const int &row, const ColumnIdx & idx) const
{
    if(idx == ColStockCode) return m_stkLhbItem[row].stockcode;
    return QVariant();
}


///StockDzjyItemModel----
StockDzjyItemModel::StockDzjyItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void StockDzjyItemModel::initData(const  QVector<StockDzjyItem> &stkDzjyList)
{
    if(m_stkDzjyItem.size()<0) return;
    this->beginResetModel();
    m_stkDzjyItem = std::move(stkDzjyList);
    this->endResetModel();
}

void StockDzjyItemModel::clearData()
{
    this->beginResetModel();
    m_stkDzjyItem.clear();
    this->endResetModel();
}

int StockDzjyItemModel::rowCount(const QModelIndex & ) const
{
    return m_stkDzjyItem.size();
}

QVariant StockDzjyItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx idx = columnIdx(index.column());
    if(role ==  Stock_Data_Role){
        switch(idx){
        case ColNo:
            return row;
            break;
        case ColStockCode:
            return m_stkDzjyItem[row].stockcode;
            break;
        case ColStockName:
            return m_stkDzjyItem[row].stockname;
            break;
        case ColDealPrice:
            return m_stkDzjyItem[row].deal_price;
            break;
        case ColClosePrice:
            return m_stkDzjyItem[row].last_close;
            break;
        case ColRate:
            return m_stkDzjyItem[row].change_ratio;
            break;
        case ColOverPriceRate:
            return m_stkDzjyItem[row].premium_rate;
            break;
        case ColIDzjyVol:
            return m_stkDzjyItem[row].volume;
            break;
        case ColTurnover:
            return m_stkDzjyItem[row].turnover;
            break;
        case ColYybBuy:
            return m_stkDzjyItem[row].yyb_buy;
        case ColYybSell:
            return m_stkDzjyItem[row].yyb_sell;
            break;
        }
    }
    return QVariant();
}

QVariant StockDzjyItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColStockCode) return m_stkDzjyItem[row].stockcode;
    return QVariant();
}




///StockRzrqRankItemModel---
StockRzrqRankItemModel::StockRzrqRankItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void StockRzrqRankItemModel::initData(const QVector<StockRzrqTopItem> &stkRzrqTopList)
{
    this->beginResetModel();
    m_stkRzrqRanktem = std::move(stkRzrqTopList);
    this->endResetModel();
}

void StockRzrqRankItemModel::appendData(const QVector<StockRzrqTopItem> &stkRzrqTopList)
{
    if(stkRzrqTopList.size()<1) return;
    int first = this->rowCount() ;

    this->beginInsertRows(QModelIndex(),first,first+stkRzrqTopList.size()-1);
    m_stkRzrqRanktem.append(stkRzrqTopList);
    this->endInsertRows();
}

void StockRzrqRankItemModel::clearData()
{
    if(m_stkRzrqRanktem.size()<1) return;
    this->beginResetModel();
    m_stkRzrqRanktem.clear();
    this->endResetModel();
}

int StockRzrqRankItemModel::rowCount(const QModelIndex &  ) const
{
    return m_stkRzrqRanktem.size();
}

QVariant StockRzrqRankItemModel::data(const QModelIndex &index, int role) const
{
    if(role == Stock_Data_Role){
        int row = index.row();
        ColumnIdx idx = columnIdx(index.column());
        switch(idx){
        case ColStockCode:
            return m_stkRzrqRanktem[row].stockcode;
            break;
        case ColStockName:
            return m_stkRzrqRanktem[row].stockname;
            break;
        case ColRatePrice:
            return m_stkRzrqRanktem[row].close_price;
            break;
        case ColUpDownPrice:
            return m_stkRzrqRanktem[row].up_down_price;
            break;
        case ColRate:
            return m_stkRzrqRanktem[row].change_ratio;
            break;
        case ColRzmre:
            return m_stkRzrqRanktem[row].rzmre;
            break;
        case ColRzjmre:
            return m_stkRzrqRanktem[row].rzjmre;
            break;
        case ColRzye:
            return m_stkRzrqRanktem[row].rzye;
            break;
        case ColRzche:
            return m_stkRzrqRanktem[row].rzche;
            break;
        case ColRqyl:
            return m_stkRzrqRanktem[row].rqyl;
            break;
        case ColRqmcl:
            return m_stkRzrqRanktem[row].rqmcl;
            break;
        case ColRqchl:
            return m_stkRzrqRanktem[row].rqchl;
            break;
        }
    }

    return QVariant();
}

QVariant StockRzrqRankItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColStockCode) return m_stkRzrqRanktem[row].stockcode;
    return QVariant();
}



///StockRzrqDetailItemModel--
StockRzrqDetailItemModel::StockRzrqDetailItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void StockRzrqDetailItemModel::initData(const QVector<StockRzrqDetailItem> &stkRzrqDetailList)
{
    this->beginResetModel();
    m_stkRzrqDetailItem = std::move(stkRzrqDetailList);
    this->endResetModel();
}

void StockRzrqDetailItemModel::clearData()
{
    if(m_stkRzrqDetailItem.size()<1) return;
    this->beginResetModel();
    m_stkRzrqDetailItem.clear();
    this->endResetModel();
}

int StockRzrqDetailItemModel::rowCount(const QModelIndex &) const
{
    return m_stkRzrqDetailItem.size();
}

QVariant StockRzrqDetailItemModel::data(const QModelIndex &index, int role) const
{
    if(role == Stock_Data_Role){
        int row = index.row();
        ColumnIdx idx = columnIdx(index.column());
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColFullDate:
            return  m_stkRzrqDetailItem[row].date;
            break;
        case ColRzmre:
            return m_stkRzrqDetailItem[row].rzmre;
            break;
        case ColRzye:
            return m_stkRzrqDetailItem[row].rzye;
            break;
        case ColRzche:
            return m_stkRzrqDetailItem[row].rzche;
            break;
        case ColRqyl:
            return m_stkRzrqDetailItem[row].rqyl;
            break;
        case ColRqmcl:
            return m_stkRzrqDetailItem[row].rqmcl;
            break;
        case ColRqchl:
            return m_stkRzrqDetailItem[row].rqchl;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant StockRzrqDetailItemModel::colIdxData(const int &, const ColumnIdx &) const
{
    return QVariant();
}




///BlockRzrqItemModel----
BlockRzrqItemModel::BlockRzrqItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void BlockRzrqItemModel::initData(const QVector<StockBoardRzrqTopItem> &bkStockRzrqList)
{
    this->beginResetModel();
    m_bkStockRzrqItem = std::move(bkStockRzrqList);
    this->endResetModel();
}

void BlockRzrqItemModel::setStockDyna(HZData::Stock *blocks,int num)
{
    for(int i=0;i<num;i++){
        m_bkStockRzrqItem[i].last_price = blocks[i].last_price;
        m_bkStockRzrqItem[i].up_down_price = blocks[i].up_down_price;
        m_bkStockRzrqItem[i].rate = blocks[i].rate;
        m_bkStockRzrqItem[i].three_rate = blocks[i].three_rate;
        m_bkStockRzrqItem[i].five_rate = blocks[i].five_rate;
        m_bkStockRzrqItem[i].turnover_rate = blocks[i].turnover_rate;
        m_bkStockRzrqItem[i].three_tuv_rate = blocks[i].three_tuv_rate;
    }
    emit dataChanged(index(0,0),index(num-1,this->columnCount()-1));
}

void BlockRzrqItemModel::clearData()
{
    if(m_bkStockRzrqItem.size()<1) return;
    this->beginResetModel();
    m_bkStockRzrqItem.clear();
    this->endResetModel();
}

int BlockRzrqItemModel::rowCount(const QModelIndex &  ) const
{
    return m_bkStockRzrqItem.size();
}

QVariant BlockRzrqItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx idx = columnIdx(index.column());
    if(role == Stock_Data_Role){
        switch (idx) {
        case ColNo:
            return row;
        case ColBKCode:
            return m_bkStockRzrqItem[row].boardcode;
            break;
        case ColStockName:
            return m_bkStockRzrqItem[row].boardname;
            break;
        case ColRatePrice:
            return m_bkStockRzrqItem[row].last_price;
            break;
        case ColUpDownPrice:
            return m_bkStockRzrqItem[row].up_down_price;
            break;
        case ColRate:
            return m_bkStockRzrqItem[row].rate;
            break;
        case ColRzmre:
            return m_bkStockRzrqItem[row].rzmre;
            break;
        case ColRiseStock:
            return QString::fromStdString(m_bkStockRzrqItem[row].risest_stock);
            break;
        case ColThreeRate:
            return m_bkStockRzrqItem[row].three_rate;
            break;
        case ColFiveRate:
            return m_bkStockRzrqItem[row].five_rate;
            break;
        case ColTurnoverRate:
            return m_bkStockRzrqItem[row].turnover_rate;
            break;
        case ColThreeTuvRate:
            return m_bkStockRzrqItem[row].three_tuv_rate;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant BlockRzrqItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(ColStockCode == idx) return m_bkStockRzrqItem[row].boardcode;
    return QVariant();
}



///XgrlStockItemModel---
XgrlStockItemModel::XgrlStockItemModel(const QString &modelName,QObject *parent)
    :AbstractDataBaseModel(modelName,parent)
{

}

void XgrlStockItemModel::initData(const QVector<StockXgrlItem> &stockXgrlItems)
{
    this->beginResetModel();
    m_stockXgrlItems = std::move(stockXgrlItems);
    this->endResetModel();
}

void XgrlStockItemModel::updateDynaData(char *data, int num)
{
    HZData::SLStock *slStock = (HZData::SLStock *)data;
    for(int i=0;i<num;i++){
        m_stockXgrlItems[i].rate =slStock[i].rate;
        m_stockXgrlItems[i].up_down_price  =slStock[i].up_down_price;
        m_stockXgrlItems[i].last_price  = slStock[i].last_price;
    }
    emit dataChanged(this->index(0,4),this->index(0,7));
}

void XgrlStockItemModel::clearData()
{
    if(m_stockXgrlItems.size()<1) return;
    this->beginResetModel();
    m_stockXgrlItems.clear();
    this->endResetModel();
}

int XgrlStockItemModel::rowCount(const QModelIndex & ) const
{
    return m_stockXgrlItems.size();
}


QVariant XgrlStockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx idx = this->columnIdx(index.column());

    if(role == Stock_Data_Role){
        switch (idx) {
        case ColNo:
            return row;
            break;
        case ColStockCode:
            return m_stockXgrlItems[row].stockcode;
            break;
        case ColStockName:
            return m_stockXgrlItems[row].stockname;
            break;
        case ColFxjg:
            return m_stockXgrlItems[row].fxjg;
            break;
        case ColRatePrice:
            return m_stockXgrlItems[row].last_price;
            break;
        case ColUpDownPrice:
            return m_stockXgrlItems[row].up_down_price;
            break;
        case ColRate:
            return m_stockXgrlItems[row].rate;
            break;
        case ColTotalRate:
            return m_stockXgrlItems[row].total_rate;
            break;
        case ColMzyqhl:
            return m_stockXgrlItems[row].mzyqhl;
            break;
        case ColSgrq:
            return m_stockXgrlItems[row].sgrq;
            break;
        case ColSgsx:
            return m_stockXgrlItems[row].sgsx;
            break;
        case ColFxsyl:
            return m_stockXgrlItems[row].syl;
            break;
        case ColZql:
            return m_stockXgrlItems[row].zql;
            break;
        case ColFxzs:
            return m_stockXgrlItems[row].fxzs;
            break;
        case ColWsfxl:
            return m_stockXgrlItems[row].wsfxl;
            break;
        case ColSsrq:
            return m_stockXgrlItems[row].ssrq;
            break;
        case ColZqgbrq:
            return m_stockXgrlItems[row].zqrq;
            break;
        case ColZqjkrq:
            return m_stockXgrlItems[row].zqjkrq;
            break;
        case ColSl:
            return m_stockXgrlItems[row].sl;
            break;
        case ColZgs:
            return m_stockXgrlItems[row].zgs;
            break;
        default:
            break;
        }
    }else if(Yugu_Data_Role == role)
    {
        switch (idx) {
        case ColFxjg:
            return m_stockXgrlItems[row].ycfxjg;
            break;
        case ColSgsx:
            return m_stockXgrlItems[row].ycsgsx;
            break;
        case ColFxsyl:
            return m_stockXgrlItems[row].ycsyl;
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant XgrlStockItemModel::colIdxData(const int &row, const ColumnIdx &idx) const
{
    if(idx == ColStockCode)  return m_stockXgrlItems[row].stockcode;
    return QVariant();
}


