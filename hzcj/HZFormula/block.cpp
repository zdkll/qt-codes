/********************************************************************
created:	十一月 2015
filename: 	public.cpp
author:     min.cheng
purpose:    数据块操作
*********************************************************************/
#include "formula_api.h"
#include "memblock.h"
#include "Environment.h"

/**
* @brief 创建内存块
* @param count 数据个数，-1表示为单值数据
* @retval NULL 申请是失败
* @retval 非NULL 申请成功，返回申请的内存
* @sa MemBlock::Alloc
*
* 创建一定数量数据的内存块，内存块被初始化为无效
*/
HZ_FORMULA_CAPI MemBlock* AllocDoubleMem(int count, double value)
{
	return  MemBlock::Alloc(count, value);
}

HZ_FORMULA_CAPI MemBlock* AllocInt64Mem(int count)
{
	return  MemBlock::Alloc(count, INVALID_MEMBLOCKDATA_INT64);
}

HZ_FORMULA_CAPI MemBlock* AllocStrMem(int count)
{
	return  MemBlock::AllocStr(count, NULL);
}

HZ_FORMULA_CAPI MemBlock* CopyStr(const char* value)
{
	return  MemBlock::Alloc(value);
}

HZ_FORMULA_CAPI const char* GetString(MemBlock *ptr)
{
	if (!ptr)
	{
		return "";
	}
	return ptr->GetString().c_str();
}

HZ_FORMULA_CAPI int GetCount(MemBlock *ptr)
{
	int num = 0;
	if (ptr)
	{
		num = ptr->GetCount();
	}
	return num;
}

HZ_FORMULA_CAPI bool IsString(MemBlock *ptr)
{
	if (!ptr)
	{
		return false;
	}
	return ptr->IsString();
}

HZ_FORMULA_CAPI bool IsNumber(MemBlock *ptr)
{
	if (!ptr)
	{
		return false;
	}
	return ptr->IsNumber();
}

HZ_FORMULA_CAPI double GetNumberAt(MemBlock *ptr,int index)
{
	if (!ptr)
	{
		return 0.0f;
	}
	return ptr->GetNumberAt(index);
}

HZ_FORMULA_CAPI char GetStringAt(MemBlock *ptr, int index)
{
	if (!ptr)
	{
		return ' ';
	}
	return ptr->GetStringAt(index);
}

HZ_FORMULA_CAPI void SetNumberAt(MemBlock* ptr, int index,double value)
{
	if (!ptr)
	{
		return;
	}
	ptr->SetNumberAt(index, value);
}

HZ_FORMULA_CAPI void SetStringAt(MemBlock* ptr, int index,char value)
{
	if (!ptr)
	{
		return;
	}
	ptr->SetStringAt(index, value);
}

HZ_FORMULA_CAPI bool IsNull(MemBlock* ptr)
{
	bool ret = true;
	if (ptr)
		ret = false;
	return ret;
}

/**
* @brief 释放内存块
* @param ptr 内存指针
* @sa MemBlock::Free
*
* 将用malloc方式申请的内存块释放
*/
HZ_FORMULA_CAPI void FreeMem(MemBlock *ptr) 
{
	MemBlock::Free(ptr);
}

#include <qdebug.h>
/**
* @brief 添加数据到输出
* @param env 环境变量
* @param block 需要输出的数据
* @param name 输出数据的名称
* @param attr 输出属性
*
* 添加MemBlock数据到FormulaENV的output
*/
HZ_FORMULA_CAPI void Output(CEnvironment *env, MemBlock *block,const char *name)
{
	MemBlock* pBlock = nullptr;
	if (block)
	{
		pBlock = MemBlock::AllocFloat(block->GetCount());
		MemBlock::Assign(pBlock, 0, block, 0, block->GetCount());
	}
    env->SetData(pBlock, name);
}
