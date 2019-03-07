/********************************************************************
created:  2018.8
author: juntang.xie
purpose: DataSubCenter 数据处理中心类
*********************************************************************/
#ifndef DATASUBCENTER_H
#define DATASUBCENTER_H

#include <QObject>
#include "datacontroller.h"
class DataSubCenter : public QObject
{
    Q_OBJECT
public:
    explicit DataSubCenter(QObject *parent = 0);

    void startSub(std::list<string> lists);
    void cancelSub();
signals:
    void subStockArrived(char *data, int num);

public slots:

private:
    void onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode);

private:
    ReqId m_nReq;
};

#endif // DATASUBCENTER_H
