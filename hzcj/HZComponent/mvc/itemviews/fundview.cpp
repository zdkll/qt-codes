#include "fundview.h"

#include <QScrollBar>
#include <QKeyEvent>

#include "fundheaderview.h"
#include "sortfilterproxymodel.h"
#include "stockdynaheaderview.h"
#include "styleditemdelegate.h"
#include "delegatepool.h"


///FundFlowView----
FundFlowView::FundFlowView(MoneyFlowType type,QWidget *parent)
    :FundBaseView(FrozenView::ColRowFrozen,type,parent)
{
    FundModel *model = new FundModel("FundFlow",this);

    StockFundProvider *dataProvider = new StockFundProvider(model,this);

    this->initTabView(model,dataProvider,false);
}

///BKStockFundFlowView---
BKStockFundFlowView::BKStockFundFlowView(QWidget *parent)
    :FundBaseView(FrozenView::ColRowFrozen,BKStockMoney,parent)
{
    BkStockFundModel *model = new BkStockFundModel("FundFlow",this);

    //修改序号样式-从0开始
    ColumnInfo colInfo = model->columnInfos()[0];
    colInfo.idx = ColNoZero;
    //设置序号
    model->setColumnInfo(0,colInfo);

    StockFundProvider *dataProvider = new StockFundProvider(model,this);

    this->initTabView(model,dataProvider,false);

    //板块和板块成分股需要固定第一行
    this->tableView()->setFrozenRowCount(1);
}

void BKStockFundFlowView::modelReset()
{
    m_stockCode.clear();
    if(m_dataModel->rowCount()>1){
        emit  itemSelected(filterDetail());
        this->tableView()->selectRow(1);
    }
}

///OptionFundFlowView---
OptionFundFlowView::OptionFundFlowView(OptFundType type,QWidget *parent)
    :FundBaseView(FrozenView::ColFrozen,OptionMoney,parent),m_optFundType(type)
{
    FundModel *model = new FundModel("OptionFundFlow",this);

    this->tableView()->setFrozenRowCount(0);
    m_optFundProvider = createDataProvider(model,type);

    this->initTabView(model,m_optFundProvider,true);

    this->headerView()->setSortIndicator(0,Qt::AscendingOrder);
}

OptionFundProvider  *OptionFundFlowView::createDataProvider(FundBaseModel *model,OptFundType type)
{
    switch (type) {
    case OFT_ZXStock:
        return new ZXStockFundProvider(model,this);
        break;
    case OFT_HistStock:
        return new HistStockFundProvider(model,this);
        break;
    case OFT_ImpIndex:
        return new ImpIndexFundProvider(model,this);
        break;
    default:
        break;
    }
    return 0;
}

void OptionFundFlowView::keyPressEvent(QKeyEvent *e)
{
    FundBaseView::keyPressEvent(e);
    if(e->key() == Qt::Key_Delete && m_optFundType == OFT_ZXStock){
        int row = this->tableView()->currentIndex().row();
        //删除自选股
        if(row>=0){
            QString stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
            if(!stockCode.isEmpty()){
                m_optFundProvider->deleteStock(stockCode);
            }
        }
    }
}

void OptionFundFlowView::modelReset()
{
    m_stockCode.clear();
    this->headerView()->setSortIndicator(0,Qt::AscendingOrder);
    if(m_dataModel->rowCount()>0)
        this->tableView()->selectRow(0);
}



///SLFundNetputView---
SLFundNetputView::SLFundNetputView(QWidget *parent)
    :QTableView(parent)
{
    initTableView();

    m_fundProvider  = new  SLStockFundProvider(m_dataModel,this);
    m_fundProvider->setMoneyFieldMask(HZData::MoneyField_Mask);
    m_fundProvider->setStockCount(20);

    m_rowRange.first = Fixed_Default_Min_Row;
    m_rowRange.second = Fixed_Default_Max_Row;
    setSortOrder(false);

    //设置样式
    initItemDelegate(m_columnInfos);
}

void SLFundNetputView::afterActive()
{
    m_fundProvider->actived(true);
}

void SLFundNetputView::afterDeactive()
{
    m_fundProvider->actived(false);
}

void SLFundNetputView::setSortOrder(bool desc)
{
    m_fundProvider->sortIndicatorChanged(HZData::leader_money_netput,desc);
}

void SLFundNetputView::setFilterFlag(int filterFlag)
{
    m_fundProvider->setFilterFlag(filterFlag);
}

void SLFundNetputView::setStockCount(int count)
{
    m_fundProvider->setStockCount(count);
}

void SLFundNetputView::onFocusOut()
{
    this->clearSelection();
    //清除当前Index
    this->setCurrentIndex(QModelIndex());
    //清除当前选中股票
    m_stockCode.clear();
}

void SLFundNetputView::showEvent(QShowEvent *e)
{
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(styleProperties());

    return QTableView::showEvent(e);
}

void SLFundNetputView::resizeEvent(QResizeEvent *e)
{
    //设置列宽
    QTableView::resizeEvent(e);
    int width = this->width();
    for(int i=0;i<m_columnInfos.size()-1;i++){
        this->setColumnWidth(i,width*m_columnInfos[i].width);
    }

    //设置行范围
    int height = this->viewport()->height();
    int rowCount = height/Default_Row_Height;
    rowCount   += Fixed_Plus_Row;
    rowCount = rowCount< m_rowRange.first?m_rowRange.first
                                        :rowCount>m_rowRange.second?m_rowRange.second:rowCount;
    if(rowCount != m_rowCount){
        m_rowCount = rowCount;
        m_fundProvider->setStockCount(rowCount);
    }
}

void SLFundNetputView::keyPressEvent(QKeyEvent *e)
{
    QTableView::keyPressEvent(e);
    if(e->key() == Qt::Key_Return)
        slotEnteredItem(this->currentIndex());
}

void SLFundNetputView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
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

void SLFundNetputView::slotEnteredItem(const QModelIndex &index)
{
    int row = index.row();
    QVariant var = m_sortModel->colIdxData(row,ColStockCode);
    if(!var.isNull())
        emit itemEntered(var.toString());
}

void SLFundNetputView::initTableView()
{
    this->verticalHeader()->hide();

    this->setShowGrid(false);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setSortingEnabled(false);

    m_dataModel = new SLFundModel("SLFund");

    m_columnInfos = m_dataModel->columnInfos();
    m_sortModel = new SortFilterProxyModel(m_dataModel,this);
    m_sortModel->setDynamicSortFilter(false);

    this->setModel(m_sortModel);

    StockDynaHeaderView *headerView = new StockDynaHeaderView(Qt::Horizontal,this);

    headerView->setColumnInfos(m_columnInfos);

    this->setHorizontalHeader(headerView);

    connect(this,&QTableView::doubleClicked,this,&SLFundNetputView::slotEnteredItem);

    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
}

void SLFundNetputView::initItemDelegate(const QVector<ColumnInfo> &columnInfos)
{
    //设置样式
    for(int i =0;i<columnInfos.size();i++){
        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(columnInfos[i].idx);
        itemDelegate->setAlignment(Qt::AlignVCenter |m_columnInfos[i].alignment);
        this->setItemDelegateForColumn(i,itemDelegate);
    }
}



