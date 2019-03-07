#ifndef CSTYLESHEET_H
#define CSTYLESHEET_H

#include <QObject>
#include "hzcore_global.h"

class HZCORESHARED_EXPORT CStyleSheet : public QObject
{
    Q_OBJECT
public:
    enum EStyleSheet
    {
        DEFAULT
    };

    static CStyleSheet* GetInstance();
    ~CStyleSheet();
    void SetStyle(EStyleSheet style);

private:
    explicit CStyleSheet(QObject *parent = 0);
    void loadStyle(const QString &filePath);

private:
    QString         m_StyleSheet;
};

#endif // CSTYLESHEET_H
