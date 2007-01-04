/* 
 * Proview   $Id: ge_curve.cpp,v 1.15 2007-01-04 08:18:35 claes Exp $
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

#include "rt_load.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow.h"
#include "glow_curvectx.h"
#include "glow_growctx.h"
#include "glow_curveapi.h"
#include "glow_msg.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_lng.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_curve.h"
#include "ge_msg.h"

#define MARK_WIDTH 2.0

void GeCurve::activate_exit()
{
  if ( close_cb)
    (close_cb)( parent_ctx);
  else
    delete this;
}

void GeCurve::activate_configure()
{
  configure_curves();
  configure_axes();
}

void GeCurve::activate_print()
{
  pwr_tFileName fname;
  pwr_tCmd cmd;

  dcli_translate_filename( fname, "$pwrp_tmp/curve.ps");
  print( fname);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s 1", fname);
  system(cmd);
}

void GeCurve::activate_background()
{

  if ( curve_color == background_dark) {
    curve_color = background_bright;
    curve_border = border_bright;
  }
  else {
    curve_color = background_dark;
    curve_border = border_dark;
  }
  grow_SetObjectFillColor( curve_object, curve_color);
  grow_SetObjectBorderColor( curve_object, curve_border);
  cd->select_color( curve_color == background_dark);
  configure_curves();
}

void GeCurve::activate_filledcurves( int set)
{

  if ( set) {
    grow_SetTrendFillCurve( curve_object, 1);
    configure_curves();
  }
  else {
    grow_SetTrendFillCurve( curve_object, 0);
    configure_curves();
  }
}

void GeCurve::activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx);
}

void GeCurve::activate_minmax_ok( double min_value, double max_value)
{

  int i = minmax_idx;
  cd->scale( cd->axis_type[i], cd->value_type[i], 
       min_value,  max_value, 
       &cd->min_value_axis[i], &cd->max_value_axis[i], 
       &cd->trend_lines[i], &cd->axis_lines[i], &cd->axis_linelongq[i],
       &cd->axis_valueq[i], cd->format[i], 
       &cd->axis_width[i], 1, 1);


  // cd->min_value_axis[minmax_idx] = min_value;
  // cd->max_value_axis[minmax_idx] = max_value;

  configure_curves();
  configure_axes();

}


//
// Callbacks from growcurve
//
int GeCurve::growcurve_cb( GlowCtx *ctx, glow_tEvent event)
{
  GeCurve	*curve;

  grow_GetCtxUserData( (GrowCtx *)ctx, (void **) &curve);

  switch ( event->event) {
  case glow_eEvent_MB1Click:
    break;
  case glow_eEvent_SliderMoveStart: {
    printf( "Slider start\n");
    if ( event->object.object_type == glow_eObjectType_NoObject)
      grow_SetMoveRestrictions( (GrowCtx *)ctx, glow_eMoveRestriction_Disable, 0, 0, NULL);
    else
      grow_SetMoveRestrictions( (GrowCtx *)ctx, glow_eMoveRestriction_HorizontalSlider, 
				200 - MARK_WIDTH/2, -MARK_WIDTH/2, 
				curve->curve_markobject);
    break;
  }
  case glow_eEvent_SliderMoved: {
    char str[40];
    int row;
    double ll_x, ll_y, ur_x, ur_y;
    double time;

    grow_MeasureNode( curve->curve_markobject, &ll_x, &ll_y, &ur_x, &ur_y);
    
    curve->last_mark_x = event->any.x;

    if ( !curve->cd->x_reverse)
      time = event->any.x *
        (curve->cd->max_value_axis[0] - curve->cd->min_value_axis[0]) / 200;
    else
      time = (200.0 - event->any.x) *
        (curve->cd->max_value_axis[0] - curve->cd->min_value_axis[0]) / 200;

    // Approximate row
    row = int ((time - curve->cd->min_value[0]) / 
        (curve->cd->max_value[0] - curve->cd->min_value[0]) *
        (curve->cd->rows - 1) + 0.5);
    if ( row > curve->cd->rows - 1)
      row = curve->cd->rows - 1;
    else if ( row < 0)
      row = 0;
    else {
      // Find exact row
      double b1, b2;
      int r = row;
      for (int i = 0;; i++) {
	if ( r == 0) {
	  b2 = (curve->cd->data[0][row] + curve->cd->data[0][r+1]) / 2;
	  if ( time < b2)
	    break;
	  r++;
	}
	else if ( r == curve->cd->rows - 1) {
	  b1 = (curve->cd->data[0][r] + curve->cd->data[0][r-1]) / 2;
	  if ( time >= b1)
	    break;
	  r--;
	}
	else {
	  b1 = (curve->cd->data[0][r] + curve->cd->data[0][r-1]) / 2;
	  b2 = (curve->cd->data[0][r] + curve->cd->data[0][r+1]) / 2;
	  if ( b1 <= time && time < b2)
	    break;
	  if ( b1 <= time)
	    r++;
	  else
	    r--;
	}
	if ( i > 100) {
	  // Corrupt data, se original row
	  r = row;
	  break;
	}	  
      }
      row = r;
    }

    for ( int i = 1; i < curve->cd->cols; i++) {
      sprintf( str, "%7.2f", curve->cd->data[i][row]);
      grow_SetAnnotation( curve->mark_annot[i], 0, str, strlen(str));
    }

    sprintf( str, "%7.2f", curve->cd->data[0][row]);
    grow_SetAnnotation( curve->mark_annot[0], 0, str, strlen(str));

    break;
  }
  case glow_eEvent_CursorMotion: {
    char str[40];
    int row;
    double time;

    curve->last_cursor_x = event->any.x;
    if ( !curve->cd->x_reverse)
      time = event->any.x *
        (curve->cd->max_value_axis[0] - curve->cd->min_value_axis[0]) / 200;
    else
      time = (200.0 - event->any.x) *
        (curve->cd->max_value_axis[0] - curve->cd->min_value_axis[0]) / 200;

    // Approximate row
    row = int ((time - curve->cd->min_value[0]) / 
        (curve->cd->max_value[0] - curve->cd->min_value[0]) *
        (curve->cd->rows - 1) + 0.5);
    if ( row > curve->cd->rows - 1)
      row = curve->cd->rows - 1;
    else if ( row < 0)
      row = 0;
    else {
      // Find exact row
      double b1, b2;
      int r = row;
      for (int i = 0;; i++) {
	if ( r == 0) {
	  b2 = (curve->cd->data[0][row] + curve->cd->data[0][r+1]) / 2;
	  if ( time < b2)
	    break;
	  r++;
	}
	else if ( r == curve->cd->rows - 1) {
	  b1 = (curve->cd->data[0][r] + curve->cd->data[0][r-1]) / 2;
	  if ( time >= b1)
	    break;
	  r--;
	}
	else {
	  b1 = (curve->cd->data[0][r] + curve->cd->data[0][r-1]) / 2;
	  b2 = (curve->cd->data[0][r] + curve->cd->data[0][r+1]) / 2;
	  if ( b1 <= time && time < b2)
	    break;
	  if ( b1 <= time)
	    r++;
	  else
	    r--;
	}
	if ( i > 100) {
	  // Corrupt data, se original row
	  r = row;
	  break;
	}	  
      }
      row = r;
    }
    for ( int i = 1; i < curve->cd->cols; i++) {
      sprintf( str, "%7.2f", curve->cd->data[i][row]);
      grow_SetAnnotation( curve->cursor_annot[i], 0, str, strlen(str));
    }

    sprintf( str, "%7.2f", curve->cd->data[0][row]);
    grow_SetAnnotation( curve->cursor_annot[0], 0, str, strlen(str));
    break;
  }
  default:
    ;
  }
  return 1;
}

int GeCurve::init_growcurve_cb( GlowCtx *fctx, void *client_data)
{
  grow_sAttributes grow_attr;
  unsigned long mask;

  GeCurve *curve = (GeCurve *) client_data;
  curve->growcurve_ctx = (CurveCtx *) fctx;

  mask = 0;
  mask |= grow_eAttr_grid_on;
  grow_attr.grid_on = 0;
  mask |= grow_eAttr_double_buffer_on;
  grow_attr.default_hot_mode = glow_eHotMode_TraceAction;
  mask |= grow_eAttr_default_hot_mode;
  grow_attr.double_buffer_on = 1;
  if ( curve->initial_right_position) { 
    mask |= grow_eAttr_initial_position;
    grow_attr.initial_position = glow_eDirection_Right;
  }
  grow_SetAttributes( curve->growcurve_ctx, &grow_attr, mask); 

  grow_SetCtxUserData( curve->growcurve_ctx, curve);

  grow_SetMoveRestrictions( curve->growcurve_ctx, glow_eMoveRestriction_Disable, 0, 0, NULL);

  grow_EnableEvent( (GrowCtx *)curve->growcurve_ctx, glow_eEvent_MB1Click,
  	glow_eEventType_CallBack, growcurve_cb);
  grow_EnableEvent( (GrowCtx *)curve->growcurve_ctx, glow_eEvent_CursorMotion, 
	glow_eEventType_CallBack, growcurve_cb);
  grow_EnableEvent( (GrowCtx *)curve->growcurve_ctx, glow_eEvent_SliderMoved, 
	glow_eEventType_CallBack, growcurve_cb);
  grow_EnableEvent( (GrowCtx *)curve->growcurve_ctx, glow_eEvent_SliderMoveStart, 
	glow_eEventType_CallBack, growcurve_cb);
  grow_EnableEvent( (GrowCtx *)curve->growcurve_ctx, glow_eEvent_MB1Press, 
        glow_eEventType_MoveNode, growcurve_cb);

  grow_CreateGrowCurve( curve->growcurve_ctx, "curve", NULL, 0, 0, 200, 30,
			curve->curve_border, 2, glow_mDisplayLevel_1, 1, 1,
                        curve->curve_color, curve, &curve->curve_object);
  grow_CreateGrowAxis( curve->growcurve_ctx, "y_axis", 0, 30, 200, 32,
		       glow_eDrawType_Line, 1, 5, 
		       glow_eDrawType_TextHelvetica, curve, 
                       &curve->curve_axisobject);

  grow_tNodeClass nc;
  grow_CreateNodeClass( curve->growcurve_ctx, "MarkNc", glow_eNodeGroup_Common, &nc);
  grow_AddLine( nc, "", 0, 0, 0, 32, glow_eDrawType_LineGray, 1, 0, NULL);
  grow_AddRect( nc, "", -MARK_WIDTH/2, 30.1, MARK_WIDTH, 1.8, glow_eDrawType_LineGray, 1, 0,
		glow_mDisplayLevel_1, 0, 0, 1,
		glow_eDrawType_Color33, NULL);
  glow_sPoint p1[3] = { {-0.1, 30.3}, {-MARK_WIDTH/2+0.1, 31},{-0.1, 31.7}};
  grow_AddPolyLine( nc, "", p1, 3, glow_eDrawType_Line, 1, 0, 1, 0, 1, glow_eDrawType_Color38,
		    1, 0);
  glow_sPoint p2[3] = { {0.1, 30.3}, { MARK_WIDTH/2-0.1, 31},{ 0.1, 31.7}};
  grow_AddPolyLine( nc, "", p2, 3, glow_eDrawType_Line, 1, 0, 1, 0, 1, glow_eDrawType_Color38,
		    1, 0);
  grow_CreateGrowSlider( curve->growcurve_ctx, "", nc, 1, 0, NULL, &curve->curve_markobject);
  grow_SetSliderInfo( curve->curve_markobject, glow_eDirection_Right,
	200, 0, 200, 0);
  // grow_SetMode( curve->growcurve_ctx, grow_eMode_Edit);

  curve->configure_curves();
  if ( curve->deferred_configure_axes)
    curve->configure_axes();
  return 1;
}

//
// Callbacks from growaxis
//
int GeCurve::growaxis_cb( GlowCtx *ctx, glow_tEvent event)
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
	              curve->cd->trend_lines[0] - 2,
		      2 * curve->cd->trend_lines[i] - 3);
            grow_Redraw( curve->growcurve_ctx);
            grow_SetAxisConf( curve->axis_lineobject, 0, 10,
                  2 * curve->cd->trend_lines[i] - 1, 1, 10, 0, "%2.0f");
            grow_Redraw( curve->growaxis_ctx);
            break;
          }
        }
      }
      break;
    }
    case glow_eEvent_MB1DoubleClick:
    {
      if ( event->object.object_type != glow_eObjectType_NoObject) {
        for ( int i = 0; i < curve->cd->cols; i++) {
          if ( event->object.object == curve->axis_object[i]) {
	    curve->open_minmax( i);
            break;
          }
        }
      }
      break;
    }
    case glow_eEvent_Resized:
    {
      if ( curve->axis_displayed)
	curve->resize();
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

int GeCurve::grownames_cb( GlowCtx *ctx, glow_tEvent event)
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
          if ( event->object.object == curve->hide_rect[i] ) {
            if ( curve->hide[i]) {
              // Check max number of curves
              int num = 0;
              for ( int i = 1; i < curve->cd->cols; i++) {
                if ( !curve->hide[i])
                  num++;
              }
              if ( num >= TREND_MAX_CURVES - 1) {
	        curve->wow->DisplayError( "Error",
					  "         Max number of curves exceeded        ");
                break;
              }
            }
            curve->hide[i] = !curve->hide[i];
            if ( curve->hide[i])
              color = glow_eDrawType_LineErase;
            else
              color = glow_eDrawType_Line;
            grow_SetObjectBorderColor( curve->hide_l1[i], color);
            grow_SetObjectBorderColor( curve->hide_l2[i], color);

            if ( curve->auto_refresh) {
              curve->configure_curves();
              curve->configure_axes();
            }
            break;
          }
          if ( event->object.object == curve->scale_rect[i] ) {
	    curve->open_minmax( i);
	    break;
	  }
        }
      }
      break;
    }
    case glow_eEvent_HotRequest:
      return 0;
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

int GeCurve::init_growaxis_cb( GlowCtx *fctx, void *client_data)
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
  	glow_eEventType_CallBack, growaxis_cb);
  grow_EnableEvent( (GrowCtx *)curve->growaxis_ctx, glow_eEvent_MB1DoubleClick,
  	glow_eEventType_CallBack, growaxis_cb);
  grow_EnableEvent( (GrowCtx *)curve->growaxis_ctx, glow_eEvent_Resized,
  	glow_eEventType_CallBack, growaxis_cb);

  if ( curve->growcurve_ctx)
    curve->configure_axes();
  else
    curve->deferred_configure_axes = 1;
  return 1;
}

int GeCurve::init_grownames_cb( GlowCtx *fctx, void *client_data)
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
  	glow_eEventType_CallBack, grownames_cb);
  grow_EnableEvent( (GrowCtx *)curve->grownames_ctx, glow_eEvent_Resized,
  	glow_eEventType_CallBack, grownames_cb);
  grow_EnableEvent( (GrowCtx *)curve->grownames_ctx, glow_eEvent_HotRequest, 
	glow_eEventType_CallBack, grownames_cb);

  // Create nodeclass for mark values
  grow_tNodeClass nc;
  grow_CreateNodeClass( curve->grownames_ctx, "MarkVal", glow_eNodeGroup_Common, &nc);
  grow_AddRect( nc, "", 0, 0, 3, 0.75, glow_eDrawType_LineGray, 1, 0,
		glow_mDisplayLevel_1, 0, 0, 0,
		glow_eDrawType_Line, NULL);
  grow_AddAnnot( nc, 0.2, 0.7, 0, glow_eDrawType_TextHelvetica, 
		 glow_eDrawType_Line,
		 2, glow_eAnnotType_OneLine, 0, glow_mDisplayLevel_1, NULL);

  // Draw header
  grow_tObject o1;
  grow_CreateGrowLine( curve->grownames_ctx, "", 0, 0.75, 60, 0.75,
			 glow_eDrawType_Color34, 2, 0, NULL, &o1);
  grow_CreateGrowRect( curve->grownames_ctx, "", 0, 0, 60, 0.8,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 0, 0, 1,
			 glow_eDrawType_Color32, NULL, &o1);
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("View"),
		       0.8, 0.6, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &o1);
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("Cursor"),
		       3, 0.6, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &o1);
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("Mark"),
		       5.7, 0.6, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &o1);
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("Unit"),
		       9.0, 0.6, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &o1);
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("Scale"),
		       11, 0.6, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &o1);
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("Attribute"),
		       14, 0.6, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &o1);
  
  for ( int i = 1; i < curve->cd->cols; i++) {
    // Draw shadowed frame
    grow_CreateGrowRect( curve->grownames_ctx, "", 0, (i-0.2), 60, 1,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 0, 0, 1,
			 glow_eDrawType_Color32, NULL, &o1);
    // Draw color rectangle
    grow_CreateGrowRect( curve->grownames_ctx, "", 0.25, (i-0.2)+0.3, 0.75, 0.5,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 1,
			 curve->cd->color[i], NULL, &curve->name_rect[i]);

    if ( curve->hide[i])
      color = glow_eDrawType_LineErase;
    else
      color = glow_eDrawType_Line;

    // Draw checkbox for hide
    grow_CreateGrowLine( curve->grownames_ctx, "", 1.4, (i-0.2)+0.45, 1.52, (i-0.2)+0.75,
			 color, 2, 0, NULL, &curve->hide_l1[i]);
    grow_CreateGrowLine( curve->grownames_ctx, "", 1.50, (i-0.2)+0.75, 1.77, (i-0.2)+0.35,
			 color, 2, 0, NULL, &curve->hide_l2[i]);
    grow_CreateGrowRect( curve->grownames_ctx, "", 1.3, (i-0.2)+0.3, 0.5, 0.5,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 0, 1, 1,
			 glow_eDrawType_Color32, NULL, &curve->hide_rect[i]);
    // Draw nodes for mark and cursor values
    grow_CreateGrowNode( curve->grownames_ctx, "", nc, 2.2, (i-0.2)+0.05, NULL, 
			 &curve->cursor_annot[i]);
    grow_CreateGrowNode( curve->grownames_ctx, "", nc, 5.4, (i-0.2)+0.05, NULL, 
			 &curve->mark_annot[i]);
    // Draw unit
    grow_CreateGrowText( curve->grownames_ctx, "", curve->cd->unit[i],
		       9.0, (i-0.2) + 0.75, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &t1);
    // Draw button for scale
    grow_CreateGrowRect( curve->grownames_ctx, "", 11, (i-0.2)+0.1, 1.2, 0.7,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 1,
			 glow_eDrawType_Color33, NULL, &curve->scale_rect[i]);
    grow_SetObjectShadowWidth( curve->scale_rect[i], 20);
    // Draw attribute name
    grow_CreateGrowText( curve->grownames_ctx, "", curve->cd->name[i],
		       14.0, (i-0.2) + 0.75, glow_eDrawType_TextHelveticaBold, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &t1);
    grow_SetAnnotation( curve->cursor_annot[i], 0, "0", 1);
    grow_SetAnnotation( curve->mark_annot[i], 0, "0", 1);
  }
  // Draw nodes for time values
  // Draw shadowed frame
  grow_CreateGrowRect( curve->grownames_ctx, "", 0, (curve->cd->cols-0.2), 60, 1,
			 glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 0, 0, 1,
			 glow_eDrawType_Color32, NULL, &o1);
  grow_CreateGrowNode( curve->grownames_ctx, "", nc, 2.2, (curve->cd->cols-0.2)+0.05, NULL, 
		       &curve->cursor_annot[0]);
  grow_CreateGrowNode( curve->grownames_ctx, "", nc, 5.4, (curve->cd->cols-0.2)+0.05, NULL, 
		       &curve->mark_annot[0]);
  // Draw unit
  grow_CreateGrowText( curve->grownames_ctx, "", "s",
		       9.0, (curve->cd->cols-0.2) + 0.75, glow_eDrawType_TextHelvetica, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &t1);
  // Draw button for scale
  grow_CreateGrowRect( curve->grownames_ctx, "", 11, (curve->cd->cols-0.2)+0.1, 1.2, 0.7,
		       glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 1,
		       glow_eDrawType_Color33, NULL, &curve->scale_rect[0]);
  grow_SetObjectShadowWidth( curve->scale_rect[0], 20);
  // Draw attribute name
  grow_CreateGrowText( curve->grownames_ctx, "", Lng::translate("Time axis"),
		       14.0, (curve->cd->cols-0.2) + 0.75, glow_eDrawType_TextHelveticaBold, 
		       glow_eDrawType_Line, 2, glow_mDisplayLevel_1, NULL, &t1);
  grow_SetAnnotation( curve->cursor_annot[0], 0, "0", 1);
  grow_SetAnnotation( curve->mark_annot[0], 0, "0", 1);

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
         2 * cd->trend_lines[idx] - 1, 1, 10, 0, "%1.0f");

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
         2 * cd->trend_lines[i] - 1, 2, 2, 0, cd->format[i]);
      x += cd->axis_width[i];
    }
  }
  axis_window_width = max( x, 1);

  double zoom_x, zoom_y;
  int width;

  curve_GetZoom( growcurve_ctx, &zoom_x, &zoom_y);
  grow_ZoomY( growaxis_ctx, zoom_y);
  grow_ZoomX( growaxis_ctx, zoom_y);

  if ( axis_displayed) {
    width = int( zoom_y * axis_window_width);
    axis_set_width( width);
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
		      cd->trend_lines[0] - 2,
		      2 * cd->trend_lines[idx] - 3);

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
	 cd->axis_lines[0],
         cd->axis_linelongq[0], cd->axis_valueq[0], 270, cd->format[0]);

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

  // Simulate cursormotion and slidermoved event to update markvalues
  glow_sEvent e;
  e.any.event = glow_eEvent_CursorMotion;
  e.any.x = last_cursor_x;
  growcurve_cb( growcurve_ctx, &e);
  e.any.event = glow_eEvent_SliderMoved;
  e.any.x = last_mark_x;
  growcurve_cb( growcurve_ctx, &e);

}

int GeCurve::read_file( char *filename)
{
  FILE *fp;
  char line[10000];
  char item_str[CURVE_MAX_COLS][80];
  int nr;
  int rows = 0;
  pwr_tFileName fname;
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

void GeCurve::set_time( pwr_tTime time)
{
  char time_str[40];
  char full_title[500];

  time_AtoAscii( &time, time_eFormat_DateAndTime, time_str, sizeof(time_str));

  sprintf( full_title, "%s   %s", title, time_str);
  write_title( full_title);
}

void GeCurve::set_title( char *str)
{
  strcpy( title, str);
  write_title( str);
}

GeCurve::~GeCurve()
{
}

GeCurve::GeCurve( void 	*gc_parent_ctx, 
		  char	*curve_name,
		  char  *filename,
                  GeCurveData *curve_data,
                  int   pos_right) :
  parent_ctx(gc_parent_ctx), growcurve_ctx(0), background_dark(glow_eDrawType_Color29),
  background_bright(glow_eDrawType_Color21),
  border_dark(glow_eDrawType_Color28),
  border_bright(glow_eDrawType_Color22),
  cd(0), axis_window_width(0), auto_refresh(1), axis_displayed(1),
  minmax_idx(0), close_cb(0), help_cb(0), initial_right_position(pos_right),
  last_cursor_x(0), last_mark_x(0), deferred_configure_axes(0)
{
  pwr_tStatus sts;

  memset( hide, 0, sizeof(hide));
  memset( name_rect, 0, sizeof(name_rect));
  memset( hide_rect, 0, sizeof(hide_rect));
  memset( scale_rect, 0, sizeof(scale_rect));
  memset( hide_l1, 0, sizeof(hide_l1));
  memset( hide_l2, 0, sizeof(hide_l2));
  memset( mark_annot, 0, sizeof(cursor_annot));
  memset( mark_annot, 0, sizeof(mark_annot));
  curve_color = background_dark;
  curve_border = border_dark;
  for ( int i = TREND_MAX_CURVES; i < CURVE_MAX_COLS; i++)
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
}

GeCurveData::GeCurveData( curve_eDataType datatype) :
  type(datatype), rows(0), cols(0), x_reverse(0)
{
  memset( data, 0, sizeof(data));
  for ( int i = 0; i < CURVE_MAX_COLS; i++) {
    strcpy( unit[i], "");
    max_value[i] = 0;
    min_value[i] = 0;
    min_value_axis[i] = 0;
    max_value_axis[i] = 0;
    trend_lines[i] = 0;
    axis_lines[i] = 0;
    axis_linelongq[i] = 0;
    axis_valueq[i] = 0;
    
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
       &min_value_axis[i], &max_value_axis[i], &trend_lines[i], &axis_lines[i], 
       &axis_linelongq[i], &axis_valueq[i], format[i], 
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
     int *trend_lines, int *axis_lines, int *axis_linelongq, int *axis_valueq, 
     char *format, double *axis_width, int not_zero, int allow_odd)
{
  double value, maxval, minval;
  int i_value;
  int n, max_n, min_n;
  int min_lines, max_lines;
  int min_zero, max_zero;
  int format_int, format_dec;
  int format_type  = 0;
  int trendlinequot = 2;
  int axlinequot = 2;
  int axvaluequot = 2;
  
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
      else if ( max_value - min_value < 1000) {
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
      else if ( max_value - min_value < 3000) {
        i_value = int(max_value/50) * 50 + 50;
        maxval = i_value;
        max_lines = i_value / 50;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/50) * 50 - 50;
        minval = i_value;
        min_lines = abs(i_value) / 50;
      }
      else if ( max_value - min_value < 10000) {
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
      else if ( max_value - min_value < 30000) {
        i_value = int(max_value/600) * 600 + 600;
        maxval = i_value;
        max_lines = i_value / 60;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/60) * 60 - 60;
        minval = i_value;
        min_lines = abs(i_value) / 60;
	format_type = 1;
	trendlinequot = 2;
	axlinequot = 10;
	axvaluequot = 10;
      }
      else if ( max_value - min_value < 60000) {
        i_value = int(max_value/600) * 600 + 600;
        maxval = i_value;
        max_lines = i_value / 600;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/600) * 600 - 600;
        minval = i_value;
        min_lines = abs(i_value) / 600;
	axlinequot = 6;
	axvaluequot = 2;
	format_type = 1;
      }
      else if ( max_value - min_value < 140000) {
        i_value = int(max_value/600) * 600 + 600;
        maxval = i_value;
        max_lines = i_value / 600;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/600) * 600 - 600;
        minval = i_value;
        min_lines = abs(i_value) / 600;
	axlinequot = 6;
	axvaluequot = 12;
	format_type = 1;
      }
      else {
        i_value = int(max_value/3600) * 3600 + 3600;
        maxval = i_value;
        max_lines = i_value / 3600;

        if ( fabs( min_value) < DBL_EPSILON)
          i_value = 0;
        else
          i_value = int(min_value/3600) * 3600 - 3600;
        minval = i_value;
        min_lines = abs(i_value) / 3600;
	axlinequot = 2 * int((max_value - min_value)/140000);
	axvaluequot = 2 * int((max_value - min_value)/140000);
	format_type = 2;
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

        i_value = int( min_value * pow(10, -min_n)) + ((min_value < 0) ? -1 : 1);
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
  *trend_lines = abs(max_lines + min_lines) + 1;
  *axis_lines = (*trend_lines - 1) * trendlinequot + 1;
  *axis_linelongq = axlinequot;
  *axis_valueq = axvaluequot;

  switch ( format_type) {
  case 0:
    // Float format
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
    break;
  case 1:
    // Hour and minute format
    strcpy( format, "%2t");
    *axis_width = 0.65 * format_int + 0.4;
    break;
  case 2:
    // Days and hour format
    strcpy( format, "%3t");
    *axis_width = 0.65 * format_int + 0.4;
    break;
  }
  printf( "%f	%f	%f	%f	%3d %5s %4.1f\n", 
              min_value, max_value, 
              *min_value_axis, *max_value_axis, *trend_lines, format, *axis_width);
  return;
}

