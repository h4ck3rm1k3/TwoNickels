AC_CONFIG_MACRO_DIR([m4])

m4_define([DIME_MAJOR], [0])
m4_define([DIME_MINOR], [9])
m4_define([DIME_MICRO], [1])
m4_define([DIME_BETA], [])
m4_define([DIME_VERSION_STRING], [DIME_MAJOR.DIME_MINOR.DIME_MICRO[]DIME_BETA])

m4_define([DIME_ABI_VERSION], [0])
m4_define([DIME_ABI_AGE], [0])
m4_define([DIME_ABI_REVISION], [0])

# **************************************************************************

AC_INIT([dime], [DIME_VERSION_STRING], [dime-support@coin3d.org])

AC_CONFIG_SRCDIR([include/dime/Base.h])
AC_CONFIG_AUX_DIR([cfg])

AC_CANONICAL_SYSTEM

AC_SUBST([DIME_MAJOR_VERSION], [DIME_MAJOR])
AC_SUBST([DIME_MINOR_VERSION], [DIME_MINOR])
AC_SUBST([DIME_MICRO_VERSION], [DIME_MICRO])
AC_SUBST([DIME_BETA_VERSION], [DIME_MICRO])
AC_SUBST([DIME_VERSION], [DIME_VERSION_STRING])

VERSION=$DIME_VERSION

AC_SUBST(DIME_INTERFACE_AGE, 0)
AC_SUBST(DIME_BINARY_AGE, 0)

dnl  Locate C++ compiler and set C++ as the default compiler to run
dnl  in tests.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG([C++])

dnl *******************************************************************
dnl * Library version control.
dnl *******************************************************************

dnl  Making releases:
dnl    DIME_MICRO_VERSION += 1;
dnl    DIME_INTERFACE_AGE += 1;
dnl    DIME_BINARY_AGE += 1;
dnl  if any functions have been added, set DIME_INTERFACE_AGE to 0.
dnl  if backwards compatibility has been broken,
dnl  set DIME_BINARY_AGE _and_ DIME_INTERFACE_AGE to 0.
dnl 

dnl  libtool versioning
LT_RELEASE=$DIME_MAJOR_VERSION.$DIME_MINOR_VERSION
LT_CURRENT=`expr $DIME_MICRO_VERSION - $DIME_INTERFACE_AGE`
LT_REVISION=$DIME_INTERFACE_AGE
LT_AGE=`expr $DIME_BINARY_AGE - $DIME_INTERFACE_AGE`
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_SUBST([SUFFIX], [])

#  These are used for constructing the dime-config file.
AC_SUBST([DIME_EXTRA_CPPFLAGS], [$CPPFLAGS])
AC_SUBST([DIME_EXTRA_CFLAGS], [$CFLAGS])
AC_SUBST([DIME_EXTRA_CXXFLAGS], [$CXXFLAGS])
AC_SUBST([DIME_EXTRA_LDFLAGS], [$LDFLAGS])
AC_SUBST([DIME_EXTRA_LIBS], [$LIBS])

dnl *******************************************************************
dnl * Miscellaneous options and initializations.
dnl *******************************************************************

AM_INIT_AUTOMAKE(libdime, $VERSION)

dnl  Default to not building a static library.
dnl  Can be overridden by the user with --enable-static.
AM_DISABLE_STATIC

dnl  Turn off default maintainer make-rules -- use ./autogen.sh instead.
AM_MAINTAINER_MODE

if $BUILD_WITH_MSVC; then
  RANLIB=:
  export RANLIB
fi

dnl  Initialize libtool
AC_PROG_LIBTOOL

CPPFLAGS="$CPPFLAGS -DDIME_INTERNAL"

sim_ac_build_library=true

dime_build_dir=`pwd`
dime_src_dir=`cd "$srcdir"; pwd`


dnl *******************************************************************
dnl * Enable/disable compilation with debug information.
dnl *******************************************************************

AC_ARG_ENABLE(debug,
  [  --enable-debug          turn on Dime debugging [default=yes]],
  [case "${enableval}" in
    yes) enable_debug=yes ;;
    no)  enable_debug=no ;;
    *) AC_MSG_ERROR(bad value "${enableval}" for --enable-debug) ;;
  esac],
  enable_debug=yes)

if test "x$enable_debug" = "xno"; then
  CXXFLAGS="$CXXFLAGS -DNDEBUG"
fi


dnl *******************************************************************
dnl * Enable/disable compilation with symbol information.
dnl *******************************************************************

if test "x$ac_cv_prog_cxx_g" = "xyes"; then
  AC_ARG_ENABLE(symbols,
    [  --enable-symbols        turn on symbol debug information [default=yes]],
    [case "${enableval}" in
      yes) enable_symbols=yes ;;
      no)  enable_symbols=no ;;
      *) AC_MSG_ERROR(bad value "${enableval}" for --enable-symbols) ;;
    esac],
    enable_symbols=yes)

  if test "x$enable_symbols" = "xno"; then
    CXXFLAGS="`echo $CXXFLAGS | sed 's/-g//'`"
  fi
fi


dnl *******************************************************************
dnl * Enable/disable compilation with profiling information
dnl * for parsing by gprof (this is a g++ only option).
dnl *******************************************************************

if test "x$ac_cv_prog_cxx_g" = "xyes"; then
  AC_ARG_ENABLE(profile,
    [  --enable-profile        turn on inclusion of profiling code [default=no]],
    [case "${enableval}" in
      yes) enable_profile=yes ;;
      no)  enable_profile=no ;;
      *) AC_MSG_ERROR(bad value "${enableval}" for --enable-profile) ;;
    esac],
    enable_profile=no)

  if test "x$enable_profile" = "xyes"; then
    CXXFLAGS="$CXXFLAGS -pg"
    LDFLAGS="$LDFLAGS -pg"
  fi
fi

dnl *******************************************************************
dnl * Enable/disable compilation with warnings turned on (g++ only).
dnl *******************************************************************

if test "x$ac_cv_prog_gxx" = "xyes"; then
  AC_ARG_ENABLE(warnings,
    [  --enable-warnings       turn on warnings when compiling [default=no]],
    [case "${enableval}" in
      yes) enable_warnings=yes ;;
      no)  enable_warnings=no ;;
      *) AC_MSG_ERROR(bad value "${enableval}" for --enable-warnings) ;;
    esac],
    enable_warnings=no)

  if test "x$enable_warnings" = "xyes"; then
    CXXFLAGS="$CXXFLAGS -W -Wall"
  fi
fi

# **************************************************************************
# Headers we might want to use

AC_CHECK_HEADERS(
  [ieeefp.h], 
  [], [], 
  [ ])

# **************************************************************************

# We want to use BSD 4.3's isinf(), isnan(), finite() if they are
# available.

AC_MSG_CHECKING([for BSD 4.3 isinf() function])
AC_TRY_LINK(
 [#include <math.h>],
 [int result = isinf(42.0);],
 [AC_DEFINE(HAVE_ISINF, 1, [whether or not isinf() is available])
  AC_MSG_RESULT([available])],
 [AC_MSG_RESULT([not available])])

AC_MSG_CHECKING([for BSD 4.3 isnan() function])
AC_TRY_LINK(
 [#include <math.h>],
 [int result = isnan(42.0);],
 [AC_DEFINE(HAVE_ISNAN, 1, [whether or not isnan() is available])
  AC_MSG_RESULT([available])],
 [AC_MSG_RESULT([not available])

  AC_MSG_CHECKING([for _isnan() function])
  AC_TRY_LINK(
   [#include <math.h>],
   [int result = _isnan(42.0);],
   [AC_DEFINE(HAVE__ISNAN, 1, [whether or not _isnan() is available])
    AC_MSG_RESULT([available])],
   [AC_MSG_RESULT([not available])])
])

AC_MSG_CHECKING([for BSD 4.3 finite() function])
AC_TRY_LINK(
 [#include <math.h>],
 [int result = finite(42.0);],
 [AC_DEFINE(HAVE_FINITE, 1, [whether or not finite() is available])
  AC_MSG_RESULT([available])],
 [AC_MSG_RESULT([not available])

  AC_MSG_CHECKING([for _finite() function])
  AC_TRY_LINK(
   [#include <math.h>],
   [int result = _finite(42.0);],
   [AC_DEFINE(HAVE__FINITE, 1, [whether or not _finite() is available])
    AC_MSG_RESULT([available])],
   [AC_MSG_RESULT([not available])])
])

AC_MSG_CHECKING([for fpclass() function])
AC_TRY_LINK(
[#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif],
 [int result = fpclass(42.0);],
 [AC_DEFINE(HAVE_FPCLASS, 1, [whether or not fpclass() is available])
  AC_MSG_RESULT([available])],
 [AC_MSG_RESULT([not available])])

AC_MSG_CHECKING([for _fpclass() function])
AC_TRY_LINK(
 [#include <math.h>],
 [int result = _fpclass(42.0);],
 [AC_DEFINE(HAVE__FPCLASS, 1, [whether or not _fpclass() is available])
  AC_MSG_RESULT([available])],
 [AC_MSG_RESULT([not available])])


# **************************************************************************
# SIM_AC_UNIQIFY_LIST( VARIABLE, LIST )
#
# This macro filters out redundant items from a list.  This macro was made
# to avoid having multiple equivalent -I and -L options for the compiler on
# the command-line, which made compilation quite messy to watch.
#
# BUGS:
#   Items with spaces are probably not supported.
#
# Authors:
#   Lars J. Aas <larsa@sim.no>
#

AC_DEFUN([SIM_AC_UNIQIFY_LIST], [
sim_ac_save_prefix=$prefix
sim_ac_save_exec_prefix=$exec_prefix
test x"$prefix" = xNONE && prefix=/usr/local
test x"$exec_prefix" = xNONE && exec_prefix='${prefix}'
sim_ac_uniqued_list=
for sim_ac_item in $2; do
  eval sim_ac_eval_item="$sim_ac_item"
  eval sim_ac_eval_item="$sim_ac_eval_item"
  if test x"$sim_ac_uniqued_list" = x; then
    sim_ac_uniqued_list="$sim_ac_item"
  else
    sim_ac_unique=true
    for sim_ac_uniq in $sim_ac_uniqued_list; do
      eval sim_ac_eval_uniq="$sim_ac_uniq"
      eval sim_ac_eval_uniq="$sim_ac_eval_uniq"
      test x"$sim_ac_eval_item" = x"$sim_ac_eval_uniq" && sim_ac_unique=false
    done
    $sim_ac_unique && sim_ac_uniqued_list="$sim_ac_uniqued_list $sim_ac_item"
  fi
done
$1=$sim_ac_uniqued_list
prefix=$sim_ac_save_prefix
exec_prefix=$sim_ac_save_exec_prefix
# unset sim_ac_save_prefix
# unset sim_ac_save_exec_prefix
# unset sim_ac_eval_item
# unset sim_ac_eval_uniq
]) # SIM_AC_UNIQIFY_LIST


SIM_AC_UNIQIFY_LIST(DIME_EXTRA_CPPFLAGS, -I$includedir $DIME_EXTRA_CPPFLAGS)
SIM_AC_UNIQIFY_LIST(DIME_EXTRA_LDFLAGS, -L$libdir $DIME_EXTRA_LDFLAGS)
SIM_AC_UNIQIFY_LIST(DIME_EXTRA_LIBS, $DIME_EXTRA_LIBS)
SIM_AC_UNIQIFY_LIST(CPPFLAGS, $CPPFLAGS)
SIM_AC_UNIQIFY_LIST(LDFLAGS, $LDFLAGS)
SIM_AC_UNIQIFY_LIST(LIBS, $LIBS)

# **************************************************************************

# no DLL setup yet
case $enable_static in
yes | true)  DIME_STATIC=true ;;
*)           DIME_STATIC=false ;;
esac

case $enable_shared in
yes | true)  DIME_STATIC=false ;;
*)           DIME_STATIC=true ;;
esac

AC_SUBST([LIBFLAGS], [])

# **************************************************************************

# **************************************************************************

#SIM_AC_MSVC_DSP_SETUP([DIME], [Dime], [dime], [-I$dime_build_dir -I$dime_build_dir/include -I$dime_src_dir/include])

# **************************************************************************

AM_CONFIG_HEADER([include/config.h])

AC_CONFIG_FILES([
	Makefile
	bin/Makefile
	src/Makefile
	src/classes/Makefile
	src/entities/Makefile
	src/objects/Makefile
	src/records/Makefile
	src/sections/Makefile
	src/tables/Makefile
	src/util/Makefile
	src/convert/Makefile
	dxf2vrml/Makefile
	dxfsphere/Makefile
	dxf2osm/Makefile
])

AC_OUTPUT

# SIM_AC_CONFIGURATION_SUMMARY

if $sim_ac_build_library; then
  echo ""
  echo "Now, run 'make install' to build and install Dime."
  echo ""
else
  echo ""
fi

