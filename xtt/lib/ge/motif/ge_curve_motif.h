/* 
 * Proview   $Id: ge_curve_motif.h,v 1.1 2007-01-04 08:22:16 claes Exp $
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

#ifndef ge_curve_motif_h
#define ge_curve_motif_h

/* ge_curve_motif.h -- Curve widget */

#ifndef ge_curve_h
#include "ge_curve.h"
#endif

class GeCurveMotif : public GeCurve {
  public:
    GeCurveMotif( void *gc_parent_ctx, Widget parent_widget, char *curve_name,
		  char *filename, GeCurveData *curve_data, int pos_right);
    Widget	parent_wid;
    Widget	grow_widget;
    Widget	curve_widget;
    Widget	axisform_widget;
    Widget	nameform_widget;
    Widget	curveform_widget;
    Widget	pane_widget;
    Widget	toplevel;
    Widget      growcurve_main_widget;
    Widget      growaxis_main_widget;
    Widget      grownames_main_widget;
    Widget      nav_widget;
    Widget      minmax_widget;
    Widget      minmax_textmin_widget;
    Widget      minmax_textmax_widget;

    void pop();
    void write_title( char *str);
    void resize();
    void open_minmax( int idx);
    void axis_set_width( int width);

    ~GeCurveMotif();

    static void activate_exit( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_configure( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_print( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_zoomin( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_zoomout( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_zoomreset( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_background( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_showname( Widget w, GeCurve *curve, XmToggleButtonCallbackStruct *data);
    static void activate_filledcurves( Widget w, GeCurve *curve, XmToggleButtonCallbackStruct *data);
    static void activate_xlimits( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void create_pane( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void create_growform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void create_nameform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void create_curveform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void create_minmax_textmin( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void create_minmax_textmax( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
    static void activate_minmax_ok( Widget w, GeCurve *curve, XmAnyCallbackStruct *data);
};

/*@}*/
#endif
















