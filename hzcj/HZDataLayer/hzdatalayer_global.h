#ifndef HZDATALAYER_GLOBAL_H
#define HZDATALAYER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HZDATALAYER_LIBRARY)
#  define HZDATALAYERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HZDATALAYERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HZDATALAYER_GLOBAL_H