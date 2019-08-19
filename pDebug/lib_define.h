#pragma once

#if defined(DYNAMIC_LIBRARY_PDEBUG)
#  define DYNLIB_PDEBUG Q_DECL_EXPORT
#else
#  define DYNLIB_PDEBUG Q_DECL_IMPORT
#endif
