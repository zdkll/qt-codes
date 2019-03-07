/********************************************************************
created:2018/07/25
author: dk.zhang
purpose:自己封装的加解密接口
*********************************************************************/

#ifndef HZOPENSSL_H
#define HZOPENSSL_H

#include <QtCore>
#include "hzauthentication_global.h"

class  HZAUTHENTICATIONSHARED_EXPORT  HZOpenssl
{
public:
    HZOpenssl(){}

    /**
     * @brief  MD5 序列化
     * @param[in]  要加密的字符串
     * @retval   序列化后的字符串
     */
    static QString HZMD5(const QString &inputStr);

    /**
     * @brief  rsa 公钥加密 --公钥为用户登录公钥，需要的话可修改此接口
     * @param[in]  要加密的字符串
     * @param[out]  加密后字符串
     * @retval   0加密成功，其他表示失败
     */
    static int  rsaPublicEncrypt(const QString &inputStr,QString &outputStr);

    /**
     * @brief  aes 对称加解密
     * @param[in]  userKey 加解密钥匙
     * @param[in]  加解密输入
     * @retval  加解密输出
     */
    static std::string  aesEncode(const std::string& userKey, const std::string& data);
    static std::string  aesDecode(const std::string& userKey, const std::string& data);

    /**
     * @brief  aes 对称加解密
     * @param[in]  userKey 加解密钥匙
     * @param[in]  data加解密输入
     * @retval  加解密输出
     */
    static QString strAesEncode(const QString& userKey, const QString& data);
    static QString  strAesDecode(const QString& userKey, const QString& data);
};

#endif // HZOPENSSL_H
