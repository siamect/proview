#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growtrend.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowTrend::GrowTrend( GlowCtx *glow_ctx, char *name, double x, double y, 
		double w, double h, glow_eDrawType border_d_type, int line_w, 
		glow_mDisplayLevel display_lev,
		int fill_rect, int display_border, 
		glow_eDrawType fill_d_type, int nodraw) : 
		GrowRect(glow_ctx,name,x,y,w,h,border_d_type,line_w,0,
		display_lev,fill_rect,display_border,fill_d_type,nodraw),
		horizontal_lines(0), vertical_lines(0), fill_curve(0),
		no_of_points(100), curve_width(1),
		curve_cnt(0), scan_time(1)
{
  for ( int i = 0; i < TREND_MAX_CURVES; i++) { 
    min_value[i] = 0;
    max_value[i]= 100;
    curve[i] = 0;
    curve_drawtype[i] = glow_eDrawType_Inherit;
    curve_fill_drawtype[i] = glow_eDrawType_Inherit;
  }

  configure_curves();
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);

}

GrowTrend::~GrowTrend()
{
  if ( ctx->nodraw) return;
  erase();
  nav_erase();
  for ( int i = 0; i < curve_cnt; i++)
    delete curve[i];
}

void GrowTrend::configure_curves() 
{
  glow_eDrawType dt, dt_fill;
  int points;
  glow_sPoint *pointarray;
  glow_sPoint *point_p;
  int	i;

  if ( strcmp( trace.data[1], "") == 0)
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
  for ( i = 0; i < points; i++)
  {
    if ( !fill_curve)
    {
      point_p->y = ur.y;
      point_p->x = ur.x - i * (ur.x - ll.x) / (points - 1);
    }
    else
    {
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

  for ( i = 0; i < curve_cnt; i++)
  {
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
	0, fill_curve, 1, dt_fill);
    ctx->nodraw--;
  }
  free( (char *) pointarray);
}

void GrowTrend::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowTrend) << endl;
  fp << int(glow_eSave_GrowTrend_max_value_0) << FSPACE << max_value[0] << endl;
  fp << int(glow_eSave_GrowTrend_max_value_1) << FSPACE << max_value[1] << endl;
  fp << int(glow_eSave_GrowTrend_min_value_0) << FSPACE << min_value[0] << endl;
  fp << int(glow_eSave_GrowTrend_min_value_1) << FSPACE << min_value[1] << endl;
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
  fp << int(glow_eSave_End) << endl;
}

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
      case glow_eSave_GrowTrend_max_value_0: fp >> max_value[0]; break;
      case glow_eSave_GrowTrend_max_value_1: fp >> max_value[1]; break;
      case glow_eSave_GrowTrend_min_value_0: fp >> min_value[0]; break;
      case glow_eSave_GrowTrend_min_value_1: fp >> min_value[1]; break;
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

void GrowTrend::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;

  if ( ll_x > ur_x)
  {
    /* Shift */
    tmp = ll_x;
    ll_x = ur_x;
    ur_x = tmp;
  }
  if ( ll_y > ur_y)
  {
    /* Shift */
    tmp = ll_y;
    ll_y = ur_y;
    ur_y = tmp;
  }

  if ( x_right * ctx->zoom_factor_x - ctx->offset_x >= ll_x &&
      	x_left * ctx->zoom_factor_x - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor_y - ctx->offset_y >= ll_y &&
       	y_low * ctx->zoom_factor_y - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();
  }
}

void GrowTrend::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	tmp;
  int 	obj_ur_x = int( x_right * ctx->zoom_factor_x) - ctx->offset_x;
  int	obj_ll_x = int( x_left * ctx->zoom_factor_x) - ctx->offset_x;
  int	obj_ur_y = int( y_high * ctx->zoom_factor_y) - ctx->offset_y;
  int   obj_ll_y = int( y_low * ctx->zoom_factor_y) - ctx->offset_y;


  if ( *ll_x > *ur_x)
  {
    /* Shift */
    tmp = *ll_x;
    *ll_x = *ur_x;
    *ur_x = tmp;
  }
  if ( *ll_y > *ur_y)
  {
    /* Shift */
    tmp = *ll_y;
    *ll_y = *ur_y;
    *ur_y = tmp;
  }

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();

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

void GrowTrend::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y);

  if ( x_right_pix >= ll_x &&
       x_left_pix <= ur_x &&
       y_high_pix >= ll_y &&
       y_low_pix <= ur_y)
  {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);
  }
}

void GrowTrend::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowTrend::draw( GlowTransform *t, int highlight, int hot, void *node, 
		      void *colornode)
{
  if ( ctx->nodraw)
    return;
  int i;
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  glow_eDrawType drawtype;

  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0)
    {
      erase( t, hot, node);
      return;
    }
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);
  if ( fill)
  {
    drawtype = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_Color57,
		 highlight, (GrowNode *)colornode, 1);
    glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype);
  }
  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_Color59,
		 highlight, (GrowNode *)colornode, 0);

  if ( fill_curve)
  {
    for ( i = 0; i < curve_cnt; i++)
      curve[i]->border = 0;

    if ( t)
    {
      GlowTransform tmp = *t * trf;
      for ( i = 0; i < curve_cnt; i++)
      {
        if ( curve[i])
          curve[i]->draw( &tmp, highlight, hot, node, colornode);
      }
    }
    else
    {
      for ( i = 0; i < curve_cnt; i++)
      {
        if ( curve[i])
          curve[i]->draw( &trf, highlight, hot, node, colornode);
      }
    }
    for ( i = 0; i < curve_cnt; i++)
      curve[i]->border = 1;
  }

  for ( i = 0; i < vertical_lines; i++)
  {
    int x = int( ll_x + double(ur_x - ll_x) / (vertical_lines + 1) * (i + 1)); 
    glow_draw_line( ctx, x, ll_y, x, ur_y, drawtype, 0, 0);
  }

  for ( i = 0; i < horizontal_lines; i++)
  {
    int y = int( ll_y + double(ur_y - ll_y) / (horizontal_lines + 1) * (i + 1)); 
    glow_draw_line( ctx, ll_x, y, ur_x, y, drawtype, 0, 0);
  }

  if ( fill_curve)
    for ( i = 0; i < curve_cnt; i++)
      curve[i]->fill = 0;

  if ( t)
  {
    GlowTransform tmp = *t * trf;
    for ( i = 0; i < curve_cnt; i++)
    {
      if ( curve[i])
        curve[i]->draw( &tmp, highlight, hot, node, colornode);
    }
  }
  else
  {
    for ( i = 0; i < curve_cnt; i++)
    {
      if ( curve[i])
        curve[i]->draw( &trf, highlight, hot, node, colornode);
    }
  }

  if ( fill_curve)
    for ( i = 0; i < curve_cnt; i++)
      curve[i]->fill = 1;

  if ( border)
  {
    glow_draw_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx, 0);
  }

}

void GrowTrend::erase( GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  ctx->set_draw_buffer_only();
  if ( border)
    glow_draw_rect_erase( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, idx);
  glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  ctx->reset_draw_buffer_only();
}

void GrowTrend::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  int i;
  if ( !(display_level & ctx->display_level))
    return;
  glow_eDrawType drawtype;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( fill)
  {
    drawtype = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_Color57,
		 0, (GrowNode *)colornode, 1);
    glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	drawtype);
  }
  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_Color59,
		 0, (GrowNode *)colornode, 0);

  if ( t)
  {
    GlowTransform tmp = *t * trf;
    for ( i = 0; i < curve_cnt; i++)
    {
      if ( curve[i])
        curve[i]->nav_draw( &tmp, highlight, node, colornode);
    }
  }
  else
  {
    for ( i = 0; i < curve_cnt; i++)
    {
      if ( curve[i])
        curve[i]->nav_draw( &trf, highlight, node, colornode);
    }
  }
  if ( border)
  {
    glow_draw_nav_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	drawtype, idx, 0);
  }
}

void GrowTrend::nav_erase( GlowTransform *t, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( border )
    glow_draw_nav_rect_erase( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	idx);
  glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	glow_eDrawType_LineErase);
}

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

int GrowTrend::trace_init()
{
  int sts;

  if ( strcmp( trace.data[0], "") == 0)
    return 1;

  sts = ctx->trace_connect_func( (void *) this, &trace);
  return sts;
}

void GrowTrend::trace_close()
{
  if ( strcmp( trace.data[0], "") == 0)
    return;

  if ( trace.p)
    ctx->trace_disconnect_func( (void *) this);
}

void GrowTrend::draw()
{
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowTrend::add_value( double value, int idx) 
{ 
  double curve_value;

  if ( idx >= curve_cnt)
    return;

  if ( max_value[idx] != min_value[idx])
    curve_value = ur.y - (value - min_value[idx]) / 
	(max_value[idx] - min_value[idx]) * (ur.y - ll.y);

  curve_value = max( ll.y, min( curve_value, ur.y));
  if ( !fill)
    erase();
  if ( !fill_curve)
    curve[idx]->add_and_shift_y_value( curve_value);
  else
    curve[idx]->add_and_shift_y_value_filled( curve_value);
  draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  nav_draw( (GlowTransform *) NULL, highlight, NULL, NULL);
}

void GrowTrend::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase();
    nav_erase();
    switch ( direction)
    {
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
}

void GrowTrend::set_trace_attr( GlowTraceData *attr)
{
  memcpy( &trace, attr, sizeof(trace));
}

void GrowTrend::get_trace_attr( GlowTraceData **attr)
{
  *attr = &trace;
}

void GrowTrend::set_scan_time( double time)
{
  scan_time = time;
  configure_curves();
}

void GrowTrend::set_range( int curve, double min, double max)
{
  if ( !( curve == 0 || curve == 1))
    return;
  max_value[curve] = max;
  min_value[curve] = min;
  configure_curves();
}

void GrowTrend::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double rotation;

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
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
	border, min_value[0], max_value[0], min_value[1], max_value[1], 
	curve_width, no_of_points, scan_time, 
	horizontal_lines, vertical_lines, line_width, rotation,
    	pass, shape_cnt, node_cnt, fp);
}
