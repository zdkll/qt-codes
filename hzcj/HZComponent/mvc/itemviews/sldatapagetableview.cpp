#include "sldatapagetableview.h"

#include <QKeyEvent>
#include <QScrollBar>

#include "styleditemdelegate.h"
#include "basedataprovider.h"
#include "stockdynaheaderview.h"
#include "delegatepool.h"
#include "sortfilterproxymodel.h"

SLDataPageTableView::SLDataPageTableView(QWidget *parent)
    :QTableView(parent),m_isAutoSelected(false)
    ,m_dataProvider(0),m_dataModel(0)
{
    this->verticalHeader()->hide();
    this->setShowGrid(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->horizontalHeader()->setStretchLastSection(true);

    this->setSortingEnabled(true);
    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
}

void SLDataPageTableView::afterActive()
{
    m_dataProvider->actived(true);
}

void SLDataPageTableView::afterDeactive()
{
    m_dataProvider->actived(false);
}

void SLDataPageTableView::showEvent(QShowEvent *e)
{
    //设置样式代理的属性值
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(styleProperties());

    return QTableView::showEvent(e);
}

void SLDataPageTableView::resizeEvent(QResizeEvent *e)
{
    QTableView::resizeEvent(e);
    int width = this->width();
    for(int i=0;i<m_columnInfos.size()-1;i++){
        this->setColumnWidth(i,width*m_columnInfos[i].width);
    }
}

void SLDataPageTableView::keyPressEvent(QKeyEvent *e)
{
    QTableView::keyPressEvent(e);
    if(e->key() == Qt::Key_Return)
        slotEnteredItem(this->currentIndex());
}

void SLDataPageTableView::currentChanged(const QModelIndex &current,  const QModelIndex & )
{
    if(!current.isValid()) return;
    int row = current.row();

    QString  stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
    if(m_stockCode != stockCode){
        m_stockCode = stockCode;
        emit itemSelected(m_stockCode);
    }
}

void SLDataPageTableView::initTableView(AbstractDataBaseModel *model,BaseDataProvider *dataProvider,bool sort)
{
    m_dataModel   = model;
    m_dataProvider= dataProvider;
    m_columnInfos = m_dataModel->columnInfos();

    if(sort){
        this->setSortingEnabled(true);
        m_sortModel = new SortFilterProxyModel(m_dataModel,this);
        m_sortModel->setDynamicSortFilter(true);
    }else{
        this->setSortingEnabled(false);
        m_sortModel = new DynaSortFilterProxyModel(m_dataModel,this);
        m_sortModel->setDynamicSortFilter(false);
    }

    StockDynaHeaderView *headerView = new StockDynaHeaderView(Qt::Horizontal,this);

    headerView->setColumnInfos(m_columnInfos);

    this->setHorizontalHeader(headerView);

    m_sortModel->setSourceModel(m_dataModel);
    this->setModel(m_sortModel);

    connect(m_dataModel,&AbstractDataBaseModel::modelReset,this,&SLDataPageTableView::slotModelReset);

    connect(this->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&SLDataPageTableView::slotSortIndicatorChanged);

    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged,this,&SLDataPageTableView::slotCurrentRangeChanged);
    connect(this->verticalScrollBar(),&QScrollBar::valueChanged,this,&SLDataPageTableView::slotCurrentRangeChanged);

    connect(this,&QTableView::doubleClicked,this,&SLDataPageTableView::slotEnteredItem);

    this->horizontalHeader()->setStretchLastSection(true);

    initItemDelegate(m_columnInfos);
}

void SLDataPageTableView::slotCurrentRangeChanged()
{
    int start  = this->verticalScrollBar()->value();
    int end = rowAt(this->viewport()->height());
    end = end == -1?m_dataModel->rowCount()-1:end;

    currentRangeChanged(start,end-start+1);
}

void SLDataPageTableView::slotModelReset()
{
    this->scrollToTop();
    m_stockCode.clear();
    if(m_isAutoSelected)
        this->selectRow(0);
    modelReset();
}

void SLDataPageTableView::slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
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
    if(sortField == HZData::none_sort)
        return;

    bool is_desc = order == Qt::AscendingOrder;

    sortIndicatorChanged(sortField,is_desc);
}

//子类自定义函数
void SLDataPageTableView::currentRangeChanged(int start,int count)
{
    m_dataProvider->currentRangeChanged(start,count);
}

void SLDataPageTableView::sortIndicatorChanged(const SortField &sortField,const bool &desc)
{
    // m_dataProvider->sortIndicatorChanged(sortField,desc);
}

void SLDataPageTableView::slotEnteredItem(const QModelIndex &index)
{
    int row = index.row();
    QVariant var = m_sortModel->colIdxData(row,ColStockCode);
    if(!var.isNull())
        emit itemEntered(var.toString());
}


void SLDataPageTableView::initItemDelegate(const QVector<ColumnInfo> &columnInfos)
{
    //设置样式
    for(int i =0;i<columnInfos.size();i++){
        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(columnInfos[i].idx);
        if (itemDelegate){
            itemDelegate->setAlignment(Qt::AlignVCenter |m_columnInfos[i].alignment);
            this->setItemDelegateForColumn(i,itemDelegate);
        }
    }
}


///RzrqTableView----
StockRzrqRankTableView::StockRzrqRankTableView(RzrqRankType type,QWidget *parent)
    :SLDataPageTableView(parent),m_rzrqRankType(type)
{
  // QString modelName = m_rzrqRankType == Rzrq_NetInput?"RzrqRankNetInput":"RzrqRankNetOutput";
    StockRzrqRankItemModel *model = new StockRzrqRankItemModel("RzrqRankNetInput",this);

    int sort = m_rzrqRankType == Rzrq_NetInput?1:2;

    m_rzrqRankProvider = new StockRzrqRankProvider(BasePageProvider::PRTOnce,model,this);

    m_rzrqRankProvider->setSort(sort);
    this->initTableView(model,m_rzrqRankProvider);
}


void StockRzrqRankTableView::setTime(UINT64 time_t)
{
    m_rzrqRankProvider->setTime(time_t);
}





