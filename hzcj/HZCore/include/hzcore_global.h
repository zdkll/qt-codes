#ifndef HZCORE_GLOBAL_H
#define HZCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HZCORE_LIBRARY)
#  define HZCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define HZCORESHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // HZCORE_GLOBAL_H
