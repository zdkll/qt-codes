#include "simikheaderview.h"
#include <QScrollBar>
#include <QMouseEvent>
#include <QHeaderView>

#include "simikmodel.h"
#include "customheaderdelegate.h"

SimiKHeaderView::SimiKHeaderView(QTableView *tableView,QWidget *parent)
    :QTableView(parent),m_tableView(tableView)
    ,m_msPressed(false),m_resizeColumn(-1),cstk_columnCount(4)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSelectionMode(NoSelection);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMouseTracking(true);
    this->setAutoScroll(false);

    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();

    m_model = new SimiKHeaderItemModel(this);
    m_model->setStkColumnCount(cstk_columnCount);
    this->setModel(m_model);

    m_font.setFamily("SimSun");
    m_font.setPixelSize(13);

    //初始化排序
    for(int i=0;i<m_model->columnCount();i++)
        m_sortMap[i] = Qt::AscendingOrder;

    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
    this->setFixedHeight(Default_Row_Height*m_model->rowCount() -1);

    this->setStockColumnCount(cstk_columnCount);

    initItemDelegate();
}

void SimiKHeaderView::setColumnWidth(int column, int width)
{
    m_tableView->setColumnWidth(column,width);
    QTableView::setColumnWidth(column,width);
}

void SimiKHeaderView::setStockColumnCount(int count)
{
    m_model->setStkColumnCount(count);

    //首行
    this->setSpan(0,0,1,count);
    //中间
    this->setSpan(0,count,2,1);

    this->setSpan(0,count+1,1,count+1);
}

void SimiKHeaderView::setColumnInfos(const QVector<ColumnInfo> &columnInfos)
{
    m_columnInfos = columnInfos;
    m_model->setColumnInfos(columnInfos);
}



void SimiKHeaderView::setSortIndicator(const int &logicalIndex,const Qt::SortOrder &sortOrder)
{
    if(m_columnInfos[logicalIndex].sortType == DisableSort)
        return;
    m_sortIndex = logicalIndex;
    m_sortMap[m_sortIndex] = sortOrder;
    //qDebug()<<"sort index:"<<logicalIndex<<"sort order:"<<sortOrder;
    m_model->setSortIndicator(m_sortIndex,sortOrder);
    m_tableView->sortByColumn(m_sortIndex,sortOrder);
}

void SimiKHeaderView::mousePressEvent(QMouseEvent *e)
{
    m_msPressed = true;
    if(m_resizeColumn>=0)
        return QTableView::mousePressEvent(e);

    int column = columnAt(e->pos().x());
    int row       = rowAt(e->pos().y());
    //合并表头第一行点击无响应
    if(row == 0 && column != cstk_columnCount)
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

void SimiKHeaderView::mouseMoveEvent(QMouseEvent *e)
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

void SimiKHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    m_msPressed = false;
    return QTableView::mouseReleaseEvent(e);
}

void SimiKHeaderView::showEvent(QShowEvent *e)
{
    if(!m_isInitedProperties){
        initItemDelegateProperties();
        m_isInitedProperties = true;
    }

    return QTableView::showEvent(e);
}

void SimiKHeaderView::initItemDelegate()
{
    m_headerDelegate = new SimiKHeaderDelegate(this);

    this->setItemDelegate(m_headerDelegate);
}

void SimiKHeaderView::initItemDelegateProperties()
{
    m_headerDelegate->setFont(m_font);
    m_headerDelegate->setColor(fontColor());
    m_headerDelegate->setPixmap(this->upPixmap(),this->downPixmap());
}

