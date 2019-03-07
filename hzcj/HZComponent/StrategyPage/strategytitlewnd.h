/********************************************************************
created:
author: juntang.xie
purpose: 用于展示策略页面的标题部分
*********************************************************************/
#ifndef STRATEGYTITLEWND_H
#define STRATEGYTITLEWND_H

#include "basewidget.h"
#include "hzinfocenter.h"

class DynaDataProvider;
class QLabel;
class StrategyTitleWnd :public BaseWidget
{
    Q_OBJECT
public:
    explicit StrategyTitleWnd(QWidget *parent = 0);

    void setDynaProvider(DynaDataProvider* pProvider);

    void updatedata(const TopicInvestDetail& data);

protected:

    virtual void
    resizeEvent(QResizeEvent *event);


private:
    void createWg();
    void updateText();
    void setObjName();
signals:

public slots:

private:
    DynaDataProvider* m_pProvider = nullptr;
    QLabel *m_pNameLabel;
    QLabel *m_pZyxImgLabel;
    QLabel *m_pCxxImgLabel;
    QLabel *m_pContentLael;

    TopicInvestDetail m_deTailData;
};

#endif // STRATEGYTITLEWND_H
