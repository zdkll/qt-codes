/********************************************************************
created:  2018/04/16
author: min.cheng   dk.zhang
purpose: 导航栏基类，提供添加行（最多两行）的功能，必须有主导航，次导航栏可选
*********************************************************************/
#ifndef NAVIGATORBAR_H
#define NAVIGATORBAR_H

#include "hzcontrols_global.h"
#include "basewidget.h"
#include <vector>
#include <map>
#include <QMap>

#include "subtab.h"

struct NaviTableItem
{
    QString name_;
    QString url_;
    NaviTableItem(){}
    NaviTableItem(const QString& name,const QString& url):
        name_(name),
        url_(url)
    {}
};

class QPushButton;
class QVBoxLayout;
class HZCONTROLSSHARED_EXPORT NavigatorBar : public BaseWidget
{
    Q_OBJECT
public:
    explicit NavigatorBar(QWidget *parent = 0,QString instName="");

    //添加导航条
    void addCSubTab(CSubTab *csubTab);
    CSubTab *addCSubTab(CSubTab::CSubTabType = CSubTab::Root_Tab, QString instName=QString());

    void addButtonToTab(const int &id,const QString &text);
    void setSubTabButtons(const int &id,const QStringList &textList);

    CSubTab *csubTab(int id){return m_csubTabs.value(id);}

signals:
    void navigatorSectionChanged(int  priButton,int secButon);
    void subTabSectionChanged(int id,int button);

private slots:
    void OnClickBack();

    void tabCurrentBtnChanged(const int &id);

private:
    void createWg();

private:
    QMap<int ,CSubTab *> m_csubTabs;
    QVBoxLayout  *m_mainLayout;
};


#endif // NAVIGATORBAR_H
