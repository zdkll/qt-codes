#include "zxstockopt.h"

ZXStockOpt::ZXStockOpt()
{
    m_hzAuthenOpt = HZAuthenOpt::instance();
    m_zxLocalSource = LocalStockSource::globalZXInstance();
}


ZXStockOpt *ZXStockOpt::instance()
{
    static ZXStockOpt zxStockOpt;
    return &zxStockOpt;
}

bool ZXStockOpt::updateZXStock(const QStringList &zxstocks, bool emitSignal, QString *errString)
{
    bool ok = false;
    if(m_hzAuthenOpt->isUserLogged()){
        m_hzAuthenOpt->updateZXStock(zxstocks);
        ok = m_hzAuthenOpt->waitForFinshed();
        //错误信息
        if(!ok && errString)
            *errString = m_hzAuthenOpt->errString();
    }else{
        ok =  m_zxLocalSource->updateStock(zxstocks,errString);
    }
    //发送信号
    if(ok && emitSignal){
        emit stockCountChanged(stockCount());
    }

    return ok;
}

bool ZXStockOpt::addZXStock(const QString& code,bool emitSignal,QString *errString)
{
    bool ok = false;
    if(m_hzAuthenOpt->isUserLogged()){
        m_hzAuthenOpt->addZXStock(code);
        ok = m_hzAuthenOpt->waitForFinshed();
        //错误信息
        if(!ok && errString)
            *errString = m_hzAuthenOpt->errString();
    }else{
        ok =  m_zxLocalSource->addStock(code,errString);
    }
    //发送信号
    if(ok && emitSignal){
        emit stockCountChanged(stockCount());
    }

    return ok;
}

bool ZXStockOpt::delZXStock(const QString& code,bool emitSignal,QString *errString)
{
    bool ok = false;
    if(m_hzAuthenOpt->isUserLogged()){
        m_hzAuthenOpt->delZXStock(code);
        ok = m_hzAuthenOpt->waitForFinshed();
        if(!ok && errString)
            *errString = m_hzAuthenOpt->errString();
    }else{
        ok = m_zxLocalSource->deleteStock(code,errString);
    }
    //发送信号
    if(ok && emitSignal){
        emit stockCountChanged(stockCount());
    }
    return ok;
}

bool ZXStockOpt::isHas(const QString& code)
{
    if(m_hzAuthenOpt->isUserLogged()){
        return m_hzAuthenOpt->isHas(code);
    }else{
        return m_zxLocalSource->isHas(code);
    }
    return false;
}

QVector<QString> ZXStockOpt::getZXStock()
{
    if(m_hzAuthenOpt->isUserLogged()){
        return  m_hzAuthenOpt->getZXStock();
    }
    return  m_zxLocalSource->getStocksList();
}

void  ZXStockOpt::informStkCountChanged()
{
    emit stockCountChanged(stockCount());
}

int ZXStockOpt::stockCount()
{
    if(m_hzAuthenOpt->isUserLogged()){
        return  m_hzAuthenOpt->getZXStock().count();
    }
    return  m_zxLocalSource->getStocksList().count();
}
