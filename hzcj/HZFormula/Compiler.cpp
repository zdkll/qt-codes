#include "Compiler.h"
#include "Environment.h"

extern "C" {
#include "luajit/lua.hpp"
}

CCompiler::CCompiler(std::shared_ptr<IRCodeCache> pIrcache)
{
	m_pIrcache = pIrcache;
	Init();
}


CCompiler::~CCompiler()
{
}

IRCodePtr CCompiler::Compile(std::shared_ptr<CEnvironment> pEnv,int& errid)
{
	IRCodePtr irPtr = nullptr;
	if (pEnv && !pEnv->IsCancle())
	{
		m_pCurrEnv = pEnv;
		if (!(irPtr = m_pIrcache->GetCode(pEnv->GetScript())))
		{
			irPtr = LuaToByteCode(pEnv->GetScript());
		}
		Init();//当前编译完成要初始化对象
	}
	else
	{
		errid = FOMULA_CALC_CANCLE;
	}
	if (!irPtr)
	{
		errid = FOMULA_COMPILE_FAILD;
	}
	return irPtr;
}

void CCompiler::Init()
{
	m_pCurrEnv = NULL;
}

IRCodePtr CCompiler::LuaToByteCode(const std::string &luascript)
{
	IRCodePtr irPtr = nullptr;
	struct lua_State *luaVM = lua_open();
	if (NULL != luaVM)
	{
		// 基本库
		luaL_openlibs(luaVM);
		/*
		if (CheckError(luaVM, luaL_loadstring(luaVM, luascript.c_str()))) {
		}
		if (CheckError(luaVM, lua_dump(luaVM, ByteCodeSave, this))) {
		}
		*/
		// 调用string.dump(loadstring(script), script, false)
		// local srcipt;
		lua_pushstring(luaVM, luascript.c_str());
		lua_setglobal(luaVM, "script");
		// loadstring(script)
		lua_getglobal(luaVM, "loadstring");
		lua_getglobal(luaVM, "script");
		lua_call(luaVM, 1, 1);
		if (!lua_isfunction(luaVM, -1)) {
			goto __LUA_CLEAN;
		}
		lua_setglobal(luaVM, "loadscript");
		// 查找string.dump
		lua_getglobal(luaVM, "string");
		lua_pushstring(luaVM, "dump");
		lua_gettable(luaVM, -2);
		if (!lua_isfunction(luaVM, -1)) {
			goto __LUA_CLEAN;
		}
		// 参数
		lua_getglobal(luaVM, "loadscript"); // arg[0]
		lua_getglobal(luaVM, "script"); // arg[1]
		lua_pushboolean(luaVM, 0); // arg[2]
		lua_call(luaVM, 3, 1);
		// 读取
		if (!lua_isstring(luaVM, -1)) {
			goto __LUA_CLEAN;
		}
		size_t size = 0;
		const char *data = (const char*)lua_tolstring(luaVM, -1, &size);
		if (NULL == data || 0 == size) {
			goto __LUA_CLEAN;
		}
		irPtr = IRCodePtr(new IRCode);
		irPtr->size = size;
		irPtr->bytecode = new unsigned char[size];
		memcpy(irPtr->bytecode, data, irPtr->size);
		m_pIrcache->AddCode(luascript, irPtr);
	}
__LUA_CLEAN:
	if (NULL != luaVM) {
		lua_close(luaVM);
		luaVM = NULL;
	}
	return irPtr;
}
