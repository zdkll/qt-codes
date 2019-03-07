#include "timedealtableview.h"

#define Default_Vertical_Section_Height  22

#include <QHeaderView>

#include "../itemmodels/timedealmodel.h"
#include "../itemdelegates/timedealitemdelegates.h"


TimeDealTableView::TimeDealTableView(QWidget *parent)
    :QTableView(parent)
{
    initTableView();
    this->setFocusPolicy(Qt::NoFocus);
}

TimeDealTableView::~TimeDealTableView()
{

}

void TimeDealTableView::setData(const QVector<HZData::HZTick> &ticks)
{
    m_dataModel->setData(ticks);
}

void TimeDealTableView::clearData()
{
 m_dataModel->clearData();
}

int TimeDealTableView::maxRowCount() const
{
    int height =  this->viewport()->height();

    return height/Default_Vertical_Section_Height;
}

int  TimeDealTableView::rowCount() const
{
    return m_dataModel->rowCount();
}

void TimeDealTableView::setStretchFactors(const QList<float> &factors)
{
    m_stretchFactors = factors;
}

void TimeDealTableView::resizeEvent(QResizeEvent *e)
{
    int width =  this->viewport()->width();
    for(int i=0;i<m_stretchFactors.size();i++){
        this->setColumnWidth(i,width*m_stretchFactors[i]);
    }

    return QTableView::resizeEvent(e);
}


void TimeDealTableView::initTableView()
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->verticalHeader()->hide();

    m_dataModel = new TimeDealModel(this);

    this->setModel(m_dataModel);

    this->verticalHeader()->setDefaultSectionSize(Default_Vertical_Section_Height);

    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->setDefaultSectionSize(Default_Row_Height);
}


