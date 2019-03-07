#include "stockwarndialog.h"
#include <QBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>
#include <QDoubleValidator>
#include <QDebug>

StockWarnDialog::StockWarnDialog(QWidget *parent):
    BaseDialog(parent),
    m_pHelper(new FramelessHelper(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    createWg();
}

void StockWarnDialog::updateData(const QVector<HZData::AllStock> &vec)
{
    m_pTopWidget->updateData(vec);
    m_pBottomWidget->updateData(vec);
}

void StockWarnDialog::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);
    pMainLayout->addWidget(createTitle());
    pMainLayout->addWidget(createBody());
}

QWidget *StockWarnDialog::createTitle()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("title");
    QLabel *label = new QLabel("自选股提醒");
    m_controlBox = new WindowControlBox(this, this, false, false);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    pLayout->addStretch();
    pLayout->addWidget(label, 0, Qt::AlignCenter);
    pLayout->addStretch();
    pLayout->addWidget(m_controlBox, 0, Qt::AlignRight);

    widget->setLayout(pLayout);

    return widget;
}
#include "hzline.h"
QWidget *StockWarnDialog::createBody()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("body");
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);

    m_pTopWidget = new TopWidget;
    pMainLayout->addWidget(m_pTopWidget);

    pMainLayout->addWidget(new HZLine(Qt::Horizontal, this), 0, Qt::AlignCenter);

    m_pCenterWidget = new CenterWidget;
    pMainLayout->addWidget(m_pCenterWidget);

    pMainLayout->addWidget(new HZLine(Qt::Horizontal, this), 0, Qt::AlignCenter);

    m_pBottomWidget = new BottomWidget;
    pMainLayout->addWidget(m_pBottomWidget);

    m_pButtonWidget = new ButtonWidget;
    pMainLayout->addWidget(m_pButtonWidget);

    buildConnect();

    widget->setLayout(pMainLayout);
    return widget;
}

void StockWarnDialog::buildConnect()
{
    connect(m_pButtonWidget, &ButtonWidget::click, [&]
    {
        this->close();
    });

    connect(m_pCenterWidget, &CenterWidget::clicked, [&](bool checked){
        qDebug() << checked;
    });

    connect(m_pBottomWidget, &BottomWidget::upTextChanged,[&](const QString &strText){
        qDebug() << strText;
    });

    connect(m_pBottomWidget, &BottomWidget::downTextChanged,[&](const QString &strText){
        qDebug() << strText;
    });

    connect(m_pBottomWidget, &BottomWidget::rateTextChanged,[&](const QString &strText){
        qDebug() << strText;
    });
}


TopWidget::TopWidget(QWidget *parent):
    BaseWidget(parent)
{
    createWg();
    setObjName();
    updateText();
}

void TopWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;
    updateText();
}

void TopWidget::createWg()
{

    m_nameLabel = new QLabel("陕西煤业");

    m_codeLabel = new QLabel(" (600225.SH)");

    QHBoxLayout *ptopLayout = new QHBoxLayout;
    ptopLayout->setSpacing(0);
    ptopLayout->setMargin(0);
    ptopLayout->addWidget(m_nameLabel);
    ptopLayout->addWidget(m_codeLabel);
    ptopLayout->addStretch();

    QFormLayout *pFormLayout = new QFormLayout;

    pFormLayout->setSpacing(5);
    pFormLayout->setMargin(0);
    m_priceLabel = new QLabel("8.04");

    m_pArrowLabel = new QLabel;

    pFormLayout->addRow(m_priceLabel, m_pArrowLabel);
    pFormLayout->setAlignment(m_pArrowLabel, Qt::AlignVCenter);
    m_rateLabel = new QLabel("0.03 0.37%");

    QHBoxLayout *playout = new QHBoxLayout;
    playout->setSpacing(7);
    playout->setMargin(0);
    playout->addLayout(pFormLayout);
    playout->addWidget(m_rateLabel);
    playout->addStretch();
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(21, 18, 0, 0);
    pMainLayout->setSpacing(9);
    pMainLayout->addLayout(ptopLayout);
    pMainLayout->addLayout(playout);
    pMainLayout->addStretch();
}

void TopWidget::updateText()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    HzQuoteFormat::QuoteItem item;
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdStockName);
    updateStockField(item,m_nameLabel);

    QString strcode = QString::fromStdString(stock.stock_code);
    QString str = strcode.left(2).toUpper();
    strcode.remove(0,2);
    strcode = " (" + strcode + "." + str + ")";
    m_codeLabel->setText(strcode);

    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLastpx);
    updateStockField(item,m_priceLabel);

    if(item.dirc == HzQuoteFormat::quote_up)
    {
        m_pArrowLabel->setProperty("state", "rise");
    }
    else if(item.dirc == HzQuoteFormat::quote_down)
    {
        m_pArrowLabel->setProperty("state", "fall");
    }
    else
    {
        m_pArrowLabel->setProperty("state", "unknown");
    }

    m_pArrowLabel->style()->unpolish(m_pArrowLabel);
    m_pArrowLabel->style()->polish(m_pArrowLabel);

    item = HzQuoteFormat::QuoteFormat::formatChange(stock.up_down_price,DECIMAL_POINT);

    QString strChange = item.strValue;

    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdChangeRate);

    item.strValue = strChange + " " + item.strValue;

    updateStockField(item,m_rateLabel);
}

void TopWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}

void TopWidget::setObjName()
{
    m_nameLabel->setObjectName("name");
    m_codeLabel->setObjectName("code");
    m_priceLabel->setObjectName("price");
    m_pArrowLabel->setObjectName("arrow");
    m_rateLabel->setObjectName("rate");
}

CenterWidget::CenterWidget(QWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void CenterWidget::createWg()
{

    QLabel *lab1 = new QLabel("资讯提醒");
    QCheckBox *pCheck = new QCheckBox;
    connect(pCheck, &QCheckBox::clicked,[&](bool checked){
        emit clicked(checked);
    });
    QLabel *lab2 = new QLabel("最新公司公告、研究报告");
    QHBoxLayout *playout = new QHBoxLayout;
    playout->setMargin(0);
    playout->setSpacing(6);
    playout->addWidget(pCheck);
    playout->addWidget(lab2);
    playout->addStretch();

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(11);
    pMainLayout->setContentsMargins(21, 11, 0, 0);

    pMainLayout->addWidget(lab1);
    pMainLayout->addLayout(playout);
    pMainLayout->addStretch();
}

BottomWidget::BottomWidget(QWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void BottomWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;

    setEditRange();
}

void BottomWidget::onUpTextEdited(const QString &strText)
{
    HZData::AllStock stock;
    if(!m_stockVec.empty())
        stock = m_stockVec[0];

    float num = strText.toFloat();

    if(num  < stock.last_price  || strText.isEmpty())
    {
        m_pUpLabel->setText(" --");
        if(m_pUpCheck->isChecked())
            emit upTextChanged(Default_Item_String);
        return;
    }

    float tmp = (num - stock.last_price) / stock.last_price;

    QString strtmp = " " + QString::number(tmp*100, 'f', 2) + "%";
    m_pUpLabel->setText(strtmp);

    if(m_pUpCheck->isChecked())
        emit upTextChanged(strText);
}

void BottomWidget::onDownTextEdited(const QString &strText)
{
    HZData::AllStock stock;
    if(!m_stockVec.empty())
        stock = m_stockVec[0];

    float num = strText.toFloat();

    if(num  > stock.last_price || strText.isEmpty())
    {
        m_pDownLabel->setText(" --");
        if(m_pDownCheck->isChecked())
            emit downTextChanged(Default_Item_String);
        return;
    }

    float tmp = (num - stock.last_price) / stock.last_price;

    QString strtmp = " " + QString::number(tmp*100, 'f', 2) + "%";
    m_pDownLabel->setText(strtmp);

    if(m_pDownCheck->isChecked())
        emit downTextChanged(strText);
}

void BottomWidget::onRateTextEdited(const QString &strText)
{
    float tmp = strText.toFloat();

    if(tmp > 10 || tmp < -10)
    {
        m_rateLineEdit->clear();
        if(m_pRateCheck->isChecked())
            emit rateTextChanged(Default_Item_String);
        return;
    }
    if(m_pRateCheck->isChecked())
        emit rateTextChanged(strText);
}

void BottomWidget::createWg()
{
    m_upValidator = new QDoubleValidator(this);
    m_downValidator = new QDoubleValidator(this);

    m_upValidator->setDecimals(2);
    m_downValidator->setDecimals(2);

    QLabel *pTitle = new QLabel("股价预警");
    m_pUpCheck = new QCheckBox;
    QLabel *pText1 = new QLabel("股价突破");
    m_upLineEdit = new QLineEdit;

    QLabel *pUnit = new QLabel("元");
    QLabel *pText2 = new QLabel("比现价涨幅");
    pText2->setObjectName("textrate");


    m_pUpLabel = new QLabel(" --");
    m_pUpLabel->setObjectName("up");

    QHBoxLayout *playout1 = new QHBoxLayout;
    playout1->setMargin(0);
    playout1->setSpacing(0);

    playout1->addWidget(m_pUpCheck);
    playout1->addSpacing(6);
    playout1->addWidget(pText1);
    playout1->addSpacing(6);
    playout1->addWidget(m_upLineEdit);
    playout1->addSpacing(6);
    playout1->addWidget(pUnit);
    playout1->addSpacing(13);
    playout1->addWidget(pText2);
    playout1->addWidget(m_pUpLabel);
    playout1->addStretch();

    m_pDownCheck = new QCheckBox;
    QLabel *pText3 = new QLabel("股价跌破");
    m_downLineEdit = new QLineEdit;

    QLabel *pUnit2 = new QLabel("元");
    QLabel *pText4 = new QLabel("比现价涨幅");
    pText4->setObjectName("textrate");

    m_pDownLabel = new QLabel(" --");
    m_pDownLabel->setObjectName("down");

    QHBoxLayout *playout2 = new QHBoxLayout;
    playout2->setMargin(0);
    playout2->setSpacing(0);

    playout2->addWidget(m_pDownCheck);
    playout2->addSpacing(6);
    playout2->addWidget(pText3);
    playout2->addSpacing(6);
    playout2->addWidget(m_downLineEdit);
    playout2->addSpacing(6);
    playout2->addWidget(pUnit2);
    playout2->addSpacing(13);
    playout2->addWidget(pText4);
    playout2->addWidget(m_pDownLabel);
    playout2->addStretch();


    m_pRateCheck = new QCheckBox;
    QLabel *pText5 = new QLabel("日涨幅");
    m_rateLineEdit = new QLineEdit;

    QLabel *pUnit3 = new QLabel("%");


    QHBoxLayout *playout3 = new QHBoxLayout;
    playout3->setMargin(0);
    playout3->setSpacing(0);

    playout3->addWidget(m_pRateCheck);
    playout3->addSpacing(6);
    playout3->addWidget(pText5);
    playout3->addSpacing(20);
    playout3->addWidget(m_rateLineEdit);
    playout3->addSpacing(6);
    playout3->addWidget(pUnit3);
    playout3->addStretch();

    QDoubleValidator *validator = new QDoubleValidator(-10, 10, 2, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    m_rateLineEdit->setValidator(validator);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(6);
    pMainLayout->setContentsMargins(21, 14, 0, 0);

    pMainLayout->addWidget(pTitle);
    pMainLayout->addLayout(playout1);
    pMainLayout->addLayout(playout2);
    pMainLayout->addLayout(playout3);
    pMainLayout->addStretch();


    connect(m_upLineEdit, &QLineEdit::textEdited, this, &BottomWidget::onUpTextEdited);
    connect(m_downLineEdit, &QLineEdit::textEdited, this, &BottomWidget::onDownTextEdited);
    connect(m_rateLineEdit, &QLineEdit::textEdited, this, &BottomWidget::onRateTextEdited);
}

void BottomWidget::setEditRange()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    m_upValidator->setBottom(stock.last_price);
    m_downValidator->setRange(0, stock.last_price, 2);

    m_upValidator->setRange(10, 20, 2);

    m_upLineEdit->setValidator(m_upValidator);
    m_downLineEdit->setValidator(m_downValidator);
}

ButtonWidget::ButtonWidget(QWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void ButtonWidget::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 22, 0, 0);
    pMainLayout->setSpacing(0);

    QPushButton *btn = new QPushButton("提交");

    connect(btn, &QPushButton::clicked, [&]{
        emit click();
    });

    pMainLayout->addWidget(btn, 0, Qt::AlignCenter);

    pMainLayout->addStretch();
}
