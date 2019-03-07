#ifndef STRATEGYDYNAPROVIDER_H
#define STRATEGYDYNAPROVIDER_H

#include <QMap>
#include <QVector>
#include <QObject>
#include "datacontroller.h"

class QTimer;
class DynaDataProvider : public QObject
{
    Q_OBJECT
public:
    DynaDataProvider(QObject *parent=Q_NULLPTR);

    void addsubstock(const QString& stock);

    void addsubstock(const QVector<QString>& stocks);

    void removestock(const QString& stock);

    void removestock(const QVector<QString>& stocks);

    void removestockAll();

    HZData::AllStock getStockData(const QString& stock);

    void startTimer();

    void stopTimer();

signals:
    void signalUpdateData();
public slots:
    void slotsTimer();
private:
    struct subItem
    {
        int reqId;
        HZData::AllStock data;
        int reqTimes = 1;
    };
    void onStockOptionalDataArrived(ReqId reqId, char *data,int num, ErrCode errCode);
    QMap<QString,subItem> m_dataMap;
    QTimer* m_pTimer = nullptr;
    bool m_bDataChanged = true;
};
#endif // STRATEGYDYNAPROVIDER_H
