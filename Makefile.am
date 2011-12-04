bin_PROGRAMS = awf-@GTK_SUFFIX@
awf_@GTK_SUFFIX@_SOURCES = awf.c
awf_@GTK_SUFFIX@_CPPFLAGS = $(GTK_CFLAGS) -D$(GTK_SUFFIX)
awf_@GTK_SUFFIX@_LDADD = $(GTK_LIBS)

desktopdir = $(datadir)/applications
desktop_DATA = awf.desktop

EXTRA_DIST = $(desktop_DATA) \
	AUTHORS COPYING ChangeLog INSTALL NEWS README autogen.sh \
	awf.desktop
