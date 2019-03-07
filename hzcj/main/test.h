#ifndef TEST_H
#define TEST_H

#include "datacontroller.h"
#include <string>
#include <QHash>
#include <unordered_map>

#include <hzinfocenter.h>
#include <hzinfodata.h>

class Test
{
public:
    Test();

    void reqStockBatch();

    void reqBkStockBatch();
protected:
    void onStockBatchDataArrived(ReqId  , char *data, int num, ErrCode );
    void onBkStockBatchDataArrived(ReqId  reqId, char *data, int num, ErrCode errCode);

private:
    QFile m_file;
    QTextStream m_outStream;
    HZData::ReqBatchDynaInfo m_reqBatchInfo;

    int m_index = 0;
    QVector<HZData::StockCode> m_stockCodes;
};

#endif // TEST_H
