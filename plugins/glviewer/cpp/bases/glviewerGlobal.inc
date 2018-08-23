#ifndef GLVIEWER_GLOBAL_H
#define GLVIEWER_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(GLVIEWER_LIBRARY)
#  define LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#define GL_ERROR_HOOK
#define LOAD_BUNNY

#if  defined(Q_OS_OSX)
    #define PLUGINPATH "../PlugIns/"
    #define MESHPATH  "../Mesh/"
#elif defined(Q_OS_WIN)
    #define PLUGINPATH "./bin/"
    #define MESHPATH  "../Mesh/"
#endif

#endif // GLVIEWER_GLOBAL_H
