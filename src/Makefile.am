bin_PROGRAMS =

if GTK4
bin_PROGRAMS += awf-gtk@GTK4_SUFFIX@
awf_gtk@GTK4_SUFFIX@_SOURCES = awf.c
awf_gtk@GTK4_SUFFIX@_CPPFLAGS = $(GTK4_CFLAGS) -I/usr/include/gtk-4.0/unix-print
awf_gtk@GTK4_SUFFIX@_LDADD = $(GTK4_LIBS)
endif

if GTK3
bin_PROGRAMS += awf-gtk@GTK3_SUFFIX@
awf_gtk@GTK3_SUFFIX@_SOURCES = awf.c
awf_gtk@GTK3_SUFFIX@_CPPFLAGS = $(GTK3_CFLAGS) -I/usr/include/gtk-3.0/unix-print
awf_gtk@GTK3_SUFFIX@_LDADD = $(GTK3_LIBS)
endif

if GTK2
bin_PROGRAMS += awf-gtk@GTK2_SUFFIX@
awf_gtk@GTK2_SUFFIX@_SOURCES = awf.c
awf_gtk@GTK2_SUFFIX@_CPPFLAGS = $(GTK2_CFLAGS) -I/usr/include/gtk-unix-print-2.0
awf_gtk@GTK2_SUFFIX@_LDADD = $(GTK2_LIBS)
endif