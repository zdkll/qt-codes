#include "strategyrelationstockswnd.h"
#include <QVBoxLayout>
#include <QLabel>
#include <qfontmetrics.h>

#include "viewtipspanel.h"
#include "quoteformat.h"
#include "httpdata.h"

#define ChanceDataIsEmpty 0
#define ChanceDataIsErr 1
#define ChanceDataNoraml 2
StrategyRelationStocksWnd::StrategyRelationStocksWnd(QWidget *parent) : BaseWidget(parent)
{
    createWg();
}

void StrategyRelationStocksWnd::setDynaProvider(DynaDataProvider *pProvider)
{
    m_pConentWnd->setDynaProvider(pProvider);
    connect(pProvider, SIGNAL(signalUpdateData()),this, SLOT(slotsTimer()));
    m_pProvider = pProvider;
}

void StrategyRelationStocksWnd::afterActive()
{
     if(m_pInfoTipsPanel->getCurrType() == ChanceDataNoraml && m_stockList.isEmpty() == false)
     {
         m_pProvider->addsubstock(m_stockList);
         HZData::ReqOptionalFundInfo req;
         std::list<std::string> list;
         for(int i =0;i < m_stockList.size();i++)
             list.push_back(m_stockList[i].toStdString());
         req.stock_code =list;
         req.mask  = HZData::MoneyField_Mask;
         m_subFundReq = gHZDataController->subOptionalMoney(req,std::bind(&StrategyRelationStocksWnd::onOptionalFundArrived
                                                                                ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
     }
}

void StrategyRelationStocksWnd::afterDeactive()
{
    m_pProvider->removestock(m_stockList);
    m_stockList.clear();
    m_funMap.clear();
    if(m_subFundReq != -1)
    {
        DataController::instance()->cancelSub(m_subFundReq);
        m_subFundReq = -1;
    }
    m_pInfoTipsPanel->switchType(ChanceDataIsEmpty);
}

void StrategyRelationStocksWnd::setStockList(int code,const QVector<QString> &stockList)
{
    if(code != 0)
    {
        m_pInfoTipsPanel->switchType(ChanceDataIsErr);
        return;
    }else if(stockList.isEmpty())
    {
        m_pInfoTipsPanel->switchType(ChanceDataIsEmpty);
        return;
    }else
    {
        m_stockList =stockList;
        m_pInfoTipsPanel->switchType(ChanceDataNoraml);
        m_pConentWnd->setStockList(stockList);
    }
}

void StrategyRelationStocksWnd::slotsTimer()
{
    if(m_pInfoTipsPanel->getCurrType() == ChanceDataNoraml)
    {
        m_pConentWnd->refresh();
    }
}

void StrategyRelationStocksWnd::createWg()
{
    QVBoxLayout* pLayout  = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    //创建标题
    QLabel* pLable = new QLabel();
    pLable->setFixedHeight(28);
    pLable->setText(QStringLiteral("相关个股"));
    pLable->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    pLayout->addWidget(pLable);
    pLayout->addSpacing(1);

    //创建内容区域
    QVector<TipsItem> tipList;
    TipsItem tipEmpty;
    tipEmpty.btObjName = "dataEmptyBt";
    tipEmpty.descLabelName = "dataEmptyLable";
    tipEmpty.descText = QStringLiteral("当前暂无数据");
    tipEmpty.type = ChanceDataIsEmpty;
    tipList.push_back(tipEmpty);
    TipsItem tipErr;
    tipErr.btObjName = "dataErrBt";
    tipErr.descLabelName = "dataErrLable";
    tipErr.descText = QStringLiteral("操作失败，稍后重试");
    tipErr.type = ChanceDataIsErr;
    tipList.push_back(tipErr);

    m_pConentWnd =  new StrategyRelationStocksInnerWnd(m_funMap);
    m_pConentWnd->createWg();
    m_pInfoTipsPanel =new ViewTipsPanel(this);
    m_pInfoTipsPanel->setTipItems(tipList);
    m_pInfoTipsPanel->setNormalWnd(m_pConentWnd,ChanceDataNoraml);
    m_pInfoTipsPanel->createWg();
    pLayout->addWidget(m_pInfoTipsPanel);
}

void StrategyRelationStocksWnd::onOptionalFundArrived(ReqId, char *data, int num, ErrCode errCode)
{
    if(m_pInfoTipsPanel->getCurrType() == ChanceDataNoraml)
    {
        if(errCode != 0) return;
        HZData::StockMoney* stockMoneys = (HZData::StockMoney*)data;
        for(int i=0;i<num;i++){
           m_funMap[stockMoneys[i].stock_code.c_str()] = *stockMoneys;
        }
    }
}

StocksItemDrawer::StocksItemDrawer(StrategyRelationStocksInnerWnd *pWnd):
    m_pMainWnd(pWnd)
{

}

StocksItemDrawer::~StocksItemDrawer()
{

}

void StocksItemDrawer::update(const HZData::AllStock &stock,const HZData::StockMoney& fund,const QRect rc, QPainter &dc)
{
    m_rc = rc;
    m_stock = stock;
    m_fund = fund;
    redraw(dc);
}

void StocksItemDrawer::update(const HZData::AllStock &stock,const HZData::StockMoney& fund,QPainter &dc)
{
    m_stock = stock;
    m_fund = fund;
    redraw(dc);
}
HzQuoteFormat::QuoteItem formatFundValue(const HZData::StockMoney& fund)
{
    HzQuoteFormat::QuoteItem item;
    int64_t input = 0;
    if(fund.money_one_day.minord_money.money_input != INVALID_INT64)
        input += fund.money_one_day.minord_money.money_input;
    if(fund.money_one_day.midord_money.money_input != INVALID_INT64)
        input += fund.money_one_day.midord_money.money_input;
    if(fund.money_one_day.bigord_money.money_input != INVALID_INT64)
        input += fund.money_one_day.bigord_money.money_input;
    if(fund.money_one_day.maxord_money.money_input != INVALID_INT64)
        input += fund.money_one_day.maxord_money.money_input;

    int64_t output = 0;
    if(fund.money_one_day.minord_money.money_output != INVALID_INT64)
        output += fund.money_one_day.minord_money.money_output;
    if(fund.money_one_day.midord_money.money_input != INVALID_INT64)
        output += fund.money_one_day.midord_money.money_output;
    if(fund.money_one_day.bigord_money.money_input != INVALID_INT64)
        output += fund.money_one_day.bigord_money.money_output;
    if(fund.money_one_day.maxord_money.money_input != INVALID_INT64)
        output += fund.money_one_day.maxord_money.money_output;

    int64_t value = input-output;
    if(input == 0 && output == 0)
        value = INVALID_INT64;
    if(value == INVALID_INT64)
        return item;
    item.strValue = HzQuoteFormat::StringUnitFormat::toFormatString(value);
    if(value > 0)
        item.dirc = HzQuoteFormat::quote_up;
    else if(value < 0)
        item.dirc = HzQuoteFormat::quote_down;
    else
        item.dirc = HzQuoteFormat::quote_equal;
    return item;
}

void changeItemClr(HzQuoteFormat::QuoteItem& item,QPainter &dc,StrategyRelationStocksInnerWnd* pMainWnd)
{

    if(item.dirc == HzQuoteFormat::quote_up)
    {
        dc.setPen(pMainWnd->getUpClr());
    }else if(item.dirc == HzQuoteFormat::quote_down)
    {
        dc.setPen(pMainWnd->getDownClr());
    }else
    {
        dc.setPen(pMainWnd->getEqualClr());
    }
}

void StocksItemDrawer::redraw(QPainter &dc)
{
    if(m_stock.stock_code.empty())
        return;
    QString stockCode = m_stock.stock_code.c_str();
    stockCode = stockCode.mid(2,stockCode.length()-2);
    QFont stockNameFont;
    stockNameFont.setBold(true);
    stockNameFont.setFamily("SimSun");
    stockNameFont.setPixelSize(16);
    QFontMetrics stockNameFm(stockNameFont);

    QFont stockCodeFont;
    stockCodeFont.setBold(true);
    stockCodeFont.setFamily("Arial");
    stockCodeFont.setPixelSize(18);
    QFontMetrics stockCodeFm(stockCodeFont);

    QFont stockFiledNameFont;
    stockFiledNameFont.setFamily("SimSun");
    stockFiledNameFont.setPixelSize(14);
    QFontMetrics stockFiledNameFm(stockFiledNameFont);

    QFont stockFiledValueFont;
    stockFiledValueFont.setBold(true);
    stockFiledValueFont.setFamily("Arial");
    stockFiledValueFont.setPixelSize(18);
    QFontMetrics stockFiledValueFm(stockFiledValueFont);

    HzQuoteFormat::QuoteItem strPriceItem = HzQuoteFormat::QuoteFormat::formatPrice(m_stock.pre_close_price,m_stock.last_price);
    HzQuoteFormat::QuoteItem strRateItem = HzQuoteFormat::QuoteFormat::formatChangeRate(m_stock.rate,m_stock.last_price,m_stock.pre_close_price);
    HzQuoteFormat::QuoteItem threeDaiesRateItem = HzQuoteFormat::QuoteFormat::formatChangeRate(m_stock.three_rate,m_stock.three_rate,0.0);
    HzQuoteFormat::QuoteItem funFlowerItem = formatFundValue(m_fund);

    int stockNameWidth = stockNameFm.boundingRect(m_stock.stock_name.c_str()).width();
    int stockCodeWidth = stockCodeFm.boundingRect(stockCode).width();
    int priceValueWidth = stockFiledValueFm.boundingRect(strPriceItem.strValue).width();
    int rateValueWidth = stockFiledValueFm.boundingRect(strRateItem.strValue).width();
    int firstLength = stockNameWidth+8+stockCodeWidth+15+priceValueWidth+8+rateValueWidth;

    int FundBuyNameWidth = stockFiledNameFm.boundingRect(QStringLiteral("主动净买")).width();
    int funBuyValueWidth = stockFiledValueFm.boundingRect(funFlowerItem.strValue).width();
    int threeRateNameWidth = stockFiledNameFm.boundingRect(QStringLiteral("三日涨幅")).width();
    int threeRateValueWidth = stockFiledValueFm.boundingRect(threeDaiesRateItem.strValue).width();
    int secondLength = FundBuyNameWidth+8+funBuyValueWidth+27+threeRateNameWidth+8+threeRateValueWidth;

    int drawWidth = firstLength;
    if(firstLength < secondLength)
        drawWidth = secondLength;
    QRect rc = m_rc;
    if(rc.width() > drawWidth)
    {
        rc.setLeft(rc.left()+ (rc.width()-drawWidth)/2);
    }

    QRect drawFirstRect = rc;
    drawFirstRect.setTop(rc.top()+35);
    drawFirstRect.setBottom(drawFirstRect.top()+25);
    //绘制股票名称
    dc.setFont(stockNameFont);
    dc.setPen(QColor(m_pMainWnd->getStockCodeAndNameClr()));
    dc.drawText(drawFirstRect,Qt::AlignLeft|Qt::AlignVCenter,m_stock.stock_name.c_str());
    //绘制股票代码
    dc.setFont(stockCodeFont);
    drawFirstRect.setLeft(drawFirstRect.left()+stockNameWidth+8);
    dc.drawText(drawFirstRect,Qt::AlignLeft|Qt::AlignVCenter,stockCode);
    //绘制最新价
    dc.setFont(stockFiledValueFont);
    changeItemClr(strPriceItem,dc,m_pMainWnd);
    drawFirstRect.setLeft(drawFirstRect.left()+stockCodeWidth+15);
    dc.drawText(drawFirstRect,Qt::AlignLeft|Qt::AlignVCenter,strPriceItem.strValue);
    //绘制涨幅
    dc.setFont(stockFiledValueFont);
    changeItemClr(strRateItem,dc,m_pMainWnd);
    drawFirstRect.setLeft(drawFirstRect.left()+priceValueWidth+8);
    dc.drawText(drawFirstRect,Qt::AlignLeft|Qt::AlignVCenter,strRateItem.strValue);


    QRect drawSecondRect = rc;
    drawSecondRect.setTop(drawFirstRect.bottom());
    drawSecondRect.setBottom(drawSecondRect.top()+30);
    //绘制主动净买名称
    dc.setFont(stockFiledNameFont);
    dc.setPen(QColor(m_pMainWnd->getStockFiledClr()));
    dc.drawText(drawSecondRect,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("主动净买"));
    //绘制主动净买值
    dc.setFont(stockFiledValueFont);
    changeItemClr(funFlowerItem,dc,m_pMainWnd);
    drawSecondRect.setLeft(drawSecondRect.left()+FundBuyNameWidth+8);
    dc.drawText(drawSecondRect,Qt::AlignLeft|Qt::AlignVCenter,funFlowerItem.strValue);
    //绘制三日涨幅名称
    dc.setPen(QColor(m_pMainWnd->getStockFiledClr()));
    drawSecondRect.setLeft(drawSecondRect.left()+funBuyValueWidth+27);
    dc.setFont(stockFiledNameFont);
    dc.drawText(drawSecondRect,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("三日涨幅"));
    //绘制三日涨幅值
    dc.setFont(stockFiledValueFont);
    changeItemClr(threeDaiesRateItem,dc,m_pMainWnd);
    drawSecondRect.setLeft(drawSecondRect.left()+threeRateNameWidth+8);
    dc.drawText(drawSecondRect,Qt::AlignLeft|Qt::AlignVCenter,threeDaiesRateItem.strValue);
}

StrategyRelationStocksInnerWnd::StrategyRelationStocksInnerWnd(QMap<QString,HZData::StockMoney>& fundMap,QWidget *parent):BaseWidget(parent),m_fundMap(fundMap)
{
}

void StrategyRelationStocksInnerWnd::setDynaProvider(DynaDataProvider *pProvider)
{
    m_pProvider =  pProvider;
}

void StrategyRelationStocksInnerWnd::setStockList(const QVector<QString> &stockList)
{
    m_stockList =stockList;
    refresh();
}

void StrategyRelationStocksInnerWnd::refresh()
{
    m_pImageLayer->clear(m_stockAreaType);
    ImgLayerInfo drawLayer =  m_pImageLayer->getLayout(m_stockAreaType);
    QPainter itemPainter(drawLayer.pix);
    int col = 3;
     int row = 2;
     for(int j = 0;j <row;j++)
     {
         for(int i =0;i < 3;i++)
         {
             int index  = j*col+i;
             if(index < m_stockItems.size())
             {
                 HZData::AllStock stock;
                 HZData::StockMoney fund;
                 if(index  < m_stockList.size())
                 {
                     stock  = m_pProvider->getStockData(m_stockList[index]);
                     if(stock.stock_name.empty())
                     {
                         stock.stock_code = m_stockList[index].toStdString();
                         stock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockList[index]).toStdString();
                     }
                     if(m_fundMap.contains(m_stockList[index]))
                         fund = m_fundMap[m_stockList[index]];
                 }
                 m_stockItems[index].update(stock,fund,itemPainter);
             }
         }
     }
     this->update();
}

void StrategyRelationStocksInnerWnd::createWg()
{
    m_pImageLayer = new ImageLayer(this);
    m_layerType = m_pImageLayer->createLayout(getRc(),getBkClr()).type;
    m_stockAreaType = m_pImageLayer->createLayout(getRc()).type;
    for(int i=0;i < 6;i++)
    {
        m_stockItems.push_back(StocksItemDrawer(this));
    }
}

void StrategyRelationStocksInnerWnd::paintEvent(QPaintEvent *e)
{
    m_pImageLayer->repaint();
}

void StrategyRelationStocksInnerWnd::resizeEvent(QResizeEvent *event)
{
    m_pImageLayer->resize(getRc(),m_layerType);
    m_pImageLayer->resize(getRc(),m_stockAreaType);
    redraw();
}

void StrategyRelationStocksInnerWnd::redraw()
{
    m_pImageLayer->clear(m_layerType);
    ImgLayerInfo drawLayer =  m_pImageLayer->getLayout(m_layerType);
    QPainter painter(drawLayer.pix);
    painter.setPen(QColor("#262626"));
    QRect rc = getRc();
    rc.setTop(rc.top());

    //绘制水平线
    QPoint pt1;
    pt1.setX(rc.left());
    pt1.setY(rc.top()+rc.height()/2);
    QPoint pt2;
    pt2.setX(rc.right());
    pt2.setY(rc.top()+rc.height()/2);
    painter.drawLine(pt1,pt2);
    int width = rc.width()/3;

    //绘制垂直线条
    for(int i=1;i < 3;i++)
    {
        pt1.setX(i*width+rc.left());
        pt1.setY(rc.top());

        pt2.setX((pt1.x()));
        pt2.setY(rc.bottom());
        painter.drawLine(pt1,pt2);
    }

    int col = 3;
    int row = 2;
    int hight = rc.height()/row;
    QRect itemRc,drawRc;
    m_pImageLayer->clear(m_stockAreaType);
    drawLayer =  m_pImageLayer->getLayout(m_stockAreaType);
    QPainter itemPainter(drawLayer.pix);
    for(int j = 0;j <row;j++)
    {
        itemRc.setTop(rc.top()+j*hight);
        itemRc.setBottom(itemRc.top()+hight);
        for(int i =0;i < 3;i++)
        {
            itemRc.setLeft(rc.left()+i*width);
            itemRc.setRight(itemRc.left()+width);
            int index  = j*col+i;
            if(index < m_stockItems.size())
            {
                HZData::AllStock stock;
                if(index  < m_stockList.size())
                {
                    stock  = m_pProvider->getStockData(m_stockList[index]);
                }
                drawRc = itemRc;
                drawRc.setTop(itemRc.top()+10);
                drawRc.setBottom(itemRc.bottom()-10);
                drawRc.setLeft(itemRc.left()+10);
                drawRc.setRight(itemRc.right()-10);
                painter.fillRect(drawRc,m_itemBkClr);

                HZData::StockMoney fund;
                if(m_fundMap.contains(stock.stock_code.c_str()))
                    fund = m_fundMap[stock.stock_code.c_str()];
                m_stockItems[index].update(stock,fund,drawRc,itemPainter);
            }
        }
    }
}

int StrategyRelationStocksInnerWnd::getrow()
{
    int num = m_stockItems.size()%3;
    if(num  > 0)
        num = m_stockItems.size()/3+1;
    else
        num  = m_stockItems.size()/3;
    return num;
}

QRect StrategyRelationStocksInnerWnd::getRc()
{
    QRect rc(0,0,this->geometry().width(),this->geometry().height());
    return rc;
}
