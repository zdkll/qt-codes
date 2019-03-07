#include "financemarster.h"
#include <QPainter>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QtMath>
#include <QFile>
#include <QVBoxLayout>
#include <QStyleOption>
#include <hzcfg.h>
#include <hzinfodata.h>
#include "ui_financemarster.h"
#include "hzfinancestyleinfo.h"

#define RIGHT_TEXT_WIDTH 55     //文本显示宽度
#define RECT_WIDTH 45           //rect本身宽度
#define GRAPH_LEFT_OFFSET 20    //图形左边距离
#define GRAPH_RECT_SPACE 13     //相邻rect的距离
#define SIM_VAL 1e+8
#define TEXT_HUNDREDS_MILLIONS QStringLiteral("亿")

#define UNITI_YUAN QStringLiteral("元")
#define UNITI_YI QStringLiteral("亿")

static QString ASSERT_VALUE(double value, int flags=0)
{
    if(value == INVALID_DOUBLE) return "";
    if(flags == 0){
        return QString::asprintf("%0.2f", value/1e8);
        //return QString::number(value/1e8,'f',2);
    }

    if(flags == 1){
        return QString::asprintf("%.2f", value+0.0001);
    }
    //        return QString::number(value,'f',2);
}

HZFinanceMarster::HZFinanceMarster(QWidget *parent) :
    BaseWidget(parent)
  ,ui(new Ui::HZFinanceMarster)
  ,m_financeIncomeGraph(new HZFInanceLabel)
  ,m_title(new QLabel(QStringLiteral("  营业收入")))
  ,m_profitTitle(new QLabel(QStringLiteral("  利润表")))
  ,m_overView(new QLabel), m_profitOverView(new QLabel)
  ,m_descript(new HZFInanceDesLabel(true))
  ,m_profitDescript(new HZFInanceDesLabel(false))
  ,m_profitGraph(new HZFinanceProfitGraphic)
  ,m_keyIndicate(new HZFinanceKeyIndicator)
  ,m_profitList(new HZFinanceProfitText)
  ,m_balanceSheet(new HZFinanceBalanceSheetText)
  ,m_cashFlow(new HZFinanceCashFlowText)
{
    ui->setupUi(this);

    m_incomeFrame = new QFrame;
    m_profitFrame = new QFrame;

    QVBoxLayout *vILayout = new QVBoxLayout;
    vILayout->setContentsMargins(0,8,0,0);
    vILayout->setSpacing(8);
    vILayout->addWidget(m_overView);
    vILayout->addWidget(m_descript);
    vILayout->addWidget(m_financeIncomeGraph);
    vILayout->setStretch(2,1);
    vILayout->setStretch(1,0);
    vILayout->setStretch(0,0);
    m_incomeFrame->setLayout(vILayout);

    QVBoxLayout *vPLayout = new QVBoxLayout;
    vPLayout->setContentsMargins(0,8,0,0);
    vPLayout->setSpacing(8);
    vPLayout->addWidget(m_profitOverView);
    vPLayout->addWidget(m_profitDescript);
    vPLayout->addWidget(m_profitGraph);
    vPLayout->setStretch(2,1);
    vPLayout->setStretch(1,0);
    vPLayout->setStretch(0,0);
    m_profitFrame->setLayout(vPLayout);

    setObjectName("HZFinanceMarster");
    m_title->setObjectName("HZFinanceTitle");
    m_title->setFixedHeight(26);
    m_profitTitle->setFixedHeight(26);
    m_profitTitle->setObjectName("HZProfitTitle");
    m_overView->setObjectName("HZIncomeOverview");
    m_profitOverView->setObjectName("HZProfitOverView");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    layout->addWidget(m_title);
    layout->addWidget(m_incomeFrame);

    layout->addWidget(m_profitTitle);
    layout->addWidget(m_profitFrame);

    layout->addWidget(m_keyIndicate);
    layout->addWidget(m_profitList);
    layout->addWidget(m_balanceSheet);
    layout->addWidget(m_cashFlow);

    layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,0);
    layout->setStretch(3,0);
    layout->setStretch(4,0);
    ui->verticalLayout->addLayout(layout);

    loadSkin();
    //reqFinanceInfo(); //这里不需要请求 外面会调用active
}

HZFinanceMarster::~HZFinanceMarster()
{
    delete ui;
}

void HZFinanceMarster::reqFinanceInfo()
{
    if(m_code.isEmpty()) return;
    auto fun = std::bind(&HZFinanceMarster::financeCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_reqId = HzInfoCenter::getInstance()->reqFinanceInfo(m_code, fun);//sh601005 sh601688  sh600623 sh600000
}

void HZFinanceMarster::setObj(const QString &obj)
{
    m_code = obj;
}

void HZFinanceMarster::afterActive()
{
    reqFinanceInfo();
}

void HZFinanceMarster::afterDeactive()
{
    HzInfoCenter::getInstance()->cancle(m_reqId);
}

void HZFinanceMarster::OnStockCodeChange()
{
    m_code = m_stockCode;
}

void HZFinanceMarster::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void HZFinanceMarster::loadSkin()
{
}


void HZFinanceMarster::financeCallBack(const InfoRet &ret, const FinanceInfo &info)
{
    if(ret.code == -1) {
        qDebug() << "requst failed!" << ret.message;
        return;
    }

    m_overView->setText("  "+info.taking.overview);
    m_profitOverView->setText("  "+info.profit.overview);
    m_financeIncomeGraph->setFinancialInfo(info.taking);
    m_profitGraph->updateProfits(info.profit.chartData);
    m_keyIndicate->updateIndicates(info.keyIndex);
    m_profitList->updateProfit(info.profit);
    m_balanceSheet->updateProfit(info.balanceSheet);
    m_cashFlow->updateProfit(info.cashFlow);
    update();
}


/**
 * @brief LabelAxis::LabelAxis
 * @param parent
 */
HZFInanceLabel::HZFInanceLabel(QWidget *parent)
    :QLabel(parent)
{
    setFixedHeight(200);
}

void HZFInanceLabel::setFinancialInfo(const FinanceTaking &datas)
{
    m_leMax = m_rtMax = -qInf();
    m_leMin = m_rtMin = qInf();
    m_quarters.clear();
    m_growths.clear();
    m_quarterCnts.clear();
    m_years.clear();

    if(datas.dataList.isEmpty()) {
        return;
    }

    m_leMax = m_rtMax = -qInf();
    m_leMin = m_rtMin = qInf();
    m_quarters.clear();
    m_growths.clear();
    m_quarterCnts.clear();
    m_years.clear();

    //计算每年的最大值
    auto CalCountVal = [&](const Q_VECTOR_LIST &lst)->double{
#if 0
        for(int i=lst.count()-1; i>=0; --i){
            if(lst[i] != 0 && lst[i]>lst[i-1])
                return lst[i]/SIM_VAL;
            else if(i==0) {
                return lst[0]/SIM_VAL;
            }
        }
#else
        Q_VECTOR_LIST tmpLst(lst);
        qSort(tmpLst.begin(), tmpLst.end());
        return tmpLst.last()/SIM_VAL;
#endif
        return 0;
    };

    //去除单位
    auto VecMultiYiFun = [&](const Q_VECTOR_LIST &lst)->Q_VECTOR_LIST{
        Q_VECTOR_LIST multiYis;
        for(int i=0; i<lst.count(); ++i) {
            multiYis.append(lst[i]/SIM_VAL);
        }
        return multiYis;
    };

    for(const FinanceTaking::FinanceTakingItem &item : datas.dataList) {
        m_quarterCnts[item.year] = CalCountVal(item.quarterVec);
        m_quarters[item.year] = VecMultiYiFun(item.quarterVec);
        m_years << item.year;
        m_growths << item.yearOnYearGrowth;
    }

    QMapIterator<QString,double> it(m_quarterCnts);
    while(it.hasNext()){
        it.next();
        m_rtMax = qMax(m_rtMax, it.value());
        m_rtMin = qMin(m_rtMin, it.value());
    }

    if(m_growths.isEmpty()) return;
    Q_VECTOR_LIST tmpGrowths = m_growths;
    qSort(tmpGrowths.begin(), tmpGrowths.end());
    m_leMax = tmpGrowths.last(), m_leMin = tmpGrowths.first();
}

void HZFInanceLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    drawText(&painter);
    drawRect(&painter);
    drawLine(&painter);
}

void HZFInanceLabel::drawLine(QPainter *painter)
{
    if(m_growths.isEmpty() || m_leMax == -qInf() || m_leMax == qInf()) return;
    const int width = mRightMargin-(RECT_WIDTH/2+5),
            pointy = height()-20,
            n = m_growths.count();
    const double fonth = painter->fontMetrics().height(),
            maxPx = 0,
            minPx = pointy;
    double xPos=0.,
            xPosPre=0.,
            yearsLenHalf=0.,
            px=0., py=0.,
            pxAdd=0., pyAdd=0.;
    painter->setRenderHint(QPainter::Antialiasing, true);

    for(int i=0; i<n-1; ++i) {
        xPos = width - 6 - i*(RECT_WIDTH+GRAPH_RECT_SPACE);
        xPosPre =  width - 6 - (i+1)*(RECT_WIDTH +GRAPH_RECT_SPACE);
        yearsLenHalf = painter->fontMetrics().width(m_years[i])/2;
        px = (m_leMax-m_growths[i])/(m_leMax-m_leMin)*pointy;
        py = (px-fonth)<=maxPx?px+fonth:(px+fonth)>=minPx?px-fonth:px;
        pxAdd = (m_leMax-m_growths[i+1])/(m_leMax-m_leMin)*pointy;
        pyAdd = (pxAdd-fonth)<=maxPx?pxAdd+fonth:(pxAdd+fonth)>=minPx?pxAdd-fonth:pxAdd;

        painter->setPen(gHZFinanceStyle->incomeEllipsePen());
        painter->drawLine(xPos, py, xPosPre, pyAdd);
        painter->setBrush(gHZFinanceStyle->incomeEllipseBrush());
        painter->drawEllipse(QPointF(xPos, py), 5., 5.);
        painter->setFont(gHZFinanceStyle->mXAxisTxFont);
        drawTextPrivate(painter, gHZFinanceStyle->xTicksPen(), Qt::AlignCenter, QRect(xPos-yearsLenHalf, pointy, yearsLenHalf*2, fonth), m_years[i]);
    }

    px =(m_leMax-m_growths[n-1])/(m_leMax-m_leMin)*pointy;
    py = (px-fonth)<=maxPx?px+fonth:(px+fonth)>=minPx?px-fonth:px;
    yearsLenHalf = painter->fontMetrics().width(m_years[n-1])/2;
    xPos = width - 6 - (n-1)*(RECT_WIDTH+GRAPH_RECT_SPACE);

    QRect yearTxtRt(xPos-yearsLenHalf, pointy, yearsLenHalf*2, fonth);
    painter->setPen(gHZFinanceStyle->incomeEllipsePen());
    painter->setBrush(gHZFinanceStyle->incomeEllipseBrush());
    painter->drawEllipse(QPointF(xPos, py), 5.,5.);

    //x axis text
    painter->setFont(gHZFinanceStyle->mXAxisTxFont);
    drawTextPrivate(painter, gHZFinanceStyle->xTicksPen(), Qt::AlignCenter, yearTxtRt, m_years[n-1]);
}

void HZFInanceLabel::drawRect(QPainter *p)
{
    if(m_quarterCnts.isEmpty()|| m_quarters.isEmpty() || m_rtMax==-qInf() || m_rtMin==qInf()) return;
    const int width = mRightMargin-(RECT_WIDTH+GRAPH_RECT_SPACE)/2,
            fonth = p->fontMetrics().height(),
            pointy=height()-20;

    p->setRenderHint(QPainter::Antialiasing, false);
    p->setPen(gHZFinanceStyle->baseLinePen());
    p->drawLine(0, pointy, width+19, pointy);

    //增加容错处理
    for(int k=0; k<m_quarters.count(); ++k) {
        double textLen = RECT_WIDTH/2, xPos = width - k*(RECT_WIDTH+GRAPH_RECT_SPACE) - 6;
        quint32 pillarHeight = m_quarterCnts[m_years[k]]*(pointy-20)/m_rtMax, leftUpY = pointy - pillarHeight;
        Q_VECTOR_LIST curQuaters = m_quarters[m_years[k]], tmpQuats=curQuaters;
        qSort(tmpQuats.begin(), tmpQuats.end());
        double ratio = qreal(pointy-leftUpY)/tmpQuats.last(),
                xval = xPos-textLen,
                yval = leftUpY, rtW = textLen*2;
        QString text(QString::number(m_quarterCnts[m_years[k]],'f',2));
        int txLenHalf = p->fontMetrics().width(text+TEXT_HUNDREDS_MILLIONS)/2;

        //display tip text
        p->save();
        p->setFont(gHZFinanceStyle->mTipTxFont);
        drawTipText(p, gHZFinanceStyle->textPen(), QRectF(xPos-txLenHalf, leftUpY-fonth, txLenHalf*2, fonth),text);
        p->restore();

        double pillar3 = (curQuaters[3]-curQuaters[2])*ratio,
                pillar2 = (curQuaters[2]-curQuaters[1])*ratio,
                pillar1 = (curQuaters[1]-curQuaters[0])*ratio,
                pillar0 = (curQuaters[0])*ratio;
        bool vf[4] = {pillar0>0,pillar1>0,pillar2>0,pillar3>0};
        QRectF mrt(xval, yval, rtW, pillarHeight);

        if(vf[3]) {//第四季度
            int secPos = 0;
            drawRectPrivate(p,
                            gHZFinanceStyle->quarterPens().at(3),
                            gHZFinanceStyle->quarterBrushs().at(3),
                            mrt);
            secPos += pillar3;
            if(vf[2]) {
                mrt.setY(yval+secPos);
                mrt.setBottom(pointy);
                drawRectPrivate(p,
                                gHZFinanceStyle->quarterPens().at(2),
                                gHZFinanceStyle->quarterBrushs().at(2),
                                mrt);
                secPos += pillar2;
            }else {
                pillar3 = (curQuaters[3]-curQuaters[1])*ratio;
                secPos = pillar3;
            }

            if(vf[1]){
                mrt.setY(yval+secPos);
                mrt.setBottom(pointy);
                drawRectPrivate(p,
                                gHZFinanceStyle->quarterPens().at(1),
                                gHZFinanceStyle->quarterBrushs().at(1),
                                mrt);
                secPos += pillar1;
            }else {
                //为下一季度值计算位置（当前季度值有效，并且进入到第二季度时，正好第二季度值无效，故3-0，计算出第一季度的相对位置）
                pillar3 = (curQuaters[3]-curQuaters[0])*ratio;
                secPos = pillar3;
            }

            if(vf[0]) {
                mrt.setY(yval+secPos);
                mrt.setBottom(pointy);
                drawRectPrivate(p,
                                gHZFinanceStyle->quarterPens().at(0),
                                gHZFinanceStyle->quarterBrushs().at(0),
                                mrt);
            }
        }else if(vf[2]) {//第3季度
            int secPos = 0;
            drawRectPrivate(p,
                            gHZFinanceStyle->quarterPens().at(2),
                            gHZFinanceStyle->quarterBrushs().at(2),
                            mrt);
            secPos += pillar2;
            if(vf[1]){
                mrt.setY(yval+secPos);
                mrt.setBottom(pointy);
                drawRectPrivate(p,
                                gHZFinanceStyle->quarterPens().at(1),
                                gHZFinanceStyle->quarterBrushs().at(1),
                                mrt);
                secPos += pillar1;
            }else {
                //同理
                pillar2 = (curQuaters[2]-curQuaters[0])*ratio;
                secPos = pillar2;
            }

            if(vf[0]) {
                mrt.setY(yval+secPos);
                mrt.setBottom(pointy);
                drawRectPrivate(p,
                                gHZFinanceStyle->quarterPens().at(0),
                                gHZFinanceStyle->quarterBrushs().at(0),
                                mrt);
            }

        }else if(vf[1]) {//第2季度
            int secPos = 0;
            drawRectPrivate(p,
                            gHZFinanceStyle->quarterPens().at(1),
                            gHZFinanceStyle->quarterBrushs().at(1),
                            mrt);
            secPos += pillar1;
            if(vf[0]) {
                mrt.setY(yval+secPos);
                mrt.setBottom(pointy);
                drawRectPrivate(p,
                                gHZFinanceStyle->quarterPens().at(0),
                                gHZFinanceStyle->quarterBrushs().at(0),
                                mrt);
            }
        }else if(vf[0]) {//第1季度
            drawRectPrivate(p,
                            gHZFinanceStyle->quarterPens().at(0),
                            gHZFinanceStyle->quarterBrushs().at(0),
                            mrt);
        }
    }
}

void HZFInanceLabel::drawText(QPainter *painter)
{
    if(m_growths.isEmpty()||m_leMax == -qInf() || m_leMin == qInf()) return;
    auto TextFormatFun = [=](double val)->QString{return QString("%1%").arg(val,0,'f',2);};
    auto TextWidthFun = [=](double val)->double{return painter->fontMetrics().width(QString("%1%").arg(val,0,'f',2));};

    const int fonth = painter->fontMetrics().height(), pointy=height()-20;
    const QVector<double> vTexts = {qCeil(m_leMax)*1., (m_leMin+m_leMax)/2., qFloor(m_leMin)*1.};
    QVector<double> tmpWidths={TextWidthFun(vTexts[0]), TextWidthFun(vTexts[1]),TextWidthFun(vTexts[2])};
    qSort(tmpWidths.begin(), tmpWidths.end());
    double leftPos = tmpWidths.last(), maxWidth=leftPos;//取y轴最大文本宽度
    mRightMargin = width() - leftPos - 1;//右边显示y轴文本

    const QVector<QRectF> rts = {
        QRectF(mRightMargin+2, fonth, maxWidth, fonth),
        QRectF(mRightMargin+2, (m_leMax-vTexts[1])/(m_leMax-m_leMin)*pointy, maxWidth, fonth),
        QRectF(mRightMargin+2, pointy-fonth, maxWidth, fonth)
    };

    painter->save();
    painter->setFont(gHZFinanceStyle->mYAxisTxFont);
    for(int i=0;i<3;++i){
        drawTextPrivate(painter, gHZFinanceStyle->yTicksPen(), Qt::AlignVCenter|Qt::AlignLeft, rts[i], TextFormatFun(vTexts[i]));
    }
    painter->restore();
}

void HZFInanceLabel::drawTipText(QPainter *p, const QPen &pen, const QRectF &rt, const QString &text)
{
    p->setPen(pen);
    p->drawText(rt, Qt::AlignCenter, text);
}

void HZFInanceLabel::drawRectPrivate(QPainter *painter, const QPen &pen, const QBrush &brush, const QRectF &rect)
{
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(rect);
}

void HZFInanceLabel::drawTextPrivate(QPainter *painter, const QPen &pen, int flags, const QRectF &rect, const QString &text)
{
    painter->setPen(pen);
    painter->drawText(rect, flags, text);
}


/***********************************************************************************************
 *
 *  HZFinanceKeyIndicator 关键指标列表
 *
 * *********************************************************************************************/
HZFInanceDesLabel::HZFInanceDesLabel(bool flags, QWidget *parent)
    :QLabel(parent)
    ,m_isIncome(flags)
{
    this->setMinimumHeight(27);
}


void HZFInanceDesLabel::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    QPainter painter(this);
    painter.translate(15,0);
    painter.setRenderHint(QPainter::Antialiasing, false);
    QPen textPen(QColor("#858585"));
    int iw = 3, ih = 10;
    QString incomeText = m_isIncome?QStringLiteral("营业收入(亿元)"):QStringLiteral("净利润(亿元)");
    QString quarterText = QStringLiteral("1-4季度");
    QString increatText = QStringLiteral("同比增长(%)");
    int text1Len = painter.fontMetrics().width(incomeText);
    int text2Len = painter.fontMetrics().width(quarterText);
    painter.setPen(QColor("#1669B9"));
    painter.setBrush(QColor("#1A4C81"));
    QPainterPath path;
    path.moveTo(0,16);
    path.lineTo(8,16);
    path.addRect(2,6,4,10);
    painter.drawPath(path);
    painter.setPen(textPen);
    painter.drawText(16, 16, incomeText);

    if(m_isIncome){
        const static QVector<QRect> rts = {
            QRect{24+text1Len,7, iw,ih},
            QRect{28+text1Len,7, iw,ih},
            QRect{32+text1Len,7, iw,ih},
            QRect{36+text1Len,7, iw,ih}
        };

        for(int i=0; i<4; ++i) {
            painter.setPen(gHZFinanceStyle->quarterPens().at(i));
            painter.setBrush(gHZFinanceStyle->quarterBrushs().at(i));
            painter.drawRect(rts[i]);
        }

        painter.save();
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(textPen);
        painter.drawText(45+text1Len, 16, quarterText);
        painter.setPen(QPen(QColor("#ffa300"),2));
        painter.setBrush(QColor("#191919"));
        painter.drawEllipse(54+text1Len+text2Len, height()/2-4, 7, 7);
        painter.setPen(textPen);
        painter.drawText(66+text1Len+text2Len, 16, increatText);
        painter.restore();
    } else {
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor("#191919"));
        painter.setPen(QPen(QColor("#529efc"),2));
        painter.drawEllipse(23+text1Len, height()/2-4, 7, 7);
        painter.setPen(textPen);
        painter.drawText(35+text1Len, 16, increatText);
        painter.restore();
    }
}




/***********************************************************************************************
 *
 *  HZFinanceKeyIndicator 关键指标列表
 *
 * *********************************************************************************************/
HZFinanceKeyIndicator::HZFinanceKeyIndicator(QWidget *parent)
    :QLabel(parent)
{
    setMinimumHeight(m_indicats.count()*m_lineH+m_titleH);
    setObjectName("HZFinanceKeyIndicator");
}

void HZFinanceKeyIndicator::updateIndicates(const FinanceKeyindex &indicate)
{
    m_titleText = indicate.title;
    QVector<QPair<QString,QString> > indicates;
    indicates.append(qMakePair(QStringLiteral("每股收益"), ASSERT_VALUE(indicate.earningsPerShare,1)+UNITI_YUAN));
    indicates.append(qMakePair(QStringLiteral("每股净资产"), ASSERT_VALUE(indicate.netAssetValuePreShare,1)+UNITI_YUAN));
    indicates.append(qMakePair(QStringLiteral("净资产收益率(ROE)"), ASSERT_VALUE(indicate.roe,1)+"%"));
    indicates.append(qMakePair(indicate.eps, indicate.epsNum));//该季度每股收益
    //    for(const FinanceKeyindex::EarningsPerShareChart &per:indicate.earningsPerShareChart){
    //        indicates.append(qMakePair(per.year, QString::number(per.earnings,'f',2)));
    //    }
    updateIndicates(indicates);
}



void HZFinanceKeyIndicator::updateIndicates(const QVector<QPair<QString, QString> > &indicates)
{
    m_indicats = indicates;
    setMinimumHeight(m_indicats.count()*m_lineH+m_titleH+15);
}

void HZFinanceKeyIndicator::paintEvent(QPaintEvent *)
{
    if(m_indicats.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    double pointx = 14., pointy = 3., gw=width()-7, gh = height();
    painter.setPen(QColor("#4f98f1"));
    QRectF rt(QPointF(pointx, pointy), QPointF(gw, m_titleH));
    painter.drawText(rt, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("关键指标"));

    painter.setPen(QColor("#858585"));
    painter.drawText(rt, Qt::AlignRight | Qt::AlignVCenter, m_titleText);

    painter.setPen(QColor("#2c2c2c"));
    rt.adjust(0.,4.,0,0);
    painter.setPen(QColor("#2f2f2f"));
    painter.drawLine(rt.bottomLeft(), rt.bottomRight());

    pointy = m_titleH+8;
    painter.setPen(QColor("#cbd4db"));
    for(int i=0; i<m_indicats.count(); ++i){
        QRectF rt(pointx, pointy + i*(m_lineH), gw-14, m_lineH);
        painter.setFont(gHZFinanceStyle->mTextFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignLeft, m_indicats[i].first);
        painter.setFont(gHZFinanceStyle->mNumberTxFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignRight, m_indicats[i].second);
    }
}


/***********************************************************************************************
 *
 *  HZFinanceProfitGraphic 利润图例
 *
 * *********************************************************************************************/
HZFinanceProfitGraphic::HZFinanceProfitGraphic(QWidget *parent)
    :QLabel(parent)
    ,m_rtMax(-qInf())
    ,m_rtMin(qInf())
    ,m_leMax(-qInf())
    ,m_leMin(qInf())
{
    setMinimumHeight(200);
    setObjectName("HZFinanceProfitGraphic");
}

void HZFinanceProfitGraphic::updateProfits(const QVector<FinanceProfit::ProfitChartData> &profits)
{
    m_years.clear();
    m_ups.clear();
    m_profits.clear();

    if(profits.isEmpty())
        return;

    for(const FinanceProfit::ProfitChartData &cd : profits) {
        m_profits << cd.retainedProfit/SIM_VAL;
        m_ups << cd.retainedProfitsUp;
        m_years << cd.year;
    }

    QVector<double> tmpUps = m_ups, tmpProfits = m_profits;
    qSort(tmpUps.begin(), tmpUps.end());
    qSort(tmpProfits.begin(), tmpProfits.end());
    m_rtMax = tmpProfits.last(), m_rtMin = tmpProfits.first();
    m_leMax = tmpUps.last() , m_leMin = tmpUps.first();
    m_rtSecMin = tmpProfits[1];
}

void HZFinanceProfitGraphic::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    QPainter painter(this);
    drawText(&painter);
    drawRect(&painter);
    drawLine(&painter);
}

void HZFinanceProfitGraphic::drawLine(QPainter *painter)
{
    if(m_ups.isEmpty() || m_leMax == -qInf() || m_leMax == qInf()) return;
    const int width = mRightMargin-(RECT_WIDTH/2+5), pointy = height()-20, n = m_ups.count();
    double maxPx = 0, minPx = pointy, fonth = painter->fontMetrics().height();
    double xPos=0., xPosPre=0., yearsLenHalf=0.,
            px=0., py=0., pxAdd=0., pyAdd=0.;

    painter->setRenderHint(QPainter::Antialiasing, true);
    for(int i=0; i<n-1; ++i) {
        xPos = width - 6 - i*(RECT_WIDTH+GRAPH_RECT_SPACE);
        xPosPre =  width - 6 - (i+1)*(RECT_WIDTH +GRAPH_RECT_SPACE);
        yearsLenHalf = painter->fontMetrics().width(m_years[i])/2;
        px = (m_leMax-m_ups[i])/(m_leMax-m_leMin)*pointy,
                pxAdd = (m_leMax-m_ups[i+1])/(m_leMax-m_leMin)*pointy;
        py = (px-fonth)<=maxPx?px+fonth:(px+fonth)>=minPx?px-fonth:px,
                pyAdd = (pxAdd-fonth)<=maxPx?pxAdd+fonth:(pxAdd+fonth)>=minPx?pxAdd-fonth:pxAdd;
        painter->setPen(gHZFinanceStyle->xTicksPen());
        painter->setFont(gHZFinanceStyle->mXAxisTxFont);
        painter->drawText(QRect(xPos-yearsLenHalf, pointy, yearsLenHalf*2, fonth), Qt::AlignCenter, m_years[i]);
        painter->setPen(gHZFinanceStyle->profitLinePen());
        painter->drawLine(xPos, py, xPosPre, pyAdd);
        painter->setBrush(gHZFinanceStyle->profitEllipseBrush());
        painter->setPen(gHZFinanceStyle->profitEllipsePen());
        painter->drawEllipse(QPointF(xPos, py), 5., 5.);
    }

    px =(m_leMax-m_ups[n-1])/(m_leMax-m_leMin)*pointy;
    py = (px-fonth)<=maxPx?px+fonth:(px+fonth)>=minPx?px-fonth:px;
    yearsLenHalf = painter->fontMetrics().width(m_years[n-1])/2;
    xPos = width - 6 - (n-1)*(RECT_WIDTH+GRAPH_RECT_SPACE);
    painter->setPen(gHZFinanceStyle->profitEllipsePen());
    painter->setBrush(gHZFinanceStyle->profitRectBrush());
    painter->drawEllipse(QPointF(xPos, py), 5., 5.);
    painter->setPen(gHZFinanceStyle->xTicksPen());
    painter->setFont(gHZFinanceStyle->mXAxisTxFont);
    painter->drawText(QRect(xPos-yearsLenHalf, pointy, yearsLenHalf*2, fonth), Qt::AlignCenter, m_years[n-1]);
}

void HZFinanceProfitGraphic::drawRect(QPainter *painter)
{
    if(m_profits.isEmpty() || m_rtMax==-qInf() || m_rtMin==qInf()) return;
    painter->save();
    if(m_rtMin>0) m_rtMin = 0;
    const int width = mRightMargin-(RECT_WIDTH+GRAPH_RECT_SPACE)/2,
            fonth = painter->fontMetrics().height(), pointy=height()-40;
    double origin = m_rtMin<0.?m_rtMax/(m_rtMax-m_rtMin)*pointy:pointy;

    painter->translate(0,fonth);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(gHZFinanceStyle->baseLinePen());
    painter->drawLine(0, origin, width+19, origin);
    for(int k=0; k<m_profits.count(); ++k) {
        QString text = QString::number(m_profits[k],'f',2);
        double textLen = RECT_WIDTH/2, xPos = width - 6 - k*(RECT_WIDTH+GRAPH_RECT_SPACE),pixel;
        bool isMinMax(m_profits[k]>0);
        pixel = (m_rtMax-m_profits[k])/(m_rtMax-m_rtMin)*(pointy);//+fonth;
        painter->setPen(gHZFinanceStyle->profitRectPen());
        painter->setBrush(gHZFinanceStyle->profitRectBrush());
        painter->drawRect(QRectF(QPointF(xPos-textLen, pixel), QPointF(xPos+textLen, origin)));
        int txLenHalf = painter->fontMetrics().width(text+TEXT_HUNDREDS_MILLIONS)/2;
        painter->setPen(gHZFinanceStyle->textPen());
        painter->setFont(gHZFinanceStyle->mTipTxFont);
        painter->drawText(QRectF(xPos-txLenHalf, isMinMax?pixel-fonth:pixel, txLenHalf*2, fonth), Qt::AlignCenter, text);
    }
    painter->restore();
}

void HZFinanceProfitGraphic::drawText(QPainter *painter)
{
    if(m_profits.isEmpty()||m_leMax == -qInf() || m_leMin == qInf()) return;
    auto TextFormatFun = [=](double val)->QString{return QString("%1%").arg(val,0,'f',2);};
    auto TextWidthFun = [=](double val)->double{return painter->fontMetrics().width(QString("%1%").arg(val,0,'f',2));};
    const int fonth = painter->fontMetrics().height(), pointy=height()-20;
    QVector<double> vTexts = {qCeil(m_leMax)*1., (qCeil(m_leMax)*1.+qFloor(m_leMin)*1.)/2., qFloor(m_leMin)*1.};
    QVector<double> tmpWidths={TextWidthFun(vTexts[0]), TextWidthFun(vTexts[1]),TextWidthFun(vTexts[2])};
    qSort(tmpWidths.begin(), tmpWidths.end());
    double leftPos = tmpWidths.last(), maxWidth=leftPos;//取y轴最大文本宽度
    mRightMargin = width() - leftPos - 1;//右边显示y轴文本

    const QVector<QRectF> rts = {
        QRectF(mRightMargin+2, fonth, maxWidth, fonth),
        QRectF(mRightMargin+2, (m_leMax-vTexts[1])/(m_leMax-m_leMin)*pointy, maxWidth, fonth),
        QRectF(mRightMargin+2, pointy-fonth, maxWidth, fonth)
    };


    painter->setFont(gHZFinanceStyle->mYAxisTxFont);
    painter->setPen(gHZFinanceStyle->yTicksPen());
    for(int i=0;i<3;++i){
        painter->drawText(rts[i], Qt::AlignVCenter|Qt::AlignLeft, TextFormatFun(vTexts[i]));
    }
}



/***********************************************************************************************
 *
 *  HZFinanceProfitText 利润列表
 *
 * *********************************************************************************************/
HZFinanceProfitText::HZFinanceProfitText(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("HZFinanceProfitText");
    setMinimumHeight(m_profits.count()*m_lineH+m_titleH);
}

void HZFinanceProfitText::updateProfit(const FinanceProfit &profit)
{
    m_titleText = profit.title.isEmpty()?QStringLiteral("暂无数据"):profit.title;
    QVector<QPair<QString,QString>> profits;
    profits.append(qMakePair(QStringLiteral("营业总收入"),           ASSERT_VALUE(profit.grossRevenus)+UNITI_YI));
    profits.append(qMakePair(QStringLiteral("营业利润"),            ASSERT_VALUE(profit.operatingProfit)+UNITI_YI));
    profits.append(qMakePair(QStringLiteral("净利润"),             ASSERT_VALUE(profit.retainedProfits)+UNITI_YI));
    profits.append(qMakePair(QStringLiteral("营业总收入同比增长"),   ASSERT_VALUE(profit.grossRevenUp,1)+"%"));
    profits.append(qMakePair(QStringLiteral("营业利润同比增长"),    ASSERT_VALUE(profit.operatinProfitUp,1)+"%"));
    profits.append(qMakePair(QStringLiteral("净利润同比增长"),         ASSERT_VALUE(profit.retainedProfitsUp,1)+"%"));
    updateProfit(profits);
}

void HZFinanceProfitText::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    if(m_profits.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    double pointx = 14., pointy = 3., gw=width()-7, gh = height();
    painter.setPen(QColor("#4e98f7"));
    QRectF rt(QPointF(pointx, pointy), QPointF(gw, m_titleH));
    painter.drawText(rt, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("利润表"));
    painter.setPen(QColor("#858585"));
    painter.drawText(rt, Qt::AlignRight |Qt::AlignVCenter, m_titleText);
    rt.adjust(0.,4.,0,0);
    painter.setPen(QColor("#2f2f2f"));
    painter.drawLine(rt.bottomLeft(), rt.bottomRight());

    pointy = m_titleH+8;
    painter.setPen(QColor("#cbd4db"));
    for(int i=0; i<m_profits.count(); ++i){
        QRectF rt(pointx, pointy + i*(m_lineH), gw-14, m_lineH);
        painter.setFont(gHZFinanceStyle->mTextFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignLeft, m_profits[i].first);
        painter.setFont(gHZFinanceStyle->mNumberTxFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignRight, m_profits[i].second);
    }
}

void HZFinanceProfitText::updateProfit(const QVector<QPair<QString, QString> > &profits)
{
    m_profits = profits;
    setMinimumHeight(m_profits.count()*m_lineH+m_titleH+15);
}

////
/// \brief HZFinanceBalanceSheetText::updateProfit
/// \param profit
///
HZFinanceBalanceSheetText::HZFinanceBalanceSheetText(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("HZFinanceProfitText");
    setMinimumHeight(m_profits.count()*m_lineH+m_titleH);
}

void HZFinanceBalanceSheetText::updateProfit(const FinanceBalanceSheet &sheet)
{
    m_titleText = sheet.title.isEmpty()?QStringLiteral("暂无数据"):sheet.title;
    QVector<QPair<QString,QString>> profits;
    profits.append(qMakePair(QStringLiteral("资产总计"),    ASSERT_VALUE(sheet.totalAssets)+UNITI_YI+UNITI_YUAN));
    profits.append(qMakePair(QStringLiteral("负债总计"),    ASSERT_VALUE(sheet.totalLiabilities)+UNITI_YI+UNITI_YUAN));
    profits.append(qMakePair(QStringLiteral("所有权益总计"), ASSERT_VALUE(sheet.totalOwnersEquity)+UNITI_YI+UNITI_YUAN));
    profits.append(qMakePair(QStringLiteral("资产负债率"), ASSERT_VALUE(sheet.assetLiabilityRatio,1)+"%"));
    updateProfit(profits);
}

void HZFinanceBalanceSheetText::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    if(m_profits.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    double pointx = 14., pointy = 3., gw=width()-7, gh = height();
    painter.setPen(QColor("#4e98f7"));
    QRectF rt(QPointF(pointx, pointy), QPointF(gw, m_titleH));
    painter.drawText(rt, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("资产负债表"));
    painter.setPen(QColor("#858585"));
    painter.drawText(rt, Qt::AlignRight |Qt::AlignVCenter, m_titleText);
    rt.adjust(0.,4.,0,0);
    painter.setPen(QColor("#2f2f2f"));
    painter.drawLine(rt.bottomLeft(), rt.bottomRight());

    pointy = m_titleH+8;
    painter.setPen(QColor("#cbd4db"));
    for(int i=0; i<m_profits.count(); ++i){
        QRectF rt(pointx, pointy + i*(m_lineH), gw-14, m_lineH);
        painter.setFont(gHZFinanceStyle->mTextFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignLeft, m_profits[i].first);
        painter.setFont(gHZFinanceStyle->mNumberTxFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignRight, m_profits[i].second);
    }
}

void HZFinanceBalanceSheetText::updateProfit(const QVector<QPair<QString, QString> > &indicates)
{
    m_profits = indicates;
    setMinimumHeight(m_profits.count()*m_lineH+m_titleH+15);
}

///
/// \brief HZFinanceCashFlowText::HZFinanceCashFlowText
/// \param parent
///
HZFinanceCashFlowText::HZFinanceCashFlowText(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("HZFinanceProfitText");
    setMinimumHeight(m_profits.count()*m_lineH+m_titleH);
}

void HZFinanceCashFlowText::updateProfit(const FinanceCashFlow &flow)
{
    m_titleText = flow.title.isEmpty()?QStringLiteral("暂无数据"):flow.title;
    QVector<QPair<QString,QString>> profits;
    profits.append(qMakePair(QStringLiteral("经营性现金流量"), ASSERT_VALUE(flow.operatingCashFlow)+UNITI_YI+UNITI_YUAN));
    profits.append(qMakePair(QStringLiteral("投资性现金净量净额"), ASSERT_VALUE(flow.netInvestmentCash)+UNITI_YI+UNITI_YUAN));
    profits.append(qMakePair(QStringLiteral("融资性现金净量净额"), ASSERT_VALUE(flow.netAmountOfFinancingCash)+UNITI_YI+UNITI_YUAN));
    updateProfit(profits);
}

void HZFinanceCashFlowText::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    if(m_profits.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    double pointx = 14., pointy = 3., gw=width()-7, gh = height();
    painter.setPen(QColor("#4e98f7"));
    QRectF rt(QPointF(pointx, pointy), QPointF(gw, m_titleH));
    painter.drawText(rt, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("现金流表"));
    painter.setPen(QColor("#858585"));
    painter.drawText(rt, Qt::AlignRight |Qt::AlignVCenter, m_titleText);
    rt.adjust(0.,4.,0,0);
    painter.setPen(QColor("#2f2f2f"));
    painter.drawLine(rt.bottomLeft(), rt.bottomRight());

    pointy = m_titleH+8;
    painter.setPen(QColor("#cbd4db"));
    for(int i=0; i<m_profits.count(); ++i){
        QRectF rt(pointx, pointy + i*(m_lineH), gw-14, m_lineH);
        painter.setFont(gHZFinanceStyle->mTextFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignLeft, m_profits[i].first);
        painter.setFont(gHZFinanceStyle->mNumberTxFont);
        painter.drawText(rt, Qt::AlignVCenter | Qt::AlignRight, m_profits[i].second);
    }
}

void HZFinanceCashFlowText::updateProfit(const QVector<QPair<QString, QString> > &indicates)
{
    m_profits = indicates;
    setMinimumHeight(m_profits.count()*m_lineH+m_titleH+15);
}
