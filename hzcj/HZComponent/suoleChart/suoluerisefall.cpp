#include "suoluerisefall.h"

#include "risefallbarview.h"

SuolueRiseFall::SuolueRiseFall(SuolueInfo info,QWidget *parent)
    :SuolueBase(info,parent)
{
  m_risefallStocks = {"000000","sh000001","sz399001","sz399005","sz399006"};
}

void SuolueRiseFall::afterActive()
{
    m_risefallBarView->afterActive();
}

void SuolueRiseFall::afterDeactive()
{
    m_risefallBarView->afterDeactive();
}

//创建内容区域窗口
QWidget* SuolueRiseFall::createContentWnd()
{
    m_risefallBarView = new RiseFallBarView(this);
    return  m_risefallBarView;
}

//导航点击以后触发
void SuolueRiseFall::onClickItem(const SuolueItem& item)
{
    m_risefallBarView->setStockCode(m_risefallStocks[item.id]);
}
