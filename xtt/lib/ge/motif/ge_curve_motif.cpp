/* 
 * Proview   $Id: ge_curve_motif.cpp,v 1.2 2008-10-31 12:51:33 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "rt_load.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "glow.h"
#include "glow_curvectx.h"
#include "glow_curvewidget_motif.h"
#include "glow_growctx.h"
#include "glow_growwidget_motif.h"
#include "glow_curveapi.h"
#include "glow_msg.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "flow_x.h"
#include "co_mrm_util.h"
#include "co_wow_motif.h"
#include "co_lng.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_curve_motif.h"
#include "ge_msg.h"

void GeCurveMotif::activate_exit( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->activate_exit();
}

void GeCurveMotif::activate_configure( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->activate_configure();
}

void GeCurveMotif::activate_print( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->activate_print();
}

void GeCurveMotif::activate_zoomin( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve_Zoom( curve->growcurve_ctx, 2.0);
}

void GeCurveMotif::activate_zoomout( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve_Zoom( curve->growcurve_ctx, 0.5);
}

void GeCurveMotif::activate_zoomreset( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
}

void GeCurveMotif::activate_background( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->activate_background();
}

void GeCurveMotif::activate_showname( Widget w, GeCurve *curve, XmToggleButtonCallbackStruct *data)
{
}

void GeCurveMotif::activate_filledcurves( Widget w, GeCurve *curve, XmToggleButtonCallbackStruct *data)
{
  curve->activate_filledcurves( data->set);
}

void GeCurveMotif::activate_xlimits( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  char value_str[80];

  sprintf( value_str, "%f",  curve->cd->min_value_axis[0]);
  XmTextSetString( ((GeCurveMotif *)curve)->minmax_textmin_widget, value_str);

  sprintf( value_str, "%f", curve->cd->max_value_axis[0]);
  XmTextSetString( ((GeCurveMotif *)curve)->minmax_textmax_widget, value_str);

  curve->minmax_idx = 0;
  XtManageChild( ((GeCurveMotif *)curve)->minmax_widget);
}

void GeCurveMotif::activate_help( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->activate_help();
}

void GeCurveMotif::create_pane( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  ((GeCurveMotif *)curve)->pane_widget = w;
}

void GeCurveMotif::create_growform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  ((GeCurveMotif *)curve)->axisform_widget = w;
}

void GeCurveMotif::create_nameform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  ((GeCurveMotif *)curve)->nameform_widget = w;
}

void GeCurveMotif::create_curveform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  ((GeCurveMotif *)curve)->curveform_widget = w;
}

void GeCurveMotif::create_minmax_textmin( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  ((GeCurveMotif *)curve)->minmax_textmin_widget = w;
}

void GeCurveMotif::create_minmax_textmax( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  ((GeCurveMotif *)curve)->minmax_textmax_widget = w;
}

void GeCurveMotif::activate_minmax_ok( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  char *value;
  double min_value, max_value;
  int nr;

  value = XmTextGetString( ((GeCurveMotif *)curve)->minmax_textmin_widget);
  nr = sscanf( value, "%lf", &min_value);
  if ( nr != 1)
    return;

  value = XmTextGetString( ((GeCurveMotif *)curve)->minmax_textmax_widget);
  nr = sscanf( value, "%lf", &max_value);
  if ( nr != 1)
    return;

  curve->activate_minmax_ok( min_value, max_value);
}

void GeCurveMotif::pop()
{
  flow_UnmapWidget( toplevel);
  flow_MapWidget( toplevel);
}

void GeCurveMotif::write_title( char *str)
{
  Arg args[1];

  XtSetArg(args[0],XmNtitle, str);
  XtSetValues( toplevel, args, 1);
}

void GeCurveMotif::resize()
{
  short height, width;
  Arg args[2];
  double zoom_y;

  XtSetArg(args[0],XmNheight, &height);
  XtGetValues(growaxis_main_widget, args, 1);

  curve_GetPreferedZoomY( growcurve_ctx, height, &zoom_y);
  grow_ZoomY( growaxis_ctx, zoom_y);
  grow_ZoomX( growaxis_ctx, zoom_y);

  width = short( zoom_y * axis_window_width);
  XtSetArg(args[0],XmNwidth,width+4);
  XtSetValues( axisform_widget, args, 1);
}

void GeCurveMotif::axis_set_width( int width)
{
  Arg args[2];

  short w = (short)width;

  XtSetArg(args[0],XmNwidth,w+4);
  XtSetValues( axisform_widget, args, 1);
}

void GeCurveMotif::open_minmax( int idx)
{
  char value_str[80];

  sprintf( value_str, "%f", cd->min_value_axis[idx]);
  XmTextSetString( minmax_textmin_widget, value_str);
  
  sprintf( value_str, "%f", cd->max_value_axis[idx]);
  XmTextSetString( minmax_textmax_widget, value_str);

  minmax_idx = idx;
  XtManageChild( minmax_widget);
}

GeCurveMotif::~GeCurveMotif()
{
  delete wow;
  XtDestroyWidget( toplevel);
}

GeCurveMotif::GeCurveMotif( void 	*gc_parent_ctx, 
			    Widget 	parent_widget,
			    char	*curve_name,
			    char  *filename,
			    GeCurveData *curve_data,
			    int   pos_right) :
  GeCurve( gc_parent_ctx, curve_name, filename, curve_data, pos_right)
{
  char		uid_filename[120] = {"xtt_curve.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	ge_curve_widget;
  char		name[] = "PwR GeCurve";

  static MrmRegisterArg	reglist[] = {
        {(char*) "gec_ctx", 0 },
	{(char*) "gec_activate_exit",(caddr_t)activate_exit },
	{(char*) "gec_activate_configure",(caddr_t)activate_configure },
	{(char*) "gec_activate_print",(caddr_t)activate_print },
	{(char*) "gec_activate_zoomin",(caddr_t)activate_zoomin },
	{(char*) "gec_activate_zoomout",(caddr_t)activate_zoomout },
	{(char*) "gec_activate_zoomreset",(caddr_t)activate_zoomreset },
	{(char*) "gec_activate_background",(caddr_t)activate_background },
	{(char*) "gec_activate_showname",(caddr_t)activate_showname },
	{(char*) "gec_activate_filledcurves",(caddr_t)activate_filledcurves },
	{(char*) "gec_activate_xlimits",(caddr_t)activate_xlimits },
	{(char*) "gec_activate_help",(caddr_t)activate_help },
	{(char*) "gec_create_pane",(caddr_t)create_pane },
	{(char*) "gec_create_growform",(caddr_t)create_growform },
	{(char*) "gec_create_nameform",(caddr_t)create_nameform },
	{(char*) "gec_create_curveform",(caddr_t)create_curveform },
	{(char*) "gec_create_minmax_textmin",(caddr_t)create_minmax_textmin },
	{(char*) "gec_create_minmax_textmax",(caddr_t)create_minmax_textmax },
	{(char*) "gec_activate_minmax_ok",(caddr_t)activate_minmax_ok }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  Lng::get_uid( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  cdh_StrncpyCutOff( title, curve_name, sizeof(title), 1);

  reglist[0].value = (caddr_t) this;

  toplevel = XtCreatePopupShell(title, 
		topLevelShellWidgetClass, parent_widget, args, 0);

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "ge_curve_window", toplevel,
			name, NULL, 0, &ge_curve_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, (char*) "minmax_dialog", toplevel,
		&minmax_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  MrmCloseHierarchy(s_DRMh);

  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( toplevel ,args,i);
      
  XtManageChild( ge_curve_widget);

  Widget w;
  grownames_main_widget = ScrolledGrowCreate( nameform_widget, 
		(char*) "GeCurveNames", NULL,
		0, init_grownames_cb, this, &w);
  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetValues( grownames_main_widget, args,i);

  XtManageChild( grownames_main_widget);

  growaxis_main_widget = GrowCreate( axisform_widget, 
		(char*) "GeCurveAxis", NULL,
		0, init_growaxis_cb, this);
  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetValues( growaxis_main_widget, args,i);

  XtManageChild( growaxis_main_widget);

  growcurve_main_widget = CurveCreate( curveform_widget, 
		(char*) "GeCurve", NULL,
		0, init_growcurve_cb, this);
  XtManageChild( growcurve_main_widget);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNpaneMinimum,75);i++;
  XtSetArg(args[i],XmNpaneMaximum,100);i++;
  nav_widget = CurveCreateNav( pane_widget, (char*) "navigator",
        args, i, growcurve_main_widget);
  XtManageChild( nav_widget);

  XtPopup( toplevel, XtGrabNone);
  XtRealizeWidget( toplevel);

  wow = new CoWowMotif( toplevel);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)activate_exit, this);

}
