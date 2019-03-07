#pragma once
/********************************************************************
created:	十一月 2015
filename: 	Runtime.h
author:     min.cheng
purpose:	运行时
*********************************************************************/
#include <string>
#include <mutex>
#include <memory>
class CEnvironment;
struct lua_State;
class CRuntime
{
public:
	CRuntime(std::shared_ptr<CEnvironment> spEnv);
	~CRuntime();


	/**
	* @brief 运行脚本
	* @param[in] bytecode lua字节码
	* @param[in] size lua字节码的长度
	* @param[in] name 公式的名称
	* @return 失败返回false，成功返回true
	*
	* 内部实现Luajit调用lua脚本运行
	* 发生
	*/
	bool Run(const unsigned char *bytecode, size_t size, const std::string &name, std::string& errmsg);


	/**
	* @brief 取消运行
	*
	* 如果LuaVM已经在运行状态，则可以使用如下的方案取消运行：
	*  http://www.cppblog.com/sleepwom/archive/2011/05/13/146321.html
	*  int mask = LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT;
	*  lua_sethook(luaVM, CancelHookProc, mask, 1);
	*
	*  function CancelHookProc(luaVM)
	*      lua_sethook(luaVM, NULL, 0, 0);
	*      luaL_error(luaVM, "Cancel execute");
	*  end
	*/
	void Cancel();
private:
	// 检查VM错误, 错误返回true
	bool CheckVMError(std::string &error, int status);

	size_t m_id;

	// 环境变量
	std::shared_ptr<CEnvironment> m_pEnv;

	// 正在运行
	bool m_bRunning;

	// Lua虚拟机
	struct lua_State *m_pLuaVM;

	std::mutex m_mutex;
};

