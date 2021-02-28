#ifndef AUDIOFILE_GLOBAL_H
#define AUDIOFILE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AUDIOFILE_LIBRARY)
#  define AUDIOFILE_EXPORT Q_DECL_EXPORT
#else
#  define AUDIOFILE_EXPORT Q_DECL_IMPORT
#endif

#endif // AUDIOFILE_GLOBAL_H
