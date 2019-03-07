#include "CalcCenter.h"
#include "EnvironmentGroup.h"
#include "Environment.h"

#include <mutex>

CEnvironmentGroup::CEnvironmentGroup(CIndiCalcCenter* pIndiCalcCenter, const MultiIndiReqParam& param, uint64_t id)
{
	m_bFinish = false;
    m_nCalcedCount = param.Calcinfo.size();
	m_param = param;
	m_id = id;
	m_pIndiCalcCenter = pIndiCalcCenter;
	m_bCancle = false;
}


CEnvironmentGroup::~CEnvironmentGroup()
{
}

uint64_t CEnvironmentGroup::GetCurrId()
{
	return m_id;
}

void CEnvironmentGroup::Cancle()
{
    for (unsigned int i = 0; i < m_pEnvVec.size();i++)
	{
		m_pIndiCalcCenter->CancleSingleCalc(m_pEnvVec[i]);
	}
	m_bCancle = true;
}

void CEnvironmentGroup::AsyncCalc()
{
	auto pfun = std::bind(&CEnvironmentGroup::CalcCalback,this, std::placeholders::_1);
	int64_t id = 0;
    for (unsigned int i = 0; i < m_param.Calcinfo.size(); i++)
	{
		IndiReqParam temp;
        temp.Calcinfo = m_param.Calcinfo[i];
		temp.pfun = pfun;
        temp.ptrData = m_param.ptrData;
		if (m_pIndiCalcCenter->AsynCalc(temp, id))
		{
			m_pEnvVec.push_back(id);
		}
	}
}

bool CEnvironmentGroup::IsFinished()
{
	return m_bFinish;
}

bool CEnvironmentGroup::IsCancle()
{
	return m_bCancle;
}

void CEnvironmentGroup::CalcCalback(IndiRspResult& ret)
{
	{
        std::lock_guard<std::mutex> loker(m_resultMutex);
		m_rspResultVec.push_back(ret);
		if (--m_nCalcedCount <= 0)
		{
            m_param.pfun(m_rspResultVec);
			m_bFinish = true;
		}
	}	
}
