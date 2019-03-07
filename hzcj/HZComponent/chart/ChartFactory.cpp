#include "chartfactory.h"
#include <qwidget.h>

#include "imagelayer.h"
#include "chartdef.h"


//K线分时绘图相关
#include "crosslinechart.h"
#include "klinechart.h"
#include "indicatorchart.h"
#include "klinetimexaxischart.h"
#include "Minchart.h"
#include "mintimexaxischart.h"
#include "minblockabnormal.h"

#include "charttitle.h"
#include "indicatorprovider.h"


IocContainer *ChartFactory::getIoc()
{
    static ChartFactory isntance;
    return &(isntance.m_ioc);
}


ChartFactory::ChartFactory()
{
    //K线指标绘图相关
    m_ioc.RegisterInherit<BaseChart,CrossLineChart,ImageLayer*>("CrossLineChart");
    m_ioc.RegisterInherit<BaseChart,KlineChart,ImageLayer*>("KlineChart");
    m_ioc.RegisterInherit<BaseChart,IndicatorChart,ImageLayer*>("IndicatorChart");
    m_ioc.RegisterInherit<BaseChart,KlineTimeXaxisChart,ImageLayer*>("KlineTimeXaxisChart");
    m_ioc.RegisterInherit<BaseChart,Minchart,ImageLayer*>("Minchart");
    m_ioc.RegisterInherit<BaseChart,MinBlockAbnormal,ImageLayer*>("MinBlockAbnormal");
    m_ioc.RegisterInherit<BaseChart,MinTimeXaxisChart,ImageLayer*>("MinTimeXaxisChart");
    m_ioc.RegisterInherit<IndicatorProvider,IndiLocalProvider,BaseChart*>("IndiLocalProvider");
    m_ioc.RegisterInherit<IndicatorProvider,IndicatorSrvProvider,BaseChart*>("IndicatorSrvProvider");

    m_ioc.RegisterNomal<ChartTitle,ImageLayer*>("ChartTitle");
}
