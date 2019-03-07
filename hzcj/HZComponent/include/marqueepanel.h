#ifndef MARQUEEPANEL_H
#define MARQUEEPANEL_H
/********************************************************************
created:
author: min.cheng
purpose: 跑马灯面板
*********************************************************************/
#include <QVector>
#include "basewidget.h"
#include "hzcomponent_global.h"
#include "componentdef.h"

class QGraphicsScene;
class MarqueeReq;
class MarqueeGraphicsView;


class HZCOMPONENTSHARED_EXPORT MarqueePanel:public BaseWidget
{
    Q_OBJECT
public:
    explicit MarqueePanel(QWidget *parent = 0,QString instName="");

    ~MarqueePanel();

    void resizeEvent(QResizeEvent *event);

    //7*24小时资讯数据回调
    void onDataCall(int ret,const QString& msg);

    //7*24数据详情
    void onDetailDataCall(int ret,const QString& msg);

    //资讯点击事件
    void onMarqueeClick(const QString& id);
signals:

public slots:
    void onTimeRefreshNews();
private:
    void updateScene();
    MarqueeReq* m_req;
    QVector<MarqueeDataItem> m_data;
    QGraphicsScene* scene;
    MarqueeGraphicsView* m_view;
};

#endif // MARQUEEPANEL_H
