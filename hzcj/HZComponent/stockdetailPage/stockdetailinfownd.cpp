#include "stockdetailinfownd.h"
#include "newswidget.h"
#include "hzinfoadapter.h"
#include "relationblockwnd.h"

StockDetailInfoWnd::StockDetailInfoWnd(QWidget *parent, QString instName)
    :StockBaseDetailInfoWnd(parent,instName),
     m_newswidget(new NewsWidget(this)),
     m_anncInfo(new HZAnncsInfo("")),
     m_newsInfo(new HZNewsInfo("")),
     m_researchReport(new HZResearchReport(""))
{
         m_currTabId = NewsTab;
}

void StockDetailInfoWnd::slotsObjChange(const QString &obj)
{
    m_stockCode = obj;
    m_relationBlockWnd->setStockCode(m_stockCode);
    slotsSwitchWnd(m_currTabId);
}

void StockDetailInfoWnd::slotsSwitchWnd(int id)
{
    StockBaseDetailInfoWnd::slotsSwitchWnd(id);
    QVector<QString> strVec;
    strVec.append(m_stockCode);
    InfoWndTabType type = (InfoWndTabType)id;
    switch (type) {
    case NewsTab:
    {
        m_newsInfo->setNextpage(0);
        m_newsInfo->setCode(strVec);
        m_newswidget->setAdapter(m_newsInfo,QStringLiteral("个股新闻"));
        switchPage(id);
    }
        break;
    case AnncTab:
    {
        m_anncInfo->setNextpage(0);
        m_anncInfo->setCode(strVec);
        m_newswidget->setAdapter(m_anncInfo,QStringLiteral("个股公告"));
        switchPage(id);
    }
        break;
    case ReportTab:
    {
        m_researchReport->setNextpage(0);
        m_researchReport->setCode(strVec);
        m_newswidget->setAdapter(m_researchReport,QStringLiteral("个股研报"));
        switchPage(id);
    }
        break;
    case BlockTab:
        switchPage(id);
        break;
    default:
        break;
    }
}

void StockDetailInfoWnd::OnStockCodeChange()
{
    if(m_newswidget)
    {
        QVector<QString> strVec;
        strVec.append(m_stockCode);
        m_newswidget->setCode(strVec);
    }
    if(m_relationBlockWnd)
    {
        m_relationBlockWnd->setStockCode(m_stockCode);
    }
}

void StockDetailInfoWnd::getStockDetailSubItemList(QVector<StockDetailSubItem> &list)
{
    StockDetailSubItem item;
    item.id = NewsTab;
    item.name = QStringLiteral("新闻");
    list.push_back(item);

    item.id = AnncTab;
    item.name = QStringLiteral("公告");
    list.push_back(item);

    item.id = ReportTab;
    item.name = QStringLiteral("研报");
    list.push_back(item);


    item.id = BlockTab;
    item.name = QStringLiteral("相关板块");
    list.push_back(item);
}

QWidget *StockDetailInfoWnd::createBody()
{
    m_newswidget->setAdapter(m_newsInfo,QStringLiteral("个股新闻"));
    QVector<QString> strVec;
    strVec.append(m_stockCode);
    m_newswidget->setCode(strVec);
    addWidget(m_newswidget,m_newswidget);

    m_relationBlockWnd = new RelationBlockWnd(this);
    m_relationBlockWnd->createWg();
    m_relationBlockWnd->setStockCode(m_stockCode);
    addWidget(m_relationBlockWnd,m_relationBlockWnd);
    connect(m_relationBlockWnd,&RelationBlockWnd::signalDbClickObjChange,
            this,&StockDetailInfoWnd::slotsChildStockListDbClick);
    return getStackWnd();
}

void StockDetailInfoWnd::switchPage(int id)
{
    int nPage = 0;
    switch (id) {
    case NewsTab:
    case AnncTab:
    case ReportTab:
    {
        nPage = 0;
    }
         break;
    case BlockTab:
    {
        nPage = 1;
    }
    break;
    default:
        break;
    }
    if(nPage != currWidgetIndex())
    {
        clearCurrWidget();
        setCurrWidget(nPage);
    }
    else
    {
        afterDeactive();
        afterActive();
    }
}
