/** 
 * @file urlparameter.h
 * @brief url参数部分 的编解码及解析
 * @sa 参考对象
 * @author zhangzewu 
 * @date 22/11/2012
 *
 * 以"&"作为分隔符，以第一个"="连接左边的Key和右边的Value, Key不能为空, 区分大小写
 * 
 * 对非数字和非字母等非安全字符 进行编码，每一个字符转换为%开头+两个16进制数字
 * 如：
 * < : %3C
 * > : %3E
 * . : %22
 * + : %20
 * ? : %3F
*/

#pragma once

#include <map>
#include <string>
#include "buffer.h"
#include <functional>
#include <vector>
#include "hzcore_global.h"

#define KEY_SPLIT '&'
#define VALUE_SPLIT '='

	/*/
	1. js_escape
					保留内容:	*，+，-，.，/，@，_，0-9，a-z，A-Z

	2. encodeURI, Uri.EscapeUriString
					保留内容:	!，#，$，&，'，(，)，*，+，,，-，.，/，:，;，=，?，@，_，~，0-9，a-z，A-Z
	
	3. encodeURIComponent, Uri.EscapeDataString
					保留内容:	!，'，(，)，*，-，.，_，~，0-9，a-z，A-Z
	
	4. HttpUtility.UrlEncode
					保留内容:	!()*-._ 其中' '转为'+'

	5. HttpUtility.UrlPathEncode 仅转码如下内容
	%00%01%02%03%04%05%06%07%08%09%0a%0b%0c%0d%0e%0f%10%11%12%13%14%15%16%17%18%19%1a%1b%1c%1d%1e%1f%20

	======================================
	Uri.EscapeUriString 默认用RFC2396，若启用国际资源标识符 (IRIs)或国际域名 (IDN) 分析则用3986
	RFC3986

	unreserved	= ALPHA | DIGIT | "-" | "." | "_" | "~"
	reserved	= gen-delims | sub-delims
	gen-delims	= ":" | "/" | "?" | "#" | "[" | "]" | "@"
	sub-delims	= "!" | "$" | "&" | "'" | "(" | ")" |
				  "*" | "+" | "," | ";" | "="

	mark		= "-" | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")"
	/*/

	//////////////////////////////////////////////////////////////////////////
	// 需要使用标准方法，请使用CUrlUtil方法，关传入eUrlParseType类型，类型的含义如下：
	// eUrlParseType_none：对所有字符进行%xx编码
	// eUrlParseType_alphanum：除字母，数字外，所有其他字符进行%xx编码
	// eUrlParseType_3986_url：RFC3986标准编码，功能等同于js中的encodeURI()，C#中的Uri.EscapeUriString()方法
	// eUrlParseType_3986_data：RFC3986标准编码，功能等同于js中的encodeURIComponent()，C#中的Uri.EscapeDataString()方法
	// eUrlParseType_js_escape: 功能等同于js中escape()方法
	// 
	// 需要使用分段组装Url串，以KEY_SPLIT和VALUE_SPLIT作为Key，Value的分隔符，请使用CUrlParameter。
	// CUrlParameter在编码时使用CUrlUtil中的方法，可以通过SetParseFunc来修改CUrlParameter中默认的编码方法
	//////////////////////////////////////////////////////////////////////////

enum eUrlParseType
{
    eUrlParseType_digit				= 0x0001,	//0-9
    eUrlParseType_low_alpha			= 0x0002,	//a-z
    eUrlParseType_high_alpha		= 0x0004,	//A-Z

    eUrlParseType_unreserved_plus	= 0x0008,	//"-" | "." | "_" | "~"
    eUrlParseType_mark_plus			= 0x0010,	//"!" | "*" | "'" | "(" | ")"
    eUrlParseType_gen_3986			= 0x0020,	//"[" | "]"
    eUrlParseType_gen_delims_plus	= 0x0040,	//":" | "/" | "?" | "#" | "@"
    eUrlParseType_gen_delims		= eUrlParseType_gen_3986 | eUrlParseType_gen_delims_plus,
    eUrlParseType_sub_delims		= 0x0080,	//"!" | "$" | "&" | "'" | "(" | ")" | "*" | "+" | "," | ";" | "="

    eUrlParseType_js_escape_plus	= 0x0100,	//* + - . / @ _
    eUrlParseType_none				= 0x0000,	//全部进行转码

    eUrlParseType_alphanum		= eUrlParseType_digit | eUrlParseType_low_alpha | eUrlParseType_high_alpha,	//字母数据
    eUrlParseType_unreserved	= eUrlParseType_alphanum | eUrlParseType_unreserved_plus,
    eUrlParseType_reserved		= eUrlParseType_gen_delims | eUrlParseType_sub_delims,
    eUrlParseType_mark			= eUrlParseType_unreserved_plus | eUrlParseType_mark_plus,	//"-" | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")"

    eUrlParseType_3986_url		= eUrlParseType_unreserved | eUrlParseType_gen_delims_plus | eUrlParseType_sub_delims,		//UrlEncode	//!，#，$，&，'，(，)，*，+，,，-，.，/，:，;，=，?，@，_，~，
    eUrlParseType_3986_data		= eUrlParseType_alphanum | eUrlParseType_mark,				//DataEncode
    eUrlParseType_js_escape		= eUrlParseType_alphanum | eUrlParseType_js_escape_plus,	//js_escape
};

class  HZCORESHARED_EXPORT CUrlParameter
{
public:
    //Url编码函数
    typedef std::function<std::string(const std::string&)> typeUrlParseFun;
    //返回Buf值
    typedef buffer::buffer_ptr buffer_ptr;
    //内部存储
    typedef std::map<std::string,std::string> keyvalue_map_tpye;


    /**
    * @brief 构造函数，构造CUrlParameter
    * @param[in] urlParameter 传入的url参数部分的字符串
    * @param[in] bDecode 传入的参数是否需要解码
    * @param[in] bHttpUtility 编码、解码方法是否需要与HttpUtility.UrlEncode一致，true时使用CUrlUtil::EncodeForHttpUtility，CUrlUtil::DecodeForHttpUtility
    * @remark 传入的url参数部分的字符串解析，若bDecode为true，则解析后要解码保存
    */
    CUrlParameter(const std::string& urlParameter=std::string(), bool bDecode=false, bool bHttpUtility = false);
    ~CUrlParameter();

    /**
    * @brief 设置默认的编码、解码方法
    * @param[in] fun_encode 编码方法
    * @param[in] fun_decode 解码方法
    * @remark CUrlParameter在编码时使用CUrlUtil中的方法，可以通过当前函数来修改CUrlParameter中默认的编码方法
    */
    void setParseFunc(typeUrlParseFun fun_encode, typeUrlParseFun fun_decode)
    {
        m_fun_encode = fun_encode;
        m_fun_decode = fun_decode;
    }

    /**
    * @brief 判断Url中的Key是否存在
    * @param[in] key 要判断的Key值
    * @retval true Key已经存在
    * @retval false Key不存在
    * @remark 判断Url中的Key是否存在，区分大小写
    */
    bool isKeyExsit(const std::string& key);

    /**
    * @brief 添加Key-Value参数
    * @param[in] key 要添加的Key值
    * @param[in] value 要添加的Value值
    * @remark 添加Key-Value参数，若已存在，则进行覆盖，Key区分大小写
    */
    void setString(const std::string& key, const std::string& value=std::string());

    /**
    * @brief 添加Key-Value参数,Value为一个内存块
    * @param[in] key 要添加的Key值
    * @param[in] Buf 要添加的内存块
    * @param[in] nBufLen 要添加的内存块的数据大小
    * @remark 添加Key-Value参数,Value为一个内存块，若Key已存在，则进行覆盖，Key区分大小写
    */
    void setBuffer(const std::string& key, const char* Buf, int nBufLen);

    /**
    * @brief 删除一对Key-Value值
    * @param[in] key 要删除的Key
    * @remark 删除一对Key-Value值，即两个KEY_SPLIT（"&"）之间的串
    */
    void delKey(const std::string& key);

    /**
    * @brief 读取指定Key的Value值
    * @param[in] key 要读取的Key值
    * @param[out] value 返回的Value值
    * @retval true Key存在，读取成功
    * @retval false Key不存在，读取失败
    * @remark 读取指定Key的Value值，返回false时Value值无效，且不产生影响，Key区分大小写
    */
    bool getString(const std::string& key, std::string& value);

    /**
    * @brief 读取指定Key的Value值到指定的buffer中
    * @param[in] key 要读取的Key值
    * @retval buffer_ptr Key存在，读取成功, 则其中包含Buffer和长度,否则为空
    * @remark 读取指定Key的Value值到指定的buffer中，buffer_ptr为std::shared_ptr<buffer>，Key区分大小写
    */
    buffer_ptr getBuffer(const std::string& key);

    /**
    * @brief 合并产生当前整个urlParameter字符串返回
    * @retval std::string urlParameter字符串，可能为空
    * @remark 合并产生当前整个urlParameter字符串返回，若无Key-Value时为空
    */
    std::string getUrlParameter();

    /**
    * @brief 追加另一个urlParameter字符串到当前串中
    * @param[in] url 要追加进来的url字符串
    * @param[in] bDecode 传入的参数是否需要解码
    * @retval 0 一个也没有追加成功
    * @retval >0 追加成功的Key-Value个数
    * @remark 追加另一个urlParameter字符串到当前串中，返回追加成功的Key-Value个数，若Key为空，则自动忽略，Key区分大小写
    * 若bDecode为true，则解析后要解码保存
    */
    int addUrlParameter(const std::string& urlParameter, bool bDecode=false);


    /**
    * @brief 追加另一个urlParameter字符串到当前串中
    * @param[in] url 要追加进来的url字符串
    * @retval 0 一个也没有追加成功
    * @retval >0 追加成功的Key-Value个数
    * @remark 追加另一个CUrlParameter到当前串中，返回追加成功的Key-Value个数，若Key为空，则自动忽略，Key区分大小写
    */
    int addUrlParameter(const CUrlParameter& urlParameter);


    /**
    * @brief Key-Value对的遍历，
    * @retval map<string>::begin()
    * @remark Key-Value对的遍历，map<string>::begin()，非线程安全，遍历过程中不应该发生数据修改
    */
    keyvalue_map_tpye::const_iterator begin();

    /**
    * @brief Key-Value对的遍历，
    * @retval map<string>::end()
    * @remark Key-Value对的遍历，map<string>::end()，非线程安全，遍历过程中不应该发生数据修改
    */
    keyvalue_map_tpye::const_iterator end();

private:
    //字符串截取
    void baseStrSplit(const std::string u, const std::string string, std::vector<std::string>& vector);
    keyvalue_map_tpye m_valuemap;
    typeUrlParseFun m_fun_encode;
    typeUrlParseFun m_fun_decode;
};

class HZCORESHARED_EXPORT CUrlUtil
{
public:
    ///编码
    static std::string Encode(const std::string &URL, eUrlParseType uType = eUrlParseType_alphanum);
    ///解码，解码时单个进行，%符号后面两个字符都为16进制字符时才符合解码规则
    static std::string Decode(const std::string &URL);

    //HttpUtility.UrlEncode 保留内容 !()*-._ 其中' '转为'+'
    static std::string EncodeForHttpUtility(const std::string &URL);
    //HttpUtility.UrlDecode 其中'+'转为' '
    static std::string DecodeForHttpUtility(const std::string &URL);
};
