/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HZFINANCESTYLEINFO_H
#define HZFINANCESTYLEINFO_H
#include <QVector>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QFont>

class HZFinanceStyleInfo
{
public:
    static HZFinanceStyleInfo *instance();

    /**
     * @brief getQuarterPens 获取季度画笔和画刷
     * @return
     * @details 季度值为四，链表索引从小到大
     */
    QVector<QPen> quarterPens();
    QVector<QBrush> quarterBrushs();
    QPen textPen() const;
    QPen xTicksPen() const;
    QPen yTicksPen() const;
    QPen baseLinePen() const;
    QPen ellipsePen() const;
    QPen linePen() const;

    QPen incomeEllipsePen() const;
    QBrush incomeEllipseBrush() const;



    /**
     * @brief profitTextPen !profit sytle
     * @return
     */
    QPen profitTextPen() const;

    QPen profitEllipsePen() const;
    QBrush profitEllipseBrush() const;

    QPen profitLinePen() const;
    QPen profitRectPen() const;
    QBrush profitRectBrush() const;

    QFont mYAxisTxFont;
    QFont mXAxisTxFont;
    QFont mTipTxFont;
    QFont mNumberTxFont;
    QFont mTextFont;

private:
    HZFinanceStyleInfo();
    QVector<QPen> gPens;
    QVector<QBrush> gBrushs;
};

#define gHZFinanceStyle HZFinanceStyleInfo::instance()
#endif // HZFINANCESTYLEINFO_H
