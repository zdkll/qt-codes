#ifndef RESPONSERECIEVER_H
#define RESPONSERECIEVER_H

#include  "datactrldef.h"
#include  "datamodeldef.h"

#include  "datactrldef.h"
#include  "./clientsocket/head.h"

//@brief 请求包接收处理类,拆解PACK，将数据按照本地结构体保存，返回给DataController
class ResponseReciever
{
public:
    ResponseReciever(){}

    //设置回调函数,指向DataController接口
    void setRecvCbFunction(OnRecvFunction recvFunc);

    //消息包到达，接收消息，保存在本地数据结构体
    void  onMessageArrived(const HZPack &pack);

    //返回最新接收包的错误信息
    const HZData::Ret &ret()const {return m_ret;}

private:
    void readBatchDyna(ReqId reqId,const std::string& strMsg);
    void readPriceTick(ReqId reqId,const std::string& strMsg);
    void readLeaderIncDecPos(ReqId reqId,const std::string& strMsg);

    void readBiddingKline(ReqId reqId,const std::string& strMsg);

    void readZhibiao(ReqId reqId,const std::string& strMsg);
    void readMidAbnormal(ReqId reqId,const std::string& strMsg);

    void readAbnormalBlock(ReqId reqId,const std::string& strMsg);
private:
    OnRecvFunction  m_recvFunc;
    HZData::Ret        m_ret;
};


#endif // RESPONSERECIEVER_H
