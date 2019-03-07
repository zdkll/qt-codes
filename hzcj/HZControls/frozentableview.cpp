#include "frozentableview.h"

#include <QMouseEvent>
#include <QBoxLayout>
#include <QScrollBar>

#include <QDebug>
#include <QGridLayout>



FrozenBaseTableView::FrozenBaseTableView(QWidget *parent )
    :QTableView(parent)
{
    this->verticalHeader()->hide();
    this->setSortingEnabled(true);

    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->horizontalScrollBar()->hide();

    QTableView::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void FrozenBaseTableView::mouseMoveEvent(QMouseEvent *e)
{
    return QWidget::mouseMoveEvent(e);
}

void  FrozenBaseTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit currentItemChanged(current);
}

///FrozenColTableView----
FrozenColTableView::FrozenColTableView(QScrollBar *hscrollBar,QWidget *parent )
    :FrozenBaseTableView(parent),m_horizBar(hscrollBar)
    ,m_columnCount(0),m_frozenColumn(0)
{
    m_horizScrollBarPolicy = Qt::ScrollBarAsNeeded;

    connect(m_horizBar,&QScrollBar::valueChanged,this,&FrozenColTableView::slotHorizBarValueChanged);
}

void FrozenColTableView::setModel(QAbstractItemModel *model)
{
    initModel(model);
    connect(m_model,&QAbstractItemModel::modelReset,this, &FrozenColTableView::slotModelReset);
}

void FrozenColTableView::initModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    m_model = model;

    m_columnCount = m_model->columnCount();
    m_columnWidths.resize(m_columnCount);

    for(int i=0;i<m_columnCount;i++)
        m_columnWidths[i] = this->columnWidth(i);
}

void FrozenColTableView::setFrozenColCount(const int &count)
{
    m_frozenColumn = count;
}

void FrozenColTableView::resizeEvent(QResizeEvent *e)
{
    QTableView::resizeEvent(e);

    updateHorizScrollBar();
}

void FrozenColTableView::keyPressEvent(QKeyEvent *e)
{
    FrozenBaseTableView::keyPressEvent(e);
    switch (e->key()) {
    case Qt::Key_Left:
    {
        int value = m_horizBar->value();
        if(--value>=m_horizBar->minimum())
            m_horizBar->setValue(value);
    }
        break;
    case Qt::Key_Right:
    {
        int value = m_horizBar->value();
        if(++value<=m_horizBar->maximum())
            m_horizBar->setValue(value);
    }
        break;

    case Qt::Key_PageUp:
        this->verticalScrollBar()->setValue(verticalScrollBar()->value()-verticalScrollBar()->pageStep());
        break;
    case Qt::Key_PageDown:
        this->verticalScrollBar()->setValue(verticalScrollBar()->value()+verticalScrollBar()->pageStep());
        break;
    case Qt::Key_Up:
        if(currentIndex().row()<verticalScrollBar()->value())
            this->verticalScrollBar()->setValue(verticalScrollBar()->value()-1);
        break;
    case Qt::Key_Down:
        if(currentIndex().row()>=verticalScrollBar()->value()+verticalScrollBar()->pageStep())
            this->verticalScrollBar()->setValue(verticalScrollBar()->value()+1);
        break;
    default:
        break;
    }
}



void FrozenColTableView::columnResized(int column, int oldWidth, int newWidth)
{
    QTableView::columnResized(column,oldWidth,newWidth);

    if(newWidth!=0)
        m_columnWidths[column] = newWidth;

    updateHorizScrollBar();
}

void FrozenColTableView::slotModelReset()
{
    m_columnCount = m_model->columnCount();
    m_columnWidths.resize(m_columnCount);

    for(int i=0;i<m_columnCount;i++)
        m_columnWidths[i] = this->columnWidth(i);
}

void FrozenColTableView::slotHorizBarValueChanged(const int &value)
{
    int col = m_frozenColumn + m_hiddenColumns.size()-1;
    while(col >= value && isColumnHidden(col)){
        m_hiddenColumns.pop();
        this->setColumnHidden(col,false);
        col = m_frozenColumn + m_hiddenColumns.size()-1;
    }

    col = m_frozenColumn+m_hiddenColumns.size();
    while(col < value && !isColumnHidden(col)){
        m_hiddenColumns.push(col);
        this->setColumnHidden(col,true);
        col = m_frozenColumn+m_hiddenColumns.size();

    }
}


void FrozenColTableView::updateHorizScrollBar()
{
    int startCol  = this->columnAt(0);
    int endCol   = this->columnAt(this->viewport()->width());
    if(endCol == -1){
        endCol =  columnCount();

        if(!m_hiddenColumns.isEmpty()){
            int spaceWidth = this->viewport()->width() - columnViewportPosition(endCol-1)-m_columnWidths[endCol-1];
            int col  =  m_hiddenColumns.last();
            while(spaceWidth>m_columnWidths[col]){
                m_hiddenColumns.pop();
                this->setColumnHidden(col,false);
                if(m_hiddenColumns.isEmpty())
                    break;
                else
                    col  =  m_hiddenColumns.last();
                spaceWidth = this->viewport()->width() - columnViewportPosition(endCol-1)-m_columnWidths[endCol-1];
            }
        }
    }

    int  pageStep = endCol - startCol - m_frozenColumn - m_hiddenColumns.size() ;
    this->m_horizBar->setRange(m_frozenColumn,columnCount() - pageStep);
    this->m_horizBar->setPageStep(pageStep);

    if(m_frozenColumn+pageStep == columnCount() && !m_horizBar->isHidden())
        if(m_horizScrollBarPolicy != Qt::ScrollBarAlwaysOn )
            m_horizBar->hide();

    if(m_frozenColumn+pageStep < columnCount() &&  m_horizBar->isHidden())
        if(m_horizScrollBarPolicy != Qt::ScrollBarAlwaysOff )
            m_horizBar->show();
}

///FrozenTableView-----------
FrozenTableView::FrozenTableView(QScrollBar *hscrollBar,QScrollBar *vscrollBar,QWidget *parent)
    :FrozenColTableView(hscrollBar,parent),m_vertBar(vscrollBar)
    ,m_rowCount(0),m_frozenRow(0)
{
    this->horizontalHeader()->setSectionsClickable(true);

    QTableView::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_vertScrollBarPolicy   = Qt::ScrollBarAsNeeded;;

    connect(m_vertBar,&QScrollBar::valueChanged,this,&FrozenTableView::slotVertBarValueChanged);
}


void FrozenTableView::setModel(QAbstractItemModel *model)
{
    initModel(model);
    m_rowCount       =  m_model->rowCount();

    connect(m_model,&QAbstractItemModel::modelReset,this, &FrozenTableView::slotModelReset);
}



void FrozenTableView::setFrozenRowCount(const int &count)
{
    m_frozenRow = count;
}

void FrozenTableView::resizeEvent(QResizeEvent *e)
{
    FrozenColTableView::resizeEvent(e);

    updateVertScrollBar();
}

void FrozenTableView::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_PageUp:
    {
        int value = m_vertBar->value();
        value    -= m_vertBar->pageStep();
        //value     = value>m_vertBar->minimum()?m_vertBar->minimum():value;
        m_vertBar->setValue(value);
        int column = currentIndex().column() == -1?0:currentIndex().column();
        this->setCurrentIndex(model()->index(value,column));
    }
        return QTableView::keyPressEvent(e);
    case Qt::Key_Up:
    {
        int row = currentIndex().row();
        // qDebug()<<"Key_Up"<<row<<m_frozenRow<<m_hiddenRows.size();
        if(row == m_frozenRow+m_hiddenRows.size()){
            m_vertBar->setValue(--row);
        }
    }
        return QTableView::keyPressEvent(e);// return QTableView::keyPressEvent(e)
    case Qt::Key_PageDown:
    {
        //  qDebug()<<currentIndex().row();
        int value   = m_vertBar->value()+m_vertBar->pageStep();

        // qDebug()<<"value :"<<value<<" pre value:"<<m_vertBar->value()<<"pageStep:"<<m_vertBar->pageStep();
        m_vertBar->setValue(value);
        int column = currentIndex().column() == -1?0:currentIndex().column();
        this->setCurrentIndex(model()->index(value,column));
    }
        return QTableView::keyPressEvent(e);
    case Qt::Key_Down:
    {
        int row = currentIndex().row();
        if(rowViewportPosition(row+1)+rowHeight(row+1)>viewport()->height()){
            // qDebug()<<"key down row:"<<m_frozenRow+m_hiddenRows.size();
            m_vertBar->setValue(m_frozenRow+m_hiddenRows.size()+1);
        }
    }
        return QTableView::keyPressEvent(e);
    default:
        break;
    }

    return FrozenColTableView::keyPressEvent(e);
}


void FrozenTableView::wheelEvent(QWheelEvent *e)
{
    int degrees =  e->angleDelta().y()/15/8;
    m_vertBar->setValue(m_vertBar->value()-degrees);
}

void FrozenTableView::slotModelReset()
{
    FrozenColTableView::slotModelReset();

    m_rowCount     =  m_model->rowCount();

    m_vertBar->setValue(m_frozenRow);
    int row;
    while(!m_hiddenRows.isEmpty()){
        row =  m_hiddenRows.pop();
        if(row<m_rowCount)
            this->setRowHidden(row,false);
    }

    //更新
    updateVertScrollBar();
}

void FrozenTableView::updateVertScrollBar()
{
    int startRow = rowAt(0) == -1?0:rowAt(0);
    int endRow  = rowAt(viewport()->height());

    if(endRow == -1){
        endRow = rowCount();

        int spaceWidth = this->viewport()->height() - rowViewportPosition(endRow-1)-rowHeight(endRow-1);
        while(spaceWidth>rowHeight(endRow-1) && !m_hiddenRows.isEmpty()){
            int row  =  m_hiddenRows.pop();
            this->setRowHidden(row,false);
            spaceWidth = this->viewport()->height() - rowViewportPosition(endRow-1)-rowHeight(endRow-1);
        }
    }
    int pageStep;
    if(m_frozenRow>0)
        pageStep = endRow - startRow - m_frozenRow- m_hiddenRows.size();
    else
        pageStep = endRow- startRow;

    m_vertBar->setRange(m_frozenRow,rowCount() - pageStep );
    m_vertBar->setPageStep(pageStep);

    if(m_frozenRow+pageStep == rowCount() && !m_vertBar->isHidden())
        if(m_vertScrollBarPolicy != Qt::ScrollBarAlwaysOn)
            m_vertBar->hide();

    if(m_frozenRow+pageStep < rowCount() &&  m_vertBar->isHidden())
        if(m_vertScrollBarPolicy != Qt::ScrollBarAlwaysOff)
            m_vertBar->show();

    //qDebug()<<"pageStep:"<<pageStep<<"range:"<<rowCount() - pageStep;
}

void FrozenTableView::slotVertBarValueChanged(const int &value)
{
    //考虑到pageStep
    int row = m_frozenRow + m_hiddenRows.size()-1;
    while(row >= value && isRowHidden(row)){
        row = m_hiddenRows.pop();
        this->setRowHidden(row,false);
        row = m_frozenRow + m_hiddenRows.size()-1;
    }
    row = m_frozenRow+m_hiddenRows.size();
    while(row < value && !isRowHidden(row)){
        m_hiddenRows.push(row);
        this->setRowHidden(row,true);
        row = m_frozenRow+m_hiddenRows.size();
    }
}


///FrozenColView----
FrozenView::FrozenView(const FrozenFlag &frozenFlag,QWidget *parent )
    :QWidget(parent),m_frozenFlag(frozenFlag)
{
    initWg();
}


QScrollBar *FrozenView::horizontalScrollBar()
{
    return m_horizScrollbar;
}

QScrollBar *FrozenView::verticalScrollBar()
{
    return  m_frozenFlag == ColRowFrozen?m_vertScrollbar:m_tableView->verticalScrollBar();
}

void FrozenView::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    this->m_tableView->setVerticalScrollBarPolicy(policy);
    if(m_frozenFlag == ColRowFrozen){
        if(policy == Qt::ScrollBarAlwaysOff)
            m_vertScrollbar->hide();
        else if(policy == Qt::ScrollBarAlwaysOn)
            m_vertScrollbar->show();
    }
}

void FrozenView::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    if(policy == Qt::ScrollBarAlwaysOff)
        m_horizScrollbar->hide();
    else if(policy == Qt::ScrollBarAlwaysOn)
        m_horizScrollbar->show();
    this->m_tableView->setHorizontalScrollBarPolicy(policy);
}

void FrozenView::initWg()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    m_horizScrollbar = new QScrollBar(Qt::Horizontal,this);
    if(m_frozenFlag == ColRowFrozen){
        m_vertScrollbar = new QScrollBar(Qt::Vertical,this);
        m_tableView = new FrozenTableView(m_horizScrollbar,m_vertScrollbar);
        mainLayout->addWidget(m_tableView,0,0);
        mainLayout->addWidget(m_vertScrollbar,0,1);
        mainLayout->addWidget(m_horizScrollbar,1,0,1,2);
    }else{
        m_tableView = new FrozenColTableView(m_horizScrollbar);
        mainLayout->addWidget(m_tableView,0,0);
        mainLayout->addWidget(m_horizScrollbar,1,0);
    }

    this->setLayout(mainLayout);
}

