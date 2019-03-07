#ifndef SuolueBase_H
#define SuolueBase_H
/********************************************************************
created:
author: min.cheng
purpose: 抽象的缩略图类。实现了动态配置导航栏项目的功能。同时支持少量样式定制
*********************************************************************/
#include <QVector>
#include <QMap>
#include <QWidget>

#include "basewidget.h"
#include "subtab.h"
//标题项定义
struct SuolueItem
{
    int id;
    QString name;
    SuolueItem(){}
    SuolueItem(int nId,const QString& strName):id(nId),name(strName){}
};

//缩略图信息
struct SuolueInfo
{
    QString className;
    QVector<SuolueItem> itemList;
    int defaultId = 0;
    int attr = 0;

    Qt::Alignment align = Qt::Alignment();
    int row = 0;
    int column = 0;
    int rowSpan = 1;
    int columnSapn =1;
    Any data;
};

//自定义属性
#define SuolueHideMoreBt 0x0001

//缩略图标题基类
class SuolueTitleBase:public BaseWidget
{
    Q_OBJECT
public:
    explicit SuolueTitleBase(SuolueInfo info,QWidget *parent = 0,QString instName="");
    virtual ~SuolueTitleBase(){}

    virtual void createWg();

    const QMap<int,SuolueItem>& getItemMap() const;

    const SuolueInfo& getSuolueInfo() const;
signals:
    void signalTabCurrentBtnChanged(int nId);
    void signalBtClicked();
public slots:
    virtual void slotsTabCurrentBtnChanged(int nId);
    virtual void slotsBtClicked();
protected:
    SuolueInfo m_slInfo;
private:
    CSubTab *m_subTab;
    QMap<int,SuolueItem> m_idMap;
};

//


//缩略图基类
class  SuolueBase:public BaseWidget
{
    Q_OBJECT
public:
    explicit SuolueBase(SuolueInfo info,QWidget *parent = 0);
    virtual ~SuolueBase();

    //数据设置好以后调用
    virtual void createWg();

    //创建标题区域
    virtual SuolueTitleBase* createTitleWnd();

    //创建内容区域窗口
    virtual QWidget* createContentWnd();

    //创建完成回调
    virtual void onCreate();

    //导航点击以后触发
    virtual void onClickItem(const SuolueItem& item);

    //点击更多以后触发
    virtual void onClickMore();

    //
    virtual void onFocusOut(){}

signals:
    void currentStockChanged();//选中股票

private slots:
    void slotsTabCurrentBtnChanged(int nId);
    void slotsBtClicked();
protected:
    SuolueInfo m_slInfo;
    SuolueTitleBase* m_title;

};

#endif // SuolueBase_H
