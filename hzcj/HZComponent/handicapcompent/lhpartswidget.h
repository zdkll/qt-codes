/********************************************************************
created:2018.10.31
author: juntang.xie
purpose: 龙虎榜盘口组件
*********************************************************************/
#ifndef LHPARTSWIDGET_H
#define LHPARTSWIDGET_H
#include "basewidget.h"
#include "quoteformat.h"
#include "datacontroller.h"
#include "hzinfocenter.h"

#pragma execution_character_set("utf-8")

class QLabel;
class QPushButton;
class QListButton;
class QMenu;
class QListWidget;
/**
  * @brief 龙虎榜盘口标题组件
  */
class LhTitieWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit LhTitieWidget(QWidget *parent = nullptr);
    ~LhTitieWidget();

    /**
     * @brief 更新数据
     * @param[in] stock 收到的个股数据
     * @retval
     */
    void updateData(const  HZData::AllStock &stock);

    //更新龙虎榜数据
    void updateLhData(const StockLhbDetialList &data);


protected:
    virtual void OnStockCodeChange();
private:
    void createWg();
    void updateText();
    void setObjName();
    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item, QLabel *pLabel);
    void setSelfStock();

    void setMenu();
    void initListWidget();

    void removeListWidget();

signals:
    void currentIndex(int index);
private slots:
    void onBtnClicked();
private:
    QLabel *m_pNameLabel;
    QLabel *m_pCodeLabel;
    QLabel *m_pPriceLabel;
    QLabel *m_pArrowLabel;
    QLabel *m_pRateLabel;
    QPushButton *m_selfStockBtn;

    HZData::AllStock m_allStock;
    QListButton *m_listBtn;
    QMenu *m_menu;
    QListWidget *m_listWidget;
    StockLhbDetialList m_lhDetailList;
    QMap<QString, int> m_dateInexMap;
};

class LhRiseFallTradiWidget;
class QHBoxLayout;
/**
  * @brief 龙虎榜涨跌组件
  */
class LhRiseFallWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit LhRiseFallWidget(QWidget *parent = nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
    //更新龙虎榜数据
    void updateLhData(const StockLhbDetialListItem &data);

private:
    void createWg();
    QHBoxLayout *createLayout();
    void setObjName();
    //合约变化
    void OnStockCodeChange();

    void startSub();

    void onLhbDataArrived(const InfoRet& infoRet, const StockLhbDetialList &data);

    //更新文本颜色
    void updataStockField(const HzQuoteFormat::QuoteItem& item, QLabel* pLabel);
    void updateText();
private:
    QLabel* m_titleLabel;       //标题
    QLabel* m_netBuyLabel;      //净买入
    QLabel* m_allBuyLabel;      //全部买入
    QLabel* m_allSellLabel;     //全部卖出
    LhRiseFallTradiWidget *m_lhRiseFall;

    StockLhbDetialListItem m_lhDetailListItem;
};

class LhBuySellView;
//龙虎榜下方控件
class LhbBottomWidget:public BaseWidget
{
   Q_OBJECT
public:
    explicit LhbBottomWidget(QWidget *parent = nullptr);


    //更新龙虎榜数据
    void updateLhData(const StockLhbDetialListItem &data);
signals:
    void clickRow(const QString &strID);

protected slots:
    void slotsBuySellViewClickRow(const QString &strID);

private:
    void createWg();


private:
    LhRiseFallWidget *m_riseFallWidget;
    LhBuySellView *m_lhBuyView;
    LhBuySellView *m_lhSellView;

};


#endif // LHPARTSWIDGET_H
