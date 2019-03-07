#ifndef HZPAGEHELPER_H
#define HZPAGEHELPER_H
/********************************************************************
created:
author: min.cheng
purpose: 负责页面管理，延迟创建页面等
*********************************************************************/
#include <QList>
#include <QMap>
#include <QStackedWidget>
#include "basepage.h"


//页面信息
struct PageInfo
{
    QString className;
    QString instanceName;
    PageInfo(){}
    PageInfo(const QString& strClass,const QString& strInstance):
        className(strClass),
        instanceName(strInstance)
    {

    }
};

#define  InvalidPage -1

//页面管理
class HzPageHelper : public BasePage
{
    Q_OBJECT
public:
    explicit HzPageHelper(QWidget *parent = 0,QString instName="");

    //设置当前页面,不存在则创建
    void setCurrPageId(int id,const DetailPageParam& info);

    //获取当前页面
    int getCurrPageId();

    //激活当前页面
    virtual void afterActive();

    //取消激活当前页面
    virtual void afterDeactive();

    void onNetIoReady(bool ready);

    //默认创建
    void defaultCreaePage(const QVector<int>& pages);
signals:

public slots:

private:
    //配置中获取 页面配置信息。目前先内存中处理
    void readPageCfg();
    //页面创建
    BasePage* createPage(const QString &className, const QString &instanceName);
    //广播页面切换消息
    void notifyPageChange(int id,int childId);
private:
    QStackedWidget *m_stackWnd;  //堆栈窗体
    QMap<int,int> m_idIndexMap;  //页面ID和堆栈窗体下标映射 key=PageId value=index
    QMap<int,int> m_indexIdMap;  //堆栈窗体下标和页面ID映射 key=index value=PageId;
    QMap<int,PageInfo> m_idInfoMap; //配置的信息
};

#endif // HZPAGEHELPER_H
