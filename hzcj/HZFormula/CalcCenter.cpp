#include "CalcCenter.h"
#include <iostream>
#include <fstream>
#include "Compiler.h"
#include "Runtime.h"
#include "Environment.h"
#include "ThreadPool.hpp"
#include "jcformuladef.h"
#include "EnvironmentGroup.h"

#include <QFile>

CIndiCalcCenter::CIndiCalcCenter()
{
	m_nIdValue = -1;
	m_pIrcache = std::shared_ptr<IRCodeCache>(new IRCodeCache);
}

CIndiCalcCenter *CIndiCalcCenter::getInstance()
{
    static CIndiCalcCenter instance;
    return &instance;
}

CIndiCalcCenter::~CIndiCalcCenter()
{
}

bool CIndiCalcCenter::AsynCalc(const IndiReqParam& param, int64_t& id)
{
	bool ret = false;
    if (param.ptrData->pTime == nullptr || param.ptrData->pTime->GetCount() <=  0)
	{
		return false;
	}
    std::string  text = m_pIrcache->GetScript(param.Calcinfo.base_info.name);
	if (text.empty())
	{
		//获取运行脚本为空
		return ret;
	}
	id = m_nIdValue++;
	std::shared_ptr<CEnvironment> pEnv(new CEnvironment(param, id, text));

	std::weak_ptr<CEnvironment> wkEnv = pEnv;
	{
		std::lock_guard<std::mutex> guard(m_mapidMutex);
		m_mapId.emplace(pEnv->GetCurrId(), pEnv);
	}

	//包装任务函数
	auto f = [wkEnv,this]()
	{
		std::shared_ptr<CEnvironment> spEnv = wkEnv.lock();
		if (spEnv && !spEnv->IsCancle()) //没有取消
		{
			spEnv->PreCalc(); //计算前预处理
			IndiRspResult retObj;
			retObj.Calcinfo = spEnv->GetInitCalcUnitParam().Calcinfo;
			retObj.taskId = spEnv->GetCurrId();
			std::string name = spEnv->GetInitCalcUnitParam().Calcinfo.base_info.name;
#pragma region LUA运行
			CCompiler compiler(m_pIrcache);
			IRCodePtr codePtr = compiler.Compile(spEnv, retObj.errId);
			if (codePtr)
			{
                CRuntime runtime(spEnv);
                if (runtime.Run(codePtr->bytecode, codePtr->size, name, retObj.msg))
                {
                    retObj.errId = FOMULA_SUCESS;
                }
                else
                {
                    retObj.errId = FOMULA_FAILD;
                }
			}
			else
			{
				retObj.errId = FOMULA_COMPILE_FAILD;
			}
#pragma endregion 

#pragma region 本地C++ 代码执行
//            std::shared_ptr<IArith> arithPtr = m_pArithFactory->Create(spEnv.get());
//            if (arithPtr)
//            {
//                //test.Begin();
//                if (arithPtr->Calc())
//                {
//                    retObj.errId = FOMULA_SUCESS;
//                }
//                else
//                {
//                    retObj.errId = FOMULA_FAILD;
//                }
//                //test.End(desc + "lua执行");
//            }
//            else
//            {
//                retObj.errId = FOMULA_COMPILE_FAILD;
//            }
#pragma endregion 
			spEnv->EndCalc(retObj);//计算完成处理
		}
		if (spEnv)
		{
			//清除任务记录
			{
				std::lock_guard<std::mutex> guard(m_mapidMutex);
				auto it = m_mapId.find(spEnv->GetCurrId());
				if (it != m_mapId.end())
				{
					m_mapId.erase(it);
				}
			}

			{
				std::lock_guard<std::mutex> lock(m_groupMapidMutex);
				for (auto it = m_multiMapId.begin(); it != m_multiMapId.end();)
				{
					if (it->second->IsFinished())
					{
						it = m_multiMapId.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}
	};//end

	Task task;
	task.pfun = f;
	ThreadPool::GetInstance()->AddTask(task);
	ret = true;
	return ret;
}


bool CIndiCalcCenter::AsynCalcGroup(const MultiIndiReqParam& param, int64_t& id)
{
    if (param.ptrData->pTime == nullptr || param.ptrData->pTime->GetCount() <=  0 || nullptr == param.pfun)
    {
        return false;
    }
	id = m_nIdValue++;
	std::shared_ptr<CEnvironmentGroup> pEnv(new CEnvironmentGroup(this, param, id));
	if (pEnv)
	{
		{
			std::lock_guard<std::mutex> lock(m_groupMapidMutex);
			m_multiMapId.emplace(id, pEnv);
		}
		pEnv->AsyncCalc();

	}
	return true;
}

bool CIndiCalcCenter::Cancle(int64_t id)
{
	if (m_nIdValue == -1)
	{
		return false;
	}
	bool ret = false;
	{
		std::lock_guard<std::mutex> lock(m_groupMapidMutex);
		auto it = m_multiMapId.find(id);
		if (it != m_multiMapId.end())
		{
			it->second->Cancle();
			m_multiMapId.erase(it);
			ret = true;
		}
	}
	return ret;
}


bool CIndiCalcCenter::CancleSingleCalc(int64_t id)
{
	if (m_nIdValue == -1)
	{
		return false;
	}
	bool ret = false;
	{
		std::lock_guard<std::mutex> guard(m_mapidMutex);
		auto it = m_mapId.find(id);
		if (it != m_mapId.end())
		{
			it->second->Cancle();
			ret = true;
		}
	}
	return ret;
}
bool CIndiCalcCenter::SynCalc(const  IndiReqParam& calcUnit, IndiRspResult& retInfo)
{
	bool ret = false;
	std::string text = m_pIrcache->GetScript(calcUnit.Calcinfo.base_info.name);
	std::shared_ptr<CEnvironment> spEnv(new CEnvironment(calcUnit, m_nIdValue++, text));
	retInfo.Calcinfo = spEnv->GetInitCalcUnitParam().Calcinfo;
	if (!text.empty())
	{
		CCompiler compiler(m_pIrcache);
		IRCodePtr codePtr = compiler.Compile(spEnv, retInfo.errId);
		if (codePtr)
		{
			CRuntime runtime(spEnv);
			ret = runtime.Run(codePtr->bytecode, codePtr->size, calcUnit.Calcinfo.base_info.name, retInfo.msg);
			if (ret)
			{
				retInfo.errId = FOMULA_SUCESS;
				spEnv->EndCalc(retInfo,false);
			}
			else
			{
				retInfo.errId = FOMULA_FAILD;
			}
		}
	}
	//retInfo.
	return ret;
}

std::string CIndiCalcCenter::GetScritpByPath(const std::string& path)
{
    std::string strRet;
    if(path.empty() == false)
    {
        QFile file(path.c_str());
        if(file.open(QIODevice::ReadOnly))
        {
            QString script = file.readAll();
            strRet = script.toStdString();
        }
    }
    return strRet;
}

