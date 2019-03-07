#ifndef SUOLUEHQVIEW_H
#define SUOLUEHQVIEW_H

#include "SuolueBase.h"
#include "slrisefallwidget.h"

//缩略图标题
class HqSuolueTitle:public SuolueTitleBase
{
    Q_OBJECT
public:
    explicit HqSuolueTitle(SuolueInfo info,QWidget *parent = 0,QString instName="");
    virtual ~HqSuolueTitle(){}

    void createWg();
};

class SuolueHqView : public SuolueBase
{
    Q_OBJECT
public:
    SuolueHqView(SuolueInfo info,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    //创建内容区域窗口
    QWidget* createContentWnd();

    //创建标题区域
    SuolueTitleBase* createTitleWnd();

    void onFocusOut();

private:
    SLRiseFallWidget *m_RiseFallWg;
};

#endif // SUOLUEHQVIEW_H
