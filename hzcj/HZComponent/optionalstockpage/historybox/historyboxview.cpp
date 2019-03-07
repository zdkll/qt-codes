#include "historyboxview.h"
#include <QHeaderView>
#include "historyboxmodel.h"

HistoryBoxView::HistoryBoxView(QWidget *parent):
    QTableView(parent)
{
    initTableView();
}

void HistoryBoxView::initTableView()
{
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setShowGrid(false);
    this->setWordWrap(false);
    this->setFocusPolicy(Qt::NoFocus);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->verticalHeader()->setDefaultSectionSize(30);
}
