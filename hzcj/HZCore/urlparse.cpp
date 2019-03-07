#include "urlparse.h"
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

class CUrlParseHlp
{
public:
	CUrlParseHlp();
	~CUrlParseHlp();
	///数字转换成字符, 0 <= n <= 15
	static char dec2hexChar(short int n);
	///字符转换成数字, c = {0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f}
	static short int hexChar2dec(char c);
	///字符不安全
	static bool isUnsafe(char c);
	///字符串不安全
	static bool isUnsafe(const std::string& str);

};

CUrlParseHlp::CUrlParseHlp()
{
}
CUrlParseHlp::~CUrlParseHlp()
{
}

bool CUrlParseHlp::isUnsafe(const std::string& str)
{
	std::string::size_type idx;
	for(idx=0; idx<str.size(); idx++)
	{
		if(isUnsafe(str[idx]))
			return true;
	}
	return false;
}
bool CUrlParseHlp::isUnsafe(char c)
{
	if ( ( '0'<=c && c<='9' ) ||
		( 'a'<=c && c<='z' ) ||
		( 'A'<=c && c<='Z' ) )
	{//保留 数字和字符
		return false;
	}
	else
		return true;
}
///数字转换成字符, 0 <= n <= 15
char CUrlParseHlp::dec2hexChar(short int n)
{
	if ( 0 <= n && n <= 9 )
		return char( short('0') + n );
	else if ( 10 <= n && n <= 15 )
		return char( short('A') + n - 10 );
	else
		return char(0);
} 

///字符转换成数字, c = {0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f}
short int CUrlParseHlp::hexChar2dec(char c)
{
	if ( '0'<=c && c<='9' )
		return short(c-'0');
	else if ( 'a'<=c && c<='f' )
		return ( short(c-'a') + 10 );
	else if ( 'A'<=c && c<='F' )
		return ( short(c-'A') + 10 );
	else
		return -1;
}

CUrlParameter::CUrlParameter( const std::string& urlParameter, bool bDecode, bool bHttpUtility /*= false*/)
{
	if(bHttpUtility)
	{
		m_fun_decode = std::bind(CUrlUtil::DecodeForHttpUtility, std::placeholders::_1);
		m_fun_encode = std::bind(CUrlUtil::EncodeForHttpUtility, std::placeholders::_1);
	}
	else
	{
		m_fun_decode = std::bind(CUrlUtil::Decode, std::placeholders::_1);
		m_fun_encode = std::bind(CUrlUtil::Encode, std::placeholders::_1, eUrlParseType_alphanum);
	}
    addUrlParameter(urlParameter, bDecode);
}

CUrlParameter::~CUrlParameter()
{

}

bool CUrlParameter::isKeyExsit( const std::string& key )
{
	return m_valuemap.find(key) != m_valuemap.end();
}

void CUrlParameter::setString( const std::string& key, const std::string& value/*=std::string()*/ )
{
	m_valuemap[key] = value;
}

void CUrlParameter::setBuffer( const std::string& key, const char* Buf, int nBufLen )
{
	m_valuemap[key] = std::string(Buf, nBufLen);
}

bool CUrlParameter::getString( const std::string& key, std::string& value )
{
	keyvalue_map_tpye::const_iterator pos = m_valuemap.find(key);
	if(pos!=m_valuemap.end())
	{
		value = pos->second;
		return true;
	}
	return false;
}

CUrlParameter::buffer_ptr CUrlParameter::getBuffer( const std::string& key )
{
	keyvalue_map_tpye::const_iterator pos = m_valuemap.find(key);
	if(pos!=m_valuemap.end())
	{
		buffer_ptr buf = buffer::CreateFactory(pos->second.size());
		memcpy_s(buf->getbuffer(), buf->size(), pos->second.c_str(), pos->second.size());
		return buf;
	}
	return CUrlParameter::buffer_ptr();
}

std::string CUrlParameter::getUrlParameter()
{
	std::string urlParameter;
	keyvalue_map_tpye::const_iterator pos = m_valuemap.begin();
	while(pos != m_valuemap.end())
	{
		if(urlParameter.size() > 0)
			urlParameter += KEY_SPLIT;
		urlParameter += m_fun_encode(pos->first);
		urlParameter += VALUE_SPLIT;
		urlParameter += m_fun_encode(pos->second);

		++pos;
	}
	return urlParameter;
}

int CUrlParameter::addUrlParameter(const std::string& urlParameter, bool bDecode)
{
	int nCount = 0;
	std::vector<std::string> keyvalvector;
    baseStrSplit("&", urlParameter, keyvalvector);
	std::vector<std::string>::const_iterator pos = keyvalvector.begin();
	while (pos != keyvalvector.end())
	{
		if(pos->size()>0)
		{
			std::string::size_type sp = pos->find(VALUE_SPLIT);
			std::string key, value;
			if(sp != std::string::npos)
			{
				key = pos->substr(0,sp);
				value = pos->substr(sp+1);
			}
			else
			{
				key = *pos;
			}
			if(bDecode)
			{
				key = m_fun_decode(key);
				value = m_fun_decode(value);
			}
            if(!isKeyExsit(key))
			{
				setString(key, value);
				nCount++;
			}
		}
		++pos;
	}
	return nCount;
}

int CUrlParameter::addUrlParameter( const CUrlParameter& url )
{
	int nCount = 0;
	CUrlParameter::keyvalue_map_tpye::const_iterator pos = url.m_valuemap.begin();
	while (pos != url.m_valuemap.end())
	{
        if(!isKeyExsit(pos->first))
		{
			setString(pos->first, pos->second);
			nCount++;
		}
		++pos;
	}
	return nCount;
}

void CUrlParameter::delKey(const std::string& key)
{
	m_valuemap.erase(key);
}

CUrlParameter::keyvalue_map_tpye::const_iterator CUrlParameter::begin()
{
	return m_valuemap.begin();
}
CUrlParameter::keyvalue_map_tpye::const_iterator CUrlParameter::end()
{
	return m_valuemap.end();
}

void CUrlParameter::baseStrSplit(const std::string u, const std::string string, std::vector<std::string>& vector)
{
	std::string::size_type pos;
	std::string str = string + u;//扩展字符串以方便操作
    unsigned int size = str.size();

    for (unsigned int i = 0; i < size; i++)
	{
		pos = str.find(u, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			vector.push_back(s);
			i = pos + u.size() - 1;
		}
	}
}

std::string CUrlUtil::DecodeForHttpUtility( const std::string &URL )
{
	//HttpUtility.UrlDecode 其中'+'转为' '
	string result = "";
	char c1,c0;
	short int i1,i0;
	for ( unsigned int i=0; i<URL.size(); i++ )
	{
		char c = URL[i];
		if ( c != '%' )
		{//字母和数字
			if(c == '+')
				result += ' ';
			else
				result += c;
		}
		else
		{//被编码的字符，进行解码
			c1 = URL[i+1];
			c0 = URL[i+2];
			i1 = CUrlParseHlp::hexChar2dec(c1);
			i0 = CUrlParseHlp::hexChar2dec(c0);
			if(i1 > -1 && i0 >-1)
			{//可以解码
				i+=2;
				result += char(i1 * 16 + i0);
			}
			else
			{//不可以解码
				result += c;
			}
		}
	}
	return result;
}

std::string CUrlUtil::EncodeForHttpUtility( const std::string &URL )
{
	//HttpUtility.UrlEncode 保留内容 !()*-._ 其中' '转为'+'
	string result = "";
	int i1, i0;
	for ( unsigned int i=0; i<URL.size(); i++ )
	{
		char c = URL[i];
		if ( ( '0'<=c && c<='9' ) ||
			( 'a'<=c && c<='z' ) ||
			( 'A'<=c && c<='Z' )
			//|| (c=='|' || c=='(' || c==')' || c=='*' || c=='-' || c=='.' || c=='_')
			)
		{//保留 数字和字符
			result += c;
		}
		else if(c == ' ')
		{
			result += '+';
		}
		else
		{//非数字和非字符 全部编码
			int j = (short int)c;
			if ( j < 0 )
				j += 256;	//去除负号

			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += CUrlParseHlp::dec2hexChar(i1);
			result += CUrlParseHlp::dec2hexChar(i0); 
		}
	}
	return result;
}

std::string CUrlUtil::Decode( const std::string &URL )
{
	string result = "";
	char c1,c0;
	short int i1,i0;
	for ( unsigned int i=0; i<URL.size(); i++ )
	{
		char c = URL[i];
		if ( c != '%' )
		{//字母和数字
			if(c == '+')
				result += ' ';
			else
				result += c;
		}
		else
		{//被编码的字符，进行解码
			c1 = URL[i+1];
			c0 = URL[i+2];
			i1 = CUrlParseHlp::hexChar2dec(c1);
			i0 = CUrlParseHlp::hexChar2dec(c0);
			if(i1 > -1 && i0 >-1)
			{//可以解码
				i+=2;
				result += char(i1 * 16 + i0);
			}
			else
			{//不可以解码
				result += c;
			}
		}
	}
	return result;
}

std::string CUrlUtil::Encode( const std::string &URL, eUrlParseType uType )
{
	string result = "";
	int i1, i0;
	for ( unsigned int i=0; i<URL.size(); i++ )
	{
		char c = URL[i];
		if (   (uType & eUrlParseType_digit				&& '0'<=c && c<='9')
			|| (uType & eUrlParseType_low_alpha			&&'a'<=c && c<='z')
			|| (uType & eUrlParseType_high_alpha		&& 'A'<=c && c<='Z')
			|| (uType & eUrlParseType_unreserved_plus	&& (c=='-' || c=='.' || c=='_' || c=='~'))
			|| (uType & eUrlParseType_mark_plus			&& (c=='!' || c=='*' || c=='\'' || c=='(' || c==')'))
			|| (uType & eUrlParseType_gen_3986			&& (c=='[' || c==']'))
			|| (uType & eUrlParseType_gen_delims_plus	&& (c==':' || c=='/' || c=='?' || c=='#' || c=='@'))
			|| (uType & eUrlParseType_sub_delims		&& (c=='!' || c=='$' || c=='&' || c=='\'' || c=='(' || c==')' || c=='*' || c=='+' || c==',' || c==';' || c=='='))
			|| (uType & eUrlParseType_js_escape_plus	&& (c=='*' || c=='+' || c=='-' || c=='.' || c=='/' || c=='@' || c=='_'))
			)
			result += c;
		else
		{//非数字和非字符 全部编码
			int j = (short int)c;
			if ( j < 0 )
				j += 256;	//去除负号

			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += CUrlParseHlp::dec2hexChar(i1);
			result += CUrlParseHlp::dec2hexChar(i0); 
		}
	}
	return result;
}

