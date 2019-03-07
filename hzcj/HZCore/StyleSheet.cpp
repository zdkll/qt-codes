#include "StyleSheet.h"
#include <QApplication>
#include <QDir>
#include <QFileInfoList>

CStyleSheet* CStyleSheet::GetInstance()
{
    static CStyleSheet styleInstance;
    return &styleInstance;
}

CStyleSheet::CStyleSheet(QObject *parent)
    : QObject(parent)
    , m_StyleSheet("")
{
}

CStyleSheet::~CStyleSheet()
{
}

void CStyleSheet::SetStyle(EStyleSheet style)
{
    switch(style)
    {
    case DEFAULT:
        loadStyle(":/skin/default/qss");
        break;
    default:
        break;
    }

    qApp->setStyleSheet(m_StyleSheet);
#ifdef Q_OS_MAC
    qApp->setStyle("fusion");
#endif
}

#include <QDebug>
void CStyleSheet::loadStyle(const QString &filePath)
{
    //qDebug() << Q_FUNC_INFO << filePath;
    m_StyleSheet = "";

    QDir dir(filePath);
    QStringList nameFilters;
    nameFilters << "*.qss";
    dir.setNameFilters(nameFilters);
    QFileInfoList fileList = dir.entryInfoList();
    int size = fileList.size();
    for(int i = 0; i < size; ++i)
    {
        QFileInfo fileInfo = fileList.at(i);
        //qDebug() << "files:" << fileInfo.fileName() << fileInfo.filePath();
        QFile qss(fileInfo.filePath());
        qss.open(QFile::ReadOnly);
        m_StyleSheet += qss.readAll();
        qss.close();
    }
}

