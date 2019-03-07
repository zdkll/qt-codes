#include "navilistview.h"

#include <QScrollBar>
#include <QStackedLayout>

#include "naviitemdelegate.h"


NaviListView::NaviListView(QWidget *parent)
    :QListView(parent)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollMode(ScrollPerItem);

    m_dataModel = new NaviStockDynaModel(this);

    connect(this, &NaviListView::clicked,this,&NaviListView::itemClicked);
}

void NaviListView::afterActive()
{
    m_dataProvider->actived(true);
}

void NaviListView::afterDeactive()
{
    m_dataProvider->actived(false);
}

//设置排序
void NaviListView::setSortIndicator(SortField sortField,bool is_desc)
{
    sortIndicatorChanged(sortField,is_desc);
}

//设置filterFlag
void  NaviListView::setFilterFlag(int filterFlag)
{
    m_dataProvider->setFilterFlag(filterFlag);
}

void  NaviListView::sortIndicatorChanged(SortField sortField,bool is_desc)
{
    m_dataProvider->sortIndicatorChanged(sortField,is_desc);
}

void NaviListView::setAutoSelected(bool enable)
{
    m_isAutoSelected  = enable;
}
void NaviListView::switchToUpDownStock(int isUp,int step)
{
    int rowCount = m_sortModel->rowCount();
    if(rowCount<1) return;
    QModelIndex index = this->currentIndex();
    int cur_row = 0;
    if(index.isValid()){  //没有选中默认从第0行切换
        cur_row = index.row();
    }

    int new_row = cur_row;
    if(isUp){
        new_row = cur_row-step;
        if(new_row<0)
            new_row = rowCount - abs(new_row)%rowCount;
    }else{
        new_row = cur_row+step;
        if(new_row>=rowCount)
            new_row = new_row%rowCount;
    }

    QModelIndex new_index = m_sortModel->index(new_row,0);
    this->setCurrentIndex(new_index);
    this->scrollTo(new_index);
    itemClicked(new_index);
}

void NaviListView::initListView(NaviStockDynaModel *model,NaviStockProvider *dataProvider,int sortRole)
{
    m_dataModel = model;
    m_dataProvider = dataProvider;

    m_sortModel = new NaviSortFilterProxyModel(m_dataModel,this);

    m_sortModel->setNaviSortRole(sortRole);
    m_sortModel->sort(0,Qt::DescendingOrder);

    m_sortModel->setSourceModel(m_dataModel);
    this->setModel(m_sortModel);
    this->setItemDelegate(new NaviItemDelegate(this));

    connect(m_dataModel,&NaviStockDynaModel::modelReset,this,&NaviListView::slotModelReset);

    connect(this->verticalScrollBar(),&QScrollBar::valueChanged,this,&NaviListView::slotCurrentRangeChanged);
    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged ,this,&NaviListView::slotCurrentRangeChanged);

    m_selectionModel = new QItemSelectionModel(m_sortModel,this);
    this->setSelectionModel(m_selectionModel);
}



void NaviListView::itemClicked(const QModelIndex &index)
{
    QString stockCode = index.data(Stock_Code_Role).toString();
    if(stockCode != m_stockCode && !stockCode.isEmpty()){
        m_stockCode = stockCode;
        emit selectedItem(m_stockCode);
    }
}

void  NaviListView::currentRangeChanged(int start,int count)
{
    m_dataProvider->currentRangeChanged(start,count);
}


//初始化选中当前股票
void NaviListView::slotModelReset()
{
    slotCurrentRangeChanged();
    if(!m_stockCode.isEmpty() && !m_isAutoSelected){
        QString stockCode = m_stockCode;
        m_stockCode.clear();
        selectStock(stockCode);
    }

    if(m_isAutoSelected && m_sortModel->rowCount()>0){
        m_stockCode.clear();
        m_selectionModel->select(m_sortModel->index(0,0),QItemSelectionModel::SelectCurrent);
        this->setCurrentIndex(m_sortModel->index(0,0));
    }
}

void  NaviListView::selectStock(const QString &stockCode)
{
    for(int row = 0;row<m_dataModel->rowCount();row++){
        QString code =  m_dataModel->index(row,0).data(Stock_Code_Role).toString();
        if(code == stockCode){
            QModelIndex index = m_sortModel->mapFromSource(m_dataModel->index(row,0));
            this->setCurrentIndex(index);
            this->scrollTo(index);
            break;
        }
    }
}

void  NaviListView::slotCurrentRangeChanged()
{
    int start   = this->verticalScrollBar()->value();
    int count = this->verticalScrollBar()->pageStep()+1;

    currentRangeChanged(start,count);
}

void NaviListView::currentChanged(const QModelIndex &current, const QModelIndex &)
{
    QString stockCode = current.data(Stock_Code_Role).toString();
    qDebug()<<"NaviListView:"<<"m_stockCode:"<<m_stockCode<<"stockCode"<<stockCode;
    if(stockCode != m_stockCode && !stockCode.isEmpty()){
        m_stockCode = stockCode;
        this->scrollTo(current);
        emit selectedItem(m_stockCode);
    }
}

void  NaviListView::showEvent(QShowEvent *e)
{
    if(!NaviItemDelegate::isInitedStyleProperties())
        NaviItemDelegate::initStyleProperties(NaviStyledProperities::styledProperties());

    return QListView::showEvent(e);
}



///NaviZXHistListView----
NaviZXHistListView::NaviZXHistListView(LocalStockType type,QWidget *parent)
    :NaviListView(parent)
{
    OptionNaviStockDynaModel *model = new OptionNaviStockDynaModel(this);
    if(type == ZXStock){
        NaviZXStockProvider *dataProvider = new NaviZXStockProvider(model,this);
        this->initListView(model,dataProvider,Stock_No_Role);
        connect(dataProvider,&NaviZXStockProvider::rowCountChanged,this,&NaviZXHistListView::rowCountChanged);
    }else{
        NaviHistStockProvider *dataProvider = new NaviHistStockProvider(model,this);
        this->initListView(model,dataProvider,Stock_No_Role);
    }

    //序号递增顺序排序
    this->m_sortModel->sort(0,Qt::AscendingOrder);
    this->m_sortModel->setDynamicSortFilter(false);
}



///NaviZXListView
NaviZXListView::NaviZXListView(QWidget *parent)
    :BaseWidget(parent,"NaviZXListView")
{
    QStackedLayout *mainLayout = new QStackedLayout;
    mainLayout->setContentsMargins(0,0,0,0);

    m_addZXStockView = new AddZXStockView(AddZXStockView::OtherZXView,this);
    m_NaviZXListView   = new NaviZXHistListView(ZXStock,this);

    mainLayout->addWidget(m_addZXStockView);
    mainLayout->addWidget(m_NaviZXListView);
    mainLayout->setStackingMode(QStackedLayout::StackAll);
    this->setLayout(mainLayout);

    connect(m_NaviZXListView,&NaviZXHistListView::selectedItem,this,&NaviZXListView::selectedItem);
    connect(m_NaviZXListView,&NaviZXHistListView::rowCountChanged,this,&NaviZXListView::rowCountChanged);
}

void NaviZXListView::afterActive()
{
    m_NaviZXListView->afterActive();
}

void NaviZXListView::afterDeactive()
{
    m_NaviZXListView->afterDeactive();
}

void NaviZXListView::setStockCode(const QString &stock)
{
    m_NaviZXListView->setStockCode(stock);
}

void NaviZXListView::switchToUpDownStock(bool isUp,int step)
{
    m_NaviZXListView->switchToUpDownStock(isUp,step);
}

void NaviZXListView::rowCountChanged(int count)
{
    emit signalRowCountChanged(count);
    qDebug()<<"show count:"<<count;
    if(count<1)
        m_addZXStockView->show();
    else
        m_addZXStockView->hide();
}



///NaviIndexListView----
NaviIndexListView::NaviIndexListView(QWidget *parent)
    :NaviListView(parent)
{
    NaviStockDynaModel *model = new NaviStockDynaModel(this);
    NaviIndexStockProvider *dataProvider = new NaviIndexStockProvider(model,this);

    this->initListView(model,dataProvider);

    m_dataProvider->sortIndicatorChanged(HZData::rise_fall_rate,true);
    //index按照涨幅排序
    this->m_sortModel->sort(0,Qt::DescendingOrder);
    m_sortModel->setNaviSortRole(Stock_No_Role);
    m_sortModel->setDynamicSortFilter(false);
}

void NaviIndexListView::setSortOrder(bool is_desc)
{
    Qt::SortOrder order = is_desc?Qt::DescendingOrder:Qt::AscendingOrder;
    m_sortModel->sort(0,order);
    m_dataProvider->sortIndicatorChanged(HZData::rise_fall_rate,is_desc);
}

void NaviIndexListView::OnStockCodeChange()
{
    qDebug()<<"NaviIndexListView OnStockCodeChange:"<<m_stockCode;
}

void NaviIndexListView::sortIndicatorChanged(SortField sortField,bool is_desc)
{
    //model自动排序
    Qt::SortOrder order = is_desc?Qt::DescendingOrder:Qt::AscendingOrder;

    //    if(((NaviIndexStockProvider*)m_dataProvider)->isOptionalIndex())
    //    {
    //        int role = sortField  == HZData::none_sort?Stock_No_Role:Stock_Rate_Role;
    //        m_sortModel->setNaviSortRole(role);
    //        m_sortModel->sort(0,order);
    //    }else//按照拉取排行结果顺序排序
    //    {
    //       m_sortModel->setNaviSortRole(Stock_No_Role);
    m_sortModel->sort(0,order);
    m_dataProvider->sortIndicatorChanged(sortField,is_desc);
    //    }
}


///NaviBlockListView------
NaviBlockListView::NaviBlockListView(QWidget *parent)
    :NaviListView(parent)
{
    NaviStockDynaModel *model = new NaviStockDynaModel(this);
    NaviBlockStockProvider *dataProvider = new NaviBlockStockProvider(model,this);
    initListView(model,dataProvider);
    m_dataProvider->sortIndicatorChanged(HZData::block_rate,true);

    //index按照涨幅排序
    this->m_sortModel->sort(0,Qt::DescendingOrder);
    m_sortModel->setNaviSortRole(Stock_No_Role);

    m_sortModel->setDynamicSortFilter(false);
}





