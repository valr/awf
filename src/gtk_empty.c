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

#include "gtk_empty.h"

/*
 * declarations
 */

G_DEFINE_TYPE (GtkEmpty, gtk_empty, GTK_TYPE_DRAWING_AREA);

static void gtk_empty_class_init (GtkEmptyClass *class);
static void gtk_empty_init (GtkEmpty *empty);
#if GTK_CHECK_VERSION(3,0,0)
static void gtk_empty_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width);
static void gtk_empty_get_preferred_height (GtkWidget *widget, gint *minimal_height, gint *natural_height);
#else
static void gtk_empty_size_request (GtkWidget *widget, GtkRequisition *requisition);
#endif

/*
 * definitions
 */

GtkWidget*
gtk_empty_new (void)
{
	return g_object_new (GTK_TYPE_EMPTY, NULL);
}

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
