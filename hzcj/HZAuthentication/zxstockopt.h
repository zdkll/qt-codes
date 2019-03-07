/********************************************************************
created:
author: dk.zhang
purpose:自选股管理类
*********************************************************************/
#ifndef ZXSTOCKOPT_H
#define ZXSTOCKOPT_H

#include "localstocksource.h"
#include "hzauthenopt.h"

class HZAUTHENTICATIONSHARED_EXPORT ZXStockOpt : public QObject
{
    Q_OBJECT
public:
    ZXStockOpt();

    static ZXStockOpt *instance();

    //以下接口都是同步接口
    bool updateZXStock(const QStringList &zxstocks,bool emitSignal = true,QString *errString = 0);//添加自选股
    bool addZXStock(const QString& code,bool emitSignal = true,QString *errString = 0);//添加自选股
    bool delZXStock(const QString& code,bool emitSignal = true,QString *errString = 0);//删除自选股
    bool isHas(const QString& code);//判断是否是自选股
    //返回自选股
    QVector<QString>  getZXStock();

    //手动通知自选股改变
    void  informStkCountChanged();
signals:
    //自选股数量改变信号/增删
    void stockCountChanged(const int count);

private:
    int stockCount();

    HZAuthenOpt       *m_hzAuthenOpt;
    LocalStockSource *m_zxLocalSource;
};

#endif // ZXSTOCKOPT_H
