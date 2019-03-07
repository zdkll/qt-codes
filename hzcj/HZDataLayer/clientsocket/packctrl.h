/********************************************************************
created:2018/4/20
author: juntang.xie  dk.zhang
purpose:Socket连接控制，登录以及包的发送接收控制类
*********************************************************************/
#ifndef PACKCTRL_H
#define PACKCTRL_H
//第一次连接和用户登录CMD
#define CmdType_FirstLogIn         100
#define CmdType_UserLogIn         1001
//发送心跳包的CMD，自定义的
#define CmdType_HeatBeat           0
//PC客户端 Version,Type
#define Client_UnVersion      1
#define Client_UnType           1
//第一次连接、用户登录和心跳包的Seq，本地自定义
#define FirstLogIn_Seq           1
#define UserLogIn_Seq           2
#define HeartBeat_Seq           3

#include<functional>
#include<QObject>

#include "ClientSocket.h"
#include "head.h"

typedef std::function<void (const HZPack &)>  OnRecvPack;

/**
  * @brief 发送和接收包操作控制类
  **/
class PackCtrl: public QObject
{
    Q_OBJECT
public:
    PackCtrl(QObject* parent = 0);
    ~PackCtrl();

    /**
     * @brief 设置接到包的回调函数，接收到一个完整的包之后会调用此函数
     * @param[in] func 回调函数
     * @retval   void
     */
    void setOnRecvPack(OnRecvPack &func){m_onRecvPack = func;}

    /**
     * @brief 设置服务器IP和端口号
     * @param[in] strIP 服务器IP
     * @param[in] nPort 端口号
     * @retval   void
     */
    void setServerInfo(QString strIP, int nPort);

    /**
     * @brief 登录数据服务器，在用户登录界面登录成功后会调用此接口
     * @param[in] uid     用户ID
     * @param[in] acessToken  Token
     * @retval    返回是否连接成功
     */
    bool logIn(quint64 uid,QString acessToken);

    void  logOut();
    /**
     * @brief 断开连接
     * @retval
     */
    bool disConnect();

    /**
     * @brief 发送数据请求包,实现包加密发送
     * @param[in] pack  已经组装好的数据包
     */
    void sendData(const HZPack &pack);

protected:
    void timerEvent(QTimerEvent *e);

Q_SIGNALS:
//    void  recvData(const HZPack &pack);

    //param[out] isReady 判断是否可以发送数据
    void sendReay(bool isReady);

    //错误
    void pkctrlErrors(int errorCode,QString errString);

private slots:
    void onHeartBeat();

    void slotDataRecv();

    void socketStateChanged(QAbstractSocket::SocketState);

private:
    void resetPack();
    void firstLogin();
    void userLogin();

    void recvPack(const HZPack &pack);
    void processData(const QByteArray &byteArray);

    void rsaEncode(const HZPack &pack);
    void aesEncode(const HZPack &pack);
    HEAD headToBigEndian(const HEAD &head);

    HEAD headFromBigEndian(const HEAD &in_head);

    void generateRandomNumber();

    struct UserInfo{
        quint64 uid;
        QString accessToken;
    };
private:
    UserInfo          m_user;
    ClientSocket  *m_socket;
    bool                 m_isLogged;
    OnRecvPack   m_onRecvPack;

    int                     m_heartBeatId;

    //接收包
    HZPack              m_pack;
    bool                 m_packing ;
    //接收一个包的缓存数据
    char                *m_datBuffer;
    //长度小于head的buff
    QByteArray      m_tmpBuf;
    char                 m_aesKey[16];
};

#endif // PACKCTRL_H
