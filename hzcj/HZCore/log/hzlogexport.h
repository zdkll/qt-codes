#ifndef HZLOGEXPORT_H
#define HZLOGEXPORT_H

#include <QtGlobal>

#ifndef HZLOG_BUILD_STATIC
# if defined(HZLOG_BULID_LIB)
#    define HZLOG_EXPORT Q_DECL_EXPORT
# else
#    define HZLOG_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define KEYBOARD_EXPORT
#endif


#endif // HZLOGEXPORT_H
