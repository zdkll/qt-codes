#include "lhpartswidget.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyle>
#include <QDebug>
#include <QMenu>
#include <QListWidget>
#include <QMessageBox>
#include "hzuserdatamgr.h"
#include "hzauthenopt.h"
#include "localstocksource.h"
#include "lhrisefalltradiwidget.h"
#include "qlistbutton.h"
#include "lhbuysellview.h"
#include "zxstockopt.h"


LhTitieWidget::LhTitieWidget(QWidget *parent):
    BaseWidget(parent),
    m_pNameLabel(new QLabel),
    m_pCodeLabel(new QLabel),
    m_pPriceLabel(new QLabel),
    m_pArrowLabel(new QLabel),
    m_pRateLabel(new QLabel),
    m_selfStockBtn(new QPushButton),
    m_listBtn(new QListButton),
    m_menu(new QMenu(this)),
    m_listWidget(new QListWidget)
{
    createWg();
    setObjName();
    updateText();

    connect(m_selfStockBtn, &QPushButton::clicked, this, &LhTitieWidget::onBtnClicked);

    initListWidget();
}

LhTitieWidget::~LhTitieWidget()
{
    removeListWidget();
}

void LhTitieWidget::updateData(const HZData::AllStock &stock)
{
    m_allStock = stock;
    updateText();
}

void LhTitieWidget::updateLhData(const StockLhbDetialList &data)
{
    m_lhDetailList = data;
    setMenu();
}


void LhTitieWidget::OnStockCodeChange()
{
    setSelfStock();
}

void LhTitieWidget::createWg()
{
    this->setFixedHeight(30);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    layout->setContentsMargins(0, 0, 5, 0);

    layout->addSpacing(10);
    layout->addWidget(m_pNameLabel);
    layout->addSpacing(14);
    layout->addWidget(m_pCodeLabel);
    layout->addSpacing(14);
    layout->addWidget(m_pPriceLabel);
    layout->addSpacing(5);
    layout->addWidget(m_pArrowLabel);
    layout->addSpacing(8);
    layout->addWidget(m_pRateLabel);
    layout->addSpacing(10);
    layout->addWidget(m_selfStockBtn);
    layout->addStretch();

    layout->addWidget(m_listBtn);

    m_listBtn->setMenu(m_menu);

    m_menu->setFixedHeight(120);

}

void LhTitieWidget::updateText()
{
    HZData::AllStock stock = m_allStock;

    HzQuoteFormat::QuoteItem item;
    item = HzQuoteFormat::QuoteFormat::format(stock, FieldIdStockName);
    updateStockField(item, m_pNameLabel);

    QString strCode = QString::fromStdString(stock.stock_code);
    m_pCodeLabel->setText(strCode.remove(0, 2));

    item = HzQuoteFormat::QuoteFormat::format(stock, FieldIdLastpx);
    updateStockField(item, m_pPriceLabel);

    m_pArrowLabel->setProperty("state", QString::number(item.dirc));
    m_pArrowLabel->style()->unpolish(m_pArrowLabel);
    m_pArrowLabel->style()->polish(m_pArrowLabel);
    m_pArrowLabel->update();

    item = HzQuoteFormat::QuoteFormat::formatChange(stock.up_down_price,DECIMAL_POINT);

    QString strChange = item.strValue;

    item = HzQuoteFormat::QuoteFormat::format(stock,FieldIdChangeRate);

    item.strValue = strChange + " " + item.strValue;

    updateStockField(item,m_pRateLabel);
}

void LhTitieWidget::setObjName()
{
    m_pNameLabel->setObjectName("name");
    m_pCodeLabel->setObjectName("code");
    m_pPriceLabel->setObjectName("price");
    m_pRateLabel->setObjectName("rate");
    m_pArrowLabel->setObjectName("arrow");
    m_selfStockBtn->setObjectName("add");
}

void LhTitieWidget::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc", QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}

void LhTitieWidget::setSelfStock()
{
    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();

    if(zxStockOpt->isHas(m_stockCode))
    {
        m_selfStockBtn->setProperty("selfstock", true);
    }
    else
    {
        m_selfStockBtn->setProperty("selfstock", false);
    }

    m_selfStockBtn->style()->unpolish(m_selfStockBtn);
    m_selfStockBtn->style()->polish(m_selfStockBtn);
    m_selfStockBtn->update();

}

void LhTitieWidget::setMenu()
{
    m_dateInexMap.clear();
    removeListWidget();
    QString str;
    for(int i = 0; i < m_lhDetailList.size(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setTextAlignment(Qt::AlignCenter);
        str = QDateTime::fromTime_t(m_lhDetailList[i].date).toString("yyyy-MM-dd");
        if(0 == i)
        {
            m_listBtn->updateText(str);
        }
        newItem->setText(str);
        m_dateInexMap.insert(str, i);
        m_listWidget->insertItem(i, newItem);

        qDebug() << Q_FUNC_INFO << str;
    }

}

void LhTitieWidget::initListWidget()
{

    connect(m_listWidget, &QListWidget::itemClicked, [&](QListWidgetItem *item){
       QString strText = item->text();

       qDebug() << m_dateInexMap[strText];

       emit currentIndex(m_dateInexMap[strText]);

       m_listBtn->updateText(strText, true);
       m_listBtn->setChecked(true);
       m_menu->hide();
    });

    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_listWidget);

    m_menu->setLayout(layout);
}

void LhTitieWidget::removeListWidget()
{
    for(int i=0;i< m_listWidget->count();i++)
    {

        QListWidgetItem *item = m_listWidget->takeItem(0); //这里是0，不是i，因为每移除一个item都会导致每个item的row发生变化
        delete item;
    }

    m_listWidget->clear();

}

void LhTitieWidget::onBtnClicked()
{

    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();

    if(zxStockOpt->isHas(m_stockCode))
    {
        zxStockOpt->delZXStock(m_stockCode);
        m_selfStockBtn->setProperty("selfstock", false);
    }
    else
    {
        if(zxStockOpt->getZXStock().size() == 100)
        {
             QMessageBox::information(this, "提示", "目前只可添加100个自选股", QMessageBox::Ok);
             return;
        }

        zxStockOpt->addZXStock(m_stockCode);
        m_selfStockBtn->setProperty("selfstock", true);
    }

    m_selfStockBtn->setProperty("selfstock",zxStockOpt->isHas(m_stockCode));

    m_selfStockBtn->style()->unpolish(m_selfStockBtn);
    m_selfStockBtn->style()->polish(m_selfStockBtn);
    m_selfStockBtn->update();
}



LhRiseFallWidget::LhRiseFallWidget(QWidget *parent):
    BaseWidget(parent),
    m_titleLabel(new QLabel),
    m_netBuyLabel(new QLabel),
    m_allBuyLabel(new QLabel),
    m_allSellLabel(new QLabel),
    m_lhRiseFall(new LhRiseFallTradiWidget(this))
{
    this->setFixedHeight(128);
    createWg();
    setObjName();
    updateText();
}

void LhRiseFallWidget::afterActive()
{
//    startSub();
}

void LhRiseFallWidget::afterDeactive()
{

}

void LhRiseFallWidget::updateLhData(const StockLhbDetialListItem &data)
{
    m_lhDetailListItem = data;
    m_lhRiseFall->updateLhData(m_lhDetailListItem);
    updateText();
}

void LhRiseFallWidget::createWg()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(16, 0, 16, 0);
    layout->addSpacing(15);
    layout->addWidget(m_titleLabel);
    layout->addSpacing(15);
    layout->addLayout(createLayout());
    layout->addSpacing(15);
    layout->addWidget(m_lhRiseFall);
    layout->addStretch();
}

QHBoxLayout *LhRiseFallWidget::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(new QLabel("净买入："));
    layout->addWidget(m_netBuyLabel);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("买入总计："));
    layout->addWidget(m_allBuyLabel);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("卖出总计："));
    layout->addWidget(m_allSellLabel);
    layout->addStretch();
    return layout;
}

void LhRiseFallWidget::setObjName()
{
    m_titleLabel->setObjectName("title");
    m_netBuyLabel->setObjectName("netbuy");
    m_allBuyLabel->setObjectName("allbuy");
    m_allSellLabel->setObjectName("allsell");
}

void LhRiseFallWidget::OnStockCodeChange()
{
//    afterActive();
}

void LhRiseFallWidget::startSub()
{
    StockLhbDetialCallBack fun = std::bind(&LhRiseFallWidget::onLhbDataArrived, this, std::placeholders::_1, std::placeholders::_2);
    HzInfoCenter::getInstance()->reqStockLhbDetial(1541088000, m_stockCode, fun);
}

void LhRiseFallWidget::onLhbDataArrived(const InfoRet &infoRet, const StockLhbDetialList &data)
{
    if(0 != infoRet.code)
        return;

    qDebug() << "LhRiseFallWidget::onLhbDataArrived";
}

void LhRiseFallWidget::updataStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc", QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}

void LhRiseFallWidget::updateText()
{
    QString str = "--";

    StockLhbDetialListItem lhDetail;
    if(!m_lhDetailListItem.reason.isEmpty())
        lhDetail = m_lhDetailListItem;
    if(lhDetail.reason.isEmpty())
    {
        m_titleLabel->setText(str);
    }
    else
    {
        QString strTmp;
        strTmp = QStringLiteral("上榜理由：");
        m_titleLabel->setText(strTmp + lhDetail.reason);
    }

    HzQuoteFormat::QuoteItem item;
    item = HzQuoteFormat::QuoteFormat::formatChange(lhDetail.net_buy,DECIMAL_POINT);
    item.strValue = HzQuoteFormat::StringUnitFormat::toFormatString(lhDetail.net_buy);
    updataStockField(item, m_netBuyLabel);

    item = HzQuoteFormat::QuoteFormat::formatChange(lhDetail.buy,DECIMAL_POINT);
    item.strValue = HzQuoteFormat::StringUnitFormat::toFormatString(lhDetail.buy);
    updataStockField(item, m_allBuyLabel);

    item = HzQuoteFormat::QuoteFormat::formatChange(-lhDetail.sell,DECIMAL_POINT);
    item.strValue = "-" + HzQuoteFormat::StringUnitFormat::toFormatString(lhDetail.sell);
    updataStockField(item, m_allSellLabel);
}

LhbBottomWidget::LhbBottomWidget(QWidget *parent):
    BaseWidget(parent),
    m_riseFallWidget(new LhRiseFallWidget),
    m_lhBuyView(new LhBuySellView(this, 0)),
    m_lhSellView(new LhBuySellView(this, 1))
{
    createWg();

    connect(m_lhBuyView,&LhBuySellView::clickRow,this,&LhbBottomWidget::slotsBuySellViewClickRow);
    connect(m_lhSellView,&LhBuySellView::clickRow,this,&LhbBottomWidget::slotsBuySellViewClickRow);
}

void LhbBottomWidget::updateLhData(const StockLhbDetialListItem &data)
{
    m_riseFallWidget->updateLhData(data);
    m_lhBuyView->updateLhData(data);
    m_lhSellView->updateLhData(data);
}

void LhbBottomWidget::slotsBuySellViewClickRow(const QString &strID)
{
    emit clickRow(strID);
}

void LhbBottomWidget::createWg()
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    mainlayout->addWidget(m_riseFallWidget);
    mainlayout->addWidget(m_lhBuyView, 1);
    mainlayout->addWidget(m_lhSellView, 1);

}
