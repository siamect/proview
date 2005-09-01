/* 
 * Proview   $Id: ge_curve.cpp,v 1.5 2005-09-01 14:57:52 claes Exp $
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

#if defined OS_VMS || defined OS_LINUX
#define LDH 1
#endif

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
#include "flow_browwidget.h"
#include "glow.h"
#include "glow_curvectx.h"
#include "glow_curvewidget.h"
#include "glow_growctx.h"
#include "glow_growwidget.h"
#include "glow_curveapi.h"
#include "glow_msg.h"

extern "C" {
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "flow_x.h"
#include "co_mrm_util.h"
#include "co_wow.h"
}
#include "co_lng.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_curve.h"
#include "ge_msg.h"

static void gec_activate_exit( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  if ( curve->close_cb)
    (curve->close_cb)( curve->parent_ctx);
  else
    delete curve;
}

static void gec_activate_configure( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->configure_curves();
  curve->configure_axes();
}

static void gec_activate_print( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  pwr_tFileName fname;
  pwr_tCmd cmd;

  dcli_translate_filename( fname, "$pwrp_tmp/curve.ps");
  curve->print( fname);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s 1", fname);
  system(cmd);
}

static void gec_activate_zoomin( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve_Zoom( curve->growcurve_ctx, 2.0);
}

static void gec_activate_zoomout( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve_Zoom( curve->growcurve_ctx, 0.5);
}

static void gec_activate_zoomreset( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
}

static void gec_activate_background( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{

  if ( curve->curve_color == curve->background_dark) {
    curve->curve_color = curve->background_bright;
    curve->curve_border = curve->border_bright;
  }
  else {
    curve->curve_color = curve->background_dark;
    curve->curve_border = curve->border_dark;
  }
  grow_SetObjectFillColor( curve->curve_object, curve->curve_color);
  grow_SetObjectBorderColor( curve->curve_object, curve->curve_border);
  curve->cd->select_color( curve->curve_color == curve->background_dark);
  curve->configure_curves();
}

static void gec_activate_showname( Widget w, GeCurve *curve, 
   XmToggleButtonCallbackStruct *data)
{

  if ( data->set) {
    Arg args[2];
    short width;

    width = 200;
    XtSetArg(args[0],XmNwidth,width+4);
    XtSetValues( curve->axisform_widget, args, 1);

    curve->axis_displayed = 0;
    XtManageChild( curve->grownames_main_widget);
    XtUnmanageChild( curve->growaxis_main_widget);

  }
  else {
    double zoom_x, zoom_y;
    short width;
    Arg args[2];

    curve_GetZoom( curve->growcurve_ctx, &zoom_x, &zoom_y);

    width = short( zoom_y * curve->axis_window_width);
    XtSetArg(args[0],XmNwidth,width+4);
    XtSetValues( curve->axisform_widget, args, 1);

    curve->axis_displayed = 1;
    XtManageChild( curve->growaxis_main_widget);
    XtUnmanageChild( curve->grownames_main_widget);
  }
}

static void gec_activate_filledcurves( Widget w, GeCurve *curve, 
   XmToggleButtonCallbackStruct *data)
{

  if ( data->set) {
    grow_SetTrendFillCurve( curve->curve_object, 1);
    curve->configure_curves();
  }
  else {
    grow_SetTrendFillCurve( curve->curve_object, 0);
    curve->configure_curves();
  }
}

static void gec_activate_xlimits( Widget w, GeCurve *curve, 
   XmAnyCallbackStruct *data)
{
  char value_str[80];

  sprintf( value_str, "%f",  curve->cd->min_value_axis[0]);
  XmTextSetString( curve->minmax_textmin_widget, value_str);

  sprintf( value_str, "%f", curve->cd->max_value_axis[0]);
  XmTextSetString( curve->minmax_textmax_widget, value_str);

  curve->minmax_idx = 0;
  XtManageChild( curve->minmax_widget);
}

static void gec_activate_help( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
}

static void gec_create_pane( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->pane_widget = w;
}

static void gec_create_growform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->axisform_widget = w;
}

static void gec_create_curveform( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->curveform_widget = w;
}

static void gec_create_minmax_textmin( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->minmax_textmin_widget = w;
}

static void gec_create_minmax_textmax( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  curve->minmax_textmax_widget = w;
}

static void gec_activate_minmax_ok( Widget w, GeCurve *curve, XmAnyCallbackStruct *data)
{
  char *value;
  double min_value, max_value;
  int nr;

  printf("Here in minmax Ok\n");

  value = XmTextGetString( curve->minmax_textmin_widget);
  nr = sscanf( value, "%lf", &min_value);
  if ( nr != 1)
    return;

  value = XmTextGetString( curve->minmax_textmax_widget);
  nr = sscanf( value, "%lf", &max_value);
  if ( nr != 1)
    return;

  int i = curve->minmax_idx;
  curve->cd->scale( curve->cd->axis_type[i], curve->cd->value_type[i], 
       min_value,  max_value, 
       &curve->cd->min_value_axis[i], &curve->cd->max_value_axis[i], 
       &curve->cd->lines[i], curve->cd->format[i], 
       &curve->cd->axis_width[i], 1, 1);


  // curve->cd->min_value_axis[curve->minmax_idx] = min_value;
  // curve->cd->max_value_axis[curve->minmax_idx] = max_value;

  curve->configure_curves();
  curve->configure_axes();

}


//
// Callbacks from growcurve
//
static int ge_growcurve_cb( GlowCtx *ctx, glow_tEvent event)
{
  GeCurve	*curve;

  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &curve);

  switch ( event->event)
  {
    case glow_eEvent_MB1Click:
      printf( "GrowCurve callback MB1\n");

      break;
    default:
      ;
  }
  return 1;
}

static int ge_init_growcurve_cb( GlowCtx *fctx, void *client_data)
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  GeCurve *curve = (GeCurve *) client_data;
  curve->growcurve_ctx = (CurveCtx *) fctx;

  mask = 0;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = 0;
  mask |= grow_eAttr_double_buffer_on;
  grow_attr.double_buffer_on = 1;
  if ( curve->initial_right_position) { 
    mask |= grow_eAttr_initial_position;
    grow_attr.initial_position = glow_eDirection_Right;
  }
  grow_SetAttributes( curve->growcurve_ctx, &grow_attr, mask); 

  grow_SetCtxUserData( curve->growcurve_ctx, curve);


  grow_EnableEvent( (GrowCtx *)curve->growcurve_ctx, glow_eEvent_MB1Click,
  	glow_eEventType_CallBack, ge_growcurve_cb);

  grow_CreateGrowCurve( curve->growcurve_ctx, "curve", NULL, 0, 0, 200, 30,
			curve->curve_border, 2, glow_mDisplayLevel_1, 1, 1,
                        curve->curve_color, curve, &curve->curve_object);
  grow_CreateGrowAxis( curve->growcurve_ctx, "y_axis", 0, 30, 200, 32,
		       glow_eDrawType_Line, 1, 5, 
		       glow_eDrawType_TextHelvetica, curve, 
                       &curve->curve_axisobject);

  curve->configure_curves();
  return 1;
}

//
// Callbacks from growaxis
//
static int ge_growaxis_cb( GlowCtx *ctx, glow_tEvent event)
{
  GeCurve	*curve;

  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &curve);

  switch ( event->event)
  {
    case glow_eEvent_MB1Click:
    {
      printf( "GrowAxis callback MB1\n");

      if ( event->object.object_type != glow_eObjectType_NoObject) {
        for ( int i = 0; i < curve->cd->cols; i++) {
          if ( event->object.object == curve->axis_object[i]) {
            grow_SetTrendLines( curve->curve_object,
	              curve->cd->lines[0] - 2,
		      2 * curve->cd->lines[i] - 3);
            grow_Redraw( curve->growcurve_ctx);
            grow_SetAxisConf( curve->axis_lineobject, 0, 10,
                  2 * curve->cd->lines[i] - 1, 1, 10, 0, "%2.0f");
            grow_Redraw( curve->growaxis_ctx);
            break;
          }
        }
      }
      break;
    }
    case glow_eEvent_MB1DoubleClick:
    {
      char value_str[80];
      printf( "GrowAxis callback MB1 doubleclick\n");

      if ( event->object.object_type != glow_eObjectType_NoObject) {
        for ( int i = 0; i < curve->cd->cols; i++) {
          if ( event->object.object == curve->axis_object[i]) {
            sprintf( value_str, "%f", 
                   curve->cd->min_value_axis[i]);
            XmTextSetString( curve->minmax_textmin_widget, value_str);

            sprintf( value_str, "%f", 
                   curve->cd->max_value_axis[i]);
            XmTextSetString( curve->minmax_textmax_widget, value_str);

            curve->minmax_idx = i;
            XtManageChild( curve->minmax_widget);
            break;
          }
        }
      }
      break;
    }
    case glow_eEvent_Resized:
    {
      Arg args[2];
      short height, width;
      double zoom_y;

      printf( "GrowAxis callback Resized\n");

      if ( curve->axis_displayed) {
        XtSetArg(args[0],XmNheight, &height);
        XtGetValues( curve->growaxis_main_widget, args, 1);

        curve_GetPreferedZoomY( curve->growcurve_ctx, height, &zoom_y);
        grow_ZoomY( curve->growaxis_ctx, zoom_y);
        grow_ZoomX( curve->growaxis_ctx, zoom_y);

        width = short( zoom_y * curve->axis_window_width);
        XtSetArg(args[0],XmNwidth,width+4);
        XtSetValues( curve->axisform_widget, args, 1);
      }
      break;
    }
    default:
      ;
  }
  return 1;
}

//
// Callbacks from grownames
//

static int ge_grownames_cb( GlowCtx *ctx, glow_tEvent event)
{
  GeCurve	*curve;

  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &curve);

  switch ( event->event)
  {
    case glow_eEvent_MB1Click:
    {
      glow_eDrawType color;

      printf( "GrowNames callback MB1\n");

      if ( event->object.object_type != glow_eObjectType_NoObject) {
        for ( int i = 0; i < curve->cd->cols; i++) {
          if ( event->object.object == curve->name_rect[i] ||
               event->object.object == curve->hide_rect[i] ) {
            if ( curve->hide[i]) {
              // Check max number of curves
              int num = 0;
              for ( int i = 1; i < curve->cd->cols; i++) {
                if ( !curve->hide[i])
                  num++;
              }
              if ( num >= TREND_MAX_CURVES - 1) {
	        wow_DisplayError( curve->toplevel, "Error",
		"         Max number of curves exceeded        ");
                break;
              }
            }
            curve->hide[i] = !curve->hide[i];
            if ( curve->hide[i])
              color = curve->cd->color[i];
            else
              color = glow_eDrawType_Line;
            grow_SetObjectFillColor( curve->hide_rect[i], color);

            if ( curve->auto_refresh) {
              curve->configure_curves();
              curve->configure_axes();
            }
            break;
          }
        }
      }
      break;
    }
    case glow_eEvent_Resized:
    {
      printf( "GrowNames callback Resized\n");
#if 0
      Arg args[2];
      short width;

      width = 200;
      XtSetArg(args[0],XmNwidth,width+4);
      XtSetValues( curve->axisform_widget, args, 1);
#endif
      break;
    }
    default:
      ;
  }
  return 1;
}

static int ge_init_growaxis_cb( GlowCtx *fctx, void *client_data)
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  GeCurve *curve = (GeCurve *) client_data;
  curve->growaxis_ctx = (GrowCtx *) fctx;

  mask = 0;
  // Double buffer is used to avoid pulldown menu at print
  mask |= grow_eAttr_double_buffer_on;
  grow_attr.double_buffer_on = 1;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = 0;
  mask |= grow_eAttr_hot_mode;
  grow_attr.hot_mode = glow_eHotMode_TraceAction;
  grow_SetAttributes( curve->growaxis_ctx, &grow_attr, mask); 

  grow_SetCtxUserData( curve->growaxis_ctx, curve);

  grow_EnableEvent( (GrowCtx *)curve->growaxis_ctx, glow_eEvent_MB1Click,
  	glow_eEventType_CallBack, ge_growaxis_cb);
  grow_EnableEvent( (GrowCtx *)curve->growaxis_ctx, glow_eEvent_MB1DoubleClick,
  	glow_eEventType_CallBack, ge_growaxis_cb);
  grow_EnableEvent( (GrowCtx *)curve->growaxis_ctx, glow_eEvent_Resized,
  	glow_eEventType_CallBack, ge_growaxis_cb);

  curve->configure_axes();
  return 1;
}

static int ge_init_grownames_cb( GlowCtx *fctx, void *client_data)
{
  grow_tObject t1;
  grow_sAttributes grow_attr;
  unsigned long mask;
  glow_eDrawType color;

  GeCurve *curve = (GeCurve *) client_data;
  curve->grownames_ctx = (GrowCtx *) fctx;

  mask = 0;
  // Double buffer is used for print
  mask |= grow_eAttr_double_buffer_on;
  grow_attr.double_buffer_on = 1;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = 0;
  mask |= grow_eAttr_hot_mode;
  grow_attr.hot_mode = glow_eHotMode_TraceAction;
  grow_SetAttributes( curve->grownames_ctx, &grow_attr, mask); 

  grow_SetCtxUserData( curve->grownames_ctx, curve);

  grow_EnableEvent( (GrowCtx *)curve->grownames_ctx, glow_eEvent_MB1Click,
  	glow_eEventType_CallBack, ge_grownames_cb);
  grow_EnableEvent( (GrowCtx *)curve->grownames_ctx, glow_eEvent_Resized,
  	glow_eEventType_CallBack, ge_grownames_cb);

  for ( int i = 1; i < curve->cd->cols; i++) {
    grow_CreateGrowRect( curve->grownames_ctx, "", 0, (i-1), 1, 1,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 1,
			 curve->cd->color[i], NULL, &curve->name_rect[i]);

    if ( curve->hide[i])
      color = curve->cd->color[i];
    else
      color = glow_eDrawType_Line;

    grow_CreateGrowRect( curve->grownames_ctx, "", 0.25, (i-1)+0.25, 0.5, 0.5,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 0, 0,
			 color, NULL, &curve->hide_rect[i]);
    grow_CreateGrowText( curve->grownames_ctx, "", curve->cd->name[i],
		       1.3, (i-1) + 0.85, glow_eDrawType_TextHelveticaBold, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &t1);
  }

  return 1;
}

void GeCurve::print( char *filename)
{
  double x;
  int w1, w2, h1, h2;
  glow_eDrawType c1, c2, c3;

  grow_GetBackgroundColor( growaxis_ctx, &c1);
  grow_GetBackgroundColor( growcurve_ctx, &c2);
  grow_GetBackgroundColor( grownames_ctx, &c3);
  grow_SetBackgroundColor( growaxis_ctx, glow_eDrawType_Color4);
  grow_SetBackgroundColor( growcurve_ctx, glow_eDrawType_Color4);
  grow_SetBackgroundColor( grownames_ctx, glow_eDrawType_Color4);
  grow_Redraw( growaxis_ctx);
  grow_Redraw( growcurve_ctx);
  grow_Redraw( grownames_ctx);
  grow_GetWindowSize( growaxis_ctx, &w1, &h1);
  grow_GetWindowSize( growcurve_ctx, &w2, &h2);
  if ( w1 + w2 == 0)
    return;
  x = double(w1) / (w1 + w2);
  grow_Print( growaxis_ctx, filename, 0.0, x, 0);
  grow_Print( growcurve_ctx, filename, x, 1.0, 0);
  grow_Print( grownames_ctx, filename, 0, 0, 1);
  grow_SetBackgroundColor( growaxis_ctx, c1);
  grow_SetBackgroundColor( growcurve_ctx, c2);
  grow_SetBackgroundColor( grownames_ctx, c3);
  grow_Redraw( growaxis_ctx);
  grow_Redraw( growcurve_ctx);
  grow_Redraw( grownames_ctx);
}

int GeCurve::configure_axes()
{
  double x = 0;
  int i, idx;

  grow_New( growaxis_ctx);
  memset( axis_object, 0, sizeof( axis_object));

  for ( i = 1; i < cd->cols; i++) {
    if ( cd->value_type[i] != pwr_eType_Boolean &&
         !hide[i]) {
      grow_CreateGrowRect( growaxis_ctx, "", x, 0,
			 cd->axis_width[i], 30,
			   glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 0, 0,
			 cd->axiscolor[i], NULL, 
			 &axis_rect[i]);
      x += cd->axis_width[i];
    }
  }

  // Draw horizontal lines with same interval as the trend object
  // Get number of horizontal lines from first not hidden float
  idx = 1;
  for ( i = 1; i < cd->cols; i++) {
    if ( cd->value_type[i] != pwr_eType_Boolean && !hide[i]) {
      idx = i;
      break;
    }
  }
  grow_CreateGrowAxis( growaxis_ctx, "",
		       -2, 0, x + 1, 30,
                       border_bright, 1, 0,
                       glow_eDrawType_TextHelvetica, NULL,
		       &axis_lineobject);
  grow_SetAxisConf( axis_lineobject, 0, 10,
         2 * cd->lines[idx] - 1, 1, 10, 0, "%1.0f");

  x = 0;
  for ( i = 1; i < cd->cols; i++) {
    if ( cd->value_type[i] != pwr_eType_Boolean &&
         !hide[i]) {
      grow_CreateGrowAxis( growaxis_ctx, "",
		       x, 0, x + cd->axis_width[i], 30,
                       glow_eDrawType_Line, 1, 6,
                       glow_eDrawType_TextHelvetica, NULL, 
		       &axis_object[i]);

      grow_SetAxisConf( axis_object[i], cd->max_value_axis[i],
         cd->min_value_axis[i],
         2 * cd->lines[i] - 1, 2, 2, 0, cd->format[i]);
      x += cd->axis_width[i];
    }
  }
  axis_window_width = max( x, 1);

  double zoom_x, zoom_y;
  short width;
  Arg args[2];

  curve_GetZoom( growcurve_ctx, &zoom_x, &zoom_y);
  grow_ZoomY( growaxis_ctx, zoom_y);
  grow_ZoomX( growaxis_ctx, zoom_y);

  if ( axis_displayed) {
    width = short( zoom_y * axis_window_width);
    XtSetArg(args[0],XmNwidth,width+4);
    XtSetValues( axisform_widget, args, 1);
  }
  return 1;
}

int GeCurve::configure_curves()
{
  int i, idx;
  glow_sCurveData gcd;
  int max_index, min_index;
  double minval, maxval;

  // Get max and min index in x */
  max_index = -1;
  min_index = -1;
  if ( cd->min_value_axis[0] > cd->min_value[0] || 
       cd->max_value_axis[0] < cd->max_value[0]) {
    for ( i = 0; i < cd->rows; i++) {
      if ( min_index == -1 && cd->data[0][i] >= cd->min_value_axis[0] )
        min_index = i;
      if ( max_index == -1 && cd->data[0][i] >= cd->max_value_axis[0] )
        max_index = i - 1;
    }
    if ( min_index == -1)
      min_index = 0;
    if ( max_index == -1)
      max_index = cd->rows - 1; 
  }
  else {
    max_index = cd->rows - 1;
    min_index = 0;
  }

  idx = 0;
  for ( i = 0; i < cd->cols; i++) {
    if ( !hide[i]) {
      gcd.max_value[idx] = cd->max_value_axis[i];
      gcd.min_value[idx] = cd->min_value_axis[i];
      gcd.data[idx] = &cd->data[i][min_index];
      gcd.color[idx] = cd->color[i];
      gcd.fillcolor[idx] = cd->fillcolor[i];
      idx++;
    }
  }
  gcd.curves = idx;
  gcd.rows = max_index - min_index + 1;
  gcd.x_reverse = cd->x_reverse;

  // Get number of horizontal lines from first not hidden float
  idx = 1;
  for ( i = 1; i < cd->cols; i++) {
    if ( cd->value_type[i] != pwr_eType_Boolean && !hide[i]) {
      idx = i;
      break;
    }
  }

  grow_SetTrendLines( curve_object,
	 // int(cd->max_value_axis[0] - cd->min_value_axis[0] - 1),
		      cd->lines[0] - 2,
	 2 * cd->lines[idx] - 3);

  if ( cd->x_reverse) {
    minval = cd->max_value_axis[0];
    maxval = cd->min_value_axis[0];
  }
  else {
    minval = cd->min_value_axis[0];
    maxval = cd->max_value_axis[0];
  }

  grow_SetAxisConf( curve_axisobject, minval, maxval, 
	 // 10 * int( cd->max_value_axis[0] - cd->min_value_axis[0]) + 1, 
	 cd->lines[0] * 2 - 1,
         2, 2, 270, cd->format[0]);

  grow_CurveConfigure( curve_object, &gcd); 
  
  return 1;
}

void GeCurve::points_added()
{
  int i, idx;
  glow_sCurveData gcd;
  int max_index, min_index;

  // Get max and min index in x */
  max_index = -1;
  min_index = -1;
  if ( cd->min_value_axis[0] > cd->min_value[0] || 
       cd->max_value_axis[0] < cd->max_value[0]) {
    for ( i = 0; i < cd->rows; i++) {
      if ( min_index == -1 && cd->data[0][i] >= cd->min_value_axis[0] )
        min_index = i;
      if ( max_index == -1 && cd->data[0][i] >= cd->max_value_axis[0] )
        max_index = i - 1;
    }
    if ( min_index == -1)
      min_index = 0;
    if ( max_index == -1)
      max_index = cd->rows - 1; 
  }
  else {
    max_index = cd->rows - 1;
    min_index = 0;
  }

  idx = 0;
  for ( i = 0; i < cd->cols; i++) {
    if ( !hide[i]) {
      gcd.max_value[idx] = cd->max_value_axis[i];
      gcd.min_value[idx] = cd->min_value_axis[i];
      gcd.data[idx] = &cd->data[i][min_index];
      gcd.color[idx] = cd->color[i];
      gcd.fillcolor[idx] = cd->fillcolor[i];
      idx++;
    }
  }
  gcd.curves = idx;
  gcd.rows = max_index - min_index + 1;
  gcd.x_reverse = cd->x_reverse;

  grow_CurveAddPoints( curve_object, &gcd);
}

int GeCurve::read_file( char *filename)
{
  FILE *fp;
  char line[10000];
  char item_str[CURVE_MAX_COLS][80];
  int nr;
  int rows = 0;
  char fname[120];
  int i, j;
  int skip_line;

  dcli_translate_filename( fname, filename);

  fp = fopen( fname, "r");
  if ( !fp)
    return 0;
  
  // Attribute names in first line
  if ( !dcli_read_line( line, sizeof(line), fp)) {  
    fclose( fp);
    return 0;
  }
  nr = dcli_parse( line, " 	", "", (char *)item_str, 
		sizeof( item_str) / sizeof( item_str[0]), 
                sizeof( item_str[0]), 0);
  if ( nr == 0) {
    fclose( fp);
    return 0;
  }

  printf( "line: %s\n", line);
  for ( i = 0; i < nr; i++)
    printf( "item: %s\n", item_str[i]);

  while( dcli_read_line( line, sizeof(line), fp)) {
    rows++;
  }    
  fclose( fp);

  printf( "Rows: %d\n", rows);

  cd = new GeCurveData( curve_eDataType_LogFile);

  cd->x_reverse = 0;
  cd->cols = nr;
  cd->rows = rows;
  for ( i = 0; i < nr; i++) {
    strcpy( cd->name[i], item_str[i]);
    cd->data[i] = (double *) malloc( rows * sizeof( double));
    if ( i == 0)
      cd->axis_type[i] = curve_eAxis_x;
    else
      cd->axis_type[i] = curve_eAxis_y;
  }

  fp = fopen( fname, "r");
  dcli_read_line( line, sizeof(line), fp);

  j = 0;
  while( dcli_read_line( line, sizeof(line), fp)) {
    dcli_parse( line, " 	", "", (char *)item_str, 
		sizeof( item_str) / sizeof( item_str[0]), 
                sizeof( item_str[0]), 0);
    skip_line = 0;
    for ( i = 0; i < cd->cols; i++) {
      nr = sscanf( item_str[i], "%lf", &cd->data[i][j]);
      if ( nr != 1) {
        if ( i == 0) {
          printf( "Unreadble line %d\n", j);
          skip_line = 1;
          cd->rows--;
          break;
        }
        else
          cd->data[i][j] = 0;
      }
    }
    if ( skip_line)
      continue;
    j++;
  }    
  fclose( fp);

  return 1;
}

void GeCurve::pop()
{
  flow_UnmapWidget( toplevel);
  flow_MapWidget( toplevel);
}

void GeCurve::set_time( pwr_tTime time)
{
  char time_str[40];
  char full_title[200];

  time_AtoAscii( &time, time_eFormat_DateAndTime, time_str, sizeof(time_str));

  sprintf( full_title, "%s   %s", title, time_str);
  write_title( full_title);
}

void GeCurve::set_title( char *str)
{
  strcpy( title, str);
  write_title( str);
}

void GeCurve::write_title( char *str)
{
  Arg args[1];

  XtSetArg(args[0],XmNtitle, str);
  XtSetValues( toplevel, args, 1);
}

GeCurve::~GeCurve()
{
  XtDestroyWidget( toplevel);
}

GeCurve::GeCurve( void 	*gc_parent_ctx, 
		Widget 	parent_widget,
		  char	*curve_name,
		  char  *filename,
                  GeCurveData *curve_data,
                  int   pos_right) :
  parent_ctx(gc_parent_ctx), background_dark(glow_eDrawType_Color29),
  background_bright(glow_eDrawType_Color21),
  border_dark(glow_eDrawType_Color28),
  border_bright(glow_eDrawType_Color22),
  cd(0), axis_window_width(0), auto_refresh(1), axis_displayed(1),
  minmax_idx(0), close_cb(0), initial_right_position(pos_right)
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
        { "gec_ctx", 0 },
	{"gec_activate_exit",(caddr_t)gec_activate_exit },
	{"gec_activate_configure",(caddr_t)gec_activate_configure },
	{"gec_activate_print",(caddr_t)gec_activate_print },
	{"gec_activate_zoomin",(caddr_t)gec_activate_zoomin },
	{"gec_activate_zoomout",(caddr_t)gec_activate_zoomout },
	{"gec_activate_zoomreset",(caddr_t)gec_activate_zoomreset },
	{"gec_activate_background",(caddr_t)gec_activate_background },
	{"gec_activate_showname",(caddr_t)gec_activate_showname },
	{"gec_activate_filledcurves",(caddr_t)gec_activate_filledcurves },
	{"gec_activate_xlimits",(caddr_t)gec_activate_xlimits },
	{"gec_activate_help",(caddr_t)gec_activate_help },
	{"gec_create_pane",(caddr_t)gec_create_pane },
	{"gec_create_growform",(caddr_t)gec_create_growform },
	{"gec_create_curveform",(caddr_t)gec_create_curveform },
	{"gec_create_minmax_textmin",(caddr_t)gec_create_minmax_textmin },
	{"gec_create_minmax_textmax",(caddr_t)gec_create_minmax_textmax },
	{"gec_activate_minmax_ok",(caddr_t)gec_activate_minmax_ok }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  Lng::get_uid( uid_filename, uid_filename);

  memset( hide, 0, sizeof(hide));
  memset( name_rect, 0, sizeof(name_rect));
  memset( hide_rect, 0, sizeof(hide_rect));
  curve_color = background_dark;
  curve_border = border_dark;
  for ( i = TREND_MAX_CURVES; i < CURVE_MAX_COLS; i++)
    hide[i] = 1;

  if ( filename) {
    sts = read_file( filename);
    if ( EVEN(sts)) return;

    cd->get_borders();
    cd->get_default_axis();
    cd->select_color( curve_color == background_dark);
  }
  else if ( curve_data) {
    cd = curve_data;
    cd->select_color( curve_color == background_dark);
  }

  // Motif
  MrmInitialize();

  strcpy( title, curve_name);

  reglist[0].value = (caddr_t) this;

  toplevel = XtCreatePopupShell(title, 
		topLevelShellWidgetClass, parent_widget, args, 0);

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "ge_curve_window", toplevel,
			name, NULL, 0, &ge_curve_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, "minmax_dialog", toplevel,
		&minmax_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  MrmCloseHierarchy(s_DRMh);

  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( toplevel ,args,i);
      
  XtManageChild( ge_curve_widget);

  growaxis_main_widget = GrowCreate( axisform_widget, 
		"GeCurveAxis", NULL,
		0, ge_init_growaxis_cb, this);
  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetValues( growaxis_main_widget, args,i);

  XtManageChild( growaxis_main_widget);

  Widget w;
  grownames_main_widget = ScrolledGrowCreate( axisform_widget, 
		"GeCurveNames", NULL,
		0, ge_init_grownames_cb, this, &w);
  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetValues( grownames_main_widget, args,i);

  XtManageChild( grownames_main_widget);

  growcurve_main_widget = CurveCreate( curveform_widget, 
		"GeCurve", NULL,
		0, ge_init_growcurve_cb, this);
  XtManageChild( growcurve_main_widget);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNpaneMinimum,75);i++;
  XtSetArg(args[i],XmNpaneMaximum,100);i++;
  nav_widget = CurveCreateNav( pane_widget, "navigator",
        args, i, growcurve_main_widget);
  XtManageChild( nav_widget);

  XtPopup( toplevel, XtGrabNone);
  XtRealizeWidget( toplevel);

  XtUnmanageChild( grownames_main_widget);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)gec_activate_exit, this);

}

GeCurveData::GeCurveData( curve_eDataType datatype) :
  type(datatype), rows(0), cols(0), x_reverse(0)
{
  memset( data, 0, sizeof(data));
  for ( int i = 0; i < CURVE_MAX_COLS; i++) {
    max_value[i] = 0;
    min_value[i] = 0;
    min_value_axis[i] = 0;
    max_value_axis[i] = 0;
    lines[i] = 0;
    axis_width[i] = 0;
  }
}

GeCurveData::~GeCurveData()
{
  for ( int i = 0; i < cols; i++)
    free( (char *) data[i]);
}

void GeCurveData::get_borders()
{
  for ( int i = 0; i < cols; i++) {
    max_value[i] = 1e-37;
    min_value[i] = 1e37;

    value_type[i] = pwr_eType_Boolean;

    for ( int j = 0; j < rows; j++) {
      if ( data[i][j] < min_value[i])
        min_value[i] = data[i][j];
      if ( data[i][j] > max_value[i])
        max_value[i] = data[i][j];
      if ( value_type[i] == pwr_eType_Boolean && 
           !( data[i][j] == 1 || data[i][j] == 0)) {
        value_type[i] = pwr_eType_Float64;
        printf( "Not Boolean %s: %f\n", name[i], data[i][j]);
      }
    }
  } 
}

void GeCurveData::get_default_axis()
{

  for ( int i = 0; i < cols; i++) {

    scale( axis_type[i], value_type[i], min_value[i],  max_value[i], 
       &min_value_axis[i], &max_value_axis[i], &lines[i], format[i], 
       &axis_width[i], 0, 0);
  }
}

void GeCurveData::select_color( bool dark_bg)
{
  int j;

  for ( int i = 0; i < cols; i++) {
    j = i % 9;
    switch( j) {
    case 0:
      // Seablue
      color[i] = glow_eDrawType_Color265;
      axiscolor[i] = glow_eDrawType_Color255;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color257;
      else
	fillcolor[i] = glow_eDrawType_Color254;
      break;
    case 1:
      // Orange
      if ( dark_bg)
	color[i] = glow_eDrawType_Color144;
      else
	color[i] = glow_eDrawType_Color146;
      axiscolor[i] = glow_eDrawType_Color135;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color137;
      else
	fillcolor[i] = glow_eDrawType_Color133;
      break;
    case 2:
      // YellowGreen
      if ( dark_bg)
	color[i] = glow_eDrawType_Color85;
      else
	color[i] = glow_eDrawType_Color87;
      axiscolor[i] = glow_eDrawType_Color75;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color67;
      else
	fillcolor[i] = glow_eDrawType_Color64;
      break;
    case 3:
      // Yellow
      if ( dark_bg)
	color[i] = glow_eDrawType_Color115;
      else
	color[i] = glow_eDrawType_Color117;
      axiscolor[i] = glow_eDrawType_Color105;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color107;
      else
	fillcolor[i] = glow_eDrawType_Color104;
      break;
    case 4:
      // Blue
      color[i] = glow_eDrawType_Color235;
      axiscolor[i] = glow_eDrawType_Color225;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color227;
      else
	fillcolor[i] = glow_eDrawType_Color214;
      break;
    case 5:
      // Violet
      color[i] = glow_eDrawType_Color205;
      axiscolor[i] = glow_eDrawType_Color195;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color197;
      else
	fillcolor[i] = glow_eDrawType_Color184;
      break;
    case 6:
      // Red
      color[i] = glow_eDrawType_Color175;
      axiscolor[i] = glow_eDrawType_Color165;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color167;
      else
	fillcolor[i] = glow_eDrawType_Color154;
      break;
    case 7:
      // Green
      color[i] = glow_eDrawType_Color295;
      axiscolor[i] = glow_eDrawType_Color285;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color287;
      else
	fillcolor[i] = glow_eDrawType_Color274;
      break;
    case 8:
      // Gray
      color[i] = glow_eDrawType_Color35;
      axiscolor[i] = glow_eDrawType_Color35;
      if ( dark_bg)
	fillcolor[i] = glow_eDrawType_Color37;
      else
	fillcolor[i] = glow_eDrawType_Color34;
      break;

    }
#if 0
    if ( j < 4)
      color[i] = (glow_eDrawType) (j * 20 + 27);
    else if ( j >= 4 && j < 8)
      color[i] = (glow_eDrawType) ((j - 4) * 20 + 37);
    else if ( j >= 8 && j < 12)
      color[i] = (glow_eDrawType) ((j - 8) * 20 + 22);
    else
      color[i] = (glow_eDrawType) ((j - 12) * 20 + 32);

    fillcolor[i] = (glow_eDrawType)(color[i] + 1);
#endif
  }
}

void GeCurveData::scale( int axis_type, int value_type, 
     double min_value, double max_value, 
     double *min_value_axis, double *max_value_axis, 
     int *lines, char *format,
     double *axis_width, int not_zero, int allow_odd)
{
  double value, maxval, minval;
  int i_value;
  int n, max_n, min_n;
  int min_lines, max_lines;
  int min_zero, max_zero;
  int format_int, format_dec;
  
  // Scale 0 - 10 for boolean
  if ( value_type == pwr_eType_Boolean) {
    maxval = 10;
    minval = 0;
    i_value = 10;
    max_lines = i_value;
    min_lines = 0;
    n = 0;
  }
  else {      
    n = 0;
    if ( (type == curve_eDataType_LogFile || type == curve_eDataType_DsTrend) 
         && axis_type == curve_eAxis_x) {
      // Time axis
      if ( max_value - min_value < 300) {
        i_value = int(max_value + 1);
        maxval = i_value;
        max_lines = i_value;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value - 1);
        minval = i_value;
        min_lines = abs(i_value);
      }
      else if ( max_value - min_value < 3000) {
        i_value = int(max_value/10) * 10 + 10;
        maxval = i_value;
        max_lines = i_value / 10;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/10) * 10 - 10;
        minval = i_value;
        min_lines = abs(i_value) / 10;
      }
      else {
        i_value = int(max_value/100) * 100 + 100;
        maxval = i_value;
        max_lines = i_value / 100;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/100) * 100 - 100;
        minval = i_value;
        min_lines = abs(i_value) / 100;
      }
    }
    else {
      min_zero = 0;
      max_zero = 0;

      // Power for max_value
      if ( (max_value < DBL_EPSILON && !not_zero) ||
           fabs(max_value) < DBL_EPSILON) {
        maxval = 0;
        max_lines = 0;
        max_n = 0;
        max_zero = 1;
      }
      else {
        value = fabs(max_value);
        n = 0;
        if ( value >= 1) {
          while ( value / 10 > 1) {
            value = value / 10;
            n++;
          }
          max_n = n;
        }
        else {
          while ( value * 10 < 10) {
            value = value * 10;
            n++;
          }
          max_n = -n;
        }
      }

      // Power for min_value
      if ( (min_value > -DBL_EPSILON && !not_zero) ||
           fabs(min_value) < DBL_EPSILON) {
        minval = 0;
        min_lines = 0;
        min_n = 0;
        min_zero = 1;
      }
      else {
        value = fabs(min_value);
        n = 0;
        if ( value >= 1) {
          while ( value / 10 > 1) {
            value = value / 10;
            n++;
          }
          min_n = n;
        }
        else {
          while ( value * 10 < 10) {
            value = value * 10;
            n++;
          }
	  min_n = -n;
        }
      }

      if ( min_zero) {
        // Use power for max_value

        i_value = int( max_value * pow(10, -max_n)) + 1;
        if ( fabs(double(i_value-1) - max_value * pow(10, -n)) < 1e-10)
          i_value--;
        if ( ODD(i_value) && i_value != 5 && !allow_odd) 
          i_value += 1;
        maxval = double(i_value) * pow( 10, max_n);
        max_lines = i_value;
        n = max_n;
      }
      else if ( max_zero) {
        // Use power for min_value

        i_value = int( min_value * pow(10, -min_n)) + 1;
        if ( fabs(double(i_value+1) - min_value * pow(10, -n)) < 1e-10)
          i_value++;
        if ( ODD(i_value) && i_value != 5 && !allow_odd) 
          i_value += 1;
        minval = double(i_value) * pow( 10, min_n);
        min_lines = i_value;
        n = min_n;
      }
      else {
        // Use largest power of min and max
        if ( max_n > min_n)
	  n = max_n;
	else
	  n = min_n;

        if ( max_value > 0) {
          i_value = int( max_value * pow(10, -n)) + 1;
          if ( fabs(double(i_value-1) - max_value * pow(10, -n)) < 1e-10)
            i_value--;
        }
        else
          i_value = int( max_value * pow(10, -n));
        if ( ODD(i_value) && i_value != 5 && !allow_odd) 
          i_value += 1;
        maxval = double(i_value) * pow( 10, n);
        max_lines = i_value;

        if ( min_value < 0) {
          i_value = int( min_value * pow(10, -n)) - 1;
          if ( fabs(double(i_value+1) - min_value * pow(10, -n)) < 1e-10)
            i_value++;
        }
        else
          i_value = int( min_value * pow(10, -n));
        if ( ODD(i_value) && i_value != 5 && !allow_odd) 
          i_value -= 1;
        minval = double(i_value) * pow( 10, n);
        min_lines = -i_value;
      }
    }
  }
  *max_value_axis = maxval;
  *min_value_axis = minval;
  *lines = max_lines + min_lines + 1;

  format_int = abs(n) + 1;
  if ( n > 0)
    format_dec = 0;
  else {
    format_dec = abs(n);
    format_int++;
  }
  if ( minval < 0)
    format_int++;
  sprintf( format, "%%%d.%df", format_int, format_dec);

  *axis_width = 0.65 * format_int + 0.4;

  printf( "%f	%f	%f	%f	%3d %5s %4.1f\n", 
              min_value, max_value, 
              *min_value_axis, *max_value_axis, *lines, format, *axis_width);
  return;
}

