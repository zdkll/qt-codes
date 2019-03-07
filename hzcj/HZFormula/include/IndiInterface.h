#pragma once
/********************************************************************
created:	三月 2016
filename: 	interface.h
author:     min.cheng
purpose:	对外数据接口
*********************************************************************/
#include "memblock.h"
#include "cfgdef.hpp"
#include "datamodeldef.h"
#include <functional>
#include <vector>
#include <QMap>
#define  FOMULA_SUCESS 0
#define  FOMULA_FAILD 1          //lua 运行错误
#define  FOMULA__SINGOUT_FAILD 2 //luaVM 计算结果异常
#define  FOMULA_COMPILE_FAILD 3
#define  FOMULA_CALC_CANCLE 4


/**
* @brief K线纵向数据
*/
struct IndiCalcData
{
    IndiCalcData()
    {
        setNull();
    }

    ~IndiCalcData()
    {
        //释放数据块
        Release();
    }

    IndiCalcData* Clone()
    {
        IndiCalcData* pOther = new IndiCalcData;
        if (pId)
        {
            pOther->pId = MemBlock::SubBlock(pId, 0, pId->GetCount());
        }
        if (pTime)
        {
            pOther->pTime = MemBlock::SubBlock(pTime, 0, pTime->GetCount());
        }
        if (pHigh)
        {
            pOther->pHigh = MemBlock::SubBlock(pHigh, 0, pHigh->GetCount());
        }
        if (pLow)
        {
            pOther->pLow = MemBlock::SubBlock(pLow, 0, pLow->GetCount());
        }
        if (pClose)
        {
            pOther->pClose = MemBlock::SubBlock(pClose, 0, pClose->GetCount());
        }
        if (pVolume)
        {
            pOther->pVolume = MemBlock::SubBlock(pVolume, 0, pVolume->GetCount());
        }
        if (pAmount)
        {
            pOther->pAmount = MemBlock::SubBlock(pAmount, 0, pAmount->GetCount());
        }
        if (pTickCount)
        {
            pOther->pTickCount = MemBlock::SubBlock(pTickCount, 0, pTickCount->GetCount());
        }
        if (pOpen)
        {
            pOther->pOpen = MemBlock::SubBlock(pOpen, 0, pOpen->GetCount());
        }
        return pOther;
    }

    void Release()
    {
        if (pId)
        {
            MemBlock::Free(pId);
            pId = nullptr;
        }
        if (pTime)
        {
            MemBlock::Free(pTime);
            pTime = nullptr;
        }
        if (pHigh)
        {
            MemBlock::Free(pHigh);
            pHigh = nullptr;
        }
        if (pLow)
        {
            MemBlock::Free(pLow);
            pLow = nullptr;
        }
        if (pClose)
        {
            MemBlock::Free(pClose);
            pClose = nullptr;
        }
        if (pVolume)
        {
            MemBlock::Free(pVolume);
            pVolume = nullptr;
        }
        if (pAmount)
        {
            MemBlock::Free(pAmount);
            pAmount = nullptr;
        }
        if (pTickCount)
        {
            MemBlock::Free(pTickCount);
            pTickCount = nullptr;
        }
        if (pOpen)
        {
            MemBlock::Free(pOpen);
            pOpen = nullptr;
        }
    }

    //加了type目的是为了效率,不需要的字段就不重置
    bool Resize(int64_t size)
    {
        if (pTime == nullptr)
        {
            //公共必填字段
            pId = MemBlock::Alloc(size,0.0);
            pTime = MemBlock::Alloc(size, 0.0);

            pHigh = MemBlock::Alloc(size, 0.0);
            pLow = MemBlock::Alloc(size, 0.0);
            pClose = MemBlock::Alloc(size, 0.0);
            pOpen = MemBlock::Alloc(size, 0.0);
            pVolume = MemBlock::Alloc(size, 0.0);
            pAmount = MemBlock::Alloc(size, 0.0);
            pTickCount = MemBlock::Alloc(size, 0.0);
        }
        else
        {
            //公共必填字段
            pId = MemBlock::Resize(pId, size);
            pTime = MemBlock::Resize(pTime, size);

            pHigh = MemBlock::Resize(pHigh, size);
            pLow = MemBlock::Resize(pLow, size);
            pClose = MemBlock::Resize(pClose, size);
            pOpen = MemBlock::Resize(pOpen, size);
            pVolume = MemBlock::Resize(pVolume, size);
            pAmount = MemBlock::Resize(pAmount, size);
            pTickCount = MemBlock::Resize(pTickCount, size);
        }
        return true;
    }

    MemBlock* pId;
    MemBlock* pTime;
    MemBlock* pHigh;
    MemBlock* pLow;
    MemBlock* pClose;
    MemBlock* pOpen;
    MemBlock* pVolume;
    MemBlock* pAmount;
    MemBlock* pTickCount;
 private:
    void setNull()
    {
        pId = nullptr;
        pTime = nullptr;
        pHigh = nullptr;
        pLow = nullptr;
        pClose = nullptr;
        pVolume = nullptr;
        pAmount = nullptr;
        pTickCount = nullptr;
        pOpen = nullptr;
    }
};

/**
* @breif 计算指标基本信息
* @author min.cheng
*/
struct IndiCalcBaseInfo
{
    IndiItemInfo base_info;   //指标基本配置信息
	std::string objName;        //当前计算合约名
    int  period;     //当前计算字符串周期值
	int type;                   //计算图区域类型

};

enum IndiDataType
{
    IndiDataSignal = 0,//单个列表
    IndiDataList = 0,//数组
};

/**
* @brief 指标单个输出数据结构
*/
struct IndiData
{
    IndiDataType type = IndiDataSignal;
	std::string name; //输出
	MemBlock* pBlock; //输出数据
    QVector<MemBlock*> blockList;
	IndiData() :pBlock(nullptr), name("")
	{

	}

	~IndiData()
	{
		name = "";
	}

	/** 
	 * @brief 释放内存块
	 */
	void ReleaseBlcok()
	{
		if (pBlock)
		{
			MemBlock::Free(pBlock);
			pBlock = nullptr;
		}
        for(int i=0;i < blockList.size();i++)
        {
            MemBlock::Free(blockList[i]);
            blockList[i] = nullptr;
        }
        blockList.clear();
		name = "";
	}

	/** 
	 * @brief 对象深拷贝
	 */
	void CopyData(const IndiData& other)
	{
        ReleaseBlcok();
		//深拷贝
		if (other.pBlock)
		{
			if (other.pBlock->IsFloat())
			{
				pBlock = MemBlock::Alloc(other.pBlock->GetCount(), INVALID_MEMBLOCKDATA_FLOAT);

			}
			else if (other.pBlock->IsInt64())
			{
				pBlock = MemBlock::Alloc(other.pBlock->GetCount(), INVALID_MEMBLOCKDATA_INT64);
			}
			else if (other.pBlock->IsString())
			{
				pBlock = MemBlock::AllocStr(other.pBlock->GetCount(), "");
			}
			MemBlock::Assign(pBlock, 0, other.pBlock, 0, other.pBlock->GetCount());
		}
        MemBlock* pB = nullptr;
        for(int i=0;i < other.blockList.size();i++)
        {
            if (other.blockList[i]->IsFloat())
            {
                pB = MemBlock::Alloc(other.blockList[i]->GetCount(), INVALID_MEMBLOCKDATA_FLOAT);
            }
            else if (other.blockList[i]->IsInt64())
            {
                pB = MemBlock::Alloc(other.blockList[i]->GetCount(), INVALID_MEMBLOCKDATA_INT64);
            }
            else if (other.blockList[i]->IsString())
            {
                pB = MemBlock::AllocStr(other.blockList[i]->GetCount(), "");
            }
            MemBlock::Assign(pB, 0, other.blockList[i], 0, other.blockList[i]->GetCount());
            blockList.push_back(pB);
        }
	}
};

/** 
 * @brief 指标请求返回结果
 */
struct IndiRspResult
{
	int errId;
	int taskId;
	std::string msg;
	IndiCalcBaseInfo Calcinfo;
	std::vector<IndiData> vecOutput;
	IndiRspResult()
	{
		errId = FOMULA_FAILD;
	}
};
/** 
 * @brief 请求回调
 */
using IndiCallBack = std::function<void(IndiRspResult&)>;

/** 
 * @brief  指标请求参数
 */
struct IndiReqParam
{
	IndiCalcBaseInfo Calcinfo;                       //指标配置数据信息
    std::shared_ptr<IndiCalcData> ptrData;                               //数据
    IndiCallBack pfun;                               //成功后的回调，同步运算忽略
    IndiReqParam() : pfun(nullptr){}
};

typedef std::shared_ptr<IndiReqParam> PtrIndiReqParam;

/**
* @brief 指标组计算请求回调
*/
using MultiIndiCallBack = std::function<void(std::vector<IndiRspResult>&)>;

/**
* @brief  指标请求请求参数 外部拆分好的K线数据
*/
struct MultiIndiReqParam
{
    std::vector<IndiCalcBaseInfo> Calcinfo;                //数据信息
    MultiIndiCallBack pfun;                               //成功后的回调，同步运算忽略
    std::shared_ptr<IndiCalcData> ptrData;

};

