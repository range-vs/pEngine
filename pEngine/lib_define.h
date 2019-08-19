#pragma once

// qt
#include <QtCore/qglobal.h>

#if defined(PENGINE_LIBRARY_PENGINE)
#  define DYNLIB_PENGINE Q_DECL_EXPORT
#else
#  define DYNLIB_PENGINE Q_DECL_IMPORT
#endif

