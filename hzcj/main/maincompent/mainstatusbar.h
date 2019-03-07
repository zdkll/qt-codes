/********************************************************************
created:  2018
author: juntang.xie
purpose:派生自StatusBar ，为主界面设计的状态栏
*********************************************************************/
#ifndef MAINSTATUSBAR_H
#define MAINSTATUSBAR_H

#include "statusbar.h"
#include "datacontroller.h"
using namespace HZData;
class QLabel;
class StatusBarIndexMgr;
class ActivityLabel;
class LocalTimeWidget;
class QPushButton;
class  MainStatusBar : public  StatusBar
{
    Q_OBJECT

public:
    MainStatusBar(QWidget *parent = 0,QString instName="");
    ~MainStatusBar();

    //激活/取消激活 用于外部调用时处理 方便资源开启和释放
    void afterActive();
    void afterDeactive();

    void setConnect(bool isOnline = false);

    void onMessage(const QString& senderPath,const MessageParams& params);
private:
   void createWg();
   void createStockPanel();
   void startSub();
   void cancelSub();
//   void onAbnormalInfoArrived(ReqId reqID, const StockAbnormalInfo &abnormalInfo, ErrCode errCode);
   void onSubDataArrived(ReqId reqID, const ShortMotionInfo& info, ErrCode errCode);
   void setTextEx(QString strCode, QString strText, int dir);
   void upDateLabel(ShortMotionType type, QLabel*label);
private slots:
    void onLabelClicked(const QString &strCode);
    void onBtnMoreClicked();
    void slotSubIndexArrived(char *data, int num);
private:
    bool m_bFlg;
    ReqId m_nReq;
    StatusBarIndexMgr  *m_indexMgr;
    QMap<QString, QLabel*> m_labelMap;
    ActivityLabel *m_pLabel;
    QString m_strCurrentCode;
    LocalTimeWidget *m_localTimeWidget;
    QPushButton *m_pBtnmore;
};
/**
*@brief 数据请求类
*/
class StatusBarIndexMgr: public QObject
{
    Q_OBJECT
public:
    StatusBarIndexMgr(QObject *parent = Q_NULLPTR):QObject(parent),m_nReq(INVALID_REQID){}

    void startSub();

    void cancelSub();

signals:
    void subStockArrived(char *data, int num);

private:
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);

private:
    ReqId m_nReq;
};

/**
*@brief 右下角时间部件
*/

class QPushButton;
class LocalTimeWidget:public BaseWidget
{
public:
    LocalTimeWidget(QWidget *parent = nullptr);
    ~LocalTimeWidget();

    virtual void afterActive();

    virtual void afterDeactive();

    void setConnect(bool isOnline = false);

protected:
    void timerEvent(QTimerEvent *e);
private:
    void createWg();
    void startReqest();
    void onTimeArrived(ReqId id, int nTime, ErrCode errCode);

private:
    ActivityLabel  *m_pTimeLabel;

    QPushButton *m_pLogoBtn;
    int m_nTimerID;
    int m_time = INT_MAX;
    int m_checkTime = 5;
};

/**
*@brief 股票信息面板
*/
class StockInfoPanel:public BaseWidget
{
    Q_OBJECT
public:
    StockInfoPanel(QWidget *parent = nullptr, QString strMarket = "");

    ActivityLabel *getInfoLabel();

private:
    void createWg();

private:
    ActivityLabel *m_pTextLabel;
    QString m_strMarket;

signals:
    void clicked(const QString &strCode);
};


#endif // MAINSTATUSBAR_H
