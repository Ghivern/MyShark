#ifndef DISSECTOR_GLOBAL_H
#define DISSECTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DISSECTOR_LIBRARY)
#  define DISSECTOR_EXPORT Q_DECL_EXPORT
#else
#  define DISSECTOR_EXPORT Q_DECL_IMPORT
#endif

#endif // DISSECTOR_GLOBAL_H
