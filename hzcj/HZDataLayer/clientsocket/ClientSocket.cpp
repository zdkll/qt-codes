#include "ClientSocket.h"
#include <QTcpSocket>
#include <QTimerEvent>
#include <QDebug>
#include "head.h"
ClientSocket::ClientSocket(QObject *parent)
    :QTcpSocket(parent)
    ,m_reConTimerId(0)
    ,m_connected(false)
{
    m_reConTimerId = this->startTimer(3000);
}

ClientSocket::~ClientSocket()
{
    if(m_reConTimerId>0)
        this->killTimer(m_reConTimerId);

}

void ClientSocket::timerEvent(QTimerEvent *e)
{
    if(m_reConTimerId == e->timerId()){
        if(m_connected && this->state() == QAbstractSocket::UnconnectedState)
            this->connectToHost(m_serverInfo.ip,m_serverInfo.port);
    }
}

void ClientSocket::setServerInfo(QString strIP, int nPort)
{
    m_serverInfo.ip    = strIP;
    m_serverInfo.port = nPort;
}

bool ClientSocket::reConnect()
{
    //如果是连接上的，需要重新连接
    if(this->state() == QAbstractSocket::ConnectedState){
        this->disconnectFromHost();
        //        this->waitForDisconnected(3000);
    }

    this->connectToHost(m_serverInfo.ip,m_serverInfo.port);
    m_connected =  waitForConnected(3000);

    return m_connected;
}

void ClientSocket::deConnnect()
{
    m_connected = false;
    this->disconnectFromHost();
}
