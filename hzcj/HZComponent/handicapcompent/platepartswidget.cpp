#include "platepartswidget.h"
#include <QLabel>
#include <QFormLayout>
#include <QStyle>


PlateDetailQuoteWidget::PlateDetailQuoteWidget(QWidget *parent):
    BaseWidget(parent),
    m_openLabel(new QLabel),
    m_highestLabel(new QLabel),
    m_volumeLabel(new QLabel),
    m_preCloseLabel(new QLabel),
    m_lowestLabel(new QLabel),
    m_turnoverLabel(new QLabel),
    m_turnoverRateLabel(new QLabel),
    m_inVolumeLabel(new QLabel),
    m_outVolumeLabel(new QLabel),
    m_allGuMoneyLabel(new QLabel),
    m_allValueLabel(new QLabel),
    m_dyGuMoneyLabel(new QLabel)
{
    this->setFixedHeight(80);
    createWg();
    setObjName();
    updateText();
}

void PlateDetailQuoteWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;
    updateText();
}

void PlateDetailQuoteWidget::createWg()
{

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(12, 5, 12, 5);

    mainLayout->setSpacing(0);

    //0
    mainLayout->addWidget(new QLabel("今开："), 0, 0);
    mainLayout->addWidget(m_openLabel, 0, 1);
    mainLayout->addWidget(new QLabel("最高："), 0, 2);
    mainLayout->addWidget(m_highestLabel, 0, 3);
    mainLayout->addWidget(new QLabel("成交量："), 0, 4);
    mainLayout->addWidget(m_volumeLabel, 0, 5);
    //1
    mainLayout->addWidget(new QLabel("昨收："), 1, 0);
    mainLayout->addWidget(m_preCloseLabel, 1, 1);
    mainLayout->addWidget(new QLabel("最低："), 1, 2);
    mainLayout->addWidget(m_lowestLabel, 1, 3);
    mainLayout->addWidget(new QLabel("成交额："), 1, 4);
    mainLayout->addWidget(m_turnoverLabel, 1, 5);
    //2
    mainLayout->addWidget(new QLabel("换手："), 2, 0);
    mainLayout->addWidget(m_turnoverRateLabel, 2, 1);
    mainLayout->addWidget(new QLabel("内盘："), 2, 2);
    mainLayout->addWidget(m_inVolumeLabel, 2, 3);
    mainLayout->addWidget(new QLabel("外盘："), 2, 4);
    mainLayout->addWidget(m_outVolumeLabel, 2, 5);
    //3
    mainLayout->addWidget(new QLabel("总股本："), 3, 0);
    mainLayout->addWidget(m_allGuMoneyLabel, 3, 1);
    mainLayout->addWidget(new QLabel("总值："), 3, 2);
    mainLayout->addWidget(m_allValueLabel, 3, 3);
    mainLayout->addWidget(new QLabel("流通股："), 3, 4);
    mainLayout->addWidget(m_dyGuMoneyLabel, 3, 5);
    //4
//    mainLayout->addWidget(new QLabel("总股本"), 4, 0);
//    mainLayout->addWidget(m_allGuMoneyLabel, 4, 1);
//    mainLayout->addWidget(new QLabel("总市值"), 4, 2);
//    mainLayout->addWidget(m_allValueLabel, 4, 3);
//    mainLayout->addWidget(new QLabel("流通股本："), 4, 4);
//    mainLayout->addWidget(m_dyGuMoneyLabel, 4, 5);


//    QGridLayout *mainlayout = new QGridLayout(this);
//    mainlayout->setContentsMargins(12, 5, 12, 5);
//    mainlayout->setSpacing(5);
//    QFormLayout *form1 = new QFormLayout;
//    form1->setSpacing(2);
//    form1->setMargin(0);
//    form1->addRow(new QLabel("今开:"), m_openLabel);

//    QFormLayout *form2 = new QFormLayout;
//    form2->setSpacing(2);
//    form2->setMargin(0);
//    form2->addRow(new QLabel("最高:"), m_highestLabel);

//    QFormLayout *form3 = new QFormLayout;
//    form3->setSpacing(2);
//    form3->setMargin(0);
//    form3->addRow(new QLabel("成交量:"), m_volumeLabel);

//    QFormLayout *form4 = new QFormLayout;
//    form4->setSpacing(2);
//    form4->setMargin(0);
//    form4->addRow(new QLabel("昨收:"), m_preCloseLabel);

//    QFormLayout *form5 = new QFormLayout;
//    form5->setSpacing(2);
//    form5->setMargin(0);
//    form5->addRow(new QLabel("最低:"), m_lowestLabel);

//    QFormLayout *form6 = new QFormLayout;
//    form6->setSpacing(2);
//    form6->setMargin(0);
//    form6->addRow(new QLabel("成交额:"), m_turnoverLabel);

//    QFormLayout *form7 = new QFormLayout;
//    form7->setSpacing(2);
//    form7->setMargin(0);
//    form7->addRow(new QLabel("换手:"), m_turnoverRateLabel);

//    QFormLayout *form8 = new QFormLayout;
//    form8->setSpacing(2);
//    form8->setMargin(0);
//    form8->addRow(new QLabel("内盘:"), m_inVolumeLabel);

//    QFormLayout *form9 = new QFormLayout;
//    form9->setSpacing(2);
//    form9->setMargin(0);
//    form9->addRow(new QLabel("外盘:"), m_outVolumeLabel);

//    QFormLayout *form10 = new QFormLayout;
//    form10->setSpacing(2);
//    form10->setMargin(0);
//    form10->addRow(new QLabel("总股本:"), m_allGuMoneyLabel);


//    QFormLayout *form11 = new QFormLayout;
//    form11->setSpacing(2);
//    form11->setMargin(0);
//    form11->addRow(new QLabel("总市值:"), m_allValueLabel);

//    QFormLayout *form12 = new QFormLayout;
//    form12->setSpacing(2);
//    form12->setMargin(0);
//    form12->addRow(new QLabel("流通股本:"), m_dyGuMoneyLabel);

//    mainlayout->addLayout(form1, 0, 0);
//    mainlayout->addLayout(form2, 0, 1);
//    mainlayout->addLayout(form3, 0, 2);
//    mainlayout->addLayout(form4, 1, 0);
//    mainlayout->addLayout(form5, 1, 1);
//    mainlayout->addLayout(form6, 1, 2);
//    mainlayout->addLayout(form7, 2, 0);
//    mainlayout->addLayout(form8, 2, 1);
//    mainlayout->addLayout(form9, 2, 2);
//    mainlayout->addLayout(form10, 3, 0);
//    mainlayout->addLayout(form11, 3, 1);
//    mainlayout->addLayout(form12, 3, 2);
}

void PlateDetailQuoteWidget::updateText()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    HzQuoteFormat::QuoteItem item;
    //今开
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdOpenpx);
    updateStockField(item,m_openLabel);
    //最高
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdHighpx);
    updateStockField(item,m_highestLabel);

    //成交量
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdVolume);
    updateStockField(item,m_volumeLabel);
    //昨收
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdPrevClosepx);
    updateStockField(item,m_preCloseLabel);
    //最低
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLowpx);
    updateStockField(item,m_lowestLabel);

    //成交额
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAmount);
    updateStockField(item,m_turnoverLabel);

    //换手
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdVolCh);
    updateStockField(item,m_turnoverRateLabel);

    //内盘
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdIn);
    updateStockField(item,m_inVolumeLabel);

    //外盘
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdOut);
    updateStockField(item,m_outVolumeLabel);

    //总股本
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAllGuMoney);
    updateStockField(item,m_allGuMoneyLabel);

    //总市值
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAllValue);
    updateStockField(item,m_allValueLabel);

    //流通股本
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdDyGuMoney);
    updateStockField(item,m_dyGuMoneyLabel);

}

void PlateDetailQuoteWidget::setObjName()
{
    m_openLabel->setObjectName("numstyle");
    m_highestLabel->setObjectName("numstyle");
    m_volumeLabel->setObjectName("numstyle");
    m_preCloseLabel->setObjectName("numstyle");
    m_lowestLabel->setObjectName("numstyle");
    m_turnoverLabel->setObjectName("numstyle");
    m_turnoverRateLabel->setObjectName("numstyle");
    m_inVolumeLabel->setObjectName("numstyle");
    m_outVolumeLabel->setObjectName("numstyle");
    m_allGuMoneyLabel->setObjectName("numstyle");
    m_allValueLabel->setObjectName("numstyle");
    m_dyGuMoneyLabel->setObjectName("numstyle");
}

void PlateDetailQuoteWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc", QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}
