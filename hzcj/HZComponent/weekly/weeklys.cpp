#include "weeklys.h"
#include "ui_weeklys.h"
#include <QGridLayout>
#include <QPainter>
#include <QFile>
#include <QDebug>

#include <hzinfodata.h>
#include <hzinfocenter.h>
#include <zxstockopt.h>

#define XOffset 35
#define YOffset 280

static const QString gWeekTexts[] = {
    QStringLiteral("周一"),
    QStringLiteral("周二"),
    QStringLiteral("周三"),
    QStringLiteral("周四"),
    QStringLiteral("周五"),
};


//日历栏
void WeeklyCalandarBar::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter p(this);
    p.setPen(Qt::white);

    QString text(QStringLiteral("本周周报"));
    QRect rt(rect());

    int xpos = 20;
    rt.setX(xpos);
    p.setFont(m_textFont);
    p.setPen(QColor("#ffffff"));
    p.drawText(rt, Qt::AlignVCenter, text);

    xpos = p.fontMetrics().width(text)+20;
    xpos += 5;
    rt.setX(xpos);
    p.setFont(m_numFont);
    text = QString("(%1)").arg(m_date);
    p.drawText(rt, Qt::AlignVCenter, text);

    xpos += p.fontMetrics().width(text);
    xpos += 5;

    p.setPen(Qt::black);
    p.drawLine(QPoint(xpos,height()/2-10), QPoint(xpos,height()/2+3));
    p.drawLine(QPoint(0,height()-1), QPoint(width(),height()-1));

    p.setPen(QPen(QColor("#4f9bff"),2));
    p.drawLine(QPoint(0,height()-1),QPoint(xpos, height()-1));
}

//------------------ 本周周报 start -----------------------------------------------------
void CurWeeklys::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter p(this);
    //p.fillRect(rect(), QColor("#191919"));

    drawTitle(&p);
    drawGrid(&p);
    drawXAxisAndLines(&p);
}

void CurWeeklys::drawTitle(QPainter *p)
{
    int offserY = 25, offsetX = 24, headerY = 110;
    p->setPen(Qt::black);
    p->drawLine(XOffset, headerY, width() - 60, headerY);

    int textOffsetX = offsetX, textOffsetY = offserY;
    QPoint linePos(offsetX, offserY);

    ///![1]
    QString text = QStringLiteral("本周您的自选股上涨");
    p->setFont(mTitleTextFont);
    p->setPen(Qt::white);
    p->drawText(linePos, text);
    textOffsetX += p->fontMetrics().width(text);
    textOffsetY += p->fontMetrics().height();

    text = QString::number(m_Datas.opt_up,'f',2)+"%";
    textOffsetX +=5;
    linePos.setX(textOffsetX);
    p->setPen(m_Datas.opt_up>0?QColor("#e62727"):m_Datas.opt_up<0?QColor("#0ec350"):QColor("#ffffff"));
    p->setFont(mTitleNumFont);
    p->drawText(linePos, text);
    textOffsetX += p->fontMetrics().width(text);

    text = QStringLiteral("，大盘上涨");
    textOffsetX += 2;
    linePos.setX(textOffsetX);
    p->setPen(Qt::white);
    p->setFont(mTitleTextFont);
    p->drawText(linePos, text);
    textOffsetX += p->fontMetrics().width(text);

    text = QString::number(m_Datas.index_up,'f',2)+"%";
    textOffsetX += 5;
    linePos.setX(textOffsetX);
    p->setPen(m_Datas.index_up>0?QColor("#e62727"):m_Datas.index_up<0?QColor("#0ec350"):QColor("#ffffff"));
    p->setFont(mTitleNumFont);
    p->drawText(linePos, text);
    textOffsetX += p->fontMetrics().width(text);

    ///![2]
    textOffsetX = offsetX;
    linePos.setY(textOffsetY+10);
    linePos.setX(textOffsetX);
    p->setFont(mTitleMidTextFont);
    p->setPen(QColor("#797979"));
    p->drawText(linePos, QStringLiteral("计算依据不包含：近6个交易日内上市的个股、近6个交易日内有过停牌的个股、退市的个股。"));
    textOffsetY += p->fontMetrics().height();

    ///![3]
    textOffsetX += 6;
    textOffsetY += 20;
    linePos.setX(textOffsetX);
    linePos.setY(textOffsetY);
    p->setPen(QPen(QColor("#ffa200"),2));
    p->setBrush(QColor("#191919"));
    p->drawEllipse(linePos, 5, 5);

    textOffsetX += 10;
    textOffsetY += 3;
    linePos.setX(textOffsetX);
    linePos.setY(textOffsetY);
    p->setPen(QColor("#adadad"));
    p->setFont(mTitleTextFont);
    text = QStringLiteral("自选股");
    p->setFont(mTitleTextFont);
    p->drawText(linePos,text);

    textOffsetX += p->fontMetrics().width(text);
    textOffsetY -= 3;
    linePos.setX(textOffsetX+23);
    linePos.setY(textOffsetY);
    p->setPen(QPen(QColor("#529efc"),2));
    p->drawEllipse(linePos,5,5);

    textOffsetX += 35;
    textOffsetY += 3;
    linePos.setX(textOffsetX);
    linePos.setY(textOffsetY);
    p->setPen(QColor("#adadad"));
    p->setFont(mTitleTextFont);
    p->drawText(linePos, QStringLiteral("大盘"));
}

void CurWeeklys::drawGrid(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, false);
    int headerBase = 110, yBase = height() - 50, xBase = width() - 60, headerY = 110;
    int lineStep = (yBase - headerBase) / 5;

    ///![1]绘制水平网格线和右侧坐标值
    p->setFont(mYAxisFont);
    int fontHalfH = p->fontMetrics().height()/2;

    p->setPen(QColor("#2f2f2f"));
    p->drawLine(XOffset,yBase,xBase,yBase);//坐标轴x宽度为width

    for(int k = 1; k<5; ++k) {
        p->setPen(QColor("#0d0d0d"));
        p->drawLine(XOffset,yBase-k*lineStep, xBase, yBase-k*lineStep);
        p->setPen(QColor("#c9d0d6"));
        p->drawText(QRect(xBase+10,yBase-k*lineStep-fontHalfH, xBase+10, fontHalfH*2),
                    Qt::AlignVCenter|Qt::AlignLeft, QString::number(m_yAxisVal[k],'f',2)+"%");
    }

    ///![2] 底部基线和头部基线右侧坐标值
    p->setPen(QColor("#c9d0d6"));
    p->drawText(QRect(xBase+10,yBase-fontHalfH, xBase+10, fontHalfH*2),
                Qt::AlignVCenter|Qt::AlignLeft, QString::number(m_yAxisVal[0],'f',2)+"%");
    p->drawText(QRect(xBase+10,headerY-fontHalfH, xBase+10, fontHalfH*2),
                Qt::AlignVCenter|Qt::AlignLeft, QString::number(m_yAxisVal[5],'f',2)+"%");
}

void CurWeeklys::drawXAxisAndLines(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, true);
    if(m_Max == -qInf() || m_Min == qInf())
        return;

    ///![1]
    int yBase = height() - 50, xBase = width() - 60, headerBase = 110;
    int step = (xBase - XOffset)/5;
    int xtick = XOffset, xtickLast;

    p->setFont(mBaseFont);
    p->setPen(QColor("#858585"));

    int points = qMin(m_Datas.selstks.count(), m_Datas.markets.count());
    ///![2] 从1开始，取中间 ,绘制底部基线和底部文本
    for(int i=1; i<points; ++i)
    {
        //绘制刻度线
        xtick = XOffset + (points-(i-0.5))*step;
        xtickLast = XOffset + (points-(i+0.5))*step;

        //底部文本
        p->setPen(QColor("#2f2f2f"));
        int textHalf = p->fontMetrics().width(m_weekTexts[points-i])/2;
        p->drawText(QRect(xtick-textHalf,yBase+8,xtick-textHalf,yBase+8), m_weekTexts[points-i]);

        qreal sel_pix_last = (m_Max - m_Datas.selstks[i].Rate) / (m_Max - m_Min) * (yBase - headerBase),
                mkt_pix_last = (m_Max - m_Datas.markets[i].Rate) / (m_Max - m_Min) * (yBase - headerBase),
                sel_pix = (m_Max -m_Datas.selstks[i-1].Rate) / (m_Max - m_Min) * (yBase - headerBase),
                mkt_pix = (m_Max - m_Datas.markets[i-1].Rate) / (m_Max - m_Min) * (yBase - headerBase);

        QPoint sp1(xtick, headerBase+sel_pix), sp2(xtickLast, headerBase+sel_pix_last),
                mp1(xtick, headerBase+mkt_pix), mp2(xtickLast, headerBase+mkt_pix_last);

        drawPointAndLinkLines(p, QColor("#191919"), QColor("#ffa200"), sp1, sp2);
        drawPointAndLinkLines(p, QColor("#191919"), QColor("#529efc"), mp1, mp2);
    }

    xtick = XOffset + (0.5)*step;
    p->setPen(QColor("#2f2f2f"));
    int textHalf = p->fontMetrics().width(m_weekTexts[0])/2;
    p->drawText(QRect(xtick-textHalf,yBase+8,xtick-textHalf,yBase+8), m_weekTexts[0]);
}

void CurWeeklys::drawPointAndLinkLines(QPainter *p, const QBrush &br, const QPen &pen, const QPoint &p1, const QPoint &p2)
{
    QPen tmpPen(pen);
    tmpPen.setWidth(2);
    p->setBrush(br);
    p->setPen(tmpPen);
    p->drawLine(p1,p2);
    p->drawEllipse(p1,5,5);
    p->drawEllipse(p2,5,5);
}

//------------------ 本周周报 end -------------------------------------------------------------

void CapitalChange::initTable(WeeklyChgModel *model, QTableView *view, WeeklyChgDelegate *deleg, QBoxLayout *layout)
{
    WeeklySortFilterProxyModel *proxyModel = new WeeklySortFilterProxyModel(CurWeeklys_Type);
    proxyModel->setSourceModel(model);
    view->setModel(proxyModel);
    view->setItemDelegate(deleg);
    view->setFocusPolicy(Qt::NoFocus);
    view->setAlternatingRowColors(false);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->horizontalHeader()->setStretchLastSection(false);
    view->horizontalHeader()->setHighlightSections(false);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setVisible(false);
    view->setShowGrid(false);
    view->setMouseTracking(true);
    view->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    view->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->sortByColumn(1,deleg->changeType() == CapitalChange_In ? Qt::AscendingOrder : Qt::DescendingOrder);
    layout->addWidget(view);

//    view->setColumnWidth(0,82);
}

weeklys::weeklys(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::weeklys)
{
    ui->setupUi(this);

    m_wnds.push_back(new WeeklyCalandarBar);
    m_wnds.push_back(new CurWeeklys);
    m_wnds.push_back(new CapitalChange);
    m_wnds.push_back(new CurUpDowns);
    m_wnds.push_back(new StealingSummary);

    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->setContentsMargins(2,2,2,2);
    hbox1->setSpacing(4);
    hbox1->addWidget(dynamic_cast<QWidget*>(m_wnds[1]));
    hbox1->addWidget(dynamic_cast<QWidget*>(m_wnds[2]));
    hbox1->setStretch(0,1);
    hbox1->setStretch(1,1);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->setContentsMargins(2,2,2,2);
    hbox2->setSpacing(4);
    hbox2->addWidget(dynamic_cast<QWidget*>(m_wnds[3]));
    hbox2->addWidget(dynamic_cast<QWidget*>(m_wnds[4]));
    hbox2->setStretch(0,1);
    hbox2->setStretch(1,1);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(2,2,2,2);
    vbox->setSpacing(0);
    vbox->addWidget(dynamic_cast<QWidget*>(m_wnds[0]));
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->setStretch(1,1);
    vbox->setStretch(2,1);
    setLayout(vbox);

    connect(dynamic_cast<CurUpDowns*>(m_wnds[3]), &CurUpDowns::signalRowClicked,
            [=](const QString &stockcode){
        dynamic_cast<StealingSummary*>(m_wnds[4])->updateForRow();
        emit signalRowClicked(stockcode);
    });

    connect(dynamic_cast<StealingSummary*>(m_wnds[4]), &StealingSummary::signalRowClicked,
            [=](const QString &stockcode){
        dynamic_cast<CurUpDowns*>(m_wnds[3])->updateForRow();
        emit signalRowClicked(stockcode);
    });
}

weeklys::~weeklys()
{
    delete ui;
}

void weeklys::afterActive()
{
    startRequest();
}

void weeklys::afterDeactive()
{

}

void weeklys::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void weeklys::startRequest()
{
    QStringList stocks;
    for(const QString &code:ZXStockOpt::instance()->getZXStock()) stocks << code;
    OptionalWeeklyCallBack fun = std::bind(&weeklys::weeklysCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_reqId = HzInfoCenter::getInstance()->reqOptionalWeekly(stocks.join(","), fun);
}

void weeklys::weeklysCallBack(const InfoRet &ret, const OptionalWeekly &data)
{
    //更新每页数据
    CurUpDowns *upDowns = dynamic_cast<CurUpDowns*>(m_wnds[3]);
    for(WeeklyWndUpdate *wnd : m_wnds) {
        wnd->updateFromContent(QVariant::fromValue(data));
        if(wnd == upDowns) {
            upDowns->firstSelect();
        }
    }
}





