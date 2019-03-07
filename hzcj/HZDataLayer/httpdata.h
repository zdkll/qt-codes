#pragma once
#include "gwdefine.h"
#include "hzdatalayer_global.h"
#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <unordered_map>
#include "httpclientdef.h"
#include "datamodeldef.h"

#include <QMap>

typedef struct _filterDetail
{
    int filterflag = -1;     //标记
    QString stock_code;          //代码 非板块忽略该字段
    QString stock_name;          //名称
    QMap<QString,HZData::StockTable> stockMap;
    QVector<HZData::StockTable> stockVec;
}FilterDetail;

struct BlockFormatDataItem
{
    QString firstChar;
    QVector<HZData::StockTable> dataList;
};

class HttpClient;
class HZDATALAYERSHARED_EXPORT Httpdata:public QObject
{
    Q_OBJECT
protected:
    Httpdata();
    ~Httpdata();
public:

static Httpdata *instance()
    {
        static Httpdata obj;
        return &obj;
    }

    /**
     * @brief 初始化数据
     * @retval
     */
    void init();

    /**
     * @brief  获取排行对应的数据量
     * @param[in] filter 过滤标记
     * @param[in] bkCode 板块code  不为空，表示获取filter下面的bkCode下面的成分股数量 否则忽略
     * @retval  成功返回对应数量，否则返回 -1
     */
    int getCountByFiter(int filter,QString bkCode="");

    /**
     * @brief 查询当前股票或板块是否属于某个分类
     * @param[in] obj 板块ID或股票ID
     * @param[in] filter 查找分类
     * @param[in]
     * @retval
     */
    bool isHasObjFromFilter(const QString& obj,int filter);

    /**
     * @brief 根据filter获取对应列表
     * @param[in] filter
     * @param[in] bkCode 板块code  不为空，表示获取filter下面的bkCode下面的成分股 否则忽略
     * @retval  失败返回空
     */
    void  getListMapByFilter(QMap<QString,HZData::StockTable>& out,int filter,QString bkCode="");
    void  getListVecByFilter(QVector<HZData::StockTable>& out,int filter,QString bkCode="");

    /**
     * @brief 根据股票代码获取filter标记 依次查找响应filter 查找成功则返回对应的filter
     *        查询顺序： 板块列表----沪深列表----指数列表
     * @param[in] 股票代码
     * @retval  获取失败返回-1
     */
     int getFilterByCode(const QString& code);

     /**
     * @brief 根据股票代码获取股票名称  查询顺序： 沪深列表----板块列表----指数列表
     * @param[in] 股票代码
     * @retval 股票名称，否则返回空
     */
     QString getCodeNameByCode(const QString& code);

     //从股票列表查找
     QString getCodeNameByStocks(const QString& code);
     //指定从板块找
     QString getCodeNameByCodeFromBk(const QString& code);

     //指定从指数找
     QString getCodeNameByCodeFromIndex(const QString& code);
     /**
     * @brief filter 转换成分类名称
     * @param[in] filter
     * @retval
     */
     QString filterCodeToName(int filter);

    /**
     * @brief  获取当前股票代码所在板块列表
     * @param[out] out 返回当前股票所在的所有板块列表
     * @param[in] stock 股票代码
     * @retval  返回板块列表
     */
     void getBkListByStock(QVector<FilterDetail>& out,const QString& stock);

    /**
     * @brief 获取板块所属的filter 判断是否属于 行业 地区 概念分类
     * @param[in] code 板块代码
     * @retval 查找不到返回-1
     */
    int getfilterByBkCode(const QString& bkCode);

    /**
     * @brief 获取按照首字符分类的板块列表 比如 AB  DC 分类
     * @param[in] filter 分类filter  比如 概念板块 行业板块 地区板块
     * @retval
     */
    QVector<BlockFormatDataItem> getFormatDataFromBkList(int filter);

    /**
     * @brief  重要指数列表
     */
    QMap<QString,QString> getImpIndexStockMap();
    QVector<QString> getImpIndexStockVec();
protected:
    void loadFromHttp();
    void readCountData();
    void readPlateData();
    void readIndexData();
    void readStockData();
    void readIndexStockChildList();

    void  onCountDataArrived(qint64 id,const QByteArray &array);
    void  onPlateDataArrived(qint64 id,const QByteArray &array);
    void  onIndexDataArrived(qint64 id,const QByteArray &array);
    void  onStockDataArrived(qint64 id,const QByteArray &array);
    void  onIndexStockChildList(qint64 id,const QByteArray& array);

    void preLoade(int filter);
    void loadCountData();
    void loadPlateData();
    void loadIndexData();
    void loadStockData();
    void loadIndexStockChildList();

private slots:
    void finished();
signals:
    void initReady(bool isReady);

private:
    QMap<int,int> filterCountMap_;           //排行数量映射表 key=filter value=count
    QMap<QString,int> bkListCountMap_;       //板块成分股数量映射表 key=bkcode value=count
    QMap<QString,int> bkCodefilterMap_;      //板块代码filter映射表 key=code val=filter 表示当前板块是属于 概念，行业，地区中的哪个

    QMap<int,FilterDetail> filterListMap_;   //排行列表映射表 key=filter value = FilterDetail
    QMap<QString,FilterDetail>  bkMap_;      //板块映射表
    QVector<FilterDetail> bkVec_;            //板块数据
    QMap<QString,QVector<FilterDetail>> bkSearchCache_; //股票代码和板块列表映射，用于搜索后缓存记录，避免持续查找 不包括全部
    QMap<int,QVector<BlockFormatDataItem>> m_blockFormatDataMap; //格式化好的板块列表

    QMap<QString,QString> m_impStocksMap; //key 股票代码  value股票代码 用于查询
    QVector<QString> m_impStockListVec;

    //指数成分股
    QMap<QString,FilterDetail> indexStockChildCodeMap; //指数对应成分股 key=指数代码  vlaue=成分股

    HttpClient *m_httpClient;
    HttpClientCallBack m_funCount;
    HttpClientCallBack m_funPlate;
    HttpClientCallBack m_funIndex;
    HttpClientCallBack m_funStock;

    bool m_bisReady = false;
    bool m_bFlgCount = false;
    bool m_bFlgPlate = false;
    bool m_bFlgIndex = false;
    bool m_bFlgIndexStockList = false;
    bool m_bFlgStock = false;

    int m_reqCountId = -1;
    int m_reqPlaterId = -1;
    int m_reqIndexId = -1;
    int m_reqstockId = -1;
    int m_reqIndexStockChildList = -1;
    QString m_strAppPath;

};



