/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INFOPAGE_EXPORT_H
#define INFOPAGE_EXPORT_H


#include <QtGlobal>

#ifndef HZINFOPAGE_MODULES_STATIC
# if defined(HZCOMPONENT_LIBRARY)
#    define HZINFOPAGE_EXPORT Q_DECL_EXPORT
# else
#    define HZINFOPAGE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define HZINFOPAGE_EXPORT
#endif

#endif // INFOPAGE_EXPORT_H
