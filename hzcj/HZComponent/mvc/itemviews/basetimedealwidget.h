/********************************************************************
created:2018/6/25
author: dk.zhang
purpose:分时成交显示窗口基类,保存动态样式属性值，
*********************************************************************/
#ifndef BASETIMEDEALWIDGET_H
#define BASETIMEDEALWIDGET_H

#include "basewidget.h"
#include "datacontroller.h"
#include "hzcomponent_global.h"

#include "timedealtableview.h"
#include "viewstyleproperties.h"

class TimeDealTitleWg;

class TimeDealModel;
class QHBoxLayout;
class QVBoxLayout;
class QScrollBar;

struct ReqRecord{
    ReqRecord():reqId(INVALID_REQID){}
    ReqId reqId;
    int index;   //-1表示最新，其他表示可寻址下标
    int count;
    bool is_desc = true;
};


//@brief 分时成交窗口-基类
//实现成交列表和垂直滚动条并列布局
class   BaseTimeDealWidget : public BaseWidget, public  TimeDealProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor colTimeColor READ colTimeColor WRITE setColTimeColor DESIGNABLE true)
    Q_PROPERTY(QColor colPriceColor READ colPriceColor WRITE setColPriceColor DESIGNABLE true)

    Q_PROPERTY(QColor colVolUpColor READ colVolUpColor WRITE setColVolUpColor DESIGNABLE true)
    Q_PROPERTY(QColor colVolDownColor READ colVolDownColor WRITE setColVolDownColor DESIGNABLE true)

    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily DESIGNABLE true )
    Q_PROPERTY(int         fontPointSize READ fontPointSize WRITE setFontPointSize DESIGNABLE true)

    Q_PROPERTY(QColor gridLineColor READ gridLineColor WRITE setGridLineColor DESIGNABLE true)

    Q_PROPERTY(QPixmap upArrowPix READ upArrowPix WRITE setUpArrowPix DESIGNABLE true)
    Q_PROPERTY(QPixmap downArrowPix READ downArrowPix WRITE setDownArrowPix DESIGNABLE true)

public:
    BaseTimeDealWidget(QWidget *parent =0,const QString &instName = QString());
    ~BaseTimeDealWidget();

    // @brief 设置整个窗口数据,内部按照表顺序和显示大小依次设置
    void setData(const QVector<HZData::HZTick> &ticks);

    //brief 设置整个窗口数据,内部按照表顺序和显示大小依次设置
    void clearData();
    // @brief 设置股票信息，股票码改变后需要重新拉取逐笔数据刷新表格
    void setStockCode(const string &stockcode,const QString &stockName = QString());

    // @brief 激活和非激活状态
    virtual void afterActive();
    virtual void afterDeactive();

    // @brief 初始化设置样式属性
    virtual void initStylePropoerty(){}

    // @brief 设置垂直滚动条是否显示
    void setShowVerticalScrollBar(const bool &show);

    // @brief 设置标题窗口
    void setTitleWg(TimeDealTitleWg *wg);

    // @brief 设置标题是否显示
    void setShowTitle(const bool &show);

    // @brief 获取某个表格
    QVector<TimeDealTableView *> tableViews(){return m_tableViews;}
    TimeDealTableView *tableView(const int &index){return m_tableViews[index];}

protected:
    void addTableView(TimeDealTableView *tableView);
    //最大可显示范围
    int       currentMaxRowCount() const;
    //实际显示范围
    int       currentRowCount() const;

protected:
    virtual void init() = 0;
    virtual void startSub() = 0;
    virtual void cancelSub();

    void showEvent(QShowEvent *e);

protected:
    bool             isInitedStyle;
    QScrollBar *m_scrollBar;
    QVector<TimeDealTableView *> m_tableViews;
    std::string         m_stockCode;
    ReqId               m_subReqId = INVALID_REQID;
    ReqRecord       m_reqRecrd;

    //数据
    HZData::Zhubi                m_zhubi;
    //是否拉取数据初始化，切换合约是否为false
    bool                m_isInitialed;
    bool                m_isActived;

private:
    void initWg();

private:
    QHBoxLayout       *m_tableViewLayout;
    TimeDealTitleWg  *m_titleWg;
    QVBoxLayout       *mainLayout;
};


class QLabel;
///标题栏
class  TimeDealTitleWg: public BaseWidget
{
    Q_OBJECT
public:
    TimeDealTitleWg(QWidget *parent = 0);
    void setStockName(const QString &stockName);

private:
    QString  m_stockName;
    QLabel *m_titleLabel;
};

#endif // BASETIMEDEALWIDGET_H
