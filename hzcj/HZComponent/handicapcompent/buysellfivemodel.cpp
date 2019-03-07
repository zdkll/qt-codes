#include "buysellfivemodel.h"

BuySellFiveModel::BuySellFiveModel(int type, QObject *parent):
    QAbstractTableModel(parent),
    m_nType(type)
{
    m_buyList << "买一"
               <<"买二" <<"买三"
                <<"买四" <<"买五";

    m_sellList<< "卖一"
                <<"卖二" <<"卖三"
                 <<"卖四" <<"卖五";
    init();
}

int BuySellFiveModel::rowCount(const QModelIndex &parent) const
{
    return 5;
}

int BuySellFiveModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant BuySellFiveModel::data(const QModelIndex &index, int role) const
{
    int nRow = index.row();
    switch (role) {
    case MyDataRole:
        if(0 == m_nType)
        {
            return QVariant::fromValue(m_dataBuy[nRow]);
        }
        else
        {
            return QVariant::fromValue(m_dataSell[nRow]);
        }
        break;
    default:
        break;
    }
    return QVariant();
}

void BuySellFiveModel::updateData(const QVector<HZData::AllStock> &vec)
{
    formatData(vec[0]);
    m_stock = vec[0];
    refrush();
}

void BuySellFiveModel::refrush()
{
    beginResetModel();
    endResetModel();
}

void BuySellFiveModel::init()
{
    for(int i = 0; i < 5; i++)
    {
        m_dataBuy[i].nType = 0;
        m_dataBuy[i].strTitle = m_buyList[i];
        m_dataBuy[i].strPrice = Default_Item_String;
        m_dataBuy[i].strVolume = Default_Item_String;

        m_dataSell[i].nType = 1;
        m_dataSell[i].strTitle = m_sellList[i];
        m_dataSell[i].strPrice = Default_Item_String;
        m_dataSell[i].strVolume = Default_Item_String;
    }

    formatData(m_stock);
//    HZData::ReqOptionalDynaInfo reqDynaInfo;
//    reqDynaInfo.stock_code.push_back("sh600519");


    OnStockDataArrived  stockDataArrived = std::bind(&BuySellFiveModel::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
//zdk 接口修改，需要替换
    //    DataController::instance()->requestOptionalDyna(reqDynaInfo,stockDataArrived);
}

void BuySellFiveModel::formatData(const HZData::AllStock &stock)
{

    HzQuoteFormat::QuoteItem item;
    for(int i = 0; i < 5; i++)
    {

        //买一价
        item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdBidpx1 + i);
        m_dataBuy[i].strPrice = item.strValue;
        //买一量
        item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdBidVolume1 + i);
        m_dataBuy[i].strVolume = item.strValue;

        //卖一价
        item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAskpx1 + i);
        m_dataSell[i].strPrice = item.strValue;

        //卖一量
        item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAskVolume1 + i);
        m_dataSell[i].strVolume = item.strValue;

//        m_dataBuy[i].strPrice = QString::number(stock.stk_buy[i].price, 'f', 2);
//        m_dataBuy[i].strVolume = QString::number(stock.stk_buy[i].volume / 10, 10);

//        m_dataSell[i].strPrice = QString::number(stock.stk_sell[i].price, 'f', 2);
//        m_dataSell[i].strVolume = QString::number(stock.stk_sell[i].volume / 10, 10);
    }
    refrush();
}

void BuySellFiveModel::onStockOptionalDataArrived(ReqId id, const QVector<HZData::AllStock> &stock, ErrCode errCode)
{
    Q_UNUSED(id)
    if(0 == errCode)
    {
        if(stock.size() > 0)
        {
            m_stock = stock[0];
            formatData(stock[0]);
        }
    }
}
