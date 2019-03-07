#include "marketmaster.h"
#include "ui_marketmaster.h"
#include <QtWidgets>
#include <QtCore>
#include <QtMath>

#include <hzinfocenter.h>
#include "updatinterface.h"
#include "infostyle.h"

///
/// \brief MarketDelegate::MarketDelegate
/// \param type
/// \param parent
///
MarketDelegate::MarketDelegate(int type,  MarketView *view, QObject *parent) :
    QStyledItemDelegate(parent)
    ,m_size(439, 87)
  ,m_view(view)
  ,m_type(type)
{


}

MarketDelegate::~MarketDelegate()
{
}


void MarketDelegate::setSize(const QSize &size)
{
    if(size.isValid()) m_size = size;
}

void MarketDelegate::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if(!index.isValid()){
        QStyledItemDelegate::paint(painter,opt,index);
        return;
    }

    int offsetX=30;
    if(m_type == FHPX_Item) { 
        QRect tmpRt;
        QVariant var = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM);
        MarketCalendar::FHPXData dts = var.value<MarketCalendar::FHPXData>();
        QRect rt(opt.rect.adjusted(offsetX,3,0,-3));
        rt.setWidth(200);
        bool isHover = opt.state.testFlag(QStyle::State_Selected) || opt.state.testFlag(QStyle::State_MouseOver);
        if(isHover) {
            painter->setPen(gInfoStyle->m_mkCldItemHoverPen);
            painter->setBrush(gInfoStyle->m_mkCldItemHoverBrush);
        } else {
            painter->setPen(gInfoStyle->m_mkCldItemNormalPen);
            painter->setBrush(gInfoStyle->m_mkCldItemNormalBrush);
        }

        tmpRt = rt;
        painter->drawRect(rt);
        painter->setPen(gInfoStyle->m_mkCldItemStockNamePen);
        painter->setFont(gInfoStyle->m_mkCldItemStockNameFont);

        int nowW, chgW, titleW;
        QRect itemRt;
        int isInValidData = false;
        int stockW = painter->fontMetrics().width(dts.stockname);
        if(dts.now == DBL_MAX) {
            isInValidData = true;
            nowW = painter->fontMetrics().width("--");
            chgW = painter->fontMetrics().width("--%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        } else {
            nowW = painter->fontMetrics().width(QString::number(dts.now,'f',2));
            chgW = painter->fontMetrics().width(QString::number(dts.chg,'f',2)+"%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        }

        //stock now chg
        painter->drawText(itemRt, Qt::AlignCenter, dts.stockname);
        painter->setFont(gInfoStyle->m_titleFont);
        itemRt.setRect( titleW + stockW+ 19, rt.y(), nowW, rt.height());
        painter->setPen(isInValidData? gInfoStyle->m_noUpDown:(dts.chg>0.?gInfoStyle->m_upPen:dts.chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown));
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.now,'f',2));
        itemRt.setRect(titleW + stockW+ nowW + 33, rt.y(), chgW, rt.height());
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.chg,'f',2)+"%");

        //detail
        rt.setX(216+offsetX);
        rt.setWidth(opt.rect.width()-216-offsetX);
        QString details = QString("%1,%2,%3")
                .arg(QStringLiteral("该年股权登记日:")+dts.record_date)
                .arg(QStringLiteral("派息日:")+dts.dividend_date)
                .arg(QStringLiteral("详情:")+dts.detail);
        painter->setPen(gInfoStyle->m_marketCldDetailPen);
        painter->setFont(gInfoStyle->m_marketCldDetailFont);
        painter->drawText(rt, Qt::AlignVCenter|Qt::AlignLeft, details);
        if(m_pressFlg && tmpRt.contains(m_pressed)) {
            qDebug() << Q_FUNC_INFO << "stock code:" << dts.stockcode;
            m_pressFlg = false;
            QTimer::singleShot(0,[=]{gInfoStyle->signalStockClicked(dts.stockcode);});
        }
    } else if(m_type == TFP_Item){
        QRect tmpRt;
        MarketCalendar::TFPData dts = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<MarketCalendar::TFPData>();
        QRect rt(opt.rect.adjusted(offsetX,3,0,-3));
        rt.setWidth(200);
        bool isHover = opt.state.testFlag(QStyle::State_Selected) || opt.state.testFlag(QStyle::State_MouseOver);
        if(isHover) {
            painter->setPen(gInfoStyle->m_mkCldItemHoverPen);
            painter->setBrush(gInfoStyle->m_mkCldItemHoverBrush);
        } else {
            painter->setPen(gInfoStyle->m_mkCldItemNormalPen);
            painter->setBrush(gInfoStyle->m_mkCldItemNormalBrush);
        }

        tmpRt = rt;
        painter->drawRect(rt);
        painter->setPen(gInfoStyle->m_mkCldItemStockNamePen);
        painter->setFont(gInfoStyle->m_mkCldItemStockNameFont);

        int nowW, chgW, titleW;
        QRect itemRt;
        int isInValidData = false;
        int stockW = painter->fontMetrics().width(dts.stockname);
        if(dts.now == DBL_MAX) {
            isInValidData = true;
            nowW = painter->fontMetrics().width("--");
            chgW = painter->fontMetrics().width("--%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        } else {
            nowW = painter->fontMetrics().width(QString::number(dts.now,'f',2));
            chgW = painter->fontMetrics().width(QString::number(dts.chg,'f',2)+"%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        }

        painter->drawText(itemRt, Qt::AlignCenter, dts.stockname);
        painter->setFont(gInfoStyle->m_titleFont);
        itemRt.setRect( titleW + stockW+ 19, rt.y(), nowW, rt.height());
        painter->setPen(isInValidData?gInfoStyle->m_noUpDown:(dts.chg>0.?gInfoStyle->m_upPen:dts.chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown));
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.now,'f',2));
        itemRt.setRect(titleW + stockW+ nowW + 33, rt.y(), chgW, rt.height());
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.chg,'f',2)+"%");

        rt.setX(216+offsetX);
        rt.setWidth(opt.rect.width()-216-offsetX);
        QString details = QString("%1,%2,%3")
                .arg(QStringLiteral("停牌时间:")+dts.suspension_date)
                .arg(QStringLiteral("复牌时间:")+dts.resumption_date)
                .arg(QStringLiteral("停牌原因:")+dts.reason);
        painter->setPen(gInfoStyle->m_marketCldDetailPen);
        painter->setFont(gInfoStyle->m_marketCldDetailFont);
        painter->drawText(rt, Qt::AlignVCenter|Qt::AlignLeft, details);
        if(m_pressFlg && tmpRt.contains(m_pressed)) {
            qDebug() << Q_FUNC_INFO << "stock code:" << dts.stockcode;
            m_pressFlg = false;
            QTimer::singleShot(0,[=]{gInfoStyle->signalStockClicked(dts.stockcode);});
        }
    } else if(m_type == SXJJ_Item){
        QRect tmpRt;
        MarketCalendar::XSJJData dts = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<MarketCalendar::XSJJData>();
        QRect rt(opt.rect.adjusted(offsetX,3,0,-3));
        rt.setWidth(200);
        bool isHover = opt.state.testFlag(QStyle::State_Selected) || opt.state.testFlag(QStyle::State_MouseOver);
        if(isHover) {
            painter->setPen(gInfoStyle->m_mkCldItemHoverPen);
            painter->setBrush(gInfoStyle->m_mkCldItemHoverBrush);
        } else {
            painter->setPen(gInfoStyle->m_mkCldItemNormalPen);
            painter->setBrush(gInfoStyle->m_mkCldItemNormalBrush);
        }

        tmpRt = rt;
        painter->drawRect(rt);
        painter->setPen(gInfoStyle->m_mkCldItemStockNamePen);
        painter->setFont(gInfoStyle->m_mkCldItemStockNameFont);

        int nowW, chgW, titleW;
        QRect itemRt;
        int isInValidData = false;
        int stockW = painter->fontMetrics().width(dts.stockname);
        if(dts.now == DBL_MAX) {
            isInValidData = true;
            nowW = painter->fontMetrics().width("--");
            chgW = painter->fontMetrics().width("--%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        } else {
            nowW = painter->fontMetrics().width(QString::number(dts.now,'f',2));
            chgW = painter->fontMetrics().width(QString::number(dts.chg,'f',2)+"%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        }

        painter->drawText(itemRt, Qt::AlignCenter, dts.stockname);
        painter->setFont(gInfoStyle->m_titleFont);
        itemRt.setRect( titleW + stockW+ 19, rt.y(), nowW, rt.height());
        painter->setPen(isInValidData?gInfoStyle->m_noUpDown:(dts.chg>0.?gInfoStyle->m_upPen:dts.chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown));
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.now,'f',2));
        itemRt.setRect(titleW + stockW+ nowW + 33, rt.y(), chgW, rt.height());
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.chg,'f',2)+"%");

        rt.setX(216+offsetX);
        rt.setWidth(opt.rect.width()-216-offsetX);
        QString details = QString("%1,%2").arg(QStringLiteral("解禁数量")+dts.scale).arg(QStringLiteral("占流通股比例:")+dts.lifted);
        painter->setPen(gInfoStyle->m_marketCldDetailPen);
        painter->setFont(gInfoStyle->m_marketCldDetailFont);
        painter->drawText(rt, Qt::AlignVCenter|Qt::AlignLeft, details);
        if(m_pressFlg && tmpRt.contains(m_pressed)) {
            qDebug() << Q_FUNC_INFO << "stock code:" << dts.stockcode;
            m_pressFlg = false;
            QTimer::singleShot(0,[=]{gInfoStyle->signalStockClicked(dts.stockcode);});
        }
    } else if(m_type == ZF_Item){
        QRect tmpRt;
        MarketCalendar::ZFData dts = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<MarketCalendar::ZFData>();
        QRect rt(opt.rect.adjusted(offsetX,3,0,-3));
        rt.setWidth(200);
        bool isHover = opt.state.testFlag(QStyle::State_Selected) || opt.state.testFlag(QStyle::State_MouseOver);
        if(isHover) {
            painter->setPen(gInfoStyle->m_mkCldItemHoverPen);
            painter->setBrush(gInfoStyle->m_mkCldItemHoverBrush);
        } else {
            painter->setPen(gInfoStyle->m_mkCldItemNormalPen);
            painter->setBrush(gInfoStyle->m_mkCldItemNormalBrush);
        }
        tmpRt = rt;
        painter->drawRect(rt);
        painter->setPen(gInfoStyle->m_mkCldItemStockNamePen);
        painter->setFont(gInfoStyle->m_mkCldItemStockNameFont);

        int nowW, chgW, titleW;
        QRect itemRt;
        int isInValidData = false;
        int stockW = painter->fontMetrics().width(dts.stockname);
        if(dts.now == DBL_MAX) {
            isInValidData = true;
            nowW = painter->fontMetrics().width("--");
            chgW = painter->fontMetrics().width("--%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        } else {
            nowW = painter->fontMetrics().width(QString::number(dts.now,'f',2));
            chgW = painter->fontMetrics().width(QString::number(dts.chg,'f',2)+"%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        }

        painter->drawText(itemRt, Qt::AlignCenter, dts.stockname);
        painter->setFont(gInfoStyle->m_titleFont);
        itemRt.setRect( titleW + stockW+ 19, rt.y(), nowW, rt.height());
        painter->setPen(isInValidData?gInfoStyle->m_noUpDown:(dts.chg>0.?gInfoStyle->m_upPen:dts.chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown));
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.now,'f',2));
        itemRt.setRect(titleW + stockW+ nowW + 33, rt.y(), chgW, rt.height());
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.chg,'f',2)+"%");

        rt.setX(216+offsetX);
        rt.setWidth(opt.rect.width()-216-offsetX);
        QString details = QString("%1,%2,%3")
                .arg(QStringLiteral("发行价:")+dts.issue_price)
                .arg(QStringLiteral("发行数量:")+dts.issue_num)
                .arg(QStringLiteral("增发上市日:")+dts.issue_date);
        painter->setPen(gInfoStyle->m_marketCldDetailPen);
        painter->setFont(gInfoStyle->m_marketCldDetailFont);
        painter->drawText(rt, Qt::AlignVCenter|Qt::AlignLeft, details);
        if(m_pressFlg && tmpRt.contains(m_pressed)) {
            qDebug() << Q_FUNC_INFO << "stock code:" << dts.stockcode;
            m_pressFlg = false;
            QTimer::singleShot(0,[=]{gInfoStyle->signalStockClicked(dts.stockcode);});
        }
    } else if(m_type == YJZF_Item){
        QRect tmpRt;
        MarketCalendar::YJPLData dts = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<MarketCalendar::YJPLData>();
        QRect rt(opt.rect.adjusted(offsetX,3,0,-3));
        rt.setWidth(200);
        bool isHover = opt.state.testFlag(QStyle::State_Selected) || opt.state.testFlag(QStyle::State_MouseOver);
        if(isHover) {
            painter->setPen(gInfoStyle->m_mkCldItemHoverPen);
            painter->setBrush(gInfoStyle->m_mkCldItemHoverBrush);
        } else {
            painter->setPen(gInfoStyle->m_mkCldItemNormalPen);
            painter->setBrush(gInfoStyle->m_mkCldItemNormalBrush);
        }
        tmpRt = rt;
        painter->drawRect(rt);
        painter->setPen(gInfoStyle->m_mkCldItemStockNamePen);
        painter->setFont(gInfoStyle->m_mkCldItemStockNameFont);

        int nowW, chgW, titleW;
        QRect itemRt;
        int isInValidData = false;
        int stockW = painter->fontMetrics().width(dts.stockname);
        if(dts.now == DBL_MAX) {
            isInValidData = true;
            nowW = painter->fontMetrics().width("--");
            chgW = painter->fontMetrics().width("--%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        } else {
            nowW = painter->fontMetrics().width(QString::number(dts.now,'f',2));
            chgW = painter->fontMetrics().width(QString::number(dts.chg,'f',2)+"%");
            titleW = (rt.width()-stockW-nowW-chgW)/2 - 16+offsetX;
            itemRt.setRect(titleW, rt.y(), stockW, rt.height());
        }

        painter->drawText(itemRt, Qt::AlignCenter, dts.stockname);
        painter->setFont(gInfoStyle->m_titleFont);
        itemRt.setRect( titleW + stockW+ 19, rt.y(), nowW, rt.height());
        painter->setPen(isInValidData?gInfoStyle->m_noUpDown:(dts.chg>0.?gInfoStyle->m_upPen:dts.chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown));
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.now,'f',2));
        itemRt.setRect(titleW + stockW+ nowW + 33, rt.y(), chgW, rt.height());
        painter->drawText(itemRt, Qt::AlignCenter, isInValidData?"--":QString::number(dts.chg,'f',2)+"%");

        rt.setX(216+offsetX);
        rt.setWidth(opt.rect.width()-216-offsetX);
        QString details = QString("%1,%2,%3,%4")
                .arg(QStringLiteral("每股收益:")+dts.eps)
                .arg(QStringLiteral("每股净资产:")+dts.naps)
                .arg(QStringLiteral("净资产收益率:")+dts.roe)
                .arg(QStringLiteral("净利润:")+dts.net_profit);
        painter->setPen(gInfoStyle->m_marketCldDetailPen);
        painter->setFont(gInfoStyle->m_marketCldDetailFont);
        painter->drawText(rt, Qt::AlignVCenter|Qt::AlignLeft, details);
        if(m_pressFlg && tmpRt.contains(m_pressed)) {
            qDebug() << Q_FUNC_INFO << "stock code:" << dts.stockcode;
            m_pressFlg = false;
            QTimer::singleShot(0,[=]{gInfoStyle->signalStockClicked(dts.stockcode);});
        }
    }
}

QSize MarketDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(m_size.width(), 46);
}

bool MarketDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if(me) {
            m_pressFlg = true;
            m_pressed = me->pos();
        }
        return true;
    }

    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

///
/// \brief MarketModel::MarketModel
/// \param type
/// \param parent
///
MarketModel::MarketModel(int type, QObject *parent)
    :QAbstractListModel(parent)
    ,m_type(type)
{

}

MarketModel::~MarketModel()
{

}

void MarketModel::addRecords(const MarketCalendar &items)
{
    m_records = items;
    flush();
    emit signalDataChanged();
}

int MarketModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(m_type == FHPX_Item){
        return m_records.FHPXDatas.count();
    } else if(m_type == TFP_Item){
        return m_records.TFPDatas.count();
    } else if(m_type == SXJJ_Item){
        return m_records.XSJJDatas.count();
    } else if(m_type == ZF_Item){
        return m_records.ZFDatas.count();
    } else if(m_type == YJZF_Item){
        return m_records.YJPLDatas.count();
    }
    return 0;
}

QVariant MarketModel::data(const QModelIndex &index, int role) const
{
    if(m_type == FHPX_Item){
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            if(index.isValid() && !m_records.FHPXDatas.isEmpty()
                    && (index.row()>=0 && index.row() < m_records.FHPXDatas.count()))
            {
                MarketCalendar::FHPXData data = m_records.FHPXDatas[index.row()];
                return QVariant::fromValue(data);
            }
        }
    } else if(m_type == TFP_Item){
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            if(index.isValid() && !m_records.TFPDatas.isEmpty()
                    && (index.row()>=0 && index.row() < m_records.TFPDatas.count())){
                return QVariant::fromValue(m_records.TFPDatas[index.row()]);
            }
        }
    } else if(m_type == SXJJ_Item){
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            if(index.isValid() && !m_records.XSJJDatas.isEmpty()
                    && (index.row()>=0 && index.row() < m_records.XSJJDatas.count())){
                return QVariant::fromValue(m_records.XSJJDatas[index.row()]);
            }
        }
    } else if(m_type == ZF_Item){
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            if(index.isValid() && !m_records.ZFDatas.isEmpty()
                    && (index.row()>=0 && index.row() < m_records.ZFDatas.count())){
            return QVariant::fromValue(m_records.ZFDatas[index.row()]);
            }
        }
    } else if(m_type == YJZF_Item){
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            if(index.isValid() && !m_records.YJPLDatas.isEmpty()
                    && (index.row()>=0 && index.row() < m_records.YJPLDatas.count())){
            return QVariant::fromValue(m_records.YJPLDatas[index.row()]); 
            }
    }
    }
    return QVariant();
}

void MarketModel::flush()
{
    beginResetModel();
    endResetModel();
}

///
/// \brief MarketView::MarketView
/// \param type
/// \param parent
///
MarketView::MarketView(int type, QWidget *parent)
    :QListView(parent)
    ,m_smartModel(new MarketModel(type))
    ,m_smartDelegate(new MarketDelegate(type,this))
{
    setObjectName("MarketView");
    setMouseTracking(false);
    setSpacing(0);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeMode(QListView::Adjust);
    setViewMode(QListView::IconMode);
    setWordWrap(true);

    setItemDelegate(m_smartDelegate);
    setModel(m_smartModel);
    initData();

    connect(m_smartDelegate, &MarketDelegate::signalStockClicked, [=](const QString &stockcode){
        qDebug() << Q_FUNC_INFO << "stockcode" << stockcode;
    });
}

MarketView::~MarketView()
{

}

void MarketView::addRecord(const MarketCalendar &item)
{
    if(m_smartModel) {
        m_smartModel->addRecords(item);
    }
}

void MarketView::stipToStockDetail(const QString &stockcode)
{
    //qDebug() << Q_FUNC_INFO <<"stockcode:" << stockcode;
    gInfoStyle->signalStockClicked(stockcode);
}

void MarketView::slotScrollChanged(int value)
{
    this->verticalScrollBar()->setValue(value);
}

void MarketView::resizeEvent(QResizeEvent *e)
{
    m_smartDelegate->setSize(e->size());
    QListView::resizeEvent(e);
}

void MarketView::paintEvent(QPaintEvent *e)
{
    if(m_smartModel){
        bool isEmpty = m_smartModel->rowCount()==0?1:0;
        if(isEmpty){
            QPainter painter(viewport());
            painter.fillRect(rect(), gInfoStyle->m_invalidBrush);
            painter.setPen(Qt::white);
            painter.setFont(gInfoStyle->m_invalidFont);
            painter.drawText(rect(), Qt::AlignCenter,QStringLiteral("暂无数据"));
        }else{
            QPainter painter(viewport());
            painter.fillRect(rect(),gInfoStyle->m_marketCldDetailBrush);
            QListView::paintEvent(e);
        }
    }
}

void MarketView::initData()
{

}

///
/// \brief MarketItem::MarketItem
/// \param type
/// \param title
/// \param parent
///
MarketItem::MarketItem(int type, const QString &title, QWidget *parent)
    :QFrame(parent)
{
    setFixedHeight(183);
    m_title = new Infoitemtitle(type, title);
    m_marketView = new MarketView(type);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);

    QHBoxLayout *hb = new QHBoxLayout;
    hb->setContentsMargins(0, 0, 0, 0);
    hb->setSpacing(0);
    hb->addWidget(m_marketView);

    vLayout->addWidget(m_title);
    vLayout->addLayout(hb);
    setLayout(vLayout);
}

void MarketItem::setData(const MarketCalendar &var, int num)
{
    m_title->setRecordNum(num);
    m_marketView->addRecord(var);
}




///
/// \brief MarketMaster::MarketMaster
/// \param adapter
/// \param parent
///
MarketMaster::MarketMaster(IUpdateRequest *adapter, QWidget *parent)
    : BasePage(parent)
    ,ui(new Ui::MarketMaster)
    ,m_iURequest(adapter)
{
    ui->setupUi(this);

    //日历
    m_calandar = new CalandarBarMaster;
    m_calandar->setObjectName("CalandarBarMaster");
    m_calandar->setFixedHeight(28);

    //子页
    m_FHPXPage   = new MarketItem(FHPX_Item, QStringLiteral("分红派息"));
    m_TFPPage    = new MarketItem(TFP_Item, QStringLiteral("停复牌"));
    m_SXJJPage   = new MarketItem(SXJJ_Item, QStringLiteral("限售解禁"));
    m_ZFPage     = new MarketItem(ZF_Item, QStringLiteral("增发"));
    m_YJZFPage   = new MarketItem(YJZF_Item, QStringLiteral("业绩披露"));

    ui->horizontalLayout->addWidget(m_calandar);
    ui->verticalLayout->addWidget(m_FHPXPage);
    ui->verticalLayout->addWidget(m_TFPPage);
    ui->verticalLayout->addWidget(m_SXJJPage);
    ui->verticalLayout->addWidget(m_ZFPage);
    ui->verticalLayout->addWidget(m_YJZFPage);

    m_iURequest->setVisitor(this);
    connect(m_calandar, &CalandarBarMaster::signalCalandarBarClicked, [&]{
        QString dateStr = m_calandar->getCurrentDate();
        startRequest(dateStr);
    });

    startTimesReq();
}

MarketMaster::~MarketMaster()
{
    delete ui;
    if(m_timesReq != -1)
        HzInfoCenter::getInstance()->cancle(m_timesReq);
}

void MarketMaster::startTimesReq()
{
    MarketCalandarTimesCallBack fun = std::bind(&MarketMaster::marketCalandarInfoTimesCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_timesReq = HzInfoCenter::getInstance()->reqMarketCalandarTimes(fun);
}

void MarketMaster::setAdapter(IUpdateRequest *adapter)
{
    //todo
}

void MarketMaster::updateList(const QVariantMap &var)
{
    MarketCalendar mMkDates = var["data"].value<MarketCalendar>();
    m_FHPXPage->setData(mMkDates, mMkDates.FHPXDatas.count());
    m_TFPPage->setData(mMkDates, mMkDates.TFPDatas.count());
    m_SXJJPage->setData(mMkDates, mMkDates.XSJJDatas.count());
    m_ZFPage->setData(mMkDates, mMkDates.ZFDatas.count());
    m_YJZFPage->setData(mMkDates, mMkDates.YJPLDatas.count());
    startRequestSnapShot(QVariant::fromValue(mMkDates));
}

void MarketMaster::updateTimes(const QVector<quint64> &times)
{
    m_calandar->setValidTimes(times);
}

void MarketMaster::updateDetails(const QVariant &var)
{
    //todo
}

void MarketMaster::afterActive()
{
    m_iURequest->initializatItems(false,QVariant::fromValue(m_calandar->getCurrentDate()));
}

void MarketMaster::afterDeactive()
{
    m_iURequest->cancleReq();
}

void MarketMaster::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MarketMaster::startRequest(const QString &date)
{
    if(date.isEmpty()) return;
    QVariantMap map;
    map["date"] = m_calandar->getCurrentDate();
    m_iURequest->requestListForParam(map);
}

void MarketMaster::startRequestSnapShot(const QVariant &var)
{
    QList<QString> stocks;
    m_MkDates = var.value<MarketCalendar>();
    if(!m_MkDates.FHPXDatas.isEmpty())
        for(const MarketCalendar::FHPXData &item:m_MkDates.FHPXDatas) stocks.append(item.stockcode);

    if(!m_MkDates.TFPDatas.isEmpty())
        for(const MarketCalendar::TFPData &item:m_MkDates.TFPDatas) stocks.append(item.stockcode);

    if(!m_MkDates.XSJJDatas.isEmpty())
        for(const MarketCalendar::XSJJData &item:m_MkDates.XSJJDatas) stocks.append(item.stockcode);

    if(!m_MkDates.ZFDatas.isEmpty())
        for(const MarketCalendar::ZFData &item:m_MkDates.ZFDatas) stocks.append(item.stockcode);

    if(!m_MkDates.YJPLDatas.isEmpty())
        for(const MarketCalendar::YJPLData &item:m_MkDates.YJPLDatas) stocks.append(item.stockcode);

    reqSnapShot(stocks);
}

void MarketMaster::reqSnapShot(const QList<QString> &stocks)
{
    std::list<std::string> stdlst;
    for(int i=0; i<stocks.count(); ++i)
        stdlst.push_back(stocks[i].toStdString());

    if(stdlst.size() == 0) return;
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stdlst;
    req.mask = HZData::SLStock_Mask;
    auto fun = std::bind(&MarketMaster::reqSnapShotResponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    DataController::instance()->requestOptionalDyna(req, fun);
}

void MarketMaster::reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err)
{
    if(err != 0) {
        qDebug() << Q_FUNC_INFO << "error:" << err;
        return;
    }

    QHash<QString, ItemDate> chgs;
    HZData::SLStock *slStocls = (HZData::SLStock *)data;
    if(slStocls) {
        for(int i=0; i<num; i++){
            QString name = QString::fromStdString(slStocls[i].stock_name);
            QString code = QString::fromStdString(slStocls[i].stock_code);
            double now = slStocls[i].last_price;
            double chg = slStocls[i].rate;
            ItemDate item = {name,code,now,chg};
            chgs[code] = item;
        }
    }

    for(int i=0; i<m_MkDates.FHPXDatas.count(); ++i) {
        QString code = m_MkDates.FHPXDatas[i].stockcode;
        if(chgs.contains(code)) {
            m_MkDates.FHPXDatas[i].chg = chgs[code].chg;
            m_MkDates.FHPXDatas[i].now = chgs[code].now;
        }
    }

    for(int i=0; i<m_MkDates.TFPDatas.count(); ++i) {
        QString code = m_MkDates.TFPDatas[i].stockcode;
        if(chgs.contains(code)) {
            m_MkDates.TFPDatas[i].chg = chgs[code].chg;
            m_MkDates.TFPDatas[i].now = chgs[code].now;
        }
    }

    for(int i=0; i<m_MkDates.XSJJDatas.count(); ++i) {
        QString code = m_MkDates.XSJJDatas[i].stockcode;
        if(chgs.contains(code)) {
            m_MkDates.XSJJDatas[i].chg = chgs[code].chg;
            m_MkDates.XSJJDatas[i].now = chgs[code].now;
        }
    }

    for(int i=0; i<m_MkDates.ZFDatas.count(); ++i) {
        QString code = m_MkDates.ZFDatas[i].stockcode;
        if(chgs.contains(code)) {
            m_MkDates.ZFDatas[i].chg = chgs[code].chg;
            m_MkDates.ZFDatas[i].now = chgs[code].now;
        }
    }

    for(int i=0; i<m_MkDates.YJPLDatas.count(); ++i) {
        QString code = m_MkDates.YJPLDatas[i].stockcode;
        if(chgs.contains(code)) {
            m_MkDates.YJPLDatas[i].chg = chgs[code].chg;
            m_MkDates.YJPLDatas[i].now = chgs[code].now;
        }
    }

    m_FHPXPage->setData(m_MkDates, m_MkDates.FHPXDatas.count());
    m_TFPPage->setData(m_MkDates, m_MkDates.TFPDatas.count());
    m_SXJJPage->setData(m_MkDates, m_MkDates.XSJJDatas.count());
    m_ZFPage->setData(m_MkDates, m_MkDates.ZFDatas.count());
    m_YJZFPage->setData(m_MkDates, m_MkDates.YJPLDatas.count());
}

void MarketMaster::marketCalandarInfoTimesCallBack(const InfoRet &ret, const MarketCalandarTimes &datas)
{
    if(ret.code == 0) {
        updateTimes(datas.datas);
    }
}
