#ifndef SUOLUERISEFALL_H
#define SUOLUERISEFALL_H

#include "SuolueBase.h"

class RiseFallBarView;
class SuolueRiseFall  : public SuolueBase
{
    Q_OBJECT
public:
    SuolueRiseFall(SuolueInfo info,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    //创建内容区域窗口
    QWidget* createContentWnd();

    //导航点击以后触发
    void onClickItem(const SuolueItem& item);

private:
  RiseFallBarView  *m_risefallBarView;
  QStringList           m_risefallStocks;
};

#endif // SUOLUERISEFALL_H
