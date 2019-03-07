/********************************************************************
created:2018.11.9
author: juntang.xie
purpose:派生自BaseWidget   龙虎榜顶部窗体
*********************************************************************/
#ifndef LHTOPWIDGET_H
#define LHTOPWIDGET_H
#include "basewidget.h"
#include "hzinfocenter.h"
#include "quoteformat.h"

#include "calandarbar.h"

/**
  * @brief 龙虎榜顶部窗体
  */
class HZCalanderCtrl;
class CustomCldBtn;
class NetBuyWidget;
class QLabel;
class QMenu;
class QWidgetAction;
class ActivityLabel;
class DrawLineTextPanel;

class LhTopWidget:public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor m_titleTextClr READ getTitleTextClr WRITE setTitleTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_titleNumClr READ getTitleNumClr WRITE setTitleNumClr DESIGNABLE true)
public:
    QColor getTitleTextClr() const{return m_titleTextClr;}
    void setTitleTextClr(QColor color){m_titleTextClr=color;}
    QColor m_titleTextClr = QColor("#cad4dc");

    QColor getTitleNumClr() const{return m_titleNumClr;}
    void setTitleNumClr(QColor color){m_titleNumClr=color;}
    QColor m_titleNumClr = QColor("#ffa200");

    explicit LhTopWidget(QWidget *parent = nullptr);

    //激活
    virtual void afterActive();
    //取消激活
    virtual void afterDeactive();
signals:
    void signalTimeSelectedChanged(qint64 time);

private:
    void createWg();
    QWidget* createNavigateBar();
    QWidget* createTitle();
    QWidget* createBody();
    QWidget* createCalandarBar();
    void reqStockLhbDatas();
    void reqStockLhbDateList();
    void cancle();
    void onStockLhbDatasCallBack(const InfoRet& ret,const StockLhbDatas& datas);
    void onStockLhbDateListCall(const InfoRet& ret,const StockLhbDateList& datas);
    void updateText();

private:
    qint64 m_listReqId;
    qint64 m_timeListId;
    QVector<NetBuyWidget *> m_netBuyVec;
    INT64 m_dateTime;
    StockLhbDatas m_stocklhbDatas;
    StockLhbDateList m_stockLhbDateList;
    DrawLineTextPanel* m_naviTextWnd{nullptr};
    QList<double> m_net_buylists;
    CustomCalandarBtnMarster *mCalandarBtn;
};

/**
  * @brief 净买入组件
  */
class NetBuyWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit NetBuyWidget(QWidget* parent = nullptr);
    //更新数据
    void updateData(const StockLhbTypeItem &Item);
private:
    void createWg();
    void updateText();
    void setObjName();
    void updateStockField(const double &in, QLabel *label);
private:
    ActivityLabel *m_nameLabel;
    QLabel *m_rateLabel;
    QLabel *m_netBuyLabel;
    QLabel *m_amountLabel;

    StockLhbTypeItem m_item;
};

#endif // LHTOPWIDGET_H
