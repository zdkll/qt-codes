/********************************************************************
created:2019.03.04
author: juntang.xie
purpose: 策略相关资讯窗口
*********************************************************************/
#include "strategyrelatedinfowidget.h"
#include <QPainter>
#include <QApplication>
#include <QDateTime>
#include <QBoxLayout>
#include <QScrollBar>
#include <QHeaderView>
#include <QDateTime>
#include <QLabel>
#include "infostyle.h"



StrategyRelatedInfoDelegate::StrategyRelatedInfoDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void StrategyRelatedInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    TopicInvestNews::STNewList  data = index.data(MYDATAROLE).value<TopicInvestNews::STNewList>();

    //去掉Focus
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    //画线和圆圈
    painter->setPen(QColor("#2f2f2f"));
    painter->drawLine(26, option.rect.top(), 26, option.rect.bottom());

    QPixmap circelpix(":/skin/default/img/1mainwindow/smallparts/circle01.png");

    painter->drawPixmap(22, option.rect.top() + 16, circelpix);


    QFontMetrics metrics = painter->fontMetrics();

    QFont font;
//    font.setStyleStrategy(QFont::NoAntialias);
    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.titlePixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.titileColor());
    QRect rc = option.rect;
    rc.adjust(53, 14, 0, 0 );
    //画标题
    QString strTitle = painter->fontMetrics().elidedText(data.title, Qt::ElideRight, rc.width());
    painter->drawText(rc, strTitle);

    rc.adjust(0, 0, -22, 0 );

    QString strTime = QDateTime::fromTime_t(data.time).toString(" yyyy-MM-dd HH:mm:ss");

    QString strText = data.source + strTime;

    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.textPixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.textColor());
    //画来源和时间
    painter->drawText(rc,Qt::AlignRight,strText);

    int nWidth, nHeight;
    QPixmap pix;
    QColor tagColor;

    QString strType = "";
    switch (data.tag) {
    case NeutralTag:       //"中性"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.zxPixmap();
        tagColor = m_styledWidget.zxColor();
        strType = "中性";
        break;
    case BullTag:    //"利好"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.lhPixmap();
        tagColor = m_styledWidget.lhColor();
        strType = "利好";
        break;
    case BadNewsTag:    //"利空"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.lkPixmap();
        tagColor = m_styledWidget.lkColor();
        strType = "利空";
        break;
    case StrongBullTag:     //"强烈利好"
        nWidth = 39;
        nHeight = 14;
        pix = m_styledWidget.qllhPixmap();
        tagColor = m_styledWidget.qllhColor();
        strType = "强烈利好";
        break;
    case StrongBadNewsTag:  //"强烈利空
        nWidth = 39;
        nHeight = 14;
        pix = m_styledWidget.qllkPixmap();
        tagColor = m_styledWidget.qllkColor();
        strType = "强烈利空";
        break;
    default:
        nWidth = 0;
        nHeight = 0;
        break;
    }

    QRect rect(option.rect);



    rect.adjust(53, 24 + metrics.height(), 0, 0);

    if(UnknownTag != data.tag)
    {
        //画状态
        painter->drawPixmap(rect.left(), rect.top(), nWidth, nHeight,  pix);

        rect.adjust(3, 1, 0, 0);

        font.setFamily(m_styledWidget.labelFontFamily());
        font.setPixelSize(m_styledWidget.labelPixelSize());
        painter->setFont(font);
        painter->setPen(tagColor);
        painter->drawText(rect, strType);

        rect.adjust(nWidth - 3 + 6, -1, 0, 0);
    }

    //画摘要
    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.textPixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.textColor());

    QString strSummary = painter->fontMetrics().elidedText(data.intro, Qt::ElideRight, rect.width());

    painter->drawText(rect,strSummary);
}


StrategyRelatedInfoModel::StrategyRelatedInfoModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

int StrategyRelatedInfoModel::rowCount(const QModelIndex &parent) const
{
    return m_lists.size();
}

int StrategyRelatedInfoModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant StrategyRelatedInfoModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case MYDATAROLE:
        return QVariant::fromValue(m_lists[row]);
        break;
    default:
        break;
    }
    return QVariant();
}


void StrategyRelatedInfoModel::updateList(const QVector<TopicInvestNews::STNewList> &vec)
{
    m_lists.append(vec);
    refrush();
}

void StrategyRelatedInfoModel::clearAll()
{
    m_lists.clear();
    refrush();
}

void StrategyRelatedInfoModel::refrush()
{
    beginResetModel();
    endResetModel();
}

StrategyRelatedInfoView::StrategyRelatedInfoView(QWidget *parent):
    QTableView(parent),
    m_newsmodel(new StrategyRelatedInfoModel(this))
{
    initTableView();
}

void StrategyRelatedInfoView::clearAll()
{
    m_newsmodel->clearAll();
    this->verticalScrollBar()->setValue(0);
}


void StrategyRelatedInfoView::updateList(const QVector<TopicInvestNews::STNewList> &vec, int nCode)
{
    if(0 != nCode){ //except
        mDataFlags = Request_Exception;
        m_newsmodel->updateList(QVector<TopicInvestNews::STNewList>{});
        mTipText = QStringLiteral("操作失败，未知错误");
        mTipUrl = ":/skin/default/img/infopage/NEWS02.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    } else {
        if(nCode == 0 && !vec.isEmpty()) {//normal
            mDataFlags = Requst_Normal;
            m_newsmodel->updateList(vec);
        } else {
            mDataFlags = Request_Null;
            m_newsmodel->updateList(QVector<TopicInvestNews::STNewList>{});
            mTipText = QStringLiteral("当前暂无数据");
            mTipUrl = ":/skin/default/img/infopage/NEWS01.png";
            gInfoStyle->signalWebAboutBlank();
            update();
        }
    }
}

void StrategyRelatedInfoView::paintEvent(QPaintEvent *e)
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

void StrategyRelatedInfoView::initTableView()
{
    this->setModel(m_newsmodel);
    //隐藏水平滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QTableView::SingleSelection);

    this->verticalHeader()->setDefaultSectionSize(62);

    //隐藏表头
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    //设置最后一行填充剩余空间
    this->horizontalHeader()->setStretchLastSection(true);

    //设置代理
    this->setItemDelegate(new StrategyRelatedInfoDelegate(this));

    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value){
        int max = verticalScrollBar()->maximum();
        if(value == max){
            emit signalRequest();
        }
    });
}



StrategyRelatedInfowidget::StrategyRelatedInfowidget(QWidget *parent):
    BaseWidget(parent),
    m_newsview(new StrategyRelatedInfoView)
{
    connect(m_newsview, &StrategyRelatedInfoView::signalRequest,[=]{
        requestNews();
    });
    createWg();
}

void StrategyRelatedInfowidget::afterActive()
{

}

void StrategyRelatedInfowidget::afterDeactive()
{
    m_newsview->clearAll();

    if(m_nReq != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_nReq);
        m_nReq = -1;
    }
}

void StrategyRelatedInfowidget::setNextPage(int nPage)
{
    m_nNextPage = nPage;
}

void StrategyRelatedInfowidget::requestTopicNews(int topic_id)
{
    m_nTopic_id = topic_id;
    requestNews();
}

void StrategyRelatedInfowidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    QWidget *titleWidget = new QWidget;
    titleWidget->setObjectName("title");
    QLabel *label = new QLabel(QStringLiteral("相关资讯"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(22);
    layout->addWidget(label);
    layout->addStretch();
    titleWidget->setLayout(layout);
    mainLayout->addWidget(titleWidget);
    mainLayout->addWidget(m_newsview);
}

void StrategyRelatedInfowidget::onTopicInvestNews(const InfoRet &ret, const TopicInvestNews &data)
{

    m_nNextPage = data.nextpage;

    m_newsview->updateList(data.datas, ret.code);
}

void StrategyRelatedInfowidget::requestNews()
{

    TopicInvestNewsCallBack func = std::bind(&StrategyRelatedInfowidget::onTopicInvestNews, this, std::placeholders::_1, std::placeholders::_2);

    if(m_nNextPage == -1)
        return;

    m_nReq = HzInfoCenter::getInstance()->reqTopicInvestNews(m_nTopic_id, m_nNextPage, func);
}


