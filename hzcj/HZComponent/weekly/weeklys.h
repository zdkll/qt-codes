/********************************************************************
created:2019.1
author: jdxchina
purpose: 对外疯转类和各个视图小组件的对完
*********************************************************************/
#ifndef WEEKLYS_H
#define WEEKLYS_H

#include <QtCore>
#include <QtWidgets>
#include <QHeaderView>
#include <QDateTime>
#include <customheaderdelegate.h>
#include "weeklydelegates.h"
#include "weeklymodel.h"
#include "weeklydef.h"

#include <HzInfoCenter/hzinfodata.h>

namespace Ui {
class weeklys;
}

//日历栏
class WeeklyCalandarBar : public QFrame, public WeeklyWndUpdate
{
    Q_OBJECT
public:
    WeeklyCalandarBar(QWidget *parent=0):QFrame(parent)
    {
        setObjectName("WeeklyCalandarBar");
        setFixedHeight(28);

        m_textFont.setFamily("SimSun");
        m_textFont.setPixelSize(12);

        m_numFont.setFamily("Arial");
        m_numFont.setPixelSize(12);
        m_numFont.setStyleStrategy(QFont::NoAntialias);
    }

    void updateFromContent(const QVariant &var)
    {
        OptionalWeekly data = var.value<OptionalWeekly>();
        QString beginDate = QDateTime::fromString(QString::number(data.sum.begin_date),"yyyyMMdd").toString("yyyy/MM/dd");
        QString endDate = QDateTime::fromString(QString::number(data.sum.end_date),"yyyyMMdd").toString("yyyy/MM/dd");
        QString dateRange(beginDate+"-"+endDate);
        setValue(dateRange);
    }

    void setValue(const QString &date)
    {
        m_date = date;
        update();
    }

protected:
    void paintEvent(QPaintEvent *);

private:
    QString m_date;
    QFont m_textFont, m_numFont;
};


//本周周报
class CurWeeklys : public QFrame, public WeeklyWndUpdate
{
    Q_OBJECT
public:
    enum{CurWeeklys_Market,CurWeeklys_Option};

    CurWeeklys(QWidget *parent = 0):QFrame(parent)
    {
        setObjectName("CurWeeklys");
        mBaseFont.setFamily("SimSun");
        mBaseFont.setPixelSize(14);

        mTitleTextFont.setFamily("SimSun");
        mTitleTextFont.setPixelSize(14);

        mTitleNumFont.setFamily("Arial");
        mTitleNumFont.setPixelSize(16);
        mTitleNumFont.setBold(true);

        mTitleMidTextFont.setFamily("SimSun");
        mTitleMidTextFont.setPixelSize(12);

        mYAxisFont.setFamily("ArialMT");
        mYAxisFont.setPixelSize(12);
    }

    void updateFromContent(const QVariant &var)
    {
        OptionalWeekly data = var.value<OptionalWeekly>();
        STCurWeeklyData tmpWeekly;
        tmpWeekly.markets = data.sum.indexs;
        tmpWeekly.selstks = data.sum.options;
        tmpWeekly.index_up = data.sum.index_rate_all;
        tmpWeekly.opt_up = data.sum.optional_rate_all;

        //计算x axis文本
        m_weekTexts.clear();
        QDate tmpDt(QDateTime::fromString(QString::number(data.sum.begin_date),"yyyyMMdd").date());
        m_weekTexts.push_back(tmpDt.toString("MM/dd"));
        for(int d=1; d<5; ++d) {
            tmpDt = tmpDt.addDays(1);
            if(tmpDt.dayOfWeek() == 7)
            {
                tmpDt = tmpDt.addDays(1);
                m_weekTexts.push_back(tmpDt.toString("MM/dd"));
            } else if(tmpDt.dayOfWeek() == 6)
            {
                tmpDt = tmpDt.addDays(2);
                m_weekTexts.push_back(tmpDt.toString("MM/dd"));
            }else
                m_weekTexts.push_back(tmpDt.toString("MM/dd"));
        }

        setValue(tmpWeekly);
    }

    /**
     * @brief setValue 更新本周周报数据
     * @param data
     */
    void setValue(const STCurWeeklyData &data)
    {
        m_Datas = data;

        m_Max = -qInf(), m_Min = qInf();
        //取两者中的最大值的最大值，最小值的最小值
        QVector<STIndexAndOptionRateDaily> selstks = data.selstks;
        for(int i=0; i<selstks.count(); ++i)
        {
            m_Max = qMax(selstks[i].Rate,m_Max);
            m_Min = qMin(selstks[i].Rate,m_Min);
        }

        QVector<STIndexAndOptionRateDaily> markets = data.markets;
        for(int i=0; i<markets.count(); ++i)
        {
            m_Max = qMax(markets[i].Rate, m_Max);
            m_Min = qMin(markets[i].Rate, m_Min);
        }

        //防止折线上的点正好在临界值上，故临界值稍微变动一下

        if(m_Min == qInf() || m_Max == -qInf())
        {
            m_yAxisVal = QVector<double>{0,0,0,0,0,0};
        }
        else {
            m_Max = qCeil(m_Max);
            m_Min = qFloor(m_Min);

            //计算右侧坐标值
            m_yAxisVal.clear();
            qreal ratio = (m_Max - m_Min) / 5.;
            for(int i=0; i<6; i++)
            {
                if(m_Min == qInf() || m_Min == -qInf())
                    m_yAxisVal.push_back(0);
                else
                    m_yAxisVal.push_back(m_Min+i*ratio);
            }
        }

        update();
    }

private:
    QFont mBaseFont, mTitleTextFont, mTitleNumFont,mTitleMidTextFont,mEllipseTitleFont,mYAxisFont;
    QVector<double> m_yAxisVal{6}; //从底部到顶部
    STCurWeeklyData m_Datas;
    double m_Max{-qInf()}, m_Min{qInf()};
    QStringList m_weekTexts;

    void paintEvent(QPaintEvent *);
    void drawTitle(QPainter *p);
    void drawGrid(QPainter *p);
    void drawXAxisAndLines(QPainter *p);
    void drawPointAndLinkLines(QPainter *p, const QBrush &br, const QPen &pen, const QPoint &p1, const QPoint &p2);
};


//本周资金异动
class CapitalChange : public QFrame, public WeeklyWndUpdate
{
    Q_OBJECT
public:
    enum{CapitalChange_In=0, CapitalChange_Out};
    CapitalChange(QWidget *parent = 0):QFrame(parent)
    {
        ///![1]
        setObjectName("CapitalChange");
        QHBoxLayout *childLayout = new QHBoxLayout;
        childLayout->setContentsMargins(0,0,0,0);
        childLayout->setSpacing(1);

        ///![2]
        QLabel *titleBar = new QLabel(QStringLiteral("  自选股本周资金异动"));
        titleBar->setFixedHeight(30);
        titleBar->setObjectName("CapitalChangeTitleBar");

        ///![3]
        QFrame *midFrame = new QFrame;
        midFrame->setFixedHeight(30);
        midFrame->setObjectName("MidTitleChangeFrame");

        QHBoxLayout *midLayout = new QHBoxLayout;
        midLayout->setContentsMargins(0,0,0,0);
        midLayout->setSpacing(1);

        QLabel *in_out_label = new QLabel(QStringLiteral("  资金流入"));
        in_out_label->setObjectName("in_out_label");
        midLayout->addWidget(in_out_label);

        in_out_label = new QLabel(QStringLiteral("  资金流出"));
        in_out_label->setObjectName("in_out_label");
        midLayout->addWidget(in_out_label);
        midFrame->setLayout(midLayout);
        midFrame->setContentsMargins(0,0,0,0);

        ///![4]
        QVBoxLayout *masterLayout = new QVBoxLayout;
        masterLayout->setContentsMargins(0,0,0,0);
        masterLayout->setSpacing(0);
        masterLayout->addWidget(titleBar);
        masterLayout->addWidget(midFrame);
        masterLayout->addLayout(childLayout);
        setLayout(masterLayout);

        ///![5]
        m_outDelegate = new WeeklyChgDelegate(CapitalChange_Out);
        m_outModel = new WeeklyChgModel(CurWeeklys_Type);
        m_outView = new QTableView;
        initTable(m_outModel, m_outView, m_outDelegate,childLayout);

        QLabel *vLine = new QLabel;
        vLine->setObjectName("VLine");
        vLine->setFixedWidth(1);
        childLayout->addWidget(vLine);

        m_inDelegate = new WeeklyChgDelegate(CapitalChange_In);
        m_inModel = new WeeklyChgModel(CurWeeklys_Type);
        m_inView = new QTableView;
        initTable(m_inModel, m_inView,m_inDelegate,childLayout);     
    }

    void updateFromContent(const QVariant &var)
    {
        OptionalWeekly data = var.value<OptionalWeekly>();
        QVector<STCapitalChange> ins, outs;
        for(const OptionalWeekly::STDetail &det: data.detail)
        {
            STCapitalChange in;
            qreal money = (det.money_in_all - det.money_out_all) /1000.;
            in.name = det.stockname;
            if(money > 0) {
                in.amount = money;
                ins << in;
            } else {
                in.amount = qAbs(money);
                outs << in;
            }
        }

        addInAmount(ins);
        addOutAmount(outs);
    }

    /**
     * @brief addInAmount 增加流入流出数据
     * @param val
     */
    void addInAmount(const QVector<STCapitalChange> &vals){m_outModel->setValue(QVariant::fromValue(vals));}
    void addOutAmount(const QVector<STCapitalChange> &vals){m_inModel->setValue(QVariant::fromValue(vals));}

protected:
    WeeklyChgModel *m_outModel, *m_inModel;
    QTableView *m_outView, *m_inView;
    WeeklyChgDelegate *m_inDelegate, *m_outDelegate;

    void initTable(WeeklyChgModel *model, QTableView *view, WeeklyChgDelegate* deleg, QBoxLayout *layout);
};

//本周涨跌
class CurUpDowns : public QFrame, public WeeklyWndUpdate
{
    Q_OBJECT
public:
    CurUpDowns(QWidget *parent = 0):QFrame(parent)
    {
        ///![1]
        setObjectName("CurUpDowns");
        m_itemDeleg = new WeeklyChgDelegate2(CurUpDowns_Type);
        m_upDownModel = new WeeklyChgModel(CurUpDowns_Type);
        m_proxyModel = new WeeklySortFilterProxyModel(CurUpDowns_Type);
        m_proxyModel->setSourceModel(m_upDownModel);

        m_upDownView = new QTableView;
        m_upDownView->setModel(m_proxyModel);
        m_upDownView->setItemDelegate(m_itemDeleg);
        m_upDownView->sortByColumn(4,Qt::DescendingOrder);

        ///![2]
        m_headerModel = new WeeklyListModel(CurUpDowns_Type);
        m_headerModel->setValue(gCurUpDowns);
        m_headerView = new WeeklyHorizonHeader(Qt::Horizontal,m_upDownView);
        m_headerView->setModel(m_headerModel);

        m_upDownView->setHorizontalHeader(m_headerView);
        m_headerView->setStretchLastSection(true);
        m_headerView->setHighlightSections(false);
        m_headerView->setVisible(true);
        m_headerView->setSectionResizeMode(QHeaderView::Stretch);

        m_upDownView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_upDownView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        m_upDownView->verticalHeader()->setVisible(false);
        m_upDownView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_upDownView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_upDownView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_upDownView->setSortingEnabled(true);
        m_upDownView->setShowGrid(true);
        m_upDownView->setMouseTracking(true);
        m_upDownView->setFocusPolicy(Qt::NoFocus);
        m_upDownView->setAlternatingRowColors(false);
        m_headerView->setSortIndicator(4,Qt::DescendingOrder);

        m_upDownView->setColumnWidth(0,83);
        m_upDownView->setColumnWidth(1,82);
        m_upDownView->setColumnWidth(2,193);
        m_upDownView->setColumnWidth(3,193);
        m_upDownView->setColumnWidth(4,193);

        ///![3]
        QLabel *titleBar = new QLabel(QStringLiteral("  自选股本周涨跌"));
        titleBar->setFixedHeight(30);
        titleBar->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        titleBar->setObjectName("CurUpDownsTitleBar");
        QVBoxLayout *hbox = new QVBoxLayout;
        hbox->setContentsMargins(0,0,0,0);
        hbox->addWidget(titleBar);
        hbox->addWidget(m_upDownView);
        setLayout(hbox);

        connect(m_upDownView, &QTableView::pressed, [=](const QModelIndex &index){
            QString stockcode = index.data(Weekly_Table_StockCode).toString();
            emit signalRowClicked(stockcode);
        });
    }

    void updateFromContent(const QVariant &var)
    {
        //(?<!\d)\d{6}(?!\d)")
        m_privateData = var;
        OptionalWeekly data = var.value<OptionalWeekly>();
        QVector<STCurUpDowns> lst;
        for(const OptionalWeekly::STDetail &det : data.detail) {
                STCurUpDowns ret;
                ret.stockname = det.stockname;
                ret.stockcode = det.stockcode;
                ret.ups = det.rate_all;
                ret.profit = det.per_income;
                lst << ret;
        }

        m_upDownModel->setValue(QVariant::fromValue(lst));
    }

    void updateForRow()
    {
        updateFromContent(m_privateData);
    }

    void firstSelect()
    {
        //默认第一行选中
        m_upDownView->selectRow(0);
    }
signals:
    void signalRowClicked(const QString stockcode);

private:
    QVariant m_privateData;
    WeeklyChgModel *m_upDownModel;
    QTableView *m_upDownView;
    WeeklyChgDelegate2 *m_itemDeleg;
    WeeklyListModel *m_headerModel;
    WeeklyHorizonHeader *m_headerView;
    WeeklySortFilterProxyModel *m_proxyModel;
};

//盯盘总结
class StealingSummary : public QFrame, public WeeklyWndUpdate
{
    Q_OBJECT
public:
    StealingSummary(QWidget *parent = 0):QFrame(parent)
    {
        ///![1]
        setObjectName("StealingSummary");
        m_itemDeleg = new WeeklyChgDelegate2(StealingSummary_Type);
        m_stealingSumModel = new WeeklyChgModel(StealingSummary_Type);
        m_proxyModel = new WeeklySortFilterProxyModel(StealingSummary_Type);
        m_proxyModel->setSourceModel(m_stealingSumModel);

        m_stealingSumView = new QTableView;
        m_stealingSumView->setModel(m_proxyModel);
        m_stealingSumView->setItemDelegate(m_itemDeleg);
        m_stealingSumView->sortByColumn(1,Qt::AscendingOrder);

        m_headerModel = new WeeklyListModel(StealingSummary_Type);
        m_headerModel->setValue(gStealingSummary);

        m_headerView = new WeeklyHorizonHeader(Qt::Horizontal,m_stealingSumView);
        m_headerView->setModel(m_headerModel);
        m_stealingSumView->setHorizontalHeader(m_headerView);
        m_headerView->setStretchLastSection(true);
        m_headerView->setHighlightSections(true);

        m_stealingSumView->setFocusPolicy(Qt::NoFocus);
        m_stealingSumView->setAlternatingRowColors(false);
        m_stealingSumView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_stealingSumView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        m_stealingSumView->verticalHeader()->setVisible(false);
        m_stealingSumView->setShowGrid(true);
        m_stealingSumView->setMouseTracking(true);
         m_stealingSumView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_stealingSumView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_stealingSumView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_stealingSumView->setWordWrap(true);
        m_stealingSumView->setSortingEnabled(true);
        m_stealingSumView->verticalHeader()->setDefaultSectionSize(43);
        m_stealingSumView->horizontalHeader()->setDefaultSectionSize(100);
        m_headerView->setSortIndicator(1,Qt::AscendingOrder);

        ///![2]
        QLabel *titleBar = new QLabel(QStringLiteral("  自选股本周盯盘总结"));
        titleBar->setFixedHeight(30);
        titleBar->setObjectName("StealingSummaryTitleBar");
        QVBoxLayout *hbox = new QVBoxLayout;
        hbox->setContentsMargins(0,0,0,0);
        hbox->addWidget(titleBar);
        hbox->addWidget(m_stealingSumView);
        setLayout(hbox);

        ///![3]
        connect(m_stealingSumView, &QTableView::pressed, [=](const QModelIndex &index){
            QString stockcode = index.data(Weekly_Table_StockCode).toString();
            emit signalRowClicked(stockcode);
        });
    }

    void updateFromContent(const QVariant &var)
    {
        m_privateData = var;
        OptionalWeekly data = var.value<OptionalWeekly>();
        QVector<STStealingSummary> lst;
        for(const OptionalWeekly::STDetail &det : data.detail) {
                STStealingSummary ret;
                QStringList wa;
                ret.stockcode = det.stockcode;
                ret.stockname = det.stockname;
                ret.ups = det.rate_all;

                if(det.lhb!=0)  wa << QStringLiteral("龙虎榜");
                if(det.fhsz != 0)  wa << QStringLiteral("分红送转");
                if(det.cb != 0) wa << QStringLiteral("财报");
                ret.des = QString(StealingSummary_Template)
                        .arg(ret.stockname)
                        .arg(det.pzyd)
                        .arg(wa.join("、") + QString::number(det.fhsz+det.lhb+det.cb));
                lst << ret;
        }

        m_stealingSumModel->setValue(QVariant::fromValue(lst));
    }

    void updateForRow()
    {
        updateFromContent(m_privateData);
    }

signals:
    void signalRowClicked(const QString stockcode);

private:
    QVariant m_privateData;
    WeeklyListModel *m_headerModel;
    WeeklyHorizonHeader *m_headerView;
    WeeklyChgModel *m_stealingSumModel;
    QTableView *m_stealingSumView;
    WeeklyChgDelegate2 *m_itemDeleg;
    WeeklySortFilterProxyModel *m_proxyModel;
};

#include "basewidget.h"

/**
 * @brief The weeklys class
 */
class weeklys : public BaseWidget
{
    Q_OBJECT

public:
    explicit weeklys(QWidget *parent = 0);
    ~weeklys();

    /**
     * @brief 对象激活
     */
    void afterActive();

    /**
     * @brief 取消激活
     */
    void afterDeactive();

signals:
    void signalRowClicked(const QString &stockcode);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::weeklys *ui;
    QList<WeeklyWndUpdate*> m_wnds;
    InfoReqId m_reqId{-1};

    void startRequest();
    void weeklysCallBack(const InfoRet& ret,const OptionalWeekly &data);
};

#endif // WEEKLYS_H
