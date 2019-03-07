#include "selfstocksettingdialog.h"
#include <QEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include "windowcontrolbox.h"
#include "zxstockopt.h"
#include "httpdata.h"
#include "searchstockview/seachstockwidget.h"

SelfStockSettingDialog::SelfStockSettingDialog(QWidget *parent):
    BaseDialog(parent),
    m_pHelper(new FramelessHelper(this)),
    m_pInfo(new QLabel),
    m_tableWidget(new QTableWidget)
{
    m_firFont.setFamily("Arial");
    m_firFont.setPixelSize(14);
    m_SecFont.setFamily("SimSun");
    m_SecFont.setPixelSize(14);

    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    this->setMouseTracking(true);
    createWg();
    initTableWidget();
    initMenu();
}

bool SelfStockSettingDialog::eventFilter(QObject *obj, QEvent *eve)
{
    if(obj == m_tableWidget->viewport())
    {
        if(eve->type() == QEvent::Drop)
        {
            const QMimeData *mime = ((QDropEvent*)eve)->mimeData();
            QByteArray encodedata = mime->data("application/x-qabstractitemmodeldatalist");
            if (encodedata.isEmpty())
            {
                return false;
            }
            QDataStream stream(&encodedata, QIODevice::ReadOnly);
            while (!stream.atEnd())
            {
                int row, col;
                QMap<int,  QVariant> roleDataMap;
                ///拖的row和col
                stream >> row >> col >> roleDataMap;
                QTableWidgetItem* pDropItem = m_tableWidget->itemAt(((QDropEvent*)eve)->pos());
                if(!pDropItem)
                {
                    return true;
                }

                //放的row
                if(pDropItem->row() == row)
                {
                    return true;
                }

                ///自己的实现TODO:
                int dragRow = row;                      //拖动行
                int dropRow = pDropItem->row();         //放下行


                TableData data;

                data = m_dataVec[dragRow];
                m_dataVec.remove(dragRow);
                m_dataVec.insert(dropRow, data);

                m_tableWidget->clearContents();




                for(int i = 0; i < m_dataVec.size(); i++)
                {

                    QTableWidgetItem *item1 = new QTableWidgetItem(m_dataVec[i].strSecCol);

                    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

                    QString strFirCol = m_dataVec[i].strFirCol;

                    QTableWidgetItem *item0 = new QTableWidgetItem("       " + strFirCol.remove(0, 2));

                    item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

                    item1->setFont(m_SecFont);

                    item0->setFont(m_firFont);

                    m_tableWidget->setItem(i,0,item0);

                    m_tableWidget->setItem(i, 1, item1);

                }

                m_tableWidget->selectRow(dropRow);
                return true;    //不要交给系统处理，否则他会给你新增一行
            }
        }else
        {
            return QWidget::eventFilter(obj,eve);
        }
    }
    return QWidget::eventFilter(obj,eve);
}

void SelfStockSettingDialog::mousePressEvent(QMouseEvent *event)
{
    BaseDialog::mousePressEvent(event);


    if(event->button() == Qt::LeftButton)
    {
        QPoint pt = event->pos();

        if(!m_menu->rect().contains(pt)){
            m_menu->hide();
        }
    }


}

void SelfStockSettingDialog::on_moveUp_clicked()
{
//    int currentRow = m_tableWidget->currentRow();
//    int toRow = m_tableWidget->currentRow() - 1;




    moveRow(m_tableWidget,m_tableWidget->currentRow(), m_tableWidget->currentRow() - 1);

}


void SelfStockSettingDialog::on_moveDown_clicked()
{
    moveRow(m_tableWidget,m_tableWidget->currentRow(),m_tableWidget->currentRow() + 2);
}

void SelfStockSettingDialog::on_remove_clicked()
{
    int index = m_tableWidget->currentRow();

    if(-1 == index)
        return;

    m_tableWidget->removeRow(index);

    m_dataVec.remove(index);

    setTextEx(m_tableWidget->rowCount());
}

void SelfStockSettingDialog::on_add_clicked()
{
    QPoint pt = this->rect().bottomRight() - QPoint(230, 220);

    m_menu->move(this->mapToGlobal(pt));
    m_menu->show();
//    m_menu->activateWindow();
}

void SelfStockSettingDialog::onRowClicked(const QString &strCode)
{
    qDebug() << "strCode = " << strCode;

    m_menu->hide();

    if(m_dataVec.size() >= 100)
    {
        QMessageBox::information(this, "提示", "目前只可添加100个自选股", QMessageBox::Ok);
        return;
    }

    foreach (TableData data, m_dataVec) {
        if(data.strFirCol == strCode)
        {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("列表中已存在该股票"));
            return;
        }
    }

    TableData data;
    m_tableWidget->insertRow(m_tableWidget->rowCount());
    int row = m_tableWidget->rowCount() - 1;

    data.strFirCol = strCode;

    data.strSecCol = Httpdata::instance()->getCodeNameByCode(strCode);

    m_dataVec.append(data);

    QTableWidgetItem *item1 = new QTableWidgetItem(Httpdata::instance()->getCodeNameByCode(strCode));

    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *item0 = new QTableWidgetItem("       " + strCode.mid(2));

    item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);


    item1->setFont(m_SecFont);

    item0->setFont(m_firFont);


    m_tableWidget->setItem(row,0,item0);

    m_tableWidget->setItem(row, 1, item1);


    setTextEx(m_tableWidget->rowCount());

}

void SelfStockSettingDialog::on_submit_clicked()
{
    this->close();

    bool ret = false;
    if(m_oldVec.size() == m_dataVec.size()  &&  !m_dataVec.isEmpty())
    {
        for(int i = 0; i < m_oldVec.size() ; i++)
        {
            if(m_oldVec[i].strFirCol != m_dataVec[i].strFirCol )
            {
                ret = false;
                break;
            }
            else
            {
                ret = true;
            }
        }
    }

    if(ret)
        return;

    m_addList.clear();
    foreach (TableData data, m_dataVec) {
        m_addList.append(data.strFirCol);
    }
//    if(m_addList.isEmpty())
//        return;
    ZXStockOpt::instance()->updateZXStock(m_addList);
}

void SelfStockSettingDialog::on_cancel_clicked()
{
    m_addList.clear();

    this->close();
}

void SelfStockSettingDialog::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(1);

    QWidget *titleWidget = new QWidget;
    titleWidget->setFixedHeight(30);
    titleWidget->setObjectName("title");

    QWidget *bodyWidget = new QWidget;
    bodyWidget->setObjectName("body");

    titleWidget->setLayout(createTitleBar());

    bodyWidget->setLayout(createBody());

    mainlayout->addWidget(titleWidget);

    mainlayout->addWidget(bodyWidget);
}

QBoxLayout *SelfStockSettingDialog::createTitleBar()
{
    WindowControlBox *windowControlBox = new WindowControlBox(this, this, false, false);

    QLabel *label = new QLabel;

    label->setText(QStringLiteral("自选股设置"));

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setSpacing(0);
    layout->setContentsMargins(28, 0, 0, 0);

    layout->addWidget(label);
    layout->addStretch();
    layout->addWidget(windowControlBox);

    return layout;
}

QBoxLayout *SelfStockSettingDialog::createBody()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
//    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0, 15, 0, 0);

    m_pInfo->setText(QStringLiteral("自选股"));
    m_pInfo->setObjectName("info");

    QVBoxLayout *buttonLayout  = new QVBoxLayout;
    buttonLayout->setSpacing(11);
    buttonLayout->setMargin(0);
    QPushButton *addBtn = new QPushButton(QStringLiteral("添加"));
    QPushButton *removeBtn = new QPushButton(QStringLiteral("删除"));
    QPushButton *moveUpBtn = new QPushButton(QStringLiteral("上移"));
    QPushButton *moveDownBtn = new QPushButton(QStringLiteral("下移"));
    addBtn->setObjectName("menubtn");
    removeBtn->setObjectName("menubtn");
    moveUpBtn->setObjectName("menubtn");
    moveDownBtn->setObjectName("menubtn");

    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(removeBtn);
    buttonLayout->addWidget(moveUpBtn);
    buttonLayout->addWidget(moveDownBtn);


    connect(addBtn, &QPushButton::clicked, this, &SelfStockSettingDialog::on_add_clicked);
    connect(moveUpBtn, &QPushButton::clicked, this, &SelfStockSettingDialog::on_moveUp_clicked);
    connect(moveDownBtn, &QPushButton::clicked, this, &SelfStockSettingDialog::on_moveDown_clicked);
    connect(removeBtn, &QPushButton::clicked, this, &SelfStockSettingDialog::on_remove_clicked);


    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(28, 0, 0, 0);
    topLayout->addWidget(m_pInfo);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    centerLayout->setSpacing(12);
    centerLayout->setContentsMargins(24, 0, 0, 0);
    centerLayout->addWidget(m_tableWidget);

    m_tableWidget->setFixedSize(350, 200);
    centerLayout->addLayout(buttonLayout);
    centerLayout->setAlignment(buttonLayout, Qt::AlignTop);
    centerLayout->addStretch();


    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(26, 0, 13, 0);

    QLabel *logo = new QLabel;
    logo->setObjectName("logo");
    QLabel *label = new QLabel(QStringLiteral("股票支持拖拽移动位置"));
    label->setObjectName("warning");

    QPushButton *submitBtn = new QPushButton(QStringLiteral("确定"));
    QPushButton *cancelBtn = new QPushButton(QStringLiteral("取消"));

    submitBtn->setObjectName("btngroup");
    cancelBtn->setObjectName("btngroup");

    connect(submitBtn, &QPushButton::clicked, this, &SelfStockSettingDialog::on_submit_clicked);
    connect(cancelBtn, &QPushButton::clicked, this, &SelfStockSettingDialog::on_cancel_clicked);

    bottomLayout->addWidget(logo);
    bottomLayout->addSpacing(5);
    bottomLayout->addWidget(label);
    bottomLayout->addStretch();
    bottomLayout->addWidget(submitBtn);
    bottomLayout->addSpacing(12);
    bottomLayout->addWidget(cancelBtn);

    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(centerLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();

    return mainLayout;
}

void SelfStockSettingDialog::initTableWidget()
{


    m_tableWidget->setColumnCount(2);

    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->verticalHeader()->hide();
    m_tableWidget->horizontalHeader()->hide();
    m_tableWidget->setShowGrid(false);
    m_tableWidget->verticalHeader()->setDefaultSectionSize(23);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);



    ZXStockOpt *zxStockOpt = ZXStockOpt::instance();

    QVector<QString> vec = zxStockOpt->getZXStock();


    for(int i = 0; i < vec.size(); i++)
    {

        TableData data;
        m_tableWidget->insertRow(m_tableWidget->rowCount());
        int row = m_tableWidget->rowCount() - 1;

        data.strFirCol = vec[i];

        data.strSecCol = Httpdata::instance()->getCodeNameByCode(vec[i]);

        m_dataVec.append(data);

        QTableWidgetItem *item1 = new QTableWidgetItem(Httpdata::instance()->getCodeNameByCode(vec[i]));

        item1->setFont(m_SecFont);

        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QTableWidgetItem *item0 = new QTableWidgetItem("       " + vec[i].remove(0, 2));

        item0->setFont(m_firFont);

        item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);


        m_tableWidget->setItem(row,0,item0);

        m_tableWidget->setItem(row, 1, item1);
    }

    m_oldVec = m_dataVec;

    setTextEx(m_tableWidget->rowCount());

    m_tableWidget->viewport()->installEventFilter(this);
    //拖放使能
    m_tableWidget->setDragDropMode(QAbstractItemView::DragDrop);
    m_tableWidget->setDragEnabled(true);
    m_tableWidget->setAcceptDrops(true);

    m_tableWidget->setDropIndicatorShown(false);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);        //以行为单位
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);        //不能编辑行内容
    m_tableWidget->setFocusPolicy(Qt::NoFocus);
}

void SelfStockSettingDialog::initMenu()
{
    m_menu = new QMenu(this);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    SeachStockWidget *searchWidget = new SeachStockWidget;
    layout->addWidget(searchWidget);
    m_menu->setLayout(layout);

    m_menu->setWindowFlags(Qt::Sheet | windowFlags());

    connect(searchWidget, &SeachStockWidget::rowClicked, this, &SelfStockSettingDialog::onRowClicked);
}

void SelfStockSettingDialog::moveRow(QTableWidget *pTable, int currentRow, int toRow)
{
    if(pTable == NULL)
        return;

    int nRowCount = pTable->rowCount();
    int nColumnCount = pTable->columnCount();


    if(currentRow == toRow)
        return;

    if(currentRow < 0 || toRow <0 ||currentRow > nRowCount || toRow > nRowCount)
        return;

    swapData(currentRow, toRow);

    if(toRow < currentRow)
        currentRow++;

    pTable->insertRow(toRow);

    for(int i = 0; i <nColumnCount;i++)
    {
        pTable->setItem(toRow,i,pTable->takeItem(currentRow,i));
    }
    if(currentRow < toRow)
        toRow--;

    pTable->removeRow(currentRow);
    pTable->selectRow(toRow);
}

void SelfStockSettingDialog::setTextEx(int nCount)
{
    m_pInfo->setText(QStringLiteral("自选股(%1只)").arg(nCount));
}

void SelfStockSettingDialog::swapData(int currentRow, int toRow)
{
    if(currentRow < toRow)
        --toRow;
    TableData tmp = m_dataVec[currentRow];
    m_dataVec[currentRow] = m_dataVec[toRow];
    m_dataVec[toRow] = tmp;
}
