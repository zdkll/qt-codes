#include "blocktradetopwnd.h"
#include "calandarbar.h"
#include "smtcurvegraph.h"
#include "quoteformat.h"
#include "httpdata.h"
BlockTradeTopWnd::BlockTradeTopWnd(QWidget *parent):
    BaseWidget(parent)
{
    createWg();
}

void BlockTradeTopWnd::afterActive()
{
    reqDateList();
}

void BlockTradeTopWnd::afterDeactive()
{
    cancle();
    m_smtCurveWnd->afterDeactive();
}

StockRzrqDetailList BlockTradeTopWnd::getDatas()
{
    return m_datas;
}

qint64 BlockTradeTopWnd::getTime()
{
    return m_dateTime;
}

void BlockTradeTopWnd::setCurrShowTime(qint64 time)
{
    m_externalTime = time;
}

void BlockTradeTopWnd::OnStockCodeChange()
{
    m_pStockName->setText(Httpdata::instance()->getCodeNameByCode(m_stockCode));
    m_pStockName->update();
}

void BlockTradeTopWnd::createWg()
{
    setObjectName("BlockTradeTopWnd");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(createNavigateBar());
    //创建融券融券曲线图
    m_smtCurveWnd = new SmtCurveGraphCanvas(this);
    m_smtCurveWnd->createWg();
    layout->addWidget(m_smtCurveWnd);
    this->setLayout(layout);
}

QWidget *BlockTradeTopWnd::createCalandarBar()
{
    mCalandarBtn = new CustomCalandarBtnMarster(false,true);
    mCalandarBtn->setObjectName("CustomCalandarBtnMarster");
    connect(mCalandarBtn, &CustomCalandarBtnMarster::signalCalandarBarClicked, [=](quint64 time){
        QString timeStr = QDateTime::fromTime_t(time).toString("yyyy-MM-dd");
        mCalandarBtn->setStrText(timeStr);
        m_dateTime = time;
        reqStockDatas();
    });
    return mCalandarBtn;
}

QWidget *BlockTradeTopWnd::createNavigateBar()
{
    QWidget *widget = new QWidget;
    widget->setFixedHeight(30);
    widget->setObjectName("navigate");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(createCalandarBar());


    //股票名称
    layout->addSpacing(10);
    m_pStockName = new QLabel;
    m_pStockName->setObjectName("filedName");
    m_pStockName->setAlignment(Qt::AlignCenter);
    m_pStockName->setText("--");
    layout->addWidget(m_pStockName);

    //融资余额
    layout->addSpacing(10);
    QLabel* pLabel = new QLabel;
    pLabel->setObjectName("filedName");
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setText(QStringLiteral("融资余额"));
    layout->addWidget(pLabel);

    layout->addSpacing(10);
    m_pRzye = new QLabel;
    m_pRzye->setObjectName("filedValue");
    m_pRzye->setAlignment(Qt::AlignCenter);
    m_pRzye->setText(QStringLiteral("--"));
    layout->addWidget(m_pRzye);

    //融券余额
    layout->addSpacing(10);
    pLabel = new QLabel;
    pLabel->setObjectName("filedName");
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setText(QStringLiteral("融券余额"));
    layout->addWidget(pLabel);

    layout->addSpacing(10);
    m_pRqye = new QLabel;
    m_pRqye->setObjectName("filedValue");
    m_pRqye->setAlignment(Qt::AlignCenter);
    m_pRqye->setText(QStringLiteral("--"));
    layout->addWidget(m_pRqye);
    layout->addStretch();
    widget->setLayout(layout);
    return widget;
}

void BlockTradeTopWnd::reqStockDatas()
{
    StockRzrqDetailListCallBack fun = std::bind(&BlockTradeTopWnd::onStocDatasCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_listReqId = HzInfoCenter::getInstance()->reqStockRzrqDetailList(m_dateTime,m_stockCode, fun);
}

void BlockTradeTopWnd::reqDateList()
{
    StockRzrqDateListCallBack fun = std::bind(&BlockTradeTopWnd::onStockDateListCall, this, std::placeholders::_1, std::placeholders::_2);
    m_timeListId = HzInfoCenter::getInstance()->reqStockRzrqDateList(fun);
}

void BlockTradeTopWnd::cancle()
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

void BlockTradeTopWnd::onStocDatasCallBack(const InfoRet &ret, const StockRzrqDetailList &datas)
{
    if(0 != ret.code)
        return;
    m_datas = datas;
    double rzye = INT_MAX;
    double rqye = INT_MAX;
    QString rzDirc = "quote_equal";
    QString rqDirc = "quote_equal";
    if(!m_datas.dataList.isEmpty())
    {
        int size = m_datas.dataList.size();
        rzye = m_datas.dataList[0].rzye;
        rqye = m_datas.dataList[0].rqye;

        if(size >=2)
        {
            HzQuoteFormat::QuoteItem item = HzQuoteFormat::QuoteFormat::formatPrice(rzye,m_datas.dataList[1].rzye);
            rzDirc = item.getDirc();
            item = HzQuoteFormat::QuoteFormat::formatPrice(rqye,m_datas.dataList[1].rqye);
            rqDirc = item.getDirc();
        }
    }
    m_pRzye->setText(HzQuoteFormat::StringUnitFormat::toFormatString(rzye));
    m_pRzye->setProperty("QuoteDirc", rzDirc);
    m_pRzye->style()->unpolish(m_pRzye);
    m_pRzye->style()->polish(m_pRzye);
    m_pRzye->update();

    m_pRqye->setText(HzQuoteFormat::StringUnitFormat::toFormatString(rqye));
    m_pRqye->setProperty("QuoteDirc", rqDirc);
    m_pRqye->style()->unpolish(m_pRqye);
    m_pRqye->style()->polish(m_pRqye);
    m_pRqye->update();

    m_smtCurveWnd->setDatas(m_datas);
    m_smtCurveWnd->afterActive();
    emit signalDataChanged();
}

void BlockTradeTopWnd::onStockDateListCall(const InfoRet &ret, const StockRzrqDateList &datas)
{
    if(0 != ret.code)
        return;
    if(!datas.isEmpty())
        m_dateList = datas;
    if(!m_dateList.isEmpty())
    {
        QVector<quint64> times;
        for(int i = 0;i < m_dateList.size();i++)
        {
            times.prepend(m_dateList[i]);
            qDebug() << QDateTime::fromTime_t(m_dateList[i]).toString("yyyy-MM-dd");
        }
        if(m_externalTime != -1)
        {
            m_dateTime = m_externalTime;
            m_externalTime =-1;
        }else
        {
            m_dateTime = m_dateList[0];
        }
        //mCalandarCtrl->addInvalidTimes(times);
        QString timeStr = QDateTime::fromTime_t(m_dateTime).toString("yyyy-MM-dd");
        mCalandarBtn->setStrText(timeStr);
        mCalandarBtn->addValidTimes(times);
    }
}
