/*
 *  awf : A Widget Factory
 *
 *  A theme preview application for gtk2 and gtk3.
 *
 *  Copyright (C) 2011, Valère Monseur (valere dot monseur at ymail dot com)
 *
 *  This file is part of awf.
 *
 *  awf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  awf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with awf. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * awf is based on the code from 'The Widget Factory'.
 *
 * The Widget Factory is a program designed to show a wide range of widgets
 * on a single window.
 *
 * Created by Richard Stellingwerff <remenic@gmail.com>.
 */

/*
 * Widgets difference between gtk2 and gtk3:
 * - gtk2: GtkOptionMenu exists. GtkSwitch doesn't exist yet.
 * - gtk3: GtkOptionMenu doesn't exist anymore. GtkSwitch exists.
 */

/*
 * includes
 */

#include "gtk_empty.h"
#include <gtk/gtk.h>

/*
 * defines
 */

#define EMPTY (gtk_empty_new ())

/*
 * enums
 */

enum
{
	COLUMN1 = 0,
	COLUMN2,
	NUM_COLS
} ;

/*
 * global local variables :)
 */

static GSList *list_system_theme = NULL;
static GSList *list_user_theme = NULL;

static GtkWidget *window;
static GtkWidget *progressbar1, *progressbar2, *progressbar3, *progressbar4;
static GtkWidget *scale1, *scale2, *scale3, *scale4, *harmony;

/*
 * local functions
 */

static GSList* awf_load_theme (gchar *directory);
static void awf_exclude_theme (gpointer theme, gpointer unused);
static gint awf_compare_theme (gconstpointer theme1, gconstpointer theme2);
static void awf_set_theme (gpointer theme, guint callback_action, GtkWidget *unused);
static void awf_refresh_theme (GtkWidget *widget, gpointer unused);

static void awf_window_set_title (void);

static GtkWidget* awf_build_menu (GtkWidget *widget);
static void awf_add_menu_item (gpointer theme, gpointer menu);

static void awf_run_me (GtkWidget *widget, gpointer unused);
static void awf_run_me_set_environment (gpointer display);

static void awf_on_scale_value_changed (GtkRange *range, gpointer unused);

/*
 * run baby, run!
 */

int main (int argc, char **argv)
{
	gchar *directory;
	GtkWidget *menubar, *toolbar;
	GtkWidget *vbox_window, *vbox_widget;
	GtkWidget *vbox_label_treeview, *vbox_other_button, *vbox_progressbar_scale;
	GtkWidget *vbox_combo_entry_spin_check_radio_button;
	GtkWidget *vbox_combo_entry, *vbox_check_button, *vbox_radio_button;
	GtkWidget *vbox_progressbar, *vbox_scale;
	GtkWidget *hbox1, *hbox_check_radio_button;
	GtkWidget *hbox_frame1, *hbox_frame2, *hbox_notebook1, *hbox_notebook2;
	GtkWidget *hbox_spin_button, *hbox_progressbar, *hbox_scale;
	GtkWidget *button1, *button2;
	GtkWidget *button3, *button4;
	GtkWidget *button5, *button6;
	GtkWidget *button7, *button8;
	GtkWidget *button9, *button10;
	GtkWidget *button11, *button12, *button13, *button14;
	GtkWidget *button15, *button16, *button17, *button18;
	GtkWidget *button19, *button20;
	GtkWidget *menu1, *menu2;
	GtkWidget *combo1, *combo2;
	GtkWidget *combo3, *combo4;
	GtkWidget *entry1, *entry2;
	GtkWidget *radio1, *radio2;
	GtkWidget *frame1, *frame2, *frame3, *frame4;
	GtkWidget *notebook1, *notebook2, *notebook3, *notebook4;
	GtkWidget *vseparator1, *vseparator2, *vseparator3;
	GtkWidget *hseparator1;
	GtkWidget *label1, *label2;
	GtkWidget *tooltip, *spinner;
	GtkWidget *vpane1, *hpane1, *hpane2;
	GtkWidget *scrolled_window;
	GtkWidget *refresh, *awf, *icon1, *icon2, *icon3;
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkWidget *view;
	GtkListStore *store;
	GtkTreeIter iter;

	gtk_init (&argc, &argv);

	/* load themes available at system level */

	list_system_theme = awf_load_theme ("/usr/share/themes");

	/* load themes available at user level */

	directory = g_build_path ("/", g_getenv ("HOME"), ".themes", NULL);
	list_user_theme = awf_load_theme (directory);
	g_free (directory);

	/* exclude themes at system level also available at user level */

	g_slist_foreach (list_user_theme, awf_exclude_theme, NULL);

	/* window layout */

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	awf_window_set_title ();

	vbox_window = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox_window);

	menubar = awf_build_menu (window);
	gtk_box_pack_start (GTK_BOX (vbox_window), menubar, FALSE, FALSE, 0);

	toolbar = gtk_toolbar_new ();
	gtk_box_pack_start (GTK_BOX (vbox_window), toolbar, FALSE, FALSE, 0);
#if GTK_CHECK_VERSION(3,0,0)
	gtk_style_context_add_class (gtk_widget_get_style_context (toolbar), "primary-toolbar");
#endif

	vbox_widget = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_window), vbox_widget);

	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_widget), hbox1);

	hseparator1 = gtk_hseparator_new ();
	gtk_container_add (GTK_CONTAINER (vbox_widget), hseparator1);

	vpane1 = gtk_vpaned_new ();
	gtk_container_add (GTK_CONTAINER (vbox_widget), vpane1);

	hpane1 = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (vpane1), hpane1);

	hpane2 = gtk_hpaned_new ();
	gtk_paned_add2 (GTK_PANED (vpane1), hpane2);

	hbox_frame1 = gtk_hbox_new (TRUE, 3);
	hbox_frame2 = gtk_hbox_new (TRUE, 3);
	gtk_widget_set_size_request (hbox_frame1, -1, 70);
	gtk_widget_set_size_request (hbox_frame2, -1, 70);
	gtk_container_set_border_width(GTK_CONTAINER (hbox_frame1), 10);
	gtk_container_set_border_width(GTK_CONTAINER (hbox_frame2), 10);

	hbox_notebook1 = gtk_hbox_new (TRUE, 3);
	hbox_notebook2 = gtk_hbox_new (TRUE, 3);
	gtk_container_set_border_width(GTK_CONTAINER (hbox_notebook1), 10);
	gtk_container_set_border_width(GTK_CONTAINER (hbox_notebook2), 10);

	gtk_paned_add1 (GTK_PANED (hpane1), hbox_frame1);
	gtk_paned_add2 (GTK_PANED (hpane1), hbox_frame2);

	gtk_paned_add1 (GTK_PANED (hpane2), hbox_notebook1);
	gtk_paned_add2 (GTK_PANED (hpane2), hbox_notebook2);

	vbox_combo_entry_spin_check_radio_button = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), 10);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox_combo_entry_spin_check_radio_button);

	vseparator1 = gtk_vseparator_new ();
	gtk_container_add (GTK_CONTAINER (hbox1), vseparator1);

	vbox_other_button = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER (vbox_other_button), 10);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox_other_button);

	vseparator2 = gtk_vseparator_new ();
	gtk_container_add (GTK_CONTAINER (hbox1), vseparator2);

	vbox_progressbar_scale = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER (vbox_progressbar_scale), 10);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox_progressbar_scale);

	vseparator3 = gtk_vseparator_new ();
	gtk_container_add (GTK_CONTAINER (hbox1), vseparator3);

	vbox_label_treeview = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER (vbox_label_treeview), 10);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox_label_treeview);

	vbox_combo_entry = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), vbox_combo_entry);

	hbox_spin_button = gtk_hbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), hbox_spin_button);

	hbox_check_radio_button = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_combo_entry_spin_check_radio_button), hbox_check_radio_button);

	vbox_check_button = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (hbox_check_radio_button), vbox_check_button);

	vbox_radio_button = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (hbox_check_radio_button), vbox_radio_button);

	vbox_progressbar = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), vbox_progressbar);

	hbox_progressbar = gtk_hbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), hbox_progressbar);

	vbox_scale = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), vbox_scale);

	hbox_scale = gtk_hbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (vbox_progressbar_scale), hbox_scale);

	/* toolbar */

	awf = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_PREFERENCES));
	refresh = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH));
	icon1 = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_FIRST));
	icon2 = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_LAST));
	icon3 = GTK_WIDGET (gtk_toggle_tool_button_new_from_stock (GTK_STOCK_ADD));

	gtk_widget_set_tooltip_text (refresh, "Refresh theme");
#if GTK_CHECK_VERSION(3,0,0)
	gtk_widget_set_tooltip_text (awf, "Run awf for gtk2");
#else
	gtk_widget_set_tooltip_text (awf, "Run awf for gtk3");
#endif
	gtk_widget_set_sensitive(icon2, FALSE);
	gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON (icon3), TRUE);
	g_signal_connect (G_OBJECT(refresh), "clicked", G_CALLBACK (awf_refresh_theme), NULL);
	g_signal_connect (G_OBJECT(awf), "clicked", G_CALLBACK (awf_run_me), NULL);

	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (awf), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (refresh), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new (), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (icon1), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (icon2), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new (), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (icon3), -1);

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

	button19 = gtk_spin_button_new_with_range (1, 10, 1);
	button20 = gtk_spin_button_new_with_range (1, 10, 1);
	gtk_widget_set_sensitive (button20, FALSE);

	gtk_box_pack_start (GTK_BOX (hbox_spin_button), button19, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_spin_button), button20, FALSE, FALSE, 0);

	/* Check buttons */

	button11 = gtk_check_button_new_with_label ("Check button1");
	button12 = gtk_check_button_new_with_label ("Check button2");
	button13 = gtk_check_button_new_with_label ("Check button3");
	button14 = gtk_check_button_new_with_label ("Check button4");

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button12), TRUE);
	gtk_widget_set_sensitive (button13, FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button14), TRUE);
	gtk_widget_set_sensitive (button14, FALSE);

	gtk_box_pack_start (GTK_BOX (vbox_check_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button11, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button12, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button13, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_check_button), button14, FALSE, FALSE, 0);

	/* radio buttons */

	radio1 = gtk_radio_button_new (NULL);
	radio2 = gtk_radio_button_new (NULL);

	button15 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Radio button1");
	button16 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Radio button2");
	button17 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio2), "Radio button3");
	button18 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio2), "Radio button4");

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button16), TRUE);
	gtk_widget_set_sensitive (button17, FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button18), TRUE);
	gtk_widget_set_sensitive (button18, FALSE);

	gtk_box_pack_start (GTK_BOX (vbox_radio_button), EMPTY, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button15, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button16, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button17, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_radio_button), button18, FALSE, FALSE, 0);

	/* other buttons */

	button1 = gtk_button_new_with_label ("Button1");
	button2 = gtk_button_new_with_label ("Button2");
	gtk_widget_set_sensitive (button2, FALSE);

	button3 = gtk_toggle_button_new_with_label ("Toggle button1");
	button4 = gtk_toggle_button_new_with_label ("Toggle button2");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button3), TRUE);
	gtk_widget_set_sensitive (button4, FALSE);

	button5 = gtk_link_button_new_with_label ("http://www.gtk.org/", "Link button1");
	button6 = gtk_link_button_new_with_label ("http://www.gtk.org/", "Link button2");

	menu1 = gtk_menu_new ();
	menu2 = gtk_menu_new ();

	gtk_menu_shell_append (GTK_MENU_SHELL (menu1), gtk_menu_item_new_with_label ("Option menu1"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu1), gtk_menu_item_new_with_label ("Item2"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu2), gtk_menu_item_new_with_label ("Option menu2"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu2), gtk_menu_item_new_with_label ("Item2"));

#if GTK_CHECK_VERSION(3,0,0)
	button7 = gtk_switch_new ();
	button8 = gtk_switch_new ();

	gtk_switch_set_active (GTK_SWITCH (button7), TRUE);
	gtk_widget_set_sensitive (button8, FALSE);
#else
	button7 = gtk_option_menu_new ();
	button8 = gtk_option_menu_new ();

	gtk_option_menu_set_menu (GTK_OPTION_MENU (button7), menu1);
	gtk_option_menu_set_menu (GTK_OPTION_MENU (button8), menu2);
	gtk_widget_set_sensitive (button8, FALSE);
#endif

	button9 = gtk_scale_button_new (GTK_ICON_SIZE_BUTTON, 1, 10, 1, NULL);
	button10 = gtk_scale_button_new (GTK_ICON_SIZE_BUTTON, 1, 10, 1, NULL);
	gtk_widget_set_sensitive (button10, FALSE);

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

	/* progress bars */

	progressbar1 = gtk_progress_bar_new ();
	progressbar2 = gtk_progress_bar_new ();
	progressbar3 = gtk_progress_bar_new ();
	progressbar4 = gtk_progress_bar_new ();

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), 0.5);

#if GTK_CHECK_VERSION(3,0,0)
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar1), GTK_ORIENTATION_HORIZONTAL);
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar2), GTK_ORIENTATION_HORIZONTAL);
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar3), GTK_ORIENTATION_VERTICAL);
	gtk_orientable_set_orientation (GTK_ORIENTABLE (progressbar4), GTK_ORIENTATION_VERTICAL);

	gtk_progress_bar_set_inverted (GTK_PROGRESS_BAR (progressbar2), TRUE);
	gtk_progress_bar_set_inverted (GTK_PROGRESS_BAR (progressbar4), TRUE);
#else
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar1), GTK_PROGRESS_LEFT_TO_RIGHT);
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar2), GTK_PROGRESS_RIGHT_TO_LEFT);
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar3), GTK_PROGRESS_BOTTOM_TO_TOP);
	gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (progressbar4), GTK_PROGRESS_TOP_TO_BOTTOM);
#endif

	gtk_box_pack_start (GTK_BOX (vbox_progressbar), progressbar1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_progressbar), progressbar2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_progressbar), progressbar3, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_progressbar), progressbar4, FALSE, FALSE, 0);

	/* scales */

	scale1 = gtk_vscale_new_with_range (0, 100, 1);
	scale2 = gtk_vscale_new_with_range (0, 100, 1);
	scale3 = gtk_hscale_new_with_range (0, 100, 1);
	scale4 = gtk_hscale_new_with_range (0, 100, 1);

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

	harmony = gtk_check_button_new_with_label ("Move in harmony");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (harmony), TRUE);
	gtk_box_pack_start (GTK_BOX (vbox_progressbar_scale), harmony, FALSE, FALSE, 0);

	/* treeview */

	view = gtk_tree_view_new ();

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Column1", renderer, "text", COLUMN1, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Column2", renderer, "text", COLUMN2, NULL);

	store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Cell1.1", COLUMN2, "Cell1.2", -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Cell2.1", COLUMN2, "Cell2.2", -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COLUMN1, "Cell3.1", COLUMN2, "Cell3.2", -1);

	model = GTK_TREE_MODEL (store);
  	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (scrolled_window, 200, -1);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (scrolled_window), view);

	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), scrolled_window, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_label_treeview), EMPTY, FALSE, FALSE, 0);

	/* labels, tooltip and spinner */

	label1 = gtk_label_new ("Label1");
	label2 = gtk_label_new ("Label2");
	gtk_widget_set_sensitive (label2, FALSE);

	tooltip = gtk_label_new ("Tooltip");
	gtk_widget_set_tooltip_text (tooltip, "Enjoy a widget factory!");

	spinner = gtk_spinner_new ();
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

	gtk_notebook_append_page (GTK_NOTEBOOK(notebook1), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook1), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook1), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook1), GTK_POS_TOP);

	gtk_notebook_append_page (GTK_NOTEBOOK(notebook2), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook2), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook2), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook2), GTK_POS_BOTTOM);

	gtk_notebook_append_page (GTK_NOTEBOOK(notebook3), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook3), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook3), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook3), GTK_POS_LEFT);

	gtk_notebook_append_page (GTK_NOTEBOOK(notebook4), gtk_label_new (""), gtk_label_new ("tab1"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook4), gtk_label_new (""), gtk_label_new ("tab2"));
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook4), gtk_label_new (""), gtk_label_new ("tab3"));
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook4), GTK_POS_RIGHT);

	gtk_box_pack_start (GTK_BOX (hbox_notebook1), notebook1, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_notebook1), notebook2, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_notebook2), notebook3, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_notebook2), notebook4, TRUE, TRUE, 0);

	/* status bar */

	gtk_box_pack_start (GTK_BOX (vbox_window), gtk_statusbar_new (), FALSE, FALSE, 0);

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
			g_printf("Usage:\n");
#if GTK_CHECK_VERSION(3,0,0)
			g_printf("\t%s\t\t\t%s\n", "awf-gtk3", "Run a widget factory");
			g_printf("\t%s %s\t\t%s\n", "awf-gtk3", "<theme>", "Run a widget factory with the specified theme");
#else
			g_printf("\t%s\t\t\t%s\n", "awf-gtk2", "Run a widget factory");
			g_printf("\t%s %s\t\t%s\n", "awf-gtk2", "<theme>", "Run a widget factory with the specified theme");
#endif
			break;
	}

	return 0;
}

static GSList*
awf_load_theme (gchar *directory)
{
	GSList *list = NULL;

	g_return_val_if_fail(directory != NULL, NULL);

	if (g_file_test (directory, G_FILE_TEST_EXISTS)) {

		GError *error = NULL;
		GDir *dir = g_dir_open (directory, 0, &error);

		if (dir) {
			gchar *theme = g_strdup (g_dir_read_name (dir));

			while (theme) {
				gchar *theme_path = g_build_path ("/", directory, g_strstrip (theme), NULL);

				if (g_file_test (theme_path, G_FILE_TEST_IS_DIR)) {
#if GTK_CHECK_VERSION(3,0,0)
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
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", (const gchar*)theme, "gtkrc:0");
	awf_window_set_title ();
}

static void
awf_refresh_theme (GtkWidget *widget, gpointer unused)
{
	gchar *theme;

	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &theme, NULL);
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", "Raleigh", "gtkrc:0");
	g_usleep (G_USEC_PER_SEC);
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", (const gchar*)theme, "gtkrc:0");
	g_free (theme);
}

static void
awf_window_set_title (void)
{
	gchar *title, *theme;

	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &theme, NULL);

#if GTK_CHECK_VERSION(3,0,0)
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
	GtkWidget *menu;
	GtkWidget *menuitem;
	GtkWidget *menuimage;
	guint accelerator_key;
	GdkModifierType accelerator_mods;
	gint i;

	struct _MenuInfo {
		gint type;
		gchar mnemonic[32];
		gchar accelerator[32];
		gchar *image;
		void (*callback)(gpointer, guint, GtkWidget*);
	} info[9] = {
		{ 0, "_System theme", "<Control>s", NULL		  , NULL },
		{ 1, ""			 , ""		  , NULL		  , NULL },
		{ 0, "_User theme"  , "<Control>u", NULL		  , NULL },
		{ 2, ""			 , ""		  , NULL		  , NULL },
		{ 0, "_Test menu"   , "<Control>t", NULL		  , NULL },
		{ 3, "_Item1"	   , "<Control>i", GTK_STOCK_OPEN, NULL },
		{ 3, "Item2"		, ""		  , GTK_STOCK_SAVE, NULL },
		{ 4, ""			 , ""		  , NULL		  , NULL },
		{ 3, "Item3"		, ""		  , NULL		  , NULL }
	};

	ag = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (widget), ag);

	menubar = gtk_menu_bar_new ();

	for (i = 0; i < 9; i++) {
		switch (info[i].type) {
			case 0:
				menu = gtk_menu_new ();
				gtk_widget_show (menu);

				menuitem = gtk_menu_item_new_with_mnemonic (info[i].mnemonic);
				gtk_menu_item_set_submenu (GTK_MENU_ITEM(menuitem), menu);

				gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);
				gtk_widget_show (menuitem);
				break;

			case 1:
				g_slist_foreach (list_system_theme, awf_add_menu_item, menu);
				break;

			case 2:
				g_slist_foreach (list_user_theme, awf_add_menu_item, menu);
				break;

			case 3:
				if (info[i].image) {
					menuitem = gtk_image_menu_item_new_with_mnemonic (info[i].mnemonic);
					menuimage = gtk_image_new_from_stock (info[i].image, GTK_ICON_SIZE_MENU);
					gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menuitem), menuimage);
				} else
					menuitem = gtk_menu_item_new_with_mnemonic (info[i].mnemonic);

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);

				gtk_accelerator_parse (info[i].accelerator, &accelerator_key, &accelerator_mods);

				if (accelerator_key && accelerator_mods)
					gtk_widget_add_accelerator(menuitem, "activate", ag,
						accelerator_key, accelerator_mods, GTK_ACCEL_VISIBLE);

				if (info[i].callback)
					g_signal_connect_swapped (G_OBJECT(menuitem), "activate",
											  G_CALLBACK(info[i].callback), widget);
				break;

			case 4:
				menuitem = gtk_separator_menu_item_new ();

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);
		}
	}

	return menubar;
}

static void
awf_add_menu_item (gpointer theme, gpointer menu)
{
	GtkWidget *menuitem = gtk_menu_item_new_with_mnemonic ((const gchar*)theme);

	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	gtk_widget_show (menuitem);

	g_signal_connect_swapped (G_OBJECT(menuitem), "activate", G_CALLBACK(awf_set_theme), theme);
}

static void
awf_run_me (GtkWidget *widget, gpointer unused)
{
	gchar *awf;

#if GTK_CHECK_VERSION(3,0,0)
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
#if GTK_CHECK_VERSION(3,0,0)
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
	g_setenv("DISPLAY", display, TRUE);
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
}
