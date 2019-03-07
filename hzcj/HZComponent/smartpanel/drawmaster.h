/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef DRAWMASTER_H
#define DRAWMASTER_H

#include <QWidget>
#include <QtWidgets>
#include <QDebug>
#include "basewidget.h"
#include "smartpaneldef.h"

#include <hzinfodata.h>
#include <hzinfocenter.h>
#include <datamodeldefs/stockinfodef.h>
#include <datacontroller.h>

/**
 * @brief The DrawStyle class
 */
class DrawStyle
{
public:
    static DrawStyle* instance();

public:
    QFont m_timeFont, m_codeAndTipFont, m_changedAndChgFont, m_contentFont;

    QBrush m_hoverBrush{QColor("#253a54")},
        m_hormalBrush{QColor("#222222")},
        m_tipsBrush{Qt::yellow},
        m_toDayBrush{QColor("#191919")},
        m_yeasterDayBrush{QColor("#191919")},
        m_titleTextBrush{QColor("#222222")},
        m_ellipseBrush{QColor("#0e0e0e")};//hover

    QPen m_hoverPen{QColor("#509BFF")},
        m_hormalPen{QColor("#414141")},
        m_tipsPen{Qt::blue},
        m_splitLine{Qt::black},
        m_linePen{QColor("#242424")},
        m_toDayPen{QColor("#cad4dc")},
        m_yeasterDayPen{QColor("#4f9bff")},
        m_toDayBorderPen{QPen(QColor("#414141"),2)},
        m_yeasterBorderPen{QPen(QColor("#4f9bff"),2)},
        m_timeTextPen{QColor("#cad4dc")},
        m_changedDownPen{QColor("#0fc351")},
        m_changedUpPen{QColor("#e62727")},
        m_noUpDownBrPen{QColor("#797979")},
        m_contentPen{QColor("#797979")},
        m_titleTextPen{QColor("#cad4dc")},
        m_ellipsePen{QPen(QColor("#adadad"),2)},
        m_ellipseHoverPen{QPen(QColor("#4f9bff"),2)};



protected:
        DrawStyle()
        {
            //时间字体
            m_timeFont = QFont("Arial");
            m_timeFont.setPixelSize(12);

            //股票代码和前30分文本提示字体
            m_codeAndTipFont = QFont("SimSun");
            m_codeAndTipFont.setPixelSize(14);

            //涨跌与涨跌幅
            m_changedAndChgFont = QFont("Arial");
            m_codeAndTipFont.setPixelSize(14);

            //内容描述字体
            m_contentFont = QFont("SimSun");
            m_contentFont.setPixelSize(12);
        }
};

#define gDrawStyle DrawStyle::instance()

/**
 * @brief The PageScrollBar class
 */
class PageScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    PageScrollBar(QWidget *parent=nullptr)
        :QScrollBar(Qt::Horizontal, parent)
    {
        m_min = 0;
        m_max = 100;
    }

    void setRanged(int min, int max)
    {
        m_max = max;
        m_min = min;
        int step = (m_max-m_min)/500;
        setRange(min,max);
        //setStep(step);
    }

    void setStep(int step)
    {
        m_step = step;
    }

private:
    int m_step;
    int m_max, m_min;
};

/**
 * @brief The DrawPanel class
 */
class DrawPanel : public QFrame
{
    Q_OBJECT
public:
    DrawPanel(int pageId=0, QWidget *parent=nullptr)
        :QFrame(parent)
        ,m_pageId(pageId)
    {
        setMouseTracking(true);
    }

    ///
    /// \brief setData 页面数据
    /// \param data
    ///
    void setData(const QVariantMap &data);

    ///
    /// \brief setPanelItems  页面数据
    /// \param items
    ///
    void setPanelItems(const QVector<PanelItem> &items);

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void signalStockCodeClicked(const QString &stockcode);
    void signalItemPressed(const QString &value);
    void signalDataChanged(int count);
    void signalAddCustomStock();

public slots:
    void slotScrollChanged(int value);

private:
    QVector<PanelItem> m_panalItems;
    QHash<int,QRect> m_itemIndexs;
    int m_pageCnt, m_scrollValue, m_scrollMaxValue,m_dataFlags{0};
    QPoint m_pressPos, m_hoverPos;
    QVector<QString> m_tmsNode;
    int m_pageId;

    QRect m_addRect;
    bool m_isPressed{false};
    QString m_tipText, m_tipUrl;

    /**
     * @brief FormatString 格式化字符串
     * @param val
     * @param flags
     * @return
     */
    QString FormatString(double val, bool flags = false);

    /**
     * @brief drawGraphFromIndex
     * @param painter
     * @param rect
     * @param index
     */
    void drawGraphFromIndex(QPainter *painter, const QRect &rect, int index);

    /**
     * @brief drawTips
     * @param painter
     * @param rect
     * @param index
     */
    void drawTips(QPainter *painter, const QRect &rect, int index);

    /**
     * @brief getItem
     * @param index
     * @return
     */
    PanelItem getItem(int index);

    /**
     * @brief findItem
     * @param point
     * @return
     */
    int findItem(const QPoint &point);

    /**
     * @brief companyOptionEventPageDataDisplay 公司大事页面数据显示
     * @param code 请求状态
     * @param data
     */
    void companyOptionEventPageDataDisplay(int code, const CompanyOptionEvents &data);

    /**
     * @brief midAbnormalInfoPageDataDisplay 盘中异动页面数据显示
     * @param code 请求状态
     * @param data
     */
    void midAbnormalInfoPageDataDisplay(int code, const HZData::MidAbnormalInfo &data);
};


namespace Ui {
class DrawMaster;
}

/**
 * @brief The DrawMaster class 对外包装类
 */
class DrawMaster : public BaseWidget
{
    Q_OBJECT

public:
    explicit DrawMaster(int pageId, QWidget *parent = 0);
    ~DrawMaster();

    void updateScroll();

    /**
     * @brief 对象激活
     */
    void afterActive();

    /**
     * @brief 取消激活
     */
    void afterDeactive();

signals:
    void signalStockCodeClicked(const QString &stockcode);

public slots:
    /**
     * @brief slotPanelDataChanged
     * @param count
     */
    void slotPanelDataChanged(int count);

    /**
     * @brief slotZXStockClicked 自选股列表项被点击
     * @param stockcode
     */
    void slotZXStockClicked(const QString &stockcode);

protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    Ui::DrawMaster *ui;
    DrawPanel *m_drawPanel{nullptr};
    QScrollBar *m_scrollBar{nullptr};
    int m_itemCnt, m_pageId{0};
    InfoReqId m_reqId{-1};
    int m_curPage{-1}, m_reqMax{1000};//盘中异动当前页，-1表示最新页
    uint m_abnormalReqId;
    CompanyOptionEvents m_srcData;
    HZData::MidAbnormalInfo m_normalInfo;
    QString m_activeStock{""};

    void startReq(const QString &stockcode="");

    /**
     * @brief companyOptionEventCallBack 公司大事回调
     */
    void companyOptionEventCallBack(const InfoRet&,const CompanyOptionEvents&);

    /**
     * @brief companyOptionEventCallBack 盘中异动回调
     */
    void panelAbnormalCallBack(ReqId,const HZData::MidAbnormalInfo&,ErrCode);

    /**
     * @brief subMidAbnormal 盘中异动实时数据推送
     */
    void subMidAbnormal(const std::string &stockcode);

    /**
     * @brief subAbnormalCallBack 盘中异动订阅推送处理
     */
    void subAbnormalCallBack(ReqId ret,const HZData::MidAbnormalInfo& data,ErrCode err);

    /**
     * @brief startReqSnapShot 行情数据请求
     */
    void startReqSnapShot(const CompanyOptionEvents &datas);
    void reqSnapShot(const QList<QString> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num,  ErrCode err);
};

#endif // DRAWMASTER_H
