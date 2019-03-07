#include "delegatepool.h"

#include <QDebug>
#include "itemdelegates.h"

DelegatePool::~DelegatePool()
{
    qDeleteAll(m_delegatesMap);
}

DelegatePool  *DelegatePool::instance()
{
    static DelegatePool delegatePool;
    return &delegatePool;
}

StockStyledItemDelegate *DelegatePool::itemDelegate(const ColumnIdx &idx)
{
    auto  it = m_delegatesMap.find(idx);

    if(it != m_delegatesMap.end())
        return  it.value();

    //没有则创建
    StockStyledItemDelegate *delegate = 0;
    auto classit = sdelClassNameMap.find(idx);
    if(classit!= sdelClassNameMap.end()){
        delegate =  m_ioc.Resolve<StockStyledItemDelegate>(classit->second);
        m_delegatesMap.insert(idx,delegate);
    }

    return delegate;
}


StockStyledItemDelegate   *DelegatePool::getItemDelegate(const ColumnIdx &idx)
{
    return m_delegatesMap.value(idx,0);
}


DelegatePool::DelegatePool()
{
    //注册
    m_ioc.RegisterInherit<StockStyledItemDelegate,NoItemDelegate>("NoItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,ZeroNoItemDelegate>("ZeroNoItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,PlainTextItemDelegate>("PlainTextItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,CodeTextItemDelegate>("CodeTextItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,StockCodeTextItemDelegate>("StockCodeTextItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,BKCodeTextItemDelegate>("BKCodeTextItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,DateItemDelegate>("DateItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,FullDateItemDelegate>("FullDateItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,TradeDircItemDelegate>("TradeDircItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,NameTextItemDelegate>("NameTextItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,RatePriceItemDelegate>("RatePriceItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,PriceItemDelegate>("PriceItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,RiseFallItemDelegate>("RiseFallItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,PercentItemDelegate>("PercentItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,VolULonglongFormatItemDelegate>("VolULonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,VolLonglongFormatItemDelegate>("VolLonglongFormatItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,CurULonglongFormatItemDelegate>("CurULonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,LonglongFormatItemDelegate>("LonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,ULonglongFormatItemDelegate>("ULonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,DoubleSimpleItemDelegate>("DoubleSimpleItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,DoubleFormatItemDelegate>("DoubleFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,YuguDoubleSimpleItemDelegate>("YuguDoubleSimpleItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,FallLonglongFormatItemDelegate>("FallLonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,RiseLonglongFormatItemDelegate>("RiseLonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,NegFallDoubleFormatItemDelegate>("NegFallDoubleFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,RiseDoubleFormatItemDelegate>("RiseDoubleFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,FallDoubleFormatItemDelegate>("FallDoubleFormatItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,NpLonglongFormatItemDelegate>("NpLonglongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,WpLonglongFormatItemDelegate>("WpLonglongFormatItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,DvalLongLongFormatItemDelegate>("DvalLongLongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,DvalDoubleFormatItemDelegate>("DvalDoubleFormatItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,RiseFallNumItemDelegate>("RiseFallNumItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,RiseFallRatioItemDelegate>("RiseFallRatioItemDelegate");

    m_ioc.RegisterInherit<StockStyledItemDelegate,NwpRateItemDelegate>("NwpRateItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,BSBLongLongFormatItemDelegate>("BSBLongLongFormatItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,RiseDayItemDelegate>("RiseDayItemDelegate");

    // 资金相关
    m_ioc.RegisterInherit<StockStyledItemDelegate,ULongLongItemDelegate>("ULongLongItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,NegFallLonglongFormatItemDelegate>("NegFallLonglongFormatItemDelegate");
    //涨停监测
    m_ioc.RegisterInherit<StockStyledItemDelegate,TimeItemDelegate>("TimeItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,PlainIntItemDelegate>("PlainIntItemDelegate");
    m_ioc.RegisterInherit<StockStyledItemDelegate,FileItemDelegate>("FileItemDelegate");
}

std::unordered_map<ColumnIdx,std::string> DelegatePool::sdelClassNameMap =
{
    {ColNo,"NoItemDelegate"},
    {ColNoZero,"ZeroNoItemDelegate"},
    {ColStockCode,"StockCodeTextItemDelegate"},
    {ColBKCode,"BKCodeTextItemDelegate"},
    {ColBKStockCode,"CodeTextItemDelegate"},

    {ColStockName,"NameTextItemDelegate"},
    {ColRiseStock,"NameTextItemDelegate"},

    {ColFile ,"FileItemDelegate"},
    {ColDate ,"DateItemDelegate"},
    {ColFullDate ,"FullDateItemDelegate"},

    {ColPrice,"PriceItemDelegate"},
    {ColRatePrice,"RatePriceItemDelegate"},
    {ColUpDownPrice,"RiseFallItemDelegate"},

    {ColRate,"RiseFallItemDelegate"},
    {ColRatePercent,"PercentItemDelegate"},
    {ColRateYearYet,"RiseFallItemDelegate"},
    {ColRiseStockRate,"RiseFallItemDelegate"},

    {ColVolume,"VolULonglongFormatItemDelegate"},
    {ColCurVolume,"CurULonglongFormatItemDelegate"},

    {ColTurnoverRate,"DoubleSimpleItemDelegate"},
    {ColTurnover,"DoubleFormatItemDelegate"},

    {ColHighestPrice,"PriceItemDelegate"},
    {ColLowestPrice,"PriceItemDelegate"},
    {ColOpenPrice,"PriceItemDelegate"},
    {ColPlainPrice,"DoubleSimpleItemDelegate"},

    {ColRatio,"DoubleSimpleItemDelegate"},
    {ColCommittee,"RiseFallItemDelegate"},
    {ColAvgPrice,"PriceItemDelegate"},
    {ColNpVolume,"NpLonglongFormatItemDelegate"},
    {ColWpVolume,"WpLonglongFormatItemDelegate"},

    {ColPbv,"DoubleSimpleItemDelegate"},

    {ColTotStock,"LonglongFormatItemDelegate"},
    {ColTotValue,"DoubleFormatItemDelegate"},
    {ColCirStock,"LonglongFormatItemDelegate"},
    {ColCirValue,"DoubleFormatItemDelegate"},

    {ColBuyPrice,"PriceItemDelegate"},
    {ColSellPrice,"PriceItemDelegate"},
    {ColBuySellBuy,"BSBLongLongFormatItemDelegate"},

    {ColRiseFallNum,"RiseFallNumItemDelegate"},
    {ColRiseFallRatio,"RiseFallRatioItemDelegate"},

    {ColProfit,"DoubleSimpleItemDelegate"},
    {ColAvgStock,"LonglongFormatItemDelegate"},
    {ColPed,"DoubleSimpleItemDelegate"},
    {ColSa,"DoubleSimpleItemDelegate"},

    {ColSpeedRate,"RiseFallItemDelegate"},
    {ColThreeRate,"RiseFallItemDelegate"},
    {ColFiveRate,"RiseFallItemDelegate"},
    {ColThreeTuvRate,"DoubleSimpleItemDelegate"},
    {ColFiveTuvRate,"DoubleSimpleItemDelegate"},
    {ColVolStock,"ULonglongFormatItemDelegate"},

    {ColNwpRate,"NwpRateItemDelegate"},
    {ColRiseDayNum,"RiseDayItemDelegate"},
    {ColMainNetput,"DvalLongLongFormatItemDelegate"},
    {ColBlockName,"NameTextItemDelegate"},
    {ColTime,"TimeItemDelegate"},

    {ColSmkStockCode,"StockCodeTextItemDelegate"},//相似K线
    {ColSmkStockName,"NameTextItemDelegate"},

    {ColAuctionVolume,"ULongLongItemDelegate"},//资金
    {ColLeaderNetput,"DvalLongLongFormatItemDelegate"},
    {ColMaxInput,"RiseLonglongFormatItemDelegate"},
    {ColBigInput,"RiseLonglongFormatItemDelegate"},
    {ColMidInput,"RiseLonglongFormatItemDelegate"},
    {ColMinInput,"RiseLonglongFormatItemDelegate"},

    {ColMaxOutput ,"NegFallLonglongFormatItemDelegate"},
    {ColBigOutput ,"NegFallLonglongFormatItemDelegate"},
    {ColMidOutput ,"NegFallLonglongFormatItemDelegate"},
    {ColMinOutput ,"NegFallLonglongFormatItemDelegate"},

    {ColMaxNetput ,"DvalLongLongFormatItemDelegate"},
    {ColBigNetput ,"DvalLongLongFormatItemDelegate"},
    {ColMidNetput ,"DvalLongLongFormatItemDelegate"},
    {ColMinNetput ,"DvalLongLongFormatItemDelegate"},

    {ColThreeLeaderNetput ,"DvalLongLongFormatItemDelegate"},
    {ColFiveLeaderNetput ,"DvalLongLongFormatItemDelegate"},
    {ColOneDayNetput ,"DvalLongLongFormatItemDelegate"},
    {ColThreeDayNetput ,"DvalLongLongFormatItemDelegate"},
    {ColFiveDayNetput ,"DvalLongLongFormatItemDelegate"},

    {ColLeaderRate ,"RiseFallItemDelegate"},
    {ColMaxRate ,"RiseFallItemDelegate"},
    {ColBigRate ,"RiseFallItemDelegate"},
    {ColMidRate ,"RiseFallItemDelegate"},
    {ColMinRate ,"RiseFallItemDelegate"},

    {ColAuctionRate ,"RiseFallItemDelegate"}, //集合竞价
    {ColPreRate ,"RiseFallItemDelegate"},
    {ColLeaderFund ,"DvalLongLongFormatItemDelegate"},
    {ColAuctionVol ,"VolULonglongFormatItemDelegate"},
    {ColAutionQaunRatio ,"DoubleFormatItemDelegate"},
    {ColAuctionTurnover ,"DoubleFormatItemDelegate"},

    {ColRfmUpLimitOpenTime ,"TimeItemDelegate"}, //涨停监测
    {ColRfmDownLimitOpenTime ,"TimeItemDelegate"},
    {ColRfmUpLimitTime ,"TimeItemDelegate"},
    {ColRfmDownLimitTime ,"TimeItemDelegate"},
    {ColContUpLimitRate ,"PlainTextItemDelegate"},
    {ColContDownLimitRate ,"PlainTextItemDelegate"},
    {ColUpLimitVolume ,"VolULonglongFormatItemDelegate"},
    {ColDownLimitVolume ,"VolULonglongFormatItemDelegate"},
    {ColFriedTime ,"TimeItemDelegate"},
    {ColFriedNum ,"ULongLongItemDelegate"},
    {ColRfmHistCont ,"PlainTextItemDelegate"},

    {ColThirtyRate ,"RiseFallItemDelegate"},//资金异动
    {ColActiveNetput ,"DvalLongLongFormatItemDelegate"},
    {ColThirtyActiveNetput ,"DvalLongLongFormatItemDelegate"},
    {ColActiveRate ,"DoubleSimpleItemDelegate"},
    {ColThreeActiveNetput ,"DvalLongLongFormatItemDelegate"},
    {ColFiveActiveNetput ,"DvalLongLongFormatItemDelegate"},

    {ColNetput ,"DvalDoubleFormatItemDelegate"},//龙虎榜
    {ColTotalBuyIn ,"RiseDoubleFormatItemDelegate"},
    {ColTotalSellOut ,"NegFallDoubleFormatItemDelegate"},
    {ColYybName ,"NameTextItemDelegate"},
    {ColStockNum ,"ULongLongItemDelegate"},
    {ColBuyTurnover,"RiseDoubleFormatItemDelegate"},
    {ColSellTurnover,"FallDoubleFormatItemDelegate"},
    {ColSlFund,"DoubleFormatItemDelegate"},
    {ColNumber ,"ULongLongItemDelegate"},
    {ColTradeDirc ,"TradeDircItemDelegate"},

    {ColDealPrice ,"DoubleSimpleItemDelegate"},//大宗交易
    {ColClosePrice ,"DoubleSimpleItemDelegate"},
    {ColOverPriceRate ,"DoubleSimpleItemDelegate"},
    {ColYybSell ,"NameTextItemDelegate"},

    {ColRzmre ,"DoubleFormatItemDelegate"},//融资融券
    {ColRzjmre ,"DvalLongLongFormatItemDelegate"},
    {ColRzche ,"DoubleFormatItemDelegate"},
    {ColRzye ,"DoubleFormatItemDelegate"},
    {ColRqyl ,"LonglongFormatItemDelegate"},
    {ColRqmcl ,"LonglongFormatItemDelegate"},
    {ColRqchl ,"LonglongFormatItemDelegate"},
    {ColRzrqye ,"DoubleFormatItemDelegate"},

    {ColFxzs ,"DoubleFormatItemDelegate"},//新股日历
    {ColWsfxl ,"DoubleFormatItemDelegate"},
    {ColFxjg ,"YuguDoubleSimpleItemDelegate"},
    {ColSgsx ,"YuguDoubleSimpleItemDelegate"},
    {ColFxsyl ,"YuguDoubleSimpleItemDelegate"},
    {ColZqgbrq ,"FullDateItemDelegate"},
    {ColZqjkrq ,"FullDateItemDelegate"},
    {ColSsrq ,"FullDateItemDelegate"},
    {ColZql ,"DoubleSimpleItemDelegate"},
    {ColSl ,"PlainIntItemDelegate"},
    {ColTotalRate ,"RiseFallItemDelegate"},
    {ColMzyqhl ,"LonglongFormatItemDelegate"}
};
