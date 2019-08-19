#pragma once

#if defined(DYNAMIC_LIBRARY_PGAME)
#  define DYNLIB_PGAME Q_DECL_EXPORT
#else
#  define DYNLIB_PGAME Q_DECL_IMPORT
#endif
