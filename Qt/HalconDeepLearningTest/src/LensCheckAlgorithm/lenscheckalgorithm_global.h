#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(LENSCHECKALGORITHM_LIB)
#  define LENSCHECKALGORITHM_EXPORT Q_DECL_EXPORT
# else
#  define LENSCHECKALGORITHM_EXPORT Q_DECL_IMPORT
# endif
#else
# define LENSCHECKALGORITHM_EXPORT
#endif
