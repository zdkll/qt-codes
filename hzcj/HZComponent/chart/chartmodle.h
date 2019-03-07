#ifndef CHARTMODLE_H
#define CHARTMODLE_H
/********************************************************************
created:  2018/5/3
author: min.cheng
purpose: 用于chart数据储存，非线程安全，公共数据存储，
*********************************************************************/
#include <map>

#include "hzcomponent_global.h"
#include "datacontroller.h"
#include "IndiInterface.h"
#include "hzinfodata.h"


struct KlineData
{
    DataKLine* pData = nullptr;
    IndiCalcData* pInidCalcData = nullptr;
};


class ChartModle
{
public:
    static ChartModle* getInstance();

	~ChartModle();

    //根据周期的数字获取到文本 比如 年K
    QString getStrPeriod(int period);

    //通过股本代码获取股票名称
    QString getObjNameById(const QString& obj);

    //根据股票代码获取交易时间节点  秒数表示
    QVector<double> getTradeNode(const QString& obj);

    //根据股票代码获取X轴分时显示时间字符串 字符表示:  9:30 11:30
    QVector<QString> getMinXaxisStrTimeList(const QString& obj);

    //根据股票代码获取分时一天的根数根数 沪深240根，其他市场各自不同
    int getMinDayCount(const QString& obj);

    //保存复权数据
    void setDrData(const QString& stock,const StockDrList& data);

    //获取复权数据
    StockDrList getDrData(const QString& stock);
private:
    QString getKlinekey(const QString& obj,int period);
    ChartModle();
    QMap<QString,StockDrList> m_drDataMap;
};

#endif // CHARTMODLE_H
