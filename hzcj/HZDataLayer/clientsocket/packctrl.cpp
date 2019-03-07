#include "packctrl.h"
#include <hzlog.h>
#include <QTcpSocket>
#include <QtEndian>
#include <QTimerEvent>
#include <QTime>
#include <QDebug>
#include "rsasignature.h"
#include "user.pb.h"
#include "stock_code.pb.h"

PackCtrl::PackCtrl(QObject *parent):QObject(parent)
  ,m_socket(new ClientSocket(this)),m_isLogged(false)
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotDataRecv()));

    connect(m_socket,&QTcpSocket::stateChanged,this,&PackCtrl::socketStateChanged);

    m_packing   = false;
    m_datBuffer = 0;

    m_heartBeatId = startTimer(3000);
}

PackCtrl::~PackCtrl()
{
    if(m_heartBeatId>0)
        this->killTimer(m_heartBeatId);
}

void PackCtrl::setServerInfo(QString strIP, int nPort)
{
    m_socket->setServerInfo(strIP,nPort);
}

bool PackCtrl::logIn(quint64 uid,QString accessToken)
{
    m_user.uid = uid;
    m_user.accessToken = accessToken;

    //重新连接登录
    bool ret =  m_socket->reConnect();
    if(!ret)
        pkctrlErrors(m_socket->error(),m_socket->errorString());

    return ret;
}

void  PackCtrl::logOut()
{
    m_user.uid = -1;
    m_user.accessToken.clear();
    m_socket->deConnnect();
}

bool PackCtrl::disConnect()
{
    m_socket->disconnectFromHost();
    return m_socket->waitForConnected(3000);
}

void PackCtrl::timerEvent(QTimerEvent *e)
{
    if(m_heartBeatId == e->timerId() && m_isLogged){
        onHeartBeat();
    }
}

void PackCtrl::sendData(const HZPack &pack)
{
    // rsa加密 pack.head.unCmd = 100
    if(CmdType_FirstLogIn == pack.head.unCmd)
    {
        rsaEncode(pack);
    }
    else
    {   //else  aes加密
        aesEncode(pack);
    }
}

void PackCtrl::onHeartBeat()
{
    Head head;
    head.unLen = sizeof(HEAD);
    head.unCmd = CmdType_HeatBeat;
    head.unVer   = Client_UnVersion;
    head.unType = Client_UnType;
    head.unSeq  =  HeartBeat_Seq;
    head.unUid = m_user.uid;

    m_socket->write((char*)&headToBigEndian(head),sizeof(Head));
}

void PackCtrl::slotDataRecv()
{
    QByteArray byteArray = m_socket->readAll();
    processData(byteArray);
}

void PackCtrl::processData(const QByteArray &byteArray)
{
    int               totalSize       = byteArray.size();
    QByteArray retByteArray = byteArray;

    if(!m_packing){
        //判断数据是否满足包头长度
        if(retByteArray.size()<sizeof(HEAD)){
            m_tmpBuf = retByteArray;
            return;
        }
        if(m_tmpBuf.size()>0){
            retByteArray = m_tmpBuf+byteArray;
            totalSize = retByteArray.size();
            m_tmpBuf.clear();
        }
        //保存包头
        HEAD head;
        memcpy(&head, retByteArray.data(), sizeof(HEAD));
        m_pack.head = headFromBigEndian(head);

        //保存包体
        m_pack.nLen = 0;
        if(!m_datBuffer)
            m_datBuffer = new char[m_pack.head.unLen - sizeof(HEAD)];

        int  bytes = m_pack.head.unLen - sizeof(HEAD);
        bytes = bytes<(totalSize-sizeof(HEAD))?bytes:(totalSize-sizeof(HEAD));
        memcpy(m_datBuffer,retByteArray.data()+sizeof(HEAD),bytes);
        m_pack.nLen = bytes;

        retByteArray = retByteArray.right(totalSize -bytes - sizeof(HEAD));

        m_packing = true;
    }else
    {
        int bytes = m_pack.head.unLen - sizeof(HEAD) - m_pack.nLen;
        bytes = bytes<totalSize?bytes:totalSize;

        memcpy(m_datBuffer+m_pack.nLen,retByteArray.data(), bytes);
        m_pack.nLen +=  bytes;

        retByteArray = retByteArray.right(totalSize - bytes);
    }

    //判断包是否接收完成
    if(m_pack.nLen < (m_pack.head.unLen - sizeof(HEAD)))
        return;

    //包接收完成，组包
    m_pack.strMsg = string(m_datBuffer,m_pack.head.unLen - sizeof(HEAD));
    if(m_datBuffer){
        delete[] m_datBuffer;
        m_datBuffer = 0;
    }
    m_packing = false;

    recvPack(m_pack);

    if(retByteArray.size()>0)
        processData(retByteArray);
}

void PackCtrl::socketStateChanged(QAbstractSocket::SocketState state)
{
    Log("NetLayerLOG","INFO","socket sate changed = %d.",state);
    qDebug() <<"socket sate changed = %d." << state;
    //连接
    if(state == QTcpSocket::ConnectedState)
    {
        resetPack();
        Log("NetLayerLOG","INFO","start first login--------------------------.");
        firstLogin();
    }
    //断开
    else if(state == QTcpSocket::UnconnectedState)
    {
        m_isLogged = false;
        emit sendReay(false);
    }
}

void PackCtrl::resetPack()
{
    m_pack = HZPack();
    m_packing = false;
    if(m_datBuffer)
    {
        delete[] m_datBuffer;
        m_datBuffer = nullptr;
    }
    m_tmpBuf.clear();
}

void PackCtrl::generateRandomNumber()
{
    memset(m_aesKey, '0', 16);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<16; i++)
    {
        m_aesKey[i] = qrand() % 100;
    }
}

void PackCtrl::firstLogin()
{

    generateRandomNumber();
    HZPack pack;

    QuoteProto::FirstConnectRequest obj;

    //    obj.set_aeskey("\x79\x76\x68\x6B\x77\x66\x6E\x68\x72\x65\x73\x63\x6C\x6B\x70\x6E");

    obj.set_aeskey(m_aesKey, 16);

    obj.SerializeToString(&pack.strMsg);

    pack.nLen = pack.strMsg.length();

    pack.head.unLen = pack.nLen + sizeof(HEAD);
    pack.head.unCmd = CmdType_FirstLogIn;
    pack.head.unVer   = Client_UnVersion;
    pack.head.unType = Client_UnType;
    pack.head.unSeq  = FirstLogIn_Seq;
    pack.head.unUid   = m_user.uid;

    sendData(pack);
}

void PackCtrl::userLogin()
{
    HZPack pack;

    QuoteProto::UserLoginRequest request;
    qDebug()<<"user login:"<<m_user.uid<<" token:"<<m_user.accessToken;
    request.set_uid( m_user.uid);
    request.set_token(m_user.accessToken.toStdString());
    request.SerializeToString(&pack.strMsg);

    pack.nLen = pack.strMsg.length();

    pack.head.unLen = pack.nLen + sizeof(HEAD);
    pack.head.unCmd = CmdType_UserLogIn;
    pack.head.unVer   = Client_UnVersion;
    pack.head.unType = Client_UnType;
    pack.head.unSeq  = UserLogIn_Seq;
    pack.head.unUid   = m_user.uid;

    sendData(pack);
}
#include <QDebug>
void PackCtrl::recvPack(const HZPack &pack)
{
    //登录
    if(CmdType_FirstLogIn == pack.head.unCmd)
    {
        //重新连接清除缓存数据
        m_tmpBuf.clear();

        QuoteProto::FirstConnectResponse obj;
        obj.ParseFromString(pack.strMsg);

        QuoteProto::Ret ret = obj.ret();
        int errcode = ret.err_code();

        //服务端解包失败，获取密钥
        if(errcode != 0) {
            Log("NetLayerLOG","ERROR","first login failed,errString = %s.",ret.err_msg().c_str());
            pkctrlErrors( errcode,QString::fromStdString(ret.err_msg()));
        }
        else{
            Log("NetLayerLOG","INFO","first login success !");

            Log("NetLayerLOG","INFO","start user login -------------------");
            userLogin();
        }
    }
    //用户连接验证
    else if(CmdType_UserLogIn == pack.head.unCmd)
    {
        QuoteProto::UserLoginResponse response;

        response.ParseFromString(pack.strMsg);

        QuoteProto::Ret ret = response.ret();
        int errcode = ret.err_code();

        if(errcode != 0) {
            Log("NetLayerLOG","ERROR","user login failed,errString = %s.",ret.err_msg().c_str());
            qDebug() << "NetLayerLOG ERROR user login failed,errString =" << QString(ret.err_msg().c_str());
            pkctrlErrors( errcode,QString::fromStdString(ret.err_msg()));
        }
        else{
            Log("NetLayerLOG","INFO","user login success!");
            m_isLogged = true;
            emit sendReay(true);
        }
    }else if(pack.head.unCmd > 0)
    {
        m_onRecvPack(pack);
        //emit recvData(pack);
    }
}


void PackCtrl::rsaEncode(const HZPack &pack)
{
    unsigned char encrypted[4098]={};

    char publicKey[] = "-----BEGIN PUBLIC KEY-----\n"\
                       "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCW8X2eQAy5v5/PuAX9nuBN337F\n"\
                       "lkxHS3tzwfx9FJTt4pSSgMgvIclKdheoWrxdsOfpGXDTY1wQUXDk820+6Hhvdkdq\n"\
                       "zOG2FGTMjl6ccBDLlyRPkB7YX8jQOaQQym5nJ5urZnTOYiozKRxeOJ9ixRTc2tQU\n"\
                       "auAr4oEWHfXgIGx2UQIDAQAB\n"\
                       "-----END PUBLIC KEY-----\n";

    int encrypted_length= RSASignature::public_encrypt((uchar *)pack.strMsg.c_str(),strlen(pack.strMsg.c_str()),(uchar *)publicKey,(uchar *)encrypted);

    if(encrypted_length == -1)
        return;

    int nHeadLen = sizeof(HEAD);
    int nSize    = nHeadLen+encrypted_length;
    Head head = pack.head;
    head.unLen = nSize;
    head = headToBigEndian(head);

    char *data = new char[nSize];
    memset(data , 0, nSize);

    memcpy(data, (char *)&head, sizeof(HEAD));
    memcpy(data + nHeadLen, (char *)&encrypted, encrypted_length);

    m_socket->write(data, nSize);

    delete []data;
}

HEAD PackCtrl::headFromBigEndian(const HEAD &in_head )
{
    HEAD head;
    head.unLen = qFromBigEndian<quint32>(in_head.unLen);
    head.unCmd = qFromBigEndian<quint16>(in_head.unCmd);
    head.unVer = qFromBigEndian<quint8>(in_head.unVer);
    head.unType = qFromBigEndian<quint8>(in_head.unType);
    head.unSeq = qFromBigEndian<quint32>(in_head.unSeq);
    head.unUid = qFromBigEndian<quint64>(in_head.unUid);
    return head;
}




void PackCtrl::aesEncode(const HZPack &pack)
{
    //    string strpasswd = "\x79\x76\x68\x6B\x77\x66\x6E\x68\x72\x65\x73\x63\x6C\x6B\x70\x6E";


    string strpasswd = string(m_aesKey,16);

    string strmiwen = TAesClass::EncodeAES(strpasswd, pack.strMsg);

    int nHeadLen = sizeof(HEAD);
    int encrypted_length = strmiwen.length();
    int nSize = sizeof(HEAD) + encrypted_length;

    Head head = pack.head;
    head.unLen = nSize;
    head = headToBigEndian(head);

    char *data = new char[nSize];
    memset(data , 0, nSize);
    memcpy(data, (char *)&head, nHeadLen);
    memcpy(data + nHeadLen, strmiwen.c_str(), encrypted_length);
    m_socket->write(data, nSize);

    delete []data;
}


HEAD PackCtrl::headToBigEndian(const HEAD &in_head)
{
    HEAD head;
    head.unLen = qToBigEndian<quint32>(in_head.unLen);
    head.unCmd = qToBigEndian<quint16>(in_head.unCmd);
    head.unVer = qToBigEndian<quint8>(in_head.unVer);
    head.unType = qToBigEndian<quint8>(in_head.unType);
    head.unSeq = qToBigEndian<quint32>(in_head.unSeq);
    head.unUid = qToBigEndian<quint64>(in_head.unUid);
    return head;
}

