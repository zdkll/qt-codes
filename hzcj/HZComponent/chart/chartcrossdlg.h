#ifndef CHARTCROSSDLG_H
#define CHARTCROSSDLG_H
/********************************************************************
created:
author: min.cheng
purpose: 十字信息小窗
*********************************************************************/
#include <QStackedWidget>
#include <QMap>
#include "basewidget.h"
#include "crossinfodlg.h"
#include "chartdef.h"
#include "datamodeldef.h"
#include "hzcomponent_global.h"
#include "quoteformat.h"

class QLabel;
#define CROSS_DLG_WIDTH 75
#define CROSS_KLINE_DLG_HEIGHT 360
#define CROSS_MIN_DLG_HEIGHT 260

/**
 * @brief 信息小窗内部面板，用于内部元素创建和布局
 */
class CrossPanel : public BaseWidget
{
    Q_OBJECT
public:
    CrossPanel(QWidget* parent=nullptr);

    //数据更新处理
    void updateInfo(const HZData::HZKline &crossTipInfo);

    //负责内部显示项目的格式
    virtual void convertNameItems(QVector<HzQuoteFormat::QuoteItem> &nameList){}

    virtual void convertValueItems(QVector<HzQuoteFormat::QuoteItem>& valList){}

    //获取外部窗口高度
    virtual void getDlgWidthAndHeight(int& nWidth,int& nHeight){}
protected:
    //创建面板
    void createWg();
protected:
    QVector<QLabel*> m_labVec;
    HZData::HZKline m_tipInfo;
};


/**
 * @brief 信息小窗内部K线面板
 */
class CrossPanelKline : public CrossPanel
{
    Q_OBJECT
public:
    CrossPanelKline(QWidget* parent=nullptr);

    void convertNameItems(QVector<HzQuoteFormat::QuoteItem> &nameList);

    void convertValueItems(QVector<HzQuoteFormat::QuoteItem>& valList);

    void getDlgWidthAndHeight(int& nWidth,int& nHeight)
    {
        nWidth = CROSS_DLG_WIDTH;
        nHeight = CROSS_KLINE_DLG_HEIGHT;
    }
};

/**
 * @brief 信息小窗内部分时面板
 */
class CrossPanelMin : public CrossPanel
{
    Q_OBJECT
public:
    CrossPanelMin(QWidget* parent=nullptr);

    void convertNameItems(QVector<HzQuoteFormat::QuoteItem> &nameList);

    void convertValueItems(QVector<HzQuoteFormat::QuoteItem>& valList);

    void getDlgWidthAndHeight(int& nWidth,int& nHeight)
    {
        nWidth = CROSS_DLG_WIDTH;
        nHeight = CROSS_MIN_DLG_HEIGHT;
    }
};

enum PanelType{
    KlinePanel = 0,
    MinlinePanel = 1
};

/**
 * @brief 十字小窗浮动的窗口
 */
class ChartCrossDlg : public BaseWidget
{
    Q_OBJECT
public:
    ChartCrossDlg(CrossPanel* pPanel,QWidget *parent = 0);
    ~ChartCrossDlg(){}
public slots:
    //设置当前显示的最新数据
    void setTipData(const HZData::HZKline &crossTipInfo);
private:
    CrossPanel* m_pPanel;
};

#endif // CHARTCROSSDLG_H
