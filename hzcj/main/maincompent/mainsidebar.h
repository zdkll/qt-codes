/********************************************************************
created:2018
author: juntang.xie
purpose:派生自SideBar 主界面侧边栏
*********************************************************************/
#ifndef MAINSIDEBAR_H
#define MAINSIDEBAR_H

#include "sidebar.h"

class MainSideBar : public SideBar
{
    Q_OBJECT
public:
    MainSideBar(QWidget *parent = 0,QString instName="");

private:
   void createWg();
   virtual void onMessage(const QString& senderPath,const MessageParams& params);
   void slotsButtonClick(int id);
private:
   //导航栏项
   struct NavigatorBarItem
   {
       int type;
       int childId = -1;
       QString name;
       QString style;
       NavigatorBarItem(){}
       NavigatorBarItem(int id,QString strName,QString styleName,int cd=-1):type(id),name(strName),style(styleName),childId(cd){}
   };
   void procePageChanged(const MessageParams& params);
   QHash<int,NavigatorBarItem> m_tabMap;

};

#endif // MAINSIDEBAR_H
