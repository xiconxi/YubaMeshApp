#ifndef YBMESH_GLOBAL_H
#define YBMESH_GLOBAL_H
#include <QtCore/QtGlobal>

#if defined(YBEMSH_LIBRARY)
#  define LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // YBMESH_GLOBAL_H
