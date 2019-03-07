/********************************************************************
created:2018/04/20
author: min.cheng juntang.xie dk.zhang
purpose: web 浏览相关窗口和自定义 webView类
*********************************************************************/
#ifndef PopWebView_H
#define PopWebView_H
#include <QWebEngineView>
#include <QWebEnginePage>
#include "hzobject.h"
#include "frameless_helper.h"
#include <QVBoxLayout>
#include <functional>
#include <memory>
#include <vector>

#include "framewidget.h"
#include "popupwindow.h"

typedef std::function<QWebEngineView *(QWebEnginePage::WebWindowType,QWebEngineView*)> ProceCreateWindow;




//内嵌View页面-----------
class InternalWebView :public QWebEngineView
{
    Q_OBJECT
public:
    InternalWebView(QUrl url, QWidget *parent = nullptr);
    InternalWebView(QWidget *parent = nullptr);
    ~InternalWebView();

protected:
//    virtual QWebEngineView * createWindow(QWebEnginePage::WebWindowType type);
    QUrl url_;
};

//PopWebView弹出窗口,内嵌View页面---------------------
class TitleBar;
class BaseView;
class HZCONTROLSSHARED_EXPORT PopWebView : public PopupWindow
{
    Q_OBJECT
public:
    explicit PopWebView(InternalWebView* view,QWidget* parent=nullptr,QString instName="");
    ~PopWebView();
    InternalWebView *webView();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    InternalWebView* pWebView_;
    QString                   m_strTile;
};

//用来管理BaseWebView弹出PopupWindow对话框
class HZCONTROLSSHARED_EXPORT BaseView : public QObject
{
public:
    ~BaseView();
    static BaseView &instance();

    QVector<PopupWindow*> windows();
    void addWindow(PopupWindow* window);
    void remove(PopupWindow* window);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    BaseView(QObject *parent = 0):QObject(parent){}
    QVector<PopupWindow*> m_windows;
};

//浏览Web窗口主页面---------------------
//#include
class HZCONTROLSSHARED_EXPORT BaseWebView : public BaseWidget
{
    Q_OBJECT
public:
    explicit BaseWebView(QUrl &url, BaseWidget *parent = nullptr,QString instName="");
    ~BaseWebView();

public:
    InternalWebView *pWebView_;

protected:

private:
    void initLayout();
};

#endif // PopWebView_H
