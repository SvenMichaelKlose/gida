#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gida], [0.0.3], [pixel@hugbox.org])
AM_INIT_AUTOMAKE([foreign])
LT_INIT
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/cpu/Makefile
                 src/vcore/Makefile])
AC_OUTPUT
