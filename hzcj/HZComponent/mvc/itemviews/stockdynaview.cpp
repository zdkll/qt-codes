#include "stockdynaview.h"
#include "stockdynaprovider.h"
#include "sortfilterproxymodel.h"

#include <QHeaderView>
#include <QScrollBar>

///StockDynaView---
StockDynaView::StockDynaView(const StockDynaType &type, QWidget *parent  )
    :StockDynaBaseView(ColFrozen,parent),m_stkDynaType(type)
{
    m_fieldHash = {{StockDyna,HZData::Stock_Mask},{BlockDyna,HZData::Block_Mask}
                   ,{IndexDyna,HZData::Index_Mask},{BKStockDyna,HZData::Stock_Mask}
                   ,{IndexStockDyna,HZData::Stock_Mask}};

    AbstractBaseStockModel *model = createModel(type);

    BaseStockDynaProvider *dataProvider = createStockDynaProvider(model,type);

    //手动排序
    model->setNeedSort(true);
    //拉取数据类型
    dataProvider->setStockFieldMask(m_fieldHash.value(type));
    this->initTableView(model,dataProvider);

    //默认涨幅排序
    if(type == BlockDyna)
        this->tableView()->sortByColumn(3,Qt::DescendingOrder);
    else
        this->tableView()->sortByColumn(4,Qt::DescendingOrder);
}

void StockDynaView::slotEnteredItem(const QModelIndex &index)
{
    if(m_stkDynaType == BlockDyna){
        int column = index.column();
        if(column  == m_sortModel->columnByIdx(ColRiseStock)){
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

void StockDynaView::modelReset()
{
    m_stockCode.clear();

    if(this->m_dataModel->rowCount()>0)
        this->tableView()->selectRow(0);
}

AbstractBaseStockModel  *StockDynaView::createModel(const StockDynaType &type)
{
    switch (type) {
    case StockDyna:
    case BKStockDyna:
    case IndexStockDyna:
        return new StockItemModel("StockDyna");
        break;
    case BlockDyna:
        return new BlockItemModel("BlockDyna");
        break;
    case IndexDyna:
        return new IndexItemModel("IndexDyna");
        break;
    default:
        break;
    }
    return 0;
}

BaseStockDynaProvider  *StockDynaView::createStockDynaProvider(AbstractBaseStockModel *model,const StockDynaType &type)
{
    switch (type) {
    case StockDyna:
    case IndexDyna:
        return new StockDynaProvider(model,this);
        break;
    case BlockDyna:
        return new BatchDynaProvider(model,this);
        break;
    case BKStockDyna:
        return new BKStockDynaProvider(model,this);
        break;
    case IndexStockDyna:
        return new IndexStockDynaProvider(model,this);
        break;
    default:
        break;
    }
    return 0;
}

BKAndStockView::BKAndStockView(QWidget *parent)
    :StockDynaBaseView(ColRowFrozen,parent)
{
    FrozenBaseTableView *view= this->tableView();

    AbstractBaseStockModel *model = new BKAndStockItemModel("StockDyna");

    ColumnInfo colInfo = model->columnInfos()[0];
    colInfo.idx = ColNoZero;
    //设置序号
    model->setColumnInfo(0,colInfo);

    //拉取数据类型
    BKAndStockDynaProvider *dataProvider = new BKAndStockDynaProvider(model,this);

    this->initTableView(model,dataProvider);
    //手动排序
    model->setNeedSort(true);
    dataProvider->setStockFieldMask(HZData::Stock_Mask);

    view->setFrozenRowCount(1);
    //默认涨幅排序
    this->tableView()->sortByColumn(4,Qt::DescendingOrder);

}

void BKAndStockView::modelReset()
{
    m_stockCode.clear();
    if(this->m_dataModel->rowCount()>1){
       emit  itemSelected(filterDetail());
        this->tableView()->selectRow(1);
    }
}

