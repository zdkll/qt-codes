#include "hzinfoadapterinterface.h"
#include "vlistmaster.h"

HZInfoAdapterInterface::~HZInfoAdapterInterface()
{
    if(mReqId!=-1)
        HzInfoCenter::getInstance()->cancle(mReqId);
}

void HZInfoAdapterInterface::setVisitor(IHZInfoUpdateObserve *obs)
{
    m_listVis = obs;
}

void HZInfoAdapterInterface::updateList(const QVariantMap &var)
{
    m_listVis->updateList(var);
}

void HZInfoAdapterInterface::abnormalReq(int reqId, int nextpage)
{
    if(reqId == -1) {
        QVariantMap vmap;
        vmap["code"] = reqId;
        vmap["page"] = nextpage;
        updateList(vmap);
    }
}

void HZInfoAdapterInterface::setCode(const QVector<QString> &codeVec)
{
    m_codeVec = codeVec;
}
