#include "IRCodeCache.h"
#include <QApplication>
#include "hzcfg.h"
#include <qfile.h>

IRCodeCache::IRCodeCache()
{
	LoadScript();
}


IRCodeCache::~IRCodeCache()
{
	for (auto it = m_irCacheMap.begin(); it != m_irCacheMap.end();it++)
	{
		delete[] it->second->bytecode;
	}
	m_irCacheMap.clear();
}

IRCodePtr IRCodeCache::GetCode(const std::string& script)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	auto it = m_irCacheMap.find(script);
	if (it != m_irCacheMap.end())
	{
		return it->second;
	}
	return nullptr;
}

#include <qtextcodec.h>
std::string GetScritpByPath(const std::string& path)
{
    std::string strRet;
    if(path.empty() == false)
    {
        QFile file(path.c_str());
        if(file.open(QIODevice::ReadOnly))
        {
            QString script = file.readAll();
            strRet = script.toStdString();
        }
    }
    return strRet;
}

std::string IRCodeCache::GetScript(const std::string& name)
{
	std::string strScript = "";
	auto& it = m_scriptMap.find(name);
	if (it != m_scriptMap.end())
	{
		strScript = it->second;
	}
	return strScript;
}


void IRCodeCache::AddScript(const std::string& name, const std::string& script)
{
	auto& it = m_scriptMap.find(name);
	if (it != m_scriptMap.end())
	{
		m_scriptMap[name] = m_strLuaHead + script;
	}
	else
	{
		m_scriptMap.emplace(name, m_strLuaHead+script);
	}
}

void IRCodeCache::AddCode(const std::string& script, IRCodePtr irPtr)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	auto it = m_irCacheMap.find(script);
	if (it != m_irCacheMap.end())
	{	
		delete[] it->second->bytecode;
		it->second->bytecode = nullptr;
		m_irCacheMap.erase(it);
	}
	m_irCacheMap.emplace(script, irPtr);
}

#include <qdebug.h>
void IRCodeCache::LoadScript()
{

    std::string curr;
    QString path = qApp->applicationDirPath() + "/";
    curr = path.toStdString();
    path  += "cfg/formual.jc";
     m_strLuaHead = GetScritpByPath(path .toStdString()) + "\n";
    const InidItemList& indiList =  Hzcfg::getInstance()->getIndiList();
	std::string strScript;
    for (auto it = indiList.begin(); it != indiList.end(); ++it)
	{
        strScript = GetScritpByPath(curr + it->script);
		if (!m_strLuaHead.empty() && !strScript.empty())
		{
            m_scriptMap.emplace(it->name, m_strLuaHead + strScript);
            std::string js = m_strLuaHead + strScript;

            //qDebug() << "script: " << js.c_str();
		}
	}
}
