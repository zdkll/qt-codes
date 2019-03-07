#include "searchstockview.h"
#include <QHeaderView>
#include <QKeyEvent>
#include <QDebug>
#include "searchstockmodel.h"


SearchStockView::SearchStockView(QWidget *parent):
    QTableView(parent),
    m_model(new SearchStockModel(this))
{
    initTableView();

    connect(this, &QTableView::doubleClicked, this, &SearchStockView::onRowClicked);

}

void SearchStockView::upDateData(QList<SearchStockData> &list)
{
    m_model->upDateData(list);
}

//void SearchStockView::keyPressEvent(QKeyEvent *e)
//{
//    QTableView::keyPressEvent(e);

//    qDebug() << e->key();
//    if(e->key() == Qt::Key_Return)
//    {

//        QModelIndex index = this->currentIndex();
//        emit rowClicked(index.data(CODEROLE).toString());
//    }
//}

void SearchStockView::initTableView()
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setShowGrid(false);
//    this->setFocusPolicy(Qt::NoFocus);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->setModel(m_model);
    this->verticalHeader()->setDefaultSectionSize(20);
    this->horizontalHeader()->setStretchLastSection(true);
}

void SearchStockView::onRowClicked(const QModelIndex &index)
{
    emit rowClicked(index.data(CODEROLE).toString());
}
