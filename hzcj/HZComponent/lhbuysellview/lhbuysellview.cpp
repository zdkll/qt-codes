#include "lhbuysellview.h"
#include <QHeaderView>
#include "lhbuysellmodel.h"
#include "lhbuyselldelegate.h"
#include <QDebug>

LhBuySellView::LhBuySellView(QWidget *parent, int nType):
    QTableView(parent),
    m_pLhmodel(new LhBuySellModel(this, nType))
{
    this->setFixedHeight(135);
    initTableView();
}

void LhBuySellView::updateLhData(const StockLhbDetialListItem &data)
{
    m_pLhmodel->updateLhData(data);
}

void LhBuySellView::resizeEvent(QResizeEvent *event)
{
    int nWidth = this->width();
    this->setColumnWidth(0, nWidth*1/6);
    this->setColumnWidth(1, nWidth*1/3);
    this->setColumnWidth(2, nWidth*1/6);
    this->setColumnWidth(3, nWidth*1/6);
}

void LhBuySellView::initTableView()
{

    this->setModel(m_pLhmodel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QTableView::SingleSelection);
    this->verticalHeader()->hide();
    this->verticalHeader()->setDefaultSectionSize(22);
    this->horizontalHeader()->setFixedHeight(25);
    this->horizontalHeader()->setStretchLastSection(true);
    this->setItemDelegate(new LhBuySellDelegate(this));

    connect(this, &QAbstractItemView::clicked,[&](const QModelIndex &index){
        //qDebug() << index.data(LHYYBIDROLE).toString();
//        qDebug() << index.data(LHYYBIDROLE).toString();
        emit clickRow(index.data(LHYYBIDROLE).toString());
    });
}
