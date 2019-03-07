#include "blockdetailinfownd.h"
#include "httpdata.h"
#include "stockdynaview.h"
#include "newswidget.h"
#include "hzinfoadapter.h"
BlockDetailInfoWnd::BlockDetailInfoWnd(QWidget *parent, QString instName)
    :StockBaseDetailInfoWnd(parent,instName),
      m_filter(-1),
      m_newswidget(new NewsWidget(this)),
      m_newsInfo(new HZNewsInfo("")),
      mPlateNewsInfo(new HZPlateNewsInfo(""))
{
    m_currTabId=BlockTab;
}

void BlockDetailInfoWnd::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
    if(m_currTabId == NewsTab)
    {
        QVector<QString> strVec;
        strVec.append(m_stockCode);
        mPlateNewsInfo->setNextpage(0);
        mPlateNewsInfo->setCode(strVec);
        m_newswidget->setAdapter(mPlateNewsInfo,QStringLiteral("板块新闻"));
    }else if(m_filter != -1)
    {
        m_pbView->setFilterInfo(m_filter,m_stockCode);
    }
}

void BlockDetailInfoWnd::slotsSwitchWnd(int id)
{
    StockBaseDetailInfoWnd::slotsSwitchWnd(id);
    int nPage = 0;
    QVector<QString> strVec;
    strVec.append(m_stockCode);
    InfoWndTabType type = (InfoWndTabType)id;
    switch (type) {
    case BlockTab:
        nPage = m_indexMap[BlockTab];
        break;
    case NewsTab:
    {
        nPage = m_indexMap[NewsTab];
        mPlateNewsInfo->setNextpage(0);
        mPlateNewsInfo->setCode(strVec);
        m_newswidget->setAdapter(mPlateNewsInfo,QStringLiteral("板块新闻"));
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

void BlockDetailInfoWnd::OnStockCodeChange()
{
    if(m_newswidget)
    {
        QVector<QString> strVec;
        strVec.append(m_stockCode);
        m_newswidget->setCode(strVec);
    }
    m_filter = Httpdata::instance()->getfilterByBkCode(m_stockCode);
    if(m_filter != -1)
        m_pbView->setFilterInfo(m_filter,m_stockCode);
}

void BlockDetailInfoWnd::getStockDetailSubItemList(QVector<StockDetailSubItem> &list)
{
    StockDetailSubItem item;
    item.id = BlockTab;
    item.name = QStringLiteral("成分股");
    list.push_back(item);

    item.id = NewsTab;
    item.name = QStringLiteral("新闻");
    list.push_back(item);
}

QWidget *BlockDetailInfoWnd::createBody()
{
    m_pbView = new  StockDynaView(StockDynaView::BKStockDyna,this);
    addWidget(m_pbView,m_pbView);
    m_indexMap[BlockTab] = 0;
    connect(m_pbView,&StockDynaView::itemEntered,
            this,&BlockDetailInfoWnd::slotsChildStockListDbClick);

    m_newswidget->setAdapter(mPlateNewsInfo,QStringLiteral("板块新闻"));
    QVector<QString> strVec;
    strVec.append(m_stockCode);
    m_newswidget->setCode(strVec);
    addWidget(m_newswidget,m_newswidget);
    m_indexMap[NewsTab] = 1;
    return getStackWnd();
}

///
/// \brief IndexDetailInfoWnd::IndexDetailInfoWnd
/// \param parent
/// \param instName
///
IndexDetailInfoWnd::IndexDetailInfoWnd(QWidget *parent, QString instName)
    :StockBaseDetailInfoWnd(parent,instName),
      m_filter(HZData::IndexStocks),
      m_newswidget(new NewsWidget(this)),
      m_newsInfo(new HZNewsInfo(""))
    ,mIndexNews(new HZIndexNewsInfo(""))
{
    m_currTabId=BlockTab;
}

void IndexDetailInfoWnd::getStockDetailSubItemList(QVector<StockDetailSubItem> &list)
{
    StockDetailSubItem item;
    item.id = BlockTab;
    item.name = QStringLiteral("成分股");
    list.push_back(item);

    item.id = NewsTab;
    item.name = QStringLiteral("新闻");
    list.push_back(item);
}

void IndexDetailInfoWnd::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
    if(m_currTabId == NewsTab)
    {
        QVector<QString> strVec;
        strVec.append(m_stockCode);
        mIndexNews->setNextpage(0);
        mIndexNews->setCode(strVec);
        m_newswidget->setAdapter(mIndexNews,QStringLiteral("指数新闻"));
    }else
    {
        m_pbView->setFilterInfo(m_filter,m_stockCode);
    }
}

void IndexDetailInfoWnd::slotsSwitchWnd(int id)
{
    StockBaseDetailInfoWnd::slotsSwitchWnd(id);
    int nPage = 0;
    QVector<QString> strVec;
    strVec.append(m_stockCode);
    InfoWndTabType type = (InfoWndTabType)id;
    switch (type) {
    case BlockTab:
        nPage = m_indexMap[BlockTab];
        break;
    case NewsTab:
    {
        nPage = m_indexMap[NewsTab];
        mIndexNews->setNextpage(0);
        mIndexNews->setCode(strVec);
        m_newswidget->setAdapter(mIndexNews,QStringLiteral("指数新闻"));
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

void IndexDetailInfoWnd::OnStockCodeChange()
{
    if(m_newswidget)
    {
        QVector<QString> strVec;
        strVec.append(m_stockCode);
        m_newswidget->setCode(strVec);
    }
    //m_filter = Httpdata::instance()->getfilterByBkCode(m_stockCode);
    m_pbView->setFilterInfo(m_filter,m_stockCode);
}

QWidget *IndexDetailInfoWnd::createBody()
{
    m_pbView = new  StockDynaView(StockDynaView::IndexStockDyna,this);
    addWidget(m_pbView,m_pbView);
    m_indexMap[BlockTab] = 0;
    connect(m_pbView,&StockDynaView::itemEntered,
            this,&BlockDetailInfoWnd::slotsChildStockListDbClick);

    m_newswidget->setAdapter(mIndexNews,QStringLiteral("指数新闻"));
    QVector<QString> strVec;
    strVec.append(m_stockCode);
    m_newswidget->setCode(strVec);
    addWidget(m_newswidget,m_newswidget);
    m_indexMap[NewsTab] = 1;
    return getStackWnd();
}
