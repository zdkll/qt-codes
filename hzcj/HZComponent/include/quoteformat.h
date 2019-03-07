#ifndef QUOTEFORMAT_H
#define QUOTEFORMAT_H
/********************************************************************
created: 用于统一客户端数据字段格式化。
author: min.cheng
purpose:
*********************************************************************/
#include <QString>
#include <QDateTime>
#include <datamodeldef.h>

//保留小数位数
#define DECIMAL_POINT 2
//五档买卖盘档数 目前五档
#define MMP_NUM 5
//初始数据格式化值
#define DEFAULT_STR_PRICE Default_Item_String
//上三角
#define ARROW_UP QStringLiteral("▲")
//下三角
#define ARROW_DOWN QStringLiteral("▼")


enum QuoteFieldType
{
    FieldIdDateTime		= 0,						// 1时间
    FieldIdLastpx,									// 2最新价
    FieldIdAveragepx,									// 3均价
    FieldIdChange,									// 4涨跌
    FieldIdPrevClosepx,								// 5前收
    FieldIdChangeRate,								// 6涨跌幅
    FieldIdOpenpx,									// 7开盘
    FieldIdHighpx,									// 8最高
    FieldIdLimitUp,									// 9涨停
    FieldIdLowpx,										// 10最低
    FieldIdLimitDown,									// 11跌停
    FieldIdVolCh,										// 12换手
    FieldIdVolRate,									// 13量比
    FieldIdAmplitude,									// 14振幅
    FieldIdTradeMoney,								// 15总成交额
    FieldIdAllVol,									// 16总手
    FieldIdTradeVol,									// 17成交量
    FieldIdEntrustPercent,							// 18委比
    FieldIdIn,										// 19内盘
    FieldIdAllGuMoney,								// 20总股本
    FieldIdOut,										// 21外盘
    FieldIdAllValue,									// 22总市值
    FieldIdValueRate,									// 23市净率
    FieldIdValueProDy,								// 24市盈动
    FieldIdDyGuMoney,									// 25流通股本
    FieldIdValueProStatic,							// 26市盈静
    FieldIdDyValue,									// 27流通市值
    FieldIdRise,										// 28上涨
    FieldIdFall,										// 29下跌
    FieldIdFlat,										// 30平盘
    FieldIdPrevSettlementpx,							// 31昨结算
    FieldIdSettlement,								// 32结算
    FieldIdClosepx,									// 33收盘
    FieldIdVolume,									// 34成交量
    FieldIdAmount,									// 35成交额
    FieldIdVolumeNow,									// 36现量
    FieldIdAmountNow,									// 37现额
    FieldIdBidpx,										// 38买
    FieldIdAskpx,										// 39卖

    FieldIdBidpx1,									// 40买一
    FieldIdBidpx2,									// 41买二
    FieldIdBidpx3,									// 42买三
    FieldIdBidpx4,									// 43买四
    FieldIdBidpx5,									// 44买五

    FieldIdAskpx1,									// 45卖一
    FieldIdAskpx2,									// 46卖二
    FieldIdAskpx3,									// 47卖三
    FieldIdAskpx4,									// 48卖四
    FieldIdAskpx5,									// 49卖五

    FieldIdBidVolume1,								// 50买一量
    FieldIdBidVolume2,								// 51买二量
    FieldIdBidVolume3,								// 52买三量
    FieldIdBidVolume4,								// 53买四量
    FieldIdBidVolume5,								// 54买五量

    FieldIdAskVolume1,								// 55卖一量
    FieldIdAskVolume2,								// 56卖二量
    FieldIdAskVolume3,								// 57卖三量
    FieldIdAskVolume4,								// 58卖四量
    FieldIdAskVolume5,								// 59卖五量
    FieldIdEntrustDiff,								// 60委差
    FieldIdPosition,							    // 61持仓
    FieldIdNetAssets,							    // 62净资产
    FieldIdRoe,                                     // 63 ROE
    FieldIdIncome,                                  // 64 收益
    FieldIdState,                                   // 65 交易状态
    FieldIdStockName,                               // 66 股票名称
    FieldIdLastField								// 最后一个
};

namespace HzQuoteFormat
{

/**
 * @brief  用于浮点数比较
 * @param[in] fa
 * @param[in] fb
 * @retval  返回0表示相等 1 表示fa大于fb 返回-1表示fa小于fb
 */
int compareFloat(double fa,double fb,double epsinon = 0.00001f);

enum UnitType
{
    SigUnit = 0,
    TenThousands = 1, //万
    Hundmillion = 2,     //亿
};

//单位名称
extern QHash<UnitType,QString>  unitHash;

//单位数量级
extern QHash<UnitType,long long>  unitMagnitudeHash;

//计算值对应的单位
extern  UnitType calUnitByValue(long long value);

extern  QString unitString(UnitType type);

extern  long long unitMagnitude(UnitType type);

extern  QString formatByUnitType(long long value,int unitType,int dot_num  =2);

//用于数据 万/亿/万亿数据格式化
class StringUnitFormat
{
public:
    static QString   toFormatString(const qulonglong &in,int pointNum = 2);

    static QString   toFormatString(const qlonglong &in,int pointNum = 2);

    static QString   toFormatString(const double &in,int pointNum = 2);

    static QString   toFormatString(const UINT32 &in,int pointNum = 2);
};

//股票方向
enum QuoteDirc
{
    quote_up=0,    //上涨
    quote_down,    //下跌
    quote_equal,   //相等
    quote_unknown, //无方向概念
};

//行情字段
struct QuoteItem
{
    QuoteDirc dirc = quote_unknown;        //字段方向
    QString strValue = DEFAULT_STR_PRICE;      //格式化后的值
    QString getDirc()
    {
        QString strDirc = "quote_equal";
        switch (dirc) {
        case HzQuoteFormat::quote_up:
            strDirc = "quote_up";
            break;
        case HzQuoteFormat::quote_down:
            strDirc = "quote_down";
            break;
        default:
            break;
        }
        return strDirc;
    }
};

class QuoteFormat
{
public:

    /**
     * @brief 格式化股票代码
     * @param[in]  股票代码
     * @retval 成功则输出： xxxxxx.SH 否则输出 --
     */
    static QString formatObjCode(const QString& obj)
    {
        QString retStr = Default_Item_String;
        if(!obj.isEmpty())
        {
            retStr = obj.mid(2)+".";
            QString preStr = obj.mid(0,2);
            preStr = preStr.toUpper();
            retStr += preStr;
        }
        return retStr;
    }

    /**
     * @brief 基本格式化方法
     * @param[in] 需要格式的数据
     * @param[in] 保留的小数位数
     * @retval  返回格式化后的数据
     */
    static QString QuoteFormatFun(const double& val, const double& initVal = INVALID_DOUBLE,const int num = 2)
    {
        QString strRet = Default_Item_String;
        if(compareFloat(val,initVal) != 0)
        {
            strRet = QString::number(val,'f',num);
        }
        return strRet;
    }

    /**
     * @brief 数字格式化，并保留指定小数位
     * @param[in] val
     * @param[in] num 小数位数
     * @retval 保留好小数位以后的数字
     */
    static double numericFormatToDouble(const double& val,const int& num)
    {
        double retf = INVALID_DOUBLE;
        if(val != INVALID_DOUBLE)
        {
            QString strRet = QString::number(val,'f',num);
            retf = strRet.toDouble();
        }
        return retf;
    }

    /**
     * @brief 数字格式化，并保留指定小数位
     * @param[in] val
     * @param[in] num 小数位数
     * @retval 返回格式化后的字符串
     */
    static QString numericFormatToString(const double& val,const int& num)
    {
        QString strRet = DEFAULT_STR_PRICE;
        if(val != INVALID_DOUBLE)
        {
            strRet = QString::number(val,'f',num);
        }
        return strRet;
    }

    /**
     * @brief 价格格式化 比如类似于高开低收，均价等
     */
    static QuoteItem formatPrice(double close,double preClose,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(close !=  DBL_MAX && preClose != DBL_MAX && preClose != 0 && close != 0)
        {
//            close = numericFormatToDouble(close,num);
//            preClose = numericFormatToDouble(preClose,num);
            if(close > preClose)
            {
                item.dirc = quote_up;
            }else if(close < preClose)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = QString::number(close,'f',num);
        }
        return item;
    }
    static QuoteItem formatPriceWithArrow(double close,double preClose,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(close !=  DBL_MAX && preClose != DBL_MAX && preClose != 0 && close != 0)
        {
//            close = numericFormatToDouble(close,num);
//            preClose = numericFormatToDouble(preClose,num);
            item.strValue = QString::number(close,'f',num);
            if(close > preClose)
            {
                item.dirc = quote_up;

                //                item.strValue += QStringLiteral("↑");
            }else if(close < preClose)
            {
                item.dirc = quote_down;
                //                item.strValue += QStringLiteral("↓");
            }else
            {
                item.dirc = quote_equal;
            }
        }
        return item;
    }

    /**
     * @brief 涨跌格式化
     */
    static QuoteItem formatChange(double close,double preClose,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(close !=  DBL_MAX && preClose != DBL_MAX && preClose != 0 && close != 0)
        {
//            close = numericFormatToDouble(close,num);
//            preClose = numericFormatToDouble(preClose,num);
            if(close > preClose)
            {
                item.dirc = quote_up;
            }else if(close < preClose)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = QString::number(close-preClose,'f',num);
        }
        return item;
    }
    static QuoteItem formatChange(double up_down_price,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(INVALID_DOUBLE != up_down_price)
        {
            //up_down_price = numericFormatToDouble(up_down_price,num);
            if(up_down_price > 0)
            {
                item.dirc = quote_up;
            }else if(up_down_price < 0)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = QString::number(up_down_price,'f',num);
        }
        return item;
    }
    static QuoteItem formatChangeWithArrow(double up_down_price,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(INVALID_DOUBLE != up_down_price)
        {
            //up_down_price = numericFormatToDouble(up_down_price,num);

            if(up_down_price > 0)
            {
                item.dirc = quote_up;
                item.strValue = ARROW_UP;
                item.strValue.append(QString::number(abs(up_down_price),'f',num));
            }else if(up_down_price < 0)
            {
                item.dirc = quote_down;
                item.strValue = ARROW_DOWN;
                item.strValue.append(QString::number(abs(up_down_price),'f',num));
            }else
            {
                item.dirc = quote_equal;
                item.strValue = QString::number(abs(up_down_price),'f',num);
            }
        }
        return item;
    }
    /**
     * @brief 涨跌幅格式化
     */
    static QuoteItem formatChangeRate(double close,double preClose,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(close !=  DBL_MAX && preClose != DBL_MAX && preClose != 0 && close != 0)
        {
//            close = numericFormatToDouble(close,num);
//            preClose = numericFormatToDouble(preClose,num);
            if(close > preClose)
            {
                item.dirc = quote_up;
            }else if(close < preClose)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = QString::number(abs(close-preClose)/preClose*100,'f',num);
            item.strValue.append("%");
        }
        return item;
    }
    static QuoteItem formatChangeRate(double rise_fall_ratio,double close,double preClose,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(rise_fall_ratio != INVALID_DOUBLE && close !=  INVALID_DOUBLE && preClose != INVALID_DOUBLE
                /*&&rise_fall_ratio != 0 && preClose != 0 && close != 0*/)
        {
//            close = numericFormatToDouble(close,num);
//            preClose = numericFormatToDouble(preClose,num);
            if(close > preClose)
            {
                item.dirc = quote_up;
            }else if(close < preClose)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = numericFormatToString(rise_fall_ratio,num);
            item.strValue.append("%");
        }
        return item;
    }


    /**
     * @brief 成交额格式化
     */
    static QuoteItem formatTurnover(double turnover,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(turnover !=  INVALID_DOUBLE )
        {
            item.strValue = StringUnitFormat::toFormatString(turnover,num);
        }
        return item;
    }

    /**
     * @brief 总手
     */
    static QuoteItem formatVolume(double vol,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(vol !=  INVALID_DOUBLE)
        {
            vol /= 100.0f; //一百股为一手
            item.strValue = StringUnitFormat::toFormatString(vol,num);
        }
        return item;
    }
    static QuoteItem formatVolume(UINT64 vol,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(vol !=  INVALID_UINT64)
        {
            double fVol = vol;
            fVol /= 100.0f; //一百股为一手
            item.strValue = StringUnitFormat::toFormatString(fVol,num);
        }
        return item;
    }

    /**
     * @brief 时间格式化
     * @param[in] time 秒数
     * @param[in] format 格式化样式
     * @retval
     */
    static QuoteItem formatTime(double time,const QString& format="yyyy-MM-dd hh:mm")
    {
        QuoteItem item;
        if(time != INVALID_DOUBLE)
        {
            QDateTime nt;
            nt.setTime_t(time);
            item.strValue = nt.toString(format);
        }
        return item;
    }
    static QuoteItem formatTime(UINT32 time,const QString& format="yyyy-MM-dd hh:mm")
    {
        QuoteItem item;
        if(time != INVALID_UINT32)
        {
            QDateTime nt;
            nt.setTime_t(time);
            item.strValue = nt.toString(format);
        }
        return item;
    }

    /**
     * @brief  委比格式化
     */
    static QuoteItem formatCommittee(double val,int num = DECIMAL_POINT)
    {
        QuoteItem item;
        if(INVALID_DOUBLE != val)
        {
            //val = numericFormatToDouble(val,num);
            if(val > 0)
            {
                item.dirc = quote_up;
            }else if(val < 0)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = QString::number(val, 'f', 2);
            item.strValue += "%";
        }
        return item;
    }

    /**
     * @brief 委差
     */
    static QuoteItem formatEntrustdiff(INT64 value)
    {
        QuoteItem item;
        if(value != INVALID_INT64)
        {
            if(value > 0)
            {
                item.dirc = quote_up;
            }else if(value < 0)
            {
                item.dirc = quote_down;
            }else
            {
                item.dirc = quote_equal;
            }
            item.strValue = QString::number(value/100,10);
        }
        return item;
    }

    /**
     * @brief 内盘格式化
     */
    static QuoteItem formatInVolume(UINT64 vol)
    {
        QuoteItem item;
        if(INVALID_INT64 != vol)
        {
            vol /= 100;
            item.strValue = StringUnitFormat::toFormatString(vol);
            item.dirc = quote_down;
        }
        return item;
    }

    /**
     * @brief 外盘格式化
     */
    static QuoteItem formatOutVolume(UINT64 vol)
    {
        QuoteItem item;
        if(INVALID_INT64 != vol)
        {
            vol /= 100;
            item.strValue = StringUnitFormat::toFormatString(vol);
            item.dirc = quote_up;
        }
        return item;
    }

    /**
     * @brief  总股本
     */
    static QuoteItem formatAllGuMoney(UINT64 value)
    {
        QuoteItem item;
        if(INVALID_INT64 != value)
        {
            item.strValue = StringUnitFormat::toFormatString(value);
        }
        return item;
    }

    /**
     * @brief 交易状态格式化
     */
    static QuoteItem formatTradeStatus(INT32 value)
    {
        QuoteItem item;
        if(INVALID_INT32 == value)
            return item;
        if(HZData::DealStatusS == value  )
        {
            item.strValue = QStringLiteral("开市前");
        }
        else if(HZData::DealStatusC == value  )
        {
            item.strValue = QStringLiteral("集合竞价");
        }
        else if(HZData::DealStatusT == value  )
        {
            item.strValue = QStringLiteral("交易中");
        }
        else if(HZData::DealStatusB == value  )
        {
            item.strValue = QStringLiteral("休市");
        }
        else if(HZData::DealStatusE == value  )
        {
            item.strValue = QStringLiteral("收盘");
        }
        else if(HZData::DealStatusP == value  )
        {
            item.strValue = QStringLiteral("停牌");
        }
        return item;
    }


    /**
     * @brief 交易状态格式化
     */
    static QuoteItem formatIntData(INT32 value)
    {
        QuoteItem item;
        if(INVALID_INT32 == value  || value < 0)
            return item;

        if(value > 0)
        {
            item.dirc = quote_up;
        }else if(value < 0)
        {
            item.dirc = quote_down;
        }else
        {
            item.dirc = quote_equal;
        }
        item.strValue = QString::number(value,10);
        return item;
    }

    /**
     * @brief  格式化股票字段
     * @param[in] stock 股票数据
     * @param[in] type 即将格式化字段的类型
     * @retval   返回指定字段格式化后的数据
     */
    static QuoteItem format(const HZData::AllStock& stock,int type)
    {
        QuoteItem item;
        switch (type) {
        case FieldIdDateTime:
            break;
        case FieldIdLastpx:                                   //最新价
            item = formatPrice(stock.last_price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdAveragepx:						        // 3均价
            item = formatPrice(stock.avg_price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdChange:									// 4涨跌
            item = formatChangeWithArrow(stock.up_down_price,DECIMAL_POINT);
            break;
        case FieldIdPrevClosepx:								// 5前收
            item.strValue = numericFormatToString(stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdChangeRate:								// 6涨跌幅
            item = formatChangeRate(stock.rate,stock.last_price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdOpenpx:									// 7开盘
            item = formatPrice(stock.open_price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdHighpx:									// 8最高
            item = formatPrice(stock.highest_price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdLimitUp:									// 9涨停
            item.dirc = quote_up;
            item.strValue = numericFormatToString(stock.upper_limit_rice,DECIMAL_POINT);
            break;
        case FieldIdLowpx:									// 10最低
            item = formatPrice(stock.lowest_price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdLimitDown:								// 11跌停
            item.dirc = quote_down;
            item.strValue = numericFormatToString(stock.down_limit_price,DECIMAL_POINT);
            break;
        case FieldIdVolCh:									// 12换手
            item.strValue = numericFormatToString(stock.turnover_rate,DECIMAL_POINT);
            if(item.strValue != DEFAULT_STR_PRICE)
                item.strValue.append("%");
            break;
        case FieldIdVolRate:									// 13量比
            item.strValue = numericFormatToString(stock.ratio,DECIMAL_POINT);
            break;
        case FieldIdAmplitude:								// 14振幅
            item.strValue = numericFormatToString(stock.sa,DECIMAL_POINT);
            if(item.strValue != DEFAULT_STR_PRICE)
                item.strValue.append("%");
            break;
        case FieldIdTradeMoney:					// 15总成交额
            break;
        case FieldIdAllVol:									// 16总手
            item = formatVolume(stock.volume);
            break;
        case FieldIdTradeVol:						// 17成交量
            break;
//        case FieldIdEntrustPercent:							// 18委比
//            item = formatCommittee(stock.committee,DECIMAL_POINT);
//            break;
        case FieldIdIn:										// 19内盘
            item = formatInVolume(stock.np_volume);
            break;
        case FieldIdAllGuMoney:								// 20总股本
            item = formatAllGuMoney(stock.tot_stock);
            break;
        case FieldIdOut:										// 21外盘
            item = formatOutVolume(stock.wp_volume);
            break;
        case FieldIdAllValue:									// 22总市值
            item.strValue = StringUnitFormat::toFormatString(stock.tot_value);
            break;
        case FieldIdValueRate:								// 23市净率
            item.strValue = StringUnitFormat::toFormatString(stock.pbv);
            break;
        case FieldIdValueProDy:								// 24市盈动
            item.strValue = StringUnitFormat::toFormatString(stock.ped);
            break;
        case FieldIdDyGuMoney:								// 25流通股本
            item.strValue = StringUnitFormat::toFormatString(stock.cir_stock);
            break;
        case FieldIdValueProStatic:							// 26市盈静
            item.strValue = StringUnitFormat::toFormatString(stock.cir_stock);
            break;
        case FieldIdDyValue:									// 27流通市值
            item.strValue = StringUnitFormat::toFormatString( stock.cir_value);
            break;
        case FieldIdRise:										// 28上涨
            break;
        case FieldIdFall:										// 29下跌
            break;
        case FieldIdFlat:										// 30平盘
            break;
        case FieldIdPrevSettlementpx:							// 31昨结算
            break;
        case FieldIdSettlement:								// 32结算
            break;
        case FieldIdClosepx:									// 33收盘
            break;
        case FieldIdVolume:									// 34成交量
            item = formatVolume(stock.volume);
            break;
        case FieldIdAmount:									// 35成交额
            item.strValue = StringUnitFormat::toFormatString(stock.turnover);
            break;
        case FieldIdVolumeNow:								// 36现量
            break;
        case FieldIdAmountNow:								// 37现额
            break;
        case FieldIdBidpx1:									// 40买一
        case FieldIdBidpx2:									// 41买二
        case FieldIdBidpx3:									// 42买三
        case FieldIdBidpx4:									// 43买四
        case FieldIdBidpx5:									// 44买五
            item = formatPrice(stock.stk_buy[type - FieldIdBidpx1].price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdAskpx1:									// 45卖一
        case FieldIdAskpx2:									// 46卖二
        case FieldIdAskpx3:									// 47卖三
        case FieldIdAskpx4:									// 48卖四
        case FieldIdAskpx5:									// 49卖五
            item = formatPrice(stock.stk_sell[type - FieldIdAskpx1].price,stock.pre_close_price,DECIMAL_POINT);
            break;
        case FieldIdBidVolume1:								// 50买一量
        case FieldIdBidVolume2:								// 51买二量
        case FieldIdBidVolume3:								// 52买三量
        case FieldIdBidVolume4:								// 53买四量
        case FieldIdBidVolume5:								// 54买五量
        {
            const UINT64& vol = stock.stk_buy[type - FieldIdBidVolume1].volume;
            if(vol != INVALID_UINT64 && vol != 0)
            {
                item.strValue = QString::number(vol / 100, 10);
            }
        }
            break;
        case FieldIdAskVolume1:								// 55卖一量
        case FieldIdAskVolume2:								// 56卖二量
        case FieldIdAskVolume3:								// 57卖三量
        case FieldIdAskVolume4:								// 58卖四量
        case FieldIdAskVolume5:								// 59卖五量
        {
            const UINT64& vol = stock.stk_sell[type - FieldIdAskVolume1].volume;
            if(vol != INVALID_UINT64  && vol != 0)
            {
                item.strValue = QString::number(vol / 100, 10);
            }
        }
            break;
//        case FieldIdEntrustDiff:								// 60委差
//            item = formatEntrustdiff(stock.buy_sell_buy);
//            break;
        case FieldIdPosition:									// 61持仓
            break;
        case FieldIdNetAssets:								// 62净资产
            item.strValue = StringUnitFormat::toFormatString(stock.ro);
            break;
        case FieldIdRoe:                                      // 63 ROE
            item.strValue = StringUnitFormat::toFormatString(stock.roe);
            if(item.strValue != DEFAULT_STR_PRICE)
                item.strValue.append("%");
            break;
        case FieldIdIncome:                                   // 64 收益
            item.strValue = StringUnitFormat::toFormatString(stock.profit);
            break;
        case FieldIdState:                                    // 65 交易状态
            item = formatTradeStatus(stock.trade_status);
            break;
        case FieldIdStockName:                                // 66 股票名称
            item.strValue = stock.stock_name.c_str();
            break;
        default:
            break;
        }
        return item;
    }

    //    /**
    //     * @brief  格式化板块字段
    //     * @param[in] bkStock 股票数据
    //     * @param[in] type 即将格式化字段的类型
    //     * @retval   返回指定字段格式化后的数据
    //     */
    //    static QuoteItem format(const str_Block& stock,QuoteFieldType& type)
    //    {
    //        return QuoteItem();
    //    }
};

}


#endif // QUOTEFORMAT_H
