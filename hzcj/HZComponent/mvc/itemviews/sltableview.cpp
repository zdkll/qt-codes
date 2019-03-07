#include "sltableview.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QKeyEvent>
#include <QGridLayout>

#include "pubmsgdef.h"
#include "sortfilterproxymodel.h"
#include "delegatepool.h"
#include "stockdynaprovider.h"
#include "optiondynaprovider.h"
#include "hzline.h"
#include "httpdata.h"


///SLTableView--------------------------------
SLTableView::SLTableView(QWidget *parent )
    :SLBaseTableview(parent),m_rowCount(0)
{
    SLStockItemModel *model   = new SLStockItemModel("SLHSStock",this);

    //不需要手动排序
    model->setNeedSort(false);
    m_stkProvider =  new SLStockDynaProvider(model,this);
    m_stkProvider->setStockFieldMask(HZData::SLStock_Mask);
    initTableView(model,1);

    this->horizontalHeader()->setSortIndicatorShown(false);
    this->horizontalHeader()->hide();

    this->sortByColumn(2,Qt::DescendingOrder);
}

void SLTableView::resizeEvent(QResizeEvent *e)
{
    SLBaseTableview::resizeEvent(e);
    //1 计算行数
    int height = this->viewport()->height();
    int rowCount = height/Default_Row_Height;
    if(rowCount != m_rowCount){
        m_rowCount = rowCount;
        m_stkProvider->setStockCount(rowCount);
    }
    //调整每行高度
    for(int i=0;i<m_rowCount;i++)
        this->setRowHeight(i,height/rowCount);
}



///SLFixedRowTableView---
SLFixedRowTableView::SLFixedRowTableView(QWidget *parent)
    :SLBaseTableview(parent)
{
    m_rowRange.first = Fixed_Default_Min_Row;
    m_rowRange.second = Fixed_Default_Max_Row;
}

void SLFixedRowTableView::setSortIndicatorShown(bool show)
{
    this->horizontalHeader()->setSortIndicatorShown(show);
}

void SLFixedRowTableView::setSortIndicator(SortField sortField,bool desc)
{
    m_stkProvider->sortIndicatorChanged(sortField,desc);
    m_dataModel->setSortIndicator(sortField,desc);
}

void SLFixedRowTableView::setRowRange(int min,int max)
{
    m_rowRange.first = min;
    m_rowRange.second = max;
}

void SLFixedRowTableView::resizeEvent(QResizeEvent *e)
{
    SLBaseTableview::resizeEvent(e);
    //1 计算行数
    int height = this->viewport()->height();
    int rowCount = height/Default_Row_Height;
    rowCount   += Fixed_Plus_Row;
    rowCount = rowCount< m_rowRange.first?m_rowRange.first
                                        :rowCount>m_rowRange.second?m_rowRange.second:rowCount;
    if(rowCount != m_rowCount){
        m_rowCount = rowCount;
        m_stkProvider->setStockCount(rowCount);
    }
}

HSZHTableView::HSZHTableView(HSZHDynaType type,QWidget *parent)
    :SLFixedRowTableView(parent),m_hszhDynaType(type)
{

}


void HSZHTableView::initSortIndicator(SortField sortField,bool desc)
{
    AbstractBaseStockModel *model   = createModel(m_hszhDynaType);
    model->setNeedSort(false);
    Qt::SortOrder order = desc?Qt::AscendingOrder:Qt::DescendingOrder;
    if(m_hszhDynaType == RiseFallRate || m_hszhDynaType == ThreeMinRate || m_hszhDynaType == FiveDayRate)
    {
        if(order == Qt::AscendingOrder)
            model->setColumnLabel(3,QStringLiteral("跌幅%"));
    }

    //全部自动排序
    initTableView(model,0);

    m_stkProvider =  new SLStockDynaProvider(model);

    //拉取字段类型
    m_stkProvider->setStockFieldMask(HZData::SLStock_Mask);
    this->horizontalHeader()->setSortIndicatorShown(false);

    m_stkProvider->sortIndicatorChanged(sortField,desc);
}

AbstractBaseStockModel *HSZHTableView::createModel(HSZHDynaType type)
{
    switch (type) {
    case RiseFallRate:
        return new SLStockItemModel("SLHSZHRate");
        break;
    case Turnover:
        return new SLStockItemModel("SLHSZHTurnover");
        break;
    case TuvRate:
        return new SLStockItemModel("SLHSZHTuvRate");
        break;
    case ThreeMinRate:
        return new SLStockItemModel("SLHSZHThreeMin");
        break;
    case FiveDayRate:
        return new SLStockItemModel("SLHSZHFiveDay");
        break;
    case FromYearYet:
        return new SLStockItemModel("SLHSZHYearYet");
        break;
    default:
        break;
    }
    return 0;
}


///SLDynaTableView----
SLDynaTableView::SLDynaTableView(SLDynaType type,QWidget *parent)
    :SLBaseTableview(parent),m_slDynaType(type)
{
    AbstractBaseStockModel *model   = createModel(type);
    initTableView(model);

    model->setNeedSort(true);
    //待扩展，目前只有一个板块缩略
    m_stkProvider =  new BatchDynaProvider(model,this);

    m_stkProvider->setStockFieldMask(HZData::Block_Mask);

    this->sortByColumn(1,Qt::DescendingOrder);
}

void SLDynaTableView::slotEnteredItem(const QModelIndex &index)
{
    if(m_slDynaType == SLBlock){
        int column = index.column();
        if(column >= m_sortModel->columnByIdx(ColRiseStock)){
            //跳转到领涨股
            int row = index.row();
            QVariant var = m_sortModel->colIdxData(row,ColRiseStockCode);
            if(!var.isNull())
                emit itemEntered(var.toString());
            return;
        }
    }
    int row = index.row();
    QVariant var = m_sortModel->colIdxData(row,ColStockCode);
    if(!var.isNull())
        emit itemEntered(var.toString());
}

AbstractBaseStockModel *SLDynaTableView::createModel(SLDynaType type)
{
    if(type == SLBlock)
        return new BlockItemModel("SLBlock",this);
    else
        return new SLStockItemModel("StockDyna",this);
    return 0;
}

void SLDynaTableView::modelReset()
{
    slotCurrentRangeChanged();
}

///SLOptionTableView----
SLOptionTableView::SLOptionTableView(OptionDynaType type,QWidget *parent)
    :SLBaseTableview(parent),m_optionType(type)
{
    AbstractBaseStockModel *model = createModel(type);

    this->initTableView(model,1);
    model->setNeedSort(false);
    this->setAutoSelected(true);

    m_sortModel->setDynamicSortFilter(true);
    m_stkProvider = createDynaProvider(model,type);
    m_stkProvider->setStockFieldMask(getMask(type));

    if(m_optionType == ZXStocks)
        this->sortByColumn(0,Qt::AscendingOrder);
}

void SLOptionTableView::keyPressEvent(QKeyEvent *e)
{
    SLBaseTableview::keyPressEvent(e);
    if(e->key() == Qt::Key_Delete && m_optionType == ZXStocks){
        int row = this->currentIndex().row();
        //删除自选股
        if(row>=0){
            QString stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
            if(!stockCode.isEmpty()){
                m_stkProvider->deleteStock(stockCode);
            }
        }
    }
}

AbstractBaseStockModel *SLOptionTableView::createModel(OptionDynaType   type)
{
    switch (type) {
    case ZXStocks:
    case HistStocks:
        return new SLStockItemModel("SLZXStock",this);
        break;
    case RelBlocks:
        return new BlockItemModel("RelationBlockList",this);
        break;
    default:
        break;
    }
    return 0;
}

BaseStockDynaProvider *SLOptionTableView::createDynaProvider(AbstractBaseStockModel *model,OptionDynaType type)
{
    switch (type) {
    case ZXStocks:
        return new ZXStockDynaProvider(model,this);
        break;
    case HistStocks:
        return new HistStockDynaProvider(model,this);
        break;
    case RelBlocks:
        return new RelationBlockListProvider(model,this);
        break;
    default:
        break;
    }
    return 0;
}

HZData::StockFieldMask  SLOptionTableView::getMask(OptionDynaType type)
{
    switch (type) {
    case ZXStocks:
    case HistStocks:
        return HZData::SLStock_Mask;
        break;
    case RelBlocks:
        return  HZData::Block_Mask;
        break;
    default:
        break;
    }
    return HZData::AllStock_Mask;
}

///RelationBlockListView----
RelationBlockListView::RelationBlockListView(QWidget *parent)
    :SLOptionTableView(RelBlocks,parent)
{
    this->m_sortModel->setDynamicSortFilter(false);
    this->setAutoSelected(true);
    connect(m_dataModel,&BlockItemModel::dataChanged,this,&RelationBlockListView::slotsDataChanged);
    connect(this,&RelationBlockListView::itemSelected,this,&RelationBlockListView::slotsItemSeleced);
}

void RelationBlockListView::slotsDataChanged()
{
    postRff();
}

void RelationBlockListView::slotsItemSeleced(const QString &stockCode)
{
    if(!stockCode.isEmpty()){
        m_selectedBkCode = stockCode;
        int filter = Httpdata::instance()->getfilterByBkCode(m_selectedBkCode);
        if(filter != -1)
        {
            emit signalsItemSelected(filter,m_selectedBkCode);
            postRff();
        }
    }
}


void RelationBlockListView::OnStockCodeChange()
{
    ((RelationBlockListProvider*)m_stkProvider)->setCurrStock(m_stockCode);
}

void RelationBlockListView::postRff()
{
    QModelIndex current = currentIndex();
    if(!current.isValid())
        return;
    int row = current.row();
    int rise = m_dataModel->colIdxData(row,ColRiseNum).toInt();
    int fall = m_dataModel->colIdxData(row,ColFallNum).toInt();
    int flat = m_dataModel->colIdxData(row,ColFlatNum).toInt();
    QString stock = m_dataModel->colIdxData(row,ColStockCode).toString();
    if(m_selectedBkCode != stock || rise != m_rise || fall != m_fall || flat != m_flat)
    {
        m_rise = rise;
        m_fall = fall;
        m_flat = flat;
        emit signalsItemRiseFallFlatChanged(stock,rise,fall,flat);
    }
}




