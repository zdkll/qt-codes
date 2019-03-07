/********************************************************************
created:	十一月 2015
filename: 	math.cpp
author:     min.cheng
purpose:	基础算法
*********************************************************************/
#include "formula_api.h"
#include "memblock.h"
#include "Environment.h"
#include "IndiArith.hpp"

void OutputMsg(CEnvironment* env, const std::string& msg)
{

}

/** 
 * @brief 指定cycle个周期从in_block中求和平均
 * @param[in] env运行时环境变量
 * @param[in] current 表示当前运算点，用于增量计算，-1 表示计算全量
 * @param[in] in_block输入的求周期的数据
 * @param[in] cycle指定求多少周期
 * @retval  MA(X,N),返回X的N日移动平均值 算法：(X1+X2+X3+...+Xn)/N
 */
HZ_FORMULA_CAPI MemBlock* MAs(CEnvironment* env, 
							 int current,
							 const MemBlock* in_block,
							 int cycle)
{
	std::string err = "";
	MemBlock *result = arith_ma(current, in_block, cycle, err);
	if (result == nullptr && !err.empty())
	{
		OutputMsg(env,err.c_str());
	}
	return result;

	//MemBlock *result = nullptr;
	//int nRetLen = in_block->GetCount() - cycle + 1;
	//int i = 0;
	//double tSum = 0.0f;
	//result = MemBlock::Alloc(nRetLen, INVALID_MEMBLOCKDATA_FLOAT);
	//for (; i < nRetLen; i++)
	//{
	//	if (i == 0)
	//	{
	//		for (int j = 0; j < cycle; j++)
	//		{
	//			tSum += in_block->GetFloatAt(j);
	//		}
	//	}
	//	else
	//	{
	//		tSum += in_block->GetFloatAt(i + cycle - 1);;
	//		tSum -= in_block->GetFloatAt(i - 1);
	//	}
	//	result->SetFloatAt(i, tSum / static_cast<double>(cycle));//除数是0
	//}
	//return result;
}

/**
* @breif 标准差(STD)
*/
HZ_FORMULA_CAPI MemBlock* STDs(CEnvironment* env, 
							   int current, 
							   const MemBlock* in_block, 
							   int cycle)
{
	std::string err = "";
	MemBlock* result = arith_stds(current, in_block, cycle, err);
	if (result == nullptr && !err.empty())
	{
		OutputMsg(env,err.c_str());
	}
	return result;
}

/**
* @breif 指数平滑移动平均(EMA)N变化 目前未使用
*/
HZ_FORMULA_CAPI MemBlock* EMAschg(CEnvironment* env, 
								  int current, 
								  const MemBlock* in_block, 
								  int cycle)
{
	if (!in_block)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock *result = NULL;
	if (!in_block->IsNumber())
	{
		OutputMsg(env, "EMAschg指定的参数in_block必须是数值类型。");
	}
	else
	{
		if (in_block->GetCount() >= cycle)
		{
			int nRetLen = in_block->GetCount() - cycle + 1;
			int i = 0;
			result = MemBlock::AllocFloat(nRetLen);
			double val = INVALID_MEMBLOCKDATA_FLOAT;
			for (; i < nRetLen; i++)
			{
				for (int j = i, k = 1; k <= cycle; j++, k++)
				{
					val = (2.0f*in_block->GetNumberAt(j) +
						static_cast<double>(k - 1)*result->GetNumberAt(i)) /
						static_cast<double>(k + 1);
					result->SetNumberAt(i, val);
				}
			}
		}
		else
		{
			OutputMsg(env, "EMAschg指定的周期大于输入的数据个数。");
		}
	}
	return result;
}

/**
* @breif 若求X的N日指数平滑移动平均，则表达式为：EMA（X，N），算法是：
		 若Y=EMA(X，N)，则Y=[2*X+(N-1)*Y’]/(N+1)，其中Y’表示上一周期的Y值。
*/
HZ_FORMULA_CAPI MemBlock* EMAsunchg(CEnvironment* env, int current, const MemBlock* in_block, int cycle)
{
	std::string err = "";
	MemBlock* result = nullptr;
	result = arith_emasunchg(current, in_block, INVALID_MEMBLOCKDATA_FLOAT, cycle, err);
	if (result == nullptr && !err.empty())
	{
		OutputMsg(env, err.c_str());
	}
	return result;
}

/**
* @breif *求移动平均值
*        Y=SMA(X,N,M) 算法：Y=[M*X+(N-M)*Y']/N,其中Y'表示上一周期Y值,N必须大于M
*	     SMA(CLOSE,30,1)表示求30日移动平均价 1为权重
*/
HZ_FORMULA_CAPI MemBlock* SMAsunchg(CEnvironment* env, int current, const MemBlock* in_block, int n, int m)
{
	std::string err = "";
	MemBlock* result = arith_smasunchg(current, in_block, INVALID_MEMBLOCKDATA_FLOAT, n, m, err);
	if (result == nullptr && !err.empty())
	{
		OutputMsg(env, err.c_str());
	}
	return result;
}

/**
* @brief  *求最低值
*         LLV(LOW,0)表示求历史最低价
*/
HZ_FORMULA_CAPI MemBlock* LLVs(CEnvironment* env, int current, const MemBlock* in_block, int n)
{
    if (!in_block)
    {
        OutputMsg(env, "输入数据为空!");
        return nullptr;
    }
    MemBlock* result = NULL;
    if (!in_block->IsNumber())
    {
        OutputMsg(env, "LLVs指定的参数in_block必须是数值类型。");
    }
    else
    {
        int nRetLen = in_block->GetCount();
        double tempVal;
        int i = 0;
        result = MemBlock::AllocFloat(nRetLen);
        for (; i < nRetLen; i++)
        {
            if (i == 0)
            {
                result->SetNumberAt(i, in_block->GetNumberAt(i));
            }
            else
            {
                tempVal = in_block->GetNumberAt(i);
                for (int j = i-1,k=0;j>=0 && k < n-1; j--,k++)
                {
                    if (tempVal > in_block->GetNumberAt(j))
                    {
                       tempVal =  in_block->GetNumberAt(j);
                    }
                }
                result->SetNumberAt(i, tempVal);
            }
        }
    }
    return result;
}

/**
* @breif 最高值(HHV)
*/
HZ_FORMULA_CAPI MemBlock* HHVs(CEnvironment* env, int current, const MemBlock* in_block, int n)
{
	if (!in_block)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!in_block->IsNumber())
	{
		OutputMsg(env, "HHVs指定的参数in_block必须是数值类型。");
	}
	else
	{
        int nRetLen = in_block->GetCount();
        double tempVal;
        int i = 0;
        result = MemBlock::AllocFloat(nRetLen);
        for (; i < nRetLen; i++)
        {
            if (i == 0)
            {
                result->SetNumberAt(i, in_block->GetNumberAt(i));
            }
            else
            {
                tempVal = in_block->GetNumberAt(i);
                for (int j = i-1,k=0;j>=0 && k < n-1; j--,k++)
                {
                    if (tempVal < in_block->GetNumberAt(j))
                    {
                       tempVal =  in_block->GetNumberAt(j);
                    }
                }
                result->SetNumberAt(i, tempVal);
            }
        }
	}
	return result;
}


/**
* @breif 前N个对应值(REF)
*/
HZ_FORMULA_CAPI MemBlock* REFs(CEnvironment* env, int current, const MemBlock* in_block, int n)
{
	if (!in_block)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!in_block->IsNumber())
	{
		OutputMsg(env, "HHVs指定的参数in_block必须是数值类型。");
	}
	else
	{
		if (/*in_block->GetCount() - 1 >= n*/ in_block->GetCount()  >= n-1)
		{
			int nRetLen = in_block->GetCount() - n;
			if (nRetLen <= 0)
				nRetLen = 1;
			result = MemBlock::AllocFloat(nRetLen);
			//int nLen = in_block->GetCount();
			//for (int i = nRetLen - 1; i >= 0 && nLen >= 0; i--)
			//{
			//	double tval = in_block->GetNumberAt(--nLen);
			//	result->SetNumberAt(i,tval );
			//}
			MemBlock::Assign(result, 0, in_block, 0, nRetLen);
		}
		else
		{
			OutputMsg(env, "REFs指定的周期必须小于输入的数据减1。");
		}
	}
	return result;
}

/**
* @brief 取较大值(MAX)二数组
*/
HZ_FORMULA_CAPI MemBlock* MAXs(CEnvironment* env, const MemBlock* dataA, const MemBlock* dataB)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "MAXs指定比较的参数必须都是数值类型");
	}else
	{
		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		double num = 0;
		result = MemBlock::AllocFloat(size);
		for (int i = dataA->GetCount() - 1, j = dataB->GetCount() - 1; i >= 0 && j >= 0; i--, j--)
		{
			if (dataA->GetNumberAt(i) - dataB->GetNumberAt(j) > 0)
			{
				num = dataA->GetNumberAt(i);
			}
			else
			{
				num = dataB->GetNumberAt(j);
			}
			result->SetNumberAt(--size, num);
		}
	}
	return result;
}

/**
* @breif 求A和B数值差的绝对值(ABS)
*/
HZ_FORMULA_CAPI MemBlock* ABSs(CEnvironment* env, const MemBlock* dataA, const MemBlock* dataB)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "ABSs指定比较的参数必须都是数值类型");
	}
	else
	{
		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		for (int i = dataA->GetCount() - 1, j = dataB->GetCount() - 1; i >= 0 && j >= 0; i--, j--)
		{
			result->SetNumberAt(--size, abs(dataA->GetNumberAt(i) - dataB->GetNumberAt(j)));
		}
	}
	return result;
}

/**
* @breif 求总和
* 求每n个周期的总和
* SUM(X,N),统计N周期中X的总和
*/
HZ_FORMULA_CAPI MemBlock* SUMs(CEnvironment* env, int current, const MemBlock* in_block, int n)
{
//    if (!in_block)
//    {
//        OutputMsg(env, "输入数据为空!");
//        return nullptr;
//    }
//    MemBlock* result = NULL;
//    if (!in_block->IsNumber())
//    {
//        OutputMsg(env, "SUMs指定的参数必须是数值类型");
//    }
//    else
//    {
//        if (in_block->GetCount() >= n)
//        {
//            int nRetLen = in_block->GetCount() - n + 1;
//            double tSum = 0; int i = 0;
//            result = MemBlock::AllocFloat(nRetLen);
//            for (; i < nRetLen; i++)
//            {
//                if (i == 0)
//                {
//                    for (int j = i; j < i + n; j++)
//                    {
//                        tSum += in_block->GetNumberAt(j);
//                    }
//                }
//                else
//                {
//                    tSum += in_block->GetNumberAt(i + n - 1);
//                    tSum -= in_block->GetNumberAt(i - 1);
//                }
//                result->SetNumberAt(i, tSum);
//            }
//        }
//        else
//        {
//            OutputMsg(env, "SUMs指定的周期大于输入的数据个数。");
//        }
//    }

//    return result;

    if (!in_block)
    {
        OutputMsg(env, "输入数据为空!");
        return nullptr;
    }
    MemBlock* result = NULL;
    if (!in_block->IsNumber())
    {
        OutputMsg(env, "SUMs指定的参数必须是数值类型");
    }
    else
    {
        int nRetLen = in_block->GetCount();
        double tSum = in_block->GetNumberAt(0);
        result = MemBlock::AllocFloat(nRetLen);
        result->SetNumberAt(0,tSum);
        for (int i=1; i < nRetLen; i++)
        {
            if (i >= n-1)
            {
                tSum -= in_block->GetNumberAt(i - n);
            }
            tSum += in_block->GetNumberAt(i);
            result->SetNumberAt(i, tSum);
        }
    }

    return result;
}

/**
* @breif 平均绝对偏差(AVEDEV)
*/
HZ_FORMULA_CAPI MemBlock* AVEDEVs(CEnvironment* env, int current, const MemBlock* in_block, int n)
{
	if (!in_block)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!in_block->IsNumber())
	{
		OutputMsg(env, "AVEDEVs指定的参数必须是数值类型");
	}
	else
	{
		if (in_block->GetCount() >= n)
		{
			int nRetLen = in_block->GetCount() - n + 1;
			MemBlock* maData = MAs(env, current, in_block, n);
			if (maData)
			{
				double tSum = 0.0; int i = 0;
				result = MemBlock::AllocFloat(nRetLen);

				for (; i < nRetLen; i++)
				{
					tSum = 0.0f;
					for (int j = i; j < i + n; j++)
					{
						tSum += abs(in_block->GetNumberAt(j) - maData->GetNumberAt(i));
					}
					result->SetNumberAt(i, tSum / static_cast<double>(n));
				}
				MemBlock::Free(maData);
			}
		}
		else
		{
			OutputMsg(env, "AVEDEVs指定的周期大于输入的数据个数。");
		}
	}
	return result;
}


/**
* @breif SUM 加法
*/
HZ_FORMULA_CAPI MemBlock* SUM(CEnvironment* env, const MemBlock* dataA, const MemBlock* dataB)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "SUBs指定比较的参数必须都是数值类型");
	}else
	{
		//if (dataA->GetCount() == dataB->GetCount())
		//{
		//	int i = 0;
		//	result = MemBlock::Alloc(dataA->GetCount(), INVALID_MEMBLOCKDATA_FLOAT);
		//	for (; i < result->GetCount(); i++)
		//	{
		//		result->SetNumberAt(i, dataA->GetNumberAt(i) + dataB->GetNumberAt(i));
		//	}
		//}
		//else
		//{
		//	OutputMsg(env, "SUBs指定比较的参数的数组元素必须相等");
		//}

		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		for (int i = dataA->GetCount() - 1, j = dataB->GetCount() - 1; i >= 0 && j >= 0; i--, j--)
		{
			result->SetNumberAt(--size, dataA->GetNumberAt(i) + dataB->GetNumberAt(j));
		}
	}
	return result;
}


/**
* @breif SUB 减法
*/
HZ_FORMULA_CAPI MemBlock* SUB(CEnvironment* env, const MemBlock* dataA, const MemBlock* dataB)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "SUBs指定比较的参数必须都是数值类型");
	}else
	{
		//if (dataA->GetCount() == dataB->GetCount())
		//{
		//	int i = 0;
		//	result = MemBlock::Alloc(dataA->GetCount(), INVALID_MEMBLOCKDATA_FLOAT);
		//	for (; i < result->GetCount(); i++)
		//	{
		//		result->SetNumberAt(i, dataA->GetNumberAt(i) - dataB->GetNumberAt(i));
		//	}
		//}
		//else
		//{
		//	OutputMsg(env, "SUBs指定比较的参数的数组元素必须相等");
		//}

		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		for (int i = dataA->GetCount() - 1, j = dataB->GetCount() - 1; i >= 0 && j >= 0;i--,j--)
		{
			result->SetNumberAt(--size, dataA->GetNumberAt(i) - dataB->GetNumberAt(j));
		}
	}
	return result;
}


/**
* @breif MUL 乘法
*/
HZ_FORMULA_CAPI MemBlock* MUL (CEnvironment* env, const MemBlock* dataA, const MemBlock* dataB)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "SUBs指定比较的参数必须都是数值类型");
	}else
	{
		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		for (int i = dataA->GetCount() - 1, j = dataB->GetCount() - 1; i >= 0 && j >= 0; i--, j--)
		{
			result->SetNumberAt(--size, dataA->GetNumberAt(i) * dataB->GetNumberAt(j));
		}

	}
	return result;
}

/**
* @breif DIV 除法
*/
HZ_FORMULA_CAPI MemBlock* DIV(CEnvironment* env, const MemBlock* dataA, const MemBlock* dataB)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "SUBs指定比较的参数必须都是数值类型");
	}else
	{
		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		double lastVal = 0.0;
		for (int i = dataA->GetCount() - 1, j = dataB->GetCount() - 1; i >= 0 && j >= 0; i--, j--)
		{
			if (dataB->GetNumberAt(j) != 0.0)
			{
				lastVal = dataA->GetNumberAt(i) / dataB->GetNumberAt(j);
			} //如果等于0 则使用上一次值填充
			result->SetNumberAt(--size, lastVal);
			
		}
	}
	return result;
}

/** 
 * @brief 单数乘 dataA数组每个值都乘以nVal
 */
HZ_FORMULA_CAPI MemBlock* MULs(CEnvironment* env, const MemBlock* dataA, int nVal)
{
	if (!dataA || nVal == 0)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber())
	{
		OutputMsg(env, "MULs指定比较的参数必须都是数值类型");
	}
	else
	{
		int size = dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		double lastVal = 0.0;
		for (int i = dataA->GetCount() - 1; i >= 0; i--)
		{
			lastVal = dataA->GetNumberAt(i) * nVal;
			result->SetNumberAt(--size, lastVal);
		}
	}
	return result;
}

/** 
 * @brief 单数除法 dataA每个数都除以nVal
 */
HZ_FORMULA_CAPI MemBlock* DIVs(CEnvironment* env, const MemBlock* dataA, int nVal)
{
	if (!dataA || nVal == 0)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber())
	{
		OutputMsg(env, "DIVs指定比较的参数必须都是数值类型");
	}
	else
	{
		int size = dataA->GetCount();
		result = MemBlock::AllocFloat(size);
		double lastVal = 0.0;
		for (int i = dataA->GetCount() - 1; i >= 0; i--)
		{
			lastVal = dataA->GetNumberAt(i) / nVal;
			result->SetNumberAt(--size, lastVal);
		}
	}
	return result;
}

/**
* @brief 两条线交叉

* 两条线交叉
  nType 0表示取左值,1表示取右值
* 用法:
*   CROSS(L,R);表示当L从下方向上穿过R时返回L或R的单值(通过nType确定)，否则返回无效值
*/
HZ_FORMULA_CAPI MemBlock* CROSS(CEnvironment* env, const MemBlock *dataA, const MemBlock *dataB,int nType)
{
	if (!dataA || !dataB)
	{
		OutputMsg(env, "输入数据为空!");
		return nullptr;
	}
	MemBlock* result = NULL;
	if (!dataA->IsNumber() || !dataB->IsNumber())
	{
		OutputMsg(env, "SUBs指定比较的参数必须都是数值类型");
	}
	else
	{
		int i = 0, j = 0;
		if (dataA->GetCount() > dataB->GetCount())
		{
			i = dataA->GetCount() - dataB->GetCount();
			j = 0;
		}
		else if (dataA->GetCount() < dataB->GetCount())
		{
			i = 0;
			j = dataB->GetCount() - dataA->GetCount();
		}
		int size = dataA->GetCount() > dataB->GetCount() ? dataB->GetCount() : dataA->GetCount();
		result = MemBlock::Alloc(size, INVALID_MEMBLOCKDATA_FLOAT);
		double pre_left = 0.0, now_left = 0.0;
		double pre_right = 0.0, now_right = 0.0;
		double bRet = INVALID_MEMBLOCKDATA_FLOAT,bSecValue = INVALID_MEMBLOCKDATA_FLOAT;
		int index = 0;
		for (; i < dataA->GetCount() && j < dataB->GetCount(); i++, j++, index++)
		{
			if (i == 0 || j == 0)
			{
				pre_left = dataA->GetNumberAt(i);
				pre_right = dataB->GetNumberAt(j);
			}
			now_left = dataA->GetNumberAt(i);
			now_right = dataB->GetNumberAt(j);
			bRet = INVALID_MEMBLOCKDATA_FLOAT;
			if (now_left >= now_right && pre_left < pre_right)
			{
				if (nType == 0)
				{
					bRet = now_left;
					if (i + 1 < dataA->GetCount())
					{
						bSecValue = dataA->GetNumberAt(i + 1);
					}
				}
				else if(nType == 1)
				{
					bRet = now_right;
					if (j + 1 < dataB->GetCount())
					{
						bSecValue = dataB->GetNumberAt(j + 1);
					}
				}
				//bRet = now_left;
			}
			pre_left = now_left;
			pre_right = now_right;
			int index = i < j ? i : j;
			if (bRet != INVALID_MEMBLOCKDATA_FLOAT)
			{
				result->SetNumberAt(index, bRet);
				if (index < result->GetCount() - 1 && i < dataA->GetCount()-1 && j < dataB->GetCount()-1)
				{
					now_left = dataA->GetNumberAt(i+1);
					now_right = dataB->GetNumberAt(j+1);
					if (!(now_right >= now_left&&  pre_right<pre_left))
					{
						result->SetNumberAt(index + 1, bSecValue);
					}
				}
			}
			bSecValue = INVALID_MEMBLOCKDATA_FLOAT;
			bRet = INVALID_MEMBLOCKDATA_FLOAT;
		}
	}
	return result;
}
