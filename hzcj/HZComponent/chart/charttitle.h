#ifndef CHARTTITLE_H
#define CHARTTITLE_H
/********************************************************************
created:
author: min.cheng
purpose: 绘图区域标题
*********************************************************************/
#include <QVector>
#include "basechart.h"
class QPushButton;
class QComboBox;
class ChartTitle: public BaseChart
{
    Q_OBJECT
public:
    ChartTitle(ImageLayer* layer);

    ~ChartTitle();

    //用于区域标题上面的数据绘制
    void updateTextItem(const std::vector<TitleTextItem>& list);

    //接收坐标刷新消息，同时清理掉上次的数据
    virtual void mainChartXaixsChnage(const QString& msg);

    //获取chart主要绘图区域
    virtual QRect getChartMainArea();

    void switchDrType();
signals:
    //标题通知当前图消息
    void signalButtonClose();
    void signalButtonParamSet();
    void signalButtonPlus();
    void signalButtonSub();
    //前后复权切换
    void signalRestorationComBoxChanged(int index);
    //指标切换
    void signalIndiSeltComBoxChanged(int index);
public slots:
    //标题通知当前图消息
    void slotButtonClose();
    void slotButtonParamSet();
    void slotButtonPlus();
    void slotButtonSub();
    //前后复权切换
    void slotRestorationComBoxChanged(int index);
    //指标切换
    void slotIndiSeltComBoxChanged(int index);
protected:
    void onCreate();
    void onResize();
    void createButton();
    void resizeButton();
protected:
    struct ChildItemInfo
    {
       QWidget* pWnd;
       int hight = 12;
       int widht = 12;
       int leftPadding = 5;
    };
    QComboBox* m_pIndiSeltComBox = nullptr;
    QComboBox* m_pReCombox;
    QVector<ChildItemInfo> m_childWndInfo;
    int m_leftPadding;
    int m_rightPos;
};

#endif // CHARTTITLE_H
