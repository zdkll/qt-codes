#ifndef PLATEMOVE_EXPORT_H
#define PLATEMOVE_EXPORT_H
#include <QtGlobal>

#ifndef HZCOMPONNET_MODULES_STATIC
# if defined(HZCOMPONENT_LIBRARY)
#    define PLATEMOVE_EXPORT Q_DECL_EXPORT
# else
#    define PLATEMOVE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define PLATEMOVE_EXPORT
#endif

#endif
