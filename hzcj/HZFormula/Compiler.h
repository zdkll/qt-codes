#pragma once
/********************************************************************
created:	十一月 2015
filename: 	Compiler.h
author:     min.cheng
purpose:	lua脚本编译成中间缓存
*********************************************************************/
#include <string>
#include "IRCodeCache.h"

class CEnvironment;
class CCompiler
{
public:
	CCompiler(std::shared_ptr<IRCodeCache> pIrcache);
	~CCompiler();

	/** 
	 * @brief 公式编译
	 */
	IRCodePtr Compile(std::shared_ptr<CEnvironment> pEnv,int& errid);

private:
	void Init();

	/**
	* @brief lua脚本转换字节码
	*
	* 加载Lua虚拟机，获取字节码
	*
	*/
	IRCodePtr LuaToByteCode(const std::string &luascript);

	std::shared_ptr<IRCodeCache> m_pIrcache;
	std::shared_ptr<CEnvironment> m_pCurrEnv;
};

