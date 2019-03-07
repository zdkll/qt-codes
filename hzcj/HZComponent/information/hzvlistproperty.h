/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HZVLISTPROPERTY_H
#define HZVLISTPROPERTY_H

#include <QObject>

class HZVListProperty : public QObject
{
    Q_OBJECT
public:
    static HZVListProperty* instance();

    /**
     * @brief listItemFamily
     * @return
     */
    QString listItemFamily() const {return m_listItemFamily; }
    void setListItemFamily(const QString &family) { m_listItemFamily = family; }

private:
    HZVListProperty(QObject *parent = 0);
    QString m_listItemFamily;
};


#define gHZVListProperty HZVListProperty::instance()
#endif // HZVLISTPROPERTY_H
