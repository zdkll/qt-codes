#include "stockadddialog.h"
#include <QLabel>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "historyboxwidget.h"
#include "infostyle.h"
#include "zxstockopt.h"

StockAddDialog::StockAddDialog(QWidget *parent):
    BaseDialog(parent),
    m_pHelper(new FramelessHelper(this)),
    m_bState(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    createWg();
}

StockAddDialog::~StockAddDialog()
{

}

void StockAddDialog::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    pMainLayout->addWidget(createTitle());
    pMainLayout->addWidget(createBody());
}

QWidget *StockAddDialog::createTitle()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("title");
    QLabel *label = new QLabel("添加股票");
    m_controlBox = new WindowControlBox(this, this, false, false);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addSpacing(173);
    pLayout->addWidget(label);
    pLayout->addWidget(m_controlBox, 0, Qt::AlignRight);

    widget->setLayout(pLayout);

    return widget;
}

QWidget *StockAddDialog::createBody()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("body");
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setAlignment(Qt::AlignCenter);
    lineEdit->setPlaceholderText("代码/简拼/名称");
    lineEdit->setFixedSize(358,34);

//    QWidget *view = new QWidget;
//    view->setObjectName("view");
//    view->setFixedSize(358,186);

    HistoryBoxWidget *view = new HistoryBoxWidget;
    view->setObjectName("view");
    view->setFixedSize(358,186);
    QPushButton *doneBtn = new QPushButton("完成");
    doneBtn->setFixedSize(100, 24);
    pLayout->addSpacing(20);
    pLayout->addWidget(lineEdit, 0, Qt::AlignCenter);
    pLayout->addSpacing(20);
    pLayout->addWidget(view, 0, Qt::AlignCenter);
    pLayout->addSpacing(22);
    pLayout->addWidget(doneBtn, 0, Qt::AlignCenter);
    pLayout->addStretch();
    widget->setLayout(pLayout);

    connect(view, &HistoryBoxWidget::stateChange, [&]{
        m_bState = true;
    });

    connect(lineEdit, &QLineEdit::textChanged, view, &HistoryBoxWidget::slotInput);
    connect(doneBtn, &QPushButton::clicked, [&]{
        gInfoStyle->signalUpdateCustomStockCache();
        emit finished();
        this->close();
        ZXStockOpt::instance()->informStkCountChanged();
    });


    connect(m_controlBox, &WindowControlBox::mainWndClose, [&]{
        if(m_bState)
        {
            ZXStockOpt::instance()->informStkCountChanged();
        }

    });

    return widget;
}
