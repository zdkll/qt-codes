#include "optionstockview.h"

#include <QScrollBar>
#include <QStackedLayout>
#include <QKeyEvent>
#include "sortfilterproxymodel.h"

/// OptionDynaView---
OptionDynaView::OptionDynaView(ViewType viewtype,QWidget *parent)
    :StockDynaBaseView(ColFrozen,parent),m_viewType(viewtype)
{
    //缩略图都不显示滚动条
    if(viewtype == SLView){
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void OptionDynaView::modelReset()
{
    m_stockCode.clear();
    this->tableView()->sortByColumn(0,Qt::AscendingOrder);
    if(this->m_sortModel->rowCount()>0 && m_viewType == OverallView){
        this->tableView()->selectRow(0);
    }
}


void OptionDynaView::initTableView(AbstractBaseStockModel  *model,BaseStockDynaProvider *dataProvider,bool isZXStock,int sortType)
{
    StockDynaBaseView::initTableView(model,dataProvider,isZXStock,sortType);

    m_stkProvider->setStockFieldMask(HZData::Stock_Mask);
    m_dataModel->setNeedSort(false);
    m_sortModel->setDynamicSortFilter(true);
    this->tableView()->sortByColumn(0,Qt::AscendingOrder);
}


///ZXDynaView---
ZXDynaView::ZXDynaView(ViewType type,QWidget *parent)
    :OptionDynaView(type,parent)
{
    OptionStockItemModel *model =  new OptionStockItemModel("ZXHistStockDyna",this);

    m_zxStkProvider = new ZXStockDynaProvider(model,this);

    this->initTableView(model,m_zxStkProvider,true);

    connect(m_zxStkProvider,&OptionDynaProvider::rowCountChanged,this,&ZXDynaView::rowCountChanged);
}

void ZXDynaView::keyPressEvent(QKeyEvent *e)
{
    OptionDynaView::keyPressEvent(e);
    // qDebug()<<"ZXDynaView keyPress:"<<e->key();
    if(e->key() == Qt::Key_Delete){
        int row = this->tableView()->currentIndex().row();
        //删除自选股
        if(row>=0){
            QString stockCode = m_sortModel->colIdxData(row,ColStockCode).toString();
            if(!stockCode.isEmpty()){
                m_zxStkProvider->deleteStock(stockCode);
            }
        }
    }
}

///ZXStockDynaViewp---
ZXStockDynaView::ZXStockDynaView(ViewType type,QWidget *parent)
    :BaseWidget(parent,"ZXStockDynaView")
{
    //初始显示添加自选股界面
    m_addZXStockView = new AddZXStockView(AddZXStockView::ZXStockView,this);
    m_zxStockDynaView = new ZXDynaView(type,this);

    QStackedLayout  *stackedLayout = new QStackedLayout;
    stackedLayout->setContentsMargins(0,0,0,0);
    stackedLayout->addWidget(m_addZXStockView);
    stackedLayout->addWidget(m_zxStockDynaView);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);

    this->setLayout(stackedLayout);

    connect(m_zxStockDynaView,&ZXDynaView::rowCountChanged,this,&ZXStockDynaView::slotRowCountChanged);

    connect(m_zxStockDynaView,SIGNAL(itemEntered(const QString&)),
            this,SIGNAL(itemEntered(const QString &)));
    connect(m_zxStockDynaView,SIGNAL(itemSelected(const QString&)),
            this,SIGNAL(itemSelected(const QString &)));
    connect(m_addZXStockView,&AddZXStockView::addFinshed,[this]{
        this->afterActive();
    });
}

void ZXStockDynaView::afterActive()
{
    m_zxStockDynaView->afterActive();
}

void ZXStockDynaView::afterDeactive()
{
    m_zxStockDynaView->afterDeactive();
}
void ZXStockDynaView::slotRowCountChanged(int rowCount)
{
    if(rowCount<1)
        m_addZXStockView->show();
    else
        m_addZXStockView->hide();
}


///HistStockDynaView----
HistStockDynaView::HistStockDynaView(ViewType type,QWidget *parent)
    :OptionDynaView(type,parent)
{
    OptionStockItemModel *model = new OptionStockItemModel("ZXHistStockDyna",this);

    HistStockDynaProvider *dataProvider = new HistStockDynaProvider(model,this);

    this->initTableView(model,dataProvider);
}


///ImpIndexDynaView----
ImpIndexDynaView::ImpIndexDynaView(ViewType type ,QWidget *parent)
    :OptionDynaView(type,parent)
{
    OptionIndexItemModel *model = new OptionIndexItemModel("ImpIndexDyna",this);

    ImpIndexDynaProvider *dataProvider = new ImpIndexDynaProvider(model,this);

    this->initTableView(model,dataProvider);

    dataProvider->setStockFieldMask(HZData::Index_Mask);
}


///AblBlockDynaView----
AblBlockDynaView::AblBlockDynaView(QWidget *parent)
    :OptionDynaView(OverallView,parent)
{
    AblBlockItemModel *model = new AblBlockItemModel("AblBlockDyna",this);

    m_dataProvider = new AblBlockDynaProvider(model,this);

    this->initTableView(model,m_dataProvider,false,1);

    this->tableView()->sortByColumn(5,Qt::DescendingOrder);
}

void AblBlockDynaView::modelReset()
{
    m_stockCode.clear();
    this->tableView()->sortByColumn(5,Qt::DescendingOrder);
    if(this->m_sortModel->rowCount()>0)
        this->tableView()->selectRow(0);
}
