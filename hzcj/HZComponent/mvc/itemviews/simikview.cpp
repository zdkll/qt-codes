#include "simikview.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QHeaderView>

#include "sortfilterproxymodel.h"
#include "simikheaderview.h"

#include "delegatepool.h"

SimiKTableView::SimiKTableView(QWidget *parent)
    :QTableView(parent),m_headerView(0)
{
    this->horizontalHeader()->setStretchLastSection(true);
    m_model       = new  SimiKModel("SimilarKLine",this);
    m_sortModel = new SortFilterProxyModel(m_model,this);
    this->setModel(m_sortModel);

    m_columnInfos = m_model->columnInfos();
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    this->setSortingEnabled(true);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedIndex(QModelIndex)));

    initItemDelegate();
}

void SimiKTableView::afterActive()
{
    //测试数据
    QVector<SimiKData> simikDatas(8);
    for(int i=0;i<8;i++){
        simikDatas[i].selectedStk.stockCode  = "sh600001";
        simikDatas[i].selectedStk.stockName = QStringLiteral("入选牛股%1").arg(i);
        simikDatas[i].selectedStk.fiveRate  = i/10.f;

        simikDatas[i].similarStk.stockCode  =  "sh600001";
        simikDatas[i].similarStk.stockName = QStringLiteral("相似牛股%1").arg(i);
        simikDatas[i].similarStk.price = 0.1;
        simikDatas[i].similarStk.rate = 2.3;
    }

    m_model->setData(simikDatas);
}

void SimiKTableView::afterDeactive()
{

}

void SimiKTableView::hideAllRowExcept(int row)
{
    for(int i=0;i<this->sortModel()->rowCount();i++)
        if(i != row) this->hideRow(i);
}

void SimiKTableView::showAllRow()
{
    for(int i=0;i<this->sortModel()->rowCount();i++)
        this->showRow(i);
}

void SimiKTableView::initItemDelegate()
{
    //设置列宽
    for(int i=0;i<m_columnInfos.size();i++){

        StockStyledItemDelegate *itemDelegate = DelegatePool::instance()->itemDelegate(m_columnInfos[i].idx);
        if(itemDelegate){
            itemDelegate->setAlignment(Qt::AlignVCenter |m_columnInfos[i].alignment);
            this->setItemDelegateForColumn(i,itemDelegate);
        }
    }
}

void SimiKTableView::resizeEvent(QResizeEvent *e)
{
    int width = this->viewport()->width();
    for(int i=0;i<m_columnInfos.size();i++){
        this->setColumnWidth(i,width*m_columnInfos[i].width);
        if(m_headerView)
            m_headerView->setColumnWidth(i,width*m_columnInfos[i].width);
    }

    return QTableView::resizeEvent(e);
}

void SimiKTableView::showEvent(QShowEvent *e)
{
    QTableView::showEvent(e);

    //设置样式代理的属性值
    if(!StockStyledItemDelegate::isInitedStyleProperties())
        StockStyledItemDelegate::initStyleProperties(styleProperties());
}

void SimiKTableView::clickedIndex(QModelIndex index)
{
    //最后一列
    if(index.column() == this->sortModel()->columnCount()-1){
        //伸展或者收缩
        bool isExpand = index.data(Stock_Data_Role).toBool();

        bool expanding = !isExpand;
        QModelIndex sourceIndex =  m_sortModel->mapToSource(index);
        m_model->toggledItemExpaned(sourceIndex.row(),expanding);
        //列表设置
        if(expanding){
            this->hideAllRowExcept(index.row());
            int height  =  rowHeight(index.row());
            this->setMaximumHeight(height);
            this->scrollTo(index);
        }else{
            this->showAllRow();
            this->setMaximumHeight(16777215);
        }
        //发送K线图伸展收缩信号
        expandSimiKline(expanding);
    }
}


#include <QPainter>
void SmKWidget::paintEvent(QPaintEvent *e)
{
    BaseWidget::paintEvent(e);
    QPainter pt(this);
    pt.fillRect(this->rect(),Qt::blue);
}

///SimiKView------------
SimiKView::SimiKView(QWidget *parent)
    :BaseWidget(parent)
{
    //界面布局
    initWg();

    //初始化列表
    initTableView();
}

void SimiKView::afterActive()
{
    m_tableView->afterActive();
    m_kWidget->afterActive();
}

void SimiKView::afterDeactive()
{
    m_tableView->afterDeactive();
    m_kWidget->afterDeactive();
}

void SimiKView::slotExpandKline(bool expand)
{
    if(expand){
        m_kWidget->show();
    }else{
        m_kWidget->hide();
    }
}

void SimiKView::initWg()
{
    m_kWidget = new SmKWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QGridLayout *top = new QGridLayout();
    top->setContentsMargins(0,0,0,0);
    top->setSpacing(0);

    m_tableView = new SimiKTableView(this);

    m_headerView    = new SimiKHeaderView(m_tableView,this);
    m_tableView->setHeaderView(m_headerView);
    m_vertScrollbar = m_tableView->verticalScrollBar();

    top->addWidget(m_headerView,0,0);
    top->addWidget(m_tableView,1,0);
    top->addWidget(m_vertScrollbar,0,1,2,1);

    mainLayout->addLayout(top);
    mainLayout->addWidget(m_kWidget);
    m_kWidget->hide();

    this->setLayout(mainLayout);
    connect(m_tableView,SIGNAL(expandSimiKline(bool)),this,SLOT(slotExpandKline(bool)));
    //    connect(m_vertScrollbar,SIGNAL(rangeChanged(int , int)),this,SLOT(slotVertBarRangeChanged(int,int)));
}

void SimiKView::slotVertBarRangeChanged(int ,int max)
{
    //    if(max == 0)
    //        m_vertScrollbar->hide();
    //    else if(!m_kWidget->isVisible())
    //        m_vertScrollbar->show();
}

void SimiKView::initTableView()
{
    this->m_headerView->setColumnInfos(m_tableView->model()->columnInfos());
}





