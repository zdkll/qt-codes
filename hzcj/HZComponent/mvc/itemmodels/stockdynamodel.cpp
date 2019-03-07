#include "stockdynamodel.h"

#include <QApplication>
#include <QDebug>

QTime m_t;

///AbstractBaseStockModel-----
AbstractBaseStockModel::~AbstractBaseStockModel()
{
    if(m_sorter){delete m_sorter;m_sorter = NULL;}
}

int AbstractBaseStockModel::stkRow(const std::string &stockCode)
{
    auto it = m_stkMap.find(stockCode);
    if( it != m_stkMap.end())
        return it->second;
    return -1;
}

void AbstractBaseStockModel::setSortIndicator(int sortField,bool desc)
{
    m_sorter->setSortIndicator(sortField,desc);
}



///SLStockItemModel---------------------
SLStockItemModel::SLStockItemModel(const QString &modelName,QObject *parent)
    :AbstractBaseStockModel(modelName,parent)
{

}

void SLStockItemModel::initData(const HZData::StockCode *stockCodes, int num)
{
    this->beginResetModel();
    m_slStocks.clear();
    m_stkMap.clear();
    m_slStocks.resize(num);
    for(int i=0;i<num;i++)
    {
        m_slStocks[i].stock_code  = stockCodes[i].stock_code ;
        m_slStocks[i].stock_name = stockCodes[i].stock_name ;
        m_stkMap.emplace(stockCodes[i].stock_code,i);
    }
    this->endResetModel();
}

void SLStockItemModel::initData(const char *data,int num)
{
    HZData::SLStock *slStock = (HZData::SLStock *)data;
    this->beginResetModel();
    m_slStocks.resize(num);
    m_stkMap.clear();
    for(int i=0;i<num;i++){
        m_slStocks[i]  = slStock[i];
        m_stkMap.emplace(slStock[i].stock_code,i);
    }
    this->endResetModel();
}

void SLStockItemModel::updateData(const char *data,int num)
{
    if(num > m_slStocks.size()) return;
    HZData::SLStock *tmpStocks = (HZData::SLStock *)data;
    //更新数据
    for(int i=0;i<num;i++){
        //        auto it = m_stkMap.find(tmpStocks[i].stock_code);
        //        if(it != m_stkMap.end())
        //            m_slStocks.replace(it->second,tmpStocks[i]);
        m_slStocks.replace(i,tmpStocks[i]);
    }

    if(m_stkMap.size()>0)
        emit dataChanged(index(0,0),index(m_slStocks.size()-1,columnCount()-1));
}

//更新快照
void SLStockItemModel::updateData(const char *data,int  ,int num,bool  )
{
    if(num > m_slStocks.size()) return;
    HZData::SLStock *tmpStocks = (HZData::SLStock *)data;
    //更新数据
    for(int i=0;i<num;i++){
        auto it = m_stkMap.find(tmpStocks[i].stock_code);
        if(it != m_stkMap.end())
            m_slStocks.replace(it->second,tmpStocks[i]);
    }

    if(m_stkMap.size()>0)
        emit dataChanged(index(0,0),index(m_stkMap.size()-1,columnCount()-1));
}


void SLStockItemModel::removeStock(const QString& stockCode)
{
    auto it = m_stkMap.find(stockCode.toStdString());
    if(it != m_stkMap.end()){
        this->beginRemoveRows(QModelIndex(),it->second,it->second);
        m_slStocks.remove(it->second);
        this->endRemoveRows();
    }

    //更新映射
    m_stkMap.clear();
    for(int i=0;i<m_slStocks.size();i++)
        m_stkMap.emplace(m_slStocks[i].stock_code,i);
}

void SLStockItemModel::clearData()
{
    if(m_slStocks.size()<1) return;
    this->beginResetModel();
    m_slStocks.clear();
    m_stkMap.clear();
    this->endResetModel();
}

int  SLStockItemModel::rowCount(const QModelIndex &parent) const
{
    return m_slStocks.count();
}

QVariant  SLStockItemModel::colIdxData(const int &row, const ColumnIdx &inColumn) const
{
    switch (inColumn) {
    case ColPlainPrice:
        return m_slStocks[row].pre_close_price;
        break;
    case ColStockCode:
        return QString::fromStdString(m_slStocks[row].stock_code);
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant  SLStockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col  = index.column();
    ColumnIdx colIdx = columnIdx(col);
    if(role == Stock_Data_Role){
        switch(colIdx){
        case ColNo:                    return row+1;
        case ColBKCode:
        case ColBKStockCode:
        case ColStockCode:         return QString::fromStdString(m_slStocks[row].stock_code);
        case ColStockName:        return QString::fromStdString(m_slStocks[row].stock_name);
        case ColPrice:            return m_slStocks[row].last_price;
        case ColUpDownPrice:      return m_slStocks[row].up_down_price;
        case ColRatePercent:
        case ColRate:                  return m_slStocks[row].rate;

        case ColPlainPrice:  return m_slStocks[row].pre_close_price;
        case ColTurnover:           return m_slStocks[row].turnover;
        case ColTurnoverRate:    return m_slStocks[row].turnover_rate;

        case ColFiveRate:        return m_slStocks[row].five_rate;
        case ColSpeedRate:    return m_slStocks[row].speed_rate;
        }
    }
    return QVariant();
}





///StockItemModel--------
StockItemModel::StockItemModel(const QString &modelName,QObject *parent)
    :AbstractBaseStockModel(modelName,parent)
{

}

void StockItemModel::initData(const HZData::StockCode *stockCodes, int num)
{
    this->beginResetModel();
    m_stocks.clear();
    m_stkMap.clear();
    m_stocks.resize(num);
    for(int i=0;i<num;i++)
    {
        m_stocks[i].stock_code  = stockCodes[i].stock_code ;
        m_stocks[i].stock_name = stockCodes[i].stock_name ;
    }
    this->endResetModel();
}

void StockItemModel::updateData(const char *data,int start,int num,bool first)
{
    if((start+num) > m_stocks.size()) return;

    HZData::Stock *tmpStocks = (HZData::Stock *)data;
    if(first){
        m_stkMap.clear();
        QVector<HZData::Stock> stocks(num);
        //需要排序,先构建临时列表，然后排序
        for(int i=0;i<num;i++)
            stocks[i] = tmpStocks[i];
        m_sorter->sort(stocks);
        for(int i=0;i<num;i++){
            m_stocks.replace(start+i,stocks[i]);
            m_stkMap.emplace(stocks[i].stock_code,start+i);
        }
    }else{
        //插入
        for(int i=0;i<num;i++){
            auto it = m_stkMap.find(tmpStocks[i].stock_code);
            if(it != m_stkMap.end())
                m_stocks.replace(it->second,tmpStocks[i]);
        }
        //需要排序，先取本地当前段数据， 然后排序
        QVector<HZData::Stock> stocks = m_stocks.mid(start,m_stkMap.size());
        m_sorter->sort(stocks);
        for(int i=0;i<stocks.size();i++)
            m_stocks.replace(start+i,stocks[i]);
    }
    if(m_stkMap.size()>0)
        emit dataChanged(index(start,1),index(start+m_stkMap.size()-1,columnCount()-1));
}

void StockItemModel::clearData()
{
    if(m_stocks.size()<1) return;
    this->beginResetModel();
    m_stocks.clear();
    m_stkMap.clear();
    this->endResetModel();
}

int  StockItemModel::rowCount(const QModelIndex &parent) const
{
    return m_stocks.count();
}


QVariant  StockItemModel::colIdxData(const int &row, const ColumnIdx &inColumn) const
{
    switch (inColumn) {
    case ColPlainPrice:
        return m_stocks[row].pre_close_price;
        break;
    case ColStockCode:
        return QString::fromStdString(m_stocks[row].stock_code);
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant StockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col  = index.column();
    ColumnIdx colIdx = columnIdx(col);

    if(role == Stock_Data_Role){
        switch(colIdx){
        case ColNo:                    return row+1;
        case ColBKCode:
        case ColBKStockCode:
        case ColStockCode:       return QString::fromStdString(m_stocks[row].stock_code);
        case ColStockName:      return QString::fromStdString(m_stocks[row].stock_name);

        case ColBlockName:      return QString::fromStdString(m_stocks[row].block_name);

        case ColPrice:         return m_stocks[row].last_price;
        case ColUpDownPrice:   return m_stocks[row].up_down_price;
        case ColRate:                return m_stocks[row].rate;

        case ColVolume:             return m_stocks[row].volume ;
        case ColRateYearYet:     return m_stocks[row].year_rate;

        case ColBuyPrice:     return m_stocks[row].buy_price;
        case ColSellPrice:      return m_stocks[row].sell_price;

        case ColSpeedRate:      return m_stocks[row].speed_rate;
        case ColTurnoverRate:   return m_stocks[row].turnover_rate;
        case ColTurnover:          return m_stocks[row].turnover;

        case ColPed:                   return m_stocks[row].ped;
        case ColHighestPrice:      return m_stocks[row].highest_price;
        case ColLowestPrice:       return m_stocks[row].lowest_price;
        case ColOpenPrice:          return m_stocks[row].open_price;
        case ColPlainPrice:     return m_stocks[row].pre_close_price;

        case ColSa:                   return m_stocks[row].sa;
        case ColRatio:               return m_stocks[row].ratio;
        case ColCommittee:       return m_stocks[row].committee;

        case ColPbv:             return m_stocks[row].pbv;

        case ColTotStock:      return m_stocks[row].tot_stock;
        case ColTotValue:      return m_stocks[row].tot_value;
        case ColCirStock:      return m_stocks[row].cir_stock;
        case ColCirValue:       return m_stocks[row].cir_value;

        case ColThreeRate:       return m_stocks[row].three_rate;
        case ColFiveRate:          return m_stocks[row].five_rate;
        case ColThreeTuvRate:  return m_stocks[row].three_tuv_rate;
        case ColFiveTuvRate:     return m_stocks[row].five_tuv_rate;
        }
    }
    return QVariant();
}


///OptionStockItemModel---
void OptionStockItemModel::initData(const HZData::StockCode *stockCodes, int num)
{
    this->beginResetModel();
    m_stocks.clear();
    m_stkMap.clear();
    m_stocks.resize(num);
    for(int i=0;i<num;i++)
    {
        m_stocks[i].stock_code  = stockCodes[i].stock_code ;
        m_stocks[i].stock_name = stockCodes[i].stock_name ;
        m_stkMap.emplace(m_stocks[i].stock_code,i);
    }
    this->endResetModel();
}

void OptionStockItemModel::updateData(const char *data,int start,int num,bool first)
{
    if((start+num) > m_stocks.size()) return;

    HZData::Stock *tmpStocks = (HZData::Stock *)data;

    //更新数据
    for(int i=0;i<num;i++){
        auto it = m_stkMap.find(tmpStocks[i].stock_code);
        if(it != m_stkMap.end()){
            m_stocks.replace(it->second,tmpStocks[i]);
        }
    }

    if(m_stkMap.size()>0)
        emit dataChanged(index(start,1),index(start+m_stkMap.size()-1,columnCount()-1));
}

void OptionStockItemModel::removeStock(const QString& stockCode)
{
    auto it = m_stkMap.find(stockCode.toStdString());
    if(it != m_stkMap.end())
    {
        this->beginRemoveRows(QModelIndex(),it->second,it->second);
        m_stocks.remove(it->second);
        this->endRemoveRows();
    }
    //更新映射
    m_stkMap.clear();
    for(int i=0;i<m_stocks.size();i++)
        m_stkMap.emplace(m_stocks[i].stock_code,i);
}



///BlockItemModel-----
BlockItemModel::BlockItemModel(const QString &modelName,QObject *parent)
    :AbstractBaseStockModel(modelName,parent)
{

}

void BlockItemModel::initData(const HZData::StockCode *stockCodes, int num)
{
    this->beginResetModel();
    m_blocks.clear();
    m_stkMap.clear();
    m_blocks.resize(num);
    for(int i=0;i<num;i++)
    {
        m_blocks[i].stock_code  = stockCodes[i].stock_code ;
        m_blocks[i].stock_name = stockCodes[i].stock_name ;
    }
    this->endResetModel();
}

//更新当前快照
void BlockItemModel::updateData(const char *data,int start,int num,bool first)
{
    if((start+num) > m_blocks.size()) return;
    HZData::Block *tmpBlocks = (HZData::Block *)data;
    if(first){
        m_stkMap.clear();
        for(int i=0;i<num;i++){
            m_blocks[start+i] = tmpBlocks[i];
            m_stkMap.emplace(tmpBlocks[i].stock_code,start+i);
        }
    }else{
        for(int i=0;i<num;i++){
            auto it = m_stkMap.find(tmpBlocks[i].stock_code);
            if(it != m_stkMap.end())
                m_blocks.replace(it->second,tmpBlocks[i]);
        }
    }
    if(m_stkMap.size()>0)
        emit dataChanged(index(start,0),index(start + m_stkMap.size()-1,columnCount()-1));
}

void BlockItemModel::clearData()
{
    if(m_blocks.size()<1) return;
    this->beginResetModel();
    m_blocks.clear();
    m_stkMap.clear();
    this->endResetModel();
}

int  BlockItemModel::rowCount(const QModelIndex &parent) const
{
    return m_blocks.count();
}

QVariant  BlockItemModel::colIdxData(const int &row, const ColumnIdx &inColumn) const
{
    if(inColumn == ColStockCode)
        return QString::fromStdString(m_blocks[row].stock_code);
    //涨跌家数
    else if(inColumn == ColRiseNum) return m_blocks[row].rise;
    else if(inColumn == ColFallNum) return m_blocks[row].fall;
    else if(inColumn == ColFlatNum)  return m_blocks[row].flat;
    else if(inColumn == ColFlatNum)  return m_blocks[row].flat;
    else if(inColumn == ColRiseStockCode) return QString::fromStdString(m_blocks[row].risest_code);

    return QVariant();
}

QVariant  BlockItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col  = index.column();
    ColumnIdx colIdx = columnIdx(col);

    if(role == Stock_Data_Role){
        switch(colIdx){
        case ColNo:                    return row+1;
        case ColBKCode:
        case ColBKStockCode:
        case ColStockCode:       return QString::fromStdString(m_blocks[row].stock_code);
        case ColStockName:      return QString::fromStdString(m_blocks[row].stock_name);

        case ColRate:                return m_blocks[row].rate;
        case ColSpeedRate:      return m_blocks[row].speed_rate;

        case ColRiseDayNum:   return m_blocks[row].rise_day_num ;
        case ColRiseStock:       return QString::fromStdString(m_blocks[row].risest_stock);
        case ColRiseStockRate: return m_blocks[row].risest_stock_rate ;

        case ColRiseNum: return m_blocks[row].rise ;
        case ColFlatNum:  return m_blocks[row].flat ;
        case ColFallNum:  return m_blocks[row].fall ;

        case ColThreeRate:        return m_blocks[row].three_rate;
        case ColFiveRate:          return m_blocks[row].five_rate;
        case ColRateYearYet:     return m_blocks[row].year_rate;

        case ColVolume:            return m_blocks[row].volume ;
        case ColTurnover:          return m_blocks[row].turnover;
        case ColTurnoverRate:          return m_blocks[row].turnover_rate;
        case ColThreeTuvRate:          return m_blocks[row].three_tuv_rate;

        case ColCirValue:           return m_blocks[row].cir_value;
        case ColTotValue:          return m_blocks[row].tot_value;

        case ColTime:                return m_blocks[row].update_time;
        }
    }
    return QVariant();
}


///AblBlockItemModel-----
void AblBlockItemModel::initData(const QVector<HZData::Block>& blocks)
{
    this->beginResetModel();
    m_blocks = std::move(blocks);
    this->endResetModel();
}

void AblBlockItemModel::updateData(const QVector<HZData::Block>& blocks)
{
    //有新的异动板块
    if(blocks.size()>m_blocks.size())
    {
        this->beginInsertRows(QModelIndex(),m_blocks.size(),blocks.size()-1);
        m_blocks.append(blocks.mid(m_blocks.size()));
        this->endInsertRows();
    }else if(blocks.size()<m_blocks.size()){//板块减少，一般是清盘，重新刷新
        this->beginResetModel();
        m_blocks = std::move(blocks);
        this->endResetModel();
    }
}



///IndexItemModel-------------------
IndexItemModel::IndexItemModel(const QString &modelName,QObject *parent)
    :AbstractBaseStockModel(modelName,parent)
{

}

//用股票码初始化数据
void IndexItemModel::initData(const HZData::StockCode *stockCodes, int num)
{
    this->beginResetModel();
    m_indexs.clear();
    m_indexs.resize(num);
    for(int i=0;i<num;i++)
    {
        m_indexs[i].stock_code  = stockCodes[i].stock_code ;
        m_indexs[i].stock_name = stockCodes[i].stock_name ;
    }
    m_stkMap.clear();
    this->endResetModel();
}

//更新当前快照
void IndexItemModel::updateData(const char *data,int start,int num,bool first)
{
    qDebug()<<start<<num<<first;
    if((start+num) > m_indexs.size()) return;
    HZData::Index *tmpIndexs = (HZData::Index *)data;
    if(first){
        m_stkMap.clear();
        QVector<HZData::Index> indexs(num);
        //本地排序
        for(int i=0;i<num;i++)
            indexs[i] = tmpIndexs[i];
        //排序
        m_sorter->sort(indexs);
        //更新数据
        for(int i=0;i<num;i++){
            m_indexs.replace(start+i,indexs[i]);
            m_stkMap.emplace(indexs[i].stock_code,start+i);
        }
    }else{
        for(int i=0;i<num;i++){
            auto it = m_stkMap.find(tmpIndexs[i].stock_code);
            if(it != m_stkMap.end())
                m_indexs.replace(it->second,tmpIndexs[i]);
        }
        //需要排序，先取本地当前段数据， 然后排序
        QVector<HZData::Index> indexs = m_indexs.mid(start,m_stkMap.size());
        m_sorter->sort(indexs);
        for(int i=0;i<indexs.size();i++)
            m_indexs.replace(start+i,indexs[i]);
    }

    if(m_stkMap.size()>0)
        emit dataChanged(index(start,1),index(start+m_stkMap.size()-1,columnCount()-1));
}

void IndexItemModel::clearData()
{
    if(m_indexs.size()<1) return;
    this->beginResetModel();
    m_indexs.clear();
    m_stkMap.clear();
    this->endResetModel();
}

int  IndexItemModel::rowCount(const QModelIndex &parent) const
{
    return m_indexs.size();
}

QVariant  IndexItemModel::colIdxData(const int &row, const ColumnIdx &inColumn) const
{
    switch (inColumn) {
    case ColPlainPrice:
        return m_indexs[row].pre_close_price;
        break;
    case ColStockCode:
        return QString::fromStdString(m_indexs[row].stock_code);
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant  IndexItemModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col  = index.column();
    ColumnIdx colIdx = columnIdx(col);

    if(role == Stock_Data_Role){
        switch(colIdx){
        case ColNo:                    return row+1;
        case ColStockCode:       return QString::fromStdString(m_indexs[row].stock_code);
        case ColStockName:      return QString::fromStdString(m_indexs[row].stock_name);

        case ColPrice:         return m_indexs[row].last_price;
        case ColUpDownPrice:   return m_indexs[row].up_down_price;
        case ColRate:                return m_indexs[row].rate;

        case ColTurnover:          return m_indexs[row].turnover;

        case ColHighestPrice:       return m_indexs[row].highest_price;
        case ColLowestPrice:        return m_indexs[row].lowest_price;
        case ColOpenPrice:           return m_indexs[row].open_price;
        case ColPlainPrice:     return m_indexs[row].pre_close_price;
        }
    }
    return QVariant();
}


///OptionIndexItemModel---
void OptionIndexItemModel::updateData(const char *data,int start,int num,bool first)
{
    if((start+num) > m_indexs.size()) return;
    HZData::Index *tmpIndexs = (HZData::Index *)data;
    if(first){
        m_stkMap.clear();
        for(int i=0;i<num;i++){
            m_indexs[start+i] = tmpIndexs[i];
            m_stkMap.emplace(tmpIndexs[i].stock_code,start+i);
        }
    }else{
        for(int i=0;i<num;i++){
            auto it = m_stkMap.find(tmpIndexs[i].stock_code);
            if(it != m_stkMap.end())
                m_indexs.replace(it->second,tmpIndexs[i]);
        }
    }
    if(m_stkMap.size()>0)
        emit dataChanged(index(start,1),index(start+m_stkMap.size()-1,columnCount()-1));
}



///BKAndStockItemModel---
BKAndStockItemModel::BKAndStockItemModel(const QString &modelName,QObject *parent)
    :StockItemModel(modelName,parent)
{

}

void BKAndStockItemModel::updateData(const char *data,int start,int num,bool first)
{
    if((start+num)>m_stocks.size()) return;
    //qDebug()<<"BKAndStockItemModel update data:"<<start<<num<<first;
    HZData::Stock *tmpStocks = (HZData::Stock *)data;
    if(first){
        m_stkMap.clear();
        //更新第一个
        m_stocks[0] = tmpStocks[0];
        m_stkMap.emplace(tmpStocks[0].stock_code,0);
        num--;
        for(int i=1;i<=num;i++){
            m_stocks[start+i] = tmpStocks[i];
        }
        QVector<HZData::Stock>  stocks = m_stocks.mid(start+1,num);
        //排序
        m_sorter->sort(stocks);

        //更新数据
        for(int i=1;i<=num;i++){
            m_stocks.replace(start+i,stocks[i-1]);
            m_stkMap.emplace(stocks[i-1].stock_code,start+i);
        }

    }else{
        for(int i=0;i<num;i++){
            auto it = m_stkMap.find(tmpStocks[i].stock_code);
            if(it != m_stkMap.end())
                m_stocks.replace(it->second,tmpStocks[i]);
        }

        //需要排序，先取本地当前段数据， 然后排序
        QVector<HZData::Stock> stocks = m_stocks.mid(start+1,m_stkMap.size()-1);
        m_sorter->sort(stocks);
        for(int i=1;i<=stocks.size();i++)
            m_stocks.replace(start+i,stocks[i-1]);
    }
    emit dataChanged(index(0,1),index(0,columnCount()-1));
    emit dataChanged(index(start+1,1),index(start+m_stkMap.size()-1,columnCount()-1));
}

void BKAndStockItemModel::updateData(int idx,const HZData::Stock &stock)
{
    m_stocks.replace(idx,stock);
    emit dataChanged(index(idx,1),index(idx,columnCount()-1));
}









