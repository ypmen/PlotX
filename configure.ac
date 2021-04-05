#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([PlotX], [0.0], [ypmen@mpifr-bonn.mpg.de])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/plotx.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

LT_INIT
# Checks for libraries.
AC_CHECK_LIB([png], [png_set_flush], [], [echo "library not found!";exit -1])
AC_CHECK_LIB([cfitsio], [ffgky], [], [echo "cfitsio library was not found!"])
BEAR_LIB_PGPLOT([echo "library found"], [echo "library not found!";exit -1])

AC_OPENMP
AX_CHECK_X86_FEATURES

CFLAGS="$CFLAGS $OPENMP_CFLAGS"
CXXFLAGS="$CXXFLAGS $OPENMP_CFLAGS"


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 examples/Makefile])
AC_OUTPUT
