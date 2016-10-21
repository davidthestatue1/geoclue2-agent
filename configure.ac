#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([GeoClueAgent], [0.0.1], [koko.fr.mu@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])


AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])

# Checks for programs.
AC_PROG_CC
AX_REQUIRE_DEFINED([GLIB_GSETTINGS])
GLIB_GSETTINGS

# Checks for libraries.
AM_PATH_GTK_3_0([3.0.0],,AC_MSG_ERROR([Gtk+ 3.0.0 or higher required.]))
PKG_CHECK_MODULES(geoclue_agent_service,
	libgeoclue-2.0 >= 2.3.1
)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])
AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS([libintl.h locale.h ])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])


AC_CONFIG_FILES([
 Makefile
 src/Makefile
 misc/Makefile
 po/Makefile.in
])

AC_OUTPUT
