#include "hzvlistproperty.h"

HZVListProperty::HZVListProperty(QObject *parent) : QObject(parent)
{

}

HZVListProperty *HZVListProperty::instance()
{
    static HZVListProperty ginstance;
    return &ginstance;
}
