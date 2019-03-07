#include "lhtopwidget.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFormLayout>
#include <QStyle>
#include "calandarbar.h"
#include "activitylabel.h"
#include "pubmsgdef.h"
#include "drawlinetextpanel.h"

LhTopWidget::LhTopWidget(QWidget *parent):
    BaseWidget(parent)
{
    this->setFixedHeight(324);
    createWg();
    m_listReqId = -1;
    m_timeListId = -1;
}

void LhTopWidget::afterActive()
{
    reqStockLhbDateList();
}

void LhTopWidget::afterDeactive()
{
    cancle();
}

void LhTopWidget::createWg()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    layout->addWidget(createNavigateBar());

//    layout->addWidget(new QLabel("hello"));

    layout->addWidget(createTitle());
    layout->addWidget(createBody());
}

QWidget *LhTopWidget::createNavigateBar()
{
    QWidget *widget = new QWidget;
    m_naviTextWnd  = new DrawLineTextPanel(widget);
    m_naviTextWnd->setFixedHeight(30);
    DrawLineTextItem item;
    QVector<DrawLineTextItem> items;

    //龙虎榜上榜：沪市
    item.itemName = "HsName";
    item.text = QStringLiteral("龙虎榜上榜： 沪市");
    item.left_padding = 18;
    item.font.setFamily("Simsun");
    item.font.setPixelSize(14);
    item.pen.setColor(m_titleTextClr);
    items.push_back(item);

    //数字 上市家数
    item=DrawLineTextItem();
    item.itemName = "HsNum";
    item.text = QStringLiteral("--");
    item.left_padding = 1;
    item.font.setFamily("Arial");
    item.font.setPixelSize(13);
    item.font.setBold(true);
    item.pen.setColor(m_titleNumClr);
    items.push_back(item);

    //家
    item=DrawLineTextItem();
    item.itemName = "HsFamily";
    item.text = QStringLiteral("家");
    item.left_padding = 2;
    item.font.setFamily("Simsun");
    item.font.setPixelSize(14);
    item.pen.setColor(m_titleTextClr);
    items.push_back(item);

    //深市
    item=DrawLineTextItem();
    item.itemName = "szName";
    item.text = QStringLiteral("深市");
    item.left_padding = 8;
    item.font.setFamily("Simsun");
    item.font.setPixelSize(14);
    item.pen.setColor(m_titleTextClr);
    items.push_back(item);

    //数字 上市家数
    item=DrawLineTextItem();
    item.itemName = "SzNum";
    item.text = QStringLiteral("--");
    item.left_padding = 2;
    item.font.setFamily("Arial");
    item.font.setPixelSize(13);
    item.font.setBold(true);
    item.pen.setColor(m_titleNumClr);
    items.push_back(item);

    //家
    item=DrawLineTextItem();
    item.itemName = "SzFamily";
    item.text = QStringLiteral("家");
    item.left_padding = 2;
    item.font.setFamily("Simsun");
    item.font.setPixelSize(14);
    item.pen.setColor(m_titleTextClr);
    items.push_back(item);
    m_naviTextWnd->addItems(items);

    widget->setFixedHeight(30);
    widget->setObjectName("navigate");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_naviTextWnd);
    layout->addWidget(createCalandarBar());
    widget->setLayout(layout);
    return widget;
}

QWidget *LhTopWidget::createTitle()
{
    QWidget *widget = new QWidget;
    widget->setFixedHeight(24);
    widget->setObjectName("title");
    QWidget *centerWidget = new QWidget;
    centerWidget->setObjectName("center");

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    QLabel *label = new QLabel;
    label->setText(QStringLiteral("净买入前十"));
    mainlayout->addSpacing(16);
    mainlayout->addWidget(label);
    mainlayout->addStretch();
    centerWidget->setLayout(mainlayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(2);
    layout->addWidget(centerWidget);
    widget->setLayout(layout);
    return widget;
}

QWidget *LhTopWidget::createBody()
{
    QWidget *widget = new QWidget;
    widget->setFixedHeight(272);
    widget->setObjectName("body");

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    for(int i = 0; i < 10; i++)
    {
        m_netBuyVec.append(new NetBuyWidget);
    }

    layout->addWidget(m_netBuyVec[0], 0, 0);
    layout->addWidget(m_netBuyVec[1], 0, 1);
    layout->addWidget(m_netBuyVec[2], 0, 2);
    layout->addWidget(m_netBuyVec[3], 0, 3);
    layout->addWidget(m_netBuyVec[4], 0, 4);
    layout->addWidget(m_netBuyVec[5], 1, 0);
    layout->addWidget(m_netBuyVec[6], 1, 1);
    layout->addWidget(m_netBuyVec[7], 1, 2);
    layout->addWidget(m_netBuyVec[8], 1, 3);
    layout->addWidget(m_netBuyVec[9], 1, 4);

    widget->setLayout(layout);

    return widget;
}

QWidget *LhTopWidget::createCalandarBar()
{
    mCalandarBtn = new CustomCalandarBtnMarster(true, true);
    connect(mCalandarBtn, &CustomCalandarBtnMarster::signalCalandarBarClicked, [=](quint64 time){
        m_dateTime = time;
        QString timeStr = QDateTime::fromTime_t(time).toString("yyyy-MM-dd");
        mCalandarBtn->setStrText(timeStr);
        reqStockLhbDatas();
        emit signalTimeSelectedChanged(time);
    });
    return mCalandarBtn;
}

void LhTopWidget::reqStockLhbDatas()
{
    StockLhbDatasCallBack fun = std::bind(&LhTopWidget::onStockLhbDatasCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_listReqId = HzInfoCenter::getInstance()->reqStockLhbDatas(m_dateTime, LhbtTotalList, fun);
}

void LhTopWidget::reqStockLhbDateList()
{
    StockLhbDateListCallBack fun = std::bind(&LhTopWidget::onStockLhbDateListCall, this, std::placeholders::_1, std::placeholders::_2);
    m_timeListId = HzInfoCenter::getInstance()->reqStockLhbDateList(fun);
}

void LhTopWidget::cancle()
{
    if(m_listReqId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_listReqId);
        m_listReqId =-1;
    }
    if(m_timeListId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_timeListId);
        m_timeListId =-1;
    }
}

void LhTopWidget::onStockLhbDatasCallBack(const InfoRet &ret, const StockLhbDatas &datas)
{
    if(0 != ret.code)
        return;

    m_stocklhbDatas = datas;
    updateText();
}

void LhTopWidget::onStockLhbDateListCall(const InfoRet &ret, const StockLhbDateList &datas)
{
    if(0 != ret.code)
        return;
    m_stockLhbDateList = datas;
    if(!m_stockLhbDateList.isEmpty())
    {
        QVector<quint64> times;
        for(int i = 0;i < m_stockLhbDateList.size();i++)
        {
            times.prepend(m_stockLhbDateList[i]);
            qDebug() << QDateTime::fromTime_t(m_stockLhbDateList[i]).toString("yyyy-MM-dd");
        }
        m_dateTime = m_stockLhbDateList[0];
        QString timeStr = QDateTime::fromTime_t(m_dateTime).toString("yyyy-MM-dd");
        mCalandarBtn->addValidTimes(times);
        mCalandarBtn->setStrText(timeStr);
//        reqStockLhbDatas();
//        emit signalTimeSelectedChanged(m_dateTime);
    }
}

#include <QDebug>
void LhTopWidget::updateText()
{
    DrawLineTextItem item = m_naviTextWnd->getItem("HsNum");
    item.text = QString::number(m_stocklhbDatas.nShCOunt);
    m_naviTextWnd->setItem(item);

    item = m_naviTextWnd->getItem("SzNum");
    item.text = QString::number(m_stocklhbDatas.nSzCount);
    m_naviTextWnd->setItem(item,true);


    for(int i = 0; i < m_stocklhbDatas.datas.size(); i++)
    {
        m_net_buylists << m_stocklhbDatas.datas[i].net_buy;
    }

    qSort(m_stocklhbDatas.datas.begin(), m_stocklhbDatas.datas.end(),
          [](const StockLhbTypeItem &ItemA,const StockLhbTypeItem &ItemB){
                        return ItemA.net_buy > ItemB.net_buy;});


    for(int i = 0; i < m_netBuyVec.size(); i++)
    {
        StockLhbTypeItem item;
        if(i <= m_stocklhbDatas.datas.size()- 1)
        {
            item = m_stocklhbDatas.datas[i];
        }

        m_netBuyVec[i]->updateData(item);
    }

}

NetBuyWidget::NetBuyWidget(QWidget *parent):
    BaseWidget(parent),
    m_nameLabel(new ActivityLabel),
    m_rateLabel(new QLabel),
    m_netBuyLabel(new QLabel),
    m_amountLabel(new QLabel)
{
    this->setFixedHeight(135);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    createWg();
    setObjName();
    updateText();

    m_nameLabel->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_nameLabel, &ActivityLabel::clicked, [&]{
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_stockCode);
        sendMessage(ROOT_FRAME,params,true);
    });
}

void NetBuyWidget::updateData(const StockLhbTypeItem &Item)
{
    m_stockCode = Item.stockcode;
    m_item = Item;
    updateText();
}

void NetBuyWidget::createWg()
{
    QWidget* widget = new QWidget;

    widget->setFixedHeight(115);

    widget->setObjectName("content");

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);

    mainlayout->addSpacing(23);
    mainlayout->addWidget(m_nameLabel, 0, Qt::AlignCenter);
    mainlayout->addSpacing(13);
    mainlayout->addWidget(m_rateLabel, 0, Qt::AlignCenter);
    mainlayout->addSpacing(13);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->addWidget(m_netBuyLabel);
    hlayout->addWidget(m_amountLabel);
    mainlayout->addLayout(hlayout);
    mainlayout->setAlignment(hlayout, Qt::AlignHCenter);
    mainlayout->addStretch();

    widget->setLayout(mainlayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(10);
    layout->addWidget(widget);
}

void NetBuyWidget::updateText()
{
    QString strValue;
    strValue = m_item.stockname;
    m_nameLabel->setText(strValue);
    strValue = HzQuoteFormat::StringUnitFormat::toFormatString(m_item.close_price) + " " +\
            HzQuoteFormat::StringUnitFormat::toFormatString(m_item.change_ratio)+ "%";

    m_rateLabel->setText(strValue);
    updateStockField(m_item.change_ratio, m_rateLabel);
    m_netBuyLabel->setText(QStringLiteral("净买入 "));
    strValue = HzQuoteFormat::StringUnitFormat::toFormatString(m_item.net_buy);
    m_amountLabel->setText(strValue);
    updateStockField(m_item.net_buy, m_amountLabel);
}

void NetBuyWidget::setObjName()
{
    m_nameLabel->setObjectName("name");
    m_rateLabel->setObjectName("rate");
    m_netBuyLabel->setObjectName("netbuy");
    m_amountLabel->setObjectName("amount");
}

void NetBuyWidget::updateStockField(const double &in, QLabel *label)
{

    if(INVALID_DOUBLE == in)
    {
        label->setProperty("QuoteDirc", QString::number(HzQuoteFormat::quote_unknown));
        label->style()->unpolish(label);
        label->style()->polish(label);
        label->update();
        return;
    }
    if(in  > 0)
    {
        label->setProperty("QuoteDirc", QString::number(HzQuoteFormat::quote_up));
    }
    else if(in  < 0)
    {
        label->setProperty("QuoteDirc", QString::number(HzQuoteFormat::quote_down));
    }
    else
    {
        label->setProperty("QuoteDirc", QString::number(HzQuoteFormat::quote_equal));
    }

    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}
