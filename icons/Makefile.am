
SUBDIRS = 16x16 32x32 48x48 64x64 128x128 scalable

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook:
	@-if test -z "$(DESTDIR)"; then				\
		echo "Updating gtk icon cache.";		\
		$(gtk_update_icon_cache);				\
	else										\
		echo "*** Icon cache not updated. Remember to run:";	\
		echo "***";								\
		echo "***   $(gtk_update_icon_cache)";	\
		echo "***";								\
	fi

uninstall-local:
	rm -f $(DESTDIR)$(datadir)/icons/hicolor/icon-theme.cache;
