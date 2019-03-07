#include "mainstatusbar.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimerEvent>
#include <QFormLayout>
#include <QTime>
#include <QMessageBox>
#include <QStyle>
#include "activitylabel.h"
#include "hzline.h"
#include "marqueepanel.h"
#include "shorttermchangedialog.h"
#include "pubmsgdef.h"


#define  Property_UpDown             "updown"
#define  Property_UpDown_Plain   0
#define  Property_UpDown_Up      1
#define  Property_UpDown_Down  2

MainStatusBar::MainStatusBar(QWidget *parent,QString instName):
    StatusBar(parent,instName),
    m_bFlg(false),
    m_indexMgr(new StatusBarIndexMgr(this)),
    m_nReq(INVALID_REQID),
    m_strCurrentCode(""),
    m_localTimeWidget(new LocalTimeWidget)
{
    createWg();
    connect(m_indexMgr,SIGNAL(subStockArrived(char*,int)),this, SLOT(slotSubIndexArrived(char*,int)));
    this->afterActive();
}

MainStatusBar::~MainStatusBar()
{

}

void MainStatusBar::afterActive()
{
    m_indexMgr->startSub();
    startSub();
    m_localTimeWidget->afterActive();
}

void MainStatusBar::afterDeactive()
{
    m_indexMgr->cancelSub();
    cancelSub();
    m_localTimeWidget->afterDeactive();
}

void MainStatusBar::setConnect(bool isOnline)
{
    m_localTimeWidget->setConnect(isOnline);
}

void MainStatusBar::onMessage(const QString &senderPath, const MessageParams &params)
{
    if(senderPath == BROAD_CASET)
    {
        auto it = params.find(HZ_CMD);
        if(it != params.end())
        {
            if(it->second == SHOW_DXYD_DLG)//显示短线异动， 已经显示则不要重复显示
            {

                QRect rc = m_pBtnmore->rect();

                QPoint point = m_pBtnmore->mapToGlobal(rc.topRight());

                ShortTermChangeDialog dlg;

                dlg.move(point - QPoint(729,370));

                dlg.exec();
            }
        }
    }
}


void MainStatusBar::createWg()
{
    createStockPanel();

    QWidget *widget;
    // firstRow zeroCol
    widget =  widgetAtPosition(firstRow,zeroCol);
    QPushButton *btn1 = new QPushButton;
    btn1->setObjectName("opinion");
    connect(btn1, &QPushButton::clicked, [this]{QMessageBox::information(this, "test", "test");});
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->setSpacing(0);
    layout1->setMargin(0);
    layout1->addWidget(btn1,0, Qt::AlignLeft);
    widget->setLayout(layout1);

    widget->hide();

    // firstRow firstCol
    widget =  widgetAtPosition(firstRow,firstCol);
    QHBoxLayout *pMarqueeLayout = new QHBoxLayout ;
    pMarqueeLayout->setSpacing(0);
    pMarqueeLayout->setMargin(0);
    QWidget* pw = new MarqueePanel;
    pMarqueeLayout->addWidget(pw);
    widget->setLayout(pMarqueeLayout);

    // firstRow secondCol
    widget =  widgetAtPosition(firstRow,secondCol);
    QFormLayout *fromSecondCol = new QFormLayout;
    fromSecondCol->setMargin(0);
    m_pLabel = new ActivityLabel;

    m_pLabel->setCursor(QCursor(Qt::PointingHandCursor));


    m_pBtnmore = new QPushButton;
    m_pBtnmore->setCursor(QCursor(Qt::PointingHandCursor));


    connect(m_pLabel, &ActivityLabel::clicked, [&]{
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_strCurrentCode);
        sendMessage(ROOT_FRAME,params,getGlobalPageId()== stock_detail_page?false:true);
    });
    connect(m_pBtnmore, &QPushButton::clicked, this , &MainStatusBar::onBtnMoreClicked);


    fromSecondCol->setFormAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_pLabel->setObjectName("secondCol");
    m_pBtnmore->setObjectName("more");

    fromSecondCol->addRow(m_pLabel, m_pBtnmore);
    fromSecondCol->setAlignment(m_pBtnmore, Qt::AlignRight);
    widget->setLayout(fromSecondCol);

    //firstRow thirdCol
    widget =  widgetAtPosition(firstRow,thirdCol);
    QHBoxLayout *localTimeLayout = new QHBoxLayout;
    localTimeLayout->setSpacing(0);
    localTimeLayout->setMargin(0);
    localTimeLayout->addWidget(m_localTimeWidget);
    widget->setLayout(localTimeLayout);
}

void MainStatusBar::createStockPanel()
{
    QWidget *widget;

    //zeroRow zeroCol
    widget =  widgetAtPosition(zeroRow,zeroCol);
    QHBoxLayout *centerTopLayout = new QHBoxLayout;
    centerTopLayout->setContentsMargins(12,0,0,0);

    StockInfoPanel *shPanel =new StockInfoPanel(this, QStringLiteral("上证指数:"));
    centerTopLayout->addWidget(shPanel);
    m_labelMap["sh000001"] = shPanel->getInfoLabel();
    shPanel->setStockCode("sh000001");


    centerTopLayout->addWidget(new HZLine(Qt::Vertical,this));

    StockInfoPanel *szPanel =new StockInfoPanel(this, QStringLiteral("深证成指:"));
    centerTopLayout->addWidget(szPanel);
    m_labelMap["sz399001"] = szPanel->getInfoLabel();
    centerTopLayout->addWidget(new HZLine(Qt::Vertical,this));
    szPanel->setStockCode("sz399001");

    StockInfoPanel *cyPanel =new StockInfoPanel(this, QStringLiteral("创业板:"));
    centerTopLayout->addWidget(cyPanel);
    m_labelMap["sz399006"] = cyPanel->getInfoLabel();
    centerTopLayout->addWidget(new HZLine(Qt::Vertical,this));
    cyPanel->setStockCode("sz399006");


    StockInfoPanel *hsPanel300 =new StockInfoPanel(this, QStringLiteral("沪深300:"));
    centerTopLayout->addWidget(hsPanel300);
    m_labelMap["sh000300"] = hsPanel300->getInfoLabel();
    centerTopLayout->addWidget(new HZLine(Qt::Vertical,this));
    hsPanel300->setStockCode("sh000300");

    hsPanel300->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);


    StockInfoPanel *zxPanel =new StockInfoPanel(this, QStringLiteral("中小板指:"));
    centerTopLayout->addWidget(zxPanel);
    m_labelMap["sz399005"] = zxPanel->getInfoLabel();
    centerTopLayout->addWidget(new HZLine(Qt::Vertical,this));
    zxPanel->setStockCode("sz399005");

    zxPanel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    StockInfoPanel *szPanel50 =new StockInfoPanel(this, QStringLiteral("上证50:"));
    szPanel50->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    centerTopLayout->addWidget(szPanel50);
    m_labelMap["sh000016"] = szPanel50->getInfoLabel();
    widget->setLayout(centerTopLayout);
    szPanel50->setStockCode("sh000016");

    //build connect
    connect(shPanel, &StockInfoPanel::clicked, this, &MainStatusBar::onLabelClicked);
    connect(szPanel, &StockInfoPanel::clicked, this, &MainStatusBar::onLabelClicked);
    connect(cyPanel, &StockInfoPanel::clicked, this, &MainStatusBar::onLabelClicked);
    connect(hsPanel300, &StockInfoPanel::clicked, this, &MainStatusBar::onLabelClicked);
    connect(zxPanel, &StockInfoPanel::clicked, this, &MainStatusBar::onLabelClicked);
    connect(szPanel50, &StockInfoPanel::clicked, this, &MainStatusBar::onLabelClicked);
}

void MainStatusBar::startSub()
{
    cancelSub();
    ReqShortMotionInfo req;
    req.type = SMCT_Total;
    req.index = -1;
    req.count_range = 1;
    req.is_desc = true; //反拉

    OnShortMotionInfoArrived fun = std::bind(&MainStatusBar::onSubDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    m_nReq = gHZDataController->subShortMotionInfo(req, fun);

//    m_nReq = gHZDataController->requestShortMotionInfo(req, fun);
}

void MainStatusBar::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

//void MainStatusBar::onAbnormalInfoArrived(ReqId reqID, const StockAbnormalInfo &abnormalInfo, ErrCode errCode)
//{
//    if(0 == errCode)
//    {
//        if(abnormalInfo.blockAbnormals.size())
//        {
//            QString strType = "";
//            switch (abnormalInfo.blockAbnormals[0].abnormal_type) {
//            case speed_up:
//                strType =  "  加速上涨";
//                break;
//            case speed_down:
//                strType =  "  加速下跌";
//                break;
//            case big_sell:
//                strType =  "  大笔卖出";
//                break;
//            case big_buy:
//                strType =  "  大笔买入";
//                break;
//            case rise_stop:
//                strType =  "  封涨停板";
//                break;
//            case fall_stop:
//                strType =  "  封跌停板";
//                break;
//            }

//            m_pLabel->setText(QString::fromStdString(abnormalInfo.blockAbnormals[0].stock_name) + strType);
//        }

//    }
//}

void MainStatusBar::onSubDataArrived(ReqId reqID, const ShortMotionInfo &info, ErrCode errCode)
{
    if(0 == errCode)
    {
        if(info.shortMotions.size())
        {
            QString strType = "";
            switch (info.shortMotions[0].short_type) {
            case OrganizationEat:
                strType =  "机构吃货";
                break;
            case LargeBid:
                strType = "有大买盘";
                break;
            case RocketLaunch:
                strType = "火箭发射";
                break;
            case RapidRebound:
                strType = "快速反弹";
                break;
            case HighDiving:
                strType = "高台跳水";
                break;
            case AccelerateDecline:
                strType = "加速下跌";
                break;
            case LargeBuy:
                strType = "大笔买入";
                break;
            case LargeSell:
                strType = "大笔卖出";
                break;
            case LargeAsk:
                strType = "有大卖盘";
                break;
            case UpperLimit:
                strType = "封涨停板";
                break;
            case DownLimit:
                strType = "封跌停板";
                break;
            case OpenUpperLimit:
                strType = "打开涨停";
                break;
            case OpenDownLimit:
                strType = "打开跌停";
                break;
            case OrganizationBuy:
                strType = "机构买单";
                break;
            case OrganizationSell:
                strType = "机构卖单";
                break;
            case OrganizationSpit:
                strType = "机构吐货";
                break;
            case BuyNew:
                strType = "买入新单";
                break;
            case SellNew:
                strType = "卖出新单";
                break;
            default:
                break;
            }
            m_strCurrentCode = QString::fromStdString(info.shortMotions[0].stock_code);
            m_pLabel->setText("  " + QString::fromStdString(info.shortMotions[0].stock_name) +" " +  strType);
            upDateLabel(info.shortMotions[0].short_type, m_pLabel);
        }
    }
}

void MainStatusBar::setTextEx(QString strCode, QString strText, int dir)
{
    m_labelMap[strCode]->setProperty(Property_UpDown,dir);
    m_labelMap[strCode]->style()->unpolish(m_labelMap[strCode]);
    m_labelMap[strCode]->style()->polish(m_labelMap[strCode]);
    m_labelMap[strCode]->setText(strText);
}

void MainStatusBar::upDateLabel(ShortMotionType type, QLabel *label)
{
    switch (type) {
    case OrganizationEat:
    case LargeBid:
    case RocketLaunch:
    case RapidRebound:
    case LargeBuy:
    case UpperLimit:
    case OpenDownLimit:
    case OrganizationBuy:
    case BuyNew:
        label->setProperty("state", "rise");
        break;

    case HighDiving:
    case AccelerateDecline:
    case LargeSell:
    case LargeAsk:
    case DownLimit:
    case OpenUpperLimit:
    case OrganizationSell:
    case OrganizationSpit:
    case SellNew:
        label->setProperty("state", "fall");
        break;
    default:
        label->setProperty("state", "default");
        break;
    }

    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}

void MainStatusBar::onLabelClicked(const QString &strCode)
{
   MessageParams params;
   params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
   params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
   params.emplace(STOCK_ID,strCode);
   sendMessage(ROOT_FRAME,params,getGlobalPageId()== stock_detail_page?false:true);
}

void MainStatusBar::onBtnMoreClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(this->sender());

    QRect rc = btn->rect();

    QPoint point = btn->mapToGlobal(rc.topRight());

    ShortTermChangeDialog dlg;

    dlg.move(point - QPoint(729,370));

    dlg.exec();
}


void MainStatusBar::slotSubIndexArrived(char *data, int num)
{
    HZData::SLStock *slData = (HZData::SLStock*)data;

    for(int i = 0; i < num; i++)
    {
        HZData::SLStock slStock = slData[i];

        QString strLast_pice = slStock.last_price == INVALID_DOUBLE?Default_Item_String:QString::number(slStock.last_price, 'f', 2);
        QString strUp_down_price = slStock.up_down_price == INVALID_DOUBLE?Default_Item_String:QString::number(slStock.up_down_price, 'f', 2);
        QString  str_identity = slStock.rate == INVALID_DOUBLE?"":(slStock.rate < -DBL_EPSILON)?QStringLiteral("↓"):QStringLiteral("↑");
        str_identity = (  -DBL_EPSILON <= slStock.rate &&  slStock.rate <= DBL_EPSILON)?"":str_identity;
        QString strRate        = slStock.rate == INVALID_DOUBLE? Default_Item_String:QString("%1%").arg(slStock.rate, 0,'f', 2);

        QString strText = strLast_pice + str_identity + "   " + strUp_down_price + "   " + strRate;

        int updown =  0;
        if(slStock.rate != INVALID_DOUBLE){
            double new_up_down  = QString::number(slStock.up_down_price,'f',2).toDouble();
            if(new_up_down > DBL_EPSILON)
                updown= Property_UpDown_Up;
            else if(new_up_down<-DBL_EPSILON)
                updown= Property_UpDown_Down;
        }
        setTextEx(QString::fromStdString(slStock.stock_code), strText, updown);
    }
}


///StatusBarIndexMgr-----
void StatusBarIndexMgr::startSub()
{
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back("sh000001");
    reqDynaInfo.stock_code.push_back("sz399001");
    reqDynaInfo.stock_code.push_back("sz399006");
    reqDynaInfo.stock_code.push_back("sh000300");
    reqDynaInfo.stock_code.push_back("sz399005");
    reqDynaInfo.stock_code.push_back("sh000016");
    reqDynaInfo.mask = HZData::SLStock_Mask;

    OnStockDynaArrived stockDynaArrived = std::bind(&StatusBarIndexMgr::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3, std::placeholders::_4);

    m_nReq = gHZDataController->subOptionalDyna(reqDynaInfo, stockDynaArrived);

}

void StatusBarIndexMgr::cancelSub()
{
    if(m_nReq != INVALID_REQID){
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}


void StatusBarIndexMgr::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    if(0 == errCode)
        emit subStockArrived(data, num);

}


LocalTimeWidget::LocalTimeWidget(QWidget *parent):
    BaseWidget(parent),
    m_pLogoBtn(new QPushButton),
    m_pTimeLabel(new ActivityLabel)
{
    createWg();
    m_nTimerID = this->startTimer(1000);
}

LocalTimeWidget::~LocalTimeWidget()
{
    this->killTimer(m_nTimerID);
}

void LocalTimeWidget::afterActive()
{
    startReqest();
}

void LocalTimeWidget::afterDeactive()
{
}

void LocalTimeWidget::setConnect(bool isOnline)
{

    m_pLogoBtn->setProperty("online", isOnline);
    m_pLogoBtn->style()->unpolish(m_pLogoBtn);
    m_pLogoBtn->style()->polish(m_pLogoBtn);
    m_pLogoBtn->update();
}

void LocalTimeWidget::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_nTimerID)
    {
        if(m_time != INT_MAX)
        {
            QDateTime utc;
            utc.setTimeSpec(Qt::UTC);
            utc.setTime_t(QDateTime::currentDateTimeUtc().toTime_t()+8*3600+m_time);
            m_pTimeLabel->setText(utc.toString("HH:mm:ss"));
        }else
        {
            m_pTimeLabel->setText("--:--:--");
        }
        m_checkTime--;
        //qDebug() << "m_checkTime: " << m_checkTime;
        if(m_checkTime < 0)
        {
            startReqest();
            m_checkTime = 5;
        }
   }

}

void LocalTimeWidget::createWg()
{
    QFormLayout * pLayout = new QFormLayout(this);
    pLayout->setSpacing(2);
    pLayout->setMargin(0);

    pLayout->setFormAlignment(Qt::AlignVCenter);

    m_pLogoBtn->setObjectName("online");
    m_pTimeLabel->setText("--:--:--");
//    m_pLogoBtn->setCursor(QCursor(Qt::PointingHandCursor));
//    connect(m_pLogoBtn, &QPushButton::clicked, [this]{QMessageBox::information(this, "test", "test");});

    pLayout->addRow(m_pLogoBtn, m_pTimeLabel);

}

void LocalTimeWidget::startReqest()
{
    OnTimeArrived  fun = std::bind(&LocalTimeWidget::onTimeArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    gHZDataController->requestTime(fun);
}


void LocalTimeWidget::onTimeArrived(ReqId id, int nTime, ErrCode errCode)
{
    Q_UNUSED(id)

    if(0 == errCode)
    {
        int utcTime = QDateTime::currentDateTimeUtc().toTime_t();
        m_time = nTime - utcTime;
    }else
    {
        m_time = INT_MAX;
    }
}


StockInfoPanel::StockInfoPanel(QWidget *parent, QString strMarket):
    BaseWidget(parent),
    m_strMarket(strMarket)
{
    createWg();
}

ActivityLabel *StockInfoPanel::getInfoLabel()
{
    return m_pTextLabel;
}

void StockInfoPanel::createWg()
{
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->setMargin(0);
    formLayout->setSpacing(5);
    m_pTextLabel = new ActivityLabel("-- -- --",this);
    m_pTextLabel->setCursor(QCursor(Qt::PointingHandCursor));
    m_pTextLabel->setObjectName("centertoplbl");
    m_pTextLabel->setProperty(Property_UpDown,Property_UpDown_Plain);
    ActivityLabel *marketLabel = new ActivityLabel(m_strMarket, this);
    marketLabel ->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    marketLabel->setCursor(QCursor(Qt::PointingHandCursor));
    formLayout->addRow(marketLabel, m_pTextLabel);
    formLayout->setFormAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    connect(m_pTextLabel, &ActivityLabel::clicked, [this]{emit clicked(m_stockCode);});
    connect(marketLabel, &ActivityLabel::clicked, [this]{emit clicked(m_stockCode);});
}
