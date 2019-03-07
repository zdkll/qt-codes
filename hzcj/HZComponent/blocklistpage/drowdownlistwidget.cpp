#include "drowdownlistwidget.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

DrowDownListWidget::DrowDownListWidget(QWidget *parent):
    BaseWidget(parent),
    m_pBtn(new QPushButton),
    m_tableWiget(new QTableWidget)
{
//    this->setFixedWidth(630);
//    this->setFixedHeight(480);
    this->setFixedSize(630, 480);
    initTableWiget();
    createWg();
}

void DrowDownListWidget::setTitle(const QString &strText)
{
    m_pBtn->setText(strText);
}

void DrowDownListWidget::setData(const QVector<BlockFormatDataItem> &vec)
{
    if(vec.isEmpty())
        return;

    m_itemVec = vec;
    setTableWidget();
}

void DrowDownListWidget::createWg()
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(1);
    QWidget *widget = new QWidget;
    widget->setObjectName("title");
    widget->setFixedHeight(30);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(11);
    layout->addWidget(m_pBtn);
    layout->addStretch();
    widget->setLayout(layout);
    mainlayout->addWidget(widget, 0, Qt::AlignTop);
    mainlayout->addWidget(m_tableWiget);

    connect(m_pBtn, &QPushButton::clicked, [&]{emit clicked();});

    connect(m_tableWiget, &QTableWidget::cellClicked, this, &DrowDownListWidget::onCellClicked);
}

void DrowDownListWidget::initTableWiget()
{
    m_tableWiget->setObjectName("tablewidget");
    m_tableWiget->setShowGrid(false);
    m_tableWiget->setFrameShape(QFrame::NoFrame);
    m_tableWiget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWiget->horizontalHeader()->setVisible(false);
    m_tableWiget->verticalHeader()->setVisible(false);
    m_tableWiget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWiget->setColumnCount(7);
}

void DrowDownListWidget::setTableWidget()
{
    QVector<QString> dataVec;
    for(int i = 0; i < m_itemVec.size(); i++)
    {
        dataVec.append(m_itemVec[i].firstChar);

        int count;
        if(0 == m_itemVec[i].dataList.size() % 6)
        {
            count = m_itemVec[i].dataList.size() / 6;
        }
        else
        {
            count = m_itemVec[i].dataList.size() / 6 + 1;
        }

        for(int j = 0; j < count*7 - 1; j++)
        {

            if((j > 0 &&(0 == ( j + 1)% 7)) || j > m_itemVec[i].dataList.size() - 1)
            {
                dataVec.append("");
            }
            else
            {
                dataVec.append(m_itemVec[i].dataList[j].stock_name);
            }

        }
    }

    int nSize = dataVec.size();

    if(nSize % 7 == 0){
        nSize /=7;
        m_tableWiget->setRowCount(nSize);
    }
    else{
        nSize = nSize / 7 + 1;
        m_tableWiget->setRowCount(nSize);
    }


    int num = 0;

    for(int row = 0; row < nSize; row++)
    {
        m_tableWiget->setRowHeight(row, ITEMHEIGHT);
        for(int column = 0; column < 7; column++)
        {
            m_tableWiget->setColumnWidth(column, ITEMWIDTH);

            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(dataVec[num]);


            m_tableWiget->setItem(row, column, tableWidgetItem);

            tableWidgetItem->setTextAlignment(Qt::AlignCenter);
//            tableWidgetItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            num++;
        }
    }


}

void DrowDownListWidget::onCellClicked(int row, int column)
{
    QTableWidget *widget = qobject_cast<QTableWidget*>(this->sender());
    if(nullptr == widget)
        return;

    QTableWidgetItem * widgetItem = widget->item(row, column);
    QString strName = widgetItem->text();

    for(int i = 0; i < m_itemVec.size(); i++)
    {
        for(int j = 0; j < m_itemVec[i].dataList.size(); j++)
        {
            if(m_itemVec[i].dataList[j].stock_name == strName)
            {
                emit cellClicked(strName, m_itemVec[i].dataList[j].stock_code);
                return;
            }
        }
    }
    return;
}
