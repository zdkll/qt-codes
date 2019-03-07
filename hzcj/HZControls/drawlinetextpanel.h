#ifndef DRAWLINETEXTPANEL_H
#define DRAWLINETEXTPANEL_H
/********************************************************************
created:
author: min.cheng
purpose: 用于绘制文本处于一行时，其中特文本或数字在字体和颜色上不一样的情况
         比如： 龙虎榜 文本加数字其中数字字体和颜色都不一样 这种情况下，可以套用
         这个类来实现，前提是文本是基于一行的,文本绘制顺序是按照添加顺序来的
         默认绘制对齐方式是： 居左，居中
*********************************************************************/
#include <QFont>
#include <QMap>
#include <QPen>
#include <QVector>
#include <QPaintEvent>
#include <QFrame>
#include "hzcontrols_global.h"
#include "basewidget.h"


struct DrawLineTextItem
{
    QFont font;       //即将绘制文本字体
    QPen pen;         //即将绘制文本的笔
    QString text;     //即将绘制的文本
    QString itemName; //即将绘制文本的名称 用于我们内部区分字段
    int left_padding = 0;
};

class HZCONTROLSSHARED_EXPORT DrawLineTextPanel :public BaseWidget
{
    Q_OBJECT
public:
    explicit DrawLineTextPanel(QWidget *parent = 0);

    void addItem(const DrawLineTextItem& item,bool bUpdate =false);

    void addItems(const QVector<DrawLineTextItem>& items,bool bUpdate=false);

    void remove(const QString& name,bool bUpdate =false);

    void removeAll(bool bUpdate =false);

    void setItem(const DrawLineTextItem& item,bool bUpdate =false);

    DrawLineTextItem getItem(const QString& name);
signals:

public slots:

protected:
    int indexOfItem(const QString& name);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *event);
    void redraw();
private:
    struct InnerItem
    {
        DrawLineTextItem item;
        int nWidth;
    };
    QVector<InnerItem> m_itemList;
};

#endif // DRAWLINETEXTPANEL_H
