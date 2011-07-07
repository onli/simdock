#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_include(wxwin.m4)
m4_include(pkg.m4)

AC_INIT(simdock, 1.2, simonedll@yahoo.it)
AC_PREREQ(2.61)
AC_COPYRIGHT([Copyright (C) 2007 Simone Della Longa])

AM_INIT_AUTOMAKE(1.9 -Wall dist-bzip2 no-define)

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([dup2 sqrt])



AC_SUBST(GCONF_REQUIRED, 2.18.0)
AC_SUBST(WNCK_REQUIRED, 2.18.0)




PKG_CHECK_MODULES(DEPS,[
	gconf-2.0 >= $GCONF_REQUIRED,
	libwnck-1.0 >= $WNCK_REQUIRED
])


#CPPFLAGS="$CPPFLAGS $DEPS_CPPFLAGS"
#CXXFLAGS="$CXXFLAGS $DEPS_CXXFLAGS"
#LDFLAGS="$LDFLAGS $DEPS_CFLAGS"
#LIBS="$LIBS $DEPS_LIBS"

echo "
---------------SIMDOCK------------------
Libs:     $LIBS
Ldflags:  $LDFLAGS
Cppflags: $CPPFLAGS
Cxxflags: $CXXFLAGS
---------------SIMDOCK------------------
"


# Checks for libraries.
# AM_INIT_AUTOMAKE

AM_OPTIONS_WXCONFIG
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

WXCONFIG=wx-config
AC_ARG_WITH(wx-config,
[[  --with-wx-config=FILE     Use the given path to wx-config when determining
                            wxWidgets configuration; defaults to "wx-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        WXCONFIG=$withval
    fi
])
AC_MSG_CHECKING([wxWidgets version])
if wxversion=`$WXCONFIG --version`; then
    AC_MSG_RESULT([$wxversion])
else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([wxWidgets is required. Try --with-wx-config.])
fi

WX_CPPFLAGS="`$WXCONFIG --cppflags`"
WX_CXXFLAGS="`$WXCONFIG --cxxflags | sed -e 's/-fno-exceptions//'`"
WX_LIBS="`$WXCONFIG --libs`"

CPPFLAGS="$DEPS_CPPFLAGS $DEPS_CFLAGS $WX_CPPFLAGS "
CXXFLAGS="$DEPS_CXXFLAGS $DEPS_CFLAGS $WX_CPPFLAGS"
LIBS="$DEPS_LIBS $WX_LIBS"



#AC_SUBST(WX_LIBS)

AC_ARG_ENABLE(debug,
[  --enable-debug,  Enable debug build],
[
  debugbuild="y"
  CXXFLAGS="${CXXFLAGS} -ggdb"
])

AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)

echo "
---------------SIMDOCK------------------
Libs:     $LIBS
Ldflags:  $LDFLAGS
Cppflags: $CPPFLAGS
Cxxflags: $CXXFLAGS
---------------SIMDOCK------------------
"



AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
