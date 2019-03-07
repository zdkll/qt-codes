#pragma once
/********************************************************************
created:	三月 2016
filename: 	arith.hpp
author:     min.cheng
purpose:	基础算
*********************************************************************/
#include "memblock.h"

/** 
 * @brief MA(X,N),返回X的N日移动平均值 算法：(X1+X2+X3+...+Xn)/N
 */
MemBlock* arith_ma(int current, const MemBlock* in_block, int cycle, std::string& err)
{
	if (!in_block)
	{
		err = "MAs输入数据为空!";
		return nullptr;
	}
	if (cycle > in_block->GetCount())
	{
		err = "MAs周期数太大!";
		return nullptr;
	}

	if (!in_block->IsNumber())
	{
		err = "MAs指定的参数in_block必须是数值类型";
		return nullptr;
	}

	MemBlock *result = NULL;
	int nRetLen = in_block->GetCount() - cycle + 1;
	int i = 0;
	double tSum = 0.0f;
	result = MemBlock::AllocFloat(nRetLen);
	for (; i < nRetLen; i++)
	{
		if (i == 0)
		{
			for (int j = 0; j < cycle; j++)
			{
				tSum += in_block->GetFloatAt(j);
			}
		}
		else
		{
			tSum += in_block->GetFloatAt(i + cycle - 1);;
			tSum -= in_block->GetFloatAt(i - 1);
		}
		result->SetFloatAt(i, tSum / static_cast<double>(cycle));//除数是0
	}
	return result;
}

/**
* @breif 标准差(STD)
*/
MemBlock* arith_stds(int current, const MemBlock* in_block, int cycle, std::string& err)
{
	if (!in_block)
	{
		err = "STDs输入数据为空!";
		return nullptr;
	}
	if (cycle > in_block->GetCount())
	{
		err = "STDs周期数太大!";
		return nullptr;
	}

	if (!in_block->IsNumber())
	{
		err = "STDs指定的参数in_block必须是数值类型";
		return nullptr;
	}

	if (cycle <= 1)
	{
		err = "STDs周期有误!";
		return nullptr;
	}

	MemBlock *result = NULL;
	int i = 0;
	double tSum = 0.0f;
	int nRetLen = in_block->GetCount() - cycle + 1;
	MemBlock* sMAdata = arith_ma(current, in_block, cycle, err);
	if (sMAdata)
	{
		result = MemBlock::AllocFloat(sMAdata->GetCount());
		for (; i < nRetLen; i++)
		{
			tSum = static_cast<double>(0);
			for (int j = i; j < i + cycle; j++)
			{
// 				double src = in_block->GetNumberAt(j);
// 				double sma = sMAdata->GetNumberAt(i);
// 				double ret = in_block->GetNumberAt(j) - sMAdata->GetNumberAt(i);
// 				double sum = ret * ret;
// 				tSum += sum;
				tSum += (in_block->GetNumberAt(j) - sMAdata->GetNumberAt(i))*
					(in_block->GetNumberAt(j) - sMAdata->GetNumberAt(i));
			}
			result->SetNumberAt(i, sqrt(tSum / static_cast<double>(cycle-1)));
		}
		MemBlock::Free(sMAdata);
	}
	return result;
}

/**
* @breif 若求X的N日指数平滑移动平均，则表达式为：EMA（X，N），算法是：
         若Y=EMA(X，N)，则Y=[2*X+(N-1)*Y’]/(N+1)，其中Y’表示上一周期的Y值。

		 */
MemBlock* arith_emasunchg(int current, const MemBlock* in_block, double lastVal, int cycle, std::string& err)
{
	if (!in_block)
	{
		err = "EMAunchg输入数据为空!";
		return nullptr;
	}
//	if (cycle >= in_block->GetCount())
//	{
//		err = "EMAunchg周期数太大!";
//		return nullptr;
//	}

	if (!in_block->IsNumber())
	{
		err = "EMAunchg指定的参数in_block必须是数值类型";
		return nullptr;
	}

	MemBlock* result = nullptr;
    int nRetLen = in_block->GetCount()/* - cycle*/;
	if (nRetLen > 0)
	{
		result = MemBlock::AllocFloat(nRetLen);
		if (lastVal != INVALID_MEMBLOCKDATA_FLOAT)
		{
			result->SetNumberAt(0, lastVal);
		}
        double val = in_block->GetNumberAt(0);
        for (int i=0; i < nRetLen; i++)
		{
            val = (2.0f * in_block->GetNumberAt(i) +
                static_cast<double>(cycle - 1) *val) /static_cast<double>(cycle + 1);
			result->SetNumberAt(i, val);
		}
	}
	return result;
}


/**
* @breif *求移动平均值
*        Y=SMA(X,N,M) 算法：Y=[M*X+(N-M)*Y']/N,其中Y'表示上一周期Y值,N必须大于M
*	     SMA(CLOSE,30,1)表示求30日移动平均价 1为权重
*/

MemBlock* arith_smasunchg(int current, const MemBlock* in_block, double lastVal, int n, int m, std::string& err)
{
    if (!in_block)
    {
        err = "SMAsunchg输入数据为空!";
        return nullptr;
    }

    if (!in_block->IsNumber())
    {
        err = "SMAsunchg指定的参数in_block必须是数值类型";
        return nullptr;
    }

    MemBlock* result = nullptr;
    int nRetLen = in_block->GetCount()/* - cycle*/;
    if (nRetLen > 0)
    {
        result = MemBlock::AllocFloat(nRetLen);
        result->SetNumberAt(0, 0);
        double lastRet =  in_block->GetNumberAt(0);//=lastVal != INVALID_MEMBLOCKDATA_FLOAT?lastVal:in_block->GetNumberAt(0);
        for (int i=1; i < nRetLen; i++)
        {
            lastRet = (static_cast<double>(m)*in_block->GetNumberAt(i) +
                static_cast<double>(n - m)*lastRet) / static_cast<double>(n);
            result->SetNumberAt(i, lastRet);
        }
    }
    return result;
}

/**
* @brief  *求最低值
*         LLV(LOW,0)表示求历史最低价
*/
