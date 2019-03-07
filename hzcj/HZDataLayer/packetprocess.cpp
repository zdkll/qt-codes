#include "packetprocess.h"

#include <QDebug>
#include "protobufs.h"
#include "datasimulator.h"


//PacketProcessor-----------------------------------------------
PacketProcessor::PacketProcessor(NetIoReadCallBack fun,QObject *parent  )
    :QObject(parent),m_netioReadFun(fun)
    ,m_packCtrl(new PackCtrl(this))
    ,m_sender(new RequestSender(m_packCtrl))
    ,m_reciever(new ResponseReciever)
{
    m_errCode = 0;
//    ,m_sender(new RequestSender(m_packCtrl))
//    ,m_reciever(new ResponseReciever())
    OnRecvPack  onRecvPack= std::bind(&ResponseReciever::onMessageArrived,m_reciever,std::placeholders::_1);
    m_packCtrl->setOnRecvPack(onRecvPack);

//    connect(m_packCtrl,&PackCtrl::recvData,this,&PacketProcessor::msgArrived);
    connect(m_packCtrl,&PackCtrl::sendReay,this,&PacketProcessor::slotSendReady);
}

PacketProcessor::~PacketProcessor()
{
    delete m_sender;
    delete m_reciever;
}

void  PacketProcessor::setServer(const QString &ip,const int &port)
{
    m_packCtrl->setServerInfo( ip, port);
}

bool PacketProcessor::logIn(quint64 uid,QString acessToken)
{
    m_sender->setUid(uid);
    return  m_packCtrl->logIn(uid,acessToken);
}

void PacketProcessor::logOut()
{
    m_packCtrl->logOut();
}

void PacketProcessor::msgArrived(const HZPack &pack)
{
    m_reciever->onMessageArrived(pack);
}
void PacketProcessor::slotSendReady(bool ready)
{
    m_sender->setSendReady(ready);
    //回调函数
    m_netioReadFun(ready);
}



