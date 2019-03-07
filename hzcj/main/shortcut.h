/********************************************************************
created: 2018,10,11
author: xuzhehu
purpose:
*********************************************************************/
#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QObject>
#include <QHash>
#include <QShortcut>

#include <hzobject.h>

/**
 * @brief The STShortCut struct
 */
typedef struct tagSTShortCut
{
    QString name; //名称
    QString value; //键值
    QString command; //抛出的cmd
    bool modify; //是否修饰 true 表示被修饰的键
    bool enable; //是否启用 true 表示启动
}STShortCut;

class CustomShortCut : public QShortcut
{
    Q_OBJECT
public:
    CustomShortCut(const STShortCut& key, QWidget *parent);

signals:
    void shortCutActived(const QString &cmd);

private:
    STShortCut mKeyData;
};

class ShortCutManager : public QObject, public HZObject
{
    Q_OBJECT

public:
    explicit ShortCutManager(QWidget *parent);
    ~ShortCutManager();

    /*!
     * \brief registerShortCut 更改父窗口
     * \param parent
     */
    void registerShortCut(QWidget *parent);

private:
    QList<CustomShortCut*> mShortKeys;
    QWidget *mParent;

    /*!
     * \brief initNativeKeys 初始化快捷键列表和注册快捷键
     * \note 针对快捷键配置文件中，modify为true的标记将会被注册（规则）
     */
    void initNativeKeys();
};

#endif // SHORTCUTTESTUI_H
