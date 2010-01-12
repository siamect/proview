/* 
 * Proview   $Id: xtt_fast.cpp,v 1.10 2008-10-31 12:51:36 claes Exp $
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
 */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "cow_wow.h"
#include "rt_xnav_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_curvectx.h"
#include "glow_curveapi.h"

#include "xtt_xnav.h"
#include "rt_fast.h"
#include "xtt_fast.h"
#include "ge_curve.h"


XttFast::XttFast( void *parent_ctx,
		  char *name,
		  pwr_sAttrRef *fast_arp,
		  int *sts) :
  xnav(parent_ctx), fast_cnt(0), timerid(0), close_cb(0), help_cb(0), first_scan(1),
  axis_configured(false)
{
  pwr_sAttrRef aref = pwr_cNAttrRef;
  pwr_tAName fast_name;
  pwr_tAName attr_name;
  pwr_sClass_DsFastCurve fp;
  int i, j;
    
  *sts = XNAV__SUCCESS;

  // Get current status of the fast objects
  i = 0;
  aref = *fast_arp;
  if ( cdh_ObjidIsNull( aref.Objid)) {
    *sts = XNAV__FASTCONFIG;
    return;
  }

  *sts = gdh_AttrrefToName( &aref, fast_name, sizeof(fast_name), 
			  cdh_mNName);
  if (EVEN(*sts)) return;

  *sts = gdh_GetObjectInfo( fast_name, &fp, sizeof(fp));
  if ( EVEN(*sts)) return;

  max_points = fp.NoOfPoints;
  fast_function = fp.Function;

  strcpy( attr_name, fast_name);
  strcat( attr_name, ".TriggTime");
  gdh_NameToAttrref( pwr_cNObjid, attr_name, &trigg_time_attr);

  if ( fast_function & fast_mFunction_BeforeTrigg) {
    strcpy( attr_name, fast_name);
    strcat( attr_name, ".TriggIndex");
    gdh_NameToAttrref( pwr_cNObjid, attr_name, &trigg_index_attr);

    strcpy( attr_name, fast_name);
    strcat( attr_name, ".FirstIndex");
    gdh_NameToAttrref( pwr_cNObjid, attr_name, &first_index_attr);

    strcpy( attr_name, fast_name);
    strcat( attr_name, ".LastIndex");
    gdh_NameToAttrref( pwr_cNObjid, attr_name, &last_index_attr);
  }

  // Create data for time axis
  gcd = new GeCurveData( curve_eDataType_DsTrend);
  gcd->data[0] = (double *) malloc( 8 * max_points);
  strcpy( gcd->name[0], "Time");
  gcd->axis_type[0] = curve_eAxis_x;
  memcpy( &time_buff, &fp.TimeBuffer, sizeof(time_buff));

  fast_cnt = 0;
  for ( i = 0; i < FAST_CURVES; i++) {
    if ( fp.CurveValid[i]) {
      gcd->data[fast_cnt + 1] = (double *) calloc( 1, 8 * max_points);
      gcd->axis_type[fast_cnt + 1] = curve_eAxis_y;
      memcpy( &buff[fast_cnt], &fp.Buffers[i], sizeof(buff[0]));
      type[fast_cnt] = (pwr_eType) fp.AttributeType[i];
      fast_idx[i] = fast_cnt + 1;
      curve_idx[fast_cnt + 1] = i;

      *sts = gdh_AttrrefToName( &fp.Attribute[i], attr_name, 
			    sizeof(attr_name), cdh_mNName);
      if (EVEN(*sts)) continue;
      strcpy( gcd->name[fast_cnt + 1], attr_name);
      fast_cnt++;
    }
  }

  // Subscribe to object
  strcpy( attr_name, fast_name);
  strcat( attr_name, ".New");
  *sts = gdh_RefObjectInfo( attr_name, (pwr_tAddress *)&new_p, 
			    &new_subid, sizeof(pwr_tBoolean));
  if ( EVEN(*sts)) return;

  for ( i = 0; i < fast_cnt; i++) {
    switch( type[i]) {
      case pwr_eType_Float32:
      case pwr_eType_Int32:
      case pwr_eType_UInt32:
        element_size[i] = 4;
        break;
      case pwr_eType_Float64:
      case pwr_eType_Int64:
      case pwr_eType_UInt64:
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
  gcd->cols = fast_cnt + 1;
  gcd->rows = max_points;

  axis_configured = true;
  for ( i = 0; i < FAST_CURVES; i++) {
    if ( fp.CurveValid[i]) {
      j = fast_idx[i];
      if ( fp.YMinValue[i] != fp.YMaxValue[i])
	gcd->scale( gcd->axis_type[j], gcd->value_type[j],
		    fp.YMinValue[i],  fp.YMaxValue[i],
		    &gcd->min_value_axis[j], &gcd->max_value_axis[j],
		    &gcd->trend_lines[j], &gcd->axis_lines[j], &gcd->axis_linelongq[j], 
		    &gcd->axis_valueq[j], gcd->format[j], &gcd->axis_width[j], 1, 1);
      else
	axis_configured = false;
    }
  }


  if ( fp.Active)
    first_scan = 0;
  fast_scan( this);
  gcd->select_color( 0);

  if ( strcmp( fp.Title, "") != 0)
    strcpy( title, fp.Title);
  else
    cdh_StrncpyCutOff( title, name, sizeof(title), 1);

  // timerid = XtAppAddTimeOut(
  //	XtWidgetToApplicationContext(parent_widget), 1000,
  //	(XtTimerCallbackProc)fast_scan, this);
}

XttFast::~XttFast()
{
}

void XttFast::pop()
{
  curve->pop();
}

void XttFast::fast_close_cb( void *ctx)
{
  XttFast *fast = (XttFast *) ctx;

  if ( fast->close_cb)
    (fast->close_cb)( fast->xnav, fast);
  else
    delete fast;
}

void XttFast::fast_help_cb( void *ctx)
{
  XttFast *fast = (XttFast *) ctx;

  if ( fast->help_cb)
    (fast->help_cb)( fast->xnav, "opg_fastcurves");
}

void XttFast::fast_scan( void *data)
{
  XttFast *fast = (XttFast *)data;
  int i, j, k;
  pwr_tStatus sts;
  int trigg_index, first_index, last_index;

  // Check if any new value
  if ( (*fast->new_p && !fast->old_new) || fast->first_scan) {

    // Update curves
    if ( fast->fast_function & fast_mFunction_BeforeTrigg) {

      // Get first, last and trigg index
      sts = gdh_GetObjectInfoAttrref( &fast->trigg_index_attr, &trigg_index,
				      sizeof(trigg_index));
      if ( EVEN(sts)) return;
      sts = gdh_GetObjectInfoAttrref( &fast->first_index_attr, &first_index,
				      sizeof(first_index));
      if ( EVEN(sts)) return;
      sts = gdh_GetObjectInfoAttrref( &fast->last_index_attr, &last_index,
				      sizeof(last_index));
      if ( EVEN(sts)) return;

       // Read into temporary buffer
      pwr_tFloat32 *tmp = (pwr_tFloat32 *)calloc( fast->max_points, 4);
      sts = gdh_GetObjectInfoAttrref( &fast->time_buff, tmp,
				      fast->max_points * 4);
      if ( EVEN(sts)) return;

      k = first_index;
      for ( j = 0; j < fast->max_points; j++) {
	if ( k >= fast->max_points)
	  k = 0;
	fast->gcd->data[0][j] = tmp[k] - tmp[trigg_index];
	if ( k == last_index)
	  break;
	k++;
      }
      // If to few points, fill with dummy data
      for ( ; j < fast->max_points; j++) {
	fast->gcd->data[0][j] = tmp[k] - tmp[trigg_index];
      }
      free( tmp);
    }
    else {
      pwr_tFloat32 *tmp = (pwr_tFloat32 *)calloc( fast->max_points, 4);
      sts = gdh_GetObjectInfoAttrref( &fast->time_buff, tmp,
				      fast->max_points * 4);
      if ( EVEN(sts)) return;

      for ( j = 0; j < fast->max_points; j++)
	fast->gcd->data[0][j] = tmp[j];
      free( tmp);
    }
    for ( i = 0; i < fast->fast_cnt; i++) {
      if ( fast->fast_function & fast_mFunction_BeforeTrigg) {
	// Read into temporary buffer
	void *tmp = calloc( fast->max_points, fast->element_size[i]);
	sts = gdh_GetObjectInfoAttrref( &fast->buff[i], tmp, 
		     fast->max_points * fast->element_size[i]);
	if ( EVEN(sts)) return;

	k = first_index;
	for ( j = 0; j < fast->max_points; j++) {
	  if ( k >= fast->max_points)
	    k = 0;
	  switch( fast->type[i]) {
	  case pwr_eType_Float32:
	    fast->gcd->data[i + 1][j] = ((pwr_tFloat32 *)tmp)[k];
	    break;
	  case pwr_eType_Float64:
	    fast->gcd->data[i + 1][j] = ((pwr_tFloat64 *)tmp)[k];
	    break;
	  case pwr_eType_Int64:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt64 *)tmp)[k];
	    break;
	  case pwr_eType_UInt64:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt64 *)tmp)[k];
	    break;
	  case pwr_eType_Int32:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt32 *)tmp)[k];
	    break;
	  case pwr_eType_UInt32:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt32 *)tmp)[k];
	    break;
	  case pwr_eType_Int16:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt16 *)tmp)[k];
	    break;
	  case pwr_eType_UInt16:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt16 *)tmp)[k];
	    break;
	  case pwr_eType_Int8:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt8 *)tmp)[k];
	    break;
	  case pwr_eType_UInt8:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt8 *)tmp)[k];
	    break;
	  default:
	    ;
	  }
	  if ( k == last_index)
	    break;
	  k++;
	}
	// If to few points, fill with 0
	for ( ; j < fast->max_points; j++) {
	  switch( fast->type[i]) {
	  case pwr_eType_Float32:
	    fast->gcd->data[i + 1][j] = ((pwr_tFloat32 *)tmp)[k];
	    break;
	  case pwr_eType_Float64:
	    fast->gcd->data[i + 1][j] = ((pwr_tFloat64 *)tmp)[k];
	    break;
	  case pwr_eType_Int64:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt64 *)tmp)[k];
	    break;
	  case pwr_eType_UInt64:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt64 *)tmp)[k];
	    break;
	  case pwr_eType_Int32:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt32 *)tmp)[k];
	    break;
	  case pwr_eType_UInt32:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt32 *)tmp)[k];
	    break;
	  case pwr_eType_Int16:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt16 *)tmp)[k];
	    break;
	  case pwr_eType_UInt16:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt16 *)tmp)[k];
	    break;
	  case pwr_eType_Int8:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt8 *)tmp)[k];
	    break;
	  case pwr_eType_UInt8:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt8 *)tmp)[k];
	    break;
	  default: ;
	  }
	}
	free(tmp);
      }
      else {
	void *tmp = calloc( fast->max_points, fast->element_size[i]);
	sts = gdh_GetObjectInfoAttrref( &fast->buff[i], tmp, 
	      fast->max_points * fast->element_size[i]);
	if ( EVEN(sts)) return;

	for ( j = 0; j < fast->max_points; j++) {
	  switch( fast->type[i]) {
	  case pwr_eType_Float32:
	    fast->gcd->data[i + 1][j] = ((pwr_tFloat32 *)tmp)[j];
	    break;
	  case pwr_eType_Float64:
	    fast->gcd->data[i + 1][j] = ((pwr_tFloat64 *)tmp)[j];
	    break;
	  case pwr_eType_Int64:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt64 *)tmp)[j];
	    break;
	  case pwr_eType_UInt64:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt64 *)tmp)[j];
	    break;
	  case pwr_eType_Int32:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt32 *)tmp)[j];
	    break;
	  case pwr_eType_UInt32:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt32 *)tmp)[j];
	    break;
	  case pwr_eType_Int16:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt16 *)tmp)[j];
	    break;
	  case pwr_eType_UInt16:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt16 *)tmp)[j];
	    break;
	  case pwr_eType_Int8:
	    fast->gcd->data[i + 1][j] = ((pwr_tInt8 *)tmp)[j];
	    break;
	  case pwr_eType_UInt8:
	    fast->gcd->data[i + 1][j] = ((pwr_tUInt8 *)tmp)[j];
	    break;
	  default: ;
	  }
	}
	free(tmp);
      }
    }

    // Get trigg time
    pwr_tTime trigg_time;
    sts = gdh_GetObjectInfoAttrref( &fast->trigg_time_attr, &trigg_time,
				      sizeof(trigg_time));
    if ( EVEN(sts)) return;

    if ( !fast->axis_configured) {
      fast->gcd->get_borders();
      fast->gcd->get_default_axis();
      if ( !fast->first_scan) {
	fast->curve->configure_curves();
	fast->curve->configure_axes();
	fast->curve->set_time( trigg_time);
      }
    }
    else {
      fast->gcd->get_borders();
      // fast->gcd->get_default_axis();
      fast->gcd->scale( fast->gcd->axis_type[0], fast->gcd->value_type[0],
		    fast->gcd->min_value[0],  fast->gcd->max_value[0],
		    &fast->gcd->min_value_axis[0], &fast->gcd->max_value_axis[0],
		    &fast->gcd->trend_lines[0], &fast->gcd->axis_lines[0], 
		    &fast->gcd->axis_linelongq[0], 
		    &fast->gcd->axis_valueq[0], fast->gcd->format[0],
		    &fast->gcd->axis_width[0], 1, 1);
      if ( !fast->first_scan) {
	fast->curve->configure_curves();
	// fast->curve->configure_axes();
	fast->curve->set_time( trigg_time);
      }
    }
    fast->first_scan = 0;
  }
  fast->old_new = *fast->new_p;


  if ( fast->timerid)
    fast->timerid->add( 1000, fast_scan, fast);
}







