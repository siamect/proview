/* rt_xtt.cpp -- Display plant and node hiererachy

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_xnav_msg.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"
#include "glow_curvectx.h"
#include "glow_curveapi.h"
#include "glow_curvewidget.h"

#include "xtt_xnav.h"
#include "xtt_trend.h"

extern "C" {
#include "flow_x.h"
}

static void trend_close_cb( void *ctx);
static void trend_scan( XttTrend *trend);

XttTrend::XttTrend(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	Widget *w,
        pwr_tObjid *trend_list,
        pwr_tObjid plotgroup,
        int *sts) :
  xnav(parent_ctx), parent_widget(parent_wid), trend_cnt(0), close_cb(0)
{
  pwr_tObjid *objid_list;
  pwr_tObjid *objid_p;
  pwr_tObjid plot_trends[XTT_TREND_MAX];
  pwr_sClass_PlotGroup plot;
  char trend_name[XTT_TREND_MAX][120];
  char object_name[XTT_TREND_MAX][120];
  pwr_sClass_DsTrend tp[XTT_TREND_MAX];
  char plot_name[120];
  int i, j, k;
  int start_idx;
  int time;
  int trend_buff_size = (int) sizeof( trend_p[0]->DataBuffer) /
                          sizeof( trend_p[0]->DataBuffer[0]);
    
  *sts = XNAV__SUCCESS;


  if ( trend_list) {
    // List of trend objects as input
    objid_list = trend_list;
  }
  else {
    // Plotgroup as input
    *sts = gdh_ObjidToName( plotgroup, plot_name, sizeof(plot_name), 
                cdh_mNName);
    if (EVEN(*sts)) return;

    *sts = gdh_GetObjectInfo( plot_name, &plot, sizeof(plot));
    if ( EVEN(*sts)) return;

    for ( i = 0; i < 20; i++) {
      if ( cdh_ObjidIsNull( plot.YObjectName[i]))
        break;
      plot_trends[i] = plot.YObjectName[i];
    }
    plot_trends[i] = pwr_cNObjid;
    objid_list = plot_trends;
  }

  // Get current status of the trend objects
  i = 0;
  objid_p = objid_list;
  while ( cdh_ObjidIsNotNull( *objid_p)) {
    *sts = gdh_ObjidToName( *objid_p, trend_name[i], sizeof(trend_name[0]), 
                cdh_mNName);
    if (EVEN(*sts)) return;

    *sts = gdh_GetObjectInfo( trend_name[i], &tp[i], sizeof(tp[0]));
    if ( EVEN(*sts)) return;

    *sts = gdh_AttrrefToName( &tp[i].DataName, object_name[i], 
                sizeof(object_name[0]), cdh_mNName);
    if (EVEN(*sts)) return;

    i++;
    objid_p++;
  }
  trend_cnt = i;

  if ( trend_cnt == 0) {
   *sts = XNAV__TRENDCONFIG;
    return;
  }

  // Calculate number of points
  max_time = 0;
  min_interval = 100000;
  for ( i = 0; i < trend_cnt; i++) {
    time = tp[i].Multiple * tp[i].ScanTime * tp[i].NoOfBuffers *
           tp[i].NoOfSample;
    if ( time > max_time)
      max_time = time;

    if ( (int) (tp[i].Multiple * tp[i].ScanTime) < min_interval) {
      min_interval = tp[i].Multiple * tp[i].ScanTime;
      min_interval_idx = i;
    }
  }

  if ( min_interval == 0) {
   *sts = XNAV__TRENDCONFIG;
    return;
  }

  max_points = max_time / min_interval;

  for ( i = 0; i < trend_cnt; i++) {
    interval[i] = tp[i].Multiple * tp[i].ScanTime / min_interval;
  }

  // Create data for time axis
  gcd = new GeCurveData( curve_eDataType_DsTrend);
  gcd->data[0] = (double *) malloc( 8 * max_points);
  strcpy( gcd->name[0], "Time");
  for ( j = 0; j < max_points; j++) {
    gcd->data[0][j] = double( j * min_interval);
  }
  gcd->axis_type[0] = curve_eAxis_x;

  for ( i = 0; i < trend_cnt; i++) {
    gcd->data[i+1] = (double *) calloc( 1, 8 * max_points);

    int write_buffer = (int) tp[i].WriteBuffer;
    start_idx = write_buffer * trend_buff_size / 2
              + int( tp[i].NextWriteIndex[write_buffer]);
    if ( start_idx == 0) {
      start_idx = tp[i].NoOfSample - 1 + trend_buff_size / 2;
      write_buffer = 1;
    }
    else if ( start_idx == trend_buff_size / 2) {
      start_idx = tp[i].NoOfSample - 1;
      write_buffer = 0;
    }
    else
      start_idx--;

    int idx = 0;
    for ( j = start_idx; j >= write_buffer * trend_buff_size/2; j--) {
      for ( k = 0; k < interval[i]; k++) {
        gcd->data[i+1][idx] = tp[i].DataBuffer[j];
        idx++;
      }
    }
    for ( j = tp[i].NoOfSample - 1 + (!write_buffer) * trend_buff_size/2;
          j >= (!write_buffer) * trend_buff_size/2; j--) {
      for ( k = 0; k < interval[i]; k++) {
        gcd->data[i+1][idx] = tp[i].DataBuffer[j];
        idx++;
      }
    }
    if ( start_idx != (int) tp[i].NoOfSample - 1 + 
                   write_buffer * trend_buff_size/2) {
      for ( j = tp[i].NoOfSample - 1 + write_buffer * trend_buff_size/2; 
            j > start_idx; j--) {
        for ( k = 0; k < interval[i]; k++) {
          gcd->data[i+1][idx] = tp[i].DataBuffer[j];
          idx++;
        }
      }
    }
    last_buffer[i] = tp[i].WriteBuffer;
    last_next_index[i] = tp[i].NextWriteIndex[last_buffer[i]];
    gcd->axis_type[i+1] = curve_eAxis_y;
  }

  // Subscribe to object
  for ( i = 0; i < trend_cnt; i++) {

    *sts = gdh_RefObjectInfo( trend_name[i], (pwr_tAddress *)&trend_p[i], 
                      &subid[i], sizeof(pwr_sClass_DsTrend));
    if ( EVEN(*sts)) return;

    strcpy( gcd->name[i+1], object_name[i]);
    
    switch( trend_p[i]->DataType) {
      case pwr_eType_Float32:
      case pwr_eType_Int32:
      case pwr_eType_UInt32:
        element_size[i] = 4;
        break;
      case pwr_eType_Float64:
        element_size[i] = 8;
        break;
      case pwr_eType_Int16:
      case pwr_eType_UInt16:
        element_size[i] = 2;
        break;
      case pwr_eType_Int8:
      case pwr_eType_UInt8:
        element_size[i] = 1;
        break;
      default:
        element_size[i] = 4;
    }
  }
  gcd->cols = trend_cnt + 1;
  gcd->rows = max_points;
  gcd->x_reverse = 1;
  gcd->get_borders();
  gcd->get_default_axis();
  gcd->select_color( 0);

  if ( !trend_list) {
    // Use axis values from plotgroup object
    for ( i = 0; i < trend_cnt; i++) {
      if ( plot.YMinValue[i] != plot.YMaxValue[i])
        gcd->scale( gcd->axis_type[i+1], gcd->value_type[i+1], 
          plot.YMinValue[i],  plot.YMaxValue[i], 
          &gcd->min_value_axis[i+1], &gcd->max_value_axis[i+1], 
          &gcd->lines[i+1], gcd->format[i+1], 
          &gcd->axis_width[i+1], 1, 1);
    }
  }

  curve = new GeCurve( this, parent_widget, name, NULL, gcd, 1);
  curve->close_cb = trend_close_cb;

  timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(parent_widget), 1000,
	(XtTimerCallbackProc)trend_scan, this);
}

XttTrend::~XttTrend()
{
  XtRemoveTimeOut( timerid);

  for ( int i = 0; i < trend_cnt; i++) {
    gdh_UnrefObjectInfo( subid[i]);
  }
  delete curve;
}

void XttTrend::pop()
{
  curve->pop();
}

static void trend_close_cb( void *ctx)
{
  XttTrend *trend = (XttTrend *) ctx;

  if ( trend->close_cb)
    (trend->close_cb)( trend->xnav, trend);
  else
    delete trend;
}

static void trend_scan( XttTrend *trend)
{
  int i, j, k;
  int write_buffer;
  int idx;
  int values;
  int trend_buff_size = (int) sizeof( trend->trend_p[0]->DataBuffer) /
                          sizeof( trend->trend_p[0]->DataBuffer[0]);

  // Check if any new value
  i = trend->min_interval_idx;
  if ( trend->trend_p[i]->NextWriteIndex[trend->trend_p[i]->WriteBuffer] !=
          trend->last_next_index[i]) {
    values = trend->trend_p[i]->NextWriteIndex[trend->trend_p[i]->WriteBuffer]
          - trend->last_next_index[i];
    if ( values < 0)
      values = values + trend->trend_p[i]->NoOfSample;

    trend->last_next_index[i] =
      trend->trend_p[i]->NextWriteIndex[trend->trend_p[i]->WriteBuffer];

    for ( k = 0; k < values; k++) {
      // Add new points
      for ( i = 0; i < trend->trend_cnt; i++) {
        // Shift data
        for ( j = trend->max_points - 1; j > 0; j--)
          trend->gcd->data[i+1][j] = trend->gcd->data[i+1][j-1];
        // Insert new value
        write_buffer = trend->trend_p[i]->WriteBuffer;
        idx = write_buffer * trend_buff_size / 2
              + int( trend->trend_p[i]->NextWriteIndex[write_buffer]);
        if ( idx == 0 || idx == trend_buff_size/2)
          idx = trend->trend_p[i]->NoOfSample - 1 + (!write_buffer) *
                 trend_buff_size/2;
        else
          idx--;
        trend->gcd->data[i+1][0] = trend->trend_p[i]->DataBuffer[idx];
      }
      trend->curve->points_added();
    }
  }

  trend->timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(trend->parent_widget), 
        1000,
	(XtTimerCallbackProc)trend_scan, trend);
}







