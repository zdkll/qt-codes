#include "fundmodel.h"

#include "mvcpublicdef.h"

///HeaderItemModel----
FundHeaderItemModel::FundHeaderItemModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_priHeaderLbls<<QStringLiteral("实时超大单统计")
                  <<QStringLiteral("实时大单统计")
                 <<QStringLiteral("实时中单统计")
                <<QStringLiteral("实时小单统计");
    m_headerColCount = 4;
}

void FundHeaderItemModel::setColumnInfos(const QVector<ColumnInfo> &columnInfos)
{
    this->beginResetModel();
    m_columnInfos = columnInfos;
    this->endResetModel();
}

int FundHeaderItemModel::columnCount(const QModelIndex &) const
{
    return m_columnInfos.size();
}

int FundHeaderItemModel::rowCount(const QModelIndex &) const
{
    return 2;
}

void FundHeaderItemModel::setSortIndicator(const int &sortIndex,const Qt::SortOrder &sortOrder)
{
    int old_column = m_sortIndicator.column;
    m_sortIndicator.column  = sortIndex;
    m_sortIndicator.order     = sortOrder;
    //更新数据
    if(old_column >=0){
        dataChanged(index(0,old_column),index(1,old_column));
    }
    dataChanged(index(0,sortIndex),index(1,sortIndex));
}

QVariant FundHeaderItemModel::data(const QModelIndex &index, int role) const
{
    int row  = index.row();
    int column = index.column();
    //表头文字
    if(role == Money_Header_Data_Role)
    {
        if(row == 0)
        {
            if(column<m_headerColCount)
                return m_columnInfos[column].label;
            else{
                if((column-m_headerColCount)%4 == 0){
                    //qDebug()<<"header column:"<<column<<" priHeader:"<<(column+1-m_headerColCount)/4;
                    return m_priHeaderLbls[(column-m_headerColCount)/4];
                }
            }
        }
        else {
            if(column>=m_headerColCount)
                return m_columnInfos[column].label;
        }
    }
    //对齐
    else if(Money_Header_Alignment_Role == role)
    {
        if(row == 0){
            if(column<m_headerColCount)
                return int(m_columnInfos[column].alignment);
            else
                return Qt::AlignCenter;
        }else{
            return int(m_columnInfos[column].alignment);
        }
    }

    //是否该列排序
    else if(Money_Header_IsSortIndex_Role == role)
    {
        if(m_sortIndicator.column == column)
            if(column<m_headerColCount || row>0)
                return true;
        return false;
    }
    //升序还是降序
    if(Money_Header_SortOrder_Role == role)
        return int(m_sortIndicator.order);

    return QVariant();
}


///SLFundModel-------
void SLFundModel::initData(const char *data,int ,int num)
{
    HZData::MoneyNetput *netputs = (HZData::MoneyNetput *)data;
    this->beginResetModel();
    m_moneyNetput.resize(num);
    for(int i=0;i<num;i++)
        m_moneyNetput[i]  = netputs[i];

    this->endResetModel();
}

void SLFundModel::updateData(const char *data,int start,int num)
{
    HZData::MoneyNetput *netputs = (HZData::MoneyNetput *)data;
    if((start+num)>m_moneyNetput.size()) return;
    for(int i=start;i<num;i++)
        m_moneyNetput.replace(i,netputs[i]);

    emit dataChanged(index(start,0),index(start+num-1,this->columnCount()-1));
}

void SLFundModel::clearData()
{
    if(m_moneyNetput.size()<1) return;
    this->beginResetModel();
    m_moneyNetput.clear();
    this->endResetModel();
}

int SLFundModel::rowCount(const QModelIndex &parent) const
{
    return m_moneyNetput.size();
}

QVariant SLFundModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int row = index.row();
    ColumnIdx colIdx = columnIdx(index.column());

    if(role ==  Stock_Data_Role){
        switch(colIdx){
        case ColStockName: return  QString::fromStdString(m_moneyNetput[row].stock_name);
        case ColOneDayNetput:    return m_moneyNetput[row].one_money_netput;
        case ColThreeDayNetput:    return m_moneyNetput[row].three_money_netput;
        case ColFiveDayNetput:    return m_moneyNetput[row].five_money_netput;
        }
    }

    return  QVariant();
}

QVariant SLFundModel::colIdxData(const int &row, const ColumnIdx &inColumn) const
{
    if(ColStockCode==inColumn) return  QString::fromStdString(m_moneyNetput[row].stock_code);

    return QVariant();
}





///FundModel--------
void FundModel::initData(const QVector<HZData::StockCode > &stockCodes)
{
    this->beginResetModel();
    m_stockMoney.clear();
    m_stockMoney.resize(stockCodes.size());
    for(int i=0;i<stockCodes.size();i++){
        m_stockMoney[i].stock_code = stockCodes[i].stock_code;
        m_stockMoney[i].stock_name = stockCodes[i].stock_name;
    }
    this->endResetModel();
}

void FundModel::initData(const char *data,int start,int num)
{
    HZData::StockMoney* stockMoneys = (HZData::StockMoney*)data;
    this->beginResetModel();
    m_stockMoney.clear();
    m_stockMoney.resize(num);
    for(int i=start;i<num;i++)
        m_stockMoney[i] = stockMoneys[i];
    this->endResetModel();
}

void FundModel::updateData(const char *data,int start,int num)
{
    if(start + num>m_stockMoney.size()) return;
    HZData::StockMoney* stockMoneys = (HZData::StockMoney*)data;
    for(int i=0;i<num;i++){
        m_stockMoney.replace(start+i,stockMoneys[i]);
    }
    emit dataChanged(index(start,0),index(start+num-1,this->columnCount()-1));
}

void FundModel::removeStock(const QString& stockCode)
{
    int row = -1;
    std::string stock_code = stockCode.toStdString();
    for(int i=0;i<m_stockMoney.size();i++){
        if(m_stockMoney[i].stock_code == stock_code){
            row = i;break;
        }
    }
    if(row>=0){
        this->beginRemoveRows(QModelIndex(),row,row);
        m_stockMoney.remove(row);
        this->endRemoveRows();
    }
}

void FundModel::clearData()
{
    if(m_stockMoney.size()<1)return;
    this->beginResetModel();
    m_stockMoney.clear();
    this->endResetModel();
}

int FundModel::rowCount(const QModelIndex & ) const
{
    return m_stockMoney.size();
}

QVariant  FundModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ColumnIdx colIdx = columnIdx(index.column());

    if(role ==  Stock_Data_Role)
    {
        switch(colIdx){
        case ColNo: return  row;
        case ColBKStockCode:
        case ColStockCode: return  QString::fromStdString(m_stockMoney[row].stock_code);
        case ColStockName: return  QString::fromStdString(m_stockMoney[row].stock_name);

        case ColPrice:         return m_stockMoney[row].money_one_day.last_price;
        case ColUpDownPrice:   return m_stockMoney[row].money_one_day.up_down_price;
        case ColRate:                return m_stockMoney[row].money_one_day.rate;
        case ColAuctionVolume: return  m_stockMoney[row].money_one_day.auction_volume;
        case ColLeaderNetput:   return m_stockMoney[row].money_one_day.leader_money.money_netput;
        case ColThreeLeaderNetput: return m_stockMoney[row].three_money_netput;
        case ColFiveLeaderNetput:   return m_stockMoney[row].five_money_netput;
            //超大单
        case ColMaxInput:    return m_stockMoney[row].money_one_day.maxord_money.money_input;
        case ColMaxOutput: return m_stockMoney[row].money_one_day.maxord_money.money_output;
        case ColMaxNetput: return  m_stockMoney[row].money_one_day.maxord_money.money_netput;
        case ColMaxRate:    return m_stockMoney[row].money_one_day.maxord_money.money_rate;
            //大单
        case ColBigInput:    return m_stockMoney[row].money_one_day.bigord_money.money_input;
        case ColBigOutput: return m_stockMoney[row].money_one_day.bigord_money.money_output;
        case ColBigNetput: return  m_stockMoney[row].money_one_day.bigord_money.money_netput;
        case ColBigRate:    return m_stockMoney[row].money_one_day.bigord_money.money_rate;
            //中单
        case ColMidInput:    return m_stockMoney[row].money_one_day.midord_money.money_input;
        case ColMidOutput: return m_stockMoney[row].money_one_day.midord_money.money_output;
        case ColMidNetput: return  m_stockMoney[row].money_one_day.midord_money.money_netput;
        case ColMidRate:    return m_stockMoney[row].money_one_day.midord_money.money_rate;
            //小单
        case ColMinInput:    return m_stockMoney[row].money_one_day.minord_money.money_input;
        case ColMinOutput: return m_stockMoney[row].money_one_day.minord_money.money_output;
        case ColMinNetput: return  m_stockMoney[row].money_one_day.minord_money.money_netput;
        case ColMinRate:    return m_stockMoney[row].money_one_day.minord_money.money_rate;
        }
    }

    return  QVariant();
}


QVariant FundModel::colIdxData(const int &row, const ColumnIdx &inColumn) const
{
    if(ColStockCode==inColumn) return  QString::fromStdString(m_stockMoney[row].stock_code);
    else if(ColPreClosePrice == inColumn) return m_stockMoney[row].money_one_day.pre_close_price;
    return QVariant();
}


///BkStockFundModel
void BkStockFundModel::updateData(const char *data,int start,int num)
{
    if(num<1) return;
    HZData::StockMoney* stockMoneys = (HZData::StockMoney*)data;
    m_stockMoney.replace(0,stockMoneys[0]);

    for(int i=1;i<num;i++){
        m_stockMoney.replace(start+i,stockMoneys[i]);
    }
    emit dataChanged(index(0,0),index(0,this->columnCount()-1));
    emit dataChanged(index(start+1,0),index(start+num-1,this->columnCount()-1));
}

void  BkStockFundModel::clearData()
{
    if(m_stockMoney.size()<1) return;
    this->beginResetModel();
    m_stockMoney.clear();
    this->endResetModel();
}

