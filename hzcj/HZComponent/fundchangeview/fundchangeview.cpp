#include "FundChangeView.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QWheelEvent>
#include "fundchangedelegate.h"
#include "fundchangemodel.h"

#define Default_Vertical_Section_Height 22

FundChangeView::FundChangeView(QWidget *parent):
    QTableView(parent),
    m_fundModel(new FundChangeModel(this))
{
//    this->setFixedHeight(186);
//    this->setMinimumWidth(900);
    initTableView();
}

int FundChangeView::maxRowCount() const
{
    int nHeight = this->viewport()->height();

    return nHeight / Default_Vertical_Section_Height;
}

void FundChangeView::upDateData(const QVector<ShortMotion> &fundVec)
{
    m_fundModel->upDateData(fundVec.mid(0, this->maxRowCount()));
//    QModelIndex index = this->selectionModel()->currentIndex();

//    FundData data = index.data(MYDATAROLE).value<FundData>();

//    emit clickRow(data.strCode);
}

void FundChangeView::clearData()
{
    m_fundModel->clearData();
}

int FundChangeView::rowCount()
{
    return m_fundModel->rowCount();
}

void FundChangeView::setCurrentIndexEx(const QModelIndex &index)
{
    this->setCurrentIndex(index);
}


//void FundChangeView::wheelEvent(QWheelEvent *event)
//{
//    Q_UNUSED(event)

//    QScrollBar *scrollBar = this->verticalScrollBar();


//    int numSteps = event->angleDelta().y()/8/15;

//    int value = scrollBar->value();
//    value -= numSteps*scrollBar->singleStep();


//    if(value != scrollBar->maximum())
//    {
//        emit wheelMoved(true);
//    }
//    else
//    {
//        emit wheelMoved(false);
//    }

//    scrollBar->setValue(value);
//    qDebug() << "value：" << value;

//    qDebug() << "scrollBar->maximum:" << scrollBar->maximum();
//}

//void FundChangeView::updateList(const STInfoData &lst)
//{
//    m_newsModel->updateList(lst.datas);
//}

//void FundChangeView::updateContent(const STDetailDescript &content)
//{

//}

void FundChangeView::initTableView()
{
    this->setModel(m_fundModel);
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
    this->setItemDelegate(new FundChangeDelegate(this));

    connect(this, &QAbstractItemView::clicked, this , &FundChangeView::onRowClicked);

}

void FundChangeView::onRowClicked(const QModelIndex &index)
{
    FundData data = index.data(MYDATAROLE).value<FundData>();

    emit clickRow(data.strCode);
}
