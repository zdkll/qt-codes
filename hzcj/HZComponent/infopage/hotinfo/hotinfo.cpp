#include "hotinfo.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QEvent>
#include <QKeyEvent>
#include <QFont>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextOption>
#include <QDateTime>
#include <QVBoxLayout>
#include <QWebEngineProfile>

///
/// \brief HotInfoModel::HotInfoModel
/// \param parent
///
HotInfoModel::HotInfoModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void HotInfoModel::addRecords(const QList<STHotInfoData> &items)
{
    m_records += items;
    flush();
}

void HotInfoModel::addRecord(const STHotInfoData &item)
{
    if(!m_records.contains(item)) {
        m_records.append(item);
        emit dataChanged(index(m_records.count()-1,0), index(rowCount(),0));
    }
}

void HotInfoModel::insertRecord(const STHotInfoData &item, int idx)
{
    if(idx && idx < m_records.count()) {
        m_records.insert(idx, item);
    }else{
        m_records.append(item);
    }

    emit dataChanged(index(0, 0), index(rowCount(),0));
}

QVariant HotInfoModel::recordByIndex(int index)
{
    if(index < 0 || index > m_records.count()) return QVariant();
    return QVariant::fromValue(m_records[index]);
}

int HotInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_records.count();
}

QVariant HotInfoModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && !m_records.isEmpty() && (index.row() >=0 && index.row() < m_records.count())) {
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            return QVariant::fromValue(m_records[index.row()]);
        }else if(HZINFOITEM_CUSTOM_ROLE_TILE_URL == role){
            QString urlStr = m_records[index.row()].url;
            //qDebug() << Q_FUNC_INFO << "url:" << urlStr;
            return QVariant::fromValue(QUrl(urlStr));
        }
    }

    return QVariant();
}

void HotInfoModel::flush()
{
    beginResetModel();
    endResetModel();
}

void HotInfoModel::resetModel()
{
    m_records.clear();
    flush();
}

QList<STHotInfoData> HotInfoModel::getRecores() const
{
    return m_records;
}

///
/// \brief HotItemDelegate::HotItemDelegate
/// \param parent
///
HotItemDelegate::HotItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
  ,m_customPix(":/skin/default/img/infopage/zixuan.png")
{

}

void HotItemDelegate::setWidth(int w)
{
    m_width = w;
}

void HotItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    if(index.isValid()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, false);
        STHotInfoData data = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STHotInfoData>();
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
        QRect rect = viewOption.rect;
        rect.setWidth(m_width);
        //painter->save();
//        if( viewOption.state.testFlag(QStyle::State_Selected) && viewOption.state.testFlag(QStyle::State_MouseOver)) {
//            painter->fillRect(option.rect, gInfoStyle->mItemMousePressBrush);
//        } else if(viewOption.state.testFlag(QStyle::State_MouseOver)){
//            painter->fillRect(option.rect, gInfoStyle->mItemMouseHoverBrush);
//        } else {
//            painter->fillRect(option.rect, QBrush(Qt::NoBrush));
//        }

        //painter->restore();
        painter->setPen(gInfoStyle->m_hLinePen);
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());

        QRect rt(rect);
        QTextOption opt(Qt::AlignLeft|Qt::AlignTop);
        opt.setWrapMode(QTextOption::NoWrap);
        opt.setUseDesignMetrics(true);

        //draw title
        painter->setFont(gInfoStyle->m_titleFont);
        painter->setPen(gInfoStyle->m_titlePen);
        QRect tmpTitleRt(rt.adjusted(HZINFOITEM_LEFT_SPACE, HZINFOITEM_TOP_SPACE, 0, 0));
        QString tmpTitle = painter->fontMetrics().elidedText(data.title, Qt::ElideRight, tmpTitleRt.width()-5);
        painter->drawText(tmpTitleRt, Qt::AlignLeft|Qt::AlignTop, tmpTitle);

        //draw content
        opt.setWrapMode(QTextOption::WordWrap);
        painter->setFont(gInfoStyle->m_descripFont);
        painter->setPen(gInfoStyle->m_descriptPen);
        QRect des(rt.x()+HZINFOITEM_LEFT_SPACE, rt.y()+36, rt.width()-HZINFOITEM_LEFT_SPACE, 28);
        QString elidTxt = painter->fontMetrics().elidedText(data.content,Qt::ElideRight,des.width()*2);
        painter->drawText(des, /*data.content*/elidTxt, opt);

        //draw tag ?
        QRect lastRt(rt.adjusted(HZINFOITEM_LEFT_SPACE, 55+HZINFOITEM_TOP_SPACE, 0, -28));
        bool isTagValid = (data.tag >0 && data.tag < HZINFOITEM_INDEX_CNT);
        painter->setFont(gInfoStyle->m_customSelFont);
        int tagNameW = painter->fontMetrics().width(isTagValid?gTagTexts[data.tag]:gTagTexts[0]);
        int tagNameH = painter->fontMetrics().height();
        QRect tagRt(lastRt.x(), lastRt.y()+13, tagNameW, tagNameH);
        if(isTagValid) {
            painter->setPen(gInfoStyle->m_idxBtnTxtPens[data.tag]);
            painter->drawRect(tagRt.adjusted(-1,-1,1,1));
            if(data.tag != 0)
                painter->drawText(tagRt, Qt::AlignCenter, gTagTexts[data.tag]);
        }

        painter->setFont(gInfoStyle->m_descripFont);
        tagNameW = painter->fontMetrics().width(isTagValid?gTagTexts[data.tag]:gTagTexts[0]);
        tagNameH = painter->fontMetrics().height();

        // draw stock
        QString codeName = data.relateName + " " + data.relateCode.mid(2);
        int codeNameW = painter->fontMetrics().width(codeName);
        int codeNameH = painter->fontMetrics().height();
        int xpos = tagRt.x()+tagRt.width()+15;

        if(!codeName.isEmpty()) {
            painter->setFont(gInfoStyle->m_descripFont);
            painter->setPen(gInfoStyle->m_descriptPen);
            QRect stockRt(xpos,tagRt.y(),codeNameW,codeNameH);
            painter->drawText(stockRt, Qt::AlignLeft | Qt::AlignTop, codeName);
            if(m_pressFlg && stockRt.contains(m_pressed)) {
                qDebug() << Q_FUNC_INFO << "stock code:" << codeName;
                m_pressFlg = false;
                QTimer::singleShot(0,[=]{gInfoStyle->signalStockClicked(data.relateCode);});
            }
        }

        //draw 自选 ?
        if(data.isCus) {
            xpos += (codeNameW +6);
            painter->drawPixmap(QRect(xpos,tagRt.y(),22,14),m_customPix);
        }

        //draw time ?
        QString formatTime("yyyy-MM-dd hh:mm");
        int timeW = painter->fontMetrics().width(formatTime);
        int th = painter->fontMetrics().height();
        if(data.time != HZINFOITEM_INVALID_VALUE ) {
            painter->setPen(gInfoStyle->m_descriptPen);
            painter->setFont(gInfoStyle->m_descripFont);
            QRect timeRt(lastRt.x(), lastRt.y()+13, lastRt.width()-16, th);
            painter->drawText(timeRt, Qt::AlignRight|Qt::AlignVCenter, QDateTime::fromTime_t(data.time).toString(formatTime));
        }

        //draw source ?
        if(!data.source.isEmpty()) {
            painter->setPen(gInfoStyle->m_descriptPen);
            painter->setFont(gInfoStyle->m_descripFont);
            QRect src(lastRt.x(), lastRt.y()+13, lastRt.width()-(timeW+32), th);
            painter->drawText(src, Qt::AlignRight|Qt::AlignVCenter, data.source);
        }
        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter,option,index);
    }
}

QSize HotItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
        return QSize(m_width, HZINFOITEM_HEIGHT_SIZE);
    else
        return QStyledItemDelegate::sizeHint(option, index);
}

bool HotItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if(me) {
            m_pressFlg = true;
            m_pressed = me->pos();
        }
    }

    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

///
/// \brief HotView::HotView
/// \param parent
///
HotView::HotView(QWidget *parent)
    :QListView(parent)
    ,m_listModel(new HotInfoModel(this))
    ,m_listDelegate(new HotItemDelegate(this))
    ,m_selectModel(new QItemSelectionModel)
{
    setSpacing(0);
    setViewMode(QListView::IconMode);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeMode(QListView::Adjust);
    setItemDelegate(m_listDelegate);
    setModel(m_listModel);
    m_selectModel->setModel(m_listModel);
    connect(verticalScrollBar(),&QScrollBar::valueChanged, [=](int value){
        int max = verticalScrollBar()->maximum();
        if(value == max)emit signalReStartRequest(value);
    });
    connect(this, &HotView::clicked, this, [=](const QModelIndex &index){
        if(m_listModel) {
            QUrl url = m_listModel->data(index,HZINFOITEM_CUSTOM_ROLE_TILE_URL).toUrl();
            qDebug() << Q_FUNC_INFO << "info:" << url;
            if(url.isValid())emit signalItemClicked(url);
        }
    });
}

void HotView::setSelected(const QString &index)
{
    //TODO
}

void HotView::setData(const QVariantMap &data)
{
    QString id          = data["id"].toString();
    QList<STHotInfoData> lst   = data["data"].value<QList<STHotInfoData>>();
    int code            = data["code"].toInt();
    int page            = data["page"].toInt();

    //qDebug() << Q_FUNC_INFO << "code:" << code << "page:" << page << "lst:" << lst.isEmpty();

    if(REQUEST_OPERATOR_EXCEPTION != code){
        m_dataFlags = Request_Exception;
        m_listModel->addRecords(QList<STHotInfoData>{});
        m_tipText = QStringLiteral("操作失败，未知错误");
        m_tipUrl = ":/skin/default/img/infopage/NEWS02.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    } else {
        if(!lst.isEmpty()) {
            m_dataFlags = Requst_Normal;
            m_listModel->addRecords(lst);
            QList<STHotInfoData> tmpLst(m_listModel->getRecores());
            if(!tmpLst.isEmpty()){
                QUrl url(tmpLst.first().url);
                emit signalDateChanged(url);
            }
        } else if(m_listModel->getRecores().isEmpty()){
            m_dataFlags = Request_Null;
            m_listModel->addRecords(QList<STHotInfoData>{});
            m_tipText = QStringLiteral("当前暂无数据");
            m_tipUrl = ":/skin/default/img/infopage/NEWS01.png";
            gInfoStyle->signalWebAboutBlank();
            update();
        }
    }
}

void HotView::resetModel()
{
    m_listModel->resetModel();
}

void HotView::resizeEvent(QResizeEvent *e)
{
    m_listDelegate->setWidth(e->size().width());
    QListView::resizeEvent(e);
}

void HotView::paintEvent(QPaintEvent *e)
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
/// \brief HotMarster::HotMarster
/// \param adapter
/// \param parent
///
HotMarster::HotMarster(IUpdateRequest *adapter, QWidget *parent) :
    BasePage(parent)
  ,m_iURequest(adapter)
  ,m_indexBtn(new IndexReqButton)
  ,m_leftView(new HotView)
  ,m_webView(new HZInfoWebPageMarster)
  ,m_webPage(new HZInfoWebPage(InsertWindow, QWebEngineProfile::defaultProfile()))
{
    setMouseTracking(true);
    m_indexBtn->setFixedHeight(102);
    m_indexBtn->setObjectName("IndexReqButton");
    m_leftView->setFixedWidth(500);

    m_webView->setWebPage(m_webPage);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    vLayout->addWidget(m_indexBtn);
    vLayout->addWidget(m_leftView);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(m_webView);
    hLayout->setStretch(0,0);
    hLayout->setStretch(1,1);
    setLayout(hLayout);

    //installEventFilter(this);
    m_iURequest->setVisitor(this);
    connect(m_indexBtn, &IndexReqButton::signalIndexChanged, [=](int tag){
        //重新请求
        if(tag != m_defaultTag) {
            m_leftView->resetModel();
            m_defaultTag = tag;
            m_tagChanged = true;
            m_iURequest->initializatItems(true, tag);
            return;
        }

        //继续请求
        QVariantMap map;
        map["tag"] = tag;
        map["source"] = 3;
        m_iURequest->requestListForParam(map);
    });

    connect(m_leftView, &HotView::signalReStartRequest, [=](int value){
        Q_UNUSED(value);
        QVariantMap map;
        map["tag"] = m_defaultTag;
        map["source"] = 3;
        m_iURequest->requestListForParam(map);
    });

    connect(m_leftView, &HotView::signalItemClicked, [&](const QUrl &url){
        if(url.isValid())m_webView->setUrl(url);
    });

    //每次在tag发生变化，且数据model重新更新时，将首项列表的详情页打开
    connect(m_leftView, &HotView::signalDateChanged, [&](const QUrl &url){
        if(m_tagChanged && url.isValid()){
            m_tagChanged = !m_tagChanged;
            if(m_webView) m_webView->setUrl(url);
        }
    });
    connect(gInfoStyle, &InfoStyle::signalWebAboutBlank, [=]{//:/skin/default/img/html/black.html
        QString h5Url = qApp->applicationDirPath();
        h5Url+="/cfg/html/black.html";
        if(m_webView) m_webView->setUrl(QUrl(/*"about:blank"*/h5Url));
    });
}

HotMarster::~HotMarster()
{
    delete m_iURequest;
}

void HotMarster::setAdapter(IUpdateRequest *adapter)
{
    //TODO
}

bool HotMarster::eventFilter(QObject *w, QEvent *e)
{
    return false;
}

void HotMarster::updateList(const QVariantMap &var)
{
    if(var.isEmpty()) return;
    QVariantMap tmpMap(var);
    tmpMap["id"] = m_iURequest->itemId();
    m_leftView->setData(tmpMap);
}

void HotMarster::afterActive()
{
    int tag = m_indexBtn->selected();
    m_defaultTag = tag;
    m_tagChanged = true;
    m_leftView->resetModel();
    m_iURequest->initializatItems(true,tag);
}

void HotMarster::afterDeactive()
{
    m_iURequest->cancleReq();
}
