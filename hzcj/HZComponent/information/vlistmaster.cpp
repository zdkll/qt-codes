#include "vlistmaster.h"
#include "vlistitemdelegate.h"
#include "vlistdef.h"
#include "infodef.h"
#include <QDebug>
#include <QtWidgets>
#include <QFile>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWebEngineProfile>

#include "infostyle.h"

HZVListMaster::HZVListMaster(QWidget *parent) :
    QWidget(parent)
{
    init();
}

HZVListMaster::~HZVListMaster()
{

}

void HZVListMaster::setAdapter(HZInfoAdapterInterface *adapter)
{
    if(adapter != nullptr) {
        m_adapter = adapter;

        //首次點擊項時彈出，code和page為正常值
        QVariantMap tmpMap;
        tmpMap["data"] = QVariant::fromValue(m_adapter->getItems());
        tmpMap["code"] = 0;
        tmpMap["page"] = 0;
        tmpMap["id"] = m_adapter->itemId();
        m_vListView->setData(tmpMap);
    }
}

void HZVListMaster::updateList(const QVariantMap &data)
{
    QVariantMap tmpMap(data);
    tmpMap["id"] = m_adapter->itemId();
    m_vListView->setData(tmpMap);
    m_adapter->setItemId("");
}

void HZVListMaster::clearAll()
{
    m_adapter = nullptr;

//    QVariantMap dataMap;
//    dataMap["id"] = "";
//    dataMap["code"] = 0;
//    dataMap["page"] = 0;
//    dataMap["data"] = QVariant::fromValue(STInfoData());
//    updateList(dataMap);

    m_vListView->clearAll();
    if(m_adapter != nullptr)
    {
        delete m_adapter;
        m_adapter = nullptr; 
    }

}

void HZVListMaster::clearWeb()
{
    m_webView->loadUrl(QUrl("about:blank"));
}

void HZVListMaster::init()
{
    setObjectName("HZVListMaster");
    m_vListView = new HZVListView;
    m_vListView->setObjectName("hzvlistview");

    m_webView = new HZInfoWebPageMarster;
    HZInfoWebPage *mWebPage = new HZInfoWebPage(PopWindow, QWebEngineProfile::defaultProfile());
    m_webView->setWebPage(mWebPage);

    QSpacerItem *space = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_vListView);
    hLayout->addWidget(m_webView);

    hLayout->addSpacerItem(space);
    hLayout->setStretch(0, 0);
    hLayout->setStretch(1, 2);
    setLayout(hLayout);
    layout()->setContentsMargins(0,0,0,0);
    connect(m_vListView, &HZVListView::signalRequest, [=](int max){
        Q_UNUSED(max)
        m_adapter->requestList();
    });
    connect(m_vListView, &HZVListView::signalClicked, [=](const QModelIndex &index){
        QUrl url(index.data(CUSTOM_ROLE_ITEM_ROLE).toString());
        if(url.isValid()) {
            m_webView->setUrl(url);
        }
    });
}

void HZVListMaster::resizeEvent(QResizeEvent *event)
{
    HZVListItemDelegate *delegate =
            qobject_cast<HZVListItemDelegate*>(m_vListView->itemDelegate());
    delegate->setWidth(VLIST_WIDTH_SIZE);
    __super::resizeEvent(event);
}


///
/// \brief HZVListModel::HZVListModel
/// \param prent
///
HZVListModel::HZVListModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void HZVListModel::addRecords(const QList<STVListData> &items)
{
    m_records += items;
    flush();
}

void HZVListModel::addRecord(const STVListData &item)
{
    if(!m_records.contains(item)) {
        m_records.append(item);
        emit dataChanged(index(m_records.count()-1,0), index(rowCount(),0));
    }
}

void HZVListModel::insertRecord(const STVListData &item, int idx)
{
    if(idx && idx < m_records.count()) {
        m_records.insert(idx, item);
    }else{
        m_records.append(item);
    }

    emit dataChanged(index(0, 0), index(rowCount(),0));
}

QVariant HZVListModel::recordByIndex(int index)
{
    if(index < 0 || index > m_records.count()) return QVariant();
    return QVariant::fromValue(m_records[index]);
}

void HZVListModel::setMsgId(const QString &id)
{
    m_msgId = id;
}

int HZVListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_records.count();
}

QVariant HZVListModel::data(const QModelIndex &index, int role) const
{
    if(CUSTOM_ROLE_ITEM == role){
        return QVariant::fromValue(m_records[index.row()]);
    } else if(CUSTOM_ROLE_ITEM_ROLE == role) {
        return QVariant::fromValue(m_records[index.row()].url);
    } else if(CUSTOM_ROLE_ITEM_IID == role) {
        return m_msgId;
    }

    return QVariant();
}

void HZVListModel::flush()
{
    beginResetModel();
    endResetModel();
}

QList<STVListData> HZVListModel::getRecores() const
{
    return m_records;
}

void HZVListModel::clearAll()
{
    m_records.clear();
    flush();
}

///
/// \brief HZVListView::HZVListView
/// \param parent
///
HZVListView::HZVListView(QWidget *parent)
    :QListView(parent)
    ,m_listModel(new HZVListModel(this))
    ,m_listDelegate(new HZVListItemDelegate(this))
{
    m_dataFlags = Requst_Normal;
    setFixedWidth(VLIST_WIDTH_SIZE);
    setMouseTracking(true);
    setSpacing(0);
    setViewMode(QListView::IconMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeMode(QListView::Adjust);
    setItemDelegate(m_listDelegate);
    setModel(m_listModel);
    connect(this, &HZVListView::clicked, [=](const QModelIndex &index){
        if(m_dataFlags == Requst_Normal) {
            STVListData data = index.data(CUSTOM_ROLE_ITEM).value<STVListData>();
            m_listModel->setMsgId(data.id);
            emit signalClicked(index);
        }
    });
    connect(verticalScrollBar(),&QScrollBar::valueChanged, [=](int value){
        int max = verticalScrollBar()->maximum();
        if(value == max)emit signalRequest(value);
    });
}

void HZVListView::setSelected(const QString &itemId)
{
    if(itemId.isEmpty())
        return;
#if 1
    int id = 0;
    QList<STVListData> lst = m_listModel->getRecores();
    for(int i=0; i < lst.count(); ++i){
        if(lst[i].id == itemId) {
            id = i;
            break;
        }
    }

    const QModelIndex &idx = m_listModel->index(id,0);
    if(idx.isValid()){
        //setCurrentIndex(idx);
        m_listModel->setMsgId(itemId);
        emit signalClicked(idx);
    }
#else
    m_listModel->setMsgId(itemId);
    //emit signalClicked(idx);
#endif
}

void HZVListView::setData(const QVariantMap &data)
{
    QString id  = data["id"].toString();
    int code    = data["code"].toInt();
    int page    = data["page"].toInt();
    STInfoData dt = data["data"].value<STInfoData>();

    if(0 != code){
        m_dataFlags = Request_Exception;
        m_listModel->addRecords(QList<STVListData>{});
        m_tipText = QStringLiteral("操作失败，未知错误");
        m_tipUrl = ":/skin/default/img/infopage/NEWS02.png";
        update();
    } else {
        if(!dt.datas.isEmpty()) {
            m_dataFlags = Requst_Normal;
            m_listModel->addRecords(dt.datas);
            setSelected(id);
        }
        else if(m_listModel->getRecores().isEmpty())
        {
            m_dataFlags = Request_Null;
            m_listModel->addRecords(QList<STVListData>{});
            m_tipText = QStringLiteral("当前暂无数据");
            m_tipUrl = ":/skin/default/img/infopage/NEWS01.png";
            update();
        }
    }
}

void HZVListView::clearAll()
{
    m_listModel->clearAll();
}

void HZVListView::paintEvent(QPaintEvent *e)
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
//        QPainter p(viewport());
//        p.fillRect(rect(),gInfoStyle->mMarketCldDetailBrush);
        QListView::paintEvent(e);
    }
    }
}
