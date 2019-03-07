#include "Environment.h"
CEnvironment::CEnvironment(const  IndiReqParam& calc_unit, int64_t id, const std::string& script)
{
	m_script = script;
	m_calcParam = calc_unit;
	m_id = id;
	m_bCancle = false;
	m_bClearResult = false;
}

CEnvironment::~CEnvironment()
{
}

int64_t CEnvironment::GetCurrId()
{
	return m_id;
}

bool CEnvironment::Cancle()
{
	m_bCancle = true;//设置取消标记
	return true;
}

bool CEnvironment::IsCancle()
{
	return m_bCancle;
}

std::string CEnvironment::GetScript()
{
	return m_script;
}

 const IndiReqParam& CEnvironment::GetInitCalcUnitParam() const
{
	return m_calcParam;
}

void CEnvironment::PreCalc()
{
}

void CEnvironment::EndCalc(IndiRspResult& retObj, bool isAsync)
{
    IndiRspResult& ret = retObj;
    if (m_bClearResult) //计算错误就删除结果
    {
        ret.errId = FOMULA__SINGOUT_FAILD;
    }
    if (ret.errId == FOMULA_SUCESS)
    {
        for (unsigned int i = 0; i < m_vecData.size(); i++)
        {
            IndiData indi;
            indi.pBlock = m_vecData[i].second;
            indi.name = m_vecData[i].first;
            ret.vecOutput.push_back(indi);
        }
    }
    if (!m_bCancle && isAsync)
    {
        m_calcParam.pfun(ret);
    }
}

void CEnvironment::SetData(MemBlock *block, const char *name)
{
    if (block && block->IsNumber() && block->GetCount() > 0)
    {
        m_vecData.push_back(std::make_pair(std::string(name), block));
    }
}


