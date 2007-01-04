/* 
 * Proview   $Id: flow_browwidget_gtk.h,v 1.1 2007-01-04 07:56:44 claes Exp $
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

#ifndef flow_browwidget_gtk_h
#define flow_browwidget_gtk_h

#include <stdlib.h>
#include "glow_std.h"

#include "flow.h"
#include "flow_ctx.h"
#include "flow_browctx.h"

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>

#if defined __cplusplus
extern "C" {
#endif

#define BROWWIDGETGTK_TYPE (browwidgetgtk_get_type())
#define BROWWIDGETGTK(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), BROWWIDGETGTK_TYPE, BrowWidgetGtk))
#define BROWWIDGETGTK_CLASS(obj) (G_TYPE_CHECK_CLASS_CAST((obj), BROWWIDGETGTK, BrowWidgetGtkClass))
#define IS_BROWWIDGETGTK(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), BROWWIDGETGTK_TYPE))
#define IS_BROWWIDGETGTK_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE((obj), BROWWIDGETGTK_TYPE))
#define BROWWIDGETGTK_GET_CLASS (G_TYPE_INSTANCE_GET_CLASS((obj), BROWWIDGETGTK, BrowWidgetGtkClass))

GType browwidgetgtk_get_type(void);

GtkWidget *browwidgetgtk_new(
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data);
GtkWidget *scrolledbrowwidgetgtk_new(
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data, GtkWidget **browwidget);

GtkWidget *brownavwidgetgtk_new( GtkWidget *main_brow);

void browwidgetgtk_get_ctx( GtkWidget *w, void **ctx);
void browwidgetgtk_modify_ctx( GtkWidget *w, void *ctx);

#if defined __cplusplus
}
#endif
#endif
