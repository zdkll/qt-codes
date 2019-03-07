#ifndef NAVISTOCKPROVIDER_H
#define NAVISTOCKPROVIDER_H

#include "datamodeldef.h"
#include "navistockdynamodel.h"
#include "dataproviderdef.h"

//导航栏行情数据管理-定时拉取排行
class NaviStockProvider : public QObject
{
    Q_OBJECT
public:
    NaviStockProvider(NaviStockDynaModel *model,QObject *parent = 0);

    virtual void actived(bool enable =  true);
    virtual void setStockCode(const QString &stockCode);

    virtual void setFilterFlag(int filterFlag);
    virtual void sortIndicatorChanged(SortField sortField,bool is_desc);
    virtual void currentRangeChanged(int start,int count);

protected slots:
    void startSub();
    void slotSubTimeOut();

protected:
    virtual void reqBatchStock();
    virtual void subData();
    void           cancelSub();

    void onBatchDynaArrived(ReqId,const char  *data,int num, ErrCode);

protected:
    NaviStockDynaModel  *m_dataModel;
    bool                              m_actived;
    bool                              m_initialed ;

    ReqId                                       m_reqBatchId = INVALID_REQID;
    HZData::ReqBatchDynaInfo      m_reqBatchInfo;
    SubReqInfo                               m_subReqInfo;

    //当前显示范围
    std::pair<int,int>          m_curRange;
    QString                       m_currentStock;
private:
    QTimer             m_t;
};


//固定股票范围，自选股/历史浏览
class NaviOptionStockProvider : public NaviStockProvider
{
    Q_OBJECT
public:
    NaviOptionStockProvider(NaviStockDynaModel *model,QObject *parent = 0)
        :NaviStockProvider(model,parent),m_rowCount(0){}

    void actived(bool enable =  true);

    void setFilterFlag(int ){}
    void sortIndicatorChanged(SortField ,bool ){}
    void currentRangeChanged(int ,int ){}

signals:
    void rowCountChanged(int count);

protected:
    void    subOptionalStock();
    virtual QVector<QString> getStockVector() = 0;

    void onOptionalStockArrived(ReqId,const char *data,int num,ErrCode errCode);

    int    m_rowCount;
};

//自选股
class NaviZXStockProvider : public NaviOptionStockProvider
{
    Q_OBJECT
public:
    NaviZXStockProvider(NaviStockDynaModel *model,QObject *parent = 0);

protected slots:
    void slotStkCountChanged(int count);

protected:
    QVector<QString> getStockVector();
};

//历史浏览
class NaviHistStockProvider : public NaviOptionStockProvider
{
public:
    NaviHistStockProvider(NaviStockDynaModel *model,QObject *parent = 0)
        :NaviOptionStockProvider(model,parent){}

protected:
    QVector<QString> getStockVector();
};



//指数
class NaviIndexStockProvider : public NaviStockProvider
{
public:
    NaviIndexStockProvider(NaviStockDynaModel *model,QObject *parent = 0);

    virtual void setStockCode(const QString &stockCode);

    //active 确定
    void  actived(bool enable = true);

    bool isOptionalIndex(){return m_isOptionalStock;}

    void currentRangeChanged(int start,int count);

protected:
    void subOptionalIndex();
    void onOptionalStockArrived(ReqId reqId,const char *data,int num,ErrCode errCode);

private:
    QVector<QString>  m_indexStocks;
    //切换页面时，当前指数是否在18个指数范围类
    bool                       m_isOptionalStock;
};

//板块导航栏-排行订阅显示
typedef NaviStockProvider NaviBlockStockProvider;

#endif // NAVISTOCKPROVIDER_H
