#pragma once
/********************************************************************
created:	十一月 2015
filename: 	Environment.h
author:     min.cheng
purpose:	公式运行时环境
*********************************************************************/
#include "memblock.h"
#include "jcformuladef.h"
#include <atomic>
#include <mutex>


class CIndiDataCache;
class  CEnvironment
{
public:
    CEnvironment(const  IndiReqParam& calc_unit, int64_t id, const std::string& script);

	~CEnvironment();

	int64_t GetCurrId();

	bool Cancle();

	bool IsCancle();

	std::string GetScript();

	const IndiReqParam& GetInitCalcUnitParam() const;

	void PreCalc();

	void EndCalc(IndiRspResult& retObj, bool isAsync = true);
	/**
	 * @brief 临时存储多个输出结果,用于指标计算完成后统一赋值
	 */
	void SetData(MemBlock *block, const char *name);
private:
	std::atomic<bool> m_bClearResult;  //指标计算后,出现了错误,需要清除指标缓存
	int64_t m_id;
	IndiReqParam m_calcParam;
	std::atomic<bool> m_bCancle;
	std::string m_script;
	std::vector<std::pair<std::string, MemBlock*>> m_vecData;//计算缓存数据
};
