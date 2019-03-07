#include "ShortTermChangeView.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QWheelEvent>
#include "ShortTermChangedelegate.h"
#include "ShortTermChangemodel.h"

#define Default_Vertical_Section_Height 22

ShortTermChangeView::ShortTermChangeView(QWidget *parent):
    QTableView(parent),
    m_shortTermModel(new ShortTermChangeModel(this)),
    m_bSelected(false)
{
//    this->setFixedHeight(186);
//    this->setMinimumWidth(900);
    initTableView();
}

int ShortTermChangeView::maxRowCount() const
{
    int nHeight = this->viewport()->height();

    return nHeight / Default_Vertical_Section_Height;
}

void ShortTermChangeView::upDateData(const QVector<ShortMotion> &fundVec)
{
    m_shortTermModel->upDateData(fundVec.mid(0, this->maxRowCount()));

    if(fundVec.isEmpty())
        return;
    if(!m_bSelected)
    {
        m_bSelected = true;
        int nRow = this->rowCount() - 1;
        emit clickedRow(QString::fromStdString(fundVec[nRow].stock_code));
    }

}

void ShortTermChangeView::clearData()
{
    m_shortTermModel->clearData();
}

int ShortTermChangeView::rowCount()
{
    return m_shortTermModel->rowCount();
}



void ShortTermChangeView::initTableView()
{
    this->setModel(m_shortTermModel);
    //隐藏水平滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QTableView::SingleSelection);

    this->verticalHeader()->setDefaultSectionSize(22);

    //隐藏表头
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    //设置最后一行填充剩余空间
    this->horizontalHeader()->setStretchLastSection(true);

    //设置代理
    this->setItemDelegate(new ShortTermChangeDelegate(this));

    connect(this, &QAbstractItemView::clicked, this , &ShortTermChangeView::onRowClicked);

}

void ShortTermChangeView::onRowClicked(const QModelIndex &index)
{
    ShortTermData data = index.data(MYDATAROLE).value<ShortTermData>();

    emit clickedRow(data.strCode);
}
