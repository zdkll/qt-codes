/********************************************************************
created:2018.10.30
author: juntang.xie
purpose:派生自BaseWidget   资金异动列表容器
*********************************************************************/
#ifndef FundChangeWidget_H
#define FundChangeWidget_H
#include "basewidget.h"
#include "datacontroller.h"
using namespace HZData;


struct DataRecord{
    ReqId reqId = INVALID_REQID;
    int index = -1;   //-1表示最新，其他表示可寻址下标
    int count = 0;
    bool is_desc = true;
};

class FundChangeView;
class QScrollBar;
class  FundChangeWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit FundChangeWidget(QWidget *parent = nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();


    void switchToUpDownStock(bool isUp,int step);

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);

    void showEvent(QShowEvent *e);


    virtual void
    keyReleaseEvent(QKeyEvent *e);


signals:
    void clickRow(const QString strCode);

private slots:
    void scrollPressed();
    void scrollActionTriggered();
    void scrollValueChanged(int value);

private:
    void createWg();
    void init();
    void startSub();
    void cancelSub();
    void reqFundData();
    //倒拉请求数据
    ReqId postFundRequest(const int &index, const int &count, const bool &is_desc);

    //请求数据回调
    void onReqDataArrived(ReqId id , const ShortMotionInfo &FundInfo, ErrCode errCode);

    //订阅数据回调
    void onSubDataArrived(ReqId id , const ShortMotionInfo &FundInfo, ErrCode errCode);


    //value 不变，更新page 和range
    void updateScrollBarPageAndRange();

    //更改value,对齐
    void alignScrollBar();

    void updateScrollBar(const int &value);

    int saveFundData(const DataRecord &recrd,const ShortMotionInfo &FundInfo);


private:
    FundChangeView *m_FundChangeView;
    QPair<int, int> m_dataRange;
    ShortMotionInfo m_fundInfo;
    QScrollBar *m_verScrollBar;
    ReqId               m_subReqId;
    DataRecord       m_reqRecrd;
    bool m_isInitialed;
    int m_nRowCount;
};

#endif // FundChangeWidget_H
