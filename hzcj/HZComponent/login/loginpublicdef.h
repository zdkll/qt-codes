#ifndef LOGINPUBLICDEF_H
#define LOGINPUBLICDEF_H

#include <QString>

namespace HZLogIn {
//用户账号
struct User{
    void clear(){userName.clear();password.clear();}
    QString userName;
    QString password;
};

//登录信息
struct LogInConfig{
    bool isRembPwd =false;// 是否记住密码
    bool isAutoLogin =false;//是否自动登录
    //历史登录用户;用户-密码
    QList< HZLogIn::User> loggedUsers;
    User            logUser; //登录账号
};

//登录方式
enum logInType{
    NoneLogIn  = 0,
    GuestLogIn = 1,  //游客
    UserLogIn,         //用户登录
    PhoneMsgLogIn, //手机短信
    EWMLogIn         //二维码登录
};

}


#endif // LOGINPUBLICDEF_H
