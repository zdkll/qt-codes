#include "fundbaseview.h"

#define Default_FundHeader_Frozen_Column 3
#define Default_FundHeader_Column              10

#include <QScrollBar>
#include <QGridLayout>
#include <QKeyEvent>

#include "stockdynaheaderview.h"
#include "fundheaderview.h"
#include "delegatepool.h"
#include "fundprovider.h"

FundBaseView::FundBaseView(FrozenView::FrozenFlag frozenFlag,MoneyFlowType type,QWidget *parent)
    : BaseWidget(parent,"fundbaseview"),m_fundFlowType(type)
{
    initWg(frozenFlag);
}

void FundBaseView::afterActive()
{
    m_moneyProvider->actived(true);
}

void FundBaseView::afterDeactive()
{
    m_moneyProvider->actived(false);
}

QScrollBar *FundBaseView::horizontalScrollBar()
{
    return m_horizScrollbar;
}

QScrollBar *FundBaseView::verticalScrollBar()
{
    return   m_vertScrollbar;
}

void FundBaseView::setFilterFlag(const int &filterFlag)
{
    m_filterFlag = filterFlag;
    m_moneyProvider->setFilterFlag(m_filterFlag);
}

void FundBaseView::setFilterDetail(const QString &filter_detail)
{
    m_filterDetail = filter_detail;
    m_moneyProvider->setFilterDetail(filter_detail);
}

void FundBaseView::setFilterInfo(const int &filterFlag,const QString &filter_detail)
{
    m_filterFlag = filterFlag;
    m_filterDetail = filter_detail;
    m_moneyProvider->setFilterInfo(filterFlag,filter_detail);
}

void FundBaseView::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    //设置样式代理的属性值
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(styleProperties());
}

void FundBaseView::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);

    if(e->key() == Qt::Key_Return)
        slotEnteredItem(this->tableView()->currentIndex());
}

void FundBaseView::slotEnteredItem(const QModelIndex &index)
{
    if(!index.isValid()) return;
    int row = index.row();

    QString stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();

    if(!stockCode.isEmpty()){
        emit itemEntered(stockCode);
    }
}

void FundBaseView::slotItemSelected(const QModelIndex &index)
{
    if(!index.isValid()) return;
    int row = index.row();

    QString stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
    if( m_stockCode !=  stockCode && !stockCode.isEmpty()){
        m_stockCode = stockCode;
        emit itemSelected(m_stockCode);
    }
}

void FundBaseView::slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    if(m_sortIndicator.column == logicalIndex && m_sortIndicator.order == order)
        return;

    SortType sortType = m_dataModel->sortType(logicalIndex);
    //不能排序 和自动排序返回
    if(sortType != CustomSort){
        if(sortType == DisableSort)
            this->tableView()->sortByColumn(m_sortIndicator.column,m_sortIndicator.order);
        return;
    }

    m_sortIndicator.column = logicalIndex;
    m_sortIndicator.order    = order;

    SortField sortField = m_sortModel->sortFieldFromColumn(logicalIndex);
    if(sortField == HZData::none_sort)
        return;

    bool is_desc = order == Qt::AscendingOrder;

    sortIndicatorChanged(sortField,is_desc);
}

void FundBaseView::slotCurrentRangeChanged()
{
    int startRow = this->verticalScrollBar()->value();
    int endRow  = this->tableView()->rowAt(this->tableView()->viewport()->height() -1);

    endRow       =  endRow == -1?m_dataModel->rowCount()-1:endRow;

    currentRangeChanged(startRow,endRow - startRow +1);
}

void FundBaseView::slotModelReset()
{
    this->tableView()->scrollToTop();
    slotCurrentRangeChanged();

    modelReset();
}

void FundBaseView::sortIndicatorChanged(const SortField &sortField,const bool &desc )
{
    m_moneyProvider->sortIndicatorChanged(sortField,desc);
}

void FundBaseView::currentRangeChanged(int start ,int  count)
{
    m_moneyProvider->currentRangeChanged(start,count);
}

void FundBaseView::modelReset()
{
    m_stockCode.clear();
    if(m_dataModel->rowCount()>0)
        this->tableView()->selectRow(0);
}

void FundBaseView::initWg(FrozenView::FrozenFlag frozenFlag)
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    m_horizScrollbar = new QScrollBar(Qt::Horizontal,this);
    if(frozenFlag == FrozenView::ColFrozen){
        m_tableView       = new FrozenColTableView(m_horizScrollbar,this);
        m_vertScrollbar  = m_tableView->verticalScrollBar();
    }else{
        m_vertScrollbar   = new QScrollBar(Qt::Vertical,this);
        m_tableView       = new FrozenTableView(m_horizScrollbar,m_vertScrollbar,this);
    }

    m_headerView    = new FundHeaderView(m_horizScrollbar,m_tableView,this);

    m_tableView->setFrozenRowCount(0);
    m_tableView->setObjectName("bodyview");
    m_headerView->setObjectName("headerview");

    m_tableView->horizontalHeader()->hide();

    mainLayout->addWidget(m_headerView,0,0);
    mainLayout->addWidget(m_tableView,1,0);
    mainLayout->addWidget(m_vertScrollbar,0,1,2,1);
    mainLayout->addWidget(m_horizScrollbar,2,0,1,2);

    this->setLayout(mainLayout);
}

void  FundBaseView::initTabView(FundBaseModel *model,BaseFundProvider *dataProvider,bool sort)
{
    m_dataModel = model;
    m_moneyProvider = dataProvider;
    m_columnInfos =  m_dataModel->columnInfos() ;

    FrozenBaseTableView *view= this->tableView();
    view->setShowGrid(true);
    view->setFrozenColCount(Default_FundHeader_Frozen_Column);
    view->verticalHeader()->setDefaultSectionSize(Default_Row_Height);

    m_headerView->setFrozenColCount(Default_FundHeader_Frozen_Column);

    if(sort){
        m_sortModel  = new SortFilterProxyModel(m_dataModel,this);
        m_sortModel->setDynamicSortFilter(true);
    }else{
        m_sortModel  = new DynaSortFilterProxyModel(m_dataModel,this);
        m_sortModel->setDynamicSortFilter(false);
    }

    m_headerView->setColumnInfos(m_columnInfos);
    m_headerView->setHeaderColCount(Default_FundHeader_Column);

    view->setModel(m_sortModel);

    connect(view,&QTableView::doubleClicked,this,&FundBaseView::slotEnteredItem);

    connect(view,&QTableView::clicked,this,&FundBaseView::slotItemSelected);
    connect(view,&FrozenBaseTableView::currentItemChanged,this,&FundBaseView::slotItemSelected);

    connect(view->horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&FundBaseView::slotSortIndicatorChanged);

    connect(this->verticalScrollBar(),&QScrollBar::valueChanged , this,&FundBaseView::slotCurrentRangeChanged);
    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged , this,&FundBaseView::slotCurrentRangeChanged);

    connect(m_dataModel,&QAbstractItemModel::modelReset,this,&FundBaseView::slotModelReset);

    this->headerView()->setSortIndicator(5,Qt::DescendingOrder);

    initItemDelegate();
}


void FundBaseView::initItemDelegate()
{
    //设置列宽
    for(int i=0;i<m_columnInfos.size();i++){
        this->headerView()->setColumnWidth(i,m_columnInfos[i].width);
        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(m_columnInfos[i].idx);
        if(itemDelegate){
            itemDelegate->setAlignment(Qt::AlignVCenter |m_columnInfos[i].alignment);
            this->tableView()->setItemDelegateForColumn(i,itemDelegate);
        }
    }

    if(m_fundFlowType == BKMoney){
        //更改代码列为板块码绘制
        int codeCol = m_dataModel->columnByIdx(ColStockCode);
        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(ColBKCode);
        this->tableView()->setItemDelegateForColumn(codeCol,itemDelegate);
    }else if(m_fundFlowType == BKStockMoney){
        int codeCol = m_dataModel->columnByIdx(ColStockCode);
        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(ColBKStockCode);
        this->tableView()->setItemDelegateForColumn(codeCol,itemDelegate);
    }
}

