#include "multihour.h"
#include <QDebug>
#include <QResizeEvent>
#include <QFile>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QDateTime>
#include <QTextLayout>
#include <QtMath>
#include <QTextDocument>
#include <QTimer>
#include <QTextCharFormat>
#include <infostyle.h>

#include "calandarbar.h"

#define FRESH_TIME 10*1000

///
/// \brief MultiHourModel::MultiHourModel
/// \param parent
///
MultiHourModel::MultiHourModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

MultiHourModel::~MultiHourModel()
{

}

void MultiHourModel::addRecords(const QList<STMultiHour> &items)
{
    m_records += items;
    flush();
}

void MultiHourModel::appendRecords(const QList<STMultiHour> &items)
{
    m_records += items;
    flush();
}

void MultiHourModel::addRecord(const STMultiHour &item)
{
    if(!m_records.contains(item)) {
        m_records.append(item);
        emit dataChanged(index(m_records.count()-1,0), index(rowCount(),0));
    }
}

void MultiHourModel::insertRecord(const STMultiHour &item, int idx)
{
    if(idx && idx < m_records.count()) {
        m_records.insert(idx, item);
    }else{
        m_records.append(item);
    }

    emit dataChanged(index(0, 0), index(rowCount(),0));
}

QVariant MultiHourModel::recordByIndex(int index)
{
    if(index < 0 || index > m_records.count())
        return QVariant();

    return QVariant::fromValue(m_records[index]);
}

void MultiHourModel::resetModel()
{
    m_records.clear();
    flush();
}

int MultiHourModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_records.count();
}

QVariant MultiHourModel::data(const QModelIndex &index, int role) const
{
    if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
        return QVariant::fromValue(m_records[index.row()]);
    } else if(HZINFOITEM_CUSTOM_ROLE_TIME_ROOT == role) {
        return QVariant::fromValue(m_records[index.row()].timeRoot);
    }

    return QVariant();
}

void MultiHourModel::flush()
{
    beginResetModel();
    endResetModel();
}

QList<STMultiHour> MultiHourModel::getRecores() const
{
    return m_records;
}


///
/// \brief MultiHourView::MultiHourView
/// \param page
/// \param parent
///
MultiHourView::MultiHourView(int page, QWidget *parent)
    :QListView(parent)
    ,m_pageId(page)
    ,m_smartModel(new MultiHourModel)
    ,m_smartDelegate(new MultiHourDelegate)
{
    setObjectName("HZSmartListPanelView");
    setMouseTracking(false);
    setSpacing(0);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeMode(QListView::Adjust);
    setViewMode(QListView::IconMode);
    setWordWrap(true);
    setItemDelegate(m_smartDelegate);
    setModel(m_smartModel);
}

MultiHourView::~MultiHourView()
{

}

void MultiHourView::addRecord(const STMultiHour &item)
{
    if(m_smartModel) {
        m_smartModel->addRecord(item);
    }
}

void MultiHourView::addRecords(const QList<STMultiHour> &item)
{
    m_smartModel->addRecords(item);
}

void MultiHourView::resetModel()
{
    m_smartModel->resetModel();
}

void MultiHourView::setData(const QVariantMap &data)
{
    QString id          = data["id"].toString();
    QList<STMultiHour> lst   = data["data"].value<QList<STMultiHour>>();
    int code            = data["code"].toInt();
    int page            = data["page"].toInt();

    //qDebug() << Q_FUNC_INFO << "code:" << code << "page:" << page << "lst:" << lst.isEmpty();
    if(REQUEST_OPERATOR_EXCEPTION != code){
        m_dataFlags = Request_Exception;
        m_smartModel->addRecords(QList<STMultiHour>{});
        m_tipText = QStringLiteral("操作失败，未知错误");
        m_tipUrl = ":/skin/default/img/infopage/NEWS02.png";
        update();
    } else {
        if(code == REQUEST_SUCCESS && (page != -2 || !lst.isEmpty()) ) {
            m_dataFlags = Requst_Normal;
            m_smartModel->addRecords(lst);
        } else {
            m_dataFlags = Request_Null;
            m_smartModel->addRecords(QList<STMultiHour>{});
            m_tipText = QStringLiteral("当前暂无数据");
            m_tipUrl = ":/skin/default/img/infopage/NEWS01.png";
            update();
        }
    }
}

void MultiHourView::slotScrollChanged(int value)
{
    this->verticalScrollBar()->setValue(value);
}

void MultiHourView::resizeEvent(QResizeEvent *e)
{
    m_smartDelegate->setSize(e->size());
    QListView::resizeEvent(e);
}

void MultiHourView::paintEvent(QPaintEvent *e)
{
    switch(m_dataFlags)
    {
    case Request_Null:
    case Request_Exception:{
        QPainter p(viewport());
        p.fillRect(rect(), gInfoStyle->m_invalidBrush);
        p.setFont(gInfoStyle->m_invalidFont);
        int textLen = p.fontMetrics().width(m_tipText);
        int textH = p.fontMetrics().height();
        int w = width()/2, h = height()/2;
        QRect rt(w-73/2, h-69/2, 73, 62);//83 83
        QPixmap pix(m_tipUrl);
        if(!pix.isNull()){
            p.drawPixmap(rt, pix);
            QRect textRt(w-textLen/2, rt.y()+rt.height()+17, textLen, textH);
            p.setPen(gInfoStyle->m_invalidPen);
            p.drawText(textRt, Qt::AlignCenter, m_tipText);
        }
    }break;
    case Requst_Normal:{
        QPainter p(viewport());
        p.fillRect(rect(),gInfoStyle->m_marketCldDetailBrush);
        QListView::paintEvent(e);
    }
    }
}

///
/// \brief MultiHourDelegate::MultiHourDelegate
/// \param parent
///
MultiHourDelegate::MultiHourDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
    ,mSize(439, 87)
{
}

MultiHourDelegate::~MultiHourDelegate()
{
}


void MultiHourDelegate::setSize(const QSize &size)
{
    if(size.isValid()) mSize = size;
}

void MultiHourDelegate::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if(!index.isValid()) {
        QStyledItemDelegate::paint(painter,opt,index);
        return;
    }

    QStyleOptionViewItem option(opt);
    option.rect.adjust(HZINFOITEM_RECT_LEFT_SPACE,0,-10,0);
    QRect vLine(option.rect.adjusted(HZINFOITEM_ROOT_WIDTH_VALUE/2+7,0,0,0));
    painter->setPen(QPen(QColor("#2f2f2f"),1));
    painter->drawLine(vLine.topLeft(), vLine.bottomLeft());

    const STMultiHour &record = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STMultiHour>();
    if(!record.timeRoot) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, false);
        QTextLayout textLayout(record.content, gInfoStyle->m_multiTitleFont);
        int leading = 4,height = 4;
        textLayout.setCacheEnabled(true);
        textLayout.beginLayout();
        while (1) {
            QTextLine line = textLayout.createLine();
            if (!line.isValid()) break;
            line.setLineWidth(mSize.width()-HZINFOITEM_RECT_LEFT_SPACE-HZINFOITEM_ROOT_WIDTH_VALUE/2-27);
            height += leading;
            line.setPosition(QPointF(12, height));
            height += line.height();
        }
        textLayout.endLayout();
        int lineCnt = textLayout.lineCount();
        int space = lineCnt * (leading+height);
        int lineHight = (textLayout.boundingRect().height()-space)/lineCnt;
        if(lineHight < 36) lineHight = 36;
        QRect titleRt(option.rect.adjusted(HZINFOITEM_ROOT_WIDTH_VALUE/2+7, 8, 0, -option.rect.height()+lineHight));
        painter->setPen(QColor("#797979"));
        textLayout.draw(painter, titleRt.topLeft());
        bool isHover = opt.state.testFlag(QStyle::State_Selected) || opt.state.testFlag(QStyle::State_MouseOver);
        QString timeStr = QDateTime::fromTime_t(record.time).toString(" hh:mm ");//hh:mm:ss
        painter->setPen(QColor("#cad4dc"));
        painter->drawText(titleRt, Qt::AlignLeft|Qt::AlignVCenter, record.title);
        painter->restore();
        drawTime(painter, titleRt, timeStr, isHover);
    }else{
        drawDay(painter, option.rect, record.time);
    }
}

QSize MultiHourDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    Q_UNUSED(opt);
    if(!index.isValid()) {
        return QStyledItemDelegate::sizeHint(opt,index);
    }

    QSize size(mSize);
    const STMultiHour &data = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STMultiHour>();
    if(!data.timeRoot) {
        QTextLayout textLayout(data.content);
        int leading = 4,height = 4;
        textLayout.setCacheEnabled(true);
        textLayout.beginLayout();
        while (1) {
            QTextLine line = textLayout.createLine();
            if (!line.isValid()) break;
            line.setLineWidth(size.width()-HZINFOITEM_RECT_LEFT_SPACE-HZINFOITEM_ROOT_WIDTH_VALUE/2-27);
            height += leading;
            line.setPosition(QPointF(12, height));
            height += line.height();
        }
        textLayout.endLayout();
        int lineCnt = textLayout.lineCount();
        int space = lineCnt * (leading+height);
        int lineHight = (textLayout.boundingRect().height()-space)/lineCnt;
        if(lineHight < 36) lineHight = 36;
        int rtH = textLayout.boundingRect().height()+18;
        size.setHeight((rtH>36?rtH:36)+lineHight);//add one line
    } else {
        size.setHeight(36);
    }
    return size;
}

void MultiHourDelegate::drawDay(QPainter *painter, const QRect &rect, quint32 time) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    QString date = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString value = QDateTime::fromTime_t(time).toString("yyyyMMdd");
    bool isDay(date==value);
    painter->setPen(QPen(isDay?QColor("#414141"):QColor("#4f9bff"),2));
    painter->setBrush(QColor("#191919"));//option.rect.adjusted(HZINFOITEM_ROOT_WIDTH_VALUE/2+7, 8
    QRect dayRt(rect.adjusted(14, (rect.height()-HZINFOITEM_ROOT_HEIGHT_VALUE)/2+4,
                              HZINFOITEM_ROOT_WIDTH_VALUE-rect.width(), HZINFOITEM_ROOT_HEIGHT_VALUE-rect.height()+4));
    painter->drawRoundedRect(dayRt, HZINFOITEM_ROOT_RADIUS_VALUE, HZINFOITEM_ROOT_RADIUS_VALUE);
    painter->setPen(isDay?QColor("#cad4dc"):QColor("#4f9bff"));
    QString week = isDay?CalandarBarFrame::weekOf(QDateTime::currentDateTime().toTime_t()):CalandarBarFrame::weekOf(time);
    painter->drawText(dayRt, Qt::AlignCenter, (isDay?QStringLiteral("今天 "):QDateTime::fromTime_t(time).toString("yyyy-MM-dd "))+week);
    painter->restore();
}

void MultiHourDelegate::drawTime(QPainter *painter, const QRect &rect, const QString &text, bool isHover) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    QRect titleRt(/*rect.adjusted(-7,0,0,0)*/rect);
    int textLen = painter->fontMetrics().width(text), fonth=painter->fontMetrics().height();
    if(isHover){
        painter->setPen(QPen(QColor("#4f9bff"),2));
        painter->setBrush(QColor("#0e0e0e"));
    } else {
        painter->setPen(QPen(QColor("#adadad"),2));
        painter->setBrush(QColor("#0e0e0e"));
    }

    QPoint ellipsePt(titleRt.topLeft());
    ellipsePt.setY(titleRt.y()+titleRt.height()/2);
    painter->drawEllipse(ellipsePt, 5, 5);
    QRect textRt(ellipsePt.x()-10-textLen, titleRt.y()+(titleRt.height()-fonth)/2, textLen, fonth);
    painter->drawText(textRt, Qt::AlignVCenter|Qt::AlignRight, text);
    painter->restore();
}

///
/// \brief MultiHourMarster::MultiHourMarster
/// \param adapter
/// \param parent
///
MultiHourMarster::MultiHourMarster(IUpdateRequest *adapter, QWidget *parent)
    :BasePage(parent)
    ,m_iURequest(adapter)
{
    ///![0]
    setObjectName("MultiHourMarster");

    ///![1]
    m_panelView = new MultiHourView(0);
    m_panelView->setObjectName("MultiHourView");
    m_vScrollBar = new QScrollBar(this);
    m_vScrollBar->setObjectName("VScrollBar");
    m_ctrlFrame = new QFrame(this);
    m_ctrlFrame->setObjectName("CtrlFrame");

    ///![2]
    m_autoRefresh = new QPushButton(m_ctrlFrame);
    m_autoRefresh->setFixedSize(90,20);
    m_autoRefresh->setObjectName("AutoRefresh");
    m_refresh = new QPushButton(m_ctrlFrame);
    m_refresh->setFixedSize(90,20);
    m_refresh->setObjectName("ManualRefresh");

    m_autoRefresh->setCheckable(true);
    m_autoRefresh->setChecked(false);
    m_refresh->setCheckable(true);
    m_refresh->setChecked(false);

    m_btnGrps = new QButtonGroup(this);
    m_btnGrps->setExclusive(true);
    m_btnGrps->addButton(m_autoRefresh,0);
    m_btnGrps->addButton(m_refresh,1);
    connect(m_btnGrps, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [=](int id){
        if(id == 0) {
            m_reqTimer.start();
        } else if(id == 1) {
            m_iURequest->initializatItems();
            m_reqTimer.stop();
        }
    });

    ///![3]
    QVBoxLayout *refreshLayout = new QVBoxLayout;
    refreshLayout->addWidget(m_autoRefresh);
    refreshLayout->addWidget(m_refresh);
    refreshLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_ctrlFrame->setLayout(refreshLayout);
    connect(m_vScrollBar, &QScrollBar::valueChanged, [=](int value){
        int max = m_vScrollBar->maximum();
        if(value == max){
            emit signalReStartRequest();
        }
    });

    ///![4]
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_panelView);
    hLayout->addWidget(m_ctrlFrame);
    hLayout->addWidget(m_vScrollBar);
    m_ctrlFrame->setFixedWidth(175);

    ///![5]
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    ///![6] scroll bar
    connect(m_vScrollBar, &QScrollBar::valueChanged, this, &MultiHourMarster::signalScrollValue);
    connect(this, &MultiHourMarster::signalScrollValue, m_panelView, &MultiHourView::slotScrollChanged);
    connect(m_panelView->verticalScrollBar(), &QScrollBar::rangeChanged, [=](int min, int max){
        m_vScrollBar->setRange(min,max);
    });
    connect(m_panelView->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value){
        m_vScrollBar->setValue(value);
    });

    m_iURequest->setVisitor(this);
    connect(this, &MultiHourMarster::signalReStartRequest, [=]{
        m_iURequest->requestListForParam();
    });


    //默认手动刷新
    m_btnGrps->button(1)->setChecked(true);
    m_reqTimer.setInterval(5000);
    connect(&m_reqTimer, &QTimer::timeout, [=]{
        //qDebug() << "MultiHourMarster timer...";
        m_iURequest->initializatItems();
    });
}

MultiHourMarster::~MultiHourMarster()
{

}

void MultiHourMarster::setAdapter(IUpdateRequest *adapter)
{
    //TODO
}

void MultiHourMarster::updateList(const QVariantMap &var)
{
    if(var.isEmpty()) return;
    m_panelView->setData(var);
}

void MultiHourMarster::updateDetails(const QVariant &var)
{
    //TODO
}

void MultiHourMarster::afterDeactive()
{
    m_iURequest->cancleReq();
    m_reqTimer.stop();//取消定时
}

void MultiHourMarster::afterActive()
{
    m_panelView->resetModel();
    m_iURequest->initializatItems(true);

    //重新启动一下刷新模式
    if(m_autoRefresh->isChecked()) {
        m_reqTimer.start();
    } else {
        m_btnGrps->button(1)->setChecked(true);
    }
}

void MultiHourMarster::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void MultiHourMarster::hideEvent(QHideEvent *event)
{
    event->accept();
    afterDeactive();
}
