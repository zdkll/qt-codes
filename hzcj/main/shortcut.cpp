#include "shortcut.h"
#include <QDebug>
#include <QFile>
#include <QEvent>
#include <QKeyEvent>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QApplication>
#include <QWidget>

#include <hzcfg.h>

/**
 * @brief CustomShortCut::CustomShortCut
 * @param key
 * @param parent
 */
CustomShortCut::CustomShortCut(const STShortCut &key, QWidget *parent)
    :QShortcut(parent)
    ,mKeyData(key)
{
    if(key.modify) {
        setKey(QKeySequence(key.value));
    }

    connect(this, &CustomShortCut::activated, [=]{
        if(mKeyData.modify && mKeyData.enable) {
            emit shortCutActived(mKeyData.command);
        }
    });
}

/**
 * @brief ShortCutManager::ShortCutManager
 * @param parent
 */
ShortCutManager::ShortCutManager(QWidget *parent) :
    mParent(parent)
{

    initNativeKeys();
}

ShortCutManager::~ShortCutManager()
{
}

void ShortCutManager::registerShortCut(QWidget *parent)
{
    qDeleteAll(mShortKeys.begin(), mShortKeys.end());
    mShortKeys.clear();
    mParent = parent;
    initNativeKeys();
}

void ShortCutManager::initNativeKeys()
{
    QString path = Hzcfg::getInstance()->getShortCutCfgPath();
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "file open failed..." << file.errorString();
        return;
    }

    QList<STShortCut> nativeKeys;
    QByteArray ba = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error == QJsonParseError::NoError) {
        QVariantList varlst = doc.toVariant().toMap()["keys"].toList();
        for(const QVariant &var : varlst) {
            QVariantMap varmap = var.toMap();
            STShortCut std;
            std.value   = varmap["value"].toString();
            std.name    = varmap["name"].toString();
            std.command = varmap["command"].toString();
            std.modify  = varmap["modify"].toBool();
            std.enable  = varmap["enable"].toBool();
            nativeKeys.push_back(std);
        }
    }

    if(nativeKeys.isEmpty()) return;
    for(const STShortCut &shortcut : nativeKeys){
        if(shortcut.modify) {
            CustomShortCut *pShortCut = new CustomShortCut(shortcut, mParent);
            mShortKeys.push_back(pShortCut);
            connect(pShortCut, &CustomShortCut::shortCutActived, [&](const QString &cmd){
                qDebug() << Q_FUNC_INFO << "------------------------------------->:" << cmd;
                QString path;
                MessageParams params;
                parseMsg(cmd, path, params);
                broadcastMessage(params);
            });
        }
    }
}

