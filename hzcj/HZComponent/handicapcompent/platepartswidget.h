/********************************************************************
created:2018.10.22
author: juntang.xie
purpose: 板块盘口组件
*********************************************************************/
#ifndef PLATEPARTSWIDGET_H
#define PLATEPARTSWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"

#pragma execution_character_set("utf-8")

class QLabel;
class PlateDetailQuoteWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit PlateDetailQuoteWidget(QWidget *parent = nullptr);
    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);

private:
    void createWg();
    void updateText();
    void setObjName();

    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);

private:
    QLabel *m_openLabel;    //今开
    QLabel *m_highestLabel; //最高
    QLabel *m_volumeLabel;  //成交量
    QLabel *m_preCloseLabel;    //昨收
    QLabel *m_lowestLabel;      //最低
    QLabel *m_turnoverLabel;    //成交额
    QLabel *m_turnoverRateLabel;    //换手
    QLabel *m_inVolumeLabel;    //内盘
    QLabel *m_outVolumeLabel;   //外盘
    QLabel *m_allGuMoneyLabel;    //总股本
    QLabel *m_allValueLabel;    //总市值
    QLabel *m_dyGuMoneyLabel;    //流通股本

    QVector<HZData::AllStock> m_stockVec;
};

#endif // PLATEPARTSWIDGET_H
