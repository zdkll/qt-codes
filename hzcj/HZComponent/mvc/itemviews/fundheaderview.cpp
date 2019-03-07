#include "fundheaderview.h"

#include <QScrollBar>
#include <QMouseEvent>

#include "fundmodel.h"
#include "customheaderdelegate.h"


FundHeaderView::FundHeaderView(QScrollBar *hscrollBar,QTableView *tableView,QWidget *parent)
    :FrozenColTableView(hscrollBar,parent),m_tableView(tableView)
    ,m_headerColCount(0),m_msPressed(false)
    ,m_resizeColumn(-1),m_isInitedProperties(false)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSelectionMode(NoSelection);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMouseTracking(true);
    this->setAutoScroll(false);

    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();

    m_font.setFamily("SimSun");
    m_font.setPixelSize(14);

    m_model = new FundHeaderItemModel(this);

    this->setModel(m_model);

    for(int i=0;i<m_model->columnCount();i++)
        m_sortMap[i] = Qt::AscendingOrder;

    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
    this->setFixedHeight(Default_Row_Height*m_model->rowCount() -1);

    initItemDelegate();
}

void FundHeaderView::setSortIndicator(const int &logicalIndex,const Qt::SortOrder &sortOrder)
{
    if(m_columnInfos[logicalIndex].sortType == DisableSort)
        return;
    m_sortIndex = logicalIndex;
    m_sortMap[m_sortIndex] = sortOrder;
    //qDebug()<<"sort index:"<<logicalIndex<<"sort order:"<<sortOrder;
    m_model->setSortIndicator(m_sortIndex,sortOrder);
    m_tableView->sortByColumn(m_sortIndex,sortOrder);
}

void FundHeaderView::setColumnWidth(int column, int width)
{
    m_tableView->setColumnWidth(column,width);
    FrozenColTableView::setColumnWidth(column,width);
}

void FundHeaderView::setColumnInfos(const QVector<ColumnInfo> &columnInfos)
{
    m_columnInfos = columnInfos;
    m_model->setColumnInfos(columnInfos);
}

void FundHeaderView::setHeaderColCount(const int &colCount)
{
    m_model->setHeaderColCount(colCount);
    m_headerColCount = colCount;
    for(int i=0;i<colCount;i++)
        this->setSpan(0,i,2,1);

    for(int i=colCount;i<m_model->columnCount();i++)
        if((i - colCount)%4 == 0)
            this->setSpan(0,i,1,4);
}

void FundHeaderView::mousePressEvent(QMouseEvent *e)
{
    m_msPressed = true;
    if(m_resizeColumn>=0)
        return QTableView::mousePressEvent(e);

    int column = columnAt(e->pos().x());
    int row       = rowAt(e->pos().y());
    //合并表头第一行点击无响应
    if(row == 0 && column>=m_headerColCount)
        return QTableView::mousePressEvent(e);

    if(column>=0 )
    {
        if(column != m_sortIndex)
            this->setSortIndicator(column,m_sortMap.value(column));
        else
            this->setSortIndicator(column,m_sortMap.value(column) == Qt::DescendingOrder?Qt::AscendingOrder:Qt::DescendingOrder);
    }
    return QTableView::mousePressEvent(e);
}

void FundHeaderView::mouseMoveEvent(QMouseEvent *e)
{
    if(m_msPressed && m_resizeColumn>=0){
        /*if(m_resizeColumn  <0)
            return  QTableView::mouseMoveEvent(e);
        else*/{
            this->setColumnWidth(m_resizeColumn, e->pos().x()
                                 - this->columnViewportPosition(m_resizeColumn)); ;//修改列宽
            return QTableView::mouseMoveEvent(e);
        }
    }
    QPoint pt = e->pos();
    QModelIndex index = this->indexAt(pt);
    //调整到第二行
    if(index.row() == 0){
        index = index.sibling(1,index.column());
        pt.setY(this->visualRect(index).top());
        index = indexAt(pt);
    }
    if(index.isValid()){
        QRect rect = visualRect(index);
        if(rect.right()-pt.x()<2){
            this->setCursor(Qt::SplitHCursor);
            m_resizeColumn = index.column();
            return QTableView::mouseMoveEvent(e);
        }else{
            QModelIndex left_index = index.sibling(index.row(),index.column()-1);
            if(pt.x() - rect.left()<2 && left_index.isValid()){
                pt.setX(rect.left()-1);
                left_index = this->indexAt(pt);
                this->setCursor(Qt::SplitHCursor);
                m_resizeColumn = left_index.column();
                return QTableView::mouseMoveEvent(e);
            }
        }
    }

    this->unsetCursor();
    m_resizeColumn = -1;
    return QTableView::mouseMoveEvent(e);
}

void FundHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    m_msPressed = false;
    return QTableView::mouseReleaseEvent(e);
}

void FundHeaderView::showEvent(QShowEvent *e)
{
    if(!m_isInitedProperties){
        initItemDelegateProperties();
        m_isInitedProperties = true;
    }

    return FrozenColTableView::showEvent(e);
}

void FundHeaderView::initItemDelegate()
{
    m_headerDelegate = new FundHeaderDelegate(this);

    this->setItemDelegate(m_headerDelegate);
}

void FundHeaderView::initItemDelegateProperties()
{
    m_headerDelegate->setFont(m_font);
    m_headerDelegate->setColor(fontColor());
    m_headerDelegate->setPixmap(upPixmap(),downPixmap());
}



