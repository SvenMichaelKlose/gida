dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT
AC_INIT([gida], [0.0.2], [pixel@hugbox.org])
AM_INIT_AUTOMAKE([foreign])

#AC_CONFIG_HEADER(includes/config.h)
CXXFLAGS="-Wall -O3"
AC_CONFIG_MACRO_DIR([m4])
AM_PROG_LIBTOOL

AC_SUBST(SUBDIRS)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl Checks for libraries.
dnl Replace `main' with a function in -lc:
AC_CHECK_LIB(c, main)
dnl Replace `main' with a function in -lm:
AC_CHECK_LIB(m, main)

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.

AC_OUTPUT(Makefile src/Makefile src/cpu/Makefile src/vcore/Makefile)
