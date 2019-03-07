/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自BaseWidget   短线异动列表容器
*********************************************************************/
#ifndef ShortTermChangeWidget_H
#define ShortTermChangeWidget_H
#include "basewidget.h"
#include "datacontroller.h"
using namespace HZData;


struct ShortDataRecord{
    ReqId reqId = INVALID_REQID;
    int index = -1;   //-1表示最新，其他表示可寻址下标
    int count = 0;
    bool is_desc = true;
};

class ShortTermChangeView;
class QScrollBar;
class  ShortTermChangeWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit ShortTermChangeWidget(QWidget *parent = nullptr);


    ~ShortTermChangeWidget();
    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);

    void showEvent(QShowEvent *e);

signals:
    void clickedRow(const QString strCode);

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

    int saveFundData(const ShortDataRecord &recrd,const ShortMotionInfo &FundInfo);


private:
    ShortTermChangeView *m_shortTermChangeView;
    QPair<int, int> m_dataRange;
    ShortMotionInfo m_fundInfo;
    QScrollBar *m_verScrollBar;
    ReqId               m_subReqId;
    ShortDataRecord       m_reqRecrd;
    bool m_isInitialed;
};

#endif // ShortTermChangeWidget_H
