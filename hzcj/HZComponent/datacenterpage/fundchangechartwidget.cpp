#include "fundchangechartwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>

//namespace fundColor {
//QColor lineColor("#2f2f2f");
//QColor riselineColor("#9a1f1f");
//QColor riseColor("#350c0f");
//QColor falllineColor("#127f3a");
//QColor fallColor("#0b2e18");
//QColor textColor("#cad4dc");
//QColor riseNumColor("#e62727");
//QColor fallNumColor("#0aaa38");
//}
//using namespace fundColor;

float m_a[6] = {200099999999, 210066666666, 120088888888, -56722222222, \
                        -45633333333, -34511111111};

FundChangeChart::FundChangeChart(QWidget *parent):
    BaseWidget(parent),
    m_pix(nullptr)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_numFont.setBold(true);
    m_numFont.setFamily("Arial");
    m_numFont.setPixelSize(13);
    m_textFont.setFamily("SimSun");
    m_textFont.setPixelSize(14);
    initData();
}

FundChangeChart::~FundChangeChart()
{
    if(m_pix)
        delete m_pix;
}

void FundChangeChart::initData()
{
    FundChangeSet fundChangeSet(6);

    QStringList lists = {QStringLiteral("上证指数"), QStringLiteral("深证成指"),
                        QStringLiteral("创业板指"), QStringLiteral("中小板指"),
                        QStringLiteral("沪深300"), QStringLiteral("上证50")};

    for(int i = 0; i < lists.size(); i++)
    {
        fundChangeSet[i].strName = lists[i];
    }

    m_fundChangeData.fundSet.append(fundChangeSet);
}


void FundChangeChart::updateData(char *data, int num)
{
    StockMoney  *stockMoney = (StockMoney*)data;
    m_netputList.clear();
    for(int i = 0; i < num; i++)
    {
        if(INVALID_INT64 == stockMoney[i].money_one_day.leader_money.money_netput)
        {
            m_netputList.push_back(0);
        }
        else
        {
            m_netputList.push_back(stockMoney[i].money_one_day.leader_money.money_netput);
        }
    }

    if(m_pix)
    {
        delete m_pix;
    }

    m_pix = new QPixmap(this->size());
    m_pix->fill(Qt::transparent);

    draw();

    this->update();
}

void FundChangeChart::paintEvent(QPaintEvent *event)
{
    BaseWidget::paintEvent(event);

    QPainter painter(this);

    painter.drawPixmap(this->rect(), *m_pix);

}

void FundChangeChart::resizeEvent(QResizeEvent *event)
{
    if(m_pix)
    {
        delete m_pix;
    }
    m_pix = new QPixmap(this->size());
    m_pix->fill(Qt::transparent);
    draw();
}

void FundChangeChart::draw()
{
    if(m_netputList.isEmpty())
        return ;

    float maxValue = 0.0f;


    for(int i = 0; i < m_netputList.size(); i++)
    {

        if(m_netputList[i] > 0)
        {
            m_fundChangeData.fundSet[i].borderColor = m_styledWidget.riseBorderColor();
            m_fundChangeData.fundSet[i].brushColor = m_styledWidget.riseBrushColor();
            m_fundChangeData.fundSet[i].numColor = m_styledWidget.riseNumColor();
        }
        else
        {
            m_fundChangeData.fundSet[i].borderColor = m_styledWidget.fallBorderColor();
            m_fundChangeData.fundSet[i].brushColor = m_styledWidget.fallBrushColor();
            m_fundChangeData.fundSet[i].numColor = m_styledWidget.fallNumColor();
        }

        m_fundChangeData.fundSet[i].strValue = QString::number(m_netputList[i] / 1e8, 'f', 2);
        m_fundChangeData.fundSet[i].value = qAbs(m_netputList[i] / 1e8);
        maxValue = qMax(maxValue, m_fundChangeData.fundSet[i].value);
    }

    if(maxValue == 0)
        return;

    m_fundChangeData.maxValue = maxValue;

    int nWidth = this->rect().width();
    int nHeight = this->rect().height();

    QFontMetrics metrics(m_textFont);

    QFontMetrics fontMetrics(m_numFont);

    int nDrawingHeight = nHeight  - metrics.height() - 11;

    int nRectMaxHeight = nDrawingHeight - 1 - fontMetrics.height() - 9 - 1;

    float unitHeight = nRectMaxHeight / m_fundChangeData.maxValue;

    float nSpacing = (float)nDrawingHeight / 3;

    QPainter painter(m_pix);
    painter.setFont(m_textFont);

    //画坐标线
    painter.setPen(m_styledWidget.lineColor());
    for(int i = 0; i < 3; i++)
    {
        painter.drawLine(0, nSpacing*i, nWidth, nSpacing*i);
    }
    painter.drawLine(0, nDrawingHeight, nWidth, nDrawingHeight);

    float nAvgWidth = (float)nWidth / 6;
    //画矩形
    QRect rc;
    for(int i = 0; i < 6; i++)
    {
        rc.setRect(nAvgWidth / 4 + i*nAvgWidth, nDrawingHeight - 1, nAvgWidth / 2, -unitHeight * m_fundChangeData.fundSet[i].value);
        painter.setPen(m_fundChangeData.fundSet[i].borderColor);
        painter.setBrush(m_fundChangeData.fundSet[i].brushColor);
        painter.drawRect(rc);
    }
    //画下方文字
    painter.setPen(m_styledWidget.textColor());
    QRect textRc;
    for(int i = 0; i < 6; i++)
    {
        textRc.setRect((nAvgWidth - metrics.width(m_fundChangeData.fundSet[i].strName)) / 2 + nAvgWidth*i, nDrawingHeight + 11 \
                       ,metrics.width(m_fundChangeData.fundSet[i].strName), metrics.height());
        painter.drawText(textRc, m_fundChangeData.fundSet[i].strName);

    }
    //画上方数字
    QRect numRc;
    painter.setFont(m_numFont);
    for(int i  = 0; i < 6; i++)
    {
        painter.setPen(m_fundChangeData.fundSet[i].numColor);
        numRc.setRect((nAvgWidth - fontMetrics.width(m_fundChangeData.fundSet[i].strValue)) / 2+ nAvgWidth*i,  \
                      nDrawingHeight -unitHeight * m_fundChangeData.fundSet[i].value -9 - fontMetrics.height()\
                      ,fontMetrics.width(m_fundChangeData.fundSet[i].strValue), fontMetrics.height());
        painter.drawText(numRc, m_fundChangeData.fundSet[i].strValue);
    }
}


FundChangeChartWidget::FundChangeChartWidget(QWidget *parent):
    BaseWidget(parent),
    m_titleLabel(new QLabel("大盘主力净流入（亿）")),
    m_nReq(INVALID_REQID)
{
    createWg();
}

void FundChangeChartWidget::afterActive()
{
    startSub();
}

void FundChangeChartWidget::afterDeactive()
{
    cancelSub();
}

void FundChangeChartWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(52, 18, 30, 39);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignTop);
    mainLayout->addSpacing(50);
    m_fundChangeChart = new FundChangeChart;
    mainLayout->addWidget(m_fundChangeChart);

    this->setLayout(mainLayout);

}

void FundChangeChartWidget::startSub()
{
    ReqOptionalFundInfo req;
    req.stock_code.push_back("sh000001");
    req.stock_code.push_back("sz399001");
    req.stock_code.push_back("sz399006");
    req.stock_code.push_back("sz399005");
    req.stock_code.push_back("sh000300");
    req.stock_code.push_back("sh000016");
    req.mask = MoneyField_Mask;

    OnMoneyArrived fun = std::bind(&FundChangeChartWidget::onDataArrived, this, std::placeholders::_1, std::placeholders::_2,\
                                   std::placeholders::_3,std::placeholders::_4);

    m_nReq = gHZDataController->subOptionalMoney(req, fun);
}

void FundChangeChartWidget::cancelSub()
{
    if(m_nReq != INVALID_REQID)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

void FundChangeChartWidget::onDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    if(0 != errCode)
        return;

    m_fundChangeChart->updateData(data, num);
//    for(int i = 0; i < num; i++)
//    {
//        StockMoney *money = (StockMoney*)&data[i];
//    }
}



