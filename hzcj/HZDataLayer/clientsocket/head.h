/********************************************************************
created: 2018
author: juntang.xie
purpose: 包头结构体定义
*********************************************************************/
#ifndef HEAD_H
#define HEAD_H
#include <iostream>
using namespace std;

#pragma pack (1)
/**
 * @brief Head 包头结构体
 */
typedef struct Head
{
    quint32 unLen;
    quint16 unCmd  ;  //命令类型
    quint8  unVer;       // 当前客户端版本
    quint8  unType;     //设备类型
    quint32 unSeq;     // 消息序列 Id
    quint64 unUid;      //  uid
}HEAD;
#pragma pack ()



/**
 * @brief Body_ 包体结构体
 */
typedef struct Body_{
    unsigned int nLen;   //数据长度
    std::string strMsg;     //数据内容
}BODY;

/**
 * @brief Pack_ 包结构体
 */
typedef struct Pack_{
    HEAD      head;
    unsigned int     nLen;
   std::string strMsg;     //数据内容
}HZPack;




#endif // HEAD_H
