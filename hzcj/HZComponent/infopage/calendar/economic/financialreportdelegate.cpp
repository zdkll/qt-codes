#include "financialreportdelegate.h"
#include <QPixmap>
#include <QEvent>
#include "hzinfodata.h"
#include "economicmaster.h"

#define FinancialItemWidth 440
#define FinancialItemXOffset 25
#define CAIBAO_CONTENT_PLATE "%1%2号发布财报"

FinancialReportDelegate::FinancialReportDelegate(EconomicView* pView,QObject *parent)
    :EconomicDelegate(FinancialReport_Item,parent),
     m_pView(pView)
{
    m_reportFont.setFamily("SimSun");
    m_reportFont.setPixelSize(14);
    m_reportFont.setStyleStrategy(QFont::NoAntialias);
    m_reportfm = new QFontMetrics(m_reportFont);

    m_optionFont.setFamily("SimSun");
    m_optionFont.setPixelSize(10);
    m_optionFont.setStyleStrategy(QFont::NoAntialias);
    m_optionfm = new QFontMetrics(m_optionFont);

    m_showFont.setFamily("SimSun");
    m_showFont.setPixelSize(14);
    m_showFont.setStyleStrategy(QFont::NoAntialias);
    m_showfm = new QFontMetrics(m_showFont);
}

void FinancialReportDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVector<FinancialReport::FinancialItem> items = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<QVector<FinancialReport::FinancialItem>>();
    QPoint currPt = m_pt;

#if 0
    qDebug() << m_pView->cursor().pos();
    qDebug() << m_pView->mapFromParent(m_pView->cursor().pos());
    qDebug() << m_pView->mapToGlobal(m_pView->cursor().pos());
    qDebug() << m_pView->mapTo(m_pView,m_pView->cursor().pos());
#endif

    quint32 time = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM_TIME).toUInt();
    QVector<FinaReportDelItem> item = calcDrawData(items, option.rect,time);
    bool bHover =  option.state & QStyle::State_MouseOver;
    QPixmap pix(":/skin/default/img/infopage/zixuan.png");

    QPen pen;
    pen.setColor("#CAD4DC");

    QPen hightPen;
    hightPen.setColor("#ffffff");

    QPen detailHightPen;
    detailHightPen.setColor("#4f9bff");

    //绘制圆圈
    QPoint pt;
    pen.setWidth(2);
    painter->setPen(pen);
    for(int i=0;i < item.size();i++)
    {
        if(bHover && item[i].stockRc.contains(currPt))
        {
            detailHightPen.setWidth(2);
            painter->setPen(detailHightPen);
            pt.setX(item[i].reportRc.left()-16);
            pt.setY(item[i].reportRc.top() + item[i].reportRc.height()/2);
            painter->drawEllipse(pt,3,3);
            detailHightPen.setWidth(1);
            painter->setPen(pen);
        }else
        {
            pt.setX(item[i].reportRc.left()-16);
            pt.setY(item[i].reportRc.top() + item[i].reportRc.height()/2);
            painter->drawEllipse(pt,3,3);
        }
    }


    //绘制财报信息
    painter->setFont(m_reportFont);
    painter->setPen(pen);
    for(int i=0;i < item.size();i++)
    {
        if(bHover && item[i].stockRc.contains(currPt))
        {
            painter->setPen(hightPen);
            painter->drawText(item[i].reportRc,Qt::AlignLeft|Qt::AlignVCenter,item[i].reportStr);
            painter->setPen(pen);
        }else
        {
            painter->drawText(item[i].reportRc,Qt::AlignLeft|Qt::AlignVCenter,item[i].reportStr);
        }
    }

    //绘制自选信息
    for(int i=0;i < item.size();i++)
    {
        painter->drawPixmap(item[i].optionRc,pix);
    }

    //绘制查看详情信息
    painter->setFont(m_showFont);
    painter->setPen(pen);
    for(int i=0;i < item.size();i++)
    {
        if(bHover && item[i].stockRc.contains(currPt))
        {
            painter->setPen(detailHightPen);
            painter->drawText(item[i].showRc,Qt::AlignLeft|Qt::AlignVCenter,item[i].showStr);
            painter->setPen(pen);
        }else
        {
            painter->drawText(item[i].showRc,Qt::AlignLeft|Qt::AlignVCenter,item[i].showStr);
        }
    }
    EconomicDelegate::paint(painter,option,index);
}

QSize FinancialReportDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(m_size.width(), 30);
}

bool FinancialReportDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseMove) {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        if(e)
        {
            m_pt = e->pos();
            quint32 time = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM_TIME).toUInt();
            QVector<FinancialReport::FinancialItem> items = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<QVector<FinancialReport::FinancialItem>>();
            QVector<FinaReportDelItem> item = calcDrawData(items, option.rect,time);
            QRect rc = m_currItemRc;
            for(int i=0;i < item.size();i++)
            {
                if(item[i].stockRc.contains(m_pt))
                {
                    rc = item[i].stockRc;
                    break;
                }
            }
            if(rc != m_currItemRc)
            {
                m_currItemRc = rc;
                m_pView->update(m_currItemRc);
                qDebug() << Q_FUNC_INFO << e->pos();
            }
        }
    }
    return false;
}


QVector<FinaReportDelItem> FinancialReportDelegate::calcDrawData(const QVector<FinancialReport::FinancialItem> &items, QRect rc, quint32 time) const
{
    rc.setLeft(rc.left()+71+FinancialItemXOffset);
    QVector<FinaReportDelItem> vec;
    int hight =0,midPos=0;
    QDate curDate = QDateTime::currentDateTime().date();
    QDate dtDate = QDateTime::fromTime_t(time).date();
    QString timeStr = dtDate.toString("MM/dd");
    for(int i = 0;i < items.size();i++)
    {
        FinaReportDelItem item;
        qDebug() << "time:" << time;
        QString prefix = dtDate>curDate?QStringLiteral("将于"):dtDate<curDate?QStringLiteral("已"):"";
        item.reportStr = items[i].name + "(" + items[i].code + ")" + QString(CAIBAO_CONTENT_PLATE).arg(prefix).arg(timeStr);
        item.stockRc = item.reportRc = rc;
        item.reportRc.setRight(item.reportRc.left()+ m_reportfm->boundingRect(item.reportStr).width());
        item.url = items[i].url;
        qDebug() << "isCustom:" << items[i].isCustom;
        if(items[i].isCustom) //自选
        {
            item.optionStr = QStringLiteral("自选");
            item.optionRc = item.reportRc;
            item.optionRc.setLeft(item.reportRc.right()+5);
            item.optionRc.setRight(item.optionRc.left() + m_optionfm->boundingRect(item.optionStr).width());
            hight = m_optionfm->height()/2+1;
            midPos = item.optionRc.top()+item.optionRc.height()/2;
            item.optionRc.setTop(midPos-hight);
            item.optionRc.setBottom(midPos+hight);
            item.stockRc.setRight(item.optionRc.right());
        }
        if(items[i].detail)
        {
            item.showRc = item.reportRc;
            item.showRc.setLeft(item.optionRc.isEmpty()?item.reportRc.right():item.optionRc.right()+5);
            item.showStr = "["+QStringLiteral("查看")+"]";
            item.showRc.setRight(item.showRc.left()+m_showfm->boundingRect(item.showStr).width());
            item.stockRc.setRight(item.showRc.right());
        }
        item.stockCode = items[i].code;
        vec.push_back(item);
        rc.setLeft(rc.left()+FinancialItemWidth);
    }
    return vec;
}
