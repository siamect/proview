/* 
 * Proview   $Id: glow_growtrend.cpp,v 1.10 2008-11-20 10:30:44 claes Exp $
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


#include <iostream>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growtrend.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

//! Constuctor
/*!
  \param glow_ctx 	The glow context.
  \param name		Name (max 31 char).
  \param x		x coordinate for position.
  \param y		y coordinate for position.
  \param w		Width.
  \param h		Height.
  \param border_d_type Border color.
  \param line_w	Linewidth of border.
  \param display_lev	Displaylevel when this object is visible.
  \param fill_rect	Rectangle is filled.
  \param display_border Border is visible.
  \param fill_d_type	Fill color.
  \param nodraw	Don't draw the object now.
*/
GrowTrend::GrowTrend( GrowCtx *glow_ctx, const char *name, double x, double y, 
		double w, double h, glow_eDrawType border_d_type, int line_w, 
		glow_mDisplayLevel display_lev,
		int fill_rect, int display_border, 
		glow_eDrawType fill_d_type, int nodraw) : 
		GrowRect(glow_ctx,name,x,y,w,h,border_d_type,line_w,0,
		display_lev,fill_rect,display_border,0,fill_d_type,nodraw),
		horizontal_lines(0), vertical_lines(0), fill_curve(0),
		no_of_points(100), curve_width(1),
		curve_cnt(0), scan_time(1), user_data(0)
{
  for ( int i = 0; i < TREND_MAX_CURVES; i++) { 
    y_min_value[i] = 0;
    y_max_value[i]= 100;
    x_min_value[i] = 0;
    x_max_value[i]= 100;
    curve[i] = 0;
    curve_drawtype[i] = glow_eDrawType_Inherit;
    curve_fill_drawtype[i] = glow_eDrawType_Inherit;
  }

  configure_curves();
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);

}

//! Destructor
/*! Remove the object from context, and erase it from the screen.
 */

GrowTrend::~GrowTrend()
{
  if ( !ctx->nodraw) {
    erase( &ctx->mw);
    erase( &ctx->navw);
  }
  for ( int i = 0; i < curve_cnt; i++)
    delete curve[i];
}

//! Configure the curves
/*! Calculate position of the points of the curves and create a polyline for each curve.
 */
void GrowTrend::configure_curves() 
{
  glow_eDrawType dt, dt_fill;
  int points;
  glow_sPoint *pointarray;
  glow_sPoint *point_p;
  int	i;


  if ( 0 /* strcmp( trace.data[1], "") == 0 */ )
    curve_cnt = 1;
  else
    curve_cnt = 2;

  no_of_points = max( 2, no_of_points);
  points = no_of_points;
  if ( fill_curve)
    points += 2;
  curve_width = min( DRAW_TYPE_SIZE, max( 1, curve_width));

  pointarray = (glow_sPoint *) calloc( points, sizeof(glow_sPoint));
  point_p = pointarray;
  for ( i = 0; i < points; i++) {
    if ( !fill_curve) {
      point_p->y = ur.y;
      point_p->x = ur.x - i * (ur.x - ll.x) / (points - 1);
    }
    else {
      point_p->y = ur.y;
      if ( i == 0)
        point_p->x = ur.x;
      else if ( i == points - 1)
        point_p->x = ll.x;
      else
        point_p->x = ur.x - (i - 1) * (ur.x - ll.x) / (points - 3);
    }
    point_p++;
  }

  for ( i = 0; i < curve_cnt; i++) {
    ctx->nodraw++;
    if ( curve[i])
      delete curve[i];
    ctx->nodraw--;

    if ( curve_drawtype[i] != glow_eDrawType_Inherit)
      dt = curve_drawtype[i];
    else
      dt = draw_type;

    if ( curve_fill_drawtype[i] != glow_eDrawType_Inherit)
      dt_fill = curve_fill_drawtype[i];
    else
      dt_fill = draw_type;

    ctx->nodraw++;
    curve[i] = new GrowPolyLine( ctx, "", pointarray, points, dt, 
	curve_width,
	0, fill_curve, 1, 0, dt_fill);
    ctx->nodraw--;
  }
  free( (char *) pointarray);
}

//! Save the content of the object to file.
/*!
  \param fp	Ouput file.
  \param mode	Not used.
*/
void GrowTrend::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowTrend) << endl;
  fp << int(glow_eSave_GrowTrend_y_max_value_0) << FSPACE << y_max_value[0] << endl;
  fp << int(glow_eSave_GrowTrend_y_max_value_1) << FSPACE << y_max_value[1] << endl;
  fp << int(glow_eSave_GrowTrend_y_min_value_0) << FSPACE << y_min_value[0] << endl;
  fp << int(glow_eSave_GrowTrend_y_min_value_1) << FSPACE << y_min_value[1] << endl;
  fp << int(glow_eSave_GrowTrend_x_max_value_0) << FSPACE << x_max_value[0] << endl;
  fp << int(glow_eSave_GrowTrend_x_max_value_1) << FSPACE << x_max_value[1] << endl;
  fp << int(glow_eSave_GrowTrend_x_min_value_0) << FSPACE << x_min_value[0] << endl;
  fp << int(glow_eSave_GrowTrend_x_min_value_1) << FSPACE << x_min_value[1] << endl;
  fp << int(glow_eSave_GrowTrend_horizontal_lines) << FSPACE << horizontal_lines << endl;
  fp << int(glow_eSave_GrowTrend_vertical_lines) << FSPACE << vertical_lines << endl;
  fp << int(glow_eSave_GrowTrend_fill_curve) << FSPACE << fill_curve << endl;
  fp << int(glow_eSave_GrowTrend_curve_drawtype_0) << FSPACE << int(curve_drawtype[0]) << endl;
  fp << int(glow_eSave_GrowTrend_curve_drawtype_1) << FSPACE << int(curve_drawtype[1]) << endl;
  fp << int(glow_eSave_GrowTrend_curve_fill_drawtype_0) << FSPACE << int(curve_fill_drawtype[0]) << endl;
  fp << int(glow_eSave_GrowTrend_curve_fill_drawtype_1) << FSPACE << int(curve_fill_drawtype[1]) << endl;
  fp << int(glow_eSave_GrowTrend_no_of_points) << FSPACE << no_of_points << endl;
  fp << int(glow_eSave_GrowTrend_curve_width) << FSPACE << curve_width << endl;
  fp << int(glow_eSave_GrowTrend_trace_data1) << FSPACE << trace.data[0] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data2) << FSPACE << trace.data[1] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data3) << FSPACE << trace.data[2] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data4) << FSPACE << trace.data[3] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data5) << FSPACE << trace.data[4] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data6) << FSPACE << trace.data[5] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data7) << FSPACE << trace.data[6] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data8) << FSPACE << trace.data[7] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data9) << FSPACE << trace.data[8] << endl;
  fp << int(glow_eSave_GrowTrend_trace_data10) << FSPACE << trace.data[9] << endl;
  fp << int(glow_eSave_GrowTrend_trace_attr_type) << FSPACE << int(trace.attr_type) << endl;
  fp << int(glow_eSave_GrowTrend_trace_color) << FSPACE << int(trace.color) << endl;
  fp << int(glow_eSave_GrowTrend_access) << FSPACE << (unsigned int)(trace.access) << endl;
  fp << int(glow_eSave_GrowTrend_cycle) << FSPACE << int(trace.cycle) << endl;
  fp << int(glow_eSave_GrowTrend_ref_object) << FSPACE << trace.ref_object << endl;
  fp << int(glow_eSave_GrowTrend_rect_part) << endl;
  GrowRect::save( fp, mode);
  fp << int(glow_eSave_GrowTrend_scan_time) << FSPACE << scan_time << endl;
  if ( user_data && ctx->userdata_save_callback) {
    fp << int(glow_eSave_GrowTrend_userdata_cb) << endl;
    (ctx->userdata_save_callback)(&fp, this, glow_eUserdataCbType_Node);
  }
  fp << int(glow_eSave_End) << endl;
}

//! Read the content of the object from file.
/*!
  \param fp	Input file.
*/
void GrowTrend::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		utmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowTrend: break;
      case glow_eSave_GrowTrend_y_max_value_0: fp >> y_max_value[0]; break;
      case glow_eSave_GrowTrend_y_max_value_1: fp >> y_max_value[1]; break;
      case glow_eSave_GrowTrend_y_min_value_0: fp >> y_min_value[0]; break;
      case glow_eSave_GrowTrend_y_min_value_1: fp >> y_min_value[1]; break;
      case glow_eSave_GrowTrend_x_max_value_0: fp >> x_max_value[0]; break;
      case glow_eSave_GrowTrend_x_max_value_1: fp >> x_max_value[1]; break;
      case glow_eSave_GrowTrend_x_min_value_0: fp >> x_min_value[0]; break;
      case glow_eSave_GrowTrend_x_min_value_1: fp >> x_min_value[1]; break;
      case glow_eSave_GrowTrend_horizontal_lines: fp >> horizontal_lines; break;
      case glow_eSave_GrowTrend_vertical_lines: fp >> vertical_lines; break;
      case glow_eSave_GrowTrend_fill_curve: fp >> fill_curve; break;
      case glow_eSave_GrowTrend_curve_drawtype_0: fp >> 
		tmp; curve_drawtype[0] = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTrend_curve_drawtype_1: fp >> 
		tmp; curve_drawtype[1] = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTrend_curve_fill_drawtype_0: fp >> 
		tmp; curve_fill_drawtype[0] = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTrend_curve_fill_drawtype_1: fp >> 
		tmp; curve_fill_drawtype[1] = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTrend_no_of_points: fp >> no_of_points; break;
      case glow_eSave_GrowTrend_curve_width: fp >> curve_width; break;
      case glow_eSave_GrowTrend_rect_part: 
        GrowRect::open( fp);
        break;
      case glow_eSave_GrowTrend_trace_data1:
        fp.get();
        fp.getline( trace.data[0], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data2:
        fp.get();
        fp.getline( trace.data[1], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data3:
        fp.get();
        fp.getline( trace.data[2], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data4:
        fp.get();
        fp.getline( trace.data[3], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data5:
        fp.get();
        fp.getline( trace.data[4], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data6:
        fp.get();
        fp.getline( trace.data[5], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data7:
        fp.get();
        fp.getline( trace.data[6], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data8:
        fp.get();
        fp.getline( trace.data[7], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data9:
        fp.get();
        fp.getline( trace.data[8], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_data10:
        fp.get();
        fp.getline( trace.data[9], sizeof(trace.data[0]));
        break;
      case glow_eSave_GrowTrend_trace_attr_type: fp >> tmp; trace.attr_type = (glow_eTraceType)tmp; break;
      case glow_eSave_GrowTrend_trace_color: fp >> tmp; trace.color = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowTrend_access: fp >> utmp; trace.access = (glow_mAccess)utmp; break;
      case glow_eSave_GrowTrend_cycle: fp >> tmp; trace.cycle = (glow_eCycle)tmp; break;
      case glow_eSave_GrowTrend_scan_time: fp >> scan_time; break;
      case glow_eSave_GrowTrend_ref_object:
        fp.get();
        fp.getline( trace.ref_object, sizeof(trace.ref_object));
        break;
      case glow_eSave_GrowTrend_userdata_cb:
	if ( ctx->userdata_open_callback)
	  (ctx->userdata_open_callback)(&fp, this, glow_eUserdataCbType_Node);
	break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowTrend:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  configure_curves();
}

//! Draw the objects if any part is inside the drawing area.
/*!
  \param ll_x		Lower left x coordinate of drawing area.
  \param ll_y		Lower left y coordinate of drawing area.
  \param ur_x		Upper right x coordinate of drawing area.
  \param ur_y		Upper right y coordinate of drawing area.
*/
void GrowTrend::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;

  if ( ll_x > ur_x) {
    /* Shift */
    tmp = ll_x;
    ll_x = ur_x;
    ur_x = tmp;
  }
  if ( ll_y > ur_y) {
    /* Shift */
    tmp = ll_y;
    ll_y = ur_y;
    ur_y = tmp;
  }

  if ( x_right * w->zoom_factor_x - w->offset_x + 1 >= ll_x &&
      	x_left * w->zoom_factor_x - w->offset_x <= ur_x &&
       	y_high * w->zoom_factor_y - w->offset_y + 1 >= ll_y &&
       	y_low * w->zoom_factor_y - w->offset_y <= ur_y)
  {
    draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  }
}

//! Draw the objects if any part is inside the drawing area, and extends the drawing area.
/*!
  \param ll_x		Lower left x coordinate of drawing area.
  \param ll_y		Lower left y coordinate of drawing area.
  \param ur_x		Upper right x coordinate of drawing area.
  \param ur_y		Upper right y coordinate of drawing area.
  
  If some part of object is inside the drawing area, and also outside the drawing area,
  the drawingarea is extended so it contains the whole objects.
*/
void GrowTrend::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	tmp;
  int 	obj_ur_x = int( x_right * w->zoom_factor_x) - w->offset_x;
  int	obj_ll_x = int( x_left * w->zoom_factor_x) - w->offset_x;
  int	obj_ur_y = int( y_high * w->zoom_factor_y) - w->offset_y;
  int   obj_ll_y = int( y_low * w->zoom_factor_y) - w->offset_y;


  if ( *ll_x > *ur_x) {
    /* Shift */
    tmp = *ll_x;
    *ll_x = *ur_x;
    *ur_x = tmp;
  }
  if ( *ll_y > *ur_y) {
    /* Shift */
    tmp = *ll_y;
    *ll_y = *ur_y;
    *ur_y = tmp;
  }

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y) {
    draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);

    // Increase the redraw area
    if ( obj_ur_x > *ur_x)
      *ur_x = obj_ur_x;
    if ( obj_ur_y > *ur_y)
      *ur_y = obj_ur_y;
    if ( obj_ll_x < *ll_x)
      *ll_x = obj_ll_x;
    if ( obj_ll_y < *ll_y)
      *ll_y = obj_ll_y;
  }
}

//! Set object highlight.
/*!
  \param on	If 1, set highlight. If 0, reset highlight.
*/
void GrowTrend::set_highlight( int on)
{
  highlight = on;
  draw();
}

//! Draw the object.
/*!
  \param t		Transform of parent node. Can be zero.
  \param highlight	Draw with highlight colors.
  \param hot		Draw as hot, with larger line width.
  \param node		Parent node. Can be zero.
  \param colornode	The node that controls the color of the object. Can be zero.
  
  The object is drawn with border, fill and shadow. If t is not zero, the current tranform is
  multiplied with the parentnodes transform, to give the appropriate coordinates for the drawing.
*/
void GrowTrend::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, 
		      void *colornode)
{
  if ( ctx->nodraw)
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int i;
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  glow_eDrawType drawtype;

  if ( fix_line_width) {
    idx = line_width;
    idx += hot;
    if ( idx < 0)
    {
      erase( w, t, hot, node);
      return;
    }
  }
  else {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( w->zoom_factor_y / w->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t) {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
  }
  else {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);
  if ( fill)
  {
    glow_eGradient grad = gradient;
    if ( gradient == glow_eGradient_No && 
	 (node && ((GrowNode *)node)->gradient != glow_eGradient_No) && !disable_gradient)
      grad = ((GrowNode *)node)->gradient;

    drawtype = ctx->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 highlight, (GrowNode *)colornode, 1);
    if ( grad == glow_eGradient_No)
      ctx->gdraw->fill_rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype);
    else {
      glow_eDrawType f1, f2;
      double rotation;

      if ( t)
	rotation = trf.rot( t);
      else
	rotation = trf.rot();
      if ( gradient_contrast >= 0) {
	f2 = GlowColor::shift_drawtype( drawtype, -gradient_contrast/2, 0);
	f1 = GlowColor::shift_drawtype( drawtype, int(float(gradient_contrast)/2+0.6), 0);
      }
      else {
	f2 = GlowColor::shift_drawtype( drawtype, -int(float(gradient_contrast)/2-0.6), 0);
	f1 = GlowColor::shift_drawtype( drawtype, gradient_contrast/2, 0);
      }
      ctx->gdraw->gradient_fill_rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, f1, f2, ctx->gdraw->gradient_rotate( rotation, grad));
    }
  }
  drawtype = ctx->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);

  if ( fill_curve)
  {
    for ( i = 0; i < curve_cnt; i++) {
      if ( curve[i])
	curve[i]->border = 0;
    }
    if ( t) {
      GlowTransform tmp = *t * trf;
      for ( i = 0; i < curve_cnt; i++)
      {
        if ( curve[i])
          curve[i]->draw( w, &tmp, highlight, hot, node, colornode);
      }
    }
    else {
      for ( i = 0; i < curve_cnt; i++)
      {
        if ( curve[i])
          curve[i]->draw( w, &trf, highlight, hot, node, colornode);
      }
    }
    for ( i = 0; i < curve_cnt; i++) {
      if ( curve[i])
	curve[i]->border = 1;
    }
  }

  for ( i = 0; i < vertical_lines; i++) {
    int x = int( ll_x + double(ur_x - ll_x) / (vertical_lines + 1) * (i + 1)); 
    ctx->gdraw->line( w, x, ll_y, x, ur_y, drawtype, 0, 0);
  }

  for ( i = 0; i < horizontal_lines; i++) {
    int y = int( ll_y + double(ur_y - ll_y) / (horizontal_lines + 1) * (i + 1)); 
    ctx->gdraw->line( w, ll_x, y, ur_x, y, drawtype, 0, 0);
  }

  if ( fill_curve)
    for ( i = 0; i < curve_cnt; i++) {
      if ( curve[i])
	curve[i]->fill = 0;
    }
  if ( t) {
    GlowTransform tmp = *t * trf;
    for ( i = 0; i < curve_cnt; i++) {
      if ( curve[i])
        curve[i]->draw( w, &tmp, highlight, hot, node, colornode);
    }
  }
  else {
    for ( i = 0; i < curve_cnt; i++) {
      if ( curve[i])
        curve[i]->draw( w, &trf, highlight, hot, node, colornode);
    }
  }

  if ( fill_curve)
    for ( i = 0; i < curve_cnt; i++) {
      if ( curve[i])
	curve[i]->fill = 1;
    }
  if ( border) {
    ctx->gdraw->rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx, 0);
  }

}

//! Erase the object.
/*!
  \param t		Transform of parent node.
  \param hot		Draw as hot, with larger line width.
  \param node		Parent node. Can be zero.
*/
void GrowTrend::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int idx;
  if ( fix_line_width) {
    idx = line_width;
    idx += hot;
    if ( idx < 0) return;
  }
  else {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( w->zoom_factor_y / w->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t) {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
  }
  else {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  w->set_draw_buffer_only();
  if ( border)
    ctx->gdraw->rect_erase( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, idx);
  ctx->gdraw->fill_rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  w->reset_draw_buffer_only();
}

//! Scan trace
/*! Calls the trace scan callback for the object.
 */
void GrowTrend::trace_scan()
{
  if ( !trace.p)
    return;

  if ( ctx->trace_scan_func)
  {
    ctx->trace_scan_func( (void *) this, trace.p);
    return;
  }    
}

//! Init trace
/*! Calls the trace connect callback for the object.
 */
int GrowTrend::trace_init()
{
  int sts;

  sts = ctx->trace_connect_func( (void *) this, &trace);
  return sts;
}

//! Close trace
/*! Calls the trace disconnect callback for the object.
 */
void GrowTrend::trace_close()
{
  if ( trace.p)
    ctx->trace_disconnect_func( (void *) this);
}

//! Redraw the area inside the objects border.
void GrowTrend::draw()
{
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->mw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->mw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->mw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->mw.offset_y + 1);
}

//! Add a new value to the specified curve
/*!
  \param value	New value.
  \param idx		Curve number.
  
  Add the new value first in the curve, and shift the other values one step forward.
*/
void GrowTrend::add_value( double value, int idx) 
{ 
  double curve_value;

  if ( idx >= curve_cnt)
    return;

  if ( y_max_value[idx] != y_min_value[idx])
    curve_value = ur.y - (value - y_min_value[idx]) / 
	(y_max_value[idx] - y_min_value[idx]) * (ur.y - ll.y);

  curve_value = max( ll.y, min( curve_value, ur.y));
  if ( !fill)
    erase( &ctx->mw);
  if ( !fill_curve)
    curve[idx]->add_and_shift_y_value( curve_value);
  else
    curve[idx]->add_and_shift_y_value_filled( curve_value);
  draw();
  draw( &ctx->navw, (GlowTransform *) NULL, highlight, 0, NULL, NULL);
}

//! Moves object to alignment line or point.
/*!
  \param x	x coordinate of alignment point.
  \param y	y coordinate of alignment point.
  \param direction Type of alignment.
*/
void GrowTrend::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->set_defered_redraw();
    draw();
    switch ( direction) {
      case glow_eAlignDirection_CenterVert:
        dx = x - (x_right + x_left) / 2;
        dy = 0;
        break;        
      case glow_eAlignDirection_CenterHoriz:
        dx = 0;
        dy = y - (y_high + y_low) / 2;
        break;        
      case glow_eAlignDirection_CenterCenter:
        dx = x - (x_right + x_left) / 2;
        dy = y - (y_high + y_low) / 2;
        break;        
      case glow_eAlignDirection_Right:
        dx = x - x_right;
        dy = 0;
        break;        
      case glow_eAlignDirection_Left:
        dx = x - x_left;
        dy = 0;
        break;        
      case glow_eAlignDirection_Up:
        dx = 0;
        dy = y - y_high;
        break;        
      case glow_eAlignDirection_Down:
        dx = 0;
        dy = y - y_low;
        break;        
    }
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;

    draw();
    ctx->redraw_defered();
}

void GrowTrend::set_trace_attr( GlowTraceData *attr)
{
  memcpy( &trace, attr, sizeof(trace));
}

void GrowTrend::get_trace_attr( GlowTraceData **attr)
{
  *attr = &trace;
}

//! Set scantime
/*!
  \param time		Scantime in seconds.
*/
void GrowTrend::set_scan_time( double time)
{
  scan_time = time;
  configure_curves();
}

//! Set the range for the specified trend in y direction.
/*!
  \param curve	Number of curve.
  \param min		Min value.
  \param max		Max value.
*/
void GrowTrend::set_range_y( int curve, double min, double max)
{
  if ( !( curve == 0 || curve == 1))
    return;
  y_max_value[curve] = max;
  y_min_value[curve] = min;
  configure_curves();
}


//! Export the object as a javabean.
/*!
  \param t		Transform of parent node. Can be zero.
  \param node		Parent node. Can be zero.
  \param pass		Export pass.
  \param shape_cnt	Current index in a shape vector.
  \param node_cnt	Counter used for javabean name. Not used for this kind of object.
  \param in_nc	Member of a nodeclass. Not used for this kind of object.
  \param fp		Output file.
  
  The object is transformed to the current zoom factor, and GlowExportJBean is used to generate
  java code for the bean.
*/
void GrowTrend::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double rotation;

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( t)
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  else
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;

  ((GrowCtx *)ctx)->export_jbean->trend( ll_x, ll_y, ur_x, ur_y,
    	draw_type, fill_drawtype, curve_drawtype[0], curve_drawtype[1],
	curve_fill_drawtype[0], curve_fill_drawtype[1], fill,
	border, y_min_value[0], y_max_value[0], y_min_value[1], y_max_value[1], 
	curve_width, no_of_points, scan_time, 
	horizontal_lines, vertical_lines, line_width, rotation,
    	pass, shape_cnt, node_cnt, fp);
}

//! Set parameters for the trend.
/*!
  \param info		Info struct.
*/
void GrowTrend::set_trend_info( glow_sTrendInfo *info)
{
  horizontal_lines = info->horizontal_lines;
  vertical_lines = info->vertical_lines;
  fill_curve = info->fill_curve;
  no_of_points = info->no_of_points;
  curve_width = info->curve_width;
  scan_time = info->scan_time;
  for ( int i = 0; i < TREND_MAX_CURVES; i++) {
    y_max_value[i] = info->y_max_value[i];
    y_min_value[i] = info->y_min_value[i];
    x_max_value[i] = info->x_max_value[i];
    x_min_value[i] = info->x_min_value[i];
    curve_drawtype[i] = info->curve_drawtype[i];
    curve_fill_drawtype[i] = info->curve_fill_drawtype[i];
  }
  configure_curves();
}

//! Conversion between different versions of Glow
/*!
  \param version	Version to convert to.
*/
void GrowTrend::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    GrowRect::convert( version);
    curve_drawtype[0] = GlowColor::convert( version, curve_drawtype[0]);
    curve_fill_drawtype[0] = GlowColor::convert( version, curve_fill_drawtype[0]);
    curve_drawtype[1] = GlowColor::convert( version, curve_drawtype[1]);
    curve_fill_drawtype[1] = GlowColor::convert( version, curve_fill_drawtype[1]);

    break;
  }
  }  
}

void GrowTrend::set_data( double *data[3], int data_curves, int data_points) 
{
  glow_eDrawType dt, dt_fill;
  int points;
  int cpoints;
  glow_sPoint *pointarray;
  glow_sPoint *point_p;
  int	i, j, idx;

  curve_cnt = data_curves - 1;
  no_of_points = max( 2, no_of_points);
  points = cpoints = min( no_of_points, data_points);
  if ( fill_curve)
    cpoints += 2;
  curve_width = min( DRAW_TYPE_SIZE, max( 1, curve_width));

  if ( fabs( data[0][points - 1] - data[0][0]) < DBL_EPSILON)
    return;

  pointarray = (glow_sPoint *) calloc( cpoints, sizeof(glow_sPoint));
  for ( j = 0; j < curve_cnt; j++) {
    point_p = pointarray;
    for ( i = 0, idx = 0; i < cpoints; i++, idx++) {

      if ( !fill_curve) {
	idx = i;
	if ( y_max_value[j] != y_min_value[j])
	  point_p->y = ur.y - (data[j+1][idx] - y_min_value[j]) / 
	      (y_max_value[j] - y_min_value[j]) * (ur.y - ll.y);

	point_p->y = max( ll.y, min( point_p->y, ur.y));	
	point_p->x = ll.x + (data[0][idx] - data[0][0]) / (data[0][points - 1] - data[0][0]) *
	  (ur.x - ll.x);
      }
      else {
      
	if ( i == 0) {
	  point_p->x = ll.x;
	  point_p->y = ur.y;
	  idx--;
	}
	else if ( i == cpoints - 1) {
	  point_p->x = ur.x;
	  point_p->y = ur.y;
	}
	else {
	  if ( y_max_value[j] != y_min_value[j])
	    point_p->y = ur.y - (data[j+1][idx] - y_min_value[j]) / 
	      (y_max_value[j] - y_min_value[j]) * (ur.y - ll.y);

	  point_p->y = max( ll.y, min( point_p->y, ur.y));	
	  point_p->x = ll.x + (data[0][idx] - data[0][0]) / (data[0][points - 1] - data[0][0]) *
	    (ur.x - ll.x);
	}
      }
      point_p++;
    }

    ctx->nodraw++;
    if ( curve[j])
      delete curve[j];
    ctx->nodraw--;

    if ( curve_drawtype[j] != glow_eDrawType_Inherit)
      dt = curve_drawtype[j];
    else
      dt = draw_type;

    if ( curve_fill_drawtype[j] != glow_eDrawType_Inherit)
      dt_fill = curve_fill_drawtype[j];
    else
      dt_fill = draw_type;

    ctx->nodraw++;
    curve[j] = new GrowPolyLine( ctx, "", pointarray, cpoints, dt, 
				 curve_width,
				 0, fill_curve, 1, 0, dt_fill);
    ctx->nodraw--;
  }
  free( (char *) pointarray);
  draw();
}

