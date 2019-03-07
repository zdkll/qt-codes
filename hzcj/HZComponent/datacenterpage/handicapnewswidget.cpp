#include "handicapnewswidget.h"
#include <QPainter>
#include <QApplication>
#include <QDateTime>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QHeaderView>
#include "infostyle.h"



HandicapNewsDelegate::HandicapNewsDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void HandicapNewsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    STVListData  data = index.data(MYDATAROLE).value<STVListData>();

    //去掉Focus
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    QFontMetrics metrics = painter->fontMetrics();

    QFont font;
//    font.setStyleStrategy(QFont::NoAntialias);
    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.titlePixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.titileColor());
    QRect rc = option.rect;
    rc.adjust(15, 11, 0, 0 );
    //画标题
    QString strTitle = painter->fontMetrics().elidedText(data.body, Qt::ElideRight, rc.width());
    painter->drawText(rc, strTitle);

//    QString strText = data.source + data.times;

//    font.setFamily(m_styledWidget.titleFontFamily());
//    font.setPixelSize(m_styledWidget.textPixelSize());
//    painter->setFont(font);
//    painter->setPen(m_styledWidget.textColor());
//    //画来源和时间
//    painter->drawText(rc,Qt::AlignRight,strText);

    int nWidth, nHeight;
    QPixmap pix;
    QColor tagColor;
    switch (data.type) {
    case NeutralTag:       //"中性"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.zxPixmap();
        tagColor = m_styledWidget.zxColor();
        break;
    case BullTag:    //"利好"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.lhPixmap();
        tagColor = m_styledWidget.lhColor();
        break;
    case BadNewsTag:    //"利空"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.lkPixmap();
        tagColor = m_styledWidget.lkColor();
        break;
    case StrongBullTag:     //"强烈利好"
        nWidth = 39;
        nHeight = 14;
        pix = m_styledWidget.qllhPixmap();
        tagColor = m_styledWidget.qllhColor();
        break;
    case StrongBadNewsTag:  //"强烈利空
        nWidth = 39;
        nHeight = 14;
        pix = m_styledWidget.qllkPixmap();
        tagColor = m_styledWidget.qllkColor();
        break;
    default:
        nWidth = 0;
        nHeight = 0;
        break;
    }

    QRect rect(option.rect);



    rect.adjust(15, 22 + metrics.height(), 0, 0);

    if(UnknownTag != data.type)
    {
        //画状态
        painter->drawPixmap(rect.left(), rect.top(), nWidth, nHeight,  pix);

        rect.adjust(3, 1, 0, 0);

        font.setFamily(m_styledWidget.labelFontFamily());
        font.setPixelSize(m_styledWidget.labelPixelSize());
        painter->setFont(font);
        painter->setPen(tagColor);
        painter->drawText(rect, data.tag);

        rect.adjust(nWidth - 3 + 6, -1, 0, 0);
    }

    //画摘要
    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.textPixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.textColor());

    QString strSummary = painter->fontMetrics().elidedText(data.desc, Qt::ElideRight, rect.width());

    painter->drawText(rect,strSummary);
}


HandicapNewsModel::HandicapNewsModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

int HandicapNewsModel::rowCount(const QModelIndex &parent) const
{
    return m_dataList.size();
}

int HandicapNewsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant HandicapNewsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case MYDATAROLE:
        return QVariant::fromValue(m_dataList[row]);
        break;
    default:
        break;
    }
    return QVariant();
}

void HandicapNewsModel::updateList(const QList<STVListData> &datas)
{
    m_dataList += datas;
    if(m_dataList.size() > 99)
    {
        m_dataList = m_dataList.mid(0,99);
    }
    refrush();
}

void HandicapNewsModel::clearAll()
{
    m_dataList.clear();
    refrush();
}

void HandicapNewsModel::refrush()
{
    beginResetModel();
    endResetModel();
}

HandicapNewsView::HandicapNewsView(QWidget *parent):
    QTableView(parent),
    m_newsmodel(new HandicapNewsModel(this))
{
//    this->setFixedHeight(186);
//    this->setMinimumWidth(900);
    initTableView();
}

void HandicapNewsView::clearAll()
{
    m_newsmodel->clearAll();
    this->verticalScrollBar()->setValue(0);
}

void HandicapNewsView::updateList(const QVariantMap &data)//QList<STVListData>
{
    QString id          = data["id"].toString();
    STInfoData lst      = data["data"].value<STInfoData>();
    int code            = data["code"].toInt();
    int page            = data["page"].toInt();

    //qDebug() << Q_FUNC_INFO << "code:" << code << "page:" << page;
    if(0 != code){ //except
        mDataFlags = Request_Exception;
        m_newsmodel->updateList(QList<STVListData>{});
        mTipText = QStringLiteral("操作失败，未知错误");
        mTipUrl = ":/skin/default/img/infopage/NEWS02.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    } else {
        if(code == 0 && (page != -2 || !lst.datas.isEmpty()) ) {//normal
            mDataFlags = Requst_Normal;
            m_newsmodel->updateList(lst.datas);
            //setSelected(id);
        } else {
            mDataFlags = Request_Null;
            m_newsmodel->updateList(QList<STVListData>{});
            mTipText = QStringLiteral("当前暂无数据");
            mTipUrl = ":/skin/default/img/infopage/NEWS01.png";
            gInfoStyle->signalWebAboutBlank();
            update();
        }
    }
}

void HandicapNewsView::paintEvent(QPaintEvent *e)
{
    switch(mDataFlags)
    {
    case Request_Null:
    case Request_Exception:{
        QPainter p(viewport());
        p.fillRect(rect(), gInfoStyle->m_invalidBrush);
        p.setFont(gInfoStyle->m_invalidFont);
        int textLen = p.fontMetrics().width(mTipText);
        int textH = p.fontMetrics().height();
        int w = width()/2, h = height()/2;
        QRect rt(w-73/2, h-69/2, 73, 62);//83 83
        QPixmap pix(mTipUrl);
        if(!pix.isNull()){
            p.drawPixmap(rt, pix);
            QRect textRt(w-textLen/2, rt.y()+rt.height()+17, textLen, textH);
            p.setPen(gInfoStyle->m_invalidPen);
            p.drawText(textRt, Qt::AlignCenter, mTipText);
        }
    }break;
    case Requst_Normal:{
        QPainter p(viewport());
        p.fillRect(rect(),gInfoStyle->m_marketCldDetailBrush);
        QTableView::paintEvent(e);
    }
    }
}

void HandicapNewsView::initTableView()
{
    this->setModel(m_newsmodel);
    //隐藏水平滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QTableView::SingleSelection);

    this->verticalHeader()->setDefaultSectionSize(80);

    //隐藏表头
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    //设置最后一行填充剩余空间
    this->horizontalHeader()->setStretchLastSection(true);

    //设置代理
    this->setItemDelegate(new HandicapNewsDelegate(this));

    connect(this, &QAbstractItemView::clicked, this , &HandicapNewsView::onRowClicked);
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value){
        int max = verticalScrollBar()->maximum();
        if(value == max){
            int rowCnt = m_newsmodel->rowCount();
            qDebug() << "rowCnt:" << rowCnt;
            if( rowCnt >= 99) return;
            emit signalRequest();
        }
    });
}

void HandicapNewsView::onRowClicked(const QModelIndex &index)
{
    STVListData data;

    data = index.data(MYDATAROLE).value<STVListData>();
    emit signalClickItem(data.id);
    qDebug() << data.id;
}

HandicapNewsWidget::HandicapNewsWidget(QWidget *parent):
    BaseWidget(parent),
    m_newsview(new HandicapNewsView)
{
    connect(m_newsview,SIGNAL(signalClickItem(const QString&)),this,SLOT(onRowClicked(const QString&)));
    connect(m_newsview, &HandicapNewsView::signalRequest,[=]{
           m_apdater->requestList();
    });
    createWg();
}

void HandicapNewsWidget::setAdapter(HZInfoAdapterInterface *apdater,const QString& titleText)
{
    m_titleText = titleText;
    m_apdater = apdater;
    apdater->setVisitor(m_newsview);
}

void HandicapNewsWidget::setCode(const QVector<QString> &codeVec)
{
    m_apdater->setCode(codeVec);
}

void HandicapNewsWidget::afterActive()
{
    m_apdater->setNextpage(0);
    m_apdater->requestList();
}

void HandicapNewsWidget::afterDeactive()
{
    m_newsview->clearAll();
}

void HandicapNewsWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_newsview);
}

void HandicapNewsWidget::onRowClicked(const QString &id)
{
    STInfoData tmpDatas;
    tmpDatas.datas = m_newsview->m_newsmodel->getRecords();
    m_apdater->setFirstDatas(tmpDatas);
    m_apdater->setItemId(id);

    int type;
    if(m_titleText == QStringLiteral("个股新闻")) {
        type = HZInfomationDlg::News_Info;
    } else if(m_titleText == QStringLiteral("个股公告")) {
        type = HZInfomationDlg::Anncs_Info;
    } else if(m_titleText == QStringLiteral("个股研报")) {
        type = HZInfomationDlg::Research_Report;
    } else if(m_titleText == QStringLiteral("板块新闻")) {
        type = HZInfomationDlg::Plate_News;
    } else if(m_titleText == QStringLiteral("指数新闻")) {
        type = HZInfomationDlg::Index_News;
    }

//    HZInfomationDlg dlg(type, m_apdater);
//    dlg.exec();
    gInfoDlg->setVisiter(type,m_apdater);
}
