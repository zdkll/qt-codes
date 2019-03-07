#include "slbasetableview.h"

#include <QKeyEvent>
#include <QScrollBar>

#include "basestockdynaprovider.h"
#include "sortfilterproxymodel.h"
#include "stockdynaheaderview.h"
#include "styleditemdelegate.h"
#include "delegatepool.h"

SLBaseTableview::SLBaseTableview(QWidget *parent )
    :QTableView(parent),m_isAutoSelected(false)
    ,m_stkProvider(0),m_dataModel(0)
{
    this->verticalHeader()->hide();

    this->setShowGrid(false);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->horizontalHeader()->setStretchLastSection(true);

    this->setSortingEnabled(true);
}

SLBaseTableview::~SLBaseTableview()
{
    if(m_stkProvider)
        delete m_stkProvider;
    if(m_dataModel)
        delete m_dataModel;
}

void SLBaseTableview::afterActive()
{
    m_stkProvider->actived(true);
}

void SLBaseTableview::afterDeactive()
{
    m_stkProvider->actived(false);
}

void  SLBaseTableview::setFilterFlag(const int &filterFlag)
{
    m_stkProvider->setFilterFlag(filterFlag);
}

void SLBaseTableview::onFocusOut()
{
    this->clearSelection();

    //清除当前Index
    this->setCurrentIndex(QModelIndex());

    //清除当前选中股票
    m_stockCode.clear();
}

void SLBaseTableview::showEvent(QShowEvent *e)
{
    //设置样式代理的属性值
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(styleProperties());

    return QTableView::showEvent(e);
}

void SLBaseTableview::resizeEvent(QResizeEvent *e)
{
    QTableView::resizeEvent(e);
    int width = this->width();
    for(int i=0;i<m_columnInfos.size()-1;i++){
        this->setColumnWidth(i,width*m_columnInfos[i].width);
    }
}
void SLBaseTableview::keyPressEvent(QKeyEvent *e)
{
    QTableView::keyPressEvent(e);
    if(e->key() == Qt::Key_Return)
        slotEnteredItem(this->currentIndex());
}

void SLBaseTableview::currentChanged(const QModelIndex &current,  const QModelIndex &previous)
{
    if(!current.isValid())
        return;
    int row = current.row();

    QString  stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
    if(m_stockCode != stockCode){
        m_stockCode = stockCode;
        emit itemSelected(m_stockCode);
    }
}

void SLBaseTableview::slotEnteredItem(const QModelIndex &index)
{
    int row = index.row();
    QVariant var = m_sortModel->colIdxData(row,ColStockCode);
    if(!var.isNull())
        emit itemEntered(var.toString());
}

void SLBaseTableview::slotCurrentRangeChanged()
{
    int start  = this->verticalScrollBar()->value();
    int end = rowAt(this->viewport()->height());
    end = end == -1?m_dataModel->rowCount()-1:end;

    currentRangeChanged(start,end-start+1);
}

void SLBaseTableview::slotModelReset()
{
    this->scrollToTop();
    m_stockCode.clear();
    if(m_isAutoSelected)
        this->selectRow(0);
    modelReset();
}

void SLBaseTableview::slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
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

void SLBaseTableview::currentRangeChanged(int start,int count)
{
    m_stkProvider->currentRangeChanged(start,count);
}

void SLBaseTableview::sortIndicatorChanged(const SortField &sortField,const bool &desc)
{
    m_stkProvider->sortIndicatorChanged(sortField,desc);

    //更新model的排序，本地手动排序时需要
    m_dataModel->setSortIndicator(sortField,!desc);
}

void SLBaseTableview::initTableView(AbstractBaseStockModel *model,int sortType)
{
    m_dataModel = model;
    m_columnInfos = m_dataModel->columnInfos();
    this->setSortingEnabled(true);

    if(sortType == 0){
        m_sortModel = new DynaSortFilterProxyModel(m_dataModel,this);
        m_sortModel->setDynamicSortFilter(false);
    }
    else{
        m_sortModel = new SortFilterProxyModel(m_dataModel,this);
        m_sortModel->setDynamicSortFilter(true);
    }

    this->setModel(m_sortModel);

    StockDynaHeaderView *headerView = new StockDynaHeaderView(Qt::Horizontal,this);

    headerView->setColumnInfos(m_columnInfos);

    this->setHorizontalHeader(headerView);

    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);

    connect(m_dataModel,&AbstractBaseModel::modelReset,this,&SLBaseTableview::slotModelReset);

    connect(this->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&SLBaseTableview::slotSortIndicatorChanged);

    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged,this,&SLBaseTableview::slotCurrentRangeChanged);
    connect(this->verticalScrollBar(),&QScrollBar::valueChanged,this,&SLBaseTableview::slotCurrentRangeChanged);

    connect(this,&QTableView::doubleClicked,this,&SLBaseTableview::slotEnteredItem);

    this->horizontalHeader()->setStretchLastSection(true);

    initItemDelegate(m_columnInfos);
}

void SLBaseTableview::initItemDelegate(const QVector<ColumnInfo> &columnInfos)
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


