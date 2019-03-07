/********************************************************************
created:  2018/04/12
author: dk.zhang
purpose:通用的侧边栏基类
*********************************************************************/
#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QMap>

#include "basewidget.h"
#include "hzcontrols_global.h"

class QButtonGroup;
class QToolButton;
class QVBoxLayout;
class QAbstractButton;
class HZLine;
class QPushButton;
class  HZCONTROLSSHARED_EXPORT SideBar : public  BaseWidget
{
    Q_OBJECT
public:
    SideBar(QWidget *parent = 0,QString instName="");
    //检查指定Id是否是按下状态
    bool isCheck(int id);
    //获取当前按下按钮ID
    int getCurrCheckId();
    //设置当前ID的按钮为按下状态 仅用于添加到组的按钮
    void setCheckId(int id);
    //设置所有为未选中项 仅用于添加到组的按钮
    void setAllUnCheck();
    QToolButton *addToolButton(QToolButton *toolButton,int id,const bool &addToGrp = true);
    QToolButton *addToolButton(const QString &text,int id,const bool &addToGrp = true,Qt::ToolButtonStyle toolButtonStyle= Qt::ToolButtonTextUnderIcon);
    QToolButton *addToolButton(const QIcon &icon,int id,const QString &text,const bool &addToGrp = true,Qt::ToolButtonStyle toolButtonStyle= Qt::ToolButtonTextUnderIcon);
    HZLine *addSeparator();

    QPushButton *addLogo();
    void addStretch();

public slots:
    virtual void slotsButtonClick(int id);
signals:
    void signalClickButton(int id);
protected:

protected:
    QButtonGroup  *m_btnGroup;
    QMap<int,QToolButton *> m_idMap;

private:
    void createWg();

private:
    QVBoxLayout    *m_vLayout;
};

#endif // SIDEBAR_H
