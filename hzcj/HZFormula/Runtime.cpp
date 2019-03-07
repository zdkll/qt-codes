#include "Runtime.h"
#include "Environment.h"

extern "C" {
    #include "luajit/lua.hpp"
}
//#pragma comment(lib, "lua51.lib")

namespace {

	// 环境变量的标识
	static const char *ENVNAME = "__JC_FENV";

	// 取消LuaVM的回调
	void LuaHookCancelCallback(lua_State *L, lua_Debug *ar) {
		lua_sethook(L, NULL, 0, 0);
		luaL_error(L, "Cancel execute");
	}
	// 加载lua字节码
	typedef struct {
		const unsigned char *bytecode;
		size_t size;
	} LuaByteCode;
	const char* LuaLoadByteCodeCallback(lua_State *L, void *ud, size_t *sz) {
		LuaByteCode *code = (LuaByteCode*)ud;
		if (0 == code->size) {
			*sz = 0;
			return NULL;
		}
		else {
			*sz = code->size;
			code->size = 0;
			return (const char*)code->bytecode;
		}
	}

} // namespace 



CRuntime::CRuntime(std::shared_ptr<CEnvironment> spEnv)
{
	m_pEnv = spEnv;
	m_bRunning = false;
	m_pLuaVM = nullptr;
	m_pLuaVM = lua_open();
}


CRuntime::~CRuntime()
{
	if (NULL != m_pLuaVM) {
		lua_close(m_pLuaVM);
		m_pLuaVM = NULL;
	}
}

bool CRuntime::Run(const unsigned char *bytecode, size_t size, const std::string &name,std::string& errmsg)
{
	/*
	if 运行取消
	返回false
	client调用开始运行
	初始化lua
	载入所有lua标准库
	加载并运行lua脚本
	if 运行失败
	打印信息，关闭虚拟机，调用client运行接收接口，并返回false
	弹出 压入的环境变量
	关闭虚拟机
	调用client运行接收接口，并返回false 返回true
	*/
	bool success = false;
    std::vector<IndiParam> params = m_pEnv->GetInitCalcUnitParam().Calcinfo.base_info.params;
	std::string error;
	LuaByteCode code = { bytecode, size };
	// 检查是否已经被取消/VM错误(1)
	if (m_pEnv->IsCancle() || NULL == m_pLuaVM) {
		goto _RUN_CLEAN_;
	}

	// 加载基本库
	luaL_openlibs(m_pLuaVM);

	// 设置环境变量
	lua_newuserdata(m_pLuaVM, 0); // 创建一个用户数据块，放在栈顶
	lua_pushlightuserdata(m_pLuaVM, m_pEnv.get());
	lua_setglobal(m_pLuaVM, ENVNAME);
	// 设置周期 (ming.cheng 目前我们不设置周期)
	//lua_pushnumber(m_pLuaVM,m_env.GetCalcSize());
	//lua_setglobal(luaVM_, "MAX_CYCLE");

	//设置参数变量
	for (auto it = params.begin(); it != params.end();it++)
	{
        lua_pushnumber(m_pLuaVM, it->value); //采用直接赋值方式
		//end dev14237
        lua_setglobal(m_pLuaVM, it->name.c_str());
	}

	// 装载字节码
	if (CheckVMError(error, lua_load(m_pLuaVM,
		LuaLoadByteCodeCallback, /*加载字节码的回调*/
		(void*)&code, /*回调上下文*/
		name.c_str())))
	{
		goto _RUN_CLEAN_;
	}
	do {
		std::lock_guard<std::mutex> lock(m_mutex);
		// 检查是否已经被取消(2)
		if (m_pEnv->IsCancle()) {
			goto _RUN_CLEAN_;
		}
		m_bRunning = true;
	} while (false);
	// 运行
	if (CheckVMError(error, lua_pcall(m_pLuaVM, 0, 0, 0))) {
		goto _RUN_CLEAN_;
	}
	// 检查是否已经被取消(3)
	if (m_pEnv->IsCancle()) {
		goto _RUN_CLEAN_;
	}
	success = true;

_RUN_CLEAN_:
	if (m_pEnv->IsCancle() || !success) {
		if (!m_pEnv->IsCancle()) {
			errmsg = error;
		}
        //JCW_DEBUG_LOG(TRACE, _T("this=%p CRuntime be Cancled"), this);
		return false;
	}
	else {
		return true;
	}
}

void CRuntime::Cancel()
{
	///*
	//if (已经取消过)
	//return;
	//设置取消标记

	//加锁保护luaVM_
	//设置VM取消运行的回调HOOK
	//*/
	//if (cancel_) {
	//	return;
	//}
	//AutoCS lock(lock_);
	//cancel_ = true;

	//if (running_ && NULL != luaVM_) {
	//	// 设置VM运行的回调：每条指令回调一次
	//	//lua_lock(luaVM_);
	//	lua_sethook(luaVM_,
	//		LuaHookCancelCallback,
	//		LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT,
	//		1);
	//	//lua_unlock(luaVM_);
	//}
}

bool CRuntime::CheckVMError(std::string &error, int status)
{
	if (status != 0)
	{
		// 发生错误
		const char *msg = NULL;
		if (!lua_isnil(m_pLuaVM, -1))
		{
			msg = lua_tostring(m_pLuaVM, -1);
			if (NULL == msg)
			{
				msg = "(error object is not a string)";
			}
		}
		else
		{
			msg = "unknown error";
		}
		error = msg;

		lua_pop(m_pLuaVM, 1);
		return true;
	}

	return false;
}
