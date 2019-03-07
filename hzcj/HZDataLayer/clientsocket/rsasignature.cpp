#include "rsasignature.h"




RSASignature::RSASignature()
{

}

RSASignature::~RSASignature()
{

}

int RSASignature::m_nResult = -1;

int RSASignature::m_nPadding = RSA_PKCS1_PADDING;



/**
 * @brief RSASignature::createRSA 载入密钥
 * @param key 密钥
 * @param publi 公钥1 私钥0
 * @return
 */
RSA * RSASignature::createRSA(unsigned char * key,int publi)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        qDebug()<< "Failed to create key BIO";
        return 0;
    }
    if(publi)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        qDebug()<< "Failed to create RSA";
    }
    return rsa;
}

/**
 * @brief RSASignature::public_encrypt 公钥加密
 * @param data 待加密数据
 * @param data_len 待加密的数据长度
 * @param key 公钥
 * @param encrypted 加密后的数据
 * @return 加密长度
 */
int RSASignature::public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    m_nResult = RSA_public_encrypt(data_len,data,encrypted,rsa,m_nPadding);
    return m_nResult;
}


/**
 * @brief RSASignature::private_decrypt 私钥解密
 * @param enc_data 待解密数据
 * @param data_len 待解密数据长度
 * @param key 私钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    int  m_nResult = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,m_nPadding);
    return m_nResult;
}


/**
 * @brief RSASignature::private_encrypt 私钥加密
 * @param data 待加密数据
 * @param data_len 待加密数据长度
 * @param key 私钥
 * @param encrypted 加密后数据
 * @return
 */
int RSASignature::private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    qDebug()<<RSA_size(rsa);
    int m_nResult = RSA_private_encrypt(data_len,data,encrypted,rsa,m_nPadding);
    return m_nResult;
}


/**
 * @brief RSASignature::public_decrypt 公钥解密
 * @param enc_data 待解密数据
 * @param data_len 待解密数据长度
 * @param key 公钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    qDebug()<<RSA_size(rsa);
    int  m_nResult = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,m_nPadding);
    return m_nResult;
}


TAesClass::TAesClass()
{

}

TAesClass::~TAesClass()
{

}

std::string TAesClass::EncodeAES(const std::string &password, const std::string &data)
{
    AES_KEY aes_key;
    if(AES_set_encrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
    {
        assert(false);
        return "";
    }
    std::string strRet;
    std::string data_bak = data;
    unsigned int data_length = data_bak.length();

    int m_nPadding  =  AES_BLOCK_SIZE - data_bak.length() % AES_BLOCK_SIZE;

    data_length += m_nPadding;
    int sp = m_nPadding;

    while (sp > 0)
    {
        data_bak += m_nPadding;
        sp--;
    }
    for(unsigned int i = 0; i < data_length/AES_BLOCK_SIZE; i++)
    {
        std::string str16 = data_bak.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        ::memset(out, 0, AES_BLOCK_SIZE);
        AES_encrypt((const unsigned char*)str16.c_str(), out, &aes_key);
        strRet += std::string((const char*)out, AES_BLOCK_SIZE);
    }
    return strRet;
}

std::string TAesClass::DecodeAES(const std::string &strPassword, const std::string &strData)
{
    AES_KEY aes_key;
    if(AES_set_decrypt_key((const unsigned char*)strPassword.c_str(), strPassword.length() * 8, &aes_key) < 0)
    {
        assert(false);
        return "";
    }
    std::string strRet;
    for(unsigned int i = 0; i < strData.length()/AES_BLOCK_SIZE; i++)
    {
        std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        ::memset(out, 0, AES_BLOCK_SIZE);
        AES_decrypt((const unsigned char*)str16.c_str(), out, &aes_key);
        strRet += std::string((const char*)out, AES_BLOCK_SIZE);
    }


    return strRet;
}
