#include "handtableview.h"

#include "handdataprovider.h"
#include "baseitemmodel.h"

#include <QHeaderView>
#include <QScrollBar>

#include "styleditemdelegate.h"

bool HandTableView::m_isInitialedStyledProperties = false;
HandTableView::HandTableView(QWidget *parent )
    :QTableView(parent),m_dataProvider(0)
    ,m_rowCount(0)
{
    this->horizontalHeader()->setStretchLastSection(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);

    this->verticalHeader()->hide();

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setShowGrid(false);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setFocusPolicy(Qt::NoFocus);
}

HandTableView::~HandTableView()
{
    if(m_dataProvider){
        delete m_dataProvider;
        m_dataProvider = 0;
    }
}

void HandTableView::slotCurrentRangeChanged()
{
    int start = this->verticalScrollBar()->value();
    int end = this->rowAt(this->viewport()->height());
    end = end == -1?m_dataModel->rowCount():end+1;
    currentRangeChanged(start,end - start +1);
}

void HandTableView::resizeEvent(QResizeEvent *e)
{
    QTableView::resizeEvent(e);
    for(int i=0;i<m_columnInfos.size();i++)
        this->setColumnWidth(i,m_columnInfos[i].width*this->viewport()->width());

    int rowCount = (this->viewport()->height()/Default_Row_Height);
    if(m_rowCount != rowCount){
        m_rowCount = rowCount;
        m_dataProvider->setShowCount(m_rowCount);
    }
}

void HandTableView::showEvent(QShowEvent *e)
{
    //设置主力净流入样式代理的属性值
    if(!HandStyledItemDelegate::isInitedStyleProperties())
        HandStyledItemDelegate::initStyleProperties(HandStyledProperities::styledProperties());

    return QTableView::showEvent(e);
}

void HandTableView::initTableView(AbstractBaseModel *model,HandDataProvider *provider)
{
    m_dataModel    = model;
    m_dataProvider = provider;
    m_columnInfos = m_dataModel->columnInfos();

    this->setModel(m_dataModel);

    connect(this->verticalScrollBar(),&QScrollBar::valueChanged , this,&HandTableView::slotCurrentRangeChanged);

    connect(this->verticalScrollBar(),&QScrollBar::rangeChanged , this,&HandTableView::slotCurrentRangeChanged);

    initItemDelegates();
}

void HandTableView::afterActive()
{
    m_dataProvider->actived(true);
}

void HandTableView::afterDeactive()
{
    m_dataProvider->actived(false);
}
