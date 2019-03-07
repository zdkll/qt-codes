/********************************************************************
created:2018.10.22
author: juntang.xie
purpose: 指数盘口组件
*********************************************************************/
#ifndef INDEXPARTSWIDGET_H
#define INDEXPARTSWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"

#pragma execution_character_set("utf-8")

class QLabel;
/**
  * @brief 指数盘口行情组件
  */
class IndexQuoteWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit IndexQuoteWidget(QWidget *parent = nullptr);

    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const QVector<HZData::AllStock> &vec);
private:
    void createWg();
    void updateText();
    void setObjName();
    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item, QLabel *label);
private:
    QVector<HZData::AllStock> m_stockVec;
    QLabel *m_volumeLabel;  //成交量
    QLabel *m_highestLabel; //最高
    QLabel *m_openLabel;    //开盘
    QLabel *m_turnoverLabel;    //成交额
    QLabel *m_lowestLabel;      //最低
    QLabel *m_preCloseLabel;    //昨收
};

class RiseFallTradioWidget;
/**
  * @brief 涨平跌组件
  */
class IndexRiseFallWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit IndexRiseFallWidget(QWidget *parent = nullptr);
    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const QVector<HZData::AllStock> &vec);
private:
    void createWg();
    void updateText();
    void updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel*label);
    void setObjName();
private:
    QVector<HZData::AllStock> m_stockVec;
    QLabel *m_pRiseLabel;
    QLabel *m_pFallLabel;
    QLabel *m_pFlatLabel;
    RiseFallTradioWidget *m_pTradioWidget;
};

/**
  * @brief 指数详情行情组件
  */
class IndexDetailQuoteWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit IndexDetailQuoteWidget(QWidget *parent = nullptr);

    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const QVector<HZData::AllStock> &vec);
private:
    void createWg();
    void updateText();
    void setObjName();
    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item, QLabel *label);
private:
    QVector<HZData::AllStock> m_stockVec;
    QLabel *m_openLabel;    //今开
    QLabel *m_highestLabel; //最高
    QLabel *m_upLimitLabel; //涨停
    QLabel *m_preCloseLabel;    //昨收
    QLabel *m_lowestLabel;      //最低
    QLabel *m_downLimitLabel; //跌停
    QLabel *m_volumeLabel;  //总手
    QLabel *m_turnoverLabel;    //成交额

};

#endif // INDEXPARTSWIDGET_H
