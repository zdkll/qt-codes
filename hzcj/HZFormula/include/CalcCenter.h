#pragma once
/********************************************************************
created:	十一月 2015
filename: 	CalcCenter.h
author:     min.cheng
purpose:	公式对外计算接口类
*********************************************************************/
#include "hzformula_global.h"
#include <string>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include "IndiInterface.h"

class ThreadPool;
class CEnvironment;
class IRCodeCache;
class CEnvironmentGroup;

class HZFORMULASHARED_EXPORT CIndiCalcCenter
{
public:
    static 	CIndiCalcCenter* getInstance();

	~CIndiCalcCenter();

	/** 
	 * @brief 异步计算指标公式
	 * @param[in] calcUnit 计算参数
	 * @retval 返回计算id
	 */
    bool AsynCalc(const IndiReqParam& param, int64_t& id);

	/**  
	 * @brief 添加指标组的异步计算,同时输入过个同合约同周期的指标信息,计算完成后统一回调
	 * @param[in] param指标组参数信息
	 * @param[out] 返回任务Id
	 * @retval
	 */
    bool AsynCalcGroup(const MultiIndiReqParam& param,int64_t& id);

	/** 
	 * @brief 取消异步指标计算
	 * @param[in] id 取消的任务id号
	 * @retval 始终返回true
	 */
	bool Cancle(int64_t id);

	/** 
	 * @brief 取消单组请求
	 */
	bool CancleSingleCalc(int64_t id);
	/** 
	 * @brief 同步计算指标
	 * @param[in] calcUnit 计算参数
	 * @param[in] retInfo 返回执行过程中的信息
	 * @retval 同步计算完成返回true，否则返回false
	 */
	bool SynCalc(const  IndiReqParam& calcUnit,  IndiRspResult& retInfo);

private:
    CIndiCalcCenter();
	std::string GetScritpByPath(const std::string& path);
	std::unordered_map<int64_t, std::shared_ptr<CEnvironment>> m_mapId; //id-->bool是否执行
	std::unordered_map<int64_t, std::shared_ptr<CEnvironmentGroup>> m_multiMapId; //id-->bool是否执行
	std::mutex m_mapidMutex;
	std::mutex m_groupMapidMutex;
	std::atomic<int64_t> m_nIdValue;
	std::shared_ptr<IRCodeCache> m_pIrcache;
};
