#pragma once
/********************************************************************
created:	三月 2016
filename: 	jcformuladef.h
author:     min.cheng
purpose:	公司结构定义
*********************************************************************/
#include<functional>
#include <map>
#include <vector>
#include <queue>
#include "memblock.h"
#include "IndiInterface.h"

/** 
 * @brief 指标数据缓存结构
 */
struct CacheInfo
{
	std::vector<std::pair<std::string,MemBlock*>> m_vecData;
    CacheInfo(){}
};






