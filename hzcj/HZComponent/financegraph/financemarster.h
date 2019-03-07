/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPen>
#include <hzinfocenter.h>
#include "basewidget.h"
#include "finance_export.h"

typedef QVector<double> Q_VECTOR_LIST;

/**
 * @brief The HZFInanceLabel class 营业收入
 */
class HZFInanceLabel : public QLabel
{
    Q_OBJECT
public:
    HZFInanceLabel(QWidget *parent=nullptr);

    /**
     * @brief setFinancialInfo 营业收入数据
     * @param datas
     */
    void setFinancialInfo(const FinanceTaking &datas);

protected:
    void paintEvent(QPaintEvent *e);

private:
    Q_VECTOR_LIST m_growths;
    QMap<QString, Q_VECTOR_LIST> m_quarters;
    QMap<QString, double> m_quarterCnts;//记录总值
    QStringList m_years;
    double m_leMax{-qInf()}, m_leMin{qInf()}, m_rtMax{-qInf()}, m_rtMin{qInf()};
    double mRightMargin{0};

    void drawLine(QPainter *painter);
    void drawRect(QPainter *painter);
    void drawText(QPainter *painter);

    void drawTipText(QPainter *p, const QPen &pen, const QRectF &rt, const QString &text);

    void drawRectPrivate(QPainter *painter, const QPen &pen, const QBrush &brush, const QRectF &rect);
    void drawTextPrivate(QPainter *painter, const QPen &pen, int flags, const QRectF &rect, const QString &text);
};

/**
 * @brief The HZFInanceDesLabel class 营业收入描述
 */
class HZFInanceDesLabel : public QLabel
{
    Q_OBJECT
public:
    HZFInanceDesLabel(bool flags, QWidget *parent=nullptr);

protected:
    void paintEvent(QPaintEvent *);

private:
    bool m_isIncome{false};
};



/**
 * @brief The HZFinanceKeyIndicator class 关键指标列表
 */
class HZFinanceKeyIndicator : public QLabel
{
    Q_OBJECT
public:
    HZFinanceKeyIndicator(QWidget *parent=nullptr);

    /**
     * @brief updateIndicates 关键指标数据
     * @param indicate
     */
    void updateIndicates(const FinanceKeyindex &indicate);

protected:
    void paintEvent(QPaintEvent *);
    void updateIndicates(const QVector<QPair<QString, QString>> &indicates);

private:
    QVector<QPair<QString,QString>> m_indicats;//
    int m_lineH{20}, m_titleH{40};
    QString m_titleText;
};

/**
 * @brief The HZFinanceProfitGraphic class 利润图例
 */
class HZFinanceProfitGraphic : public QLabel
{
    Q_OBJECT
public:
    HZFinanceProfitGraphic(QWidget *parent=nullptr);

    /**
     * @brief updateProfits 利润图例数据
     * @param profits
     */
    void updateProfits(const QVector<FinanceProfit::ProfitChartData> &profits);

protected:
    void paintEvent(QPaintEvent *e);

private:
    double m_rtMax, m_rtMin, m_leMax, m_leMin, m_rtSecMin, mRightMargin{0};
    QVector<double> m_ups, m_profits;
    QStringList m_years;

    void drawLine(QPainter *painter);
    void drawRect(QPainter *painter);
    void drawText(QPainter *painter);
};

/**
 * @brief The HZFinanceProfitText class 利润列表
 */
class HZFinanceProfitText : public QLabel
{
    Q_OBJECT
public:
    HZFinanceProfitText(QWidget *parent=nullptr);

    /**
     * @brief updateProfit 利润列表数据
     * @param profit
     */
    void updateProfit(const FinanceProfit &profit);

protected:
    void paintEvent(QPaintEvent *e);
    void updateProfit(const QVector<QPair<QString, QString>> &indicates);

private:
    QVector<QPair<QString,QString>> m_profits;//
    int m_lineH{20}, m_titleH{40};
    QString m_titleText;
};

/**
 * @brief The HZFinanceBalanceSheetText class 资产负债
 */
class HZFinanceBalanceSheetText : public QLabel
{
    Q_OBJECT
public:
    HZFinanceBalanceSheetText(QWidget *parent=nullptr);
    void updateProfit(const FinanceBalanceSheet &profit);

protected:
    void paintEvent(QPaintEvent *e);
    void updateProfit(const QVector<QPair<QString, QString>> &indicates);

private:
    QVector<QPair<QString,QString>> m_profits;//
    int m_lineH{20}, m_titleH{40};
    QString m_titleText;
};

/**
 * @brief The HZFinanceCashFlowText class 现金流量
 */
class HZFinanceCashFlowText : public QLabel
{
    Q_OBJECT
public:
    HZFinanceCashFlowText(QWidget *parent=nullptr);
    void updateProfit(const FinanceCashFlow &profit);

protected:
    void paintEvent(QPaintEvent *e);
    void updateProfit(const QVector<QPair<QString, QString>> &indicates);

private:
    QVector<QPair<QString,QString>> m_profits;//
    int m_lineH{20}, m_titleH{40};
    QString m_titleText;
};


/**
 * @brief The Widget class 财务总列表
 */

namespace Ui{
    class HZFinanceMarster;
}

class FINANCE_GRAPH_EXPORT HZFinanceMarster : public BaseWidget
{
    Q_OBJECT
public:
    explicit HZFinanceMarster(QWidget *parent = 0);
    ~HZFinanceMarster();

    /**
     * @brief reqFinanceInfo 财务数据请求
     */
    void reqFinanceInfo();

    /**
     * @brief setObj 重写激活和取消激活功能
     * @param obj
     */
    void setObj(const QString& obj);
    void afterActive();
    void afterDeactive();

protected:
    void OnStockCodeChange();
    void paintEvent(QPaintEvent *event);

private:
    Ui::HZFinanceMarster *ui;
    QFrame *m_incomeFrame, *m_profitFrame;
    QLabel *m_title, *m_profitTitle, *m_overView, *m_profitOverView;
    HZFInanceDesLabel *m_descript, *m_profitDescript;
    HZFInanceLabel *m_financeIncomeGraph;
    HZFinanceProfitGraphic *m_profitGraph;

    HZFinanceKeyIndicator *m_keyIndicate;
    HZFinanceProfitText *m_profitList;
    HZFinanceBalanceSheetText *m_balanceSheet;
    HZFinanceCashFlowText *m_cashFlow;
    InfoReqId m_reqId{-1};

    QString m_code{""};

    void loadSkin();
    void financeCallBack(const InfoRet &ret,const FinanceInfo &info);
};

#endif // WIDGET_H
