#pragma once

#if defined(DYNAMIC_LIBRARY_PCORE)
#  define DYNLIB_PCORE Q_DECL_EXPORT
#else
#  define DYNLIB_PCORE Q_DECL_IMPORT
#endif
