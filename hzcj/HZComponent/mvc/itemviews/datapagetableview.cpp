#include "datapagetableview.h"

#include <QScrollBar>
#include <QDesktopServices>

#include "baseitemmodel.h"
#include "databasemodel.h"
#include "sortfilterproxymodel.h"
#include "stockdynaheaderview.h"

#include "basedataprovider.h"

#include "styleditemdelegate.h"
#include "delegatepool.h"
#include "styleditemdelegate.h"

#include "itemdelegates.h"

DataPageBaseView::DataPageBaseView(QWidget *parent)
    :FrozenView(ColFrozen,parent),m_dataProvider(0)
    ,m_enterType(EnterStock)
{
    FrozenBaseTableView *view = this->tableView();
    view->setShowGrid(true);
    view->setFrozenColCount(3);

    view->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
}

DataPageBaseView::~DataPageBaseView()
{
    if(m_dataProvider)
        delete m_dataProvider;
}

void DataPageBaseView::afterActive()
{
    if(m_dataProvider)
        m_dataProvider->actived(true);
}

void DataPageBaseView::afterDeactive()
{
    m_stockCode.clear();
    if(m_dataProvider)
        m_dataProvider->actived(false);
}

void DataPageBaseView::setSortIndicatorShown(bool show)
{
    m_headerView->setSortIndicatorShown(show);
}

void DataPageBaseView::slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    if(m_sortIndicator.column == logicalIndex && m_sortIndicator.order == order)
        return;

    SortType sortType = m_sortModel->sortType(logicalIndex);
    //自动排序返回 (不能排序点击事件排除)
    if(sortType != CustomSort)
        return;
    m_sortIndicator.column = logicalIndex;
    m_sortIndicator.order    = order;

    SortField sortField = m_sortModel->sortFieldFromColumn(logicalIndex);
    if(sortField == HZData::none_sort)  return;

    bool is_desc = order == Qt::AscendingOrder;

    sortIndicatorChanged(sortField,is_desc);
}

void DataPageBaseView::slotCurrentRangeChanged()
{
    int start = this->verticalScrollBar()->value();
    int end =  this->tableView()->lastRow();
    end = end == -1?m_dataModel->rowCount()-1:end;

    qDebug()<<"slotCurrentRangeChanged:"<<start<<end;
    currentRangeChanged(start,end-start+1);
}

void DataPageBaseView::currentRangeChanged(int start,int count)
{
    if(m_dataProvider)
        m_dataProvider->currentRangeChanged(start,count);
}

void DataPageBaseView::sortIndicatorChanged(SortField sortField, bool is_desc)
{
    if(m_dataProvider)
        m_dataProvider->setSortIndicator(sortField,is_desc);
}

void DataPageBaseView::slotModelReset()
{
    m_stockCode.clear();
    this->tableView()->scrollToTop();
    slotCurrentRangeChanged();

    this->tableView()->selectRow(0);
    modelReset();
}

void DataPageBaseView::selectedItem(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    int row = index.row();
    QString  stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
    if(m_stockCode != stockCode && !stockCode.isEmpty()){
        m_stockCode = stockCode;
        emit itemSelected(m_stockCode);
    }
}

void DataPageBaseView::enteredItem(const QModelIndex &index)
{
    int row = index.row();
    QString ret;
    if(m_enterType == EnterStock)
        ret = m_sortModel->colIdxData(row,ColStockCode).toString();
    else
        ret = m_sortModel->colIdxData(row,ColYybId).toString();
    if(!ret.isEmpty())
        emit itemEntered(ret);
}

void DataPageBaseView::showEvent(QShowEvent *e)
{
    //设置样式代理的属性值
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(this->styleProperties());

    int showCount = this->tableView()->viewport()->height()/Default_Row_Height+1;
    if(m_showCount != showCount){
        m_showCount = showCount;
        showCountChanged(m_showCount);
    }
    return QWidget::showEvent(e);
}

void DataPageBaseView::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    int showCount = this->tableView()->viewport()->height()/Default_Row_Height+1;
    if(m_showCount != showCount){
        m_showCount = showCount;
        showCountChanged(m_showCount);
    }
}

void DataPageBaseView::initTableView(AbstractDataBaseModel *model,BaseDataProvider *provider,bool sort,bool zxstock)
{
    m_dataModel = model;

    FrozenBaseTableView *view =  this->tableView();
    if(zxstock)
        m_headerView = new ZXStockDynaHeaderView(Qt::Horizontal,this);
    else
        m_headerView = new StockDynaHeaderView(Qt::Horizontal,this);

    view->setHorizontalHeader(m_headerView);

    if(sort)
        m_sortModel = new SortFilterProxyModel(model,this);
    else
        m_sortModel = new DynaSortFilterProxyModel(model,this);

    view->setModel(m_sortModel);
    m_columnInfos = m_dataModel->columnInfos();

    m_headerView->setColumnInfos(m_columnInfos);

    m_dataProvider = provider;

    connect(view->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&DataPageBaseView::slotSortIndicatorChanged);

    connect(this->verticalScrollBar(),&QScrollBar::valueChanged , this,&DataPageBaseView::slotCurrentRangeChanged);

    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged , this,&DataPageBaseView::slotCurrentRangeChanged);

    connect(m_dataModel,&AbstractBaseModel::modelReset,this,&DataPageBaseView::slotModelReset);

    connect(view,&FrozenBaseTableView::doubleClicked,this,&DataPageBaseView::enteredItem);
    connect(view,&FrozenBaseTableView::currentItemChanged,this,&DataPageBaseView::selectedItem);

    this->initItemDelegates();
}


void DataPageBaseView::initItemDelegates()
{
    for(int i=0;i<m_columnInfos.size();i++){
        //设置列宽
        this->tableView()->setColumnWidth(i,m_columnInfos[i].width);
        //设置样式
        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(m_columnInfos[i].idx);
        if(itemDelegate){
            itemDelegate->setAlignment(Qt::AlignVCenter |m_columnInfos[i].alignment);
            this->tableView()->setItemDelegateForColumn(i,itemDelegate);
        }
    }
}


///BiddingTableView---
BiddingTableView::BiddingTableView(BiddingType type,QWidget *parent)
    :DataPageBaseView(parent),m_bidType(type)
{
    QString modelName = getModelName(type);
    qDebug()<<"model name:"<<modelName;

    BiddingItemModel *model = new BiddingItemModel(modelName,this);
    BaseDataProvider *dataProvider = createDataProvider(model,type);

    bool sort = (m_bidType == Bid_ZXStock)?true:false;
    bool zxstock = (m_bidType == Bid_ZXStock)?true:false;
    this->initTableView(model,dataProvider,sort,zxstock);

    //按照固定列排序
    if(m_bidType == Bid_Rate)
        this->tableView()->sortByColumn(3,Qt::DescendingOrder);
    else if(m_bidType == Bid_ZXStock)
        this->tableView()->sortByColumn(0,Qt::AscendingOrder);
    else
        this->tableView()->sortByColumn(4,Qt::DescendingOrder);
}

QString BiddingTableView::getModelName(BiddingType type)
{
    switch(type)
    {
    case Bid_LeaderFund:
        return "BidLeaderFund";
        break;
    case Bid_QuantityRatio:
        return "BidQuantityRatio";
        break;
    case Bid_Rate:
        return "BidRate";
        break;
    case Bid_PreRate:
        return "BidPreRate";
        break;
    case Bid_ZXStock:
        return "BidZXStock";
        break;
    }
    return QString();
}

BaseDataProvider *BiddingTableView::createDataProvider(BiddingItemModel *model,BiddingType type)
{
    if(type == Bid_ZXStock)
        return new BiddingOpotionalProvider(model,this);
    else
        return new BiddingDataProvider(model,this);

    return Q_NULLPTR;
}



///RiseFallMonitorView---
RiseFallMonitorView::RiseFallMonitorView(HZData::RFMonitorType type,QWidget *parent)
    :DataPageBaseView(parent),m_RFMtype(type)
{
    RFMonitorItemModel *model = new RFMonitorItemModel(getModelName(type),this);

    RiseFallMoniProvider *dataProvider =  new RiseFallMoniProvider(model,this);

    dataProvider->setRFMonitorType(type);
    this->initTableView(model,dataProvider);

    //时间 从最新时间排序显示
    this->tableView()->sortByColumn(4,Qt::DescendingOrder);
}

QString RiseFallMonitorView::getModelName(HZData::RFMonitorType type)
{
    switch (type) {
    case HZData::RFM_type_zt: //涨停
        return  "RFMzt";
        break;
    case HZData::RFM_type_dt://跌停
        return  "RFMdt";
        break;
    case HZData::RFM_type_zthb://涨停开板
        return  "RFMztkb";
        break;
    case HZData::RFM_type_dtkb://跌停开板
        return  "RFMdtkb";
        break;
    case HZData::RFM_type_lb:	//连板
        return  "RFMlb";
        break;
    case HZData::RFM_type_zb://炸板
        return  "RFMzb";
        break;
    case HZData::RFM_type_yestoday_zt://昨日涨停
        return  "RFMyzt";
        break;
    default:
        break;
    }
    return QString();
}


FundAbnormalView::FundAbnormalView(HZData::FundAbnormalType type,QWidget *parent)
    :DataPageBaseView(parent),m_fatType(type)
{
    FundAbnormalItemModel *model = new FundAbnormalItemModel("FundAbnormal",this);

    BaseDataProvider *dataProvider;
    bool zxstock = false;
    if(m_fatType == HZData::fat_zxstock){
        zxstock = true;
        dataProvider = new  FamlOptionalProvider(model,this);
    }else{
        FunAbnormalProvider  *famlProvider = new FunAbnormalProvider(model,this);
        famlProvider->setFunAbnormalType(m_fatType);
        dataProvider = famlProvider;
    }

    this->initTableView(model,dataProvider,zxstock,zxstock);

    if(m_fatType == HZData::fat_abnormal || m_fatType == HZData::fat_first_increase_buy)
        this->tableView()->sortByColumn(5,Qt::DescendingOrder);
    else if(m_fatType == HZData::fat_low_rise)
        this->tableView()->sortByColumn(4,Qt::DescendingOrder);
    else
        this->tableView()->sortByColumn(0,Qt::AscendingOrder);

    connect(this,SIGNAL(itemSelected(const QString &)),this,SLOT(slotSeletedStock(const QString &)));
}

void FundAbnormalView::slotSeletedStock(const QString &stockCode)
{
    qDebug()<<"FundAbnormalView slotSeletedStock:"<<stockCode;
}

///LhbStockView
LhbStockView::LhbStockView(StockLhbType  type,QWidget *parent)
    :DataPageBaseView(parent),m_stkLhbType(type)
{
    LhbStockItemModel *model = new LhbStockItemModel("LhbStock",this);
    m_lhbStockProvider = new LhbStockProvider(model,this);
    m_lhbStockProvider->setStockLhbType(LhbtTotalList);

    this->initTableView(model,m_lhbStockProvider,true,false);
    this->tableView()->sortByColumn(5,Qt::DescendingOrder);
}

void LhbStockView::setStockLhbType(StockLhbType  type)
{
    m_stkLhbType = type;
    m_lhbStockProvider->setStockLhbType(type);
}

void LhbStockView::setTime(UINT64 time_t)
{
    m_lhbStockProvider->setTime(time_t);
}

void LhbStockView::currentRangeChanged(int start,int  count)
{
    m_lhbStockProvider->currentRangeChanged(start,count);
}




///LhbYybDrbView-----
LhbYybDrbView::LhbYybDrbView(QWidget *parent)
    :DataPageBaseView(parent)
{
    LhbYybDrbStockItemModel *model = new LhbYybDrbStockItemModel("LhbYybDrb",this);
    m_YybDrbProvider = new LhbYybDrbProvider(model,this);

    this->initTableView(model,m_YybDrbProvider);

    this->tableView()->setFrozenColCount(2);
    this->setEnterType(EnterYyb);
}

void LhbYybDrbView::setTime(UINT64 time_t)
{
    m_YybDrbProvider->setTime(time_t);
}

void LhbYybDrbView::currentRangeChanged(int start,int count)
{
    m_YybDrbProvider->currentRangeChanged(start,count);
}


///LhbYybSlbView-----
LhbYybSlbView::LhbYybSlbView(QWidget *parent)
    :DataPageBaseView(parent)
{
    LhbYybSlbStockItemModel *model = new LhbYybSlbStockItemModel("LhbYybSlb",this);
    m_YybSlbProvider = new LhbYybSlbProvider(model,this);

    this->initTableView(model,m_YybSlbProvider);

    this->tableView()->setFrozenColCount(2);
    this->setEnterType(EnterYyb);
}

void LhbYybSlbView::currentRangeChanged(int  start,int count)
{
    m_YybSlbProvider->currentRangeChanged(start,count);
}


///LhbYybHybView---
//龙虎榜-营业部活跃榜
LhbYybHybView::LhbYybHybView(QWidget *parent)
    :DataPageBaseView(parent)
{
    LhbYybHybStockItemModel *model = new LhbYybHybStockItemModel("LhbYybHyb",this);
    m_YybHybProvider = new LhbYybHybProvider(model,this);

    this->initTableView(model,m_YybHybProvider);

    this->tableView()->setFrozenColCount(2);
    this->setEnterType(EnterYyb);
}

void LhbYybHybView::currentRangeChanged(int  start,int count)
{
    m_YybHybProvider->currentRangeChanged(start,count);
}


///LhbYybSbView----
LhbYybSbView::LhbYybSbView(QWidget *parent)
    :DataPageBaseView(parent)
{
    m_dataModel = new LhbYybSbItemModel("YybSb",this);
    this->initTableView(m_dataModel,0,false);

    this->tableView()->setFrozenColCount(2);
}

void LhbYybSbView::setYybDetailStatistics(const QVector<StockYybDetailStatistics>& statistics)
{
    m_dataModel->initData(statistics);
}


///龙虎榜-营业部详细-历史操作
LhbYybHoView::LhbYybHoView(QWidget *parent)
    :DataPageBaseView(parent)
{
    m_dataModel = new LhbYybHoItemModel("YybHist",this);
    this->initTableView(m_dataModel,0,false);
}

void LhbYybHoView::setYybDetaiHistory(const  QVector<StockYybDetaiHistory>& history)
{
    m_dataModel->initData(history);
}


///StockRzrqDetailView----
StockRzrqDetailView::StockRzrqDetailView(QWidget *parent)
    :DataPageBaseView(parent)
{
    m_model = new StockRzrqDetailItemModel("StockRzrqDetail",this);
    //m_stkRzrqDetailProvider = new StockRzrqDetailProvider(m_model,this);

    this->initTableView(m_model,0);
    this->tableView()->setFrozenColCount(2);
}

void StockRzrqDetailView::setDataList(QVector<StockRzrqDetailItem> &dataList)
{
    m_model->initData(dataList);
}

//void StockRzrqDetailView::setStockCode(const QString &stockCode)
//{
//    m_stockCode = stockCode;
//    m_stkRzrqDetailProvider->setStockCode(stockCode);
//}

//void StockRzrqDetailView::setTime(UINT64 time_t)
//{
//    m_stkRzrqDetailProvider->setTime(time_t);
//}


///BlockRzrqView---
BlockRzrqView::BlockRzrqView(QWidget *parent)
    :DataPageBaseView(parent)
{
    BlockRzrqItemModel *model = new BlockRzrqItemModel("BlockRzrqRank",this);

    m_blockRzrqProvider = new  BlockRzrqProvider(model,this);

    this->initTableView(model,m_blockRzrqProvider);
}

void BlockRzrqView::setTime(UINT64 time_t)
{
    m_blockRzrqProvider->setTime(time_t);
}


///BlockStockRzrqView
BlockStockRzrqView::BlockStockRzrqView(QWidget *parent)
    :DataPageBaseView(parent)
{
    BlockStockRzrqItemModel *model = new BlockStockRzrqItemModel("StockRzrqRank",this);

    m_bkStkRzrqProvider = new BlockStockRzrqProvider(model,this);

    connect(m_bkStkRzrqProvider,&BlockStockRzrqProvider::bkStockCountChanged,this,&BlockStockRzrqView::bkStockCountChanged);
    this->initTableView(model,m_bkStkRzrqProvider);
}

void BlockStockRzrqView::setTime(UINT64 time_t)
{
    m_bkStkRzrqProvider->setTime(time_t);
}

void BlockStockRzrqView::setStockCode(const QString &blockCode)
{
    m_bkStkRzrqProvider->setBlockCode(blockCode);
}



///StockDzjyView----
StockDzjyView::StockDzjyView(QWidget *parent)
    :DataPageBaseView(parent)
{
    StockDzjyItemModel *model = new StockDzjyItemModel("StockDzjy",this);

    m_stkDzjyProvider = new StockDzjyProvider(model,this);

    this->initTableView(model,m_stkDzjyProvider,false);
}


void StockDzjyView::setTime(UINT64 time_t)
{
    m_stkDzjyProvider->setTime(time_t);
}



///StockRzrqView----
StockRzrqView::StockRzrqView(QWidget *parent)
    :DataPageBaseView(parent)
{
    StockRzrqRankItemModel *model = new StockRzrqRankItemModel("StockRzrqRank",this);

    m_stkRzrqProvider = new StockRzrqRankProvider(BasePageProvider::PRTAllPage,model,this);

    m_stkRzrqProvider->setSort(0);

    this->initTableView(model,m_stkRzrqProvider,false);
}

void StockRzrqView::setTime(UINT64 time_t)
{
    m_stkRzrqProvider->setTime(time_t);
}

void StockRzrqView::showCountChanged(int count)
{
    m_stkRzrqProvider->setShowCount(count);
}



///NewStockPreView---
bool XgrlTableView::m_isSettedPixmap = false;
XgrlTableView::XgrlTableView(XgrlType type,QWidget *parent)
    :DataPageBaseView(parent),m_xgrlType(type)
    ,m_fileColumn(-1)
{
    XgrlStockItemModel *xgrlModel = new XgrlStockItemModel(getModelName(m_xgrlType),this);
    m_xgrlProvider = new XgrlStockProvider(m_xgrlType,xgrlModel,this);

    this->initTableView(xgrlModel,m_xgrlProvider);

    for(int i=0;i<m_columnInfos.size();i++){
        if(m_columnInfos[i].idx == ColFile){
            m_fileColumn = i;
            break;
        }
    }

    if(m_xgrlType != Xgrl_Xgssbx)
        this->tableView()->setSelectionMode(QAbstractItemView::NoSelection);
    //如果点击招股书则显示招股书
    connect(this->tableView(),&QTableView::clicked,this,&XgrlTableView::slotItemClicked);
}

void XgrlTableView::setTime(UINT64 timet)
{
    m_xgrlProvider->setTime(timet);
}


void XgrlTableView::slotItemClicked(const QModelIndex &index)
{
    if(m_fileColumn >=0 && index.column() == m_fileColumn)
    {
        QString url = index.data(Stock_Data_Role).toString();
        QDesktopServices::openUrl(QUrl(url));
    }
}

void XgrlTableView::showEvent(QShowEvent *e)
{
    if(!m_isSettedPixmap){
        FileItemDelegate::setPixmap(filePixmap(),fileHoverPixmap());
        YuguDoubleSimpleItemDelegate::setPixmap(ygPixmap());
        m_isSettedPixmap = true;
    }

    DataPageBaseView::showEvent(e);
}

QString XgrlTableView::getModelName(XgrlType type)
{
    switch (type) {
    case Xgrl_Xgsg:
        return "XgrlXgsg";
        break;
    case Xgrl_Xgyg:
        return "XgrlNewStockPre";
        break;
    case Xgrl_Jrsg:
        return "XgrlJrsg";
        break;
    case Xgrl_Jrss:
        return "XgrlJrss";
        break;
    case Xgrl_Yfxdss:
        return "XgrlYfxdss";
        break;
    case Xgrl_Xgssbx:
        return "XgrlXgssbx";
        break;
    default:
        break;
    }
}

