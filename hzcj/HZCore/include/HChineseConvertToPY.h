#ifndef HChineseConvertToPY_H__
#define HChineseConvertToPY_H__

/********************************************************************
	created:	2014/08/13
	created:	13:8:2014   17:37
	file base:	HChineseConvertToPY
	file ext:	h
	author:		CHE
	
	purpose:	汉字转换成拼音，只提供静态方法。
				每个汉字的拼音音节之间用空格隔开
*********************************************************************/
#include <map>
#include "hzcore_global.h"

using std::map;
typedef char __In_char;
typedef char __Out_char;
//本结构体将不作内存管理
struct Exchar{
	Exchar(__In_char *_d = NULL, int size = 0):data(_d),length(size){}
	~Exchar(){}
	char *data;
	int length;
};
class HZCORESHARED_EXPORT HChineseConvertToPY
{
public:
	/*	purpose: 返回汉字的拼音字符串，字符串编码为ASCII码，拼音音节之间以空格隔开，并以'\0'结尾
	 *	params:
	 *	return: 返回result字符串的长度(不包含'\0')
	 *			ascii：传入ASCII码以'\0'结尾的字符串
	 *			size：若不给定长度，将会调用strlen()
	 *			result: 返回结果，传入的时候不要给它值，在函数内部将会用new操作给它开辟空间
	 */
	static size_t convert(const __In_char *ascii, __Out_char *&result, int size = -1);

	/*	purpose: 返回汉字的拼音首字母的字符串，字符串编码为ASCII码，并以'\0'结尾
	 *	params:
	 *	return: 返回result字符串的长度(不包含'\0')
	 *			ascii：传入ASCII码以'\0'结尾的字符串
	 *			size：若不给定长度，将会调用strlen()
	 *			result: 返回结果，传入的时候不要给它值，在函数内部将会用new操作给它开辟空间
	 */
	static size_t standingInitial(const __In_char *ascii, __Out_char *&result, int size = -1);
protected:
	bool init();		//若数据被损坏或者第一次使用，就向磁盘写入拼音音节数据
private:
	HChineseConvertToPY(){}
	~HChineseConvertToPY(){}
private:
	static map<int,Exchar> dataMap;
};
#endif // HChineseConvertToPY_H__
