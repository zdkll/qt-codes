#pragma once
/********************************************************************
created:	五月 2016
filename: 	Ioc.hpp
author:     min.cheng
purpose:	IOC容器,用于配置依赖关系,也是可复用的工厂方法,内容来自
<<深入应用C++11>>,我增加了继承关系和普通对象的创建功能.
*********************************************************************/
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Any.hpp"
#include "NonCopyable.hpp"

class IocContainer :public NonCopyable
{
public:
	IocContainer(void){}
	~IocContainer(){}

	/**
	* @brief 依赖关系配置
	*/
	template<class T, typename Depende, typename... Args>
	typename std::enable_if<!std::is_base_of<T, Depende>::value>::type RegisterDepend(const std::string& strKey)
	{
		std::function<T*(Args...)> function = [](Args... args){return new T(new Depende(args...)); };
		Register(strKey, function);
	}

	/**
	* @brief 继承关系配置
	*/
	template<class Base, typename Derived, typename... Args>
	typename std::enable_if<std::is_base_of<Base, Derived>::value>::type RegisterInherit(const std::string& strKey)
	{
		std::function<Base*(Args...)> function = [](Args... args){return new Derived(args...); };
		Register(strKey, function);
	}

	/**
	* @brief 普通对象注册
	*/
	template<class T, typename... Args>
	void RegisterNomal(const std::string& strKey)
	{
		std::function<T*(Args...)> function = [](Args... args){return new T(args...); };
		Register(strKey, function);
	}

	/**
	* @brief 创建对象
	*/
//	template<class T, typename...Args>
//	T* Resolve(const std::string& strKey, Args&&... args)
//	{
//		if (m_creatorMap.find(strKey) == m_creatorMap.end())
//		{
//			throw std::invalid_argument("对象没有向IOC容器注册");
//			return nullptr;
//		}
//		Any resolver = m_creatorMap[strKey];
//		std::function<T*(Args...)> function = resolver.AnyCast<std::function<T*(Args...)>>();
//		return function(args...);
//	}

	template<class T, typename...Args>
    T* Resolve(const std::string& strKey, const Args&... args)
	{
		if (m_creatorMap.find(strKey) == m_creatorMap.end())
		{
			throw std::invalid_argument("对象没有向IOC容器注册");
			return nullptr;
		}
		Any resolver = m_creatorMap[strKey];
		std::function<T*(Args...)> function = resolver.AnyCast<std::function<T*(Args...)>>();
		return function(args...);
	}

	/**
	* @brief 创建智能指针对象
	*/
	template<class T, typename... Args>
	std::shared_ptr<T> ResolveShared(const std::string& strKey, Args... args)
	{
		T* t = Resolve<T>(strKey, args...);
		return std::shared_ptr<T>(t);
	}

	/**
	* @breif 删除注册的对象
	*/
	void Delete(const std::string& strKey)
	{
		auto iter = m_creatorMap.find(strKey);
		if (iter != m_creatorMap.end())
		{
			m_creatorMap.erase(iter);
		}
	}
private:
	void Register(const std::string& strKey, Any constructor)
	{
		if (m_creatorMap.find(strKey) != m_creatorMap.end())
			throw std::invalid_argument("不允许重复注册对象到Ioc容器");
		m_creatorMap.emplace(strKey, constructor);
	}
	std::unordered_map<std::string, Any> m_creatorMap;
};
