/* 
 * Proview   $Id: glow_growwidget_gtk.h,v 1.1 2007-01-04 08:07:43 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#include <stdlib.h>
#include "glow_std.h"

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_growctx.h"

#define GROWWIDGETGTK_TYPE (growwidgetgtk_get_type())
#define GROWWIDGETGTK(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GROWWIDGETGTK_TYPE, GrowWidgetGtk))
#define GROWWIDGETGTK_CLASS(obj) (G_TYPE_CHECK_CLASS_CAST((obj), GROWWIDGETGTK, GrowWidgetGtkClass))
#define IS_GROWWIDGETGTK(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GROWWIDGETGTK_TYPE))
#define IS_GROWWIDGETGTK_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE((obj), GROWWIDGETGTK_TYPE))
#define GROWWIDGETGTK_GET_CLASS (G_TYPE_INSTANCE_GET_CLASS((obj), GROWWIDGETGTK, GrowWidgetGtkClass))

GType growwidgetgtk_get_type(void);

GtkWidget *growwidgetgtk_new(
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data);
GtkWidget *scrolledgrowwidgetgtk_new(
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, GtkWidget **growwidget);

GtkWidget *grownavwidgetgtk_new( GtkWidget *main_grow);
