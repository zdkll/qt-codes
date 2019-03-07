/********************************************************************
created:  2018
author: juntang.xie
purpose:派生自NavigatorBar ，为主界面设计的导航栏
*********************************************************************/
#ifndef MAINNAVIGATORBAR_H
#define MAINNAVIGATORBAR_H
/********************************************************************
created:
author: min.cheng
purpose: 顶部导航栏实现
*********************************************************************/
#include <QWidget>
#include <QHash>
#include "navigatorbar.h"

class QPushButton;
class CSubTab;

class  MainNavigatorBar : public NavigatorBar
{
    Q_OBJECT
public:
    MainNavigatorBar(QWidget *parent = 0,QString instName="");

    void onNetIoReady(bool ready);
private slots:
    void slotSubTabSectionChanged(int,int);
private:
    void createWg();
    virtual void onMessage(const QString& senderPath,const MessageParams& params);
    void procePageChanged(const MessageParams& params);
private:
    //导航栏项
    struct NavigatorBarItem
    {
        int type;
        int childId = -1;
        int thridChildId=-1;
        QString name;
        NavigatorBarItem(){}
        NavigatorBarItem(int id,QString strName,int cd=-1,int thirdid=-1):type(id),childId(cd),thridChildId(thirdid),name(strName){}
    };
    QHash<int,NavigatorBarItem> m_tabMap;
    CSubTab* m_rootTab;
};
#endif // MAINNAVIGATORBAR_H
