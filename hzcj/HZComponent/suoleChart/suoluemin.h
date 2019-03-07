#ifndef SUOLUEMIN_H
#define SUOLUEMIN_H
/********************************************************************
created:
author: min.cheng
purpose: 分时缩略图
*********************************************************************/
#include "SuolueBase.h"
#include "mincanvas.h"

class SuolueMin : public SuolueBase
{
    Q_OBJECT
public:
    explicit SuolueMin(SuolueInfo info,QWidget *parent = 0);
    virtual ~SuolueMin();

    void afterActive();

    void afterDeactive();

    //创建内容区域窗口
    QWidget* createContentWnd();

    //导航点击以后触发
    void onClickItem(const SuolueItem& item);

    //点击更多以后触发
    void onClickMore();
private:
    QVector<ChartItemInfo> getChartInfo();
private:
    QMap<int,QString> m_codeMap;
    MinCanvas* m_minChart;
    QString m_obj;
};

#endif // SUOLUEMIN_H
