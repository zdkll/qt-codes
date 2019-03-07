#ifndef BASEINFODEF_H
#define BASEINFODEF_H

#include "datamodelpublic.h"

//===================协议返回==============================
namespace HZData
{
//搜索返回股票信息
struct CodeInfo
{
    std::string	type ;				//类型
    std::string	code ;				//代码
    std::string	name ;		  //名称
};

struct UserLoginKickInfo {
    UINT64  		        uid = INVALID_UINT64;				//用户uid
    DeviceType          device_type ;		//用户登录的设备type
    std::string             kick_desc ;	//剔除玩家描述
};


//==================  请求=====================================

//搜索请求-返回 CodeInfo
struct ReqQueryCode
{
    HZData::SearchType  search_type;
    std::string            search_word;
};
}

#endif // BASEINFODEF_H
