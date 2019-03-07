#pragma execution_character_set("utf-8")
#pragma once
/********************************************************************
created:	八月 2016
filename: 	EnvironmentGroup.h
author:     min.cheng
purpose:	用于指标计算中的单元组,主要功能是用于记录多个指标计算,当全部
            回调以后在统一通知外部客户,方便外部进行同合约同周期的多个指标
			计算方法.
*********************************************************************/
#include "jcformuladef.h"
class CIndiCalcCenter;
class CIndiDataCache;
class CEnvironment;
class CEnvironmentGroup
{
public:
    CEnvironmentGroup(CIndiCalcCenter* pIndiCalcCenter, const MultiIndiReqParam& param, uint64_t id);
	~CEnvironmentGroup();

	uint64_t GetCurrId();

	void Cancle();

	void AsyncCalc();

	bool IsFinished();

	bool IsCancle();
private:
	void CalcCalback(IndiRspResult& ret);
	std::vector<uint64_t> m_pEnvVec;
	uint64_t m_id;
	CIndiCalcCenter* m_pIndiCalcCenter;
    MultiIndiReqParam m_param;
	int m_nCalcedCount;
	std::mutex m_resultMutex;
	std::vector<IndiRspResult> m_rspResultVec;
	std::atomic<bool> m_bFinish;
	std::atomic<bool> m_bCancle;
};

