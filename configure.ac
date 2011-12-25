AC_INIT([A widget factory], [1.2.1])
AC_CONFIG_SRCDIR([src/awf.c])

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

PKG_CHECK_MODULES(GTK3, gtk+-3.0, GTK3_SUFFIX=gtk3,
					AC_MSG_ERROR([gtk3 is required to compile awf]))

AC_SUBST(GTK3_CFLAGS)
AC_SUBST(GTK3_LIBS)
AC_SUBST(GTK3_SUFFIX)

PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= 2.24, GTK2_SUFFIX=gtk2,
					AC_MSG_ERROR([gtk2.24 is required to compile awf]))

AC_SUBST(GTK2_CFLAGS)
AC_SUBST(GTK2_LIBS)
AC_SUBST(GTK2_SUFFIX)

AC_CONFIG_FILES([
Makefile
src/Makefile
icons/Makefile
icons/16x16/Makefile
icons/22x22/Makefile
icons/24x24/Makefile
icons/32x32/Makefile
icons/48x48/Makefile
icons/64x64/Makefile
icons/128x128/Makefile
icons/scalable/Makefile
])

AC_OUTPUT
