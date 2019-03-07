#include "stockinfowidget.h"
#include <QBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QMessageBox>
#include "hzuserdatamgr.h"
#include "hzauthenopt.h"
#include "localstocksource.h"
#include "zxstockopt.h"


HandicapTitleWidget::HandicapTitleWidget(QWidget *parent):
    BaseWidget(parent),
    m_nameLabel(new QLabel),
    m_codeLabel(new QLabel),
    m_priceLabel(new QLabel),
    m_pArrowLabel(new QLabel),
    m_rateLabel(new QLabel),
    m_pAddBtn(new QPushButton)/*,
      m_pWarnBtn(new QPushButton),
      m_pSetBtn(new QPushButton)*/
{
    this->setFixedHeight(76);
    createWg();
    setObjName();
    updateText();

    connect(m_pAddBtn, &QPushButton::clicked, this, &HandicapTitleWidget::onAddBtnClicked);
    //    connect(m_pWarnBtn, &QPushButton::clicked, this, &HandicapTitleWidget::onWarningBtnClicked);

    connect(ZXStockOpt::instance(), &ZXStockOpt::stockCountChanged, [&](const int count){
        if(this->isVisible())
        {
            if(0 == count)
            {
                setSelfStock(m_stockCode);
            }
        }
    });

}

void HandicapTitleWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;

    updateText();
    m_warndlg.updateData(vec);
}

void HandicapTitleWidget::setSelfStock(const QString &strCode)
{
    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();

    if(zxStockOpt->isHas(strCode))
    {
        m_pAddBtn->setProperty("selfstock", true);
    }
    else
    {
        m_pAddBtn->setProperty("selfstock", false);
    }

    m_pAddBtn->style()->unpolish(m_pAddBtn);
    m_pAddBtn->style()->polish(m_pAddBtn);
    m_pAddBtn->update();
}

void HandicapTitleWidget::OnStockCodeChange()
{
    setSelfStock(m_stockCode);
}

void HandicapTitleWidget::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);

    QHBoxLayout *pTopLayout = new QHBoxLayout;
    pTopLayout->setMargin(0);
    pTopLayout->setSpacing(0);

    pTopLayout->addSpacing(14);
    pTopLayout->addWidget(m_nameLabel);
    pTopLayout->addSpacing(6);
    pTopLayout->addWidget(m_codeLabel);
    pTopLayout->addSpacing(8);
    pTopLayout->addWidget(m_pAddBtn);
    pTopLayout->addStretch();
    //    pTopLayout->addWidget(m_pWarnBtn);
    //    pTopLayout->addSpacing(2);
    //    pTopLayout->addWidget(m_pSetBtn);
    pTopLayout->addSpacing(10);

    QHBoxLayout *pBottomLayout = new QHBoxLayout;
    pBottomLayout->setMargin(0);
    pBottomLayout->setSpacing(0);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setMargin(0);
    formLayout->setSpacing(6);


    formLayout->addRow(m_priceLabel, m_pArrowLabel);
    formLayout->setAlignment(m_pArrowLabel, Qt::AlignVCenter);

    pBottomLayout->addSpacing(14);
    pBottomLayout->addLayout(formLayout);
    pBottomLayout->addSpacing(11);
    pBottomLayout->addWidget(m_rateLabel);
    pBottomLayout->addStretch();
    pMainLayout->addSpacing(12);
    pMainLayout->addLayout(pTopLayout);
//    pMainLayout->addSpacing(10);
    pMainLayout->addLayout(pBottomLayout);
    pMainLayout->addStretch();
}

void HandicapTitleWidget::updateText()
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
    strcode = strcode + "." + str;
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
    m_pArrowLabel->update();

    item = HzQuoteFormat::QuoteFormat::formatChange(stock.up_down_price,DECIMAL_POINT);

    QString strChange = item.strValue;

    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdChangeRate);

    item.strValue = strChange + "\n" + item.strValue;

    updateStockField(item,m_rateLabel);
}

void HandicapTitleWidget::setObjName()
{
    m_nameLabel->setObjectName("name");
    m_codeLabel->setObjectName("code");
    m_priceLabel->setObjectName("price");
    m_rateLabel->setObjectName("rate");
    m_pArrowLabel->setObjectName("arrow");
    m_pAddBtn->setObjectName("add");
    //    m_pWarnBtn->setObjectName("warn");
    //    m_pSetBtn->setObjectName("set");

//    QFont font = m_codeLabel->font();
//    font.setStyleStrategy(QFont::NoAntialias);
//    m_codeLabel->setFont(font);

}

void HandicapTitleWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}

void HandicapTitleWidget::onAddBtnClicked()
{
    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();



    if(zxStockOpt->isHas(m_stockCode))
    {
        zxStockOpt->delZXStock(m_stockCode);
        m_pAddBtn->setProperty("selfstock", false);
    }
    else
    {
        if(zxStockOpt->getZXStock().size() == 100)
        {
             QMessageBox::information(this, "提示", "目前只可添加100个自选股", QMessageBox::Ok);
             return;
        }

        zxStockOpt->addZXStock(m_stockCode);
        m_pAddBtn->setProperty("selfstock", true);
    }

    m_pAddBtn->setProperty("selfstock",zxStockOpt->isHas(m_stockCode));


    m_pAddBtn->style()->unpolish(m_pAddBtn);
    m_pAddBtn->style()->polish(m_pAddBtn);
    m_pAddBtn->update();
    return;
}

void HandicapTitleWidget::onWarningBtnClicked()
{
    m_warndlg.exec();
}

HandicapCentralWidget::HandicapCentralWidget(QWidget *parent):
    BaseWidget(parent),
    m_openLabel(new QLabel),
    m_highestLabel(new QLabel),
    m_volumeLabel(new QLabel),
    m_preCloseLabel(new QLabel),
    m_lowestLabel(new QLabel),
    m_turnoverLabel(new QLabel),
    m_upLimitLabel(new QLabel),
    m_avePriceLabel(new QLabel),
    m_outVolumeLabel(new QLabel),
    m_downLimitLabel(new QLabel),
    m_volumeRatioLabel(new QLabel),
    m_inVolumeLabel(new QLabel),
    m_amplitudeLabel(new QLabel),
    m_turnoverRateLabel(new QLabel)
{
    this->setFixedHeight(102);
    createWg();
    setObjName();
    updateText();
}

void HandicapCentralWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;

    updateText();
}

void HandicapCentralWidget::createWg()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->setContentsMargins(14, 9, 5, 5);
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
    mainLayout->addWidget(new QLabel("涨停："), 2, 0);
    mainLayout->addWidget(m_upLimitLabel, 2, 1);
    mainLayout->addWidget(new QLabel("均价："), 2, 2);
    mainLayout->addWidget(m_avePriceLabel, 2, 3);
    mainLayout->addWidget(new QLabel("外  盘："), 2, 4);
    mainLayout->addWidget(m_outVolumeLabel, 2, 5);
    //3
    mainLayout->addWidget(new QLabel("跌停："), 3, 0);
    mainLayout->addWidget(m_downLimitLabel, 3, 1);
    mainLayout->addWidget(new QLabel("量比："), 3, 2);
    mainLayout->addWidget(m_volumeRatioLabel, 3, 3);
    mainLayout->addWidget(new QLabel("内  盘："), 3, 4);
    mainLayout->addWidget(m_inVolumeLabel, 3, 5);
    //4
    mainLayout->addWidget(new QLabel("振幅："), 4, 0);
    mainLayout->addWidget(m_amplitudeLabel, 4, 1);
    mainLayout->addWidget(new QLabel("换手："), 4, 2);
    mainLayout->addWidget(m_turnoverRateLabel, 4, 3);
}

void HandicapCentralWidget::updateText()
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
    item.strValue += "手";
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
    //涨停
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLimitUp);
    updateStockField(item,m_upLimitLabel);
    //均价
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAveragepx);
    updateStockField(item,m_avePriceLabel);
    //外盘
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdOut);
    updateStockField(item,m_outVolumeLabel);
    //跌停
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdLimitDown);
    updateStockField(item,m_downLimitLabel);
    //量比
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdVolRate);
    updateStockField(item,m_volumeRatioLabel);
    //内盘
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdIn);
    updateStockField(item,m_inVolumeLabel);
    //振幅
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAmplitude);
    updateStockField(item,m_amplitudeLabel);
    //换手
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdVolCh);
    updateStockField(item,m_turnoverRateLabel);

}

void HandicapCentralWidget::setObjName()
{
    m_openLabel->setObjectName("numstyle"); //今开
    m_highestLabel->setObjectName("numstyle"); //最高
    m_volumeLabel->setObjectName("numstyle");   //成交量
    m_preCloseLabel->setObjectName("numstyle"); //昨收
    m_lowestLabel->setObjectName("numstyle");   //最低
    m_turnoverLabel->setObjectName("numstyle"); //成交额
    m_upLimitLabel->setObjectName("numstyle");  //涨停
    m_avePriceLabel->setObjectName("numstyle"); //均价
    m_outVolumeLabel->setObjectName("numstyle");    //外盘
    m_downLimitLabel->setObjectName("numstyle");    //跌停
    m_volumeRatioLabel->setObjectName("numstyle");  //量比
    m_inVolumeLabel->setObjectName("numstyle"); //内盘
    m_amplitudeLabel->setObjectName("numstyle");    //振幅
    m_turnoverRateLabel->setObjectName("numstyle"); //换手

    QFont font = m_openLabel->font();
    font.setStyleStrategy(QFont::NoAntialias);
    m_openLabel->setFont(font);

    m_openLabel->setFont(font);
    m_highestLabel->setFont(font);
    m_volumeLabel->setFont(font);
    m_preCloseLabel->setFont(font);
    m_lowestLabel->setFont(font);
    m_turnoverLabel->setFont(font);
    m_upLimitLabel->setFont(font);
    m_avePriceLabel->setFont(font);
    m_outVolumeLabel->setFont(font);
    m_downLimitLabel->setFont(font);
    m_volumeRatioLabel->setFont(font);
    m_inVolumeLabel->setFont(font);
    m_amplitudeLabel->setFont(font);
    m_turnoverRateLabel->setFont(font);
}

void HandicapCentralWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}


HandicapBottomWidget::HandicapBottomWidget(QWidget *parent):
    BaseWidget(parent),
    m_incomeLabel(new QLabel),
    m_proDyLabel(new QLabel),
    m_netAssetsLabel(new QLabel),
    m_dyGuMoneyLabel(new QLabel),
    m_dyValueLabel(new QLabel),
    m_valueRateLabel(new QLabel),
    m_allGuMoneyLabel(new QLabel),
    m_allValueLabel(new QLabel),
    m_roeLabel(new QLabel)
{
    this->setFixedHeight(66);
    createWg();
    setObjName();
    updateText();
}

void HandicapBottomWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;
    updateText();
}

void HandicapBottomWidget::createWg()
{
    //    QGridLayout *mainLayout = new QGridLayout(this);
    //    mainLayout->setContentsMargins(14, 9, 5, 5);
    //    mainLayout->setSpacing(4);
    //    //0
    //    mainLayout->addWidget(new QLabel("每股收益"), 0, 0);
    //    mainLayout->addWidget(m_incomeLabel, 0, 1);
    //    mainLayout->addWidget(new QLabel("市盈率"), 0, 2);
    //    mainLayout->addWidget(m_proDyLabel, 0, 3);
    //    mainLayout->addWidget(new QLabel("每股净资产"), 0, 4);
    //    mainLayout->addWidget(m_netAssetsLabel, 0, 5);
    //    //1
    //    mainLayout->addWidget(new QLabel("流通股本"), 1, 0);
    //    mainLayout->addWidget(m_dyGuMoneyLabel, 1, 1);
    //    mainLayout->addWidget(new QLabel("流通市值"), 1, 2);
    //    mainLayout->addWidget(m_dyValueLabel, 1, 3);
    //    mainLayout->addWidget(new QLabel("市净率"), 1, 4);
    //    mainLayout->addWidget(m_valueRateLabel, 1, 5);
    //    //2
    //    mainLayout->addWidget(new QLabel("总股本"), 2, 0);
    //    mainLayout->addWidget(m_allGuMoneyLabel, 2, 1);
    //    mainLayout->addWidget(new QLabel("总市值"), 2, 2);
    //    mainLayout->addWidget(m_allValueLabel, 2, 3);
    //    mainLayout->addWidget(new QLabel("ROE"), 2, 4);
    //    mainLayout->addWidget(m_roeLabel, 2, 5);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QHBoxLayout *pFirLayout = new QHBoxLayout;
    pFirLayout->setMargin(0);
    pFirLayout->setSpacing(2);

    QFormLayout *pFirForm0 = new QFormLayout;
    pFirForm0->setMargin(0);
    pFirForm0->setSpacing(4);
    pFirForm0->addRow(new QLabel("每股收益"), m_incomeLabel);

    QFormLayout *pFirForm1 = new QFormLayout;
    pFirForm1->setMargin(0);
    pFirForm1->setSpacing(4);
    pFirForm1->addRow(new QLabel("市盈率"), m_proDyLabel);

    QFormLayout *pFirForm2 = new QFormLayout;
    pFirForm2->setMargin(0);
    pFirForm2->setSpacing(4);
    pFirForm2->addRow(new QLabel("每股净资产"), m_netAssetsLabel);

    pFirLayout->addSpacing(14);
    pFirLayout->addLayout(pFirForm0);
    pFirLayout->addLayout(pFirForm1);
    pFirLayout->addLayout(pFirForm2);

    QHBoxLayout *pSecLayout = new QHBoxLayout;
    pSecLayout->setMargin(0);
    pSecLayout->setSpacing(2);

    QFormLayout *pSecForm0 = new QFormLayout;
    pSecForm0->setMargin(0);
    pSecForm0->setSpacing(4);
    pSecForm0->addRow(new QLabel("流通股本"), m_dyGuMoneyLabel);

    QFormLayout *pSecForm1 = new QFormLayout;
    pSecForm1->setMargin(0);
    pSecForm1->setSpacing(4);
    pSecForm1->addRow(new QLabel("流通市值"), m_dyValueLabel);

    QFormLayout *pSecForm2 = new QFormLayout;
    pSecForm2->setMargin(0);
    pSecForm2->setSpacing(4);
    pSecForm2->addRow(new QLabel("市净率"), m_valueRateLabel);

    pSecLayout->addSpacing(14);
    pSecLayout->addLayout(pSecForm0);
    pSecLayout->addLayout(pSecForm1);
    pSecLayout->addLayout(pSecForm2);

    QHBoxLayout *pThrLayout = new QHBoxLayout;
    pThrLayout->setMargin(0);
    pThrLayout->setSpacing(2);

    QFormLayout *pThrForm0 = new QFormLayout;
    pThrForm0->setMargin(0);
    pThrForm0->setSpacing(4);
    pThrForm0->addRow(new QLabel("总股本"), m_allGuMoneyLabel);

    QFormLayout *pThrForm1 = new QFormLayout;
    pThrForm1->setMargin(0);
    pThrForm1->setSpacing(4);
    pThrForm1->addRow(new QLabel("总市值"), m_allValueLabel);

    QFormLayout *pThrForm2 = new QFormLayout;
    pThrForm2->setMargin(0);
    pThrForm2->setSpacing(4);
    pThrForm2->addRow(new QLabel("ROE"), m_roeLabel);

    pThrLayout->addSpacing(14);
    pThrLayout->addLayout(pThrForm0);
    pThrLayout->addLayout(pThrForm1);
    pThrLayout->addLayout(pThrForm2);

    mainLayout->addSpacing(5);
    mainLayout->addLayout(pFirLayout);
    mainLayout->addLayout(pSecLayout);
    mainLayout->addLayout(pThrLayout);

}

void HandicapBottomWidget::updateText()
{
    HZData::AllStock stock;
    if(!m_stockVec.isEmpty())
    {
        stock = m_stockVec[0];
    }

    HzQuoteFormat::QuoteItem item;

    //每股收益
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdIncome);
    updateStockField(item,m_incomeLabel);
    //市盈率
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdValueProDy);
    updateStockField(item,m_proDyLabel);
    //每股净资产
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdNetAssets);
    updateStockField(item,m_netAssetsLabel);
    //流通股本
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdDyGuMoney);
    updateStockField(item,m_dyGuMoneyLabel);
    //流通市值
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdDyValue);
    updateStockField(item,m_dyValueLabel);
    //市净率
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdValueRate);
    updateStockField(item,m_valueRateLabel);
    //总股本
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAllGuMoney);
    updateStockField(item,m_allGuMoneyLabel);
    //总市值
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdAllValue);
    updateStockField(item,m_allValueLabel);
    //ROE
    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdRoe);
    updateStockField(item,m_roeLabel);

}

void HandicapBottomWidget::setObjName()
{ 
    m_incomeLabel->setObjectName("numstyle"); //每股收益
    m_proDyLabel->setObjectName("numstyle"); //市盈率
    m_netAssetsLabel->setObjectName("numstyle");   //每股净资产
    m_dyGuMoneyLabel->setObjectName("numstyle"); //流通股本
    m_dyValueLabel->setObjectName("numstyle");   //流通市值
    m_valueRateLabel->setObjectName("numstyle"); //市净率
    m_allGuMoneyLabel->setObjectName("numstyle");  //总股本
    m_allValueLabel->setObjectName("numstyle"); //总市值
    m_roeLabel->setObjectName("numstyle");    //ROE

    QFont font = m_incomeLabel->font();
    font.setStyleStrategy(QFont::NoAntialias);
    m_incomeLabel->setFont(font);

    m_proDyLabel->setFont(font);
    m_netAssetsLabel->setFont(font);
    m_dyGuMoneyLabel->setFont(font);
    m_dyValueLabel->setFont(font);
    m_valueRateLabel->setFont(font);
    m_allGuMoneyLabel->setFont(font);
    m_allValueLabel->setFont(font);
    m_roeLabel->setFont(font);

}

void HandicapBottomWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}



SimilarKlineHandicapTitleWidget::SimilarKlineHandicapTitleWidget(QWidget *parent):
    BaseWidget(parent),
    m_nameLabel(new QLabel),
    m_codeLabel(new QLabel),
    m_priceLabel(new QLabel),
    m_pArrowLabel(new QLabel),
    m_rateLabel(new QLabel),
    m_pAddBtn(new QPushButton),
    m_indiLabel(new QLabel)/*,
      m_pWarnBtn(new QPushButton),
      m_pSetBtn(new QPushButton)*/
{
    this->setFixedHeight(76);
    createWg();
    setObjName();
    updateText();

    connect(m_pAddBtn, &QPushButton::clicked, this, &SimilarKlineHandicapTitleWidget::onAddBtnClicked);
    //    connect(m_pWarnBtn, &QPushButton::clicked, this, &HandicapTitleWidget::onWarningBtnClicked);

    connect(ZXStockOpt::instance(), &ZXStockOpt::stockCountChanged, [&](const int count){
        if(this->isVisible())
        {
            if(0 == count)
            {
                setSelfStock(m_stockCode);
            }
        }
    });

}

void SimilarKlineHandicapTitleWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_stockVec = vec;

    updateText();
    m_warndlg.updateData(vec);
}

void SimilarKlineHandicapTitleWidget::setSelfStock(const QString &strCode)
{
    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();

    if(zxStockOpt->isHas(strCode))
    {
        m_pAddBtn->setProperty("selfstock", true);
    }
    else
    {
        m_pAddBtn->setProperty("selfstock", false);
    }

    m_pAddBtn->style()->unpolish(m_pAddBtn);
    m_pAddBtn->style()->polish(m_pAddBtn);
    m_pAddBtn->update();
}

void SimilarKlineHandicapTitleWidget::OnStockCodeChange()
{
    setSelfStock(m_stockCode);
}

void SimilarKlineHandicapTitleWidget::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);

    QHBoxLayout *pTopLayout = new QHBoxLayout;
    pTopLayout->setMargin(0);
    pTopLayout->setSpacing(0);

    pTopLayout->addSpacing(14);
    pTopLayout->addWidget(m_nameLabel);
    pTopLayout->addSpacing(6);
    pTopLayout->addWidget(m_codeLabel);
    pTopLayout->addSpacing(8);
    pTopLayout->addWidget(m_pAddBtn);
    pTopLayout->addSpacing(14);
    pTopLayout->addWidget(m_indiLabel);
    pTopLayout->addStretch();
    //    pTopLayout->addWidget(m_pWarnBtn);
    //    pTopLayout->addSpacing(2);
    //    pTopLayout->addWidget(m_pSetBtn);
    pTopLayout->addSpacing(10);

//    m_indiLabel->setStyleSheet("background:red");

    QHBoxLayout *pBottomLayout = new QHBoxLayout;
    pBottomLayout->setMargin(0);
    pBottomLayout->setSpacing(0);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setMargin(0);
    formLayout->setSpacing(6);


    formLayout->addRow(m_priceLabel, m_pArrowLabel);
    formLayout->setAlignment(m_pArrowLabel, Qt::AlignVCenter);

    pBottomLayout->addSpacing(14);
    pBottomLayout->addLayout(formLayout);
    pBottomLayout->addSpacing(11);
    pBottomLayout->addWidget(m_rateLabel);
    pBottomLayout->addStretch();
    pMainLayout->addSpacing(12);
    pMainLayout->addLayout(pTopLayout);
//    pMainLayout->addSpacing(10);
    pMainLayout->addLayout(pBottomLayout);
    pMainLayout->addStretch();
}

void SimilarKlineHandicapTitleWidget::updateText()
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
    strcode = strcode + "." + str;
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
    m_pArrowLabel->update();

    item = HzQuoteFormat::QuoteFormat::formatChange(stock.up_down_price,DECIMAL_POINT);

    QString strChange = item.strValue;

    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdChangeRate);

    item.strValue = strChange + "\n" + item.strValue;

    updateStockField(item,m_rateLabel);
}

void SimilarKlineHandicapTitleWidget::setObjName()
{
    m_nameLabel->setObjectName("name");
    m_codeLabel->setObjectName("code");
    m_priceLabel->setObjectName("price");
    m_rateLabel->setObjectName("rate");
    m_pArrowLabel->setObjectName("arrow");
    m_pAddBtn->setObjectName("add");
    m_indiLabel->setObjectName("indi");
    //    m_pWarnBtn->setObjectName("warn");
    //    m_pSetBtn->setObjectName("set");

//    QFont font = m_codeLabel->font();
//    font.setStyleStrategy(QFont::NoAntialias);
//    m_codeLabel->setFont(font);

}

void SimilarKlineHandicapTitleWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}

void SimilarKlineHandicapTitleWidget::onAddBtnClicked()
{
    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();



    if(zxStockOpt->isHas(m_stockCode))
    {
        zxStockOpt->delZXStock(m_stockCode);
        m_pAddBtn->setProperty("selfstock", false);
    }
    else
    {
        if(zxStockOpt->getZXStock().size() == 100)
        {
             QMessageBox::information(this, "提示", "目前只可添加100个自选股", QMessageBox::Ok);
             return;
        }

        zxStockOpt->addZXStock(m_stockCode);
        m_pAddBtn->setProperty("selfstock", true);
    }

    m_pAddBtn->setProperty("selfstock",zxStockOpt->isHas(m_stockCode));


    m_pAddBtn->style()->unpolish(m_pAddBtn);
    m_pAddBtn->style()->polish(m_pAddBtn);
    m_pAddBtn->update();
    return;
}

void SimilarKlineHandicapTitleWidget::onWarningBtnClicked()
{
    m_warndlg.exec();
}
