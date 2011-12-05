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
 *  along with awf.  If not, see <http://www.gnu.org/licenses/>.
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
 * awf has been made in a couple of hours, so design and code is simple
 * and sometimes dirty.
 *
 * Widgets difference between gtk2 and gtk3:
 * - gtk2: GtkOptionMenu exists. GtkSwitch doesn't exist yet.
 * - gtk3: GtkOptionMenu doesn't exist anymore. GtkSwitch exists.
 */

#include <gtk/gtk.h>

/**********************************
 * Empty custom widget definition *
 **********************************/

typedef struct _GtkEmpty		GtkEmpty;
typedef struct _GtkEmptyClass	GtkEmptyClass;

struct _GtkEmpty
{
	GtkDrawingArea parent;
};

struct _GtkEmptyClass
{
	GtkDrawingAreaClass parent_class;
};

G_DEFINE_TYPE (GtkEmpty, gtk_empty, GTK_TYPE_DRAWING_AREA);

#define GTK_TYPE_EMPTY			(gtk_empty_get_type ())
#define GTK_EMPTY(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_EMPTY, GtkEmpty))
#define GTK_EMPTY_CLASS(obj)	(G_TYPE_CHECK_CLASS_CAST ((obj), GTK_EMPTY,  GtkEmptyClass))
#define GTK_IS_EMPTY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_EMPTY))
#define GTK_IS_EMPTY_CLASS(obj)	(G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_EMPTY))
#define GTK_EMPTY_GET_CLASS		(G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_EMPTY, GtkEmptyClass))

#if GTK_CHECK_VERSION(3,0,0)
static void
gtk_empty_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width)
{
	*minimal_width = *natural_width = 10;
}

static void
gtk_empty_get_preferred_height (GtkWidget *widget, gint *minimal_height, gint *natural_height)
{
	*minimal_height = *natural_height = 10;
}
#else
static void
gtk_empty_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	requisition->width = 10;
	requisition->height = 10;
}
#endif

static void
gtk_empty_class_init (GtkEmptyClass *class)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

#if GTK_CHECK_VERSION(3,0,0)
	widget_class->get_preferred_width = gtk_empty_get_preferred_width;
	widget_class->get_preferred_height = gtk_empty_get_preferred_height;
#else
	widget_class->size_request = gtk_empty_size_request;
#endif
}

static void
gtk_empty_init (GtkEmpty *empty)
{
}

static GtkWidget*
gtk_empty_new (void)
{
	return g_object_new (GTK_TYPE_EMPTY, NULL);
}

/*******************************
 * A widget factory definition *
 *******************************/

/* main widget */

GtkWidget *window;

/* menu and toolbar */

static GtkWidget* create_menu (GtkWidget *window);
static void add_theme (gchar *directory_name, GtkWidget *menu);
static void set_theme (gpointer theme, guint callback_action, GtkWidget *widget);
static void refresh_theme (GtkWidget *widget, gpointer item);
static void run_awf (GtkWidget *widget, gpointer item);

/* scales and progressbars */

GtkWidget *progressbar1, *progressbar2, *progressbar3, *progressbar4;
GtkWidget *scale1, *scale2, *scale3, *scale4, *harmony;

static void on_scale_value_changed (GtkRange *range, gpointer user_data);

/* empty widget used as separator */

#define EMPTY (gtk_empty_new ())

/* enum for the treeview */

enum
{
	COLUMN1 = 0,
	COLUMN2,
	NUM_COLS
} ;

/* run baby, run! */

int main (int argc, char **argv)
{
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
	GtkWidget *refresh, *awf, *icon1, *icon2, *toggle;
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkWidget *view;
	GtkListStore *store;
	GtkTreeIter iter;

    gtk_init (&argc, &argv);

	/* window */

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
#if GTK_CHECK_VERSION(3,0,0)
	gtk_window_set_title (GTK_WINDOW (window), "A widget factory - gtk3");
#else
	gtk_window_set_title (GTK_WINDOW (window), "A widget factory - gtk2");
#endif
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	vbox_window = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox_window);

	menubar = create_menu (window);
	gtk_container_add (GTK_CONTAINER (vbox_window), menubar);

	toolbar = gtk_toolbar_new();
	gtk_container_add (GTK_CONTAINER (vbox_window), toolbar);

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

	refresh = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH));
	awf = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_PREFERENCES));
	icon1 = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_FIRST));
	icon2 = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_LAST));
	toggle = GTK_WIDGET (gtk_toggle_tool_button_new_from_stock (GTK_STOCK_ADD));

	gtk_widget_set_tooltip_text (refresh, "Refresh theme");
#if GTK_CHECK_VERSION(3,0,0)
	gtk_widget_set_tooltip_text (awf, "Run awf for gtk2");
#else
	gtk_widget_set_tooltip_text (awf, "Run awf for gtk3");
#endif
	gtk_widget_set_sensitive(icon2, FALSE);
	gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON (toggle), TRUE);
	g_signal_connect (G_OBJECT(refresh), "clicked", G_CALLBACK (refresh_theme), NULL);
	g_signal_connect (G_OBJECT(awf), "clicked", G_CALLBACK (run_awf), NULL);

	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (awf), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (refresh), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new (), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (icon1), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (icon2), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new (), -1);
	gtk_toolbar_insert (GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM (toggle), -1);

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
	button7 = gtk_switch_new  ();
	button8 = gtk_switch_new  ();

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

	g_signal_connect ((gpointer)scale1, "value_changed", G_CALLBACK (on_scale_value_changed), NULL);
	g_signal_connect ((gpointer)scale2, "value_changed", G_CALLBACK (on_scale_value_changed), NULL);
	g_signal_connect ((gpointer)scale3, "value_changed", G_CALLBACK (on_scale_value_changed), NULL);
	g_signal_connect ((gpointer)scale4, "value_changed", G_CALLBACK (on_scale_value_changed), NULL);

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
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1,      
                                                 "Column1", renderer,
                                                 "text", COLUMN1, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1,      
                                                 "Column2", renderer,
                                                 "text", COLUMN2, NULL);

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

	gtk_widget_show_all (window);
	gtk_main ();

    return 0;
}

static GtkWidget*
create_menu (GtkWidget *window)
{
	GtkAccelGroup *ag;
	GtkWidget *menubar;
	GtkWidget *menu;
	GtkWidget *menuitem;
	GtkWidget *menuimage;
	guint accelerator_key;
	GdkModifierType accelerator_mods;
	gchar *directory;
	gint i;

	struct _MenuInfo
	{
		gint type;
		gchar mnemonic[32];
		gchar accelerator[32];
		gchar *image;
		void (*callback)(gpointer, guint, GtkWidget*);
	} info[9] = {
	{ 0, "_System theme", "<Control>s", NULL             , NULL             },
	{ 2, ""             , ""          , NULL             , NULL             },
	{ 0, "_User theme"  , "<Control>u", NULL             , NULL             },
	{ 3, ""             , ""          , NULL             , NULL             },
	{ 0, "_Test menu"   , "<Control>t", NULL             , NULL             },
	{ 1, "_Item1"       , "<Control>i", GTK_STOCK_OPEN   , NULL             },
	{ 1, "Item2"        , ""          , GTK_STOCK_SAVE   , NULL             },
	{ 4, ""             , ""          , NULL             , NULL             },
	{ 1, "Item3"        , ""          , NULL             , NULL             }};

	ag = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (window), ag);

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
										G_CALLBACK(info[i].callback), window);
				break;

			case 2:
				directory = g_strdup ("/usr/share/themes");
				add_theme (directory, menu);
				g_free (directory);
				break;

			case 3:
		        directory = g_build_path ("/", g_getenv ("HOME"), ".themes", NULL);
				add_theme (directory, menu);
				g_free (directory);
				break;

			case 4:
				menuitem = gtk_separator_menu_item_new ();

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);
		}
	}

	return menubar;
}

static void add_theme (gchar *directory_name, GtkWidget *menu)
{
    g_return_if_fail (directory_name != NULL);

	if (g_file_test (directory_name, G_FILE_TEST_EXISTS))
	{
		GError *error = NULL;
		GDir *directory = g_dir_open (directory_name, 0, &error);

		if (directory) {
			const gchar *theme_name = g_strdup (g_dir_read_name (directory));

			while (theme_name) {
				gchar *theme_path_name = g_build_path ("/", directory_name, theme_name, NULL);

				if (g_file_test (theme_path_name, G_FILE_TEST_IS_DIR)) {
#if GTK_CHECK_VERSION(3,0,0)
					gchar *theme_subpath_name = g_build_path ("/", theme_path_name, "gtk-3.0", NULL);
#else
					gchar *theme_subpath_name = g_build_path ("/", theme_path_name, "gtk-2.0", NULL);
#endif
					if (g_file_test (theme_subpath_name, G_FILE_TEST_IS_DIR)) {
						GtkWidget *menuitem = gtk_menu_item_new_with_mnemonic (theme_name);
						gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
						gtk_widget_show (menuitem);

						g_signal_connect_swapped (G_OBJECT(menuitem), "activate",
												  G_CALLBACK(set_theme), (gpointer)theme_name);
					}

					g_free (theme_subpath_name);
				}

				g_free (theme_path_name);

				theme_name = g_strdup (g_dir_read_name (directory));
			}

			g_dir_close (directory);
		}

		if (error) {
			g_fprintf (stderr, "unable to open directory: %s (%s)\n",
					   directory_name,
					   error->message);

			g_error_free (error); error = NULL;
		}
	}
}

static void
set_theme (gpointer theme, guint callback_action, GtkWidget *widget)
{
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", 
									 (const gchar*)theme, "gtkrc:0");
}

static void
refresh_theme (GtkWidget *widget, gpointer item)
{
	gchar *theme;

	g_object_get (gtk_settings_get_default (), "gtk-theme-name", &theme, NULL);
	gtk_settings_set_string_property (gtk_settings_get_default (), "gtk-theme-name", 
									 (const gchar*)theme, "gtkrc:0");
}

static void
run_awf_set_environment (gpointer display)
{
	g_setenv("DISPLAY", display, TRUE);
}

static void
run_awf (GtkWidget *widget, gpointer item)
{
    GError *error = NULL;
    gchar **argv; gint argp;
    gchar *display;

#if GTK_CHECK_VERSION(3,0,0)
	gchar *awf = g_find_program_in_path ("awf-gtk2");
#else
	gchar *awf = g_find_program_in_path ("awf-gtk3");
#endif

	if (awf) {
		g_shell_parse_argv (awf, &argp, &argv, &error);

		if (error) {
			GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
									GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
									"Unable to parse command: %s (%s)", awf, error->message);

			gtk_dialog_run (GTK_DIALOG (dialog));
			gtk_widget_destroy (dialog);

			g_free (awf); 
			g_error_free (error); error = NULL;
			return;
		}

		display = gdk_screen_make_display_name (gtk_window_get_screen (GTK_WINDOW (window)));

		g_spawn_async (NULL, argv, NULL,
					   G_SPAWN_SEARCH_PATH | G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_STDERR_TO_DEV_NULL,
					   run_awf_set_environment, display, NULL, &error);

		if (error) {
			GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
									GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
									"Unable to spawn command: %s (%s)", awf, error->message);

			gtk_dialog_run (GTK_DIALOG (dialog));
			gtk_widget_destroy (dialog);

			g_error_free (error); error = NULL;
		}

		g_free(display);
		g_strfreev(argv);
		g_free (awf);
	} else {
		GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
#if GTK_CHECK_VERSION(3,0,0)
								"Cannot find awf-gtk2 in path");
#else
								"Cannot find awf-gtk3 in path");
#endif

		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
	}
}

static void
on_scale_value_changed (GtkRange *range, gpointer user_data)
{
	gdouble value;
	int i;

	value = gtk_range_get_value (range);

	if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (harmony)))
		return;

	if (scale1 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale1), value);
	if (scale2 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale2), value);
	if (scale3 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale3), value);
	if (scale4 != (GtkWidget*)range) gtk_range_set_value (GTK_RANGE (scale4), value);

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar1), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar2), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar3), value / 100.0);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar4), value / 100.0);
}
