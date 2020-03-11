/**
 * awf : A Widget Factory
 *
 * A theme preview application for gtk2 and gtk3.
 *
 * Copyright (C) 2011-2017 Valère Monseur (valere dot monseur at ymail dot com)
 *
 * This file is part of awf.
 *
 * awf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * awf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with awf. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * awf is based on the code from 'The Widget Factory'.
 *
 * The Widget Factory is a program designed to show a wide range of widgets
 * on a single window.
 *
 * Created by Richard Stellingwerff <remenic@gmail.com>.
 *
 *
 * Widgets difference between gtk2 and gtk3:
 * - gtk2: GtkOptionMenu exists. GtkSwitch doesn't exist yet.
 * - gtk3: GtkOptionMenu doesn't exist anymore. GtkSwitch exists.
 */

// includes

#include <glib/gprintf.h>
#include <glib-unix.h>
#include "gtk_empty.h"
#include <gtk/gtk.h>

// defines

#define EMPTY (gtk_empty_new ())

// enums

enum {
	COLUMN1 = 0,
	COLUMN2,
	NUM_COLS
} ;

// global local variables :)

static GSList *list_system_theme = NULL;
static GSList *list_user_theme = NULL;

static gboolean refresh_theme = FALSE;

static GtkWidget *window;
static GtkWidget *progressbar1, *progressbar2, *progressbar3, *progressbar4;
static GtkWidget *scale1, *scale2, *scale3, *scale4, *harmony, *showtext;
static GtkWidget *spinner;

// local functions

static GSList* awf_load_theme (gchar *directory);
static void awf_exclude_theme (gpointer theme, gpointer unused);
static gint awf_compare_theme (gconstpointer theme1, gconstpointer theme2);
static void awf_set_theme (gpointer theme, guint callback_action, GtkWidget *unused);
static void awf_refresh_theme (GtkWidget *widget, gpointer unused);
static gboolean awf_check_refresh_theme (gpointer unused);
static gboolean awf_sighup_handler (gpointer unused);

static void awf_window_set_title (void);

static GtkWidget* awf_build_menu (GtkWidget *widget);
static void awf_add_menu_item (gpointer theme, gpointer menu);

static void awf_run_me (GtkWidget *widget, gpointer unused);
static void awf_run_me_set_environment (gpointer display);

static void awf_on_scale_value_changed (GtkRange *range, gpointer unused);

static void awf_showtext_clicked (GtkWidget *widget, gpointer unused);

// run baby, run!

GtkWidget *statusbar;
GSList *theme_group = NULL;

int main (int argc, char **argv)
{
	gchar *directory;
	GtkWidget *menubar, *toolbar;
	GtkWidget *vbox_window, *vbox_widget;
	GtkWidget *vbox_label_treeview, *vbox_other_button, *vbox_progressbar_scale;
	GtkWidget *vbox_combo_entry_spin_check_radio_button;
	GtkWidget *vbox_combo_entry, *vbox_check_button, *vbox_radio_button;
	GtkWidget *vbox_progressbar, *vbox_scale;
	GtkWidget *vbox_buttonbox;
	GtkWidget *hbox1, *hbox_check_radio_button;
	GtkWidget *hbox_frame1, *hbox_frame2, *hbox_notebook1, *hbox_notebook2;
	GtkWidget *hbox_spin_button, *hbox_progressbar, *hbox_scale;
	GtkWidget *button1, *button2;
	GtkWidget *button3, *button4;
	GtkWidget *button5, *button6;
	GtkWidget *button7, *button8, *button9, *button10;
	GtkWidget *button11, *button12, *button13, *button14, *button15, *button16;
	GtkWidget *button17, *button18, *button19, *button20, *button21, *button22;
	GtkWidget *button23, *button24;
	GtkWidget *menu;
	GtkWidget *combo1, *combo2;
	GtkWidget *combo3, *combo4;
	GtkWidget *entry0, *entry1, *entry2;
	GtkWidget *radio1, *radio2, *radio3, *radio4;
	GtkWidget *frame1, *frame2, *frame3, *frame4;
	GtkWidget *notebook1, *notebook2, *notebook3, *notebook4;
	GtkWidget *vseparator1, *vseparator2, *vseparator3;
	GtkWidget *hseparator1;
	GtkWidget *label1, *label2;
	GtkWidget *tooltip;
	GtkWidget *vpane1, *hpane1, *hpane2;
	GtkWidget *scrolled_window1, *scrolled_window2;
	GtkWidget *refresh, *awf, *icon1, *icon2, *icon3;
	GtkWidget *levelbar1, *levelbar2;
	GtkCellRenderer *renderer1, *renderer2;
	GtkTreeModel *model1, *model2;
	GtkWidget *view1, *view2;
	GtkListStore *store1, *store2;
	GtkTreeIter iter;
#if GTK_CHECK_VERSION (3,14,0)
	GdkRGBA color;
#else
	GdkColor color;
#endif

	const gchar* scale_icons[] =
	{
		"awf",
		NULL
	};

	gtk_init (&argc, &argv);

	/* load themes available at system level */

	list_system_theme = awf_load_theme ("/usr/share/themes");
	list_system_theme = g_slist_sort (list_system_theme, (GCompareFunc) awf_compare_theme);

	/* load themes available at user level */

	directory = g_build_path ("/", g_getenv ("HOME"), ".themes", NULL);
	list_user_theme = awf_load_theme (directory);
	list_user_theme = g_slist_sort (list_user_theme, (GCompareFunc) awf_compare_theme);
	g_free (directory);

	/* exclude themes at system level also available at user level */

	g_slist_foreach (list_user_theme, awf_exclude_theme, NULL);

	/* window layout */

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	awf_window_set_title ();

#if GTK_CHECK_VERSION (3,2,0)
	vbox_window = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_window), FALSE);
#else
	vbox_window = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (window), vbox_window);

	menubar = awf_build_menu (window);
	gtk_box_pack_start (GTK_BOX (vbox_window), menubar, FALSE, FALSE, 0);

	toolbar = gtk_toolbar_new ();
	gtk_box_pack_start (GTK_BOX (vbox_window), toolbar, FALSE, FALSE, 0);
#if GTK_CHECK_VERSION (3,0,0)
	gtk_style_context_add_class (gtk_widget_get_style_context (toolbar), "primary-toolbar");
#endif

#if GTK_CHECK_VERSION (3,2,0)
	vbox_widget = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_widget), FALSE);
#else
	vbox_widget = gtk_vbox_new (FALSE, 0);
#endif
	gtk_box_pack_start (GTK_BOX (vbox_window), vbox_widget, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (hbox1), FALSE);
#else
	hbox1 = gtk_hbox_new (FALSE, 0);
#endif
	gtk_box_pack_start (GTK_BOX (vbox_widget), hbox1, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	hseparator1 = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
#else
	hseparator1 = gtk_hseparator_new ();
#endif
	gtk_container_add (GTK_CONTAINER (vbox_widget), hseparator1);

#if GTK_CHECK_VERSION (3,2,0)
	vpane1 = gtk_paned_new (GTK_ORIENTATION_VERTICAL);
#else
	vpane1 = gtk_vpaned_new ();
#endif
	gtk_box_pack_start (GTK_BOX (vbox_widget), vpane1, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	hpane1 = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
#else
	hpane1 = gtk_hpaned_new ();
#endif
	gtk_paned_pack1 (GTK_PANED (vpane1), hpane1, TRUE, FALSE);

#if GTK_CHECK_VERSION (3,2,0)
	hpane2 = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
#else
	hpane2 = gtk_hpaned_new ();
#endif
	gtk_paned_pack2 (GTK_PANED (vpane1), hpane2, TRUE, FALSE);

#if GTK_CHECK_VERSION (3,2,0)
	hbox_frame1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	hbox_frame2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_set_homogeneous (GTK_BOX (hbox_frame1), TRUE);
	gtk_box_set_homogeneous (GTK_BOX (hbox_frame2), TRUE);
#else
	hbox_frame1 = gtk_hbox_new (TRUE, 3);
	hbox_frame2 = gtk_hbox_new (TRUE, 3);
#endif
	gtk_widget_set_size_request (hbox_frame1, -1, 70);
	gtk_widget_set_size_request (hbox_frame2, -1, 70);
	gtk_container_set_border_width (GTK_CONTAINER (hbox_frame1), 10);
	gtk_container_set_border_width (GTK_CONTAINER (hbox_frame2), 10);

#if GTK_CHECK_VERSION (3,2,0)
	hbox_notebook1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	hbox_notebook2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_set_homogeneous (GTK_BOX (hbox_notebook1), TRUE);
	gtk_box_set_homogeneous (GTK_BOX (hbox_notebook2), TRUE);
#else
	hbox_notebook1 = gtk_hbox_new (TRUE, 3);
	hbox_notebook2 = gtk_hbox_new (TRUE, 3);
#endif
	gtk_container_set_border_width (GTK_CONTAINER (hbox_notebook1), 10);
	gtk_container_set_border_width (GTK_CONTAINER (hbox_notebook2), 10);

	gtk_paned_add1 (GTK_PANED (hpane1), hbox_frame1);
	gtk_paned_add2 (GTK_PANED (hpane1), hbox_frame2);

	gtk_paned_add1 (GTK_PANED (hpane2), hbox_notebook1);
	gtk_paned_add2 (GTK_PANED (hpane2), hbox_notebook2);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_combo_entry_spin_check_radio_button = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_combo_entry_spin_check_radio_button), FALSE);
#else
	vbox_combo_entry_spin_check_radio_button = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_set_border_width (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), 10);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox_combo_entry_spin_check_radio_button, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	vseparator1 = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
#else
	vseparator1 = gtk_vseparator_new ();
#endif
	gtk_container_add (GTK_CONTAINER (hbox1), vseparator1);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_other_button = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_other_button), FALSE);
#else
	vbox_other_button = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_set_border_width (GTK_CONTAINER (vbox_other_button), 10);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox_other_button, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	vseparator2 = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
#else
	vseparator2 = gtk_vseparator_new ();
#endif
	gtk_container_add (GTK_CONTAINER (hbox1), vseparator2);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_progressbar_scale = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
	gtk_box_set_homogeneous (GTK_BOX (vbox_progressbar_scale), FALSE);
#else
	vbox_progressbar_scale = gtk_vbox_new (FALSE, 10);
#endif
	gtk_container_set_border_width (GTK_CONTAINER (vbox_progressbar_scale), 10);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox_progressbar_scale, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	vseparator3 = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
#else
	vseparator3 = gtk_vseparator_new ();
#endif
	gtk_container_add (GTK_CONTAINER (hbox1), vseparator3);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_label_treeview = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_label_treeview), FALSE);
#else
	vbox_label_treeview = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_set_border_width (GTK_CONTAINER (vbox_label_treeview), 10);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox_label_treeview, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_combo_entry = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_combo_entry), FALSE);
#else
	vbox_combo_entry = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), vbox_combo_entry);

#if GTK_CHECK_VERSION (3,2,0)
	hbox_spin_button = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (hbox_spin_button), TRUE);
#else
	hbox_spin_button = gtk_hbox_new (TRUE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), hbox_spin_button);

#if GTK_CHECK_VERSION (3,2,0)
	hbox_check_radio_button = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (hbox_check_radio_button), FALSE);
#else
	hbox_check_radio_button = gtk_hbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), hbox_check_radio_button);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_check_button = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_check_button), FALSE);
#else
	vbox_check_button = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (hbox_check_radio_button), vbox_check_button);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_radio_button = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_radio_button), FALSE);
#else
	vbox_radio_button = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (hbox_check_radio_button), vbox_radio_button);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_progressbar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_progressbar), FALSE);
#else
	vbox_progressbar = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), vbox_progressbar);

#if GTK_CHECK_VERSION (3,2,0)
	hbox_progressbar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (hbox_progressbar), TRUE);
#else
	hbox_progressbar = gtk_hbox_new (TRUE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), hbox_progressbar);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_scale = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vbox_scale), FALSE);
#else
	vbox_scale = gtk_vbox_new (FALSE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), vbox_scale);

#if GTK_CHECK_VERSION (3,2,0)
	hbox_scale = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (hbox_scale), TRUE);
#else
	hbox_scale = gtk_hbox_new (TRUE, 0);
#endif
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), hbox_scale);

	/* toolbar */
	awf = GTK_WIDGET (gtk_tool_button_new (gtk_image_new_from_icon_name ("awf", gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar))), NULL));
#if GTK_CHECK_VERSION (3,10,0)
	refresh = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
	gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (refresh), "gtk-refresh");
#else
	refresh = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH));
#endif
#if GTK_CHECK_VERSION (3,10,0)
	icon1 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
	gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon1), "gtk-goto-first");
#else
	icon1 = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_FIRST));
#endif
#if GTK_CHECK_VERSION (3,10,0)
	icon2 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
	gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon2), "gtk-goto-last");
#else
	icon2 = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_LAST));
#endif
#if GTK_CHECK_VERSION (3,10,0)
	icon3 = GTK_WIDGET (gtk_tool_button_new (NULL, NULL));
	gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (icon3), "gtk-add");
#else
	icon3 = GTK_WIDGET (gtk_toggle_tool_button_new_from_stock (GTK_STOCK_ADD));
#endif

	gtk_widget_set_tooltip_text (refresh, "Refresh theme");
#if GTK_CHECK_VERSION (3,0,0)
	gtk_widget_set_tooltip_text (awf, "Run awf for gtk2");
#else
	gtk_widget_set_tooltip_text (awf, "Run awf for gtk3");
#endif
	gtk_widget_set_sensitive (icon2, FALSE);
	gtk_toggle_tool_button_set_active (GTK_TOOL_BUTTON (icon3), TRUE);
	g_signal_connect (G_OBJECT (refresh), "clicked", G_CALLBACK (awf_refresh_theme), NULL);
	g_signal_connect (G_OBJECT (awf), "clicked", G_CALLBACK (awf_run_me), NULL);

	/* entry0 = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (entry0), "Search"); */

	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM (awf), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM (refresh), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), gtk_separator_tool_item_new (), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM (icon1), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM (icon2), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), gtk_separator_tool_item_new (), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM (icon3), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), gtk_separator_tool_item_new (), -1);
	/* gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM (entry0), -1); */

	/* combo boxes and entries */

	combo1 = gtk_combo_box_text_new_with_entry ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo1), "Combo box entry");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo1), "Item2");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo1), 0);

	combo2 = gtk_combo_box_text_new_with_entry ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo2), "Combo box entry (disabled)");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo2), "Item2");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo2), 0);
	gtk_widget_set_sensitive (combo2, FALSE);

	combo3 = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo3), "Combo box");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo3), "Item2");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo3), 0);

	combo4 = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo4), "Combo box (disabled)");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo4), "Item2");
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo4), 0);
	gtk_widget_set_sensitive (combo4, FALSE);

	entry1 = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (entry1), "Entry");

	entry2 = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (entry2), "Entry (disabled)");
	gtk_widget_set_sensitive (entry2, FALSE);

	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), combo1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), combo2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), combo3, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), combo4, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), entry1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), entry2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_combo_entry), EMPTY, FALSE, FALSE, 0);

	/* spin buttons */

	button23 = gtk_spin_button_new_with_range (1, 10, 1);
	button24 = gtk_spin_button_new_with_range (1, 10, 1);
	gtk_widget_set_sensitive (button24, FALSE);

	gtk_box_pack_start (GTK_BOX (hbox_spin_button), button23, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_spin_button), button24, FALSE, FALSE, 0);

	/* Check buttons */

	button11 = gtk_check_button_new_with_label ("Check button1");
	button12 = gtk_check_button_new_with_label ("Check button2");
	button13 = gtk_check_button_new_with_label ("Check button3");
	button14 = gtk_check_button_new_with_label ("Check button4");
	button15 = gtk_check_button_new_with_label ("Check button5");
	button16 = gtk_check_button_new_with_label ("Check button6");

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button12), TRUE);
	gtk_toggle_button_set_inconsistent ( GTK_TOGGLE_BUTTON (button13), TRUE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button15), TRUE);
	gtk_toggle_button_set_inconsistent ( GTK_TOGGLE_BUTTON (button16), TRUE);
	gtk_widget_set_sensitive (button14, FALSE);
	gtk_widget_set_sensitive (button15, FALSE);
	gtk_widget_set_sensitive (button16, FALSE);

	gtk_box_pack_start (GTK_BOX (vbox_check_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button11, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button12, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button13, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button14, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button15, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button16, FALSE, FALSE, 0);

	/* radio buttons */

	radio1 = gtk_radio_button_new (NULL);
	radio2 = gtk_radio_button_new (NULL);
	radio3 = gtk_radio_button_new (NULL);
	radio4 = gtk_radio_button_new (NULL);

	button17 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Radio button1");
	button18 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Radio button2");
	button19 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio2), "Radio button3");
	button20 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio3), "Radio button4");
	button21 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio3), "Radio button5");
	button22 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio4), "Radio button6");

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button18), TRUE);
	gtk_toggle_button_set_inconsistent ( GTK_TOGGLE_BUTTON (button19), TRUE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button21), TRUE);
	gtk_toggle_button_set_inconsistent ( GTK_TOGGLE_BUTTON (button22), TRUE);
	gtk_widget_set_sensitive (button20, FALSE);
	gtk_widget_set_sensitive (button21, FALSE);
	gtk_widget_set_sensitive (button22, FALSE);

	gtk_box_pack_start (GTK_BOX (vbox_radio_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button17, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button18, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button19, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button20, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button21, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button22, FALSE, FALSE, 0);

	/* other buttons */

	button1 = gtk_button_new_with_label ("Button1");
	button2 = gtk_button_new_with_label ("Button2");
	gtk_widget_set_sensitive (button2, FALSE);

	button3 = gtk_toggle_button_new_with_label ("Toggle button1");
	button4 = gtk_toggle_button_new_with_label ("Toggle button2");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button3), TRUE);
	gtk_widget_set_sensitive (button4, FALSE);

	button5 = gtk_font_button_new ();

#if GTK_CHECK_VERSION (3,14,0)
	gdk_rgba_parse (&color, "#7796ba");
	button6 = gtk_color_button_new_with_rgba (&color);
#else
	gdk_color_parse ("#7796ba", &color);
	button6 = gtk_color_button_new_with_color (&color);
#endif

	button7 = gtk_file_chooser_button_new ("Select file...", GTK_FILE_CHOOSER_ACTION_OPEN);

#if GTK_CHECK_VERSION (3,0,0)
	button8 = gtk_switch_new ();
	gtk_switch_set_active (GTK_SWITCH (button8), TRUE);
#else
	menu = gtk_menu_new ();
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), gtk_menu_item_new_with_label ("Option menu"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), gtk_menu_item_new_with_label ("Item"));

	button8 = gtk_option_menu_new ();
	gtk_option_menu_set_menu (GTK_OPTION_MENU (button8), menu);
#endif

	button9 = gtk_link_button_new_with_label ("https://github.com/valr/awf", "Link button AWF");
	button10 = gtk_scale_button_new (GTK_ICON_SIZE_BUTTON, 1, 10, 1, scale_icons);

	gtk_box_pack_start (GTK_BOX (vbox_other_button), button1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button3, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button4, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button5, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button6, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button7, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button8, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button9, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), button10, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), EMPTY, FALSE, FALSE, 0);

#if GTK_CHECK_VERSION (3,6,0)
	levelbar1 = gtk_level_bar_new ();
	gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar1), GTK_LEVEL_BAR_MODE_CONTINUOUS);
	gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar1), 0.6);

	levelbar2 = gtk_level_bar_new_for_interval (0, 5);
	gtk_level_bar_set_mode (GTK_LEVEL_BAR (levelbar2), GTK_LEVEL_BAR_MODE_DISCRETE);
	gtk_level_bar_set_value (GTK_LEVEL_BAR (levelbar2), 2);

	gtk_box_pack_start (GTK_BOX (vbox_other_button), levelbar1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_other_button), levelbar2, FALSE, FALSE, 0);
#endif

	/* progress bars */

	progressbar1 = gtk_progress_bar_new ();
	progressbar2 = gtk_progress_bar_new ();
	progressbar3 = gtk_progress_bar_new ();
	progressbar4 = gtk_progress_bar_new ();

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), 0.5);

#if GTK_CHECK_VERSION (3,0,0)
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar1), GTK_ORIENTATION_HORIZONTAL);
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar2), GTK_ORIENTATION_HORIZONTAL);
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar3), GTK_ORIENTATION_VERTICAL);
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar4), GTK_ORIENTATION_VERTICAL);

	gtk_progress_bar_set_inverted (GTK_PROGRESS_BAR (progressbar2), TRUE);
	gtk_progress_bar_set_inverted (GTK_PROGRESS_BAR (progressbar4), TRUE);

	gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar1), FALSE);
	gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar3), FALSE);
#else
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar1), GTK_PROGRESS_LEFT_TO_RIGHT);
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar2), GTK_PROGRESS_RIGHT_TO_LEFT);
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar3), GTK_PROGRESS_TOP_TO_BOTTOM);
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar4), GTK_PROGRESS_BOTTOM_TO_TOP);

	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), "");
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), "");
#endif

	gtk_box_pack_start (GTK_BOX (vbox_progressbar), progressbar1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_progressbar), progressbar2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_progressbar), progressbar3, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_progressbar), progressbar4, FALSE, FALSE, 0);

	/* scales */

#if GTK_CHECK_VERSION (3,2,0)
	scale1 = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, 0, 100, 1);
	scale2 = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, 0, 100, 1);
	scale3 = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
	scale4 = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
#else
	scale1 = gtk_vscale_new_with_range (0, 100, 1);
	scale2 = gtk_vscale_new_with_range (0, 100, 1);
	scale3 = gtk_hscale_new_with_range (0, 100, 1);
	scale4 = gtk_hscale_new_with_range (0, 100, 1);
#endif
	gtk_widget_set_size_request (scale1, -1, 100);
	gtk_widget_set_size_request (scale2, -1, 100);
	gtk_widget_set_size_request (scale3, 100, -1);
	gtk_widget_set_size_request (scale4, 100, -1);

	gtk_scale_set_draw_value (GTK_SCALE (scale1), FALSE);
	gtk_scale_set_draw_value (GTK_SCALE (scale2), FALSE);
	gtk_scale_set_draw_value (GTK_SCALE (scale3), FALSE);
	gtk_scale_set_draw_value (GTK_SCALE (scale4), FALSE);

	gtk_range_set_value (GTK_RANGE (scale1), 50);
	gtk_range_set_value (GTK_RANGE (scale2), 50);
	gtk_range_set_value (GTK_RANGE (scale3), 50);
	gtk_range_set_value (GTK_RANGE (scale4), 50);

	gtk_range_set_inverted (GTK_RANGE (scale2), TRUE);
	gtk_range_set_inverted (GTK_RANGE (scale4), TRUE);

	g_signal_connect ((gpointer)scale1, "value_changed", G_CALLBACK (awf_on_scale_value_changed), NULL);
	g_signal_connect ((gpointer)scale2, "value_changed", G_CALLBACK (awf_on_scale_value_changed), NULL);
	g_signal_connect ((gpointer)scale3, "value_changed", G_CALLBACK (awf_on_scale_value_changed), NULL);
	g_signal_connect ((gpointer)scale4, "value_changed", G_CALLBACK (awf_on_scale_value_changed), NULL);

	gtk_box_pack_start (GTK_BOX (hbox_scale), scale1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_scale), scale2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_scale), scale3, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_scale), scale4, FALSE, FALSE, 0);

#if GTK_CHECK_VERSION (3,2,0)
	vbox_buttonbox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
#else
	vbox_buttonbox = gtk_vbutton_box_new ();
#endif
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbox_buttonbox), GTK_BUTTONBOX_START);
	gtk_box_pack_start (GTK_BOX (vbox_progressbar_scale), vbox_buttonbox, FALSE, FALSE, 0);

	harmony = gtk_check_button_new_with_label ("Move in harmony");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (harmony), TRUE);
	gtk_box_pack_start (GTK_BOX (vbox_buttonbox), harmony, FALSE, FALSE, 0);

	showtext = gtk_check_button_new_with_label ("Show text");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (showtext), FALSE);
	gtk_box_pack_start (GTK_BOX (vbox_buttonbox), showtext, FALSE, FALSE, 0);
	g_signal_connect (showtext, "clicked", G_CALLBACK (awf_showtext_clicked), NULL);

	/* treeview */

	view1 = gtk_tree_view_new ();

	renderer1 = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view1), -1, "Column1", renderer1, "text", COLUMN1, NULL);

	renderer1 = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view1), -1, "Column2", renderer1, "text", COLUMN2, NULL);

	store1 = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

	gtk_list_store_append (store1, &iter);
	gtk_list_store_set (store1, &iter, COLUMN1, "Cell1.1", COLUMN2, "Cell1.2", -1);
	gtk_list_store_append (store1, &iter);
	gtk_list_store_set (store1, &iter, COLUMN1, "Cell2.1", COLUMN2, "Cell2.2", -1);

	model1 = GTK_TREE_MODEL (store1);
	gtk_tree_view_set_model (GTK_TREE_VIEW (view1), model1);

	scrolled_window1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (scrolled_window1, 200, -1);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window1), GTK_POLICY_NEVER, GTK_POLICY_NEVER);
	gtk_container_add (GTK_CONTAINER (scrolled_window1), view1);

	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), scrolled_window1, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), EMPTY, FALSE, FALSE, 0);

	view2 = gtk_tree_view_new ();

	renderer2 = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view2), -1, "Longer column1", renderer2, "text", COLUMN1, NULL);

	renderer2 = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view2), -1, "Longer column2", renderer2, "text", COLUMN2, NULL);

	store2 = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

	gtk_list_store_append (store2, &iter);
	gtk_list_store_set (store2, &iter, COLUMN1, "Longer cell1.1", COLUMN2, "Longer cell1.2", -1);
	gtk_list_store_append (store2, &iter);
	gtk_list_store_set (store2, &iter, COLUMN1, "Longer cell2.1", COLUMN2, "Longer cell2.2", -1);
	gtk_list_store_append (store2, &iter);
	gtk_list_store_set (store2, &iter, COLUMN1, "Longer cell3.1", COLUMN2, "Longer cell3.2", -1);
	gtk_list_store_append (store2, &iter);
	gtk_list_store_set (store2, &iter, COLUMN1, "Longer cell4.1", COLUMN2, "Longer cell4.2", -1);

	model2 = GTK_TREE_MODEL (store2);
	gtk_tree_view_set_model (GTK_TREE_VIEW (view2), model2);

	scrolled_window2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (scrolled_window2, 200, -1);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window2), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (scrolled_window2), view2);

	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), scrolled_window2, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), EMPTY, FALSE, FALSE, 0);

	/* labels, tooltip and spinner */

	label1 = gtk_label_new ("Label1");
	label2 = gtk_label_new ("Label2");
	gtk_widget_set_sensitive (label2, FALSE);

	tooltip = gtk_label_new ("Tooltip");
	gtk_widget_set_tooltip_text (tooltip, "Enjoy a widget factory!");

	spinner = gtk_spinner_new ();
	gtk_widget_set_size_request (spinner, 10, 10);
	gtk_spinner_start (GTK_SPINNER (spinner));

	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), label1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), label2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), tooltip, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), spinner, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), EMPTY, FALSE, FALSE, 0);

	/* frames */

	frame1 = gtk_frame_new ("Frame (shadow in)");
	frame2 = gtk_frame_new ("Frame (shadow out)");
	frame3 = gtk_frame_new ("Frame (shadow etched in)");
	frame4 = gtk_frame_new ("Frame (shadow etched out)");

	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_OUT);
	gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_ETCHED_IN);
	gtk_frame_set_shadow_type (GTK_FRAME (frame4), GTK_SHADOW_ETCHED_OUT);

	gtk_box_pack_start (GTK_BOX (hbox_frame1), frame1, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_frame1), frame2, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_frame2), frame3, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_frame2), frame4, TRUE, TRUE, 0);

	/* notebooks */

	notebook1 = gtk_notebook_new ();
	notebook2 = gtk_notebook_new ();
	notebook3 = gtk_notebook_new ();
	notebook4 = gtk_notebook_new ();

	gtk_notebook_append_page (GTK_NOTEBOOK (notebook1), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook1), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook1), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook1), GTK_POS_TOP);

	gtk_notebook_append_page (GTK_NOTEBOOK (notebook2), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook2), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook2), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook2), GTK_POS_BOTTOM);

	gtk_notebook_append_page (GTK_NOTEBOOK (notebook3), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook3), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook3), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook3), GTK_POS_LEFT);

	gtk_notebook_append_page (GTK_NOTEBOOK (notebook4), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook4), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook4), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook4), GTK_POS_RIGHT);

	gtk_box_pack_start (GTK_BOX (hbox_notebook1), notebook1, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_notebook1), notebook2, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_notebook2), notebook3, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_notebook2), notebook4, FALSE, TRUE, 0);

	/* status bar */

	statusbar = gtk_statusbar_new ();
	gtk_box_pack_start (GTK_BOX (vbox_window), statusbar, FALSE, FALSE, 0);
	gtk_statusbar_push (GTK_STATUSBAR (statusbar), gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "gne"), "This is AWF 2.0.0");

	/* refresh on SIGHUP */

	g_unix_signal_add (SIGHUP, awf_sighup_handler, NULL);
	g_timeout_add_seconds (1, awf_check_refresh_theme, NULL);

	/* go! */

	switch (argc)
	{
		case 2:
			if (g_slist_find_custom (list_system_theme, (gconstpointer)argv[1], &awf_compare_theme) ||
				g_slist_find_custom (list_user_theme, (gconstpointer)argv[1], &awf_compare_theme))
				awf_set_theme (argv[1], 0, NULL);

		case 1:
			gtk_widget_show_all (window);
			gtk_main ();
			break;

		default:
			g_printf ("Usage:\n");
#if GTK_CHECK_VERSION (3,0,0)
			g_printf ("\t%s\t\t\t%s\n", "awf-gtk3", "Run a widget factory");
			g_printf ("\t%s %s\t\t%s\n", "awf-gtk3", "<theme>", "Run a widget factory with the specified theme");
#else
			g_printf ("\t%s\t\t\t%s\n", "awf-gtk2", "Run a widget factory");
			g_printf ("\t%s %s\t\t%s\n", "awf-gtk2", "<theme>", "Run a widget factory with the specified theme");
#endif
			break;
	}

	return 0;
}

static GSList*
awf_load_theme (gchar *directory)
{
	GSList *list = NULL;

	g_return_val_if_fail (directory != NULL, NULL);

	if (g_file_test (directory, G_FILE_TEST_EXISTS)) {

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
				/* don't free 'theme', it's the data part of GSList elements */

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

static void
awf_exclude_theme (gpointer theme, gpointer unused)
{
	GSList *found_theme = g_slist_find_custom (list_system_theme, (gconstpointer)theme, &awf_compare_theme);

	if (found_theme) {
		g_free (found_theme->data);
		list_system_theme = g_slist_delete_link (list_system_theme, found_theme);
	}
}

static gint
awf_compare_theme (gconstpointer theme1, gconstpointer theme2)
{
	return g_strcmp0 ((gchar*)theme1, (gchar*)theme2);
}

static void
awf_set_theme (gpointer theme, guint callback_action, GtkWidget *unused)
{
#if GTK_CHECK_VERSION (3,16,0)
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", (const gchar*) theme, "gtkrc:0");
#else
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", (const gchar*) theme, "gtkrc:0");
#endif
	awf_window_set_title ();
}

static void
awf_refresh_theme (GtkWidget *widget, gpointer unused)
{
	gchar *default_theme = NULL, *current_theme = NULL, *text;

	if (g_slist_find_custom (list_system_theme, "Default", &awf_compare_theme))
		default_theme = g_strdup ("Default");
	else if (g_slist_find_custom (list_system_theme, "Raleigh", &awf_compare_theme))
		default_theme = g_strdup ("Raleigh");

	if (default_theme) {
		g_object_get (gtk_settings_get_default (), "gtk-theme-name", &current_theme, NULL);

#if GTK_CHECK_VERSION (3,16,0)
		gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", default_theme, NULL);
		g_usleep (G_USEC_PER_SEC);
		gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", (const gchar*) current_theme, NULL);
#else
		gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", default_theme, NULL);
		g_usleep (G_USEC_PER_SEC);
		gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", (const gchar*) current_theme, NULL);
#endif

		text = g_strjoin (" ", "Theme", (const gchar*) current_theme, "reloaded.", NULL);
		gtk_statusbar_push (GTK_STATUSBAR (statusbar), gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "gne"), text);

		g_free (current_theme);
		g_free (default_theme);
		g_free (text);
	} else {
		g_warning ("No default theme found (neither \"Default\" nor \"Raleigh\"), refresh of theme might not work.");
	}
}

static gboolean
awf_check_refresh_theme (gpointer unused)
{
	if (refresh_theme) {
		refresh_theme = FALSE;
		awf_refresh_theme (NULL, NULL);
	}

	return TRUE;
}

static gboolean
awf_sighup_handler (gpointer unused)
{
	refresh_theme = TRUE;
	return G_SOURCE_CONTINUE;
}

static void
awf_window_set_title (void)
{
	gchar *title, *theme;

	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &theme, NULL);

#if GTK_CHECK_VERSION (3,0,0)
	title = g_strjoin (" - ", "A widget factory", theme, "Gtk3", NULL);
#else
	title = g_strjoin (" - ", "A widget factory", theme, "Gtk2", NULL);
#endif

	gtk_window_set_title (GTK_WINDOW (window), title);

	g_free (title);
	g_free (theme);
}

static GtkWidget*
awf_build_menu (GtkWidget *widget)
{
	GtkAccelGroup *ag;
	GtkWidget *menubar;
	GtkWidget *menu, *menu2;
	GtkWidget *menuitem;
	GtkWidget *menuimage;
	GSList *group1 = NULL, *group2 = NULL;
	guint accelerator_key;
	GdkModifierType accelerator_mods;
	gint i, j;

	struct _MenuInfo {
		gint type;
		gchar mnemonic[32];
		gchar accelerator[32];
		gchar *image;
		void (*callback) (gpointer, guint, GtkWidget*);
		gboolean checked;
		gboolean inconsistent;
		gboolean disabled;
	} info[30] = {
		{ 0, "_System theme" , "<Control>s" },
		{ 1, "" },
		{ 0, "_User theme"   , "<Control>u" },
		{ 2, "" },
		{ 0, "_Test menu"    , "<Control>t" },
		{ 7, "" },
#if GTK_CHECK_VERSION (3,10,0)
		{ 3, "", "<Control>o", "gtk-open" },
		{ 3, "", "<Control>s", "gtk-save" },
		{ 3, "", "", "gtk-save-as" },
		{ 9, "More..." },
		{ 3, "", "<Control>p", "gtk-print" },
#else
		{ 3, "", "<Control>o", GTK_STOCK_OPEN },
		{ 3, "", "<Control>s", GTK_STOCK_SAVE },
		{ 3, "", "", GTK_STOCK_SAVE_AS },
		{ 9, "More..." },
		{ 3, "", "<Control>p", GTK_STOCK_PRINT },
#endif
		{ 4, "" },
		{ 5, "Radio 1 (unchecked)"     , "", NULL, NULL, FALSE, FALSE, FALSE },
		{ 5, "Radio 2 (checked)"       , "", NULL, NULL, TRUE,  FALSE, FALSE },
		{ 5, "Radio 3 (inconsistent)"  , "", NULL, NULL, FALSE, TRUE,  FALSE },
		{ 4, ""              , ""          , NULL, NULL },
		{ 6, "Check 1 (unchecked)"     , "", NULL, NULL, FALSE, FALSE, FALSE },
		{ 6, "Check 2 (checked)"       , "", NULL, NULL, TRUE,  FALSE, FALSE },
		{ 6, "Check 3 (inconsistent)"  , "", NULL, NULL, FALSE, TRUE,  FALSE },
		{ 4, ""              , ""          , NULL, NULL },
		{ 5, "Radio 1 (unchecked)"     , "", NULL, NULL, FALSE, FALSE, TRUE },
		{ 5, "Radio 2 (checked)"       , "", NULL, NULL, TRUE,  FALSE, TRUE },
		{ 5, "Radio 3 (inconsistent)"  , "", NULL, NULL, FALSE, TRUE,  TRUE },
		{ 4, ""              , ""          , NULL, NULL },
		{ 6, "Check 1 (unchecked)"     , "", NULL, NULL, FALSE, FALSE, TRUE },
		{ 6, "Check 2 (checked)"       , "", NULL, NULL, TRUE,  FALSE, TRUE },
		{ 6, "Check 3 (inconsistent)"  , "", NULL, NULL, FALSE, TRUE,  TRUE },
		{ 4, "" },
#if GTK_CHECK_VERSION (3,10,0)
		{ 3, "", "<Control>w", "gtk-close", NULL, FALSE, FALSE, TRUE },
		{ 3, "", "<Control>q", "gtk-quit" },
#else
		{ 3, "", "<Control>w", GTK_STOCK_CLOSE, NULL, FALSE, FALSE, TRUE },
		{ 3, "", "<Control>q", GTK_STOCK_QUIT },
#endif
	}, subinfo[3] = {
#if GTK_CHECK_VERSION (3,10,0)
		{ 3, "", "<Control>x", "gtk-cut" },
		{ 3, "", "<Control>c", "gtk-copy" },
		{ 3, "", "<Control>v", "gtk-paste" },
#else
		{ 3, "", "<Control>x", GTK_STOCK_CUT },
		{ 3, "", "<Control>c", GTK_STOCK_COPY },
		{ 3, "", "<Control>v", GTK_STOCK_PASTE },
#endif
	};

	ag = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (widget), ag);

	menubar = gtk_menu_bar_new ();

	for (i = 0; i < 30; i++) {
		switch (info[i].type) {
			case 0:
				menu = gtk_menu_new ();
				gtk_widget_show (menu);

				menuitem = gtk_menu_item_new_with_mnemonic (info[i].mnemonic);
				gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);

				gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);
				gtk_widget_show (menuitem);
				break;

			case 9:
				menu2 = gtk_menu_new ();
				gtk_widget_show (menu2);

				menuitem = gtk_menu_item_new_with_mnemonic (info[i].mnemonic);
				gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu2);

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);

				for (j = 0; j < 3; j++) {

					if (subinfo[j].image) {
#if GTK_CHECK_VERSION (3,10,0)
						menuitem = gtk_image_menu_item_new_from_stock (subinfo[j].image, NULL);
#else
						menuitem = gtk_image_menu_item_new_from_stock (subinfo[j].image, NULL);
#endif
					} else {
						menuitem = gtk_menu_item_new_with_mnemonic (subinfo[j].mnemonic);
					}

					gtk_widget_set_sensitive (menuitem, !subinfo[j].disabled);
					gtk_menu_shell_append (GTK_MENU_SHELL (menu2), menuitem);
					gtk_widget_show (menuitem);

					gtk_accelerator_parse (subinfo[j].accelerator, &accelerator_key, &accelerator_mods);

					if (accelerator_key && accelerator_mods)
						gtk_widget_add_accelerator (menuitem, "activate", ag,
							accelerator_key, accelerator_mods, GTK_ACCEL_VISIBLE);

					if (subinfo[j].callback)
						g_signal_connect_swapped (G_OBJECT (menuitem), "activate",
												G_CALLBACK (subinfo[j].callback), widget);
				}
				break;

			case 1:
				g_slist_foreach (list_system_theme, awf_add_menu_item, menu);
				break;

			case 2:
				g_slist_foreach (list_user_theme, awf_add_menu_item, menu);
				break;

			case 3:
				if (info[i].image) {
#if GTK_CHECK_VERSION (3,10,0)
					menuitem = gtk_image_menu_item_new_from_stock (info[i].image, NULL);
#else
					menuitem = gtk_image_menu_item_new_from_stock (info[i].image, NULL);
#endif
				} else {
					menuitem = gtk_menu_item_new_with_mnemonic (info[i].mnemonic);
				}

				gtk_widget_set_sensitive (menuitem, !info[i].disabled);
				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);

				gtk_accelerator_parse (info[i].accelerator, &accelerator_key, &accelerator_mods);

				if (accelerator_key && accelerator_mods)
					gtk_widget_add_accelerator (menuitem, "activate", ag,
						accelerator_key, accelerator_mods, GTK_ACCEL_VISIBLE);

				if (info[i].callback)
					g_signal_connect_swapped (G_OBJECT (menuitem), "activate",
											G_CALLBACK (info[i].callback), widget);
				break;

			case 4:
				menuitem = gtk_separator_menu_item_new ();

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);
				break;

			case 5:
				if (info[i].disabled) {
					if (info[i].inconsistent)
						menuitem = gtk_radio_menu_item_new_with_mnemonic (NULL, info[i].mnemonic);
					else {
						menuitem = gtk_radio_menu_item_new_with_mnemonic (group2, info[i].mnemonic);
						group2 = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem));
					}
				} else {
					if (info[i].inconsistent)
						menuitem = gtk_radio_menu_item_new_with_mnemonic (NULL, info[i].mnemonic);
					else {
						menuitem = gtk_radio_menu_item_new_with_mnemonic (group1, info[i].mnemonic);
						group1 = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem));
					}
				}

				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), info[i].checked);
				gtk_check_menu_item_set_inconsistent (GTK_CHECK_MENU_ITEM (menuitem), info[i].inconsistent);
				gtk_widget_set_sensitive (menuitem, !info[i].disabled);

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);
				break;

			case 6:
				menuitem = gtk_check_menu_item_new_with_mnemonic (info[i].mnemonic);
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), info[i].checked);
				gtk_check_menu_item_set_inconsistent (GTK_CHECK_MENU_ITEM (menuitem), info[i].inconsistent);
				gtk_widget_set_sensitive (menuitem, !info[i].disabled);

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);
				break;

			case 7:
				menuitem = gtk_tearoff_menu_item_new ();
				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
		}
	}

	return menubar;
}

static void
awf_add_menu_item (gpointer theme, gpointer menu)
{
	gchar *current_theme;
	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &current_theme, NULL);

	GtkWidget *menuitem = gtk_radio_menu_item_new_with_label (theme_group, (const gchar*)theme);
	theme_group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem));

	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	gtk_widget_show (menuitem);

	g_signal_connect_swapped (G_OBJECT (menuitem), "activate", G_CALLBACK (awf_set_theme), theme);

	if (strcmp((const gchar*)current_theme, (const gchar*)theme) == 0)
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menuitem), TRUE);
	g_free (current_theme);
}

static void
awf_run_me (GtkWidget *widget, gpointer unused)
{
	gchar *awf;

#if GTK_CHECK_VERSION (3,0,0)
	awf = g_find_program_in_path ("awf-gtk2");
#else
	awf = g_find_program_in_path ("awf-gtk3");
#endif

	if (awf) {
		GError *error = NULL;
		gchar *theme, *awf_with_theme;
		gint argp; gchar **argv;
		gchar *display;

		g_object_get (gtk_settings_get_default (), "gtk-theme-name", &theme, NULL);
		awf_with_theme = g_strjoin (" ", awf, theme, NULL);
		g_shell_parse_argv (awf_with_theme, &argp, &argv, &error);

		if (error) {
			GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
									GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
									"Unable to parse command: %s (%s)", awf_with_theme, error->message);

			gtk_dialog_run (GTK_DIALOG (dialog));
			gtk_widget_destroy (dialog);

			g_strfreev (argv);
			g_free (awf_with_theme);
			g_free (theme);
			g_free (awf);
			g_error_free (error); error = NULL;
			return;
		}

		display = gdk_screen_make_display_name (gtk_window_get_screen (GTK_WINDOW (window)));

		g_spawn_async (NULL, argv, NULL,
					G_SPAWN_SEARCH_PATH | G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_STDERR_TO_DEV_NULL,
						awf_run_me_set_environment, display, NULL, &error);

		if (error) {
			GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
									GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
									"Unable to spawn command: %s (%s)", awf_with_theme, error->message);

			gtk_dialog_run (GTK_DIALOG (dialog));
			gtk_widget_destroy (dialog);

			g_error_free (error); error = NULL;
		}

		g_free (display);
		g_strfreev (argv);
		g_free (awf_with_theme);
		g_free (theme);
		g_free (awf);
	} else {
		GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
#if GTK_CHECK_VERSION (3,0,0)
								"Unable to find awf-gtk2 in path");
#else
								"Unable to find awf-gtk3 in path");
#endif

		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
	}
}

static void
awf_run_me_set_environment (gpointer display)
{
	g_setenv ("DISPLAY", display, TRUE);
}

static void
awf_on_scale_value_changed (GtkRange *range, gpointer unused)
{
	gdouble value;

	if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (harmony)))
		return;

	value = gtk_range_get_value (range);

	if (scale1 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale1), value);
	if (scale2 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale2), value);
	if (scale3 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale3), value);
	if (scale4 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale4), value);

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), value / 100.0);

	if (value >= 50.0)
		gtk_spinner_start (GTK_SPINNER (spinner));
	else
		gtk_spinner_stop (GTK_SPINNER (spinner));

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (showtext))) {
#if !GTK_CHECK_VERSION (3,0,0)
		gchar *progress_text;

		progress_text = g_strdup_printf ("%i %%", (int)value);

		gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), progress_text);
		gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), progress_text);

		g_free (progress_text);
#endif
	}
}

static void
awf_showtext_clicked (GtkWidget *widget, gpointer unused)
{
	gdouble progress_value;
	gchar *progress_text;

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (showtext))) {
		#if GTK_CHECK_VERSION (3,0,0)
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar1), TRUE);
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar3), TRUE);
		#else
			progress_value = gtk_range_get_value (GTK_RANGE (scale1));
			progress_text = g_strdup_printf ("%i %%", (int)progress_value);

			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), progress_text);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), progress_text);

			g_free (progress_text);
		#endif
	} else {
		#if GTK_CHECK_VERSION (3,0,0)
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar1), FALSE);
			gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar3), FALSE);
		#else
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar1), "");
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar3), "");
		#endif
	}
}

