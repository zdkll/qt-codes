#include "buysellfiveview.h"
#include <QHeaderView>
#include "buysellfivemodel.h"
#include "buysellfivedelegate.h"


BuySellFiveView::BuySellFiveView(int type, QWidget *parent):
    QTableView(parent),
    m_model(new BuySellFiveModel(type, this))
{
    if(0 == type)
    {
        this->setObjectName("buyfiveview");
    }
    else
    {
       this->setObjectName("sellfiveview");
    }
    this->setFixedHeight(100);
    this->setFixedWidth(175);
    initTableView();
}

void BuySellFiveView::updateData(const QVector<HZData::AllStock> &vec)
{
    m_model->updateData(vec);
}

void BuySellFiveView::initTableView()
{
    this->setModel(m_model);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setSelectionMode(QTableView::NoSelection);

    this->verticalHeader()->setDefaultSectionSize(20);

    //隐藏表头
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    //设置最后一行填充剩余空间
    this->horizontalHeader()->setStretchLastSection(true);

    //设置代理
    this->setItemDelegate(new BuySellFiveDelegate(this));
}
