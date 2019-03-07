/********************************************************************
created:2018.10.17
author: juntang.xie
purpose: 龙虎榜涨跌组件
*********************************************************************/
#ifndef LHRISEFALLTRADIWIDGET_H
#define LHRISEFALLTRADIWIDGET_H
#include "basewidget.h"
#include "datamodeldefs/stockinfodef.h"
#include "updownwidget.h"
#include "hzinfocenter.h"

class LhRiseFallTradiWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit LhRiseFallTradiWidget(QWidget *parent = nullptr);

    /**
     * @brief 更新数据接口
     * @param[in] stock 传入的股票数据
     * @retval
     */
    void updateData(const HZData::AllStock &stock);

    //更新龙虎榜数据
    void updateLhData(const StockLhbDetialListItem &data);

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    INT64 m_nValues[2];
    INT64 m_nTotal;
    float m_upRate;
    float m_downRate;
    ColorWidget m_colorWidget;
    QString m_strUpRate;
    QString m_strDownRate;
};

#endif // LHRISEFALLTRADIWIDGET_H
