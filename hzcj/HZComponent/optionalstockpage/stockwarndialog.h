/********************************************************************
created: 2018.10.8
author: juntang.xie
purpose: 自选股提醒对话框
*********************************************************************/
#ifndef STOCKWARNDIALOG_H
#define STOCKWARNDIALOG_H
#include "basewidget.h"
#include "frameless_helper.h"
#include "windowcontrolbox.h"
#include "hzcomponent_global.h"
#include "datacontroller.h"
#include "quoteformat.h"
#pragma execution_character_set("utf-8")

/**
  * @brief  上方标题部分
  */
class QLabel;
class TopWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = nullptr);
    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);
private:
    void createWg();
    void updateText();
    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);
    void setObjName();

private:
    QVector<HZData::AllStock> m_stockVec;
    QLabel *m_nameLabel;
    QLabel *m_codeLabel;
    QLabel *m_priceLabel;
    QLabel *m_pArrowLabel;
    QLabel *m_rateLabel;
};
/**
  * @brief  资讯提醒部分
  */
class CenterWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit CenterWidget(QWidget *parent = nullptr);
signals:
    void clicked(bool checked);
private:
    void createWg();
};
/**
  * @brief  股价预警部分
  */
class QDoubleValidator;
class QLineEdit;
class QCheckBox;
class BottomWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit BottomWidget(QWidget *parent = nullptr);

    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);

private slots:
    void onUpTextEdited(const QString &strText);
    void onDownTextEdited(const QString &strText);
    void onRateTextEdited(const QString &strText);

signals:
    void upTextChanged(const QString &strText);
    void downTextChanged(const QString &strText);
    void rateTextChanged(const QString &strText);
private:
    void createWg();
    void setEditRange();

private:
    QVector<HZData::AllStock> m_stockVec;
    QLineEdit *m_upLineEdit;
    QLineEdit *m_downLineEdit;
    QLineEdit *m_rateLineEdit;
    QDoubleValidator *m_upValidator;
    QDoubleValidator *m_downValidator;
    QLabel *m_pUpLabel;
    QLabel *m_pDownLabel;
    QCheckBox *m_pUpCheck;
    QCheckBox *m_pDownCheck;
    QCheckBox *m_pRateCheck;
};
/**
  * @brief  底部按钮部分
  */
class ButtonWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit ButtonWidget(QWidget *parent = nullptr);

signals:
    void click();
private:
    void createWg();
};
/**
  * @brief  自选股提醒窗体
  */
class HZCOMPONENTSHARED_EXPORT StockWarnDialog:public BaseDialog
{
    Q_OBJECT
public:
    explicit StockWarnDialog(QWidget *parent = nullptr);

    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);

private:
    void createWg();
    QWidget *createTitle();
    QWidget *createBody();
    void buildConnect();

private:
    FramelessHelper *m_pHelper;
    WindowControlBox *m_controlBox;
    TopWidget *m_pTopWidget;
    CenterWidget *m_pCenterWidget;
    BottomWidget *m_pBottomWidget;
    ButtonWidget *m_pButtonWidget;
};

#endif // STOCKWARNDIALOG_H
