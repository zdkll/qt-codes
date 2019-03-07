/********************************************************************
created:2018.9
author: juntang.xie
purpose: 买卖五档和涨跌比例窗口类
*********************************************************************/
#ifndef BUYSELLFIVEWIDGET_H
#define BUYSELLFIVEWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "handicapstyledwidget.h"
#pragma execution_character_set("utf-8")

/**
  * @brief 买卖比例窗体
  */
class TradingRatioWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit TradingRatioWidget(QWidget *parent = nullptr);
    ~TradingRatioWidget();

    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    void formatData(const QVector<HZData::AllStock> &vec);

private:
    QVector<HZData::AllStock> m_stockVec;
    HandicapStyledWidget m_styledWidget;
    UINT64 m_allBuyAmount;
    UINT64 m_allSellAmmount;
    QString m_strRise;
    QString m_strDown;
};


class BuySellFiveView;
/**
  * @brief 买卖五档主窗体
  */
class BuySellFiveWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit BuySellFiveWidget(QWidget *parent = nullptr);
    ~BuySellFiveWidget();

    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);
private:
    void createWg();
private:
    TradingRatioWidget *m_ratio;
    BuySellFiveView* m_buyFiveView;
    BuySellFiveView* m_sellFiveView;
};

#endif // BUYSELLFIVEWIDGET_H
