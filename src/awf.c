/**
 * Copyright 2020      | Fabrice Creuzot (luigifab) <code~luigifab~fr>
 * https://github.com/luigifab/awf
 *
 * Copyright 2011-2017 | Valère Monseur <valere~monseur~ymail~com>
 * https://github.com/valr/awf
 *
 * AWF is originally based on the code from The Widget Factory created by Richard Stellingwerff <remenic~gmail~com>
 *
 * This program is free software, you can redistribute it or modify
 * it under the terms of the GNU General Public License (GPL) as published
 * by the free software foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty, without even the implied warranty of
 * merchantability or fitness for a particular purpose. See the
 * GNU General Public License (GPL) for more details.
 *
 *
 * The Widget Factory is a program designed to show a wide range of widgets on a single window.
 *
 * Translations update:
 *  xgettext -d awf -o src/awf.pot -k_ -s src/awf.c
 *  cp src/awf.pot src/fr/awf.po
 *  msgmerge src/fr/awf.po src/awf.pot -o src/fr/awf.po
 *  msgfmt src/fr/awf.po -o src/fr/LC_MESSAGES/awf.mo
 *
 * GTK versions tested:
 *  Ubuntu 11.04 (live) GTK 3.0  + GTK 2.24 + GLIB 2.28
 *  Ubuntu 11.10 (live) GTK 3.2  + GTK 2.24 + GLIB 2.30
 *  Ubuntu 12.04 (live) GTK 3.4  + GTK 2.24 + GLIB 2.32
 *  Ubuntu 13.04 (live) GTK 3.6  + GTK 2.24 + GLIB 2.36
 *  Ubuntu 13.10 (live) GTK 3.8  + GTK 2.24 + GLIB 2.38
 *  Ubuntu 14.04 (live) GTK 3.10
 *  Ubuntu 14.10 (live) GTK 3.12
 *  Ubuntu 15.04 (live) GTK 3.14
 *  Ubuntu 15.10 (live) GTK 3.16
 *  Ubuntu 16.04 (live) GTK 3.18
 *  Ubuntu 16.10 (live) GTK 3.20
 *  Debian Testing      GTK 3.24 + GTK 3.98 + GTK 2.24
 */

// includes

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gtk/gtkunixprint.h>
#include <gdk/gdk.h>
#include <time.h>

#if GLIB_CHECK_VERSION (2,30,0)
	#include <glib-unix.h>
#endif

// defines

#define _(String) gettext (String)
#define GETTEXT_PACKAGE "awf"

#if !defined(G_SOURCE_CONTINUE)
	#define G_SOURCE_CONTINUE TRUE // glib >= 2.32
#endif
#if GTK_CHECK_VERSION (3,2,0)
	#define BOXH (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0))
	#define BOXV (gtk_box_new (GTK_ORIENTATION_VERTICAL, 0))
	#define BOXHSEP (gtk_separator_new (GTK_ORIENTATION_HORIZONTAL))
	#define BOXVSEP (gtk_separator_new (GTK_ORIENTATION_VERTICAL))
	#define BOXHPANE (gtk_paned_new (GTK_ORIENTATION_HORIZONTAL))
	#define BOXVPANE (gtk_paned_new (GTK_ORIENTATION_VERTICAL))
#else
	#define BOXH (gtk_hbox_new (FALSE, 0))
	#define BOXV (gtk_vbox_new (FALSE, 0))
	#define BOXHSEP (gtk_hseparator_new ())
	#define BOXVSEP (gtk_vseparator_new ())
	#define BOXHPANE (gtk_hpaned_new ())
	#define BOXVPANE (gtk_vpaned_new ())
#endif

// enums

enum {
	COLUMN1 = 0,
	COLUMN2,
	NUM_COLS
};

// global local variables

static gchar *current_theme;
static GSList *list_system_theme;
static GSList *list_user_theme;
static GtkWidget *window, *statusbar;
static GtkWidget *progressbar1, *progressbar2, *progressbar3, *progressbar4;
static GtkWidget *scale1, *scale2, *scale3, *scale4, *scale5, *scale6;
static GtkWidget *levelbar1, *levelbar2, *levelbar3, *levelbar4, *levelbar5, *levelbar6, *levelbar7, *levelbar8;
static GtkWidget *notebook1, *notebook2, *notebook3, *notebook4;
static gchar *screenshot;

// local functions

static GSList* awf_load_theme (gchar *directory);
static void awf_exclude_theme (gpointer theme, gpointer unused);
static gint awf_compare_theme (gconstpointer theme1, gconstpointer theme2);
static void awf_set_theme (gpointer theme, gpointer unused);
static void awf_refresh_theme (GtkWidget *unused1, gpointer unused2);
static gboolean awf_sighup_handler (gpointer unused);
static void awf_update_progressbars (GtkRange *range, gpointer unused);
static void awf2_update_statusbar (gchar *text, gboolean withtime);
static void awf2_update_widgets (GtkWidget *widget, gpointer unused);
static gboolean awf2_take_screenshot (gpointer unused);
// widgets
static void awf2_create_window ();
static void awf2_boxpack (GtkBox *box, GtkWidget *widget, gboolean fill, gboolean expand, guint padding, guint spacing);
static void awf2_create_menubar (GtkWidget *menubar);
static void awf2_create_toolbar (GtkWidget *root);
static void awf2_create_combos_entries (GtkWidget *root);
static void awf2_create_spinbuttons (GtkWidget *root);
static void awf2_create_checkbuttons (GtkWidget *root);
static void awf2_create_radiobuttons (GtkWidget *root);
static void awf2_create_otherbuttons (GtkWidget *root);
static void awf2_create_progressbars (GtkWidget *vroot1, GtkWidget *vroot2, GtkWidget *hroot1, GtkWidget *hroot2);
static void awf2_create_labels (GtkWidget *root);
static void awf2_create_spinners (GtkWidget *root);
static void awf2_create_expander (GtkWidget *root);
static void awf2_create_frames (GtkWidget *lroot, GtkWidget *rroot);
static void awf2_create_notebooks (GtkWidget *lroot, GtkWidget *rroot);
static void awf2_create_treview (GtkWidget *root);
// menuitems
static GtkWidget* awf2_new_menu (GtkWidget *root, gchar *text);
static GtkWidget* awf2_new_menu_tearoff (GtkWidget *menu);
static GtkWidget* awf2_new_menu_separator (GtkWidget *menu);
static GtkWidget* awf2_new_menu_check (GtkWidget *menu, gchar *text, gboolean checked, gboolean inconsistent, gboolean disabled);
static GtkWidget* awf2_new_menu_radio (GtkWidget *menu, gchar *text, gboolean checked, gboolean inconsistent, gboolean disabled, GSList *group);
static GtkWidget* awf2_new_menu_item (GtkWidget *menu, gchar *text, gchar *accel, gchar *image, gboolean disabled);
// dialogs
static void awf2_show_dialog_open (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_open_recent (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_save (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_properties (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_page_setup (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_print (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_about (GtkWidget *widget, gpointer unused);
static void awf2_show_dialog_calendar (GtkWidget *widget, gpointer unused);
// gtk3
#if GTK_CHECK_VERSION (3,4,0) && !GTK_CHECK_VERSION (3,98,0)
static void awf2_scroll_notebook_tabs (GtkWidget *widget, GdkEventScroll *event);
#endif

// run baby, run!

int main (int argc, char **argv) {

	gint opt;
	gchar *directory;
	GSList *iterator;

	// load themes
	list_system_theme = awf_load_theme ("/usr/share/themes");
	list_system_theme = g_slist_sort (list_system_theme, (GCompareFunc)awf_compare_theme);

	directory = g_build_path ("/", g_getenv ("HOME"), ".themes", NULL);
	list_user_theme = awf_load_theme (directory);
	list_user_theme = g_slist_sort (list_user_theme, (GCompareFunc)awf_compare_theme);
	g_free (directory);

	g_slist_foreach (list_user_theme, awf_exclude_theme, NULL);

	// init
	while ((opt = getopt (argc, argv, "vs:t:lh")) != -1) {
		switch (opt) {
			case 'v':
				g_printf ("%s\n", VERSION);
				return 0;
			case 's':
				screenshot = optarg;
				break;
			case 't':
				if (g_slist_find_custom (list_system_theme, optarg, &awf_compare_theme) ||
					g_slist_find_custom (list_user_theme, optarg, &awf_compare_theme))
					awf_set_theme (optarg, NULL);
				break;
			case 'l':
				g_printf ("Available themes:\n");
				for (iterator = list_system_theme; iterator; iterator = iterator->next)
					g_printf (" %s\n", (gchar*)iterator->data);
				for (iterator = list_user_theme; iterator; iterator = iterator->next)
					g_printf (" %s\n", (gchar*)iterator->data);
				return 0;
			case 'h':
			default:
				g_printf ("\n");
				g_printf ("A widget factory %s (theme is reloaded on sighup)\n", VERSION);
				g_printf (" compiled with gtk %d.%d.%d and glib %d.%d.%d\n",
					GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION,
					GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
				g_printf ("  started with gtk %d.%d.%d and glib %d.%d.%d\n\n",
					#if GTK_CHECK_VERSION (3,0,0)
						gtk_get_major_version (), gtk_get_minor_version (), gtk_get_micro_version (),
					#else
						gtk_major_version, gtk_minor_version, gtk_micro_version,
					#endif
					glib_major_version, glib_minor_version, glib_micro_version);
				g_printf ("Usage: awf-gtk2 (for gtk 2.24+) or awf-gtk3 (for gtk 3.0+) or awf-gtk4 (for gtk 3.98+)\n");
				g_printf (" %s %s\n", "-v ", "Show version number (and quit)");
				g_printf (" %s %s\n", "-l ", "List available themes (and quit)");
				g_printf (" %s %s\n", "-t <theme> ", "Run with the specified theme");
				g_printf (" %s %s\n", "-s <filename>", "Run and take/save a png screenshot on sighup");
				g_printf ("\n");
				return 0;
		}
	}

	#if GTK_CHECK_VERSION (3,98,0)
		gtk_init ();
	#else
		gtk_init (&argc, &argv);
	#endif

	// locale
	setlocale (LC_ALL, "");
	if (g_file_test ("/usr/share/locale", G_FILE_TEST_IS_DIR))
		bindtextdomain(GETTEXT_PACKAGE, "/usr/share/locale");
	if (g_file_test ("/var/www/awf/src", G_FILE_TEST_IS_DIR))
		bindtextdomain(GETTEXT_PACKAGE, "/var/www/awf/src");
	if (g_file_test ("/home/gtk324/awf/src", G_FILE_TEST_IS_DIR))
		bindtextdomain(GETTEXT_PACKAGE, "/home/gtk324/awf/src");
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	// window
	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &current_theme, NULL);
	#if GLIB_CHECK_VERSION (2,30,0)
		g_unix_signal_add (SIGHUP, awf_sighup_handler, NULL); // glib >= 2.30
	#endif
	awf2_create_window ();

	return 0;
}

static GSList* awf_load_theme (gchar *directory) {

	GSList *list = NULL;

	g_return_val_if_fail (directory != NULL, NULL);

	if (g_file_test (directory, G_FILE_TEST_IS_DIR)) {

		GError *error = NULL;
		GDir *dir = g_dir_open (directory, 0, &error);

		if (dir) {
			gchar *theme = g_strdup (g_dir_read_name (dir));

			while (theme) {
				gchar *theme_path = g_build_path ("/", directory, g_strstrip (theme), NULL);

				if (g_file_test (theme_path, G_FILE_TEST_IS_DIR)) {
					#if GTK_CHECK_VERSION (3,0,0)
						gchar *theme_subpath = g_build_path ("/", theme_path, "gtk-3.0", NULL);
					#else
						gchar *theme_subpath = g_build_path ("/", theme_path, "gtk-2.0", NULL);
					#endif
					if (g_file_test (theme_subpath, G_FILE_TEST_IS_DIR))
						list = g_slist_prepend (list, theme);

					g_free (theme_subpath);
				}

				g_free (theme_path);
				// don't free 'theme', it's the data part of GSList elements

				theme = g_strdup (g_dir_read_name (dir));
			}

			g_dir_close (dir);
		}

		if (error) {
			g_fprintf (stderr, "unable to open directory: %s (%s)\n", directory, error->message);
			g_error_free (error); error = NULL;
		}
	}

	if (list)
		list = g_slist_reverse (list);

	return list;
}

static void awf_exclude_theme (gpointer theme, gpointer unused) {

	GSList *found_theme = g_slist_find_custom (list_system_theme, (gconstpointer)theme, &awf_compare_theme);

	if (found_theme) {
		g_free (found_theme->data);
		list_system_theme = g_slist_delete_link (list_system_theme, found_theme);
	}
}

static gint awf_compare_theme (gconstpointer theme1, gconstpointer theme2) {

	return g_strcmp0 ((gchar*)theme1, (gchar*)theme2);
}

static void awf_set_theme (gpointer theme, gpointer unused) {

	g_object_set (gtk_settings_get_default (), "gtk-theme-name", (gchar*)theme,  NULL);
	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &current_theme, NULL);

	if (window)
		gtk_window_resize (GTK_WINDOW (window), 50, 50);

	if (statusbar)
		awf2_update_statusbar (g_strdup_printf (_("Theme %s loaded."), current_theme), FALSE);
}

static void awf_refresh_theme (GtkWidget *unused1, gpointer unused2) {

	gchar *default_theme = NULL;

	if (g_slist_find_custom (list_system_theme, "Default", &awf_compare_theme))
		default_theme = "Default";
	else if (g_slist_find_custom (list_system_theme, "Raleigh", &awf_compare_theme))
		default_theme = "Raleigh";

	if (default_theme) {

		g_object_set (gtk_settings_get_default (), "gtk-theme-name", default_theme, NULL);
		g_usleep (G_USEC_PER_SEC / 2);
		g_object_set (gtk_settings_get_default (), "gtk-theme-name", current_theme, NULL);

		awf2_update_statusbar (g_strdup_printf (_("Theme %s reloaded at"), current_theme), TRUE);

		if (screenshot)
			g_timeout_add_seconds (1, awf2_take_screenshot, NULL);
	}
	else {
		g_warning ("No default theme found (neither \"Default\" nor \"Raleigh\"), refresh of theme not work.");
	}
}

static gboolean awf_sighup_handler (gpointer unused) {

	awf_refresh_theme (NULL, NULL);
	return G_SOURCE_CONTINUE;
}

static void awf_update_progressbars (GtkRange *range, gpointer unused) {

	gdouble value;
	value = gtk_range_get_value (range);

	// range(0..1)
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), value / 100.0);

	// range(0..100)
	if (scale1 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale1), value);
	if (scale2 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale2), value);
	if (scale3 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale3), value);
	if (scale4 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale4), value);
	if (scale5 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale5), value);
	if (scale6 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale6), value);

	#if GTK_CHECK_VERSION (3,6,0)
		// range(0..1)
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar1), value / 100.0);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar2), value / 100.0);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar5), value / 100.0);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar6), value / 100.0);
		// range(0..5)
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar3), value / 100.0 * gtk_level_bar_get_max_value (GTK_LEVEL_BAR (levelbar3)));
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar4), value / 100.0 * gtk_level_bar_get_max_value (GTK_LEVEL_BAR (levelbar4)));
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar7), value / 100.0 * gtk_level_bar_get_max_value (GTK_LEVEL_BAR (levelbar7)));
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar8), value / 100.0 * gtk_level_bar_get_max_value (GTK_LEVEL_BAR (levelbar8)));
	#endif

	#if !GTK_CHECK_VERSION (3,0,0)
		if (gtk_progress_bar_get_text (GTK_PROGRESS_BAR (progressbar1))) {
			gchar *progress_text;
			progress_text = g_strdup_printf ("%i %%", (int)value);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), progress_text);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), progress_text);
			g_free (progress_text);
		}
	#endif
}

static void awf2_update_statusbar (gchar *text, gboolean withtime) {

	char buffer[12];
	time_t rawtime;
	struct tm *timeinfo;

	if (withtime) {
		time (&rawtime);
		timeinfo = localtime (&rawtime);
		strftime (buffer, sizeof buffer, " %T.", timeinfo);

		gtk_statusbar_push (GTK_STATUSBAR (statusbar), gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "gné"),
			g_strdup_printf ("%s%s", text, buffer));
	}
	else {
		gtk_statusbar_push (GTK_STATUSBAR (statusbar), gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "gné"), text);
	}
}

static void awf2_update_widgets (GtkWidget *widget, gpointer unused) {

	gchar *progress_text;

	if (gtk_notebook_get_scrollable (GTK_NOTEBOOK (notebook1))) {

		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook1), FALSE);
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook2), FALSE);
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook3), FALSE);
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook4), FALSE);

		#if GTK_CHECK_VERSION (3,0,0)
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar1), FALSE);
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar3), FALSE);
		#else
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), NULL);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), NULL);
		#endif

		gtk_scale_set_draw_value (GTK_SCALE (scale1), FALSE);
		gtk_scale_clear_marks (GTK_SCALE (scale2));
		gtk_scale_set_draw_value (GTK_SCALE (scale3), FALSE);
		gtk_scale_set_draw_value (GTK_SCALE (scale4), FALSE);
		gtk_scale_clear_marks (GTK_SCALE (scale5));
		gtk_scale_clear_marks (GTK_SCALE (scale6));
	}
	else {
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook1), TRUE);
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook2), TRUE);
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook3), TRUE);
		gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook4), TRUE);

		#if GTK_CHECK_VERSION (3,0,0)
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar1), TRUE);
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar3), TRUE);
		#else
			progress_text = g_strdup_printf ("%i %%", (int)gtk_range_get_value (GTK_RANGE (scale1)));
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), progress_text);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), progress_text);
			g_free (progress_text);
		#endif

		gtk_scale_set_draw_value (GTK_SCALE (scale1), TRUE);
		gtk_scale_add_mark (GTK_SCALE (scale2), 25, GTK_POS_TOP, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale2), 50, GTK_POS_TOP, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale2), 75, GTK_POS_TOP, NULL);
		gtk_scale_set_draw_value (GTK_SCALE (scale3), TRUE);
		gtk_scale_set_draw_value (GTK_SCALE (scale4), TRUE);
		gtk_scale_add_mark (GTK_SCALE (scale5), 25, GTK_POS_LEFT, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale5), 50, GTK_POS_LEFT, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale5), 75, GTK_POS_LEFT, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale6), 25, GTK_POS_RIGHT, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale6), 50, GTK_POS_RIGHT, NULL);
		gtk_scale_add_mark (GTK_SCALE (scale6), 75, GTK_POS_RIGHT, NULL);
	}
}

static gboolean awf2_take_screenshot (gpointer unused) {

	#if GTK_CHECK_VERSION (3,98,0)
		GdkSurface *root;
	#else
		GdkWindow *root;
	#endif
	GdkPixbuf *image;
	gint width, height;

	#if GTK_CHECK_VERSION (3,98,0)
		root = gtk_native_get_surface (GTK_NATIVE (window));
		width  = gdk_surface_get_width (root);
		height = gdk_surface_get_height (root);
		//image = gdk_pixbuf_get_from_window (root, 0, 0, width, height);
	#elif GTK_CHECK_VERSION (3,0,0)
		root = gtk_widget_get_window (GTK_WIDGET (window));
		gtk_window_get_size (GTK_WINDOW (window), &width, &height);
		image = gdk_pixbuf_get_from_window (root, 0, 0, width, height);
	#else
		root = gtk_widget_get_window (GTK_WIDGET (window));
		gtk_window_get_size (GTK_WINDOW (window), &width, &height);
		image = gdk_pixbuf_get_from_drawable (NULL, root, gdk_colormap_get_system (), 0, 0, 0, 0, width, height);
	#endif

	if (image) {
 		gdk_pixbuf_save (image, screenshot, "png", NULL, "compression", "9", NULL);
		awf2_update_statusbar (g_strdup_printf (_("Theme reloaded, then screenshot saved (%s) at"), screenshot), TRUE);
	}

	return FALSE;
}

// layout

static void awf2_create_window () {

	// https://developer.gnome.org/gtk3/stable/GtkContainer.html

	GtkWidget *vbox_window, *menubar, *toolbar, *widgets;
	GtkWidget *hbox_columns = BOXH, *vseparator1 = BOXVSEP, *vseparator2 = BOXVSEP, *vseparator3 = BOXVSEP, *hseparator1 = BOXHSEP;
	GtkWidget *vbox_column1 = BOXV, *vbox_combo_entry = BOXV, *hbox_spin = BOXH, *hbox_check_radio = BOXH, *vbox_check = BOXV, *vbox_radio = BOXV;
	GtkWidget *vbox_column2 = BOXV, *vbox_buttons = BOXV;
	GtkWidget *vbox_column3 = BOXV, *vbox_progressbar1 = BOXV, *vbox_progressbar2 = BOXV, *hbox_progressbar1 = BOXH, *hbox_progressbar2 = BOXH;
	GtkWidget *vbox_column4 = BOXV, *vbox_others = BOXV, *hbox_label = BOXH, *hbox_spinner = BOXH;
	GtkWidget *vpane = BOXVPANE, *hpane1 = BOXHPANE, *hpane2 = BOXHPANE;
	GtkWidget *hbox_frame1 = BOXH, *hbox_frame2 = BOXH, *hbox_notebook1 = BOXH, *hbox_notebook2 = BOXH;

	// base
	#if GTK_CHECK_VERSION (3,98,0)
		window = gtk_window_new ();
		g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (g_main_loop_quit), NULL);
	#else
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	#endif

	gtk_window_set_icon_name (GTK_WINDOW (window), "awf");

		vbox_window = BOXV;
		gtk_container_add (GTK_CONTAINER (window), vbox_window);

			#if GTK_CHECK_VERSION (3,98,0)
				menubar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
				gtk_widget_set_margin_top (menubar, 35);
				gtk_container_add (GTK_CONTAINER (vbox_window), menubar);

				toolbar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
				gtk_widget_set_margin_top (menubar, 65);
				gtk_container_add (GTK_CONTAINER (vbox_window), toolbar);
			#else
				menubar = gtk_menu_bar_new ();
				awf2_create_menubar (menubar);
				awf2_boxpack (GTK_BOX (vbox_window), menubar, FALSE, FALSE, 0, 0);

				toolbar = gtk_toolbar_new ();
				awf2_boxpack (GTK_BOX (vbox_window), toolbar, FALSE, FALSE, 0, 0);
				awf2_create_toolbar (toolbar);
			#endif

			widgets = BOXV;
			awf2_boxpack (GTK_BOX (vbox_window), widgets, TRUE, TRUE, 0, 0);

		statusbar = gtk_statusbar_new ();
		awf2_boxpack (GTK_BOX (vbox_window), statusbar, FALSE, FALSE, 0, 0);
		awf2_update_statusbar (g_strdup_printf (_("AWF %s / Theme %s loaded."), VERSION, current_theme), FALSE);

	// columns layout
	awf2_boxpack (GTK_BOX (widgets), hbox_columns, TRUE, TRUE, 0, 0);

		// column 1
		awf2_boxpack (GTK_BOX (hbox_columns), vbox_column1, TRUE, TRUE, 5, 0);
			awf2_boxpack (GTK_BOX (vbox_column1), vbox_combo_entry, FALSE, TRUE, 5, 3);
				awf2_create_combos_entries (vbox_combo_entry);
			awf2_boxpack (GTK_BOX (vbox_column1), hbox_spin, FALSE, FALSE, 5, 0);
				awf2_create_spinbuttons (hbox_spin);
			awf2_boxpack (GTK_BOX (vbox_column1), hbox_check_radio, FALSE, TRUE, 5, 0);
				awf2_boxpack (GTK_BOX (hbox_check_radio), vbox_check, TRUE, TRUE, 0, 0);
					awf2_create_checkbuttons (vbox_check);
				awf2_boxpack (GTK_BOX (hbox_check_radio), vbox_radio, TRUE, TRUE, 0, 0);
					awf2_create_radiobuttons (vbox_radio);
		awf2_boxpack (GTK_BOX (hbox_columns), vseparator1, FALSE, FALSE, 0, 0);

		// column 2
		awf2_boxpack (GTK_BOX (hbox_columns), vbox_column2, TRUE, TRUE, 5, 0);
			awf2_boxpack (GTK_BOX (vbox_column2), vbox_buttons, FALSE, TRUE, 5, 3);
				awf2_create_otherbuttons (vbox_buttons);
		awf2_boxpack (GTK_BOX (hbox_columns), vseparator2, FALSE, FALSE, 0, 0);

		// column 3
		awf2_boxpack (GTK_BOX (hbox_columns), vbox_column3, TRUE, TRUE, 5, 0);
			awf2_boxpack (GTK_BOX (vbox_column3), vbox_progressbar1, FALSE, TRUE, 5, 10);
			awf2_boxpack (GTK_BOX (vbox_column3), hbox_progressbar1, FALSE, FALSE, 5, 10);
			awf2_boxpack (GTK_BOX (vbox_column3), hbox_progressbar2, FALSE, FALSE, 5, 10);
			awf2_boxpack (GTK_BOX (vbox_column3), vbox_progressbar2, FALSE, TRUE, 5, 10);
				awf2_create_progressbars (vbox_progressbar1, vbox_progressbar2, hbox_progressbar1, hbox_progressbar2);
		awf2_boxpack (GTK_BOX (hbox_columns), vseparator3, FALSE, FALSE, 0, 0);

		// column 4
		awf2_boxpack (GTK_BOX (hbox_columns), vbox_column4, TRUE, TRUE, 5, 0);
			awf2_boxpack (GTK_BOX (vbox_column4), vbox_others, FALSE, TRUE, 5, 3);
				awf2_create_treview (vbox_others);
				awf2_boxpack (GTK_BOX (vbox_others), hbox_label, FALSE, TRUE, 5, 0);
					awf2_create_labels (hbox_label);
				awf2_boxpack (GTK_BOX (vbox_others), hbox_spinner, FALSE, TRUE, 5, 0);
					awf2_create_spinners (hbox_spinner);
				awf2_create_expander (vbox_others);

	awf2_boxpack (GTK_BOX (widgets), hseparator1, FALSE, FALSE, 0, 0);

	// paned layout
	awf2_boxpack (GTK_BOX (widgets), vpane, TRUE, TRUE, 0, 0);

		gtk_paned_pack1 (GTK_PANED (vpane), hpane1, TRUE, FALSE);

			gtk_box_set_homogeneous (GTK_BOX (hbox_frame1), TRUE);
			gtk_box_set_spacing (GTK_BOX (hbox_frame1), 3);
			#if GTK_CHECK_VERSION (3,12,0)
				gtk_widget_set_size_request (hbox_frame1, -1, 50);
				gtk_widget_set_margin_start (hbox_frame1, 10);
				gtk_widget_set_margin_end (hbox_frame1, 10);
				gtk_widget_set_margin_top (hbox_frame1, 10);
				gtk_widget_set_margin_bottom (hbox_frame1, 10);
			#else
				gtk_widget_set_size_request (hbox_frame1, -1, 70);
				gtk_container_set_border_width (GTK_CONTAINER (hbox_frame1), 10);
			#endif
			gtk_paned_add1 (GTK_PANED (hpane1), hbox_frame1);

			gtk_box_set_homogeneous (GTK_BOX (hbox_frame2), TRUE);
			gtk_box_set_spacing (GTK_BOX (hbox_frame2), 3);
			#if GTK_CHECK_VERSION (3,12,0)
				gtk_widget_set_size_request (hbox_frame2, -1, 50);
				gtk_widget_set_margin_start (hbox_frame2, 10);
				gtk_widget_set_margin_end (hbox_frame2, 10);
				gtk_widget_set_margin_top (hbox_frame2, 10);
				gtk_widget_set_margin_bottom (hbox_frame2, 10);
			#else
				gtk_widget_set_size_request (hbox_frame2, -1, 70);
				gtk_container_set_border_width (GTK_CONTAINER (hbox_frame2), 10);
			#endif
			gtk_paned_add2 (GTK_PANED (hpane1), hbox_frame2);

			awf2_create_frames (hbox_frame1, hbox_frame2);

		gtk_paned_pack2 (GTK_PANED (vpane), hpane2, TRUE, FALSE);

			gtk_box_set_homogeneous (GTK_BOX (hbox_notebook1), TRUE);
			gtk_box_set_spacing (GTK_BOX (hbox_notebook1), 3);
			#if GTK_CHECK_VERSION (3,12,0)
				gtk_widget_set_margin_start (hbox_notebook1, 10);
				gtk_widget_set_margin_end (hbox_notebook1, 10);
				gtk_widget_set_margin_top (hbox_notebook1, 10);
				gtk_widget_set_margin_bottom (hbox_notebook1, 10);
			#else
				gtk_container_set_border_width (GTK_CONTAINER (hbox_notebook1), 10);
			#endif
			gtk_paned_add1 (GTK_PANED (hpane2), hbox_notebook1);

			gtk_box_set_homogeneous (GTK_BOX (hbox_notebook2), TRUE);
			gtk_box_set_spacing (GTK_BOX (hbox_notebook2), 3);
			#if GTK_CHECK_VERSION (3,12,0)
				gtk_widget_set_margin_start (hbox_notebook2, 10);
				gtk_widget_set_margin_end (hbox_notebook2, 10);
				gtk_widget_set_margin_top (hbox_notebook2, 10);
				gtk_widget_set_margin_bottom (hbox_notebook2, 10);
			#else
				gtk_container_set_border_width (GTK_CONTAINER (hbox_notebook2), 10);
			#endif
			gtk_paned_add2 (GTK_PANED (hpane2), hbox_notebook2);

			awf2_create_notebooks (hbox_notebook1, hbox_notebook2);

	// go
	#if GTK_CHECK_VERSION (3,98,0)
		gtk_window_set_title (GTK_WINDOW (window), _("A widget factory - GTK 4"));
		//gtk_style_context_add_class (gtk_widget_get_style_context (toolbar), "primary-toolbar");
		gtk_widget_show (window);
		while (TRUE) g_main_context_iteration (NULL, TRUE);
	#elif GTK_CHECK_VERSION (3,0,0)
		gtk_window_set_title (GTK_WINDOW (window), _("A widget factory - GTK 3"));
		gtk_style_context_add_class (gtk_widget_get_style_context (toolbar), "primary-toolbar");
		gtk_widget_show_all (window);
		gtk_main ();
	#else
		gtk_window_set_title (GTK_WINDOW (window), _("A widget factory - GTK 2"));
		gtk_widget_show_all (window);
		gtk_main ();
	#endif
}

static void awf2_boxpack (GtkBox *box, GtkWidget *widget, gboolean fill, gboolean expand, guint padding, guint spacing) {

	// https://developer.gnome.org/gtk3/stable/GtkBox.html

	#if GTK_CHECK_VERSION (3,98,0)
		if (fill) {
			gtk_widget_set_halign(widget, GTK_ALIGN_FILL);
			gtk_widget_set_valign(widget, GTK_ALIGN_FILL);
		}
		if (expand) {
			gtk_widget_set_hexpand (widget, TRUE);
			gtk_widget_set_vexpand (widget, TRUE);
		}
		if (GTK_IS_CONTAINER (widget) && (padding > 0)) {
			gtk_widget_set_margin_start (widget, padding);
			gtk_widget_set_margin_end (widget, padding);
			gtk_widget_set_margin_top (widget, padding);
			gtk_widget_set_margin_bottom (widget, padding);
		}
		if (GTK_IS_BOX (widget) && (spacing > 0)) {
			gtk_box_set_spacing (GTK_BOX (widget), spacing);
		}
		gtk_container_add (GTK_CONTAINER (box), widget);
	#else
		if (GTK_IS_INFO_BAR (box))
			box = GTK_BOX (gtk_info_bar_get_content_area (GTK_INFO_BAR (box)));
		if (GTK_IS_CONTAINER (widget) && (padding > 0))
			gtk_container_set_border_width (GTK_CONTAINER (widget), padding);
		if (GTK_IS_BOX (widget) && (spacing > 0))
			gtk_box_set_spacing (GTK_BOX (widget), spacing);
		gtk_box_pack_start (box, widget, fill, expand, 0);
	#endif
}

static void awf2_create_menubar (GtkWidget *menubar) {

	// https://developer.gnome.org/gtk3/stable/GtkMenu.html
	// https://developer.gnome.org/gtk3/stable/GtkMenuItem.html

	GtkWidget *menu, *submenu, *menuitem;
	GSList *iterator, *group = NULL;

	menu = awf2_new_menu (menubar, _("_System theme"));
	for (iterator = list_system_theme; iterator; iterator = iterator->next) {
		menuitem = awf2_new_menu_radio (menu, iterator->data, FALSE, FALSE, FALSE, group);
		#if !GTK_CHECK_VERSION (3,98,0)
			group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem));
		#endif
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf_set_theme), iterator->data);
		#if !GTK_CHECK_VERSION (3,98,0)
			if (strcmp ((gchar*)current_theme, (gchar*)iterator->data) == 0)
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), TRUE);
		#endif
	}

	menu = awf2_new_menu (menubar, _("_User theme"));
	for (iterator = list_user_theme; iterator; iterator = iterator->next) {
		menuitem = awf2_new_menu_radio (menu, iterator->data, FALSE, FALSE, FALSE, group);
		#if !GTK_CHECK_VERSION (3,98,0)
			gtk_radio_menu_item_set_group (GTK_RADIO_MENU_ITEM (menuitem), group);
		#endif
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf_set_theme), iterator->data);
		#if !GTK_CHECK_VERSION (3,98,0)
			if (strcmp ((gchar*)current_theme, (gchar*)iterator->data) == 0)
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), TRUE);
		#endif
	}

	menu = awf2_new_menu (menubar, _("_Options"));
		awf2_new_menu_tearoff (menu);

		menuitem = awf2_new_menu_item (menu, "", "<Control>o", "gtk-open", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_open), NULL);

		menuitem = awf2_new_menu_item (menu, _("Open recent file"), "", "", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_open_recent), NULL);

		menuitem = awf2_new_menu_item (menu, _("Calendar"), "", "", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_calendar), NULL);

		menuitem = awf2_new_menu_item (menu, "", "<Control>s", "gtk-save", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_save), NULL);

		menuitem = awf2_new_menu_item (menu, _("Refresh theme"), "F5", "gtk-refresh", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf_refresh_theme), NULL);

		menuitem = awf2_new_menu_item (menu, "", "<Alt>Return", "gtk-properties", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_properties), NULL);

		menuitem = awf2_new_menu_item (menu, "", "", "gtk-page-setup", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_page_setup), NULL);

		menuitem = awf2_new_menu_item (menu, "", "<Control>p", "gtk-print", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_print), NULL);

		menuitem = awf2_new_menu_item (menu, "", "", "gtk-about", FALSE);
		g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf2_show_dialog_about), NULL);

		submenu = awf2_new_menu (menu, _("More..."));
			awf2_new_menu_item (submenu, "", "<Control>x", "gtk-cut", FALSE);
			awf2_new_menu_item (submenu, "", "<Control>c", "gtk-copy", FALSE);
			awf2_new_menu_item (submenu, "", "<Control>v", "gtk-paste", FALSE);

		awf2_new_menu_separator (menu);
		awf2_new_menu_check (menu, g_strdup_printf ("Check 1 %s", _("(unchecked)")), FALSE, FALSE, FALSE);
		awf2_new_menu_check (menu, g_strdup_printf ("Check 2 %s", _("(checked)")), TRUE, FALSE, FALSE);
		awf2_new_menu_check (menu, g_strdup_printf ("Check 3 %s", _("(inconsistent)")), FALSE, TRUE, FALSE);

		awf2_new_menu_separator (menu);
		awf2_new_menu_check (menu, g_strdup_printf ("Check 1 %s", _("(unchecked)")), FALSE, FALSE, TRUE);
		awf2_new_menu_check (menu, g_strdup_printf ("Check 2 %s", _("(checked)")), TRUE, FALSE, TRUE);
		awf2_new_menu_check (menu, g_strdup_printf ("Check 3 %s", _("(inconsistent)")), FALSE, TRUE, TRUE);

		awf2_new_menu_separator (menu);
		menuitem = awf2_new_menu_radio (menu, g_strdup_printf ("Radio 1 %s", _("(unchecked)")), FALSE, FALSE, FALSE, NULL);
		#if !GTK_CHECK_VERSION (3,98,0)
			group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem));
		#endif
		menuitem = awf2_new_menu_radio (menu, g_strdup_printf ("Radio 2 %s", _("(checked)")), TRUE, FALSE, FALSE, group);
				 awf2_new_menu_radio (menu, g_strdup_printf ("Radio 3 %s", _("(inconsistent)")), FALSE, TRUE, FALSE, NULL);

		awf2_new_menu_separator (menu);
		menuitem = awf2_new_menu_radio (menu, g_strdup_printf ("Radio 1 %s", _("(unchecked)")), FALSE, FALSE, TRUE, NULL);
		#if !GTK_CHECK_VERSION (3,98,0)
			group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem));
		#endif
		menuitem = awf2_new_menu_radio (menu, g_strdup_printf ("Radio 2 %s", _("(checked)")), TRUE, FALSE, TRUE, group);
				 awf2_new_menu_radio (menu, g_strdup_printf ("Radio 3 %s", _("(inconsistent)")), FALSE, TRUE, TRUE, NULL);

		awf2_new_menu_separator (menu);
		awf2_new_menu_item (menu, "", "<Control>w", "gtk-close", TRUE);
		menuitem = awf2_new_menu_item (menu, "", "<Control>q", "gtk-quit", FALSE);
		#if GTK_CHECK_VERSION (3,98,0)
			g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (g_main_loop_quit), NULL);
		#else
			g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (gtk_main_quit), NULL);
		#endif
}

static void awf2_create_toolbar (GtkWidget *root) {

	#if !GTK_CHECK_VERSION (3,98,0)

		// https://developer.gnome.org/gtk3/stable/GtkToolbar.html
		// https://developer.gnome.org/gtk3/stable/GtkMenuToolButton.html
		// https://developer.gnome.org/gtk3/stable/GtkToolButton.html
		// https://developer.gnome.org/gtk3/stable/GtkSeparatorToolItem.html
		// https://developer.gnome.org/gtk3/stable/GtkToggleToolButton.html
		// https://developer.gnome.org/gtk3/stable/GtkMenu.html
		// https://developer.gnome.org/gtk3/stable/GtkMenuItem.html

		GtkWidget *icon1, *icon2, *icon3, *icon4, *icon5, *icon6, *icon7, *icon8, *icon9, *menu;

		icon1 = GTK_WIDGET (gtk_menu_tool_button_new (NULL, NULL));
		gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon1), "gtk-open");
		g_signal_connect (G_OBJECT (icon1), "clicked", G_CALLBACK (awf2_show_dialog_open), NULL);

		icon2 = GTK_WIDGET (gtk_menu_tool_button_new (NULL, NULL));
		gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon2), "gtk-open");
		menu = gtk_menu_new ();
		awf2_new_menu_item (menu, "Menu item", "", "", FALSE);
		gtk_widget_show_all (menu); // very important
		gtk_menu_tool_button_set_menu (GTK_MENU_TOOL_BUTTON (icon2), menu);
		g_signal_connect (G_OBJECT (icon2), "clicked", G_CALLBACK (awf2_show_dialog_open_recent), NULL);

		icon3 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
		gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon3), "gtk-save");
		g_signal_connect (G_OBJECT (icon3), "clicked", G_CALLBACK (awf2_show_dialog_save), NULL);

		icon4 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
		gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon4), "gtk-refresh");
		g_signal_connect (G_OBJECT (icon4), "clicked", G_CALLBACK (awf_refresh_theme), NULL);

		#if GTK_CHECK_VERSION (3,10,0)
			icon5 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
			gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon5), "gtk-goto-first");
		#else
			icon5 = GTK_WIDGET (gtk_tool_button_new_from_stock ("gtk-goto-first"));
		#endif

		#if GTK_CHECK_VERSION (3,10,0)
			icon6 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
			gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon6), "gtk-goto-last");
			gtk_widget_set_sensitive (icon6, FALSE);
		#else
			icon6 = GTK_WIDGET (gtk_tool_button_new_from_stock ("gtk-goto-last"));
			gtk_widget_set_sensitive (icon6, FALSE);
		#endif

		icon7 = GTK_WIDGET (gtk_toggle_tool_button_new ());
		gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon7), "gtk-add");
		gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON (icon7), TRUE);
		g_signal_connect (G_OBJECT (icon7), "clicked", G_CALLBACK (awf2_update_widgets), NULL);

		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon1), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon2), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon3), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon4), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), gtk_separator_tool_item_new (), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon5), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon6), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), gtk_separator_tool_item_new (), -1);
		gtk_toolbar_insert (GTK_TOOLBAR (root), GTK_TOOL_ITEM (icon7), -1);
	#endif
}

static void awf2_create_combos_entries (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkComboBoxText.html
	// https://developer.gnome.org/gtk3/stable/GtkComboBox.html
	// https://developer.gnome.org/gtk3/stable/GtkEntry.html

	GtkWidget *combo1, *combo2, *combo3, *combo4, *entry1, *entry2, *entry3, *entry4;

	combo1 = gtk_combo_box_text_new_with_entry ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo1), "Combo box entry 1");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo1), "Combo box entry 2");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo1), 0);

	combo2 = gtk_combo_box_text_new_with_entry ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo2), "Combo box entry 1 (disabled)");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo2), "Combo box entry 2 (disabled)");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo2), 0);
	gtk_widget_set_sensitive (combo2, FALSE);

	combo3 = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo3), "Combo box 1");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo3), "Combo box 2");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo3), 0);

	combo4 = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo4), "Combo box 1 (disabled)");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo4), "Combo box 2 (disabled)");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo4), 0);
	gtk_widget_set_sensitive (combo4, FALSE);

	entry1 = gtk_entry_new ();
	#if !GTK_CHECK_VERSION (3,98,0)
		gtk_entry_set_text (GTK_ENTRY (entry1), "Entry");
	#endif

	entry2 = gtk_entry_new ();
	#if !GTK_CHECK_VERSION (3,98,0)
		gtk_entry_set_text (GTK_ENTRY (entry2), "Entry (disabled)");
	#endif
	gtk_widget_set_sensitive (entry2, FALSE);

	entry3 = gtk_entry_new ();
	#if !GTK_CHECK_VERSION (3,98,0)
		gtk_entry_set_text (GTK_ENTRY (entry3), "Entry");
	#endif
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY (entry3), GTK_ENTRY_ICON_PRIMARY, "gtk-clear");
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY (entry3), GTK_ENTRY_ICON_SECONDARY, "gtk-find");

	entry4 = gtk_entry_new ();
	#if !GTK_CHECK_VERSION (3,98,0)
		gtk_entry_set_text (GTK_ENTRY (entry4), "Entry (disabled)");
	#endif
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY (entry4), GTK_ENTRY_ICON_PRIMARY, "gtk-clear");
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY (entry4), GTK_ENTRY_ICON_SECONDARY, "gtk-find");
	gtk_widget_set_sensitive (entry4, FALSE);

	#if GTK_CHECK_VERSION (3,2,0)
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry1), "Placeholder");
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry3), "Placeholder");
	#endif

	gtk_container_add (GTK_CONTAINER (root), combo1);
	gtk_container_add (GTK_CONTAINER (root), combo2);
	gtk_container_add (GTK_CONTAINER (root), combo3);
	gtk_container_add (GTK_CONTAINER (root), combo4);
	gtk_container_add (GTK_CONTAINER (root), entry1);
	gtk_container_add (GTK_CONTAINER (root), entry2);
	gtk_container_add (GTK_CONTAINER (root), entry3);
	gtk_container_add (GTK_CONTAINER (root), entry4);
}

static void awf2_create_spinbuttons (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkSpinButton.html

	GtkWidget *spinbutton1, *spinbutton2;

	spinbutton1 = gtk_spin_button_new_with_range (-100, 100, 1);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinbutton1), 1);

	spinbutton2 = gtk_spin_button_new_with_range (-100, 100, 1);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinbutton2), 1);
	gtk_widget_set_sensitive (spinbutton2, FALSE);

	awf2_boxpack (GTK_BOX (root), spinbutton1, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (root), BOXH, TRUE, TRUE, 0, 0); // empty space
	awf2_boxpack (GTK_BOX (root), spinbutton2, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (root), BOXH, TRUE, TRUE, 0, 0); // empty space
}

static void awf2_create_checkbuttons (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkCheckButton.html

	GtkWidget *checkbutton1, *checkbutton2, *checkbutton3, *checkbutton4, *checkbutton5, *checkbutton6;

	checkbutton1 = gtk_check_button_new_with_label ("Check button 1");

	checkbutton2 = gtk_check_button_new_with_label ("Check button 2");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton2), TRUE);

	checkbutton3 = gtk_check_button_new_with_label ("Check button 3");
	#if GTK_CHECK_VERSION (3,98,0)
		gtk_check_button_set_inconsistent (GTK_CHECK_BUTTON (checkbutton3), TRUE);
	#else
		gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (checkbutton3), TRUE);
	#endif

	checkbutton4 = gtk_check_button_new_with_label ("Check button 4");
	gtk_widget_set_sensitive (checkbutton4, FALSE);

	checkbutton5 = gtk_check_button_new_with_label ("Check button 5");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton5), TRUE);
	gtk_widget_set_sensitive (checkbutton5, FALSE);

	checkbutton6 = gtk_check_button_new_with_label ("Check button 6");
	#if GTK_CHECK_VERSION (3,98,0)
		gtk_check_button_set_inconsistent (GTK_CHECK_BUTTON (checkbutton6), TRUE);
	#else
		gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (checkbutton6), TRUE);
	#endif
	gtk_widget_set_sensitive (checkbutton6, FALSE);

	gtk_container_add (GTK_CONTAINER (root), checkbutton1);
	gtk_container_add (GTK_CONTAINER (root), checkbutton2);
	gtk_container_add (GTK_CONTAINER (root), checkbutton3);
	gtk_container_add (GTK_CONTAINER (root), checkbutton4);
	gtk_container_add (GTK_CONTAINER (root), checkbutton5);
	gtk_container_add (GTK_CONTAINER (root), checkbutton6);
}

static void awf2_create_radiobuttons (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkRadioButton.html

	GtkWidget *radio1, *radio2, *radio3, *radio4;
	GtkWidget *radiobutton1, *radiobutton2, *radiobutton3, *radiobutton4, *radiobutton5, *radiobutton6;

	radio1 = gtk_radio_button_new (NULL);
	radio2 = gtk_radio_button_new (NULL);
	radio3 = gtk_radio_button_new (NULL);
	radio4 = gtk_radio_button_new (NULL);

	radiobutton1 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Radio button 1");

	radiobutton2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Radio button 2");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton2), TRUE);

	radiobutton3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio2), "Radio button 3");
	#if GTK_CHECK_VERSION (3,98,0)
	#else
		gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (radiobutton3), TRUE);
	#endif

	radiobutton4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio3), "Radio button 4");
	gtk_widget_set_sensitive (radiobutton4, FALSE);

	radiobutton5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio3), "Radio button 5");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton5), TRUE);
	gtk_widget_set_sensitive (radiobutton5, FALSE);

	radiobutton6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio4), "Radio button 6");
	#if GTK_CHECK_VERSION (3,98,0)
	#else
		gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (radiobutton6), TRUE);
	#endif
	gtk_widget_set_sensitive (radiobutton6, FALSE);

	gtk_container_add (GTK_CONTAINER (root), radiobutton1);
	gtk_container_add (GTK_CONTAINER (root), radiobutton2);
	gtk_container_add (GTK_CONTAINER (root), radiobutton3);
	gtk_container_add (GTK_CONTAINER (root), radiobutton4);
	gtk_container_add (GTK_CONTAINER (root), radiobutton5);
	gtk_container_add (GTK_CONTAINER (root), radiobutton6);
}

static void awf2_create_otherbuttons (GtkWidget *root) {

	const gchar *scale_icons[] = { "awf", NULL };
	GtkWidget *button1, *button2, *button3, *button4, *button5, *button6, *button7, *button8, *button9, *button10, *button11, *button12;
	GtkWidget *combomenu1, *combomenu2;

	button1 = gtk_button_new_with_label ("Button 1");

	button2 = gtk_button_new_with_label ("Button 2");
	gtk_widget_set_sensitive (button2, FALSE);

	button3 = gtk_toggle_button_new_with_label ("Toggle button 1");

	button4 = gtk_toggle_button_new_with_label ("Toggle button 2");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button3), TRUE);
	gtk_widget_set_sensitive (button4, FALSE);

	#if GTK_CHECK_VERSION (3,0,0)
		GdkRGBA color;
		gdk_rgba_parse (&color, "#7796ba");
		button5 = gtk_color_button_new_with_rgba (&color);
	#else
		GdkColor color;
		gdk_color_parse ("#7796ba", &color);
		button5 = gtk_color_button_new_with_color (&color);
	#endif

	button6 = gtk_font_button_new ();
	gtk_widget_set_size_request (GTK_WIDGET (button5), 170, -1);

	button7 = gtk_file_chooser_button_new ("GtkFileChooserDialog", GTK_FILE_CHOOSER_ACTION_OPEN);

	button8 = gtk_file_chooser_button_new ("GtkFileChooserDialog", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

	#if GTK_CHECK_VERSION (3,0,0)
		button9 = gtk_switch_new ();
		gtk_switch_set_active (GTK_SWITCH (button9), TRUE);

		button10 = gtk_switch_new ();
		gtk_switch_set_active (GTK_SWITCH (button10), FALSE);
		gtk_widget_set_sensitive (button10, FALSE);
	#else
		combomenu1 = gtk_menu_new ();
		gtk_menu_shell_append (GTK_MENU_SHELL (combomenu1), gtk_menu_item_new_with_label ("Option 1"));
		gtk_menu_shell_append (GTK_MENU_SHELL (combomenu1), gtk_menu_item_new_with_label ("Option 2"));
		gtk_menu_shell_append (GTK_MENU_SHELL (combomenu1), gtk_menu_item_new_with_label ("Option 3"));

		button9 = gtk_option_menu_new ();
		gtk_option_menu_set_menu (GTK_OPTION_MENU (button9), combomenu1);

		combomenu2 = gtk_menu_new ();
		gtk_menu_shell_append (GTK_MENU_SHELL (combomenu2), gtk_menu_item_new_with_label ("Option 1"));
		gtk_menu_shell_append (GTK_MENU_SHELL (combomenu2), gtk_menu_item_new_with_label ("Option 2"));
		gtk_menu_shell_append (GTK_MENU_SHELL (combomenu2), gtk_menu_item_new_with_label ("Option 3"));

		button10 = gtk_option_menu_new ();
		gtk_option_menu_set_menu (GTK_OPTION_MENU (button10), combomenu2);
		gtk_widget_set_sensitive (button10, FALSE);
	#endif

	button11 = gtk_link_button_new_with_label ("https://github.com/luigifab/awf", _("Link button"));

	// https://developer.gnome.org/gtk3/stable/GtkScaleButton.html

	#if GTK_CHECK_VERSION (3,98,0)
		button12 = gtk_scale_button_new (1, 10, 1, scale_icons);
	#else
		button12 = gtk_scale_button_new (GTK_ICON_SIZE_BUTTON, 1, 10, 1, scale_icons);
	#endif

	gtk_container_add (GTK_CONTAINER (root), button1);
	gtk_container_add (GTK_CONTAINER (root), button2);
	gtk_container_add (GTK_CONTAINER (root), button3);
	gtk_container_add (GTK_CONTAINER (root), button4);
	gtk_container_add (GTK_CONTAINER (root), button5);
	gtk_container_add (GTK_CONTAINER (root), button6);
	gtk_container_add (GTK_CONTAINER (root), button7);
	gtk_container_add (GTK_CONTAINER (root), button8);
	gtk_container_add (GTK_CONTAINER (root), button9);
	gtk_container_add (GTK_CONTAINER (root), button10);
	gtk_container_add (GTK_CONTAINER (root), button11);
	gtk_container_add (GTK_CONTAINER (root), button12);
}

static void awf2_create_progressbars (GtkWidget *vroot1, GtkWidget *vroot2, GtkWidget *hroot1, GtkWidget *hroot2) {

	// https://developer.gnome.org/gtk3/stable/GtkProgressBar.html
	// https://developer.gnome.org/gtk3/stable/GtkScale.html
	// https://developer.gnome.org/gtk3/stable/GtkLevelBar.html

	#if GTK_CHECK_VERSION (3,0,0)
		progressbar1 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar1, 170, -1);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), 0.5);
		gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar1), GTK_ORIENTATION_HORIZONTAL);
		gtk_widget_set_tooltip_text (progressbar1, "progressbar");
	#else
		progressbar1 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar1, 170, -1);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), 0.5);
		gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar1), GTK_PROGRESS_LEFT_TO_RIGHT);
		gtk_widget_set_tooltip_text (progressbar1, "progressbar");
	#endif

	#if GTK_CHECK_VERSION (3,0,0)
		progressbar2 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar2, 170, -1);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), 0.5);
		gtk_progress_bar_set_inverted (GTK_PROGRESS_BAR (progressbar2), TRUE);
		gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar2), GTK_ORIENTATION_HORIZONTAL);
		gtk_widget_set_tooltip_text (progressbar2, "progressbar");
	#else
		progressbar2 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar2, 170, -1);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), 0.5);
		gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar2), GTK_PROGRESS_RIGHT_TO_LEFT);
		gtk_widget_set_tooltip_text (progressbar2, "progressbar");
	#endif

	#if GTK_CHECK_VERSION (3,2,0)
		scale1 = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
		gtk_widget_set_size_request (scale1, 170, -1);
		gtk_range_set_value (GTK_RANGE (scale1), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale1), FALSE);
		gtk_widget_set_tooltip_text (scale1, "scale");
		g_signal_connect ((gpointer)scale1, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#else
		scale1 = gtk_hscale_new_with_range (0, 100, 1);
		gtk_widget_set_size_request (scale1, 170, -1);
		gtk_range_set_value (GTK_RANGE (scale1), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale1), FALSE);
		gtk_widget_set_tooltip_text (scale1, "scale");
		g_signal_connect ((gpointer)scale1, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#endif

	#if GTK_CHECK_VERSION (3,2,0)
		scale2 = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
		gtk_widget_set_size_request (scale2, 170, -1);
		gtk_range_set_value (GTK_RANGE (scale2), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale2), FALSE);
		gtk_range_set_inverted (GTK_RANGE (scale2), TRUE);
		gtk_widget_set_tooltip_text (scale2, "scale");
		g_signal_connect ((gpointer)scale2, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#else
		scale2 = gtk_hscale_new_with_range (0, 100, 1);
		gtk_widget_set_size_request (scale2, 170, -1);
		gtk_range_set_value (GTK_RANGE (scale2), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale2), FALSE);
		gtk_range_set_inverted (GTK_RANGE (scale2), TRUE);
		gtk_widget_set_tooltip_text (scale2, "scale");
		g_signal_connect ((gpointer)scale2, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#endif

	#if GTK_CHECK_VERSION (3,0,0)
		progressbar3 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar3, -1, 100);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), 0.5);
		gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar3), GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_tooltip_text (progressbar3, "progressbar");
	#else
		progressbar3 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar3, -1, 100);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), 0.5);
		gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar3), GTK_PROGRESS_TOP_TO_BOTTOM);
		gtk_widget_set_tooltip_text (progressbar3, "progressbar");
	#endif

	#if GTK_CHECK_VERSION (3,0,0)
		progressbar4 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar4, -1, 100);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), 0.5);
		gtk_progress_bar_set_inverted (GTK_PROGRESS_BAR (progressbar4), TRUE);
		gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar4), GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_tooltip_text (progressbar4, "progressbar");
	#else
		progressbar4 = gtk_progress_bar_new ();
		gtk_widget_set_size_request (progressbar4, -1, 100);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), 0.5);
		gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar4), GTK_PROGRESS_BOTTOM_TO_TOP);
		gtk_widget_set_tooltip_text (progressbar4, "progressbar");
	#endif

	#if GTK_CHECK_VERSION (3,2,0)
		scale3 = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, 0, 100, 1);
		gtk_widget_set_size_request (scale3, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale3), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale3), FALSE);
		gtk_widget_set_tooltip_text (scale3, "scale");
		g_signal_connect ((gpointer)scale3, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#else
		scale3 = gtk_vscale_new_with_range (0, 100, 1);
		gtk_widget_set_size_request (scale3, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale3), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale3), FALSE);
		gtk_widget_set_tooltip_text (scale3, "scale");
		g_signal_connect ((gpointer)scale3, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#endif

	#if GTK_CHECK_VERSION (3,2,0)
		scale4 = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, 0, 100, 1);
		gtk_widget_set_size_request (scale4, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale4), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale4), FALSE);
		gtk_range_set_inverted (GTK_RANGE (scale4), TRUE);
		gtk_widget_set_tooltip_text (scale4, "scale");
		g_signal_connect ((gpointer)scale4, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#else
		scale4 = gtk_vscale_new_with_range (0, 100, 1);
		gtk_widget_set_size_request (scale4, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale4), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale4), FALSE);
		gtk_range_set_inverted (GTK_RANGE (scale4), TRUE);
		gtk_widget_set_tooltip_text (scale4, "scale");
		g_signal_connect ((gpointer)scale4, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#endif

	#if GTK_CHECK_VERSION (3,2,0)
		scale5 = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, 0, 100, 1);
		gtk_widget_set_size_request (scale5, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale5), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale5), FALSE);
		gtk_widget_set_tooltip_text (scale5, "scale");
		g_signal_connect ((gpointer)scale5, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#else
		scale5 = gtk_vscale_new_with_range (0, 100, 1);
		gtk_widget_set_size_request (scale5, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale5), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale5), FALSE);
		gtk_widget_set_tooltip_text (scale5, "scale");
		g_signal_connect ((gpointer)scale5, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#endif

	#if GTK_CHECK_VERSION (3,2,0)
		scale6 = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, 0, 100, 1);
		gtk_widget_set_size_request (scale6, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale6), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale6), FALSE);
		gtk_range_set_inverted (GTK_RANGE (scale6), TRUE);
		gtk_widget_set_tooltip_text (scale6, "scale");
		g_signal_connect ((gpointer)scale6, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#else
		scale6 = gtk_vscale_new_with_range (0, 100, 1);
		gtk_widget_set_size_request (scale6, -1, 100);
		gtk_range_set_value (GTK_RANGE (scale6), 50);
		gtk_scale_set_draw_value (GTK_SCALE (scale6), FALSE);
		gtk_range_set_inverted (GTK_RANGE (scale6), TRUE);
		gtk_widget_set_tooltip_text (scale6, "scale");
		g_signal_connect ((gpointer)scale6, "value_changed", G_CALLBACK (awf_update_progressbars), NULL);
	#endif

	#if GTK_CHECK_VERSION (3,6,0)
		levelbar1 = gtk_level_bar_new ();
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar1), GTK_LEVEL_BAR_MODE_CONTINUOUS);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar1), 0.5);
		gtk_widget_set_tooltip_text (levelbar1, "levelbar");

		levelbar2 = gtk_level_bar_new ();
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar2), GTK_LEVEL_BAR_MODE_CONTINUOUS);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar2), 0.5);
		#if GTK_CHECK_VERSION (3,8,0)
			gtk_level_bar_set_inverted (GTK_LEVEL_BAR (levelbar2), TRUE);
		#endif
		gtk_widget_set_tooltip_text (levelbar2, "levelbar");

		levelbar3 = gtk_level_bar_new_for_interval (0, 5);
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar3), GTK_LEVEL_BAR_MODE_DISCRETE);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar3), 2);
		gtk_widget_set_tooltip_text (levelbar3, "levelbar");

		levelbar4 = gtk_level_bar_new_for_interval (0, 5);
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar4), GTK_LEVEL_BAR_MODE_DISCRETE);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar4), 2);
		#if GTK_CHECK_VERSION (3,8,0)
			gtk_level_bar_set_inverted (GTK_LEVEL_BAR (levelbar4), TRUE);
		#endif
		gtk_widget_set_tooltip_text (levelbar4, "levelbar");

		levelbar5 = gtk_level_bar_new ();
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar5), GTK_LEVEL_BAR_MODE_CONTINUOUS);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar5), 0.5);
		gtk_orientable_set_orientation (GTK_ORIENTABLE (levelbar5), GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_tooltip_text (levelbar5, "levelbar");

		levelbar6 = gtk_level_bar_new ();
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar6), GTK_LEVEL_BAR_MODE_CONTINUOUS);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar6), 0.5);
		#if GTK_CHECK_VERSION (3,8,0)
			gtk_level_bar_set_inverted (GTK_LEVEL_BAR (levelbar6), TRUE);
		#endif
		gtk_orientable_set_orientation (GTK_ORIENTABLE (levelbar6), GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_tooltip_text (levelbar6, "levelbar");

		levelbar7 = gtk_level_bar_new_for_interval (0, 5);
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar7), GTK_LEVEL_BAR_MODE_DISCRETE);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar7), 2);
		gtk_orientable_set_orientation (GTK_ORIENTABLE (levelbar7), GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_tooltip_text (levelbar7, "levelbar");

		levelbar8 = gtk_level_bar_new_for_interval (0, 5);
		gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar8), GTK_LEVEL_BAR_MODE_DISCRETE);
		gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar8), 2);
		#if GTK_CHECK_VERSION (3,8,0)
			gtk_level_bar_set_inverted (GTK_LEVEL_BAR (levelbar8), TRUE);
		#endif
		gtk_orientable_set_orientation (GTK_ORIENTABLE (levelbar8), GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_tooltip_text (levelbar8, "levelbar");
	#endif

	gtk_container_add (GTK_CONTAINER (vroot1), progressbar1);
	gtk_container_add (GTK_CONTAINER (vroot1), progressbar2);
	gtk_container_add (GTK_CONTAINER (vroot1), scale1);
	gtk_container_add (GTK_CONTAINER (vroot1), scale2);
	awf2_boxpack (GTK_BOX (hroot1), progressbar3, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (hroot1), progressbar4, FALSE, FALSE, 0, 0);
	#if GTK_CHECK_VERSION (3,6,0)
		awf2_boxpack (GTK_BOX (hroot1), levelbar5, FALSE, FALSE, 0, 0);
		awf2_boxpack (GTK_BOX (hroot1), levelbar6, FALSE, FALSE, 0, 0);
		awf2_boxpack (GTK_BOX (hroot1), levelbar7, FALSE, FALSE, 0, 0);
		awf2_boxpack (GTK_BOX (hroot1), levelbar8, FALSE, FALSE, 0, 0);
	#endif
	awf2_boxpack (GTK_BOX (hroot2), scale3, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (hroot2), scale4, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (hroot2), scale5, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (hroot2), scale6, FALSE, FALSE, 0, 0);
	#if GTK_CHECK_VERSION (3,6,0)
		gtk_container_add (GTK_CONTAINER (vroot2), levelbar1);
		gtk_container_add (GTK_CONTAINER (vroot2), levelbar2);
		gtk_container_add (GTK_CONTAINER (vroot2), levelbar3);
		gtk_container_add (GTK_CONTAINER (vroot2), levelbar4);
	#endif
}

static void awf2_create_labels (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkLabel.html

	GtkWidget *label1, *label2;

	label1 = gtk_label_new ("Label 1");
	gtk_widget_set_tooltip_text (label1, "label");

	label2 = gtk_label_new ("Label 2");
	gtk_widget_set_tooltip_text (label2, "label");
	gtk_widget_set_sensitive (label2, FALSE);

	awf2_boxpack (GTK_BOX (root), label1, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (root), BOXH, TRUE, TRUE, 0, 0); // empty space
	awf2_boxpack (GTK_BOX (root), label2, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (root), BOXH, TRUE, TRUE, 0, 0); // empty space
}

static void awf2_create_spinners (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkSpinner.html

	GtkWidget *spinner1, *spinner2;

	spinner1 = gtk_spinner_new ();
	gtk_widget_set_size_request (spinner1, 20, 20);
	gtk_widget_set_tooltip_text (spinner1, "spinner");
	gtk_spinner_start (GTK_SPINNER (spinner1));

	spinner2 = gtk_spinner_new ();
	gtk_widget_set_size_request (spinner2, 20, 20);
	gtk_widget_set_tooltip_text (spinner2, "spinner");
	gtk_widget_set_sensitive (spinner2, FALSE);
	gtk_spinner_start (GTK_SPINNER (spinner2));

	awf2_boxpack (GTK_BOX (root), spinner1, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (root), BOXH, TRUE, TRUE, 0, 0); // empty space
	awf2_boxpack (GTK_BOX (root), spinner2, FALSE, FALSE, 0, 0);
	awf2_boxpack (GTK_BOX (root), BOXH, TRUE, TRUE, 0, 0); // empty space
}

static void awf2_create_expander (GtkWidget *root) {

	// https://developer.gnome.org/gtk3/stable/GtkExpander.html

	GtkWidget *expander, *scrolled_window, *tv;
	GtkTextBuffer *buffer;

	expander = gtk_expander_new (_("More..."));
	gtk_widget_set_size_request (expander, 200, 100);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (scrolled_window, 200, 100);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	tv = gtk_text_view_new ();
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv), GTK_WRAP_WORD);
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER (buffer), _("A widget factory is a theme preview application for gtk2, gtk3 and gtk4. It displays the various widget types provided by GTK in a single window allowing to see the visual effect of the applied theme."), -1);
	gtk_container_add (GTK_CONTAINER (scrolled_window), tv);
	gtk_container_add (GTK_CONTAINER (expander), scrolled_window);

	gtk_container_add (GTK_CONTAINER (root), expander);
}

static void awf2_create_frames (GtkWidget *lroot, GtkWidget *rroot) {

	// https://developer.gnome.org/gtk3/stable/GtkFrame.html

	GtkWidget *frame1, *frame2, *frame3, *frame4;

	#if GTK_CHECK_VERSION (3,0,0)
		frame1 = gtk_frame_new ("Frame (shadow in)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
		gtk_style_context_add_class (gtk_widget_get_style_context (frame1), "border-inset");
	#else
		frame1 = gtk_frame_new ("Frame (shadow in)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
	#endif

	#if GTK_CHECK_VERSION (3,0,0)
		frame2 = gtk_frame_new ("Frame (shadow out)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_OUT);
		gtk_style_context_add_class (gtk_widget_get_style_context (frame2), "border-outset");
	#else
		frame2 = gtk_frame_new ("Frame (shadow out)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_OUT);
	#endif

	#if GTK_CHECK_VERSION (3,0,0)
		frame3 = gtk_frame_new ("Frame (shadow etched in)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_ETCHED_IN);
		gtk_style_context_add_class (gtk_widget_get_style_context (frame3), "border-groove");
	#else
		frame3 = gtk_frame_new ("Frame (shadow etched in)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_ETCHED_IN);
	#endif

	#if GTK_CHECK_VERSION (3,0,0)
		frame4 = gtk_frame_new ("Frame (shadow etched out)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame4), GTK_SHADOW_ETCHED_OUT);
		gtk_style_context_add_class (gtk_widget_get_style_context (frame4), "border-ridge");
	#else
		frame4 = gtk_frame_new ("Frame (shadow etched out)");
		gtk_frame_set_shadow_type (GTK_FRAME (frame4), GTK_SHADOW_ETCHED_OUT);
	#endif

	gtk_container_add (GTK_CONTAINER (lroot), frame1);
	gtk_container_add (GTK_CONTAINER (lroot), frame2);
	gtk_container_add (GTK_CONTAINER (rroot), frame3);
	gtk_container_add (GTK_CONTAINER (rroot), frame4);
}

static void awf2_create_notebooks (GtkWidget *lroot, GtkWidget *rroot) {

	// https://developer.gnome.org/gtk3/stable/GtkNotebook.html

	int i;
	GtkWidget *tab;

	notebook1 = gtk_notebook_new ();
	notebook2 = gtk_notebook_new ();
	notebook3 = gtk_notebook_new ();
	notebook4 = gtk_notebook_new ();

	#if GTK_CHECK_VERSION (3,4,0) && !GTK_CHECK_VERSION (3,98,0)
		gtk_widget_add_events (GTK_WIDGET (notebook1), GDK_SCROLL_MASK);
		g_signal_connect (GTK_WIDGET (notebook1), "scroll-event", G_CALLBACK (awf2_scroll_notebook_tabs), NULL);
		gtk_widget_add_events (GTK_WIDGET (notebook2), GDK_SCROLL_MASK);
		g_signal_connect (GTK_WIDGET (notebook2), "scroll-event", G_CALLBACK (awf2_scroll_notebook_tabs), NULL);
		gtk_widget_add_events (GTK_WIDGET (notebook3), GDK_SCROLL_MASK);
		g_signal_connect (GTK_WIDGET (notebook3), "scroll-event", G_CALLBACK (awf2_scroll_notebook_tabs), NULL);
		gtk_widget_add_events (GTK_WIDGET (notebook4), GDK_SCROLL_MASK);
		g_signal_connect (GTK_WIDGET (notebook4), "scroll-event", G_CALLBACK (awf2_scroll_notebook_tabs), NULL);
	#endif

	gtk_notebook_popup_enable (GTK_NOTEBOOK (notebook1));
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook1), FALSE);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook1), GTK_POS_TOP);
	for (i = 1; i <= 4; i++) {
		tab = gtk_label_new ("");
		gtk_notebook_append_page (GTK_NOTEBOOK (notebook1), tab, gtk_label_new (g_strdup_printf ("tab%d", i)));
		gtk_notebook_set_tab_reorderable (GTK_NOTEBOOK (notebook1), tab, TRUE);
	}

	gtk_notebook_popup_enable (GTK_NOTEBOOK (notebook2));
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook2), FALSE);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook2), GTK_POS_BOTTOM);
	for (i = 1; i <= 4; i++) {
		tab = gtk_label_new ("");
		gtk_notebook_append_page (GTK_NOTEBOOK (notebook2), tab, gtk_label_new (g_strdup_printf ("tab%d", i)));
		gtk_notebook_set_tab_reorderable (GTK_NOTEBOOK (notebook2), tab, TRUE);
	}

	gtk_notebook_popup_enable (GTK_NOTEBOOK (notebook3));
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook3), FALSE);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook3), GTK_POS_LEFT);
	for (i = 1; i <= 4; i++) {
		tab = gtk_label_new ("");
		gtk_notebook_append_page (GTK_NOTEBOOK (notebook3), tab, gtk_label_new (g_strdup_printf ("tab%d", i)));
		gtk_notebook_set_tab_reorderable (GTK_NOTEBOOK (notebook3), tab, TRUE);
	}

	gtk_notebook_popup_enable (GTK_NOTEBOOK (notebook4));
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook4), FALSE);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook4), GTK_POS_RIGHT);
	for (i = 1; i <= 4; i++) {
		tab = gtk_label_new ("");
		gtk_notebook_append_page (GTK_NOTEBOOK (notebook4), tab, gtk_label_new (g_strdup_printf ("tab%d", i)));
		gtk_notebook_set_tab_reorderable (GTK_NOTEBOOK (notebook4), tab, TRUE);
	}

	gtk_container_add (GTK_CONTAINER (lroot), notebook1);
	gtk_container_add (GTK_CONTAINER (lroot), notebook2);
	gtk_container_add (GTK_CONTAINER (rroot), notebook3);
	gtk_container_add (GTK_CONTAINER (rroot), notebook4);
}

static void awf2_create_treview (GtkWidget *root) {

	GtkWidget *scrolled_window, *view;
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkListStore *store;
	GtkTreeIter iter;

	view = gtk_tree_view_new ();
	gtk_widget_set_size_request (view, 200, 200);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Longer column 1", renderer, "text", COLUMN1, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Longer column 2", renderer, "text", COLUMN2, NULL);

	store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Longer cell 1.1", COLUMN2, "Longer cell 1.2", -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Longer cell 2.1", COLUMN2, "Longer cell 2.2", -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Longer cell 3.1", COLUMN2, "Longer cell 3.2", -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Longer cell 4.1", COLUMN2, "Longer cell 4.2", -1);

	model = GTK_TREE_MODEL (store);
	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (scrolled_window, 200, 200);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (scrolled_window), view);

	gtk_container_add (GTK_CONTAINER (root), scrolled_window);
}

// menuitems

static GtkWidget* awf2_new_menu (GtkWidget *root, gchar *text) {

	// https://developer.gnome.org/gtk3/stable/GtkMenu.html
	// https://developer.gnome.org/gtk3/stable/GtkMenuItem.html

	GtkWidget *menu, *menuitem;

	#if !GTK_CHECK_VERSION (3,98,0)
		menu = gtk_menu_new ();
		menuitem = gtk_menu_item_new_with_mnemonic (text);
		gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
		gtk_menu_shell_append (GTK_MENU_SHELL (root), menuitem);
	#endif

	return menu;
}

static GtkWidget* awf2_new_menu_tearoff (GtkWidget *menu) {

	// https://developer.gnome.org/gtk3/stable/GtkTearoffMenuItem.html

	GtkWidget *menuitem;

	#if GTK_CHECK_VERSION (3,98,0)
	#elif GTK_CHECK_VERSION (3,0,0)
		menuitem = gtk_tearoff_menu_item_new ();
		gtk_style_context_add_class (gtk_widget_get_style_context (menuitem), "tearoff");
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	#else
		menuitem = gtk_tearoff_menu_item_new ();
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	#endif

	return menuitem;
}

static GtkWidget* awf2_new_menu_separator (GtkWidget *menu) {

	// https://developer.gnome.org/gtk3/stable/GtkSeparatorMenuItem.html

	GtkWidget *menuitem;

	#if !GTK_CHECK_VERSION (3,98,0)
		menuitem = gtk_separator_menu_item_new ();
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	#endif

	return menuitem;
}

static GtkWidget* awf2_new_menu_check (GtkWidget *menu, gchar *text, gboolean checked, gboolean inconsistent, gboolean disabled) {

	// https://developer.gnome.org/gtk3/stable/GtkCheckMenuItem.html

	GtkWidget *menuitem;

	#if !GTK_CHECK_VERSION (3,98,0)
		menuitem = gtk_check_menu_item_new_with_mnemonic (text);
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), checked);
		gtk_check_menu_item_set_inconsistent (GTK_CHECK_MENU_ITEM (menuitem), inconsistent);
		gtk_widget_set_sensitive (menuitem, !disabled);
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	#endif

	return menuitem;
}

static GtkWidget* awf2_new_menu_radio (GtkWidget *menu, gchar *text, gboolean checked, gboolean inconsistent, gboolean disabled, GSList *group) {

	// https://developer.gnome.org/gtk3/stable/GtkRadioMenuItem.html

	GtkWidget *menuitem;

	#if !GTK_CHECK_VERSION (3,98,0)
		menuitem = gtk_radio_menu_item_new_with_mnemonic (group, text);
		gtk_check_menu_item_set_inconsistent (GTK_CHECK_MENU_ITEM (menuitem), inconsistent);
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), checked);
		gtk_widget_set_sensitive (menuitem, !disabled);
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	#endif

	return menuitem;
}

static GtkWidget* awf2_new_menu_item (GtkWidget *menu, gchar *text, gchar *accel, gchar *image, gboolean disabled) {

	// https://developer.gnome.org/gtk3/stable/GtkMenuItem.html
	// https://developer.gnome.org/gtk3/stable/GtkImageMenuItem.html

	GtkWidget *menuitem;
	GdkModifierType accelerator_mods;
	guint accelerator_key;

	#if !GTK_CHECK_VERSION (3,98,0)
		if (strlen (image) > 0)
			menuitem = gtk_image_menu_item_new_from_stock (image, NULL);
		else
			menuitem = gtk_menu_item_new_with_mnemonic (text);

		gtk_widget_set_sensitive (menuitem, !disabled);
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

		//gtk_accelerator_parse (accel, &accelerator_key, &accelerator_mods);
		//if (accelerator_key && accelerator_mods)
		//	gtk_widget_add_accelerator (menuitem, "activate", , accelerator_key, accelerator_mods, GTK_ACCEL_VISIBLE);
	#endif

	return menuitem;
}

// dialogs

static void awf2_show_dialog_open (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkFileChooserDialog.html

	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new (
		"GtkFileChooserDialog",
		GTK_WINDOW (window),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"gtk-cancel",
		GTK_RESPONSE_CANCEL,
		"gtk-open",
		GTK_RESPONSE_ACCEPT,
		NULL);

	gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void awf2_show_dialog_open_recent (GtkWidget *widget, gpointer unused) {

	#if !GTK_CHECK_VERSION (3,98,0)

		// https://developer.gnome.org/gtk3/stable/GtkRecentChooserDialog.html

		GtkWidget *dialog;

		dialog = gtk_recent_chooser_dialog_new (
			"GtkRecentChooserDialog",
			GTK_WINDOW (window),
			"gtk-cancel",
			GTK_RESPONSE_CANCEL,
			"gtk-open",
			GTK_RESPONSE_ACCEPT,
			NULL);

		gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
	#endif
}

static void awf2_show_dialog_save (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkFileChooserDialog.html

	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new (
		"GtkFileChooserDialog",
		GTK_WINDOW (window),
		GTK_FILE_CHOOSER_ACTION_SAVE,
		"gtk-cancel",
		GTK_RESPONSE_CANCEL,
		"gtk-save",
		GTK_RESPONSE_ACCEPT,
		NULL);

	gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void awf2_show_dialog_properties (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkMessageDialog.html

	GtkWidget *dialog;

	dialog = gtk_message_dialog_new (
		GTK_WINDOW (window),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_YES_NO,
		"GtkMessageDialog");

	gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), _("A widget factory is a theme preview application for gtk2, gtk3 and gtk4. It displays the various widget types provided by GTK in a single window allowing to see the visual effect of the applied theme."));

	gtk_window_set_title (GTK_WINDOW (dialog), "GtkMessageDialog");
	gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void awf2_show_dialog_page_setup (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkPageSetupUnixDialog.html

	GtkWidget *dialog;

	dialog = gtk_page_setup_unix_dialog_new (
		"GtkPageSetupUnixDialog",
		GTK_WINDOW (window));

	gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void awf2_show_dialog_print (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkPrintUnixDialog.html

	GtkWidget *dialog;

	dialog = gtk_print_unix_dialog_new (
		"GtkPrintUnixDialog",
		GTK_WINDOW (window));

	gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void awf2_show_dialog_about (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkAboutDialog.html

	gtk_show_about_dialog (GTK_WINDOW (window),
		"version", VERSION,
		"comments", g_strdup_printf ("%s\n\n%s%s",
			_("A widget factory is a theme preview application for gtk2, gtk3 and gtk4. It displays the various widget types provided by GTK in a single window allowing to see the visual effect of the applied theme."),
			g_strdup_printf (" compiled with gtk %d.%d.%d and glib %d.%d.%d\n",
				GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION,
				GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION),
			g_strdup_printf ("  started with gtk %d.%d.%d and glib %d.%d.%d",
				#if GTK_CHECK_VERSION (3,0,0)
					gtk_get_major_version (), gtk_get_minor_version (), gtk_get_micro_version (),
				#else
					gtk_major_version, gtk_minor_version, gtk_micro_version,
				#endif
				glib_major_version, glib_minor_version, glib_micro_version)),
		"website", "https://github.com/luigifab/awf",
		"copyright", "Copyright © 2011-2017 Valère Monseur\nCopyright © 2020 Fabrice Creuzot",
		"icon-name", "awf",
		"logo-icon-name", "awf",
		"license", "A widget factory is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.",
		"wrap-license", TRUE,
		NULL);
}

static void awf2_show_dialog_calendar (GtkWidget *widget, gpointer unused) {

	// https://developer.gnome.org/gtk3/stable/GtkDialog.html
	// https://developer.gnome.org/gtk3/stable/GtkInfoBar.html
	// https://developer.gnome.org/gtk3/stable/GtkCalendar.html

	GtkWidget *dialog, *bar, *label, *calendar, *vbox = BOXV;
	GtkContainer *area;

	dialog = gtk_dialog_new_with_buttons (
		NULL,
		GTK_WINDOW (window),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		NULL,
		NULL);

	gtk_widget_set_size_request (GTK_WIDGET (dialog), 350, -1);

	// info bars
	bar = gtk_info_bar_new_with_buttons ("gtk-ok", GTK_RESPONSE_OK, NULL);
	gtk_info_bar_set_message_type (GTK_INFO_BAR (bar), GTK_MESSAGE_INFO);
	label = gtk_label_new (_("This is an info bar."));
	awf2_boxpack (GTK_BOX (bar), label, FALSE, FALSE, 0, 0);
	gtk_container_add (GTK_CONTAINER (vbox), bar);

	bar = gtk_info_bar_new_with_buttons (_("Ok"), GTK_RESPONSE_OK, NULL);
	gtk_info_bar_set_message_type (GTK_INFO_BAR (bar), GTK_MESSAGE_QUESTION);
	label = gtk_label_new (_("This is a question bar."));
	awf2_boxpack (GTK_BOX (bar), label, FALSE, FALSE, 0, 0);
	gtk_container_add (GTK_CONTAINER (vbox), bar);

	bar = gtk_info_bar_new ();
	gtk_info_bar_set_message_type (GTK_INFO_BAR (bar), GTK_MESSAGE_WARNING);
	label = gtk_label_new (_("This is a warning bar."));
	awf2_boxpack (GTK_BOX (bar), label, FALSE, FALSE, 0, 0);
	gtk_container_add (GTK_CONTAINER (vbox), bar);

	bar = gtk_info_bar_new ();
	gtk_info_bar_set_message_type (GTK_INFO_BAR (bar), GTK_MESSAGE_ERROR);
	label = gtk_label_new (_("This is an error bar."));
	awf2_boxpack (GTK_BOX (bar), label, FALSE, FALSE, 0, 0);
	gtk_container_add (GTK_CONTAINER (vbox), bar);

	// calendar
	calendar = gtk_calendar_new ();
	#if !GTK_CHECK_VERSION (3,98,0)
		gtk_calendar_set_display_options (GTK_CALENDAR (calendar), GTK_CALENDAR_SHOW_HEADING |
			GTK_CALENDAR_SHOW_DAY_NAMES | GTK_CALENDAR_SHOW_WEEK_NUMBERS);
	#endif
	gtk_container_add (GTK_CONTAINER (vbox), calendar);

	// dialog
	area = GTK_CONTAINER (gtk_dialog_get_content_area (GTK_DIALOG (dialog)));
	awf2_boxpack (GTK_BOX (area), vbox, FALSE, FALSE, 5, 5);

	gtk_dialog_add_button (GTK_DIALOG (dialog), "gtk-cancel", 0);
	gtk_dialog_add_button (GTK_DIALOG (dialog), "gtk-ok", 0);

	#if GTK_CHECK_VERSION (3,98,0)
		gtk_widget_show (dialog);
	#else
		gtk_widget_show_all (dialog);
	#endif

	gtk_window_set_title (GTK_WINDOW (dialog), "GtkDialog");
	gtk_window_set_icon_name (GTK_WINDOW (dialog), "awf");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

// https://github.com/mate-desktop/mate-control-center/blob/master/capplets/common/capplet-util.c
// source function capplet_dialog_page_scroll_event_cb of mate-appearance-properties from mate-control-center, GNU GPL 2

#if GTK_CHECK_VERSION (3,4,0) && !GTK_CHECK_VERSION (3,98,0)
static void awf2_scroll_notebook_tabs (GtkWidget *widget, GdkEventScroll *event) {

	GtkNotebook *notebook = GTK_NOTEBOOK (widget);
	GtkWidget *child, *event_widget, *action_widget;

	child = gtk_notebook_get_nth_page (notebook, gtk_notebook_get_current_page (notebook));
	if (child == NULL)
		return;

	event_widget = gtk_get_event_widget ((GdkEvent *)event);

	// Ignore scroll events from the content of the page
	if (event_widget == NULL || event_widget == child || gtk_widget_is_ancestor (event_widget, child))
		return;

	// And also from the action widgets
	action_widget = gtk_notebook_get_action_widget (notebook, GTK_PACK_START);
	if (event_widget == action_widget || (action_widget != NULL && gtk_widget_is_ancestor (event_widget, action_widget)))
		return;

	action_widget = gtk_notebook_get_action_widget (notebook, GTK_PACK_END);
	if (event_widget == action_widget || (action_widget != NULL && gtk_widget_is_ancestor (event_widget, action_widget)))
		return;

	switch (event->direction)
	{
		case GDK_SCROLL_RIGHT:
		case GDK_SCROLL_DOWN:
			gtk_notebook_next_page (notebook);
			break;
		case GDK_SCROLL_LEFT:
		case GDK_SCROLL_UP:
			gtk_notebook_prev_page (notebook);
			break;
		case GDK_SCROLL_SMOOTH:
			switch (gtk_notebook_get_tab_pos (notebook))
			{
				case GTK_POS_LEFT:
				case GTK_POS_RIGHT:
					if (event->delta_y > 0)
						gtk_notebook_next_page (notebook);
					else if (event->delta_y < 0)
						gtk_notebook_prev_page (notebook);
					break;
				case GTK_POS_TOP:
				case GTK_POS_BOTTOM:
					if (event->delta_x > 0)
						gtk_notebook_next_page (notebook);
					else if (event->delta_x < 0)
						gtk_notebook_prev_page (notebook);
					break;
			}
			break;
	}
}
#endif

