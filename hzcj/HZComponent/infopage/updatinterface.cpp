#include "updatinterface.h"
#include <QVariantMap>
#include <QDebug>


void IUpdateRequest::setItemId(const QString &id)
{
    m_curId = id;
}

void IUpdateRequest::setVisitor(IUpdatePerformer *performer)
{
    mAbstractPer = performer;
}

void IUpdateRequest::abnormalReq(int reqId, int nextpage)
{
    if(reqId == -1) {
        QVariantMap vmap;
        vmap["code"] = reqId;
        vmap["page"] = nextpage;
        updateList(vmap);
    }
}

void IUpdateRequest::updateList(const QVariantMap &var)
{
    qDebug() << Q_FUNC_INFO;
    mAbstractPer->updateList(var);
}

void IUpdateRequest::updateDetails(const QVariant &var)
{
    mAbstractPer->updateDetails(var);
}
