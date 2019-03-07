#ifndef HZAUTHENTICATION_GLOBAL_H
#define HZAUTHENTICATION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HZAUTHENTICATION_LIBRARY)
#  define HZAUTHENTICATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HZAUTHENTICATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HZAUTHENTICATION_GLOBAL_H
