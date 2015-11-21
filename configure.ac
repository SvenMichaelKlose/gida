dnl Process this file with autoconf to produce a configure script.
AC_INIT
AM_INIT_AUTOMAKE(gida,0.0.2)
#AC_CONFIG_HEADER(includes/config.h)
CXXFLAGS="-Wall -O2 -pipe"
#CXXFLAGS="-Wall -O0 -pipe -g"

AM_PROG_LIBTOOL

AC_SUBST(SUBDIRS)
#AC_SUBST(STRIP)

dnl Checks for programs.
AC_CHECK_PROG(SHELL,sh)
AC_CHECK_PROG(PERL,perl5 perl)
#AC_CHECK_PROG(STRIP,strip)
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
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
