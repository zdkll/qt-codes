#include "hzopenssl.h"

#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <string>

#include <openssl/md5.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/aes.h>

QString HZOpenssl::HZMD5(const QString &inputStr)
{
    char *ch = new char[inputStr.size()];
    memcpy(ch,inputStr.toStdString().data(),inputStr.size());

    unsigned char *data =  (unsigned char *)ch;

    unsigned char md[16];
    MD5(data,inputStr.size(),md);

    QString retStr;
    for(int i=0;i<16;i++){
        retStr += QString("%1").arg(md[i],2,16,QChar('0'));
    }

    delete[] ch;
    return retStr;
}


int HZOpenssl::rsaPublicEncrypt(const QString &inputStr,QString &outputStr)
{
    unsigned char publicKey[] = "-----BEGIN PUBLIC KEY-----\n"\
                                " MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCuG53rp3kSO9Fnnp4Y1\n"\
                                "1leyTGyK+6jT68t69LMH8PYXsl1dq7RPRSPBfm8jCGgWhJs0P//ytjwmr0ZVH19\n"\
                                "W7WmB0aETfWCM4kraNh7h4ve7o+/mlhTibugESBVGh1cDreqTexKJZ1Ja2LP\n"\
                                "OQOvsvqWZHHfsi36VkaVbGHQTgJObwIDAQAB\n"\
                                "-----END PUBLIC KEY-----\n";

    RSA *rsa= NULL;
    BIO *keybio;
    if((keybio = BIO_new_mem_buf(publicKey, -1))==NULL){
        return -1;
    }
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    if(!rsa)
        return -2;

    std::string str = inputStr.toStdString();
    int  padding = RSA_PKCS1_PADDING;

    char encrypted[4098];

    int encrypted_length = RSA_public_encrypt(str.length(),(unsigned char *)str.c_str(),(unsigned char *)encrypted,rsa,padding);

    QByteArray byteArray = QByteArray::fromRawData(encrypted,encrypted_length);

    byteArray = byteArray.toBase64();

    outputStr = QString(byteArray);

    BIO_free(keybio);
    return 0;
}


std::string HZOpenssl::aesEncode(const std::string& userKey, const std::string& data)
{
    unsigned char *inputData = NULL;
    unsigned char *encryptData = NULL;

    unsigned char key[AES_BLOCK_SIZE+1];
    unsigned char ivec[AES_BLOCK_SIZE];

    memset(key, 0x00, sizeof(key));
    memcpy(key, userKey.c_str(), AES_BLOCK_SIZE);

    int data_len = data.length();
    int new_data_len = 0;
    if(data_len%AES_BLOCK_SIZE)
        new_data_len =(data_len/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE;
    else
        new_data_len = data_len;

    inputData = (unsigned char *)calloc(new_data_len+1,sizeof(char));

    memcpy(inputData,data.c_str(),data_len);

    encryptData = (unsigned char *)calloc(new_data_len+1,sizeof(char));

    AES_KEY  aes_key;
    if(AES_set_encrypt_key(key,AES_BLOCK_SIZE*8,&aes_key)<0){
        fprintf(stderr, "Unable to set encryption key in AES...\n");
        return "";
    }
    for(int i=0; i<AES_BLOCK_SIZE; i++)
    {
        ivec[i] = 0;
    }

    AES_cbc_encrypt(inputData,  encryptData,
                    new_data_len, &aes_key, ivec, AES_ENCRYPT);

    std::string outStr =  std::string((char*) encryptData,new_data_len);

    free(inputData);
    free(encryptData);

    return outStr;
}

std::string HZOpenssl::aesDecode(const std::string& userKey, const std::string& data)
{
    unsigned char *encryptData = (unsigned char *)calloc(data.length()+1,sizeof(char));
    unsigned char *decryptData = (unsigned char *)calloc(data.length()+1,sizeof(char));

    memcpy(encryptData,data.c_str(),data.length());

    unsigned char ivec[AES_BLOCK_SIZE];
    unsigned char key[AES_BLOCK_SIZE+1];
    memset(key, 0x00, sizeof(key));
    memcpy(key, userKey.c_str(), AES_BLOCK_SIZE);

    AES_KEY aes_key;

    if(AES_set_decrypt_key(key, 128, &aes_key)<0){
        fprintf(stderr, "Unable to set decryption key in AES...\n");
        return "";
    }

    for(int i=0; i<AES_BLOCK_SIZE; i++)
    {//必须要有
        ivec[i] = 0;
    }

    AES_cbc_encrypt((unsigned char *)encryptData, (unsigned char *)decryptData,
                    data.length(), &aes_key, ivec, AES_DECRYPT);

    std::string outStr =  std::string((char*)decryptData);
    free(decryptData);

    return outStr;
}


QString HZOpenssl::strAesEncode(const QString& userKey, const QString& data)
{
    unsigned char *inputData = NULL;
    unsigned char *encryptData = NULL;

    unsigned char key[AES_BLOCK_SIZE+1];
    unsigned char ivec[AES_BLOCK_SIZE];

    memset(key, 0x00, sizeof(key));
    memcpy(key, userKey.toLatin1().data(), AES_BLOCK_SIZE);

    int data_len = data.length();
    int new_data_len = 0;
    if(data_len%AES_BLOCK_SIZE)
        new_data_len =(data_len/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE;
    else
        new_data_len = data_len;

    inputData = (unsigned char *)calloc(new_data_len+1,sizeof(char));

    memcpy(inputData,data.toLatin1().data(),data_len);

    encryptData = (unsigned char *)calloc(new_data_len+1,sizeof(char));

    AES_KEY  aes_key;
    if(AES_set_encrypt_key(key,AES_BLOCK_SIZE*8,&aes_key)<0){
        fprintf(stderr, "Unable to set encryption key in AES...\n");
        return "";
    }
    for(int i=0; i<AES_BLOCK_SIZE; i++)
    {
        ivec[i] = 0;
    }

    AES_cbc_encrypt(inputData,  encryptData,
                    new_data_len, &aes_key, ivec, AES_ENCRYPT);

    QByteArray byteArray = QByteArray::fromRawData((char*) encryptData,new_data_len);

    QString result = byteArray.toBase64();
    free(inputData);
    free(encryptData);

    return result;
}

QString  HZOpenssl::strAesDecode(const QString& userKey, const QString& data)
{
    QByteArray byteArray = QByteArray::fromBase64(data.toLatin1());
    unsigned char *encryptData = (unsigned char *)calloc(byteArray.size()+1,sizeof(char));
    unsigned char *decryptData = (unsigned char *)calloc(data.size()+1,sizeof(char));

    memcpy(encryptData,byteArray.data(),byteArray.size());

    unsigned char ivec[AES_BLOCK_SIZE];
    unsigned char key[AES_BLOCK_SIZE+1];
    memset(key, 0x00, sizeof(key));
    memcpy(key, userKey.toLatin1().data(), AES_BLOCK_SIZE);

    AES_KEY aes_key;

    if(AES_set_decrypt_key(key, 128, &aes_key)<0){
        fprintf(stderr, "Unable to set decryption key in AES...\n");
        return "";
    }

    for(int i=0; i<AES_BLOCK_SIZE; i++)
    {//必须要有
        ivec[i] = 0;
    }

    AES_cbc_encrypt((unsigned char *)encryptData, (unsigned char *)decryptData,
                    data.length(), &aes_key, ivec, AES_DECRYPT);

    std::string outStr =  std::string((char*)decryptData);
    free(encryptData);
    free(decryptData);

    return QString::fromStdString(outStr);
}

