#ifndef SIMILARKLINEMAINWND_H
#define SIMILARKLINEMAINWND_H
/********************************************************************
created:
author: min.cheng
purpose: 相似K线布局管理
*********************************************************************/
#include "datacenterbasewnd.h"
#include "basewidget.h"
#include "hzchildwndmgr.h"

enum SimilarKlineChildWndType
{
    SimilarKlineType = 0,
    RecordReviewType,
};

class SimiKView;
//右侧盘口
class SimilarKlineHandicapWidget;
class SimilarKlineRightPanel: public BaseWidget
{
    Q_OBJECT
public:
    explicit SimilarKlineRightPanel(QWidget *parent = nullptr, QString instName = "");

    void afterDeactive();

    void afterActive();

    void switchStocks(const QString& stock1,const QString& stock2);
signals:
    void signalSwitchSimilarKlineWnd(int64_t time);
    void signalSwitchRecordReviewWnd();
public slots:
    void slotsSwtichWnd(int id);

private:
    SimilarKlineHandicapWidget *m_rightHandicap;
};


//左侧窗口
class SimilarKlineMainWnd : public NormalChildWnd
{
    Q_OBJECT
public:
    explicit SimilarKlineMainWnd(QWidget *parent = nullptr, QString instName = "");

    void afterDeactive();

    void afterActive();

    void createWg();
signals:

public slots:
    void slotSwitchSimilarKlineWnd(int64_t time);
    void slotSwitchRecordReviewWnd();
private:
    //相似K线
    ChildWndItem createSimilarKlineWnd();
    //战绩回顾
    ChildWndItem createRecordReviewWnd();
private:
    //子页面管理器
    HzChildWndMgr<SimilarKlineMainWnd>* m_pMainChildWndMgr = nullptr;
    //右侧窗口
    SimilarKlineRightPanel* m_pRightWnd;

    //左侧列表
    SimiKView  *m_similarKView;

    //时间戳
    int64_t m_currTime = -1;
};

#endif // SIMILARKLINEMAINWND_H
