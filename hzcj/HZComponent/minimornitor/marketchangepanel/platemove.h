/********************************************************************
created: 2018,9,13
author:  xuzhehu
purpose: 板块异动组件
*********************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPaintEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <basewidget.h>
#include <datactrldef.h>
#include <QGraphicsSceneHoverEvent>
#include "platemove_export.h"

namespace Ui {
class HZPlateMove;
}

/**
 * @brief The STVLineData struct 时间线结构
 * @memberof pos draw position data
 * @memberof draw Time string
 */
struct STVLineData{
  QPoint pos;
  QString time;
  bool operator ==(const STVLineData &data)const {
      return data.pos == this->pos && data.time == this->time;
  }
};

/**
 * @brief the tagPMBlockItem struct 每个时间点位置项
 * @memberof stock_code     股票码
 * @memberof stock_name     股票名称
 * @memberof is_marster     是否为主节点 （主节点与非主节点的UI布局不一样）
 * @memberof update_time    成交时间
 * @memberof rate           涨幅
 * @memberof up_down_price  涨跌额
 */
typedef struct tagPMBlockItem {
    QString stock_code;
    QString stock_name;
    bool is_marster;
    UINT32 update_time = INVALID_UINT32 ;
    double rate = INVALID_DOUBLE;
    double up_down_price= INVALID_DOUBLE;
    bool operator ==(const tagPMBlockItem& item) const
    {
        return this->stock_code == item.stock_code;
    }
}STPMBlockItem;

/**
 * @brief the tagMarsterItem struct 节点列表
 */
typedef struct tagMarsterItem
{
    UINT32 time;
    UINT32 count;
    QList<STPMBlockItem> items;
}STMarsterItem;




/**
 * @brief The HZVLineLabel class 时间线
 */
class HZVLineLabel : public QLabel
{
    Q_OBJECT
public:
    HZVLineLabel(QWidget *parent=nullptr);

    void addData(const STVLineData &data);
    void clear();

protected:
    void paintEvent(QPaintEvent *);

private:
    QVector<STVLineData> m_datas;
};

/**
 * @brief The HZStockLabel class 股票名称显示和用户点击跳转类
 */
class HZStockLabel : public QLabel,public HZObject
{
    Q_OBJECT
public:
    HZStockLabel(const QString &stockname,const QString &stockcode,QWidget *parent=nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

    QString mStockCode;
};

/**
 * @brief The HZLabelDisplayItem class 数值显示标签
 */
class HZLabelDisplayItem : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus)
public:
    HZLabelDisplayItem(QWidget *parent=nullptr)
        :QLabel(parent)
    {
        setObjectName("HZLabelDisplayItem");
    }

    HZLabelDisplayItem(qreal var, const QString &ext="", QWidget *parent=nullptr)
        :QLabel(parent)
    {
        setObjectName("HZLabelDisplayItem");
        setStatus(var>0?"red":var<0?"blue":"white");
        setText(QString("%1%2").arg(QString::number(var,'f',2)).arg(ext));
    }

    void updateDate(qreal var, const QString &ext="")
    {
        setStatus(var>0?"red":var<0?"blue":"white");
        setText(QString("%1%2").arg(QString::number(var,'f',2)).arg(ext));
    }

    /**
     * @brief status 当前状态
     * @return
     */
    QString status() { return m_status;}

    /**
     * @brief setStatus 设置当前状态
     * @param sta
     */
    void setStatus(QString sta) { m_status = sta;}

private:
    QString m_status;
};


/**
 * @brief The HZItemButton class 每一个条目封装类
 */
class HZItemButton : public BaseWidget//, public HZObject
{
    Q_OBJECT
    Q_PROPERTY(bool isMarster READ isMarster WRITE setIsMarster)

public:
    HZItemButton(const STPMBlockItem &content, bool isMarster=false, QWidget *parent=nullptr);
    ~HZItemButton();

    /**
     * @brief setContent 设置内容
     * @param content
     */
    void setContent(const STPMBlockItem &content);

    /**
     * @brief itemTime 项时间值
     * @return
     */
    QString itemTime() const;

    /**
     * @brief isMarster 是否为主节点项
     * @return
     */
    bool isMarster() const;

    /**
     * @brief setIsMarster 设置当前节点为主节点项
     * @param isMarster
     */
    void setIsMarster(bool isMarster);

    /**
     * @brief itemIId 项标签
     * @return
     */
    QString itemIId() const;

protected:
//    void mousePressEvent(QMouseEvent *e);

signals:
    void signalItemClicked(const QString &var);

private:
    STPMBlockItem m_content;
    bool m_isMarster;
    HZLabelDisplayItem *mLabRate{nullptr},*mLabUpDown{nullptr};

    void init();
};

/**
 * @brief The HZPlateMove class 板块异动外部类
 */
class HZPlateMove : public QWidget
{
    Q_OBJECT

public:
    explicit HZPlateMove(QWidget *parent = 0);
    ~HZPlateMove();

    /**
     * @brief request 请求
     */
    void request();

    /**
     * @brief subPlate 订阅
     */
    void subPlate();

    /**
     * @brief cancleSub 取消订阅
     */
    void cancleSub();

    /**
     * @brief addMarster 增加一个主的条目
     * @param datas
     */
    void initMarster(const STMarsterItem &datas);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::HZPlateMove *ui;
    int mPlateCnt{0};
    bool mLocker{true};
    QList<HZItemButton*> m_btns;
    QMap<UINT32, STMarsterItem> m_marsterMaps;
    QVBoxLayout *m_vLayout{nullptr};
    ReqId m_subId;

    /**
     * @brief platDataCallBack 数据请求回调
     * @param id
     * @param info
     * @param code
     */
    void platDataCallBack(ReqId id, const HZData::BlockAbnormalInfo& info, ErrCode code);
};

/**
 * @brief The HZPlateMove class 板块异动外部类
 */
class PLATEMOVE_EXPORT HZPlateMoveMarster : public BaseWidget
{
    Q_OBJECT

public:
    HZPlateMoveMarster(QWidget *parent = 0);
    ~HZPlateMoveMarster();

    /**
     * @brief 对象激活
     */
    virtual void afterActive();

    /**
     * @brief 取消激活
     */
    virtual void afterDeactive();

private:
    HZPlateMove *m_plateMove;
    QLabel *m_title;
    QTimer mTimer;
    void loadStyle();

};

#endif // MAINWINDOW_H
