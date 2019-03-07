#pragma once
/********************************************************************
created:	十一月 2015
filename: 	IRCode.h
author:     min.cheng
purpose:	lua字节码，中间缓存
*********************************************************************/
#include <unordered_map>
#include <memory>
//#include "memblock.h"
#include <mutex>
struct IRCode
{
	unsigned char *bytecode;
	size_t size;
};

typedef std::shared_ptr<IRCode>	IRCodePtr;

class IRCodeCache
{
public:
	IRCodeCache();
	~IRCodeCache();

	IRCodePtr GetCode(const std::string& script);

	void AddCode(const std::string& script, IRCodePtr irPtr);

	std::string GetScript(const std::string& name);

	void AddScript(const std::string& name, const std::string& script);
private:
	void LoadScript();
	std::mutex m_mutex;
	std::unordered_map<std::string, IRCodePtr> m_irCacheMap;
	std::unordered_map<std::string, std::string> m_scriptMap; //name path
	std::string m_strLuaHead;	
};

