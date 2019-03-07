#ifndef PAGEFACTORY_H
#define PAGEFACTORY_H
/********************************************************************
created:
author: min.cheng
purpose:  负责页面的创建
*********************************************************************/
#include <QObject>
#include "hzcomponent_global.h"
#include "Ioc.hpp"
#include "basepage.h"

class HZCOMPONENTSHARED_EXPORT PageFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief  Ioc容器，用于对象创建和注册
     */
    static IocContainer* getIoc();
private:
    PageFactory(QObject *parent=NULL);
private:
    IocContainer m_ioc;
};

#endif // PAGEFACTORY_H
