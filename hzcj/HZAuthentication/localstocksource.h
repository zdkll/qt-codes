/********************************************************************
created:2018/08/29
author: min.cheng dk.zhang
purpose:本地股票管理类，实现自选股和最近浏览等本地记录
*********************************************************************/
#ifndef LOCALSTOCKSOURCE_H
#define LOCALSTOCKSOURCE_H

#include <QVector>
#include <QMap>
#include <memory>
#include "hzauthentication_global.h"

class   HZAUTHENTICATIONSHARED_EXPORT LocalStockSource
{
public:
    LocalStockSource(const QString &fileName);
    ~LocalStockSource();

    static void init();
    static void saveStocks();

    static LocalStockSource *globalZXInstance();//游客自选股

    static LocalStockSource *globalHistInstance(); //浏览历史股票

    static LocalStockSource *getZXInstance(){return m_zxStockSource;}
    static LocalStockSource *getHistInstance(){return m_histStockSource;}

    static void unInit();

    virtual bool updateStock(const QStringList &zxstocks,QString* errString = 0);

    virtual bool addStock(const QString& code,QString *errString = 0);

    virtual bool deleteStock(const QString& code,QString *errString = 0);

    virtual bool isHas(const QString& code){return m_stocks.contains(code);}

    virtual void load();

    virtual void save();

    bool isNeedSaved(){return m_isNeedSaved;}

    QVector<QString> getStocksList(){return m_stocks;}

private:
    QString getFilePath();
    static LocalStockSource  *m_zxStockSource;
    static LocalStockSource  *m_histStockSource;

    bool  m_isNeedSaved; //是否需要保存，股票增删后需重新保存

    QVector<QString>  m_stocks;
    QString                  m_fileName;
};


#endif // LOCALSTOCKSOURCE_H
