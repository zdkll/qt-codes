/********************************************************************
created:	十一月 2015
filename: 	Data.cpp
author:     min.cheng
purpose:	基础数据请求接口
*********************************************************************/
#include "formula_api.h"
#include "memblock.h"
#include "Environment.h"

#define TrendDirectUp 1
#define TrendDirectDown 2

#define BuyPoint 1
#define SellPoint 2

#define TrendInfoUp 1
#define TrendInfoDown 2
#define TrendInfoShock 3

void OutputErr(CEnvironment* env, const std::string& msg)
{

}

MemBlock* CalcSrcData(CEnvironment* env, MemBlock* data)
{
	MemBlock* pRet = nullptr;
	if (data)
	{
        pRet = MemBlock::Alloc(data->GetCount(), 0.0);
        MemBlock::Assign(pRet, 0, data, 0, data->GetCount());
	}
	return pRet;
}

HZ_FORMULA_CAPI MemBlock* KLINETIME(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
	MemBlock* data = nullptr;
    if (srcData->pTime)
	{
        data = CalcSrcData(env, srcData->pTime);
	}
	return data;
}

HZ_FORMULA_CAPI MemBlock* OPEN(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pOpen)
    {
        data = CalcSrcData(env, srcData->pOpen);
    }
    return data;
}

HZ_FORMULA_CAPI MemBlock* CLOSE(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pClose)
    {
        data = CalcSrcData(env, srcData->pClose);
    }
    return data;
}

HZ_FORMULA_CAPI MemBlock* HIGH(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pHigh)
    {
        data = CalcSrcData(env, srcData->pHigh);
    }
    return data;
}

HZ_FORMULA_CAPI MemBlock* LOW(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pLow)
    {
        data = CalcSrcData(env, srcData->pLow);
    }
    return data;
}

HZ_FORMULA_CAPI MemBlock* VOLUME(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pVolume)
    {
        data = CalcSrcData(env, srcData->pVolume);
    }
    return data;
}

HZ_FORMULA_CAPI MemBlock* AMOUNT(CEnvironment* env)
{
    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pAmount)
    {
        data = CalcSrcData(env, srcData->pAmount);
    }
    return data;
}

HZ_FORMULA_CAPI MemBlock* TICKCOUNT(CEnvironment* env)
{

    std::shared_ptr<IndiCalcData> srcData = env->GetInitCalcUnitParam().ptrData;
    MemBlock* data = nullptr;
    if (srcData->pTickCount)
    {
        data = CalcSrcData(env, srcData->pTickCount);
    }
    return data;
}
