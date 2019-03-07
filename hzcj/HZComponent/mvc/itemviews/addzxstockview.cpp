#include "addzxstockview.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QVariant>

#include "stockadddialog.h"

AddZXStockView::AddZXStockView(ZXViewType type,QWidget *parent)
    :BaseWidget(parent,"addzxstockview")
{
    createWg();

    this->setProperty("ZXViewType",type);
}

void AddZXStockView::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->setContentsMargins(0,0,0,0);
    centerLayout->setSpacing(16);
    centerLayout->addStretch(1);

    //内容
    QPushButton *addBtn = new QPushButton(this);
    addBtn->setFixedSize(84,84);
    addBtn->setObjectName("addbtn");
    QLabel          *label     = new QLabel(QStringLiteral("暂无股票 点击添加"),this);
    label->setObjectName("hintlabel");

    centerLayout->addWidget(addBtn,0,Qt::AlignCenter);
    centerLayout->addWidget(label);

    centerLayout->addStretch(1);

    mainLayout->addStretch(1);
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch(1);

    this->setLayout(mainLayout);

//    m_addStockDlg = new StockAddDialog(this);

    connect(addBtn,&QPushButton::clicked,this,&AddZXStockView::slotAddStock);

}

void AddZXStockView::slotAddStock()
{
//  m_addStockDlg->exec();

  StockAddDialog dlg;
  dlg.exec();
//  emit addFinshed();
}
