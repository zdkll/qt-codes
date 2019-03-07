#include "indexpartswidget.h"
#include <QLabel>
#include <QStyle>
#include <QBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include "risefalltradiowidget.h"

IndexQuoteWidget::IndexQuoteWidget(QWidget *parent):
    BaseWidget(parent),
    m_volumeLabel(new QLabel),
    m_highestLabel(new QLabel),
    m_openLabel(new QLabel),
    m_turnoverLabel(new QLabel),
    m_lowestLabel(new QLabel),
    m_preCloseLabel(new QLabel)
{
    this->setFixedHeight(50);
    createWg();
    setObjName();
    updateText();
}

void IndexQuoteWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;
    updateText();
}

void IndexQuoteWidget::createWg()
{
    QGridLayout* mainLayout = new QGridLayout(this);

    mainLayout->setContentsMargins(12, 6, 12, 6);

    QFormLayout *formLayout1 = new QFormLayout;
    formLayout1->setSpacing(2);
    formLayout1->setMargin(0);
    formLayout1->addRow(new QLabel("成交量:"), m_volumeLabel);
//    mainLayout->setAlignment(formLayout1, Qt::AlignCenter);
    QFormLayout *formLayout2 = new QFormLayout;
    formLayout2->setSpacing(2);
    formLayout2->setMargin(0);
    formLayout2->addRow(new QLabel("最高:"), m_highestLabel);

    QFormLayout *formLayout3 = new QFormLayout;
    formLayout3->setSpacing(2);
    formLayout3->setMargin(0);
    formLayout3->addRow(new QLabel("开盘:"), m_openLabel);

    QFormLayout *formLayout4 = new QFormLayout;
    formLayout4->setSpacing(2);
    formLayout4->setMargin(0);
    formLayout4->addRow(new QLabel("成交额:"), m_turnoverLabel);

    QFormLayout *formLayout5 = new QFormLayout;
    formLayout5->setSpacing(2);
    formLayout5->setMargin(0);
    formLayout5->addRow(new QLabel("最低:"), m_lowestLabel);

    QFormLayout *formLayout6 = new QFormLayout;
    formLayout6->setSpacing(2);
    formLayout6->setMargin(0);
    formLayout6->addRow(new QLabel("昨收:"), m_preCloseLabel);

    mainLayout->addLayout(formLayout1, 0, 0);
    mainLayout->addLayout(formLayout2, 0, 1);
    mainLayout->addLayout(formLayout3, 0, 2);
    mainLayout->addLayout(formLayout4, 1, 0);
    mainLayout->addLayout(formLayout5, 1, 1);
    mainLayout->addLayout(formLayout6, 1, 2);
}

void IndexQuoteWidget::updateText()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    HzQuoteFormat::QuoteItem item;
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdOpenpx);//今开
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
}

void IndexQuoteWidget::setObjName()
{
    m_openLabel->setObjectName("numstyle"); //今开
    m_highestLabel->setObjectName("numstyle"); //最高
    m_volumeLabel->setObjectName("numstyle");   //成交量
    m_preCloseLabel->setObjectName("numstyle"); //昨收
    m_lowestLabel->setObjectName("numstyle");   //最低
    m_turnoverLabel->setObjectName("numstyle"); //成交额
}

void IndexQuoteWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *label)
{
    label->setText(item.strValue);
    label->setProperty("QuotePanelDirc",QString::number(item.dirc));
    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}

IndexRiseFallWidget::IndexRiseFallWidget(QWidget *parent):
    BaseWidget(parent),
    m_pRiseLabel(new QLabel),
    m_pFallLabel(new QLabel),
    m_pFlatLabel(new QLabel),
    m_pTradioWidget(new RiseFallTradioWidget)
{
    this->setFixedHeight(65);
    createWg();
    setObjName();
    updateText();
}

void IndexRiseFallWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;
    updateText();
}

void IndexRiseFallWidget::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->setSpacing(0);
    layout1->setMargin(0);

    QFormLayout *formLayout1 = new QFormLayout;
    formLayout1->setMargin(0);
    formLayout1->setSpacing(0);
    formLayout1->addRow(new QLabel("上涨: "), m_pRiseLabel);

    QFormLayout *formLayout2 = new QFormLayout;
    formLayout2->setMargin(0);
    formLayout2->setSpacing(0);
    formLayout2->addRow(new QLabel("下跌: "), m_pFallLabel);

    QFormLayout *formLayout3 = new QFormLayout;
    formLayout3->setMargin(0);
    formLayout3->setSpacing(0);
    formLayout3->addRow(new QLabel("平家: "), m_pFlatLabel);

    layout1->addSpacing(12);
    layout1->addLayout(formLayout1, 1);
    layout1->addLayout(formLayout2, 1);
    layout1->addLayout(formLayout3, 1);
    layout1->addSpacing(12);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->setSpacing(0);
    layout2->setMargin(12);

    layout2->addWidget(m_pTradioWidget);

    pMainLayout->addSpacing(12);
    pMainLayout->addLayout(layout1);
    pMainLayout->addLayout(layout2);
    pMainLayout->addStretch();
}

void IndexRiseFallWidget::updateText()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    INT64 riseFall[3];
    riseFall[0] = stock.rise;
    riseFall[1] = stock.flat;
    riseFall[2] = stock.fall;

    m_pTradioWidget->updateData(riseFall);

    HzQuoteFormat::QuoteItem item;

    item = HzQuoteFormat::QuoteFormat::formatIntData(stock.rise);
    m_pRiseLabel->setText(item.strValue);
//    updateStockField(item, m_pRiseLabel);

    item = HzQuoteFormat::QuoteFormat::formatIntData(stock.fall);
    m_pFallLabel->setText(item.strValue);
//    updateStockField(item, m_pFallLabel);

    item = HzQuoteFormat::QuoteFormat::formatIntData(stock.flat);
    m_pFlatLabel->setText(item.strValue);
//    updateStockField(item, m_pFlatLabel);
}

void IndexRiseFallWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *label)
{
    label->setText(item.strValue);
    label->setProperty("QuotePanelDirc", QString::number(item.dirc));
    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}

void IndexRiseFallWidget::setObjName()
{
    m_pRiseLabel->setObjectName("rise");
    m_pFallLabel->setObjectName("fall");
    m_pFlatLabel->setObjectName("flat");
}

IndexDetailQuoteWidget::IndexDetailQuoteWidget(QWidget *parent):
    BaseWidget(parent),
    m_openLabel(new QLabel),
    m_highestLabel(new QLabel),
    m_upLimitLabel(new QLabel),
    m_preCloseLabel(new QLabel),
    m_lowestLabel(new QLabel),
    m_downLimitLabel(new QLabel),
    m_volumeLabel(new QLabel),
    m_turnoverLabel(new QLabel)
{
    this->setFixedHeight(70);
    createWg();
    setObjName();
    updateText();
}

void IndexDetailQuoteWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;
    updateText();
}

void IndexDetailQuoteWidget::createWg()
{
    QGridLayout *mainlayout = new QGridLayout(this);
    mainlayout->setMargin(12);
    mainlayout->setSpacing(0);
    mainlayout->setVerticalSpacing(5);
    QFormLayout *form1 = new QFormLayout;
    form1->setSpacing(5);
    form1->setMargin(0);
    form1->addRow(new QLabel("今开:"), m_openLabel);

    QFormLayout *form2 = new QFormLayout;
    form2->setSpacing(5);
    form2->setMargin(0);
    form2->addRow(new QLabel("最高:"), m_highestLabel);

    QFormLayout *form3 = new QFormLayout;
    form3->setSpacing(5);
    form3->setMargin(0);
    form3->addRow(new QLabel("涨停:"), m_upLimitLabel);

    QFormLayout *form4 = new QFormLayout;
    form4->setSpacing(5);
    form4->setMargin(0);
    form4->addRow(new QLabel("昨收:"), m_preCloseLabel);

    QFormLayout *form5 = new QFormLayout;
    form5->setSpacing(5);
    form5->setMargin(0);
    form5->addRow(new QLabel("最低:"), m_lowestLabel);

    QFormLayout *form6 = new QFormLayout;
    form6->setSpacing(5);
    form6->setMargin(0);
    form6->addRow(new QLabel("跌停:"), m_downLimitLabel);

    QFormLayout *form7 = new QFormLayout;
    form7->setSpacing(5);
    form7->setMargin(0);
    form7->addRow(new QLabel("总手:"), m_volumeLabel);

    QFormLayout *form8 = new QFormLayout;
    form8->setSpacing(5);
    form8->setMargin(0);
    form8->addRow(new QLabel("成交额:"), m_turnoverLabel);


    mainlayout->addLayout(form1, 0, 0);
    mainlayout->addLayout(form2, 0, 1);
    mainlayout->addLayout(form3, 0, 2);
    mainlayout->addLayout(form4, 1, 0);
    mainlayout->addLayout(form5, 1, 1);
    mainlayout->addLayout(form6, 1, 2);
    mainlayout->addLayout(form7, 2, 0);
    mainlayout->addLayout(form8, 2, 1);
}

void IndexDetailQuoteWidget::updateText()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    HzQuoteFormat::QuoteItem item;
    //今开
    item = HzQuoteFormat::QuoteFormat::format(stock, FieldIdOpenpx);
    updateStockField(item, m_openLabel);

    //最高
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdHighpx);
    updateStockField(item,m_highestLabel);

    //涨停
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLimitUp);
    updateStockField(item,m_upLimitLabel);

    //昨收
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdPrevClosepx);
    updateStockField(item,m_preCloseLabel);

    //最低
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLowpx);
    updateStockField(item,m_lowestLabel);

    //跌停
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLimitDown);
    updateStockField(item,m_downLimitLabel);

    //总手
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAllVol);
    updateStockField(item,m_volumeLabel);


    //成交额
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAmount);
    updateStockField(item,m_turnoverLabel);
}

void IndexDetailQuoteWidget::setObjName()
{
    m_openLabel->setObjectName("numstyle");
    m_highestLabel->setObjectName("numstyle");
    m_upLimitLabel->setObjectName("numstyle");
    m_preCloseLabel->setObjectName("numstyle");
    m_lowestLabel->setObjectName("numstyle");
    m_downLimitLabel->setObjectName("numstyle");
    m_volumeLabel->setObjectName("numstyle");
    m_turnoverLabel->setObjectName("numstyle");
}

void IndexDetailQuoteWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *label)
{
    label->setText(item.strValue);
    label->setProperty("QuotePanelDirc", QString::number(item.dirc));
    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}
