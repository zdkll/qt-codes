#include "strategytodaychancewnd.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPainter>
#include <QDateTime>
#include <qfontmetrics.h>
#include "strategydynaprovider.h"
#include "hzinfocenter.h"
#include "httpdata.h"
#include "quoteformat.h"
#include "viewtipspanel.h"

#define ChanceDataIsEmpty 0
#define ChanceDataIsErr 1
#define ChanceDataNoraml 2


StrategyTodayChanceWnd::StrategyTodayChanceWnd(QWidget *parent, ChanceType type) : BaseWidget(parent),
    m_chanceType(type)
{
    createWg();
}

void StrategyTodayChanceWnd::setDynaProvider(DynaDataProvider *pProvider)
{
    m_pProvider = pProvider;
    connect(m_pProvider, SIGNAL(signalUpdateData()),this, SLOT(slotsTimer()));
}

void StrategyTodayChanceWnd::afterActive()
{
    TopicInvestCallBack fun = std::bind(&StrategyTodayChanceWnd::onTopicInfoCallBack, this, std::placeholders::_1, std::placeholders::_2);
    cancleTopicReq();
    m_reqTopicId = HzInfoCenter::getInstance()->reqTopicInvest(fun);
}

void StrategyTodayChanceWnd::afterDeactive()
{
    cancleTopicReq();
    QStandardItemModel * pModel = (QStandardItemModel *)m_pTodyChanceView->model();
    if(pModel && pModel->rowCount() > 0)
        pModel->removeRows(0,pModel->rowCount());
}

void StrategyTodayChanceWnd::setTitleText(const QString &strText)
{
    m_pTitleLabel->setText(strText);
}

void StrategyTodayChanceWnd::selectFirstRow()
{
    m_pTodyChanceView->setCurrentIndex(m_pTodyChanceView->model()->index(0,0));
    TodayChanceItem firstItem = m_pTodyChanceView->model()->index(0,0).data(Qt::UserRole + 1).value<TodayChanceItem>();
    signalsClickId(firstItem.topic_id,firstItem.codeName);

    qDebug() << Q_FUNC_INFO << firstItem.topic_id << firstItem.codeName;

}

void StrategyTodayChanceWnd::clearSelect()
{
    m_pTodyChanceView->clearSelection();
}

void StrategyTodayChanceWnd::slotsTimer()
{
    if(m_pInfoTipsPanel->getCurrType() == ChanceDataNoraml)
    {
        QStandardItemModel * pModel = (QStandardItemModel *)m_pTodyChanceView->model();
        if(pModel && pModel->rowCount() > 0)
        {
            for(int i=0;i < pModel->rowCount();i++)
            {
                QStandardItem *item = pModel->item(i);
                QVariant var = item->data(Qt::UserRole+1);
                TodayChanceItem itemData = var.value<TodayChanceItem>();
                QVector<TodayChanceDynaData>& stocks = itemData.relationStocks;
                for(int j =0;j < stocks.size();j++)
                {
                    auto dynaData = m_pProvider->getStockData(stocks[j].stockCode);
                    stocks[j].rateItem = HzQuoteFormat::QuoteFormat::formatChangeRate(dynaData.rate,dynaData.last_price,dynaData.pre_close_price);
                }
                item->setData(QVariant::fromValue(itemData), Qt::UserRole+1);
            }
            m_pTodyChanceView->update();
            emit updateReady();
        }
    }
}

void StrategyTodayChanceWnd::slotsClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QVariant var = index.data(Qt::UserRole+1);
        TodayChanceItem itemData = var.value<TodayChanceItem>();
        signalsClickId(itemData.topic_id,itemData.codeName);
    }
}

void StrategyTodayChanceWnd::onTopicInfoCallBack(const InfoRet &ret, const TopicInvest &data)
{
    qDebug() << "onTopicInfoCallBack: data size: " << data.todayChance.size();

    if(m_chanceType == TodayChanceType){
        m_chanceDataVec = data.todayChance;
    }
    else{
        m_chanceDataVec = data.recentHotspot;
    }


    if(ret.code != 0)
    {
        m_pInfoTipsPanel->switchType(ChanceDataIsErr);
         return;
    }else if(m_chanceDataVec.isEmpty())
    {
        m_pInfoTipsPanel->switchType(ChanceDataIsEmpty);
        return;
    }else
    {
        m_pInfoTipsPanel->switchType(ChanceDataNoraml);
    }
    QStandardItemModel * pModel = (QStandardItemModel *)m_pTodyChanceView->model();
    if(pModel)
    {
        m_pProvider->removestockAll();
        QVector<QString> stockList;
        TodayChanceItem firstItem;
        for (int i=0; i<m_chanceDataVec.size(); ++i) {
            QStandardItem *pItem = new QStandardItem;
            TodayChanceItem itemData;
            itemData.topic_id = m_chanceDataVec[i].topic_id;
            itemData.codeName = m_chanceDataVec[i].topic_name;
            itemData.desc = m_chanceDataVec[i].topic_describe;
            if(itemData.desc.size() > 55)
                itemData.desc = itemData.desc.mid(0,58) + "...";
            itemData.strTime = QDateTime::fromTime_t(m_chanceDataVec[i].update_time).toString("HH:mm:ss");
            QVector<QString> stocks =  m_chanceDataVec[i].related_stock;
            for(int j =0;j < stocks.size() && j < 2;j++)
            {
                TodayChanceDynaData stItem;
                stItem.stockName = Httpdata::instance()->getCodeNameByCode(stocks[j]);
                stItem.stockCode = stocks[j];
                itemData.relationStocks.push_back(stItem);
                stockList.push_back(stItem.stockCode);
            }
            pItem->setData(QVariant::fromValue(itemData), Qt::UserRole+1);
            pModel->appendRow(pItem);
            if(i ==0)
                firstItem = itemData;
        }
//        if(pModel->rowCount() > 0)
//        {
//             m_pTodyChanceView->setCurrentIndex(pModel->index(0,0));
//              signalsClickId(firstItem.topic_id,firstItem.codeName);

//        }
        m_pProvider->addsubstock(stockList);
    }


//    if(ret.code != 0)
//    {
//        m_pInfoTipsPanel->switchType(ChanceDataIsErr);
//         return;
//    }else if(data.todayChance.isEmpty())
//    {
//        m_pInfoTipsPanel->switchType(ChanceDataIsEmpty);
//        return;
//    }else
//    {
//        m_pInfoTipsPanel->switchType(ChanceDataNoraml);
//    }
//    QStandardItemModel * pModel = (QStandardItemModel *)m_pTodyChanceView->model();
//    if(pModel)
//    {
//        m_pProvider->removestockAll();
//        QVector<QString> stockList;
//        TodayChanceItem firstItem;
//        for (int i=0; i<data.todayChance.size(); ++i) {
//            QStandardItem *pItem = new QStandardItem;
//            TodayChanceItem itemData;
//            itemData.topic_id = data.todayChance[i].topic_id;
//            itemData.codeName = data.todayChance[i].topic_name;
//            itemData.desc = data.todayChance[i].topic_describe;
//            if(itemData.desc.size() > 55)
//                itemData.desc = itemData.desc.mid(0,58) + "...";
//            itemData.strTime = QDateTime::fromTime_t(data.todayChance[i].update_time).toString("HH:mm:ss");
//            QVector<QString> stocks =  data.todayChance[i].related_stock;
//            for(int j =0;j < stocks.size() && j < 2;j++)
//            {
//                TodayChanceDynaData stItem;
//                stItem.stockName = Httpdata::instance()->getCodeNameByCode(stocks[j]);
//                stItem.stockCode = stocks[j];
//                itemData.relationStocks.push_back(stItem);
//                stockList.push_back(stItem.stockCode);
//            }
//            pItem->setData(QVariant::fromValue(itemData), Qt::UserRole+1);
//            pModel->appendRow(pItem);
//            if(i ==0)
//                firstItem = itemData;
//        }
//        if(pModel->rowCount() > 0)
//        {
//             m_pTodyChanceView->setCurrentIndex(pModel->index(0,0));
//              signalsClickId(firstItem.topic_id,firstItem.codeName);

//        }
//        m_pProvider->addsubstock(stockList);
//    }
}

void StrategyTodayChanceWnd::createWg()
{
    QVBoxLayout* pLayout  = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    //创建标题
    m_pTitleLabel = new QLabel();
    m_pTitleLabel->setFixedHeight(26);
    m_pTitleLabel->setText(QStringLiteral("主题投资-今日机会"));
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addSpacing(1);

    //创建今日机会页面
    QVector<TipsItem> tipList;
    TipsItem tipEmpty;
    tipEmpty.btObjName = "dataEmptyBt";
    tipEmpty.descLabelName = "dataEmptyLable";
    tipEmpty.descText = QStringLiteral("当前暂无数据");
    tipEmpty.type = ChanceDataIsEmpty;
    tipList.push_back(tipEmpty);
    TipsItem tipErr;
    tipErr.btObjName = "dataErrBt";
    tipErr.descLabelName = "dataErrLable";
    tipErr.descText = QStringLiteral("操作失败，稍后重试");
    tipErr.type = ChanceDataIsErr;
    tipList.push_back(tipErr);

    m_pInfoTipsPanel =new ViewTipsPanel(this);
    m_pInfoTipsPanel->setTipItems(tipList);
    m_pInfoTipsPanel->setNormalWnd(createListView(),ChanceDataNoraml);
    m_pInfoTipsPanel->createWg();
    pLayout->addWidget(m_pInfoTipsPanel);

    QStandardItemModel *pModel = new QStandardItemModel();
    TodayChanceDelegate *pItemDelegate = new TodayChanceDelegate(this,this);
    m_pTodyChanceView->setItemDelegate(pItemDelegate);
    m_pTodyChanceView->setModel(pModel);
}

QWidget *StrategyTodayChanceWnd::createListView()
{
    m_pTodyChanceView = new QListView();
    connect(m_pTodyChanceView, SIGNAL(clicked(const QModelIndex &)),this, SLOT(slotsClicked(const QModelIndex &)));
    return m_pTodyChanceView;
}

void StrategyTodayChanceWnd::cancleTopicReq()
{
    if(m_reqTopicId != -1)
    {
         HzInfoCenter::getInstance()->cancle(m_reqTopicId);
         m_reqTopicId = -1;
    }
}

TodayChanceDelegate::TodayChanceDelegate(StrategyTodayChanceWnd* pStyle,QObject *parent):
    QStyledItemDelegate(parent),
    m_style(pStyle)
{
}

TodayChanceDelegate::~TodayChanceDelegate()
{

}

void TodayChanceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();
        QVariant var = index.data(Qt::UserRole+1);
        TodayChanceItem itemData = var.value<TodayChanceItem>();

        // item 矩形区域
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        QPainterPath path;
        path.moveTo(rect.topRight());
        path.lineTo(rect.topLeft());
        path.quadTo(rect.topLeft(), rect.topLeft());
        path.lineTo(rect.bottomLeft());
        path.quadTo(rect.bottomLeft(), rect.bottomLeft());
        path.lineTo(rect.bottomRight());
        path.quadTo(rect.bottomRight(), rect.bottomRight());
        path.lineTo(rect.topRight());
        path.quadTo(rect.topRight(), rect.topRight());

        // 鼠标悬停或者选中时改变背景色
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(QPen(m_style->m_itemSelectedClr));
            painter->setBrush(m_style->m_itemSelectedClr);
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(m_style->m_itemHoverClr));
            painter->setBrush(m_style->m_itemHoverClr);
            painter->drawPath(path);
        }
        painter->setPen(QPen(m_style->getItemBorderClr()));
        painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
        // 策略名称
        QRectF rcObjName = rect;
        rcObjName.setLeft(rect.left()+14);
        rcObjName.setBottom(rect.top()+25);
        QFont fontObjName;
        fontObjName.setFamily(m_style->m_codeNameFontFamily);
        fontObjName.setPixelSize(m_style->m_codeNameFontSize);
        painter->setPen(QPen(m_style->m_codeNameTextClr));
        painter->setFont(fontObjName);
        painter->drawText(rcObjName,itemData.codeName,Qt::AlignBottom|Qt::AlignLeft);

        //时间
        QRectF rcTime = rect;
        rcTime.setRight(rect.right()-10);
        rcTime.setBottom(rect.top()+25);
        QFont fontTime;
        fontTime.setFamily(m_style->m_descTextFontFamily);
        fontTime.setPixelSize(m_style->m_descTextFontSize);
        painter->setPen(QPen(m_style->m_descTextTextClr));
        painter->setFont(fontTime);
        painter->drawText(rcTime,itemData.strTime,Qt::AlignBottom|Qt::AlignRight);

        //绘制内容
        QRectF rcContent = rect;
        rcContent.setTop(rect.top()+36);
        rcContent.setLeft(rect.left()+14);
        rcContent.setRight(rect.right()-86);
        rcContent.setBottom(rect.bottom()-35);
        painter->drawText(rcContent,itemData.desc,Qt::AlignVCenter|Qt::AlignLeft);

        //绘制股票代码
        QString strTemp;
        TodayChanceDynaData tempData;
        QRectF tempRc;
        QRectF rc = rect;
        rc.setLeft(rc.left()+14);
        rc.setTop(rcContent.bottom());
        rc.setBottom(rect.bottom()-10);
        QFont dynaFont;
        dynaFont.setFamily(m_style->m_codeNameFontFamily);
        dynaFont.setPixelSize(m_style->m_codeNameFontSize);
        dynaFont.setBold(true);
        for(int i = 0;i < itemData.relationStocks.size();i++)
        {
            //股票名称
            tempData = itemData.relationStocks[i];
            QFontMetrics fm(fontObjName);
            rc.setRight(rc.left()+fm.boundingRect(tempData.stockName).width()+5);
            tempRc = rc;
            painter->setFont(fontObjName);
            painter->setPen(QPen(m_style->m_codeNameTextClr));
            painter->drawText(rc,tempData.stockName,Qt::AlignBottom|Qt::AlignLeft);

            //涨跌幅
            rc.setRight(tempRc.left()+130);
            rc.setLeft(tempRc.right());

            painter->setFont(dynaFont);
            strTemp = tempData.rateItem.strValue;
            if(tempData.rateItem.dirc == HzQuoteFormat::quote_up)
            {
                painter->setPen(QPen(m_style->m_upClr));
                strTemp = "+"+strTemp;
            }else if(tempData.rateItem.dirc == HzQuoteFormat::quote_down)
            {
                painter->setPen(QPen(m_style->m_downClr));
            }else
            {
                painter->setPen(QPen(m_style->m_equalClr));
            }
            painter->drawText(rc,strTemp,Qt::AlignBottom|Qt::AlignLeft);
            rc.setLeft(rc.right());
        }
        painter->restore();
    }
}

QSize TodayChanceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 101);
}

//TodayChanceModel::TodayChanceModel(int type, QObject *parent)
//{

//}

//TodayChanceModel::~TodayChanceModel()
//{

//}

//int TodayChanceModel::rowCount(const QModelIndex &parent) const
//{
//    return 0;
//}

//QVariant TodayChanceModel::data(const QModelIndex &index, int role) const
//{
//    return QVariant();
//}
