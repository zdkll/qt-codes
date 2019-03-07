#include "calandarbar.h"

#include <QtCore>
#include <QPainter>
#include <QPixmap>

#include "updatinterface.h"
#include "hzinfodata.h"
#include "infostyle.h"
#include "hzcalandarctrl.h"

#define CALANDARITEM_WIDTH 140


///
/// \brief CustomCldBtn::mousePressEvent
/// \param e
///
void CustomCldBtn::setStrText(const QString &text)
{
    m_text = text;
    this->update();
}

void CustomCldBtn::mousePressEvent(QMouseEvent *e)
{
    m_isHover = 1;
    QPoint glbPos = mapToGlobal(m_forward?rect().bottomRight():rect().bottomLeft());
    emit signalClicked(glbPos);
    QPushButton::mouseMoveEvent(e);
}

void CustomCldBtn::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
    QPainter p(this);
    int xPos = 15;
    QPixmap pix(m_iconPath);
    if(!pix.isNull()) {
        QRect pixRt(xPos, (height()-10)/2, 10, 10);
        p.drawPixmap(pixRt, pix);
    }

    xPos += 30;
    int textW = width() - 10;
    if(!m_text.isEmpty()) {
        p.setFont(gInfoStyle->m_dateTextFont);
        textW = p.fontMetrics().width(m_text);
        QRect textRt(xPos, 0, textW, height());
        p.setPen(m_isHover?gInfoStyle->m_dateSelClr:QColor("#adadad"));
        p.drawText(textRt, Qt::AlignCenter, m_text);
        textW += 10;
    }

    xPos += textW-2;
    int ih = height()/2-2;
    QPointF points[4] = {
        QPointF(xPos*1., ih),
        QPointF(xPos+8*1., ih),
        QPointF(xPos+4*1., ih+4),
        QPointF(xPos, ih),
    };

    p.setPen(m_isHover? gInfoStyle->m_dateArrowSelClr : QColor("#808080"));
    p.setBrush(m_isHover? gInfoStyle->m_dateArrowSelClr : QColor("#808080"));
    p.drawPolygon(points, 4);
}

void CustomCldBtn::slotMenuVisible(bool visible)
{
    m_isHover = visible;
    update();
}

///
/// \brief CalandarBarFrame::CalandarBarFrame
/// \param parent
///
CalandarBarFrame::CalandarBarFrame(QWidget *parent)
    :QFrame(parent)
    ,m_pressPos(QPoint(-1,-1))
    ,m_isPressed(false)
    ,m_calandarInfo(new CalandarData)
{
    ///![0] 初始化时间
    //test();

    ///![0] 初始化游标参数
    m_calandarInfo->start  = 0;
    m_calandarInfo->end    = m_dates.count();
    m_calandarInfo->time   = QDateTime::currentDateTime().toTime_t();
    m_calandarInfo->index  = 0;//表示今天

    ///![1] 初始化当前默认展示项
    QString toDay = QDateTime::currentDateTime().toString("yyyyMMdd");
    setStartDay(toDay);
}

void CalandarBarFrame::initDate(const QVector<quint64> &dates)
{
    m_dates.clear();
    foreach(quint32 time, dates) m_dates.append(CalandarDataDetail{time, 0});

    ///![0] 初始化游标参数
    m_calandarInfo->start  = 0;
    m_calandarInfo->end    = m_dates.count();
    m_calandarInfo->time   = QDateTime::currentDateTime().toTime_t();
    m_calandarInfo->index  = 0;
    update();
}

void CalandarBarFrame::timeScroll(bool forward)
{
    m_visualItems = qCeil(width()*1./CALANDARITEM_WIDTH);
    if(m_calandarInfo->end > m_visualItems){
        m_paintForward = forward;
        if(forward) {
            if(m_calandarInfo->index < m_calandarInfo->end-m_visualItems)
                m_calandarInfo->index = m_calandarInfo->index+1;
        } else {
            if(m_calandarInfo->index > 0)
             m_calandarInfo->index = qMax(m_calandarInfo->index-1, m_calandarInfo->start);
        }
    }else {
        m_paintForward = false;
    }

    qDebug() << "index:" << m_calandarInfo->index << "cnt:" << m_calandarInfo->end;
    update();
}

void CalandarBarFrame::setStartDay(const QString &day, bool select)
{
    if(day.isEmpty() || m_dates.isEmpty()) return;
    int index = -1;
    for(int i=0; i<m_dates.count(); ++i){
        QString tmpDay = QDateTime::fromTime_t(m_dates[i].date).toString("yyyyMMdd");
        if(day == tmpDay){
            index = i;
            break;
        }
    }

    if(index == -1) {
        m_paintForward = false;
        m_calandarInfo->index = 0;
        resetFlags(0, select);
    } else { 
        m_visualItems = qCeil(width()*1./CALANDARITEM_WIDTH);
        if(m_calandarInfo->end > m_visualItems){
            m_paintForward = true;
            int tmpVal = m_calandarInfo->end - m_calandarInfo->index;//调整游标位置
            if(tmpVal < m_visualItems) {
                m_calandarInfo->index = m_calandarInfo->index - (m_visualItems-tmpVal);
            }else{
                m_calandarInfo->index = index;
            }
        }else{
            m_paintForward = false;
        }

        //正好是第一个值，从左边开始刷
        if(index == 0)
            m_paintForward = false;
        resetFlags(index, select);//设置目标标志
    }

    //qDebug() << Q_FUNC_INFO << "day:" << day << "index:" << index << "Sentry:" << mCalandarInfo->index;

    update();
}

QString CalandarBarFrame::currentDate() const
{
    for(int i=0; i<m_dates.count(); ++i){
         if(m_dates[i].flags)
             return QDateTime::fromTime_t(m_dates[i].date).toString("yyyyMMdd");
    }
    return "";
}

QString CalandarBarFrame::weekOf(quint32 time)
{
    QString weekOf = QDateTime::fromTime_t(time).toString("dddd");

#if 0 //english
    if(weekOf == "Sunday"){
        weekOf = QStringLiteral("星期日");
    } else if(weekOf == "Monday") {
        weekOf = QStringLiteral("星期一");
    }else if(weekOf == "Tuesday") {
        weekOf = QStringLiteral("星期二");
    }else if(weekOf == "Wednesday") {
        weekOf = QStringLiteral("星期三");
    }else if(weekOf == "Thursday") {
        weekOf = QStringLiteral("星期四");
    }else if(weekOf == "Friday") {
        weekOf = QStringLiteral("星期五");
    }else if(weekOf == "Saturday") {
        weekOf = QStringLiteral("星期六");
    }
#endif
    return weekOf;
}

void CalandarBarFrame::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_pressPos = event->pos();
    update();
}

//待优化
void CalandarBarFrame::paintEvent(QPaintEvent *e)
{
    int itemCnt=0;
    m_visualItems = qCeil(width()*1./CALANDARITEM_WIDTH);
    if(m_calandarInfo->end > m_visualItems){
        int tmpVal = m_calandarInfo->end - m_calandarInfo->index;
        if(tmpVal < m_visualItems) {
            m_calandarInfo->index = m_calandarInfo->index - (m_visualItems-tmpVal);
        }
        itemCnt = m_visualItems;
    }else {
        m_calandarInfo->index = 0;
        itemCnt = m_calandarInfo->end;
    }

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#222222"));
    painter.setPen(Qt::red);
    QRect calRt(rect());//.adjusted(2,2,-2,-2)
    int visualW = width();
    if(m_paintForward){
        for(int i=itemCnt-1; i>=0; --i){
            int idx = m_calandarInfo->index+itemCnt-i-1;
            QRect itemRt(visualW-(i+1)*CALANDARITEM_WIDTH, calRt.y(), CALANDARITEM_WIDTH, calRt.height());
            QString text = QString("%1 (%2)").arg(weekOf(m_dates[idx].date))
                    .arg(QDateTime::fromTime_t(m_dates[idx].date).toString("MM-dd"));
            painter.setFont(gInfoStyle->m_dateScrollTextFont);
            if(itemRt.contains(m_pressPos) && m_isPressed) {//pressed
                m_isPressed = false;
                resetFlags(idx);
                update();//自更新
            }
            painter.setPen(Qt::black);
            painter.drawLine(QPoint(itemRt.topRight().x(), itemRt.topRight().y()+6),
                             QPoint(itemRt.bottomRight().x(), itemRt.bottomRight().y()-6));
            if(m_dates[idx].flags){
                if(isToDay(idx)){
                    QRect toDayRt( visualW-((i)*CALANDARITEM_WIDTH)-3-18 , calRt.y()+3,18,18);
                    if(gInfoStyle->gIconUrls.contains("today") && !gInfoStyle->gIconUrls.isEmpty()) {
                        QPixmap pixmap(gInfoStyle->gIconUrls["today"]);
                        if(!pixmap.isNull()) painter.drawPixmap(toDayRt, pixmap);
                    }
                }
                painter.setPen(QPen(QColor("#459bff"),2));
                painter.drawLine(itemRt.bottomLeft(), itemRt.bottomRight());
                painter.setPen(gInfoStyle->m_dateScrollSelClr);
                painter.drawText(itemRt, Qt::AlignCenter, text);
            }else{
                painter.setPen(Qt::NoPen);
                painter.drawLine(itemRt.bottomLeft(), itemRt.bottomRight());
                painter.setPen(gInfoStyle->m_dateScrollClr);
                painter.drawText(itemRt, Qt::AlignCenter, text);
            }
        }
    } else {
        for(int i=0; i<itemCnt; ++i){
            QRect itemRt(i*CALANDARITEM_WIDTH, calRt.y(), CALANDARITEM_WIDTH, calRt.height());
            painter.setFont(gInfoStyle->m_dateScrollTextFont);
            if(itemRt.contains(m_pressPos) && m_isPressed) {//pressed
                m_isPressed = false;
                resetFlags(m_calandarInfo->index+i);
                update();//自更新
            }
            QString text = QString("%1 (%2)").arg(weekOf(m_dates[m_calandarInfo->index+i].date))
                    .arg(QDateTime::fromTime_t(m_dates[m_calandarInfo->index+i].date).toString("MM-dd"));
            painter.setPen(Qt::black);
            painter.drawLine(QPoint(itemRt.topRight().x(), itemRt.topRight().y()+6),
                             QPoint(itemRt.bottomRight().x(), itemRt.bottomRight().y()-6));
            if(m_dates[m_calandarInfo->index+i].flags){
                if(isToDay(m_calandarInfo->index+i)){
                    QRect toDayRt((i+1)*CALANDARITEM_WIDTH-3-18, calRt.y()+3,18,18);
                    if(gInfoStyle->gIconUrls.contains("today") && !gInfoStyle->gIconUrls.isEmpty()) {
                        QPixmap pixmap(gInfoStyle->gIconUrls["today"]);
                        if(!pixmap.isNull()) painter.drawPixmap(toDayRt, pixmap);
                    }
                }
                painter.setPen(QPen(QColor("#459bff"),2));
                painter.drawLine(itemRt.bottomLeft(), itemRt.bottomRight());
                painter.setPen(gInfoStyle->m_dateScrollSelClr);
                painter.drawText(itemRt, Qt::AlignCenter, text);
            }else{
                painter.setPen(Qt::NoPen);
                painter.drawLine(itemRt.bottomLeft(), itemRt.bottomRight());
                painter.setPen(gInfoStyle->m_dateScrollClr);
                painter.drawText(itemRt, Qt::AlignCenter, text);
            }
        }
    }
}

void CalandarBarFrame::test()
{
    QVector<quint64> times;
    for(int i=1; i<=100; i++){
        QDateTime dt(QDateTime::currentDateTime().addDays(-i));
        times.prepend(dt.toTime_t());
    }

    for(int i=0; i<=100; i++){
        QDateTime dt(QDateTime::currentDateTime().addDays(i));
        times.append(dt.toTime_t());
    }

    initDate(times);
}

void CalandarBarFrame::resetFlags(int index, bool statu)
{
    for(int i=0; i<m_dates.count(); ++i) m_dates[i].flags = 0;
    m_dates[index].flags = statu;
    emit signalCalandarClicked();
}

bool CalandarBarFrame::isToDay(int index) const
{
    if(index < 0 || index > m_dates.count()) return 0;
    QString toDay = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString selDay = QDateTime::fromTime_t(m_dates[index].date).toString("yyyyMMdd");
    return toDay == selDay;
}

void CalandarBtn::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    QPainter painter(this);
    QRect rt(rect());
    painter.setPen(QColor("#010101"));
    if(m_forward){
        painter.drawLine(QPoint(rt.topLeft().x(), rt.topLeft().y()+3),
                         QPoint(rt.bottomLeft().x(), rt.bottomLeft().y()-3));
        painter.drawLine(QPoint(rt.topRight().x(), rt.topRight().y()+3),
                         QPoint(rt.bottomRight().x(), rt.bottomRight().y()-3));
    }else {
        painter.drawLine(QPoint(rt.topRight().x(), rt.topRight().y()+3),
                         QPoint(rt.bottomRight().x(), rt.bottomRight().y()-3));
    }
}

///
/// \brief CalandarBarMaster::CalandarBarMaster
/// \param parent
///
CalandarBarMaster::CalandarBarMaster(bool hidetoday,QWidget *parent) :
    QFrame(parent)
{
    m_calandarBar = new CalandarBarFrame;
    m_leftBtn = new CalandarBtn(0);
    m_leftBtn->setFixedWidth(27);
    m_leftBtn->setMinimumHeight(20);
    m_leftBtn->setObjectName("CalandarLeftBtn");
    m_rightBtn = new CalandarBtn(1);
    m_rightBtn->setFixedWidth(27);
    m_rightBtn->setMinimumHeight(20);
    m_rightBtn->setObjectName("CalandarRightBtn");

    m_calandarCtrl   = new HZCalanderCtrl(hidetoday);
    m_cldMenu        = new QMenu(this);
    m_menuAct        = new QWidgetAction(m_cldMenu);
    m_calandarBtn    = new CustomCldBtn(QStringLiteral("自定义"));
    m_calandarBtn->setFixedWidth(110);
    m_cldMenu->setObjectName("CalanderMenu");
    m_cldMenu->setAttribute(Qt::WA_TranslucentBackground);
    m_cldMenu->installEventFilter(this);
    m_menuAct->setDefaultWidget(m_calandarCtrl);
    m_cldMenu->addAction(m_menuAct);

    QHBoxLayout *hb = new QHBoxLayout;
    hb->setContentsMargins(0,0,0,0);
    hb->setSpacing(11);
    hb->addWidget(m_leftBtn);
    hb->addWidget(m_calandarBar);
    hb->addWidget(m_rightBtn);
    hb->addWidget(m_calandarBtn);
    hb->setStretch(1,2);
    setLayout(hb);

    connect(m_leftBtn, &QPushButton::clicked, [=]{m_calandarBar->timeScroll(0);});
    connect(m_rightBtn, &QPushButton::clicked, [=]{m_calandarBar->timeScroll(1);});
    connect(m_calandarBar, &CalandarBarFrame::signalCalandarClicked, this, &CalandarBarMaster::signalCalandarBarClicked);
    connect(m_calandarBtn, &CustomCldBtn::signalClicked, [=](const QPoint& pt){
        if(!pt.isNull()) {
            m_cldMenu->move(pt.x()-m_calandarCtrl->width()-2, pt.y()+4);
            m_calandarBtn->slotMenuVisible(true);
            QTimer::singleShot(200,[&]{
                m_cldMenu->exec();
                m_calandarBtn->slotMenuVisible(false);
            });
        }
    });

    //bug!!!
    connect(m_calandarCtrl, &HZCalanderCtrl::signalDateClicked, [=](quint64 time){
        QString timeStr = QDateTime::fromTime_t(time).toString("yyyyMMdd");
        m_calandarBar->setStartDay(timeStr);       
    });

    connect(m_calandarCtrl, &HZCalanderCtrl::signalDatePressed, [=](quint64 time){
        m_cldMenu->hide();
        QString timeStr = QDateTime::fromTime_t(time).toString("yyyyMMdd");
        m_calandarBar->setStartDay(timeStr);
    });
}

CalandarBarMaster::~CalandarBarMaster()
{
    if(m_cldMenu != nullptr)
    {
        delete m_cldMenu;
        m_cldMenu = nullptr;
    }
}

QString CalandarBarMaster::getCurrentDate() const
{
    return m_calandarBar->currentDate();
}

void CalandarBarMaster::setCurrentDate(const quint64 time)
{
    m_calandarCtrl->setDate(time);
}

void CalandarBarMaster::setValidTimes(const QVector<quint64> &times)
{
    m_calandarCtrl->addValidTimes(times);
    m_calandarBar->initDate(times);

    int index = -1;
    QString curTime(QDateTime::currentDateTime().toString("yyyyMMdd"));
    for(int i=times.count()-1; i>=0; --i) {
        QString tmpTime = QDateTime::fromTime_t(times[i]).toString("yyyyMMdd");
        if(tmpTime == curTime) {
            index = i;
            break;
        }
    }

    if(index != -1) {
        //默认显示当前自然日的下一个自然日
        bool isValid(index+1 < times.count());
        m_calandarCtrl->setDate(times[isValid?index+1:index]);
    }else {
        //没有查找到，则显示列表的最后一项
        m_calandarCtrl->setDate(times.last());
    }
}

bool CalandarBarMaster::eventFilter(QObject *w, QEvent *e)
{
    if(w == m_cldMenu) {
        if(e->type() == QEvent::MouseButtonPress
                || e->type() == QEvent::MouseButtonRelease
                || e->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *ev = static_cast<QMouseEvent*>(e);
            if(ev) {
                QPoint leftTop = m_cldMenu->mapToGlobal(QPoint(0,0));
                QPoint rightBotton(leftTop.x()+m_cldMenu->width(), leftTop.y()+m_cldMenu->height());
                return QRect(leftTop, rightBotton).contains(ev->globalPos());
            }
        }
    }

    return false;
}

CustomCalandarBtnMarster::CustomCalandarBtnMarster(bool forward, bool hidetoday,QWidget *parent)
    :QFrame(parent)
    ,m_forward(forward)
{
    ///![0]
    setFixedWidth(130);
    setObjectName("CustomCalandarBtnMarster");
    m_calandarCtrl   = new HZCalanderCtrl(hidetoday);
    m_cldMenu        = new QMenu;
    m_menuAct        = new QWidgetAction(this);
    m_calandarBtn    = new CustomCldBtn(QStringLiteral("自定义"),forward);
    m_cldMenu->setObjectName("CalanderMenu");
    m_cldMenu->setAttribute(Qt::WA_TranslucentBackground);
    m_cldMenu->installEventFilter(this);
    m_menuAct->setDefaultWidget(m_calandarCtrl);
    m_cldMenu->addAction(m_menuAct);

    ///![1]
    QHBoxLayout *hb = new QHBoxLayout;
    hb->setContentsMargins(0,0,0,0);
    hb->setSpacing(0);
    hb->addWidget(m_calandarBtn);
    setLayout(hb);

    ///![2]
    connect(m_calandarBtn, &CustomCldBtn::signalClicked, [=](const QPoint& pt){
        if(!pt.isNull()) {
            if(m_forward)
                m_cldMenu->move(pt.x()-m_calandarCtrl->width()-2, pt.y()+2);
            else
                m_cldMenu->move(pt.x()+2, pt.y()+2);

            m_calandarBtn->slotMenuVisible(true);
            QTimer::singleShot(200,[&]{
                m_cldMenu->exec();
                m_calandarBtn->slotMenuVisible(false);
            });
        }
    });

    //bug!!!
    connect(m_calandarCtrl, &HZCalanderCtrl::signalDateClicked, [=](quint64 time){
//        mCldMenu->aboutToHide();
//        mCldMenu->hide();
//        emit signalCalandarBarClicked(time);
    });

    connect(m_calandarCtrl, &HZCalanderCtrl::signalDatePressed, [=](quint64 time){
        m_cldMenu->hide();
        emit signalCalandarBarClicked(time);
    });
}

void CustomCalandarBtnMarster::setStrText(const QString &text)
{
    m_calandarBtn->setStrText(text);
}

/**
 * @brief CustomCalandarBtnMarster::addValidTimes 增加时间列表
 * @param times 时间戳列表，
 * note：时间戳列表是升序的
 */
void CustomCalandarBtnMarster::addValidTimes(const QVector<quint64> &times)
{
    m_calandarCtrl->addValidTimes(times);

    int index = -1;
    QString curTime(QDateTime::currentDateTime().toString("yyyyMMdd"));
    for(int i=times.count()-1; i>=0; --i) {
        QString tmpTime = QDateTime::fromTime_t(times[i]).toString("yyyyMMdd");
        if(tmpTime == curTime) {
            index = i;
            break;
        }
    }

    if(index != -1) {
        //默认显示当前自然日的下一个自然日
        bool isValid(index+1 < times.count());
        m_calandarCtrl->setDate(times[isValid?index+1:index]);
        emit signalCalandarBarClicked(times[isValid?index+1:index]);
    }else {
        //没有查找到，则显示列表的最后一项
        m_calandarCtrl->setDate(times.last());
        emit signalCalandarBarClicked(times.last());
    }
}

bool CustomCalandarBtnMarster::eventFilter(QObject *w, QEvent *e)
{
    if(w == m_cldMenu) {
        if(e->type() == QEvent::MouseButtonPress
                || e->type() == QEvent::MouseButtonRelease
                || e->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *ev = static_cast<QMouseEvent*>(e);
            if(ev) {
                QPoint leftTop = m_cldMenu->mapToGlobal(QPoint(0,0));
                QPoint rightBotton(leftTop.x()+m_cldMenu->width(), leftTop.y()+m_cldMenu->height());
                QRect rt(leftTop, rightBotton);
                if(rt.contains(ev->globalPos())) return true;
            }
        }
    }
    return false;
}
