#pragma once

#if defined(DYNAMIC_LIBRARY_PRENDER_OPENGL)
#  define DYNLIB_PRENDER_OPENGL Q_DECL_EXPORT
#else
#  define DYNLIB_PRENDER_OPENGL Q_DECL_IMPORT
#endif
