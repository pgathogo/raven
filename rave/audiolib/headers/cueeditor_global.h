#ifndef CUEEDITOR_GLOBAL_H
#define CUEEDITOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CUEEDITOR_LIBRARY)
#  define CUEEDITOR_EXPORT Q_DECL_EXPORT
#else
#  define CUEEDITOR_EXPORT Q_DECL_IMPORT
#endif

#endif // CUEEDITOR_GLOBAL_H
