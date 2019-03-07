#include "suoluefactory.h"
#include "suoluemin.h"
#include "suoluehqview.h"
#include "suolueblock.h"
#include "suoluefund.h"
#include "suoluezxhist.h"
#include "suoluehotinfo.h"
#include "suoluerisefall.h"

IocContainer *SuolueFactory::getIoc()
{
    static SuolueFactory isntance;
    return &(isntance.m_ioc);
}

SuolueFactory::SuolueFactory()
{
    //缩略图注册
    m_ioc.RegisterNomal<SuolueBase,SuolueInfo, QWidget*>("SuolueBase");
    m_ioc.RegisterInherit<SuolueBase,SuolueMin,SuolueInfo, QWidget*>("SuolueMin");
    m_ioc.RegisterInherit<SuolueBase,SuolueHqView,SuolueInfo, QWidget*>("SuolueHqView");
    m_ioc.RegisterInherit<SuolueBase,SuolueBlock,SuolueInfo, QWidget*>("SuolueBlock");
    m_ioc.RegisterInherit<SuolueBase,SuolueFund,SuolueInfo, QWidget*>("SuolueFund");
    m_ioc.RegisterInherit<SuolueBase,SuolueZxHist,SuolueInfo, QWidget*>("SuolueZxHist");
    m_ioc.RegisterInherit<SuolueBase,SuoLueHotInfo,SuolueInfo, QWidget*>("SuoLueHotInfo");
    m_ioc.RegisterInherit<SuolueBase,SuolueRiseFall,SuolueInfo, QWidget*>("SuolueRiseFall");

}
