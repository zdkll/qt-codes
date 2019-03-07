﻿#ifndef FINANCE_EXPORT_H
#define FINANCE_EXPORT_H

#include <QtGlobal>

#ifndef HZCOMPONNET_MODULES_STATIC
# if defined(HZCOMPONENT_LIBRARY)
#    define FINANCE_GRAPH_EXPORT Q_DECL_EXPORT
# else
#    define FINANCE_GRAPH_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define FINANCE_GRAPH_EXPORT
#endif


#endif /*FINANCE_GRAPH_EXPORT*/
