/********************************************************************
created:2018.9
author: juntang.xie
purpose:热门资讯缩略
*********************************************************************/
#ifndef SUOLUEHOTINFO_H
#define SUOLUEHOTINFO_H
#include "SuolueBase.h"

class InfoWidget;
class SuoLueHotInfo : public SuolueBase
{
    Q_OBJECT
public:
    SuoLueHotInfo(SuolueInfo info,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    //布局界面
    QWidget *createContentWnd();

    //点击更多以后触发
    void onClickMore();

    void onClickItem(const SuolueItem& item);

    //创建标题区域
    virtual SuolueTitleBase* createTitleWnd();
private:
    InfoWidget *m_infoWidget = nullptr;
};

#endif // SUOLUEHOTINFO_H
