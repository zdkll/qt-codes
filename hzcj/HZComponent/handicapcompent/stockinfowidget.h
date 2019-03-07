/********************************************************************
created:2018.9
author: juntang.xie
purpose: 盘口行情窗口类
*********************************************************************/
#ifndef STOCKINFOWIDGET_H
#define STOCKINFOWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"
#include "stockwarndialog.h"

#pragma execution_character_set("utf-8")

class QLabel;
class QPushButton;
/**
  * @brief 盘口上方标题部分组件
  */
class HandicapTitleWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit HandicapTitleWidget(QWidget *parent = nullptr);
    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);
    //设置自选股股票代码
    void setSelfStock(const QString &strCode);

protected:
    void OnStockCodeChange();
private:
    void createWg();
    void updateText();
    void setObjName();

    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);

private slots:
    void onAddBtnClicked();
    void onWarningBtnClicked();

private:
    QLabel *m_nameLabel;
    QLabel *m_codeLabel;
    QLabel *m_priceLabel;
    QLabel *m_pArrowLabel;
    QLabel *m_rateLabel;

    QPushButton *m_pAddBtn;
//    QPushButton *m_pWarnBtn;
//    QPushButton *m_pSetBtn;
    StockWarnDialog m_warndlg;
    QVector<HZData::AllStock> m_stockVec;
};

/**
  * @brief 盘口上方中间部分组件
  */
class HandicapCentralWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit HandicapCentralWidget(QWidget *parent = nullptr);
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
    QLabel *m_upLimitLabel;     //涨停
    QLabel *m_avePriceLabel;    //均价
    QLabel *m_outVolumeLabel;   //外盘
    QLabel *m_downLimitLabel;   //跌停
    QLabel *m_volumeRatioLabel; //量比
    QLabel *m_inVolumeLabel;    //内盘
    QLabel *m_amplitudeLabel;   //振幅
    QLabel *m_turnoverRateLabel;    //换手

    QVector<HZData::AllStock> m_stockVec;
};

/**
  * @brief 盘口上方底部组件
  */
class HandicapBottomWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit HandicapBottomWidget(QWidget *parent = nullptr);
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
    QLabel *m_incomeLabel;    //每股收益
    QLabel *m_proDyLabel;    //市盈率
    QLabel *m_netAssetsLabel;    //每股净资产
    QLabel *m_dyGuMoneyLabel;    //流通股本
    QLabel *m_dyValueLabel;    //流通市值
    QLabel *m_valueRateLabel;    //市净率
    QLabel *m_allGuMoneyLabel;    //总股本
    QLabel *m_allValueLabel;    //总市值
    QLabel *m_roeLabel;    //ROE

    QVector<HZData::AllStock> m_stockVec;
};



/**
  * @brief 相似K线上方标题部分组件
  */
class SimilarKlineHandicapTitleWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit SimilarKlineHandicapTitleWidget(QWidget *parent = nullptr);
    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);
    //设置自选股股票代码
    void setSelfStock(const QString &strCode);

protected:
    void OnStockCodeChange();
private:
    void createWg();
    void updateText();
    void setObjName();

    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);

private slots:
    void onAddBtnClicked();
    void onWarningBtnClicked();

private:
    QLabel *m_nameLabel;
    QLabel *m_codeLabel;
    QLabel *m_priceLabel;
    QLabel *m_pArrowLabel;
    QLabel *m_rateLabel;
    QLabel *m_indiLabel;

    QPushButton *m_pAddBtn;
//    QPushButton *m_pWarnBtn;
//    QPushButton *m_pSetBtn;
    StockWarnDialog m_warndlg;
    QVector<HZData::AllStock> m_stockVec;
};


#endif // STOCKINFOWIDGET_H
