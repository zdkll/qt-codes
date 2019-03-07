#include "economicmaster.h"
#include "ui_economicmaster.h"
#include <QtWidgets>
#include <QtCore>
#include <QtMath>
#include <QPixmap>
#include <QApplication>
#include <datacontroller.h>
#include <pubmsgdef.h>
#include <QTimer>
#include "hzinfodata.h"
#include "infostyle.h"
#include "financialreportdelegate.h"
#include "infodef.h"

#include <hzlog.h>
#include <hzinfocenter.h>

///
/// \brief EconomicDelegate::EconomicDelegate
/// \param type
/// \param parent
///
EconomicDelegate::EconomicDelegate(int type,QObject *parent) :
    QStyledItemDelegate(parent)
  ,m_type(type)
  ,m_size(439, 87)
  ,m_pressed(-1,-1)
  ,m_pressFlg(false)
  ,m_customPix(":/skin/default/img/infopage/zixuan.png")
{
    m_countryFlags = QMap<QString,QString>{
        {"中国", ":/skin/default/img/infopage/China.png"},
        {"欧盟", ":/skin/default/img/infopage/EU.png"},
        {"美国", ":/skin/default/img/infopage/USA.png"},
        {"日本", ":/skin/default/img/infopage/Japan.png"},
        {"今天", ":/skin/default/img/infopage/today.png"}
    };
}

EconomicDelegate::~EconomicDelegate()
{
}


void EconomicDelegate::setSize(const QSize &size)
{
    if(size.isValid()) m_size = size;
}

void EconomicDelegate::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if(!index.isValid()){
        QStyledItemDelegate::paint(painter,opt,index);
        return;
    }

    switch(m_type) {
    case MajorMeet_Item:
    {
        painter->save();
        const STEconmicMajorMeetUi &record = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STEconmicMajorMeetUi>();
        QRect rt(opt.rect.adjusted(79, 0,0,0));
        rt.setWidth(opt.rect.width()-81);
        painter->setPen(QColor("#2f2f2f"));
        painter->drawLine(rt.topLeft(), rt.bottomLeft());
        switch(record.type) {
        case 0:
        {
            //标题
            QRect titleRt(rt.adjusted(18,0,0,0));
            painter->setPen(gInfoStyle->m_meetTitle);
            painter->setFont(gInfoStyle->m_marketCldDetailFont);
            painter->drawText(titleRt, Qt::AlignLeft|Qt::AlignVCenter, record.title.isEmpty()?"--Not Title--":record.title);
            painter->setPen(QPen(gInfoStyle->m_mllipsePen,2));
            painter->setBrush(gInfoStyle->m_ellipseBrush);
            QPoint ellipsePt(rt.topLeft());
            ellipsePt.setY(rt.y()+rt.height()/2);
            painter->drawEllipse(ellipsePt, 5, 5);
            if(m_pressFlg && titleRt.contains(m_pressed)){
                QTimer::singleShot(0, [=]{gInfoStyle->signalFinaoReportDetail(QStringLiteral("重大会议"),record.url);});//url
                m_pressFlg = false;
            }
        }break;
        case 1:
        {
            //相关板块
            QString headText(QStringLiteral("相关板块"));
            int headW = painter->fontMetrics().width(headText);
            QFont font("SimSun");
            font.setPixelSize(14);
            font.setBold(true);
            painter->setFont(font);
            painter->setPen(QColor("#cad4dc"));
            painter->drawText(rt.adjusted(18,0,0,0), Qt::AlignLeft|Qt::AlignVCenter, headText);

            font.setPixelSize(13);
            font.setBold(false);
            painter->setFont(font);
            painter->setPen(QColor("#e62727"));

            int textW = 0;
            QRect boardRt(rt.adjusted(18+headW+35,0,0,0));
            QVector<STEconmicMajorMeetUi::StockInfoUi> brds = record.rltBoard;
            for(int i=0; i<brds.count(); ++i) {
                bool isInvalidData = brds[i].chg == DBL_MAX?1:0;
                QPen clrPen = isInvalidData?gInfoStyle->m_noUpDown:(brds[i].chg > 0. ? gInfoStyle->m_upPen: brds[i].chg < 0. ? gInfoStyle->m_downPen : gInfoStyle->m_noUpDown);
                QRect tmpRt(boardRt);
                if(i == 0){
                    painter->setPen(clrPen);
                    //板块部分不用显示涨幅
                    //QString dtxt = brds[i].stockname+" "+QString::number(brds[i].chg,'f',2)+"%";
                    QString dtxt = brds[i].stockname;
                    int tw = painter->fontMetrics().width(dtxt);
                    tmpRt.setWidth(tw);
                    painter->drawText(tmpRt, Qt::AlignLeft|Qt::AlignVCenter, dtxt);
                } else {
                    //板块部分不用显示涨幅
                    //QString dtxtpre = brds[i-1].stockname+" "+QString::number(brds[i-1].chg,'f',2)+"%",
                    //        dtxt = brds[i].stockname+" "+QString::number(brds[i].chg,'f',2)+"%";
                    QString dtxtpre = brds[i-1].stockname,
                            dtxt = brds[i].stockname;
                    int tw = painter->fontMetrics().width(dtxt);
                    textW += painter->fontMetrics().width(dtxtpre) + 21;
                    painter->setPen(Qt::white);
                    painter->drawLine(QPoint(boardRt.x()+textW, boardRt.y()+boardRt.height()/2-5),
                                      QPoint(boardRt.x()+textW, boardRt.y()+boardRt.height()/2+5));
                    textW += 21;
                    tmpRt.setX(boardRt.x()+textW);
                    tmpRt.setWidth(tw);
                    painter->setPen(clrPen);
                    painter->drawText(tmpRt, Qt::AlignLeft|Qt::AlignVCenter, dtxt);
                }

#if 0 //板块部分不用跳转
                if(mPressFlg && tmpRt.contains(mPressed)){
                    qDebug() << Q_FUNC_INFO << "borads:" << brds[i].stockname;
                    gInfoStyle->signalStockClicked(brds[i].stockcode);
                }
#endif
            }
        }break;
        case 2:
        {
            //相关股票
            QString headText(QStringLiteral("相关股票"));
            int headW = painter->fontMetrics().width(headText);
            QFont font("SimSun");
            font.setPixelSize(14);
            font.setBold(true);
            painter->setFont(font);
            painter->setPen(QColor("#cad4dc"));
            painter->drawText(rt.adjusted(18,0,0,0), Qt::AlignLeft|Qt::AlignVCenter, headText);

            font.setPixelSize(13);
            font.setBold(false);
            painter->setFont(font);
            painter->setPen(QColor("#e62727"));

            QRect stockRt(rt.adjusted(18+headW+35,0,0,0));
            QRect tmpRt(stockRt);
            QString dtxt="";
            int textW=0;
            QVector<STEconmicMajorMeetUi::StockInfoUi> stks = record.rltStock;
            for(int i=0; i<stks.count(); ++i) {
                bool isInvalidData = stks[i].chg == DBL_MAX?1:0;
                QPen clrPen = isInvalidData?gInfoStyle->m_noUpDown:(stks[i].chg>0.?gInfoStyle->m_upPen:stks[i].chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown);
                if(i==0) {
                    dtxt = QString("%1(%2) %3%").arg(stks[i].stockname).arg(stks[i].stockcode).arg(isInvalidData?"--":QString::number(stks[i].chg,'f',2));
                    int dtxtW = painter->fontMetrics().width(dtxt);
                    tmpRt.setWidth(dtxtW);
                    clrPen = isInvalidData?gInfoStyle->m_noUpDown:(stks[i].chg>0.?gInfoStyle->m_upPen:stks[i].chg<0.?gInfoStyle->m_downPen:gInfoStyle->m_noUpDown);
                    painter->setPen(clrPen);
                    painter->drawText(tmpRt, Qt::AlignCenter, dtxt);
                    if(stks[i].isCustom){
                        QRect customRt(tmpRt.x()+3+dtxtW,tmpRt.y()+7,15,14);
                        if(!m_customPix.isNull())  painter->drawPixmap(customRt, m_customPix);
                    }
                }else {
                    QString dtxtpre = QString("%1(%2) %3%").arg(stks[i-1].stockname).arg(stks[i-1].stockcode).arg(isInvalidData?"--":QString::number(stks[i-1].chg,'f',2));
                    dtxt = QString("%1(%2) %3%").arg(stks[i].stockname).arg(stks[i].stockcode).arg(QString::number(stks[i].chg,'f',2));
                    int tw = painter->fontMetrics().width(dtxt);
                    int customw = 22;
                    textW += painter->fontMetrics().width(dtxtpre)+21;

                    if(textW+22+tw +customw >= stockRt.width()) break;
                    painter->setPen(Qt::white);
                    painter->drawLine(QPoint(stockRt.x()+textW, stockRt.y()+stockRt.height()/2-5),
                                      QPoint(stockRt.x()+textW, stockRt.y()+stockRt.height()/2+5));
                    textW += 21;
                    tmpRt.setX(stockRt.x()+textW);
                    tmpRt.setWidth(tw);
                    painter->setPen(clrPen);
                    painter->drawText(tmpRt, Qt::AlignLeft|Qt::AlignVCenter, dtxt);
                    if(stks[i].isCustom){
                        QRect customRt(tmpRt.x()+3+tw,tmpRt.y()+7,15,14);
                        if(!m_customPix.isNull())  painter->drawPixmap(customRt, m_customPix);
                    }
                }

                if(m_pressFlg && tmpRt.contains(m_pressed)){
                    QTimer::singleShot(0, [=]{gInfoStyle->signalStockClicked(stks[i].stockcode);});
                    m_pressFlg = false;
                }
            }
        }break;
        }
        painter->restore();
    }break;
    case FinancialReport_Item:{ /*TODO*/ }break;
    case EconomicData_Item:
    {
        const STEconomicDataUi &record = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STEconomicDataUi>();
        QRect rt(opt.rect.adjusted(79, 0,0,0));
        rt.setWidth(opt.rect.width()-81);
        painter->save();
        painter->setPen(QColor("#2f2f2f"));
        painter->drawLine(rt.topLeft(), rt.bottomLeft());

        QString timeText = QDateTime::fromTime_t(record.time).toString("hh:mm");
        int textLen = painter->fontMetrics().width(timeText), fonth=painter->fontMetrics().height();
        QRect textRt(rt.x()-10-textLen, rt.y()+(rt.height()-fonth)/2, textLen, fonth);
        switch(record.type)
        {
        case 0:
        {
            //draw title
            painter->setPen(QColor("#cad4dc"));
            painter->setFont(gInfoStyle->m_marketCldDetailFont);
            painter->drawText(rt.adjusted(18,0,0,0), Qt::AlignLeft|Qt::AlignVCenter, record.name.isEmpty()?"--Not Title--":record.name);

            //draw ellipse
            painter->setPen(QPen(QColor("#adadad"),2));
            painter->setBrush(QColor("#0e0e0e"));
            QPoint ellipsePt(rt.topLeft());
            ellipsePt.setY(rt.y()+rt.height()/2);
            painter->drawEllipse(ellipsePt, 5, 5);
            painter->setFont(gInfoStyle->m_descripFont);
            painter->drawText(textRt, Qt::AlignVCenter|Qt::AlignRight, timeText);
        }break;
        case 1:
        {
            textRt.setWidth(30);
            textRt.setHeight(20);
            if(!record.region.isEmpty() && m_countryFlags.contains(record.region)){
                QPixmap pixmap(m_countryFlags[record.region]);
                if(!pixmap.isNull()) painter->drawPixmap(textRt, pixmap);
            }

            int xpos = 18;
            QString publicValue = QString("%1: %2  ").arg(QStringLiteral("公布")).arg(record.act_value);
            int pbvWidth = painter->fontMetrics().width(publicValue);
            painter->setPen(Qt::red);
            painter->drawText(rt.adjusted(xpos,0,0,0), Qt::AlignLeft|Qt::AlignVCenter, publicValue);
            xpos += pbvWidth;

            //draw text
            QStringList texts;
            texts.append(QString("%1: %2  ").arg(QStringLiteral("前值")).arg(record.pre_value));
            texts.append(QString("%1: %2  ").arg(QStringLiteral("预测")).arg(record.exp_value));
            texts.append(QString("%1: %2  ").arg(QStringLiteral("来源")).arg(record.source));

            //draw title
            painter->setPen(QColor("#cad4dc"));
            painter->drawText(rt.adjusted(xpos,0,0,0), Qt::AlignLeft|Qt::AlignVCenter, texts.join(' '));
        }break;
        }
        painter->restore();
    }break;
    }
}

bool EconomicDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(m_type == MajorMeet_Item){
        if(event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if(me) {
                m_pressFlg = true;
                m_pressed = me->pos();
            }
            return true;
        } else if(event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if(me){
                m_pressFlg = false;
                m_pressed = QPoint(-1,-1);
            }
        } else if(event->type() == QEvent::MouseMove) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if(me) m_pressed = me->pos();
        }
    } else {
        return QStyledItemDelegate::editorEvent(event,model,option,index);
    }

    return false;
}

QSize EconomicDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch(m_type) {
    case MajorMeet_Item:{
        int itemH = 30;
        const STEconmicMajorMeetUi &record = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STEconmicMajorMeetUi>();
        if(record.type == 3) itemH = 10;
        return QSize(m_size.width(),itemH);
    }
    case FinancialReport_Item:
    {
        //TODO
    }
    case EconomicData_Item:
    {
        int itemH = 30;
        const STEconmicMajorMeetUi &record = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STEconmicMajorMeetUi>();
        if(record.type == 2) itemH = 10;
        return QSize(m_size.width(), itemH);
    }
    }

    return QSize(100,100);
}

///
/// \brief EconomicModel::EconomicModel
/// \param type
/// \param parent
///
EconomicModel::EconomicModel(int type, QObject *parent)
    :QAbstractListModel(parent)
    ,m_type(type)
{
    connect(this, &EconomicModel::signalDataChanged, [=]{
        startRequestSnapShot();
    });
}

void EconomicModel::addRecords(const QVector<STEconmicMajorMeetUi> &items)
{
    m_majors = items;
    flush();
    emit signalDataChanged();
}

void EconomicModel::addRecords(const QVector<FinancialReport::FinancialItem> &items)
{
    m_financialReps = items;
    flush();
    emit signalDataChanged();
}

void EconomicModel::addRecords(const QVector<STEconomicDataUi> &items)
{
    m_economicDatas = items;
    flush();
}

QVector<STEconmicMajorMeetUi> EconomicModel::getMajorMeetRecords()
{
    return m_majors;
}

QVector<FinancialReport::FinancialItem> EconomicModel::getFinancialRecords()
{
    return m_financialReps;
}

const QVector<STEconomicDataUi> EconomicModel::getEconomicRecords()
{
    return m_economicDatas;
}

void EconomicModel::setViewRect(const QRect &rc)
{
    m_viewRc = rc;
}

int EconomicModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    switch(m_type) {
    case MajorMeet_Item: return m_majors.count();
    case FinancialReport_Item:
    {
        int row =0,count=0;
        this->calcFinancialReportRowAndCount(row,count);
        return row;
    }
    case EconomicData_Item: return m_economicDatas.count();
    }
    return 0;
}

QVariant EconomicModel::data(const QModelIndex &index, int role) const
{
    switch(m_type) {
    case MajorMeet_Item:
    {
        int row = index.row();
        if(!index.isValid() || m_majors.isEmpty() || (row <0 || row > m_majors.count()-1)) return QVariant();
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role)
            return QVariant::fromValue(m_majors[row]);
    }
    case EconomicData_Item:
    {
        int row = index.row();
        if(m_economicDatas.isEmpty() || !index.isValid()) return QVariant();
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role)
            return QVariant::fromValue(m_economicDatas[row]);
    }
    case FinancialReport_Item:
    {
        if(!index.isValid()) return QVariant();
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            int row =0,count=0;
            calcFinancialReportRowAndCount(row,count);
            if(row !=0 && count !=0)
            {
                QVector<FinancialReport::FinancialItem> datas = m_financialReps.mid(index.row()*count,count);
                if(datas.size() > index.column())
                    return QVariant::fromValue(datas);
            }
            return QVariant();
        }
        if(HZINFOITEM_CUSTOM_ROLE_ITEM_TIME == role){
            return m_financeReportTime;
        }
    }break;
    }

    return QVariant();
}

void EconomicModel::calcFinancialReportRowAndCount(int &row, int &count) const
{
    row = 0;
    count = 0;
    if(!m_financialReps.isEmpty())
    {
        count = m_viewRc.width();
        count = count/440 == 0?1:count/440;
        if(m_financialReps.size()%count == 0)
        {
            row = m_financialReps.size()/count;
        }else
        {
            row = m_financialReps.size()/count+1;
        }
    }
}

void EconomicModel::startRequestSnapShot()
{
    QList<QString> stocks;
    if(m_type == MajorMeet_Item){
#if 0 //板块部分
        foreach(const STEconmicMajorMeetUi &dt, mMajors){
            foreach(const STEconmicMajorMeetUi::StockInfoUi &ui, dt.rltBoard){
                if(!ui.stockcode.isEmpty()) stocks.append(ui.stockcode);
            }
        }
#endif
        foreach(const STEconmicMajorMeetUi &dt, m_majors){
            //省了计算，最大取10只代码（最大也无法显示10只）
            int stockMax = qMin(dt.rltStock.count(), 10);
            for(int i=0; i<stockMax; ++i) {
                if(!dt.rltStock[i].stockcode.isEmpty()) stocks.append(dt.rltStock[i].stockcode);
            }
        }
        reqSnapShot(stocks);
    } else if(m_type == FinancialReport_Item){
        //todo
    }
}

void EconomicModel::reqSnapShot(const QList<QString> &stocks)
{
    std::list<std::string> stdlst;
    for(int i=0; i<stocks.count(); ++i)
        stdlst.push_back(stocks[i].toStdString());

    if(stdlst.size() == 0) return;
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stdlst;
    req.mask = HZData::SLStock_Mask;
    auto fun = std::bind(&EconomicModel::reqSnapShotResponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    DataController::instance()->requestOptionalDyna(req, fun);
}

void EconomicModel::reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err)
{
    if(m_type == MajorMeet_Item){
        if(err != 0) {
            qDebug() << Q_FUNC_INFO << "error:" << err;
            return;
        }

        QHash<QString, qreal> chgs;
        HZData::SLStock *slStocls = (HZData::SLStock *)data;
        if(slStocls) {
            for(int i=0; i<num; i++){
                QString code = QString::fromStdString(slStocls[i].stock_code);
                chgs[code] = slStocls[i].rate;
            }
        }

#if 0 //板块部分
        for(int i=0; i<mMajors.count();++i){
            for(int j=0; j<mMajors[i].rltBoard.count(); ++j){
                if(chgs.contains(mMajors[i].rltBoard[j].stockcode)) {
                    mMajors[i].rltBoard[j].chg = chgs[mMajors[i].rltStock[j].stockcode];
                }
            }
        }
#endif
        for(int i=0; i<m_majors.count();++i){
            int stockMax = qMin(m_majors[i].rltStock.count(), 10);
            for(int j=0; j<stockMax; ++j){
                if(chgs.contains(m_majors[i].rltStock[j].stockcode)) {
                    m_majors[i].rltStock[j].chg = chgs[m_majors[i].rltStock[j].stockcode];
                }
            }
        }

        emit dataChanged(index(0,0), index(m_majors.count()-1,0));
    } else if(m_type == FinancialReport_Item){
        //todo
    } 
}

void EconomicModel::flush()
{
    beginResetModel();
    endResetModel();
}

void EconomicModel::setFinanceReportTime(quint32 time)
{
    m_financeReportTime = time;
}

///
/// \brief EconomicView::EconomicView
/// \param type
/// \param parent
///
EconomicView::EconomicView(int type, QWidget *parent)
    :QListView(parent)
    ,m_type(type)
    ,m_smartModel(new EconomicModel(type))
{
    if(type == FinancialReport_Item)
    {
        m_smartDelegate =  new FinancialReportDelegate(this);
    }else
    {
        m_smartDelegate =  new EconomicDelegate(type);
    }
    setObjectName("EconomicView");
    setMouseTracking(false);
    setSpacing(0);
    setDragEnabled(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeMode(QListView::Adjust);
    setViewMode(QListView::IconMode);
    setDragEnabled(false);
    setWordWrap(true);

    setItemDelegate(m_smartDelegate);
    setModel(m_smartModel);
    initData();
}

EconomicView::~EconomicView()
{

}

void EconomicView::setBtnClicked(int id)
{
    qDebug() << Q_FUNC_INFO << "id:" << id;
    if(m_type == EconomicData_Item){
        QVector<STEconomicDataUi> tmpDts;
        if(id == All_Country) {
            m_smartModel->addRecords(m_ecoUiData);
        } else if(id == China_Country){
            for(const STEconomicDataUi &ui: m_ecoUiData)if(ui.region == QStringLiteral("中国")) tmpDts.append(ui);
            m_smartModel->addRecords(tmpDts);
        } else if(id == Americ_Country) {
            for(const STEconomicDataUi &ui: m_ecoUiData)if(ui.region == QStringLiteral("美国")) tmpDts.append(ui);
            m_smartModel->addRecords(tmpDts);
        } else if(id == Japan_Country) {
            for(const STEconomicDataUi &ui: m_ecoUiData)if(ui.region == QStringLiteral("日本")) tmpDts.append(ui);
            m_smartModel->addRecords(tmpDts);
        } else if(id == EuropeanUnion_Country){
            for(const STEconomicDataUi &ui: m_ecoUiData)if(ui.region == QStringLiteral("欧盟")) tmpDts.append(ui);
            m_smartModel->addRecords(tmpDts);
        }
    }
}

void EconomicView::setCacheData(const QVector<STEconomicDataUi> &datas)
{
    if(m_type == EconomicData_Item)
        m_ecoUiData = datas;
}

void EconomicView::slotScrollChanged(int value)
{
    this->verticalScrollBar()->setValue(value);
}

void EconomicView::resizeEvent(QResizeEvent *e)
{
    m_smartDelegate->setSize(e->size());
    m_smartModel->setViewRect(this->geometry());
    QListView::resizeEvent(e);
}

void EconomicView::paintEvent(QPaintEvent *e)
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
            QListView::paintEvent(e);
        }
    }
}

void EconomicView::initData()
{
}

///
/// \brief EconomicItem::EconomicItem
/// \param title
/// \param parent
///
EconomicItem::EconomicItem(int type, const QString &title, QWidget *parent)
    :QFrame(parent)
    ,m_type(type)
{
    setMinimumHeight(183);
    m_title = new Infoitemtitle(type, title);
    if(m_type ==  FinancialReport_Item)
    {
        m_economicView = new FinancialReportView();
    }else
    {
        m_economicView = new EconomicView(m_type);
        connect(m_title, &Infoitemtitle::signalBtnClicked, [=](int id){
            qDebug() << Q_FUNC_INFO << "id:" << id;
            m_economicView->setBtnClicked(id);
        });
    }

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);

    QHBoxLayout *hb = new QHBoxLayout;
    hb->setContentsMargins(/*30*/0, 0, 0, 0);
    hb->setSpacing(0);
    hb->addWidget(m_economicView);

    vLayout->addWidget(m_title);
    vLayout->addLayout(hb);
    setLayout(vLayout);
}

#include "zxstockopt.h"
void EconomicItem::setData(const QVariant &var, quint32 time)
{
    switch(m_type) {
    case MajorMeet_Item:{
        if(var.canConvert<MajorMeeting>()){
            MajorMeeting meet = var.value<MajorMeeting>();
            QVector<STEconmicMajorMeetUi> uiDatas;
            m_title->setRecordNum(meet.datas.count());
            foreach(const MajorMeeting::MajorMeetPrivate &dt, meet.datas){
                STEconmicMajorMeetUi item;
                item.type = 0;
                item.title = dt.title;
                item.url = dt.url;
                uiDatas.push_back(item);
                if(!dt.relate_board.isEmpty()){
                    QVector<STEconmicMajorMeetUi::StockInfoUi> stks;
                    foreach(const QString &stock, dt.relate_board){
                        STEconmicMajorMeetUi::StockInfoUi info={stock, "", 0};
                        stks.append(info);
                    }
                    item.type = 1;
                    item.rltBoard = stks;
                    uiDatas.push_back(item);
                }
                if(!dt.relate_stock.isEmpty()) {
                    QVector<STEconmicMajorMeetUi::StockInfoUi> stks;
                    for(int i=0; i<dt.relate_stock.count(); ++i) {
                        bool isCustom = ZXStockOpt::instance()->isHas(dt.relate_stock[i].stockcode);
                        STEconmicMajorMeetUi::StockInfoUi info={dt.relate_stock[i].stockname, dt.relate_stock[i].stockcode, 0, isCustom};
                        stks.append(info);
                    }
                    item.type = 2;
                    item.rltStock = stks;
                    uiDatas.push_back(item);
                }
                item.type = 3;
                uiDatas.push_back(item);
            }
            m_economicView->m_smartModel->addRecords(uiDatas);
            m_title->setRecordNum(meet.datas.count());
        }
    }break;
    case FinancialReport_Item:{
        if(var.canConvert<FinancialReport>()){
            FinancialReport report = var.value<FinancialReport>();
            m_title->setRecordNum(report.datas.count());
            m_economicView->m_smartModel->setFinanceReportTime(time);
            m_economicView->m_smartModel->addRecords(report.datas);
            m_title->setRecordNum(report.datas.count());
        }
    }break;
    case EconomicData_Item:{
        if(var.canConvert<EconomicData>()) {
            EconomicData econo = var.value<EconomicData>();
            QVector<STEconomicDataUi> uiDatas;
            m_title->setRecordNum(econo.datas.count());
            foreach(const EconomicData::EconomicDataItem &dt, econo.datas){
                STEconomicDataUi item;
                item.type = 0;
                item.time = dt.time;
                item.name = dt.name;
                //qDebug() << Q_FUNC_INFO << "time:" << item.time << QDateTime::fromTime_t(dt.time).toString("yyyyMMdd hh:mm");
                item.pre_value = dt.pre_value;
                item.exp_value = dt.exp_value;
                item.act_value = dt.act_value;
                item.region = dt.region;
                item.source = dt.source;
                uiDatas.push_back(item);

                item.type = 1;
                uiDatas.push_back(item);

                item.type = 2;
                uiDatas.push_back(item);
            }
            m_economicView->m_smartModel->addRecords(uiDatas);
            m_economicView->setCacheData(uiDatas);
            m_economicView->setBtnClicked(m_title->currentItem());//重新刷新一下数据
            m_title->setRecordNum(-1);
        }
    }break;
    }
}

///
/// \brief EconomicMaster::EconomicMaster
/// \param parent
///
EconomicMaster::EconomicMaster(IUpdateRequest *adapter, QWidget *parent)
    : BasePage(parent)
    ,ui(new Ui::EconomicMaster)
    ,m_iURequest(adapter)
{
    ui->setupUi(this);

    //日历
    m_calandar = new CalandarBarMaster;
    m_calandar->setObjectName("CalandarBarMaster");
    m_calandar->setFixedHeight(28);

    //子页
    m_majorMeetPage      = new EconomicItem(MajorMeet_Item,QStringLiteral("重大会议"));
    m_financialRepPage   = new EconomicItem(FinancialReport_Item,QStringLiteral("财报发布"));
    m_economicDataPage   = new EconomicItem(EconomicData_Item,QStringLiteral("经济数据"));

    ui->horizontalLayout->addWidget(m_calandar);
    ui->verticalLayout->addWidget(m_majorMeetPage);
    ui->verticalLayout->addWidget(m_financialRepPage);
    ui->verticalLayout->addWidget(m_economicDataPage);

    m_iURequest->setVisitor(this);
    connect(m_calandar, &CalandarBarMaster::signalCalandarBarClicked, [&]{
        QString dateStr = m_calandar->getCurrentDate();
        startRequest(dateStr);
    });

    startTimesReq();
}

EconomicMaster::~EconomicMaster()
{
    delete ui;
    if(m_timesReq != -1)HzInfoCenter::getInstance()->cancle(m_timesReq);
}

void EconomicMaster::startTimesReq()
{
    EconomicDataTimesCallBack fun = std::bind(&EconomicMaster::economicCalandarInfoTimesCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_timesReq = HzInfoCenter::getInstance()->reqEconomicDataTimes(fun);
}

void EconomicMaster::setAdapter(IUpdateRequest *adapter)
{
    //todo
}

void EconomicMaster::updateList(const QVariantMap &var)
{
    QString timeStr = m_calandar->getCurrentDate();//yyyyMMdd;
    quint32 timeInt = QDateTime::fromString(timeStr,"yyyyMMdd").toTime_t();

    QVariant data = var["data"];
    m_majorMeetPage->setData(data,timeInt);
    m_financialRepPage->setData(data,timeInt);
    m_economicDataPage->setData(data,timeInt);
}

void EconomicMaster::updateTimes(const QVector<quint64> &times)
{
//    mIURequest->cancleReq();
    Log("ZIXUAN","WARN","EconomicMaster::updateTimes times count %d", times.count());
    m_calandar->setValidTimes(times.isEmpty()?m_iURequest->getCalandarTimes():times);
}

void EconomicMaster::updateDetails(const QVariant &var)
{
    //todo
}

void EconomicMaster::afterDeactive()
{
    if(m_iURequest)
        m_iURequest->cancleReq();
}

void EconomicMaster::afterActive()
{
    if(m_iURequest)
        m_iURequest->initializatItems(false,QVariant::fromValue(m_calandar->getCurrentDate()));
}


void EconomicMaster::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void EconomicMaster::startRequest(const QString &date)
{
    if(date.isEmpty() || nullptr== m_iURequest)
        return;

    QVariantMap map;
    map["date"] = m_calandar->getCurrentDate();
    m_iURequest->requestListForParam(map);
}

void EconomicMaster::economicCalandarInfoTimesCallBack(const InfoRet &ret, const EconomicDataTimes &datas)
{
    if(ret.code != 0) return;
    updateTimes(datas.datas);
}

///
/// \brief FinancialReportView::FinancialReportView
/// \param parent
///
FinancialReportView::FinancialReportView(QWidget *parent)
    :EconomicView(FinancialReport_Item,parent)
{
    setMouseTracking(true);
}

FinancialReportView::~FinancialReportView()
{

}

///
/// \brief FinancialReportView::mousePressEvent
/// \param event
///
void FinancialReportView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index =this->indexAt(event->pos());
    QRect rc = this->visualRect(index);
    QAbstractItemDelegate* delegate = itemDelegate(index);
    FinancialReportDelegate* pFinancialDelegate = (FinancialReportDelegate*)delegate;

    quint32 time = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM_TIME).toUInt();
    QVector<FinancialReport::FinancialItem> items = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<QVector<FinancialReport::FinancialItem>>();
    QVector<FinaReportDelItem> data = pFinancialDelegate->calcDrawData(items,rc,time);
    for(int i =0;i < data.size();i++)
    {
        if(data[i].optionRc.contains(event->pos()) || data[i].reportRc.contains(event->pos()))
        {
            qDebug() << "click report: " << "stockCode: " << data[i].stockCode;
            QTimer::singleShot(0, [=]{gInfoStyle->signalStockClicked(data[i].stockCode);});
            return;
        }else if(data[i].showRc.contains(event->pos()))
        {
            qDebug() << "click show detail: " << "stockCode: " << data[i].stockCode;
            //弹出详情页
            gInfoStyle->signalFinaoReportDetail(QStringLiteral("财报发布"), data[i].url);
            return;
        }
    }
}
