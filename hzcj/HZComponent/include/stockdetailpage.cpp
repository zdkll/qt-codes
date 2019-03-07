#include "stockdetailpage.h"
#include "quotepagelayout.h"
#include "stockdetailinfownd.h"
#include "stockdetailklineminwnd.h"
#include "stockdetaillistwnd.h"
#include "quotepanelwnd.h"
#include "pubmsgdef.h"
#include "localstocksource.h"
StockDetailPage::StockDetailPage(QWidget *parent, QString instName)
    :StockBaseDetailPage(parent,instName)
{
    if(m_stockCode.isEmpty())
        m_stockCode = "sh600004";
    createWg();
}

StockBaseDetailListWnd *StockDetailPage::createLeftListWnd()
{
    return new StockDetailListWnd(this);
}

StockBaseDetailInfoWnd *StockDetailPage::createBottomWnd()
{
    return new StockDetailInfoWnd(this);
}
