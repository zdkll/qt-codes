#include "freezetableview.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QApplication>


FreezeTableView::FreezeTableView(QWidget *parent)
    :QTableView(parent),m_model(0),m_freezeColumn(1)
{
    initTable();
}

FreezeTableView::FreezeTableView(QAbstractItemModel * model,QWidget *parent )
    :QTableView(parent),m_model(model),m_freezeColumn(1)
{
    initTable();
    setModel(model);
}

void FreezeTableView::setModel(QAbstractItemModel *model)
{
    m_model = model;
    initFromModel(model);
}

void FreezeTableView::setFreezeColumn(const int &column)
{
    m_freezeColumn = column;
    if(!m_model)
        return;

    Q_ASSERT(m_freezeColumn>=0 && m_freezeColumn<=model()->columnCount());

    updateFrozenTable();
}

void FreezeTableView::setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior)
{
    QTableView::setSelectionBehavior(behavior);
    frozenTableView->setSelectionBehavior(behavior);
}

void FreezeTableView::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    QTableView::setSelectionMode(mode);
    frozenTableView->setSelectionMode(mode);
}

void FreezeTableView::setColumnWidth(int column, int width)
{
    QTableView::setColumnWidth(column,width);
    if(column<m_freezeColumn){
        frozenTableView->setColumnWidth(column,width);
        updateFrozenTableGeometry();
    }
}

void FreezeTableView::setVerticalSetcionSize(int size)
{
    QTableView::verticalHeader()->setDefaultSectionSize(size);
    frozenTableView->verticalHeader()->setDefaultSectionSize(size);
}

void FreezeTableView::setSortingEnabled(bool enabled)
{
    QTableView::setSortingEnabled(enabled);
    frozenTableView->setSortingEnabled(enabled);
}

void FreezeTableView::setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate)
{
    if(column<m_freezeColumn)
        frozenTableView->setItemDelegateForColumn(column,delegate);
    else QTableView::setItemDelegateForColumn(column,delegate);
}

void FreezeTableView::setColumnHidden(int column, bool hide)
{
    if(column<m_freezeColumn)
        frozenTableView->setColumnHidden(column,hide);
    QTableView::setColumnHidden(column,hide);
}

void FreezeTableView::setSortIndicator(const int &column,const Qt::SortOrder &order)
{
    if(column<m_freezeColumn){
        frozenTableView->horizontalHeader()->setSortIndicator(column,order);

        frozenTableView->horizontalHeader()->setSortIndicatorShown(true);
        horizontalHeader()->setSortIndicatorShown(false);
    }else{
        horizontalHeader()->setSortIndicator(column,order);

        frozenTableView->horizontalHeader()->setSortIndicatorShown(false);
        horizontalHeader()->setSortIndicatorShown(true);
    }
}

void FreezeTableView::setShowHorizontalHeader(const bool &enable)
{
    this->horizontalHeader()->setHidden(!enable);
    frozenTableView->horizontalHeader()->setHidden(!enable);
}

void FreezeTableView::setShowVerticalHeader(const bool &enable)
{
    this->verticalHeader()->setHidden(!enable);
    frozenTableView->verticalHeader()->setHidden(!enable);
}

void FreezeTableView::setVerticalScrollMode(ScrollMode mode)
{
    QTableView::setVerticalScrollMode(mode);
    frozenTableView->setVerticalScrollMode(mode);
}

void FreezeTableView::setHorizontalScrollMode(ScrollMode mode)
{
    QTableView::setHorizontalScrollMode(mode);
    frozenTableView->setHorizontalScrollMode(mode);
}

FreezeTableView::~FreezeTableView()
{
    if(frozenTableView)
        delete frozenTableView;
}

void FreezeTableView::setObjectName(const QString &objName)
{
    QTableView::setObjectName(objName);
    this->horizontalHeader()->setObjectName(objName);
    frozenTableView->setObjectName(objName);

    this->horizontalHeader()->setObjectName(objName);
    frozenTableView->horizontalHeader()->setObjectName(objName);

    this->style()->unpolish(horizontalHeader());
    this->style()->polish(horizontalHeader());

    this->style()->unpolish(horizontalScrollBar());
    this->style()->polish(horizontalScrollBar());

    this->style()->unpolish(verticalScrollBar());
    this->style()->polish(verticalScrollBar());

    this->style()->unpolish(frozenTableView);
    this->style()->polish(frozenTableView);

    frozenTableView->style()->unpolish(frozenTableView->horizontalHeader());
    frozenTableView->style()->polish(frozenTableView->horizontalHeader());

//    QFont ft = qApp->font();
//    ft.setStyleStrategy(QFont::NoAntialias);
//    qApp->setFont(ft);
}

void FreezeTableView::setShowGrid(const bool &show)
{
    QTableView::setShowGrid(show);
    frozenTableView->setShowGrid(show);
}

void FreezeTableView::initFromModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    frozenTableView->setModel(model);
    frozenTableView->setSelectionModel(selectionModel());

    updateFrozenTable();
}

void FreezeTableView::initTable()
{
    frozenTableView = new QTableView(this);
    frozenTableView->setObjectName("frozentableview");
    frozenTableView->setFocusPolicy(Qt::NoFocus);
    frozenTableView->verticalHeader()->hide();
    frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    viewport()->stackUnder(frozenTableView);
    //! [init part1]

    //! [init part2]
    frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableView->show();

    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);
    frozenTableView->setVerticalScrollMode(ScrollPerPixel);

    //    this->setSelectionMode(QAbstractItemView::SingleSelection);
    //    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    frozenTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //    frozenTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //! [init part3]
    //connect the headers and scrollbars of both tableviews together
    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
            &FreezeTableView::updateSectionWidth);
    connect(verticalHeader(),&QHeaderView::sectionResized, this,
            &FreezeTableView::updateSectionHeight);

    connect(frozenTableView->verticalScrollBar(), &QAbstractSlider::valueChanged,
            verticalScrollBar(), &QAbstractSlider::setValue);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged,
            frozenTableView->verticalScrollBar(), &QAbstractSlider::setValue);

    connect(horizontalHeader(),&QHeaderView::sortIndicatorChanged,this,&FreezeTableView::sortIndicatorChanged);
    connect(frozenTableView->horizontalHeader(),&QHeaderView::sortIndicatorChanged,horizontalHeader(),&QHeaderView::sortIndicatorChanged);

    connect(frozenTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SIGNAL(doubleClicked(const QModelIndex &)));
}

void FreezeTableView::updateFrozenTable()
{
    for (int col = 0; col < model()->columnCount(); ++col)
        if(col<m_freezeColumn)
            frozenTableView->setColumnHidden(col, false);
        else
            frozenTableView->setColumnHidden(col, true);

    for(int col=0;col<m_freezeColumn;col++)
        frozenTableView->setColumnWidth(col, columnWidth(col) );

    updateFrozenTableGeometry();
}

//! [sections]
void FreezeTableView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    if (logicalIndex < m_freezeColumn){
        frozenTableView->setColumnWidth(logicalIndex, newSize);
        updateFrozenTableGeometry();
    }
}

void FreezeTableView::updateSectionHeight(int logicalIndex, int /* oldSize */, int newSize)
{
    frozenTableView->setRowHeight(logicalIndex, newSize);
}



//! [sections]
void FreezeTableView::sortIndicatorChanged(int column,Qt::SortOrder)
{
    if(column<m_freezeColumn){
        frozenTableView->horizontalHeader()->setSortIndicatorShown(true);
        horizontalHeader()->setSortIndicatorShown(false);
    }else{
        frozenTableView->horizontalHeader()->setSortIndicatorShown(false);
        horizontalHeader()->setSortIndicatorShown(true);
    }
}

//! [resize]
void FreezeTableView::resizeEvent(QResizeEvent * event)
{
    QTableView::resizeEvent(event);
    updateFrozenTableGeometry();
}
//! [resize]


//! [navigate]
QModelIndex FreezeTableView::moveCursor(CursorAction cursorAction,
                                        Qt::KeyboardModifiers modifiers)
{

    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);
    //    qDebug()<<"move cursor,current column:"<< current.column()
    //           <<frozenTableView->width()<<columnWidth(0)<<visualRect(current).topLeft().x();
    if (cursorAction == MoveLeft && current.column() >= m_freezeColumn
            && visualRect(current).topLeft().x() < frozenTableView->width() ){

        const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
                - frozenTableView->width();
        //        qDebug()<<"set value:"<<newValue;
        horizontalScrollBar()->setValue(newValue);
    }
    return current;
}
//! [navigate]

void FreezeTableView::scrollTo (const QModelIndex & index, ScrollHint hint){
    if (index.column() >= m_freezeColumn)
        QTableView::scrollTo(index, hint);
}

//! [geometry]
void FreezeTableView::updateFrozenTableGeometry()
{
    int w = 0;
    for(int i=0;i<m_freezeColumn;i++)
        w += columnWidth(i);
    frozenTableView->setGeometry(verticalHeader()->width() + frameWidth(),
                                 frameWidth(), w,
                                 viewport()->height()+horizontalHeader()->height());
}
//! [geometry]
