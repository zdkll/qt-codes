/********************************************************************
created:2019.1
author: chengmin
purpose:
*********************************************************************/
#ifndef FINANCIALREPORTDELEGATE_H
#define FINANCIALREPORTDELEGATE_H
#include "economicmaster.h"
#include "hzinfodata.h"
/*!
 * \brief 财报发布代理
 */
class EconomicView;
struct FinaReportDelItem
{
    QString stockCode;
    QString url;

    //财报
    QString reportStr;
    QRect reportRc =  QRect();
    //自选
    QString optionStr;
    QRect optionRc =  QRect();
    //查看
    QString showStr;
    QRect showRc =  QRect();

    //整体单个股票区域
    QRect stockRc;
};

class FinancialReportDelegate: public EconomicDelegate
{
    Q_OBJECT
public:
    explicit FinancialReportDelegate(EconomicView* pView,QObject *parent = 0);

    QVector<FinaReportDelItem> calcDrawData(const QVector<FinancialReport::FinancialItem>& items,QRect rc, quint32 time) const;
protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    struct DelReportItem
    {
        QString code;
        QRect report =  QRect();
        QRect show =  QRect();
        QRect option =  QRect();
    };
    QFont m_reportFont;
    QFontMetrics* m_reportfm;
    QFont m_optionFont;
    QFontMetrics* m_optionfm;
    QFont m_showFont;
    QFontMetrics* m_showfm;
    EconomicView* m_pView;
    QPoint m_pt;
    QRect m_currItemRc;
};

#endif // FINANCIALREPORTDELEGATE_H
