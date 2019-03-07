#include "stockdynabaseview.h"

#include <QScrollBar>
#include <QHeaderView>
#include <QKeyEvent>
#include <QGridLayout>
#include <QPainter>
#include <QStyle>

#include "delegatepool.h"
#include "stockdynaheaderview.h"
#include "stockdynamodel.h"
#include "sortfilterproxymodel.h"

StockDynaBaseView::StockDynaBaseView(const FrozenFlag &frozenFlag,QWidget *parent)
    :FrozenView(frozenFlag,parent),m_stkProvider(0)
{
    FrozenBaseTableView *view= this->tableView();
    view->setShowGrid(true);
    view->setFrozenColCount(3);

    view->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
}

StockDynaBaseView::~StockDynaBaseView()
{
    if(m_stkProvider)
        delete m_stkProvider;
}

void StockDynaBaseView::afterActive()
{
    m_stkProvider->actived(true);
    //qDebug()<<"34=====StockDynaBaseView afterActive:"<<stockDynaType();
}

void StockDynaBaseView::afterDeactive()
{
    m_stkProvider->actived(false);
    //qDebug()<<"40=====StockDynaBaseView afterDeactive:"<<stockDynaType();
}

void StockDynaBaseView::setFilterFlag(const int &filterFlag)
{
    m_filterFlag = filterFlag;
    m_stkProvider->setFilterFlag(filterFlag);
}
void StockDynaBaseView::setFilterDetail(const QString &filter_detail)
{
    m_filterDetail = filter_detail;
    m_stkProvider->setFilterDetail(m_filterDetail);
}

void StockDynaBaseView::setFilterInfo(const int &filterFlag,const QString &filter_detail)
{
    m_filterFlag   = filterFlag;
    m_filterDetail = filter_detail;
    m_stkProvider->setFilterInfo(m_filterFlag,m_filterDetail);
}

void StockDynaBaseView::onFocusOut()
{
    this->tableView()->clearSelection();
    this->tableView()->setCurrentIndex(QModelIndex());
    m_stockCode.clear();
}

void StockDynaBaseView::slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    if(m_sortIndicator.column == logicalIndex && m_sortIndicator.order == order)
        return;

    SortType sortType = m_dataModel->sortType(logicalIndex);
    //自动排序返回（不能排序表头点击事件排除）
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

void StockDynaBaseView::slotCurrentRangeChanged()
{
    int start = this->verticalScrollBar()->value();
    int end =  this->tableView()->lastRow();
    end = end == -1?m_dataModel->rowCount():end;

    currentRangeChanged(start,end-start+1);
}

void StockDynaBaseView::slotModelReset()
{
    this->tableView()->scrollToTop();
    slotCurrentRangeChanged();
    modelReset();
}

void StockDynaBaseView::showEvent(QShowEvent *e)
{
    //设置样式代理的属性值
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(this->styleProperties());

    return QWidget::showEvent(e);
}

void StockDynaBaseView::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);
    if(e->key() == Qt::Key_Return)
        slotEnteredItem(this->tableView()->currentIndex());
}

void StockDynaBaseView::initTableView(AbstractBaseStockModel  *model,BaseStockDynaProvider *dataProvider,bool isZXStock,int sortType)
{
    m_dataModel  = model;
    m_stkProvider = dataProvider;
    FrozenBaseTableView *view= this->tableView();

    if(sortType == 0)
        m_sortModel  = new DynaSortFilterProxyModel(m_dataModel,this);
    else
        m_sortModel  = new SortFilterProxyModel(m_dataModel,this);
    m_sortModel->setDynamicSortFilter(false);

    m_columnInfos =  m_dataModel->columnInfos() ;

    //表头
    BaseStockDynaHeaderView *headerView;

    if(isZXStock)
        headerView = new ZXStockDynaHeaderView(Qt::Horizontal,view);
    else
        headerView = new StockDynaHeaderView(Qt::Horizontal,view);

    headerView->setColumnInfos(m_columnInfos);

    view->setModel(m_sortModel);

    view->setHorizontalHeader(headerView);

    connect(view->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&StockDynaBaseView::slotSortIndicatorChanged);

    connect(this->verticalScrollBar(),&QScrollBar::valueChanged , this,&StockDynaBaseView::slotCurrentRangeChanged);

    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged , this,&StockDynaBaseView::slotCurrentRangeChanged);

    connect(m_dataModel,&AbstractBaseStockModel::modelReset,this,&StockDynaBaseView::slotModelReset);

    connect(view,&QTableView::doubleClicked,this,&StockDynaBaseView::slotEnteredItem);
    connect(view,&FrozenBaseTableView::currentItemChanged,this,&StockDynaBaseView::slotCurrentItem);

    initItemDelegate();
}

void StockDynaBaseView::slotCurrentItem(const QModelIndex &index)
{
    selectedItem(index);
}

void StockDynaBaseView::slotEnteredItem(const QModelIndex &index)
{
    enteredItem(index);
}

void StockDynaBaseView::sortIndicatorChanged(const SortField &sortField,const bool &desc )
{
    m_stkProvider->sortIndicatorChanged(sortField,desc);

    m_dataModel->setSortIndicator(sortField,!desc);
}

void StockDynaBaseView::currentRangeChanged(int  start ,int  count )
{
    m_stkProvider->currentRangeChanged(start,count);
}

void StockDynaBaseView::selectedItem(const QModelIndex &index)
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

void StockDynaBaseView::enteredItem(const QModelIndex &index)
{
    int row = index.row();

    QString  stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
    if(!stockCode.isEmpty())
        emit itemEntered(stockCode);
}

void StockDynaBaseView::initItemDelegate()
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
