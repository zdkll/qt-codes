#ifndef BASEPAGE_H
#define BASEPAGE_H

#include "hzcontrols_global.h"
#include "basewidget.h"

#include <QDebug>

//命令参数解析了常用的几种命令，cmd中包含了全部命令（包括已经解析的，方便用户进行命令扩展）
struct DetailPageParam
{
    //股票代码
    QString stock = "";
    //周期
    int period = -1;
    //子窗口ID
    int childWndId = -1;
    //三级子窗口ID
    int thirdChildWndId = -1;
    //龙虎榜ID
    QString winnersListId;
    //时间
    qint64 timeNumber = -1;
    //当前发命令所在子窗口ID
    int fromChildId = -1;
    //排行Flag
    int filterFlag = -1;

    //命令集合
    MessageParams cmd;
};


class HZCONTROLSSHARED_EXPORT BasePage : public BaseWidget
{
    Q_OBJECT
public:
    explicit BasePage(QWidget *parent = 0,QString instName="");

    virtual void recvParam(const DetailPageParam &param,bool bCurrPage = false);

    virtual void afterActive();

    virtual void afterDeactive();
protected:
    QWidget* parent_;
};

#endif // BASEPAGE_H
