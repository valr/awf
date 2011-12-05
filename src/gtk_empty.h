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

#ifndef __GTK_EMPTY_H__
#define __GTK_EMPTY_H__

#include <gtk/gtk.h>

#define GTK_TYPE_EMPTY			(gtk_empty_get_type ())
#define GTK_EMPTY(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_EMPTY, GtkEmpty))
#define GTK_EMPTY_CLASS(obj)	(G_TYPE_CHECK_CLASS_CAST ((obj), GTK_EMPTY,  GtkEmptyClass))
#define GTK_IS_EMPTY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_EMPTY))
#define GTK_IS_EMPTY_CLASS(obj)	(G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_EMPTY))
#define GTK_EMPTY_GET_CLASS		(G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_EMPTY, GtkEmptyClass))

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

GtkWidget* gtk_empty_new (void);

#endif /* __GTK_EMPTY_H__ */
