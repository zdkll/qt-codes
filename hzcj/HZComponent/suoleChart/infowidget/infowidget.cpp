#include "infowidget.h"
#include <QDesktopServices>
#include <QBoxLayout>
#include <QDebug>
#include <QUrl>
#include <QScrollBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include "commondelegate.h"
#include "hzinfomationdlg.h"
#include "infostyle.h"


///
/// \brief InfoWndTabView::InfoWndTabView
/// \param parent
///
InfoWndTabView::InfoWndTabView(TableModel *model, QWidget *parent)
    :QTableView(parent), mModel(model)
{
    setModel(mModel);
}

void InfoWndTabView::afterActive()
{
    mNextPage = 0;
    mDataFlags = Request_Null;
    mModel->clearData();
    mTipText = QStringLiteral("当前暂无数据");
    mTipUrl = ":/skin/default/img/infopage/NEWS01.png";
    startReq();
}

void InfoWndTabView::afterDeactive()
{
    mDataFlags = Request_Null;
    mModel->clearData();
}


void InfoWndTabView::paintEvent(QPaintEvent *e)
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
            QRect textRt(w-textLen/2, rt.y()+rt.height()+10, textLen, textH);
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

void InfoWndTabView::startReq(int page)
{
    m_callback = std::bind(&InfoWndTabView::onhotNewsArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    HzInfoCenter::getInstance()->reqHostNews(0,page,2, m_callback);
}

/**
 * @brief InfoWndTabView::onhotNewsArrived
 * @param errCode
 * @param vec
 * @param page
 * 根据code和page值，确定当前显示样式
 * 展示页最大显示50条~
 */
void InfoWndTabView::onhotNewsArrived(const InfoRet &errCode, HostNews &vec, int page)
{
    mNextPage = page;
    if(mModel == nullptr) return;
    if(REQUEST_OPERATOR_EXCEPTION != errCode.code){
        mDataFlags = Request_Exception;
        mModel->setRecords(HostNews{});
        mTipText = QStringLiteral("操作失败，未知错误");
        mTipUrl = ":/skin/default/img/infopage/NEWS02.png";
        update();
    } else {
        if(!vec.isEmpty()) {
            mDataFlags = Requst_Normal;
            if( mModel->getRecords().count() < 50) {
                mModel->setRecords(vec);
                startReq(page);
            }
        } else {
            if(mModel->getRecords().isEmpty()) {
                mDataFlags = Request_Null;
                mModel->setRecords(HostNews{});
                mTipText = QStringLiteral("当前暂无数据");
                mTipUrl = ":/skin/default/img/infopage/NEWS01.png";
                update();
            }
        }
    }
}



///
/// \brief InfoWidget::InfoWidget
/// \param parent
///
InfoWidget::InfoWidget(QWidget *parent):
    QWidget(parent),
    m_model(new TableModel(this))
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    table_view = new InfoWndTabView(m_model,this);
    layout->addWidget(table_view);
    initTableView();
}

InfoWidget::~InfoWidget()
{

}

void InfoWidget::resizeEvent(QResizeEvent *event)
{
    int width = this->width();

    table_view->setColumnWidth(0,width*0.8);

    table_view->setColumnWidth(1,width*0.2);

}

void InfoWidget::afterActive()
{
    if(table_view)
        table_view->afterActive();
}

void InfoWidget::afterDeactive()
{
    if(table_view)
        table_view->afterDeactive();
}

void InfoWidget::initTableView()
{
    table_view->horizontalHeader()->setStretchLastSection(true);

//    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );//根据列内容来定列宽
    table_view->verticalHeader()->hide();
    table_view->horizontalHeader()->hide();

    table_view->verticalHeader()->setDefaultSectionSize(22);

    table_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table_view->setShowGrid(false);
    table_view->setEditTriggers(QTableView::NoEditTriggers);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view->setItemDelegate(new CommonDelegate(this));
    connect(table_view, &QAbstractItemView::clicked, this, &InfoWidget::onRowClicked);
}

void InfoWidget::onRowClicked(const QModelIndex &index)
{
    QString curId = index.data(UrlIdRole).toString();
    STInfoData tmpDatas;
    HostNews hns = table_view->mModel->getRecords();
    for(const HostNewItem &hn:hns) {
        STVListData rd;
        rd.id       = hn.id;     //id
        rd.body     = hn.title;   //标题
        rd.times    = QDateTime::fromTime_t(hn.time).toString("yyyy-MM-dd hh:mm:ss");  //时间
        rd.desc     = hn.desc;   //摘要
        rd.tag      = hn.tag;    //状态
        rd.source   = hn.source; //来源
        rd.url      = hn.url;//详情页地址
        tmpDatas.datas.push_back(rd);
    }

    HZRMZXInfo infoInf(curId);
    infoInf.setFirstDatas(tmpDatas);
    infoInf.setNextpage(table_view->mNextPage);

//    HZInfomationDlg dlg(HZInfomationDlg::Host_News, &infoInf);
//    dlg.exec();

    gInfoDlg->setVisiter(HZInfomationDlg::Host_News, &infoInf);
}


