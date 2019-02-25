#ifndef IOCCONTAINTER_H
#define IOCCONTAINTER_H

#include <unordered_map>
#include <string>
#include <functional>
#include "any.h"
#include "noncopyable.h"
//
class IocContainter : public NonCopyable
{
public:
    IocContainter(){}
    ~IocContainter(){}

    //配置依赖关系
    template<class T,typename Depende,typename...Args>
    typename std::enable_if<!std::is_base_of<T,Depende>::value>::type RegisterDepend(const std::string &strKey)
    {
        std::function<T*(Args)> function = [](Args...args){return new T(new Depende(args...));};
        Register(strKey,function);
    }

    //配置继承关系
    template<class Base,typename Derived,typename...Args>
    typename std::enable_if<std::is_base_of<Base,Derived>::value>::type RegisterInherit(const std::string& strKey)
    {
        std::function<Base*(Args...)> function = [](Args...args){return new Derived(args...);};
        Register(strKey,function);
    }

    //普通对象
    template<class T,typename...Args>
    void RegisterNormal(const std::string& strKey){
        std::function<T* (Args...)> function = [](Args...args){return new T(args...);};
        Register(strKey,function);
    }

    template<class T,typename...Args>
    T* Resolve(const std::string&strKey,const Args&...args){
        if(m_creatorMap.find(strKey) == m_creatorMap.end()){
            throw std::invalid_argument("对象没有向IOC注册！");
            return nullptr;
        }
        Any resolver = m_creatorMap[strKey];
        std::function<T*(Args...)> funtion = resolver.AnyCast<std::function<T*(Args...)>>();
        return funtion(args...);
    }

template<class T,typename...Args>
    T* ResolveShared(const std::string& strKey,Args...args){
        T *t = Resolve<T>(strKey,args...);
        return std::shared_ptr<T>(t);
    }

    void Delete(const std::string& strKey){
        auto iter = m_creatorMap.find(strKey);
        if(iter != m_creatorMap.end()){
            m_creatorMap.erase(iter);
        }
    }

private:
    void Register(const std::string& strKey,Any constructor){
        if(m_creatorMap.find(strKey) != m_creatorMap.end()){
            throw std::invalid_argument("不允许注册重复对象到容器！");
        }
        m_creatorMap.emplace(strKey,constructor);
    }
    std::unordered_map<std::string,Any> m_creatorMap;
};

#endif // IOCCONTAINTER_H
