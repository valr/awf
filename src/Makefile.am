bin_PROGRAMS = awf-@GTK3_SUFFIX@ awf-@GTK2_SUFFIX@

awf_@GTK3_SUFFIX@_SOURCES = awf.c gtk_empty.c gtk_empty.h
awf_@GTK3_SUFFIX@_CPPFLAGS = $(GTK3_CFLAGS) -I/usr/include/gtk-3.0/unix-print
awf_@GTK3_SUFFIX@_LDADD = $(GTK3_LIBS)

awf_@GTK2_SUFFIX@_SOURCES = awf.c gtk_empty.c gtk_empty.h
awf_@GTK2_SUFFIX@_CPPFLAGS = $(GTK2_CFLAGS) -I/usr/include/gtk-unix-print-2.0
awf_@GTK2_SUFFIX@_LDADD = $(GTK2_LIBS)
