#include "newsview.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include "newsdelegate.h"
#include "newsmodel.h"
#include "infodef.h"
#include <QPaintEvent>
#include <QPainter>

#include "infostyle.h"

NewsView::NewsView(QWidget *parent):
    QTableView(parent),
    m_newsModel(new NewsModel(this))
{
    this->setFixedHeight(186);
//    this->setMinimumWidth(900);
    initTableView();
}

void NewsView::clearAll()
{
    m_newsModel->clearAll();
    this->verticalScrollBar()->setValue(0);
}

void NewsView::updateList(const QVariantMap &data)//QList<STVListData>
{
    QString id          = data["id"].toString();
    STInfoData lst      = data["data"].value<STInfoData>();
    int code            = data["code"].toInt();
    int page            = data["page"].toInt();

    //qDebug() << Q_FUNC_INFO << "code:" << code << "page:" << page;
    if(0 != code){ //except
        mDataFlags = Request_Exception;
        m_newsModel->updateList(QList<STVListData>{});
        mTipText = QStringLiteral("操作失败，未知错误");
        mTipUrl = ":/skin/default/img/infopage/NEWS02.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    } else {
        if(code == 0 && (page != -2 || !lst.datas.isEmpty()) ) {//normal
            mDataFlags = Requst_Normal;
            m_newsModel->updateList(lst.datas);
            //setSelected(id);
        } else {
            mDataFlags = Request_Null;
            m_newsModel->updateList(QList<STVListData>{});
            mTipText = QStringLiteral("当前暂无数据");
            mTipUrl = ":/skin/default/img/infopage/NEWS01.png";
            gInfoStyle->signalWebAboutBlank();
            update();
        }
    }
}

void NewsView::paintEvent(QPaintEvent *e)
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

void NewsView::initTableView()
{
    this->setModel(m_newsModel);
    //隐藏水平滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QTableView::SingleSelection);

    this->verticalHeader()->setDefaultSectionSize(61);

    //隐藏表头
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    //设置最后一行填充剩余空间
    this->horizontalHeader()->setStretchLastSection(true);

    //设置代理
    this->setItemDelegate(new NewsDelegate(this));

    connect(this, &QAbstractItemView::clicked, this , &NewsView::onRowClicked);
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value){
        int max = verticalScrollBar()->maximum();
        if(value == max){
            int rowCnt = m_newsModel->rowCount();
            qDebug() << "rowCnt:" << rowCnt;
            if( rowCnt >= 99) return;
            emit signalRequest();
        }
    });
}

void NewsView::onRowClicked(const QModelIndex &index)
{
    STVListData data;

    data = index.data(MYDATAROLE).value<STVListData>();
    emit signalClickItem(data.id);
    qDebug() << data.id;
}
