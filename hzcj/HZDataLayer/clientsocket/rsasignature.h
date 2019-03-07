/********************************************************************
created: 2018
author: juntang.xie
purpose: 封装openssl进行rsa和aes加解密
*********************************************************************/

#ifndef RSASIGNATURE_H
#define RSASIGNATURE_H
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <QDebug>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include "../hzdatalayer_global.h"
using namespace std;

class  HZDATALAYERSHARED_EXPORT RSASignature
{
public:
    RSASignature();
    ~RSASignature();
    static RSA * createRSA(unsigned char * key,int publi);
    /**
     * @brief
     * @param[in] data 要加密的数据
     * @param[in] data_len 加密数据长度
     * @param[in] key 加密公钥
     * @param[out] encrypted 加密后的数据
     * @retval 失败返回0， 成功返回飞0值
     */
    static int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted);
    static int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);
    static int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted);
    static int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);

private:
    static int m_nResult;
    static int m_nPadding;    //支持填充的列表 RSA_PKCS1_PADDING ：PKCS＃1 v1.5填充。这是目前使用最广泛的模式。
};


class TAesClass
{
public:
    TAesClass();
    ~TAesClass();
    /**
     * @brief
     * @param[in] password 加密密钥
     * @param[in] data 要加密的数据
     * @retval 返回加密后的数据
     */
    static std::string EncodeAES( const std::string& password, const std::string& data );            //AES 加密数据
    static std::string DecodeAES( const std::string& strPassword, const std::string& strData );            //AES 解密数据

};


#endif // RSASIGNATURE_H
