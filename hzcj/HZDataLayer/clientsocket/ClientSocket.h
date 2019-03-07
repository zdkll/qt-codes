#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
/********************************************************************
created: 2018
author: juntang.xie
purpose: 网路层  实现连接服务器  断线重连
*********************************************************************/
#include <QObject>

#include <QTcpSocket>

class QTcpSocket;
class HeartBeat;
class ClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    ClientSocket(QObject *parent = 0);

    ~ClientSocket();

    /**
     * @brief 设置服务器ip和端口
     * @param[in]strIP 服务器IP
     * @param[in]nPort 端口
     * @retval 无返回值
     */
    void setServerInfo(QString strIP, int nPort);

    /**
     * @brief 连接服务器
     * @retval 成功返回true,失败返回false
     */
    bool reConnect();

    //不再重连,用户退出了
    void deConnnect();

protected:
    void timerEvent(QTimerEvent *e);

private:
    struct ServerInfo{
        QString ip;
        int         port;
    };

private:
    //重连定时
    int                  m_reConTimerId;

    bool               m_connected;

    ServerInfo   m_serverInfo;
};

#endif // ClientSocket_H
