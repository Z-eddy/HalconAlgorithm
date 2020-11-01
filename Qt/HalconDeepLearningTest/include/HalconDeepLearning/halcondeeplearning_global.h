#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(HALCONDEEPLEARNING_LIB)
#  define HALCONDEEPLEARNING_EXPORT Q_DECL_EXPORT
# else
#  define HALCONDEEPLEARNING_EXPORT Q_DECL_IMPORT
# endif
#else
# define HALCONDEEPLEARNING_EXPORT
#endif
