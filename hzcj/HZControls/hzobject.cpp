#include "hzobject.h"
#include <mutex>
#include <QMap>
#include <QVector>
#include <QMetaType>
#include "urlparse.h"

struct cmdItem{
    HZObject* sender = nullptr;
    QString msg;
};

class InternalMsgObj
{
public:
    InternalMsgObj(){
    }

    void attach(const QString& path,HZObject* obj){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_msgMap_[path] = obj;
    }

    void detach(const QString& path){
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it= m_msgMap_.find(path);
            if(it !=  m_msgMap_.end()){
                m_msgMap_.erase(it);
            }
            for(auto it = m_historyCmd.begin();it != m_historyCmd.end();){
                if(it->sender && it->sender->getPath() == path && path.isEmpty() == false){
                    it = m_historyCmd.erase(it);
                }else{
                    ++it;
                }
            }
    }

    void push(HZObject* sender,const QString& msg,bool bRecord)
    {
        MessageParams params;
        QString path;
        if(HZObject::parseMsg(msg,path,params))
        {
            HZObject* obj = nullptr;
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                auto it = m_msgMap_.find(path);
                if(it != m_msgMap_.end()){
                    obj = it.value();
                }
            }
            if(obj/* && obj != sender*/){
                if(bRecord){
                    InsertQueue(sender,msg);
                }
                obj->onMessage(sender->getPath(),params);
            }

        }
    }

    void broadcast(HZObject* sender,const QString& msg)
    {
        MessageParams params;
        QString path;
        if(HZObject::parseMsg(msg,path,params) && path == BROAD_CASET)
        {
            QMap<QString,HZObject*> msgMap;
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                msgMap = m_msgMap_;
            }
            for(auto it = msgMap.begin();it != msgMap.end();it++){
                if(it.value() && it.value() != sender){
                   // it->second->onMessage(sender->getPath(),params);
                    it.value()->onMessage(BROAD_CASET,params); //广播就隐藏发送者
                }
            }
        }
    }

    void get(HZObject* sender,const QString& msg,Any& outData)
    {
        MessageParams params;
        QString path;
        if(HZObject::parseMsg(msg,path,params))
        {
            HZObject* obj = nullptr;
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                auto it = m_msgMap_.find(path);
                if(it != m_msgMap_.end()){
                    obj = it.value();
                }
            }
            if(obj){
                obj->onProcessGetMessage(path,params,outData);
            }
        }
    }

    void back(){
        if(!m_historyCmd.empty())
        {
            cmdItem item =m_historyCmd[m_historyCmd.size()-1];
            m_historyCmd.erase(m_historyCmd.end()-1);
            push(item.sender,item.msg,false);
            m_currMsg = item;
        }
    }

    //用于回退设置默认命令
    void setDfaultMsg(const cmdItem& item)
    {
        m_currMsg = item;
    }

    void setPageId(int id)
    {
        m_currPageId = id;
    }

    int getPageId()
    {
        return m_currPageId;
    }

    void setChildPageId(int id)
    {
        m_childPageId = id;
    }

    int getChildPageId()
    {
        return m_childPageId;
    }
private:
    void InsertQueue(HZObject* sender,const QString& msg){
        if(m_historyCmd.size() > 20)
        {
            while(m_historyCmd.size() > 20)
            {
                m_historyCmd.pop_front();
            }
        }
        if(!m_currMsg.msg.isEmpty())
        {
            m_historyCmd.push_back(m_currMsg);
        }
        m_currMsg.sender = sender;
        m_currMsg.msg = msg;
    }
    QMap<QString,HZObject*> m_msgMap_;
    std::mutex m_mutex;
    QVector<cmdItem> m_historyCmd;
    cmdItem m_currMsg;
    std::mutex m_mutexHist;
    int m_currPageId;
    int m_childPageId = -1;
};

InternalMsgObj* HZObject::m_sMsgBus = new InternalMsgObj;

HZObject::HZObject(QString instName):
    m_owner(nullptr),
    m_path(instName),
    m_stockCode("")
{
}

void HZObject::unInit()
{
    if (HZObject::m_sMsgBus)
	{
        delete HZObject::m_sMsgBus;
        HZObject::m_sMsgBus = nullptr;
	}
}

void HZObject::init()
{
    qRegisterMetaType<MessageParams>();
}

HZObject::~HZObject()
{
    if (m_sMsgBus)
        m_sMsgBus->detach(m_path);
}


void HZObject::attachMessageBus(bool bRoot)
{
    if(m_owner || bRoot == true){
        m_sMsgBus->attach(m_path,this);
    }
}

void HZObject::detachMessageBus()
{
    m_sMsgBus->detach(m_path);
}

void HZObject::sendMessage(const QString& rever,const MessageParams& params, bool bRecord)
{
     m_sMsgBus->push(this,createMsg(rever,params),bRecord);
}

void HZObject::broadcastMessage(const MessageParams& params)
{
    m_sMsgBus->broadcast(this,createMsg(BROAD_CASET,params));
}

void HZObject::getMessage(const QString& rever,const MessageParams& params,Any& outData)
{
    m_sMsgBus->get(this,createMsg(rever,params),outData);
}

int HZObject::getGlobalPageId()
{
    return m_sMsgBus->getPageId();
}

int HZObject::getGlobalChildPageId()
{
    return m_sMsgBus->getChildPageId();
}

void HZObject::setGlobalPageId(int id)
{
    m_sMsgBus->setPageId(id);
}

void HZObject::setGlobalChildPageId(int id)
{
    m_sMsgBus->setChildPageId(id);
}

void HZObject::MsgBack()
{
    m_sMsgBus->back();
}

void HZObject::setDefaultMsg(const QString& rever,const MessageParams& params)
{
    cmdItem item;
    item.sender = this;
    item.msg = createMsg(rever,params);
    m_sMsgBus->setDfaultMsg(item);
}

QString HZObject::getPath()
{
    return m_path;
}

void HZObject::onMessage(const QString& senderPath, const MessageParams &params)
{

}

bool HZObject::onProcessGetMessage(const QString& path,MessageParams& params,Any& outData)
{
    return false;
}

bool HZObject::parseMsg(const QString &msg,QString &path, MessageParams &params)
{
    std::string strMsg = msg.toStdString();
    size_t len = strMsg.find_first_of('?');
    if (std::string::npos != len)
    {
        path = QString::fromStdString(strMsg.substr(0, len));
        std::string data = strMsg.substr(len + 1, strMsg.size());
        CUrlParameter urlParam(data,true);
        for(auto it= urlParam.begin();it != urlParam.end();it++)
        {
            params.emplace(QString::fromStdString(it->first),QString::fromStdString(it->second));
        }
        return true;
    }
    return false;
}

QString HZObject::createMsg(const QString &objPath, const MessageParams &params)
{
    CUrlParameter urlParam;
    QString msg;
    for(auto it = params.begin();it != params.end();it++)
    {
        urlParam.setString(it->first.toStdString(),it->second.toStdString());
    }
    msg = objPath + "?";
    msg.append(QString::fromStdString(urlParam.getUrlParameter()));
    return msg;
}

void HZObject::afterActive()
{

}

void HZObject::afterDeactive()
{

}

void HZObject::onNetIoReady(bool ready)
{
    if(ready)
    {
        this->afterActive();
    }
}

void HZObject::setStockCode(const QString &stock)
{
    m_stockCode = stock;
    OnStockCodeChange();
}

QString HZObject::getStockCode()
{
    return m_stockCode;
}

void HZObject::OnStockCodeChange()
{

}

void HZObject::setOwner(HZObject *owner)
{
    m_owner = owner;
    if(owner && m_path.isEmpty() == false){
        m_path = owner->getPath() + "//" + m_path;
        attachMessageBus();
    }
}

HZObject* HZObject::getOwner()
{
    return m_owner;
}


