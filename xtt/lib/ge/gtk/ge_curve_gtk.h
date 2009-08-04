/* 
 * Proview   $Id: ge_curve_gtk.h,v 1.3 2008-07-17 11:21:25 claes Exp $
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

#ifndef ge_curve_gtk_h
#define ge_curve_gtk_h

/* ge_curve_gtk.h -- Curve widget */

#ifndef ge_curve_h
#include "ge_curve.h"
#endif

#include "co_wow_gtk.h"

class GeCurveGtk : public GeCurve {
  public:
    GeCurveGtk( void *gc_parent_ctx, GtkWidget *parent_widget, char *curve_name,
		char *filename, GeCurveData *curve_data, int pos_right);
    GtkWidget	*parent_wid;
    GtkWidget	*grow_widget;
    GtkWidget	*curve_widget;
    GtkWidget	*axisform_widget;
    GtkWidget	*nameform_widget;
    GtkWidget	*curveform_widget;
    GtkWidget	*pane_widget;
    GtkWidget	*toplevel;
    GtkWidget   *growcurve_main_widget;
    GtkWidget   *growaxis_main_widget;
    GtkWidget   *grownames_main_widget;
    GtkWidget   *nav_widget;
    GtkWidget   *minmax_widget;
    GtkWidget   *minmax_textmin_widget;
    GtkWidget   *minmax_textmax_widget;
    GtkWidget   *tools_higher_res;
    GtkWidget   *tools_lower_res;
    CoWowFocusTimerGtk focustimer;

    void pop();
    void write_title( char *str);
    void resize();
    void open_minmax( int idx);
    void axis_set_width( int width);
    void create_minmax_dialog();
    void set_inputfocus();
    void enable_resolution_buttons();

    ~GeCurveGtk();

    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_configure( GtkWidget *w, gpointer data);
    static void activate_print( GtkWidget *w, gpointer data);
    static void activate_zoomin( GtkWidget *w, gpointer data);
    static void activate_zoomout( GtkWidget *w, gpointer data);
    static void activate_zoomreset( GtkWidget *w, gpointer data);
    static void activate_page_left( GtkWidget *w, gpointer data);
    static void activate_page_right( GtkWidget *w, gpointer data);
    static void activate_scroll_left( GtkWidget *w, gpointer data);
    static void activate_scroll_right( GtkWidget *w, gpointer data);
    static void activate_higher_res( GtkWidget *w, gpointer data);
    static void activate_lower_res( GtkWidget *w, gpointer data);
    static void activate_background( GtkWidget *w, gpointer data);
    static void activate_showname( GtkWidget *w, gpointer data);
    static void activate_filledcurves( GtkWidget *w, gpointer data);
    static void activate_xlimits( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void activate_minmax_ok( GtkWidget *w, gpointer data);
    static void activate_minmax_cancel( GtkWidget *w, gpointer data);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
};

#endif
















