#ifndef HZOBJECT_H
#define HZOBJECT_H
/********************************************************************
created:
author: min.cheng
purpose: 封装HZCJ 对象类，用于处理对象间消息传递，派发，漏油等。
*********************************************************************/
#include "hzcontrols_global.h"
#include <map>
#include <functional>
#include <QString>
#include <Any.hpp>

//广播路径
#define BROAD_CASET "broadcast"
//基于key value的消息格式
typedef std::map<QString,QString> MessageParams;
// 参数列表： 1 之前请求的URL 2 响应对象的路径  3 返还的参数列表
typedef  std::function<void(const QString&,const QString&,const MessageParams&)> OnMessageResponse;
//内部消息处理
class InternalMsgObj;


class HZCONTROLSSHARED_EXPORT HZObject
{
public:
    /**
     * @brief  应用程序初始化需要调用，用于注册QT消息的元类型，
     *         用于支持自定义QT消息参数，应用程序初始化调用。
     */
    static void init();


    /**
     * @brief  应用程序退出前，其他模块资源释放后调用。
     */
    static void unInit();

    /**
     * @brief 构造对象
     * @param[in] instName 实例名称，用于区分不同对象。
     *            重复可能会导致消息漏油路径错误，
     */
    HZObject(QString instName="");

    /**
     * @brief 析构
     */
    ~HZObject();

    /**
     * @brief 设置当前对象所属的对象，构建对象路径用于消息漏油。
     * @param[in] owner 当前对象拥有者的指针，要求owner有自己的instName。
     *            否则消息会传递失败。
     */
    void setOwner(HZObject* owner);

    /**
     * @brief 获取当前对象的拥有者，当前接口可以递归出整个路径
     * @retval  返回SetOwner设置的对象指针，否则返回空。
     */
    HZObject* getOwner();

    /**
     * @brief  附加到全局消息总线上，可以监听全局消息。
     *         附件后，可以通过重写OnMessage来获取消息。
     *         基于效率原因，需要显示调用是否附件到全局。
     *         注意调用SetOwner后，则不需要再显示调用AttachMessageBus。
     * @param[in] bRoot 是否是根对象。默认情况下除根对象外不需要关系。
     */
    void attachMessageBus(bool bRoot=false);

    /**
     * @brief 取消消息注册，建议对象析构前调用
     * @retval
     */
    void detachMessageBus();

    /**
     * @brief 用于设置应用程序的初始命令，确保消息回退正确处理第一次回退，多次调用以最后一次为准。
     * @param[in] rever 消息接收者
     * @param[in] params 消息体
     */
    void setDefaultMsg(const QString& rever,const MessageParams& params);

    /**
     * @brief 获取消息漏油的路径，根对象以外的其他对象通过SetOwner来自动构建。
     * @retval 失败返回空。
     */
    QString getPath();

    /**
     * @brief 用于处理SendMessage  BroadcastMessage消息处理
     * @param[in] senderPath 获取发送则路径 广播始终是BROAD_CASET。
     * @param[in] params消息体
     */
    virtual void onMessage(const QString& senderPath,const MessageParams& params);

    /**
     * @brief 用于处理GetMessage消息
     * @param[in] path 等于GetMessage传入的rever。
     * @param[in] params消息体
     * @param[out] 成功返回outData
     */
    virtual bool onProcessGetMessage(const QString& path,MessageParams& params,Any& outData);

    /**
     * @brief  将消息体转换成url消息格式，便于后期消息传递和web相关交互。
     * @param[in] objPath对象路径
     * @param[in] params 消息体
     * @retval  比如返回 root_frame?switch_page=1&hide_panel=stockpanel
     */
    static QString createMsg(const QString& objPath,const MessageParams& params);

    /**
     * @brief 将URL格式的消息转换成我们的路径和MessageParams 格式。
     * @param[in] msg url如：root_frame?switch_page=1&hide_panel=stockpanel
     * @param[out] 返回路径path 例如root_frame
     * @param[out] 返回对应的Key value 如 switch_page 对应 “1 ”
     * @retval 成功返回true，否则返回false
     */
    static bool parseMsg(const QString& msg,QString& path,MessageParams& params);

    /**
     * @brief 同步发送消息，对方接收并处理函数完成回调后返回。
     * @param[in] rever 消息接收者
     * @param[in] params key-value消息结构。
     * @param[in] bRecord true表示记录到回退历史，否则不记录。回退记录是相对于
     *            整个应用程序。
     */
    void sendMessage(const QString& rever,const MessageParams& params,bool bRecord=false);

    /**
     * @brief 用于向所有附加到消息总线上的对象广播消息，通过重写OnMessage获取。
     *        广播消息时OnMessage的senderPath始终是BROAD_CASET。
     *        发送者不会获取到自己的广播消息。
     * @param[in] params
     */
    void broadcastMessage(const MessageParams& params);

    /**
     * @brief 通过指定对象路径获取相应的数据。
     * @param[in] rever 消息接收者路径。rever必须重写过OnProcessGetMessage，并且处理过相应
     *            的消息请求，否则无效。
     * @param[in] params key value消息结构
     * @param[out] outData用于返回获取到的数据
     */
    void getMessage(const QString& rever,const MessageParams& params,Any& outData);

    /**
     * @brief 获取当前pageId
     * @retval 返回当前页面ID
     */
    int getGlobalPageId();
    int getGlobalChildPageId();

    /**
     * @brief 设置当前pageId
     */
    void setGlobalPageId(int id);
    void setGlobalChildPageId(int id);

    /**
     * @brief 用于消息中心命令回退。
     */
    static void MsgBack();


    /**
     * @brief 设置当前合约 目前多个对象都会有当前合约的属性
     */
    virtual void setStockCode(const QString& stock);
    QString getStockCode();

    /**
     * @brief 对象激活
     */
    virtual void afterActive();

    /**
     * @brief 取消激活
     */
    virtual void afterDeactive();

    /**
     * @brief   网络连接发生变化
     */
    virtual void onNetIoReady(bool ready);

    /**
     * @brief 合约变化时被调用
     * @retval
     */
    virtual void OnStockCodeChange();
 protected:
    //合约
    QString m_stockCode;
 private:
    //所属对象
    HZObject* m_owner;
    //对象路径
    QString m_path;
    //消息处理类，用于消息派发，漏油等
    static InternalMsgObj* m_sMsgBus;
    friend InternalMsgObj;
};



#endif // HZOBJECT_H
