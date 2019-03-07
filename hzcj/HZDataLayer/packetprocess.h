/********************************************************************
created:2018/04/20
author: dk.zhang
purpose: 负责发送请求 RequestSender -封包发送，接收ResponseReciever-解包
*********************************************************************/
#ifndef PACKETPROCESS_H
#define PACKETPROCESS_H

#include <atomic>
#include <functional>
#include <map>
#include <memory>

#include <QtCore>
#include <QObject>

#include  "datamodeldef.h"
#include  "clienthds.h"
#include  "SyncQueue.hpp"
#include  "datactrldef.h"

#include "requestsender.h"
#include "responsereciever.h"

// @brief 包处理器-中介作用，给DataController 调用
class PacketProcessor : public QObject
{
    Q_OBJECT
public:
    PacketProcessor(NetIoReadCallBack fun,QObject *parent = 0);
    ~PacketProcessor();

    //设置服务器
    void setServer(const QString &ip,const int &port);
    //用户登录数据服务器
    bool logIn(quint64 uid,QString acessToken);
    //退出登录
    void logOut();
    //最新接收包错误代码
    const ErrCode &errCode() const {return m_reciever->ret().err_code;}
    //最新接收包错误信息
    const string &errString(){return m_reciever->ret().err_msg;}

      RequestSender* sender(){return m_sender;}
      ResponseReciever* reciever(){return m_reciever;}

protected slots:
    void msgArrived(const HZPack &pack);
    void slotSendReady(bool);

private:
    NetIoReadCallBack    m_netioReadFun;
    ErrCode                     m_errCode;
    QString                      m_errString;
    PackCtrl                    *m_packCtrl;
    RequestSender         *m_sender;
    ResponseReciever    *m_reciever;
};

#endif // PACKETPROCESS_H
