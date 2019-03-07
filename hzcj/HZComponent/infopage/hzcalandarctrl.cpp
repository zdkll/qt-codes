#include "hzcalandarctrl.h"
#include "ui_hzcalandarctrl.h"

#include <QDateTime>
#include <QDate>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>

#define MONTH_RANGE 12

static const QString gWeekNames[] = {
    QStringLiteral("日"),
    QStringLiteral("一"),
    QStringLiteral("二"),
    QStringLiteral("三"),
    QStringLiteral("四"),
    QStringLiteral("五"),
    QStringLiteral("六")
};

static int weekOf(const QString &name)
{
    int index;
    if(name == QStringLiteral("星期日")){
        index = 0;
    } else if(name == QStringLiteral("星期一")) {
        index = 1;
    }else if(name == QStringLiteral("星期二")) {
        index = 2;
    }else if(name == QStringLiteral("星期三")) {
        index = 3;
    }else if(name == QStringLiteral("星期四")) {
        index = 4;
    }else if(name == QStringLiteral("星期五")) {
        index = 5;
    }else if(name == QStringLiteral("星期六")) {
        index = 6;
    }
    return index;
}

///
/// \brief CalandarDisplay::CalandarDisplay
/// \param parent
///
CalandarDisplay::CalandarDisplay(QWidget *parent)
    :QFrame(parent)
    ,m_press(QPoint(-1,-1))
    ,m_hover(QPoint(-1,-1))
    ,m_invalidRt(QRect(0, 0, width(), height()/7))
{
    setMouseTracking(true);
    setObjectName("CalandarDisplay");
}

void CalandarDisplay::setCurrentDate(const QDate &date)
{
    if(!date.isValid())return;
    int year = date.year(), preYear = year, nextYear = year;
    int month = date.month(), preMonth=month, nextMonth=month;
    if(--preMonth < 1){
        preYear = preYear-1;
        preMonth = 12;
    }

    if(++nextMonth > MONTH_RANGE){
        nextYear = nextYear+1;
        nextMonth = 1;
    }

    m_verInfo.premonthEnd = -1;
    m_verInfo.nextmonthStart = -1;
    m_verInfo.data = QVector<CldPrivate::STDateInfo>{};

    QDate preDt(preYear, preMonth, date.day()), nextDt(nextYear, nextMonth, date.day());
    int preDays = preDt.daysInMonth(), curDays = date.daysInMonth();
    QString firstDayStr = QDate(date.year(), date.month(), 1).toString("dddd");
    int firstIdx(weekOf(firstDayStr));
    QVector<CldPrivate::STDateInfo> tmpDates;
    if(firstIdx != 0) {//取上个月最后几天
        for(int i=firstIdx; i>0; --i) {
            CldPrivate::STDateInfo info={preDays, QDateTime(QDate(preDt.year(),preDt.month(), preDays)).toTime_t(), true};
            tmpDates.prepend(info);
            --preDays;
        }
    }

    //取本月天数
    for(int i=1; i<=curDays; ++i) {
        CldPrivate::STDateInfo info={i, QDateTime(QDate(date.year(),date.month(), i)).toTime_t(), true};
        tmpDates.append(info);
    }

    m_verInfo.nextmonthStart = tmpDates.count();

    //取下个月剩余天数
    if(tmpDates.count() < 42) {
        int end = 42 - tmpDates.count();
        for(int i = 1; i<=end; ++i){
            CldPrivate::STDateInfo info={i, QDateTime(QDate(nextDt.year(),nextDt.month(), i)).toTime_t(), true};
            tmpDates.append(info);
        }
    }

    m_defaultTime = QDateTime(date).toTime_t();
    m_verInfo.premonthEnd = firstIdx-1;
    m_verInfo.data = tmpDates;

//    quint64 time64(QDateTime(date).toTime_t());
//    setCurrentClickDate(time64);
    update();
}

void CalandarDisplay::setCurrentClickDate(quint64 time)
{
    m_defaultTime = time;

    int idx = -1;
    for(int i=0; i<m_verInfo.data.count(); ++i) {
        m_verInfo.data[i].enabled = false;
        if(m_verInfo.data[i].dayTime == time) {
            m_verInfo.data[i].enabled = true;
            idx = i;
        }
    }

    if(idx != -1) {
        QPoint pt(pointOf(idx));
        if(!m_invalidRt.contains(pt) && findOf(pt)){
            m_press = pt;
            m_isPressed = true;
            update();
        }
    }
}

void CalandarDisplay::disabledTime(quint64 time)
{
    if(!m_validTms.contains(time))
        m_validTms.append(time);
    update();
}

void CalandarDisplay::disabledTimes(const QVector<quint64> &times)
{
    m_validTms = times;
    update();
}

void CalandarDisplay::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing,true);
    drawGrid(&p);
}

void CalandarDisplay::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    m_invalidRt = QRect(0,0,event->size().width(), event->size().height()/7);
    setCurrentClickDate(m_defaultTime);
}

void CalandarDisplay::mousePressEvent(QMouseEvent *event)
{
    if(!m_invalidRt.contains(event->pos()) && findOf(event->pos())){
        emit signalDatePressed(m_defaultTime);
        m_press = event->pos();
        m_isPressed = true;
        update();
    }
    QFrame::mousePressEvent(event);
}

void CalandarDisplay::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = event->pos();
    update();
    QFrame::mouseMoveEvent(event);
}

bool CalandarDisplay::findOf(const QPoint &pt)
{
    int rCnt = 7, cCnt = 7;
    int rowH = height()/rCnt, colW = width()/cCnt;
    for(int col=0; col<cCnt; ++col) {
        for(int row=1; row<rCnt; ++row) {
            QRect itemRt(col*colW, row*rowH, colW, rowH);
            if(itemRt.contains(pt)) {
                int index = (row - 1) * 7 + col;
                if(m_verInfo.data.isEmpty() || (index < 0 && index > m_verInfo.data.count()-1)) return false;
                if(!m_validTms.contains(m_verInfo.data[index].dayTime))return false;
                m_defaultTime = m_verInfo.data[index].dayTime;
                return true;
            }
        }
    }

    //没有查到
    return false;
}

QPoint CalandarDisplay::pointOf(int order)
{
    QPoint idx(-1,-1);
    int rCnt = 7, cCnt = 7;
    int rowH = height()/rCnt, colW = width()/cCnt;
    for(int col=0; col<cCnt; ++col) {
        for(int row=1; row<rCnt; ++row) {
            int pos = (row - 1) * 7 + col;
            if(order == pos){
                QRect itemRt(col*colW, row*rowH, colW, rowH);
                return itemRt.center();
            }
        }
    }

    return idx;
}

void CalandarDisplay::drawGrid(QPainter *p)
{
    int rCnt = 7, cCnt = 7;
    int rowH = height()/rCnt, colW = width()/cCnt;
    p->setBrush(QColor("#414141"));
    p->setPen(QColor("#414141"));
    QRect rowRt(0, 0, width(), rowH+1);
    p->drawRect(rowRt);

    p->save();
    for(int col=0; col<cCnt; ++col) {
        for(int row=0; row<rCnt; ++row) {
            QRect itemRt(col*colW, row*rowH, colW, rowH);
            if(row == 0) { //week bar
                drawWeekBar(p, itemRt, col);
            } else {
                if(m_gridEnabed) {
                    p->setPen(Qt::white);
                    p->drawRect(itemRt);
                }
                drawItem(p, itemRt, row, col);
            }
        }
    }
    p->restore();
}

void CalandarDisplay::drawWeekBar(QPainter *p, const QRect &rt, int col)
{
    p->save();
    p->setPen(QColor("#9da2a8"));
    p->drawText(rt, Qt::AlignCenter, gWeekNames[col]);
    p->restore();
}

void CalandarDisplay::drawItem(QPainter *p, const QRect &rt, int row, int col)
{
    int index = (row - 1) * 7 + col;
    if(m_verInfo.data.isEmpty() || (index < 0 && index > m_verInfo.data.count()-1))  return;

    //设置是否当月时间样式
    bool isCurMonth = (index > m_verInfo.premonthEnd) && (index < m_verInfo.nextmonthStart),
            isHover = m_validTms.contains(m_verInfo.data[index].dayTime),
            isClicked = isHover && (m_defaultTime==m_verInfo.data[index].dayTime);

    p->setPen(isCurMonth ? QColor("#f4a023"):QColor("#b17164"));
    if(rt.contains(m_hover) && isHover) {
        p->setPen(Qt::white);
        p->setBrush(Qt::NoBrush);
        p->drawRoundedRect(rt.adjusted(2,2,-2,-2), 3, 3);
    }

    if(rt.contains(m_press) && isClicked) {
        p->setPen(Qt::black);
        p->setBrush(QColor("#f4a023"));
        p->drawRoundedRect(rt.adjusted(1,1,-1,-1), 3, 3);
        p->drawText(rt.adjusted(1,1,-1,-1), Qt::AlignCenter, QString::number(m_verInfo.data[index].day));
        if(m_isPressed){
            emit signalDateClicked(m_verInfo.data[index].dayTime);
            m_isPressed = false;
        }
    }else {
        if(!m_validTms.contains(m_verInfo.data[index].dayTime)) p->setPen(QColor("#6b6b6b"));
        p->drawText(rt, Qt::AlignCenter, QString::number(m_verInfo.data[index].day));
    }
}



///
/// \brief Widget::Widget
/// \param parent
///
HZCalanderCtrl::HZCalanderCtrl(bool hidetoday,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HZCalanderCtrl)
    ,m_display(new CalandarDisplay)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(218, 208);
    ui->verticalLayout_2->addWidget(m_display);

    m_date = QDateTime::currentDateTime().date();
    ui->btn_today->setText(QString("%1:%2").arg(QStringLiteral("转到今天")).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    m_display->setCurrentDate(QDateTime::currentDateTime().date());

    connect(ui->btnMAdd, SIGNAL(clicked(bool)), SLOT(slotDateChanged()));
    connect(ui->btnMSub, SIGNAL(clicked(bool)), SLOT(slotDateChanged()));
    connect(ui->btnYAdd, SIGNAL(clicked(bool)), SLOT(slotDateChanged()));
    connect(ui->btnYSub, SIGNAL(clicked(bool)), SLOT(slotDateChanged()));
    connect(m_display, &CalandarDisplay::signalDatePressed, this, [=](quint64 time){
        emit signalDatePressed(time);
    });
    connect(m_display, &CalandarDisplay::signalDateClicked, this, [=](const quint64 &time){
        if(m_cacheTimes.contains(time)) {
            QDateTime dt(QDateTime::fromTime_t(time));
            if(dt.isValid()) {
                m_date = dt.date();
                ui->labDisplay->setText(dt.toString("yyyy-MM-dd"));
                emit signalDateClicked(time);
            }
        }
    });
    connect(ui->btn_today, &QPushButton::clicked, [=]{//转到今天
        QString toDayStr = ui->btn_today->text().split(":").last();
        if(!toDayStr.isEmpty()){
            QDateTime dt(QDateTime::fromString(toDayStr, "yyyy-MM-dd"));
            ui->labDisplay->setText(toDayStr);
            m_display->setCurrentDate(dt.date());
            m_display->setCurrentClickDate(dt.toTime_t());
            emit signalDateClicked(dt.toTime_t());
            m_date = dt.date();
        }
    });

    if(hidetoday)
        ui->frameBottomBar->hide();
}

HZCalanderCtrl::~HZCalanderCtrl()
{
    delete ui;
}

void HZCalanderCtrl::addValidTimes(const QVector<quint64> &times)
{
    if(times.isEmpty()) return;
    m_cacheTimes = times;
    m_display->disabledTimes(times);
    m_startTime = QDateTime::fromTime_t(times.first()).date();
    m_endTime = QDateTime::fromTime_t(times.last()).date();
    update();
    qDebug() << Q_FUNC_INFO << "start:" << m_startTime.toString("yyyyMMdd") << "end:" << m_endTime.toString("yyyyMMdd");
}

void HZCalanderCtrl::setDate(quint64 time)
{
    QDateTime dt(QDateTime::fromTime_t(time));
    if(!dt.isValid()) return;
    m_date = dt.date();
    m_display->setCurrentDate(dt.date());
    m_display->setCurrentClickDate(time);
    ui->labDisplay->setText(dt.toString("yyyy-MM-dd"));
    emit signalDateClicked(time);
}

void HZCalanderCtrl::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::yellow);
    p.drawRect(rect());
    p.drawRect(rect().adjusted(1,1,-1,-1));
}

void HZCalanderCtrl::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_display->update();
}

void HZCalanderCtrl::slotDateChanged()
{
    int y = m_date.year(), m = m_date.month(), d = m_date.day();
    int tmpY=y, tmpM=m, tmpD=d;
    QPushButton *btn = static_cast<QPushButton*>(sender());
    if(btn == nullptr) return;
    if(btn == ui->btnMAdd) {
        //先预算
        if(m_date >= m_endTime) return;
        if(++tmpM > MONTH_RANGE) { tmpM=1, ++tmpY; }
        QDate calDt(tmpY,tmpM,tmpD);
        if(m_date >= calDt) return;
        //实际计算
        if(++m > MONTH_RANGE) { m=1, ++y; }
    } else if(btn == ui->btnMSub) {
        if(m_date<=m_startTime) return;
        if(--tmpM < 1) { tmpM=12, --tmpY;}
        QDate calDt(tmpY,tmpM,tmpD);
        if(m_date <= calDt) return;
        if(--m < 1){  m=12, --y; }
    } else if(btn == ui->btnYAdd) {
        if(m_date > m_endTime) return;
        ++tmpY;
        QDate calDt(tmpY,tmpM,tmpD);
        if(calDt > m_endTime) return;
        ++y;
    } else if(btn == ui->btnYSub) {
        if(m_date < m_startTime) return;
        --tmpY;
        QDate calDt(tmpY,tmpM,tmpD);
        if(calDt < m_startTime) return;
        --y;
    }

    //取当月最后一天，与计算出的数值比较 比如2月份，只有28天
    QDate tmpDt(y,m,1);
    int dayMax(tmpDt.daysInMonth());
    m_date = QDate(y, m, d>dayMax?dayMax:d);
    ui->labDisplay->setText(m_date.toString("yyyy-MM-dd"));
    m_display->setCurrentDate(m_date);
}



