#ifndef DATAPROVIDERDEF_H
#define DATAPROVIDERDEF_H

#include "datamodeldef.h"

struct SubReqInfo
{
    ReqId reqId = INVALID_REQID;
    int start = 0;
    int count = 0;

    HZData::StockFieldMask     stock_mask;
    HZData::MoneyFieldMask money_mask;
};


#endif // DATAPROVIDERDEF_H
