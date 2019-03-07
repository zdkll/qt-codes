#include "platemove.h"
#include "ui_platemove.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QDateTime>

#include <datacontroller.h>
#include <pubmsgdef.h>

#define VLINE_WIDTH 60
#define XPOS_VAR VLINE_WIDTH-24

HZPlateMove::HZPlateMove(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HZPlateMove)
{
    ui->setupUi(this);
    ui->lab_slider->setFixedWidth(45);

    m_vLayout = new QVBoxLayout;
    ui->right_layout->addLayout(m_vLayout);
}

HZPlateMove::~HZPlateMove()
{
    delete ui;

    //leak memory
    if(m_vLayout != nullptr){
        delete m_vLayout;
        m_vLayout = nullptr;
    }
}

void HZPlateMove::request()
{
    HZData::ReqBlockAbnormalInfo reqInfo;
    reqInfo.block_code = "";
    reqInfo.update_time ;
    reqInfo.is_desc = true;
    reqInfo.count_range = 200;
    OnBlockAbnormalInfoArrived fun = std::bind(&HZPlateMove::platDataCallBack,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    m_subId = gHZDataController->requestBlockAbnormalInfo(reqInfo,fun);
}

//订阅板块异动
void HZPlateMove::subPlate()
{
    HZData::ReqBlockAbnormalInfo reqInfo;
    reqInfo.block_code = "";
    reqInfo.update_time ;
    reqInfo.is_desc = true;
    reqInfo.count_range = 200;
    OnBlockAbnormalInfoArrived fun = std::bind(&HZPlateMove::platDataCallBack,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    m_subId = gHZDataController->subBlockAbnormalInfo(reqInfo, fun);
}

void HZPlateMove::cancleSub()
{
    gHZDataController->cancelSub(m_subId);
}

void HZPlateMove::initMarster(const STMarsterItem &datas)
{
    qDebug() << Q_FUNC_INFO << datas.items.count();
    mLocker = false;
    if(mPlateCnt == datas.items.count()){//update
        QList<STPMBlockItem> items = datas.items;
        for(int i=0; i<items.count(); ++i) {
            for(int j=0; j<m_btns.count(); ++j) {
                if(m_btns[j]->itemIId() == items[i].stock_code){
                    qDebug() << Q_FUNC_INFO << m_btns[j]->itemIId();
                    m_btns[j]->setContent(items[i]);
                }
            }
        }
    } else {//create
        //防止数据重叠出现
        auto it = m_btns.begin();
        for(; it!=m_btns.end(); ++it)
        {
            (*it)->setParent(NULL);
            delete *it;
        }

        m_btns.clear();
        ui->right_layout->removeItem(m_vLayout);

        //m_btns.clear();
        m_vLayout = new QVBoxLayout;
        m_vLayout->setSpacing(1);
        QList<STPMBlockItem> items = datas.items;
        mPlateCnt = items.count();
        for(int i=0; i<items.count(); ++i) {
            HZItemButton *btn = new HZItemButton(items[i], items[i].is_marster, this);
            btn->setIsMarster(items[i].is_marster);
            m_vLayout->addWidget(btn);
            m_btns.append(btn);
        }

        ui->right_layout->addLayout(m_vLayout);
        ui->right_layout->setSpacing(1);
    }

//    update();
    ui->scrollArea->viewport()->update();
    mLocker = true;
}

void HZPlateMove::paintEvent(QPaintEvent *event)
{
    __super::paintEvent(event);
    ui->lab_slider->clear();
    if(mLocker && !m_btns.isEmpty()) {
        for(HZItemButton *btn : m_btns) {
            if(btn && btn->isMarster()) {
                int y = btn->mapToParent(QPoint(0,0)).y();
                STVLineData data={QPoint(XPOS_VAR+4, y+8/*(btn->height()+5)/2*/), btn->itemTime()};
                ui->lab_slider->addData(data);
            }
        }
    }
}

void HZPlateMove::platDataCallBack(ReqId id, const HZData::BlockAbnormalInfo &info, ErrCode code)
{
    STMarsterItem records;
    QVector<HZData::BlockAbnormal> plates = info.blockAbnormals;
    int max = qMin(20, plates.count());
    for(int k=0; k<max; ++k){
        QList<STPMBlockItem> items;
        const HZData::BlockAbnormal block = plates[k];
        for(int i=0; i<block.count(); ++i) {
            STPMBlockItem item;
            item.is_marster     = (i==0)?true:false;
            item.stock_name     = QString::fromStdString(block[i].stock_name);
            item.stock_code     = QString::fromStdString(block[i].stock_code);
            item.update_time    = block[i].update_time;
            item.up_down_price  = block[i].up_down_price;
            item.rate           = block[i].rate*100;
            items.append(item);
        }
        records.items << items;
    }

    initMarster(records);
}


///
/// \brief HZVLineLabel::HZVLineLabel
/// \param parent
///
HZVLineLabel::HZVLineLabel(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("HZVLineLabel");
}

void HZVLineLabel::addData(const STVLineData &data)
{
    if(!m_datas.contains(data)){
        m_datas.append(data);
    }
}

void HZVLineLabel::clear()
{
    m_datas.clear();
}


void HZVLineLabel::paintEvent(QPaintEvent *e)
{
    __super::paintEvent(e);
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    for(const STVLineData &data : m_datas) {
        int rx=4, ry=rx;
        paint.setPen(QPen(QColor("#272727"), 1));
        paint.drawLine(data.pos, QPoint(data.pos.x(),height()));
        paint.setPen(QPen(QColor("#54606b"), 2));
        paint.setBrush(QColor("#0e0e0e"));
        paint.drawEllipse(data.pos, rx, ry);
        paint.setPen(QColor("#adadad"));
        paint.drawText(QRect(0, data.pos.y()-ry-2, XPOS_VAR-rx+5, ry*2+6), Qt::AlignHCenter | Qt::AlignLeft, data.time);
    }
}


///
/// \brief HZItemButton::HZItemButton
/// \param parent
///
HZItemButton::HZItemButton(const STPMBlockItem &content, bool isMarster, QWidget *parent)
    :BaseWidget(parent)
    ,m_content(content)
    ,m_isMarster(false)
{
    setObjectName("HZItemButton");
    installEventFilter(this);
#if 1
    if(isMarster){
        setFixedHeight(35);
        QHBoxLayout *hLayout = new QHBoxLayout;
        HZStockLabel* mLabTitle = new HZStockLabel(content.stock_name,content.stock_code);
        mLabRate = new HZLabelDisplayItem(content.rate, "%");
        QLabel *quickUp = new QLabel(QStringLiteral("快速拉升"));
        quickUp->setObjectName("QuickUp");
        quickUp->setFixedSize(60,19);
        quickUp->setAlignment(Qt::AlignCenter);
        quickUp->setStyleSheet("background:#f4a122;color:#0e0e0e;");
        //HZLabelDisplayItem* mLabUpDown = new HZLabelDisplayItem(content.up_down_price);
        QSpacerItem *space1 = new QSpacerItem(9, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
        hLayout->addWidget(mLabTitle);
        hLayout->addSpacerItem(space1);
        hLayout->addWidget(mLabRate);

        QSpacerItem *space2 = new QSpacerItem(6, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
        hLayout->addSpacerItem(space2);
        hLayout->addWidget(quickUp);
        mLabTitle->setObjectName("LabTitle");

        QSpacerItem *space3 = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
        QHBoxLayout *childLayout = new QHBoxLayout;
        childLayout->addSpacerItem(space3);
        hLayout->addLayout(childLayout);

        setLayout(hLayout);
        layout()->setContentsMargins(14,0,7,0);
    }else{
        setFixedHeight(19);
        QHBoxLayout *hLayout = new QHBoxLayout;
        HZStockLabel *mLabTitle = new HZStockLabel(content.stock_name, content.stock_code);
        mLabRate = new HZLabelDisplayItem(content.rate,"%");
        mLabUpDown = new HZLabelDisplayItem(content.up_down_price);

        QHBoxLayout *childLayout = new QHBoxLayout;
        childLayout->setSpacing(12);
        childLayout->setContentsMargins(0,0,7,0);
        childLayout->addWidget(mLabUpDown);
        childLayout->addWidget(mLabRate);

        QSpacerItem *space = new QSpacerItem(36, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
        hLayout->addWidget(mLabTitle);
        hLayout->addSpacerItem(space);
        hLayout->addLayout(childLayout);

        hLayout->setSpacing(12);
        mLabTitle->setObjectName("LabTitle");
        setLayout(hLayout);
        layout()->setContentsMargins(14,0,7,0);
    }
#endif
}

HZItemButton::~HZItemButton()
{
}

void HZItemButton::setContent(const STPMBlockItem &content)
{
    m_content = content;
    mLabRate->updateDate(content.rate,"%");
    if(!m_isMarster)
        mLabUpDown->updateDate(content.up_down_price);
    update();
}

QString HZItemButton::itemTime() const
{
    return QDateTime::fromTime_t(m_content.update_time).toString("hh:mm");
}

bool HZItemButton::isMarster() const
{
    return m_isMarster;
}

void HZItemButton::setIsMarster(bool isMarster)
{
    m_isMarster = isMarster;
}

QString HZItemButton::itemIId() const
{
    return m_content.stock_code;
}

//void HZItemButton::mousePressEvent(QMouseEvent *e)
//{
//    e->accept();
//    if(e->button() == Qt::LeftButton){
//        ///emit signalItemClicked(m_content.stock_code);

//        MessageParams params;
//        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
//        params.emplace(STOCK_ID,m_content.stock_code);
//        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
//        sendMessage(ROOT_FRAME,params,true);
//    }
//}

HZPlateMoveMarster::HZPlateMoveMarster(QWidget *parent)
    :BaseWidget(parent)
    ,m_plateMove(new HZPlateMove)
    ,m_title(new QLabel(QStringLiteral(" 异动详情")))
{
    setObjectName("HZPlateMoveMarster");
    m_title->setFixedHeight(26);
    m_title->setObjectName("Move_Title");
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);

    vLayout->addWidget(m_title);
    vLayout->addWidget(m_plateMove);
    setLayout(vLayout);
    layout()->setContentsMargins(0,0,0,0);

    loadStyle();

    mTimer.setInterval(2000);
    connect(&mTimer, &QTimer::timeout, [=]{
        qDebug() << Q_FUNC_INFO << " req timer: "  ;
        m_plateMove->request();
    });
}

HZPlateMoveMarster::~HZPlateMoveMarster()
{

}

void HZPlateMoveMarster::afterActive()
{
    //执行一次板块异动请求
    //m_plateMove->request();
    mTimer.start();
}

void HZPlateMoveMarster::afterDeactive()
{
    //取消订阅
    mTimer.stop();
    m_plateMove->cancleSub();
}

void HZPlateMoveMarster::loadStyle()
{
//    QFile file(":/img/style.css");
//    if(!file.open(QIODevice::ReadOnly)) {
//        qDebug() << Q_FUNC_INFO << file.errorString();
//        return;
//    }
//    setStyleSheet(file.readAll());
//    file.close();
}

HZStockLabel::HZStockLabel(const QString &stockname,const QString &stockcode,QWidget *parent)
    :QLabel(stockname,parent)
    ,mStockCode(stockcode)
{
    setMouseTracking(true);
    installEventFilter(this);
}

bool HZStockLabel::eventFilter(QObject *watched, QEvent *event)
{
    if(this==watched) {
        if(event->type() == QEvent::Enter) {
            setCursor(QCursor(Qt::PointingHandCursor));
        } else if(event->type() == QEvent::Leave) {
            setCursor(QCursor(Qt::ArrowCursor));
        } else if(event->type() == QEvent::MouseButtonPress
                  || event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *ev = static_cast<QMouseEvent*>(event);
            if(ev) {
                if(ev->button() == Qt::LeftButton){
                    MessageParams params;
                    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
                    params.emplace(STOCK_ID,mStockCode);
                    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
                    sendMessage(ROOT_FRAME,params,true);
                }
            }
        }
    }

    return QLabel::eventFilter(watched,event);
}


