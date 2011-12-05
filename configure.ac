AC_INIT([A widget factory], [1.0.1])
AC_CONFIG_SRCDIR([src/awf.c])

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

AC_ARG_WITH([gtk3], AS_HELP_STRING([--with-gtk3], [Build with gtk3 instead of gtk2.]))
AS_IF([test "x$with_gtk3" = "xyes"],
      [PKG_CHECK_MODULES(GTK, gtk+-3.0, GTK_SUFFIX=gtk3,
                         AC_MSG_ERROR([gtk3 is required to compile awf]))],
      [PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.24, GTK_SUFFIX=gtk2,
                         AC_MSG_ERROR([gtk2.24 is required to compile awf]))])

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_SUFFIX)

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
