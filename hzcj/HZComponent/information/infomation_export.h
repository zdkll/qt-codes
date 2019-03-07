#ifndef INFOMATION_EXPORT_H
#define INFOMATION_EXPORT_H

#include <QtGlobal>

#ifndef HZCOMPONNET_MODULES_STATIC
# if defined(HZCOMPONENT_LIBRARY)
#    define INFOMATION_EXPORT Q_DECL_EXPORT
# else
#    define INFOMATION_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define INFOMATION_EXPORT
#endif


#endif
