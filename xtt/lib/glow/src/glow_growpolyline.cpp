#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growpolyline.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_grownode.h"


GrowPolyLine::GrowPolyLine( GlowCtx *glow_ctx, char *name, 
		glow_sPoint *pointarray, int point_cnt,
		glow_eDrawType border_d_type, int line_w, 
		int fix_line_w, int fill, int display_border, int display_shadow,
		glow_eDrawType fill_d_type, int closed, int nodraw) :
		GlowPolyLine(glow_ctx,pointarray,point_cnt,border_d_type,line_w,
		fix_line_w,fill,closed), 
    		hot(0), pzero(ctx), highlight(0), inverse(0), user_data(NULL),
		dynamic(0), dynamicsize(0),
		original_border_drawtype(border_d_type),
		original_fill_drawtype(fill_d_type), fill_drawtype(fill_d_type),
		border(display_border), fill_eq_border(0), current_point(0),
		shadow(display_shadow), shadow_width(5), relief(glow_eRelief_Up),
		shadow_contrast(2), disable_shadow(0), fill_eq_light(0), fill_eq_shadow(0)
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();
  strcpy( last_group, "");

  if ( ctx->grid_on && point_cnt > 0)
  {
    double x_grid, y_grid, x, y;
    int i;

    if ( point_cnt <= 2)
    {
      // Position all points on the grid
      for ( i = 0; i < a_points.a_size; i++)
      {
         ctx->find_grid( ((GlowPoint *)a_points[i])->x, 
	       ((GlowPoint *)a_points[i])->y, &x_grid, &y_grid);
        ((GlowPoint *)a_points[i])->posit( x_grid, y_grid);
      }
    }
    else
    {
      // Position the first on the grid
      x = ((GlowPoint *)a_points[0])->x;
      y = ((GlowPoint *)a_points[0])->y;
      ctx->find_grid( x, y, &x_grid, &y_grid);
      for ( i = 0; i < a_points.a_size; i++)
      {
        ((GlowPoint *)a_points[i])->posit(
		((GlowPoint *)a_points[i])->x + x_grid - x,
		((GlowPoint *)a_points[i])->y + y_grid - y);
      }
    }
  }
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  get_node_borders();
}

int GrowPolyLine::shadow_direction()
{
  double a1, a2, a1_old, a0_old;
  double a_sum1 = 0;
  double a_sum2 = 0;
  int p_num = a_points.a_size;
  if ( (points[p_num - 1].x == points[0].x && points[p_num - 1].y == points[0].y))
    p_num--;

  for ( int i = 0; i < p_num; i++) {
    if ( i == p_num - 1) {
      if ( points[0].x == points[i].x) {
	a1 = 90;
	if ( points[0].y < points[i].y)
	  a1 += 180;
      }
      else
	a1 = atan(double(points[0].y - points[i].y)/(points[0].x - points[i].x)) * 180 / M_PI;
      if ( points[0].x < points[i].x)
	a1 += 180;
    }
    else {
      if ( points[i+1].x == points[i].x) {
	a1 = 90;
	if ( points[i+1].y < points[i].y)
	  a1 += 180;
      }
      else
	a1 = atan(double(points[i+1].y - points[i].y)/(points[i+1].x - points[i].x)) * 180 / M_PI;
      if ( points[i+1].x < points[i].x)
	a1 += 180;
    }
    if ( i == 0) {
      a0_old = a1;
      // printf( "a1: %d %f\n", i, a1);
    }
    else if ( i > 0) {
      a2 = a1 - (a1_old - 180);
      if ( a2 < 0)
	a2 += 360;
      else if ( a2 >= 360)
	a2 -= 360;
      a_sum1 += a2;
      a_sum2 += 360 - a2;

      // printf( "a1: %d %f %f %f\n", i, a1, a2, 360 - a2);
    }
    if ( i == p_num - 1) {
      a2 = a0_old - (a1 - 180);
      if ( a2 < 0)
	a2 += 360;
      else if ( a2 >= 360)
	a2 -= 360;
      a_sum1 += a2;
      a_sum2 += 360 - a2;
      // printf( "a1: %d %f %f %f sum: %f %f diff: %f\n", i, a1, a2, 360 - a2, a_sum1, a_sum2, a_sum2 - a_sum1);
    }
    a1_old = a1;
  }
  int dir;
  XPoint last_point;
  if ( points[0].x == points[a_points.a_size-1].x && points[0].y == points[a_points.a_size-1].y)
    last_point = points[a_points.a_size-2];
  else
    last_point = points[a_points.a_size-1];

  if ( a_sum2 - a_sum1 < 0) {
    if ( points[0].x == last_point.x) {
      if ( points[0].x < points[1].x)
	dir = 1;
      else
	dir = -1;
    }
    else if ( points[0].x > last_point.x)
      dir = 1;
    else
      dir = -1;
  }
  else {
    if ( points[0].x == last_point.x) {
      if ( points[0].x < points[1].x)
	dir = 1;
      else
	dir = -1;
    }
    else if ( points[0].x < last_point.x)
      dir = 1;
    else
      dir = -1;
  }
  return dir;
}

void GrowPolyLine::calculate_shadow( glow_sShadowInfo **s, int *num, int ish, int highlight, 
			 void *colornode, int javaexport)
{
  glow_sShadowInfo *sp;
  double x;
  int pos01;
  int pos12;
  int i;
  glow_eDrawType light_drawtype;
  glow_eDrawType dark_drawtype;
    
  int p_num = a_points.a_size;
  if ( points[0].x == points[p_num-1].x && points[0].y == points[p_num-1].y)
    p_num--;
  sp = (glow_sShadowInfo *) calloc( p_num + 1, sizeof(glow_sShadowInfo));

  glow_eDrawType fillcolor = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 highlight, (GrowNode *)colornode, 1);

  int drawtype_incr = shadow_contrast;
  if ( relief == glow_eRelief_Down)
    drawtype_incr = -shadow_contrast;
  if ( javaexport) {
    light_drawtype = (glow_eDrawType) -drawtype_incr;
    dark_drawtype = (glow_eDrawType) drawtype_incr;
  }
  else {
    light_drawtype = ((GrowCtx *)ctx)->shift_drawtype( fillcolor, -drawtype_incr, 
								      (GrowNode *)colornode);
    dark_drawtype = ((GrowCtx *)ctx)->shift_drawtype( fillcolor, drawtype_incr, 
								     (GrowNode *)colornode);
  }

  pos01 = shadow_direction();

  for ( i = 0; i < p_num; i++) {
    double sx0, sx1, sx2, sy0, sy1, sy2;
    double k01, m01, k12, m12;

    if ( i == 0) {
      sx0 = points[p_num - 1].x;
      sy0 = points[p_num - 1].y;
    }
    else {
      sx0 = points[i-1].x;
      sy0 = points[i-1].y;
    }
    sx1 = points[i].x;
    sy1 = points[i].y;
    if ( i == p_num - 1) {
      sx2 = points[0].x;
      sy2 = points[0].y;
    }
    else {
      sx2 = points[i+1].x;
      sy2 = points[i+1].y;
    }

    pos12 = pos01;
    if ( i == -1) {
    }
    else {
      if ( fabs( sx0 - sx1) < DBL_EPSILON) {
	if (sx1 > sx2 && sy1 < sy0)
	  pos12 = - pos01;
	else if ( sx1 < sx2 && sy1 > sy0)
	  pos12 = - pos01;
      }
      else if ( fabs( sx1 - sx2) < DBL_EPSILON) {
	if ( sx0 < sx1 && sy2 > sy1)
	  pos12 = - pos01;
	else if (  sx0 > sx1 && sy2 < sy1)
	  pos12 = - pos01;
      }
      else if ( sx1 > sx0 && sx2 < sx1)
	pos12 = -pos01;
      else if ( sx1 < sx0 && sx2 > sx1)
	pos12 = -pos01;
    }
    
    if ( fabs( sx0 - sx1) < DBL_EPSILON)  {
      if ( fabs( sx1 - sx2) < DBL_EPSILON) {
	x = sx1 + pos01 * ish;
	sp[i].x = int( x + 0.5);
	sp[i].y = int( sy1 + 0.5);
      }
      else {
	k12 = (sy2 - sy1)/(sx2 - sx1);
	m12 = sy1 - sx1 * k12 + pos12 * ish / fabs(cos(atan(k12)));
      
	x = sx1 + pos01 * ish;
	sp[i].x = int( x + 0.5);
	sp[i].y = int( k12 * x + m12 + 0.5);
      }
    }
    else if ( fabs( sx1 - sx2) < DBL_EPSILON) {
      k01 = (sy1 - sy0)/(sx1 - sx0);
      m01 = sy0 - sx0 * k01 + pos01 * ish / fabs(cos(atan(k01)));

      x = sx1 + pos12 * ish;
      sp[i].x = int(  x + 0.5);
      sp[i].y = int( k01 * x + m01 + 0.5);
    }
    else {
      k01 = (sy1 - sy0)/(sx1 - sx0);
      k12 = (sy2 - sy1)/(sx2 - sx1);
      m01 = sy0 - sx0 * k01 + pos01 * ish / fabs(cos(atan(k01)));
      m12 = sy1 - sx1 * k12 + pos12 * ish / fabs(cos(atan(k12)));
      if ( fabs( k01 - k12) < DBL_EPSILON) {
	// Identical lines
	k12 = -k12;
	m12 = sy2 - k12 * sx2;

	x = (m12 - m01)/(k01 - k12);
	sp[i].x = int( x + 0.5);
	sp[i].y = int( k12 * x + m12 + 0.5);
	k12 = k01;
	m12 = m01;
      }
      else {
	x = (m12 - m01)/(k01 - k12);
	sp[i].x = int( x + 0.5);
	sp[i].y = int( k12 * x + m12 + 0.5);
      }
    }
    if ( pos12 == 1)
      sp[i].drawtype = light_drawtype;
    else
      sp[i].drawtype = dark_drawtype;
    pos01 = pos12;
  }
  sp[p_num].x = sp[0].x;
  sp[p_num].y = sp[0].y;
  if ( points[0].x == points[a_points.a_size-1].x && points[0].y == points[a_points.a_size-1].y)
    p_num++;

  *s = sp;
  *num = p_num;
}

void GrowPolyLine::draw( GlowTransform *t, int highlight, int hot, void *node, 
			 void *colornode)
{
  int i;
  glow_eDrawType drawtype;
  int idx;
  if ( node && ((GrowNode *)node)->line_width)
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  double x1, y1;

  for ( i = 0; i < a_points.a_size; i++)
  {
    if (!t)
    {
      x1 = trf.x( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }
    else
    {
      x1 = trf.x( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }

    point_p->x = int( x1 * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    point_p->y = int( y1 * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    point_p++;
  }
  if ( fill)
  {
    if ( fill_eq_border)
      drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
    else      
      drawtype = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 highlight, (GrowNode *)colornode, 1);
    if ( fill_eq_light && node && ((GrowNode *)node)->shadow)
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( drawtype, -shadow_contrast, 
						   (GrowNode *)colornode);
    else if ( fill_eq_shadow && node && ((GrowNode *)node)->shadow)
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( drawtype, shadow_contrast, 
						   (GrowNode *)colornode);
    glow_draw_fill_polyline( ctx, points, a_points.a_size, drawtype, 0);
  }

  int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow &&
    !fill_eq_light && !fill_eq_shadow;

  if ( display_shadow && shadow_width != 0) {
    glow_sShadowInfo *sp;
    int p_num;

    double trf_scale = trf.vertical_scale( t);
    int ish = int( shadow_width / 100 * trf_scale * 
      min((x_right - x_left)*ctx->zoom_factor_x, (y_high - y_low)*ctx->zoom_factor_y) + 0.5);

    if ( ish >= 1) {
      calculate_shadow( &sp, &p_num, ish, highlight, colornode, 0);

      XPoint p[4];
      for ( i = 0; i < p_num - 1; i++) {
	p[0].x = points[i].x;
	p[0].y = points[i].y;
	p[1].x = sp[i].x;
	p[1].y = sp[i].y;
	p[3].x = points[i+1].x;
	p[3].y = points[i+1].y;
	p[2].x = sp[i+1].x;
	p[2].y = sp[i+1].y;

	glow_draw_fill_polyline( ctx, p, 4, sp[i].drawtype, 0);
      }
      free( sp);
    }
  }
  if ( border || !(fill || (display_shadow && shadow_width != 0))) {
    drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
    glow_draw_polyline( ctx, points, a_points.a_size, drawtype, idx, 0);
  }
}

void GrowPolyLine::erase( GlowTransform *t, int hot, void *node)
{
  int i;
  int idx;
  if ( node && ((GrowNode *)node)->line_width)
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  double x1, y1;

  for ( i = 0; i < a_points.a_size; i++)
  {
    if (!t)
    {
      x1 = trf.x( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }
    else
    {
      x1 = trf.x( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }

    point_p->x = int( x1 * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    point_p->y = int( y1 * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    point_p++;
  }
  int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow &&
    !fill_eq_light && !fill_eq_shadow;

  ctx->set_draw_buffer_only();
  if ( border || !(fill || (display_shadow && shadow_width != 0)))
    glow_draw_polyline_erase( ctx, points, a_points.a_size, idx);
  if ( fill || (display_shadow && shadow_width != 0))
    glow_draw_fill_polyline( ctx, points, a_points.a_size, 
		glow_eDrawType_LineErase, 0);
  ctx->reset_draw_buffer_only();
}

void GrowPolyLine::move( double delta_x, double delta_y, int grid)
{
  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - 2*DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - 2*DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + 2*DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + 2*DRAW_MP);
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    erase();
    nav_erase();
    ctx->find_grid( x_left + delta_x / ctx->zoom_factor_x,
	y_low + delta_y / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    erase();
    nav_erase();
    dx = delta_x / ctx->zoom_factor_x;
    dy = delta_y / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - 2*DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - 2*DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + 2*DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + 2*DRAW_MP);
  ctx->redraw_defered();
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowPolyLine::move_current_point( int delta_x, int delta_y, int grid)
{
  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - 4*DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - 4*DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + 4*DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + 4*DRAW_MP);
  if ( grid)
  {
    double x, y, x_grid, y_grid;

    erase();
    nav_erase();

    x = trf.x( ((GlowPoint *)a_points[current_point])->x, 
		((GlowPoint *)a_points[current_point])->y);
    y = trf.y( ((GlowPoint *)a_points[current_point])->x, 
		((GlowPoint *)a_points[current_point])->y);
    x += double( delta_x) / ctx->zoom_factor_x;
    y += double( delta_y) / ctx->zoom_factor_y;
    ctx->find_grid( x, y, &x_grid, &y_grid);

    trf.reverse( x_grid, y_grid, &((GlowPoint *)a_points[current_point])->x, 
		&((GlowPoint *)a_points[current_point])->y);
    zoom();
    get_node_borders();
  }
  else
  {
    double x, y;

    erase();
    nav_erase();

    x = trf.x( ((GlowPoint *)a_points[current_point])->x, 
		((GlowPoint *)a_points[current_point])->y);
    y = trf.y( ((GlowPoint *)a_points[current_point])->x, 
		((GlowPoint *)a_points[current_point])->y);
    x += double( delta_x) / ctx->zoom_factor_x;
    y += double( delta_y) / ctx->zoom_factor_y;
    trf.reverse( x, y, &((GlowPoint *)a_points[current_point])->x, 
		&((GlowPoint *)a_points[current_point])->y);
    zoom();
    get_node_borders();
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - 4*DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - 4*DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + 4*DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + 4*DRAW_MP);
  ctx->redraw_defered();
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

GrowPolyLine::~GrowPolyLine()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase();
  nav_erase();

  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
}

void GrowPolyLine::move_noerase( int delta_x, int delta_y, int grid)
{
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    ctx->find_grid( x_left + double( delta_x) / ctx->zoom_factor_x,
	y_low + double( delta_y) / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    dx = double( delta_x) / ctx->zoom_factor_x;
    dy = double( delta_y) / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

int GrowPolyLine::local_event_handler( glow_eEvent event, double x, double y)
{

  if ( ((GrowCtx *)ctx)->edit_mode == grow_eMode_EditPolyLine &&
	ctx->a_sel[0] == this)
  {
    int 	x1, xc, y1, yc;
    int 	i;
    int		dx = 4;
    int		dy = 4;

    xc = int( trf.x( x, y) * ctx->zoom_factor_x) + ctx->offset_x;
    yc = int( trf.y( x, y) * ctx->zoom_factor_y) + ctx->offset_y;

    for ( i = 0; i < a_points.a_size; i++)
    {
      x1 = int( trf.x( ((GlowPoint *)a_points[i])->x, 
		((GlowPoint *)a_points[i])->y)
		* ctx->zoom_factor_x) + ctx->offset_x;
      y1 = int( trf.y( ((GlowPoint *)a_points[i])->x, 
		((GlowPoint *)a_points[i])->y)
		* ctx->zoom_factor_y) + ctx->offset_y;


      if ( abs( x1 - xc) < dx && abs( y1 - yc) < dy)
      {
        // cout << "Event handler: Hit in polyline point" << endl;
        current_point = i;
        return 1;
      }
    }
  }
  else if ( ((GrowCtx *)ctx)->edit_mode != grow_eMode_EditPolyLine)
  {
    double 	x1, x2, y1, y2;
    int 	i;

    trf.reverse( 0.1 * line_width, 0.1 * line_width, &x2, &y2);
    trf.reverse( 0, 0, &x1, &y1);
    double dx = fabs( x2 - x1);
    double dy = fabs( y2 - y1);

    for ( i = 0; i < a_points.a_size -1; i++)
    {
      x1 = ((GlowPoint *)a_points[i])->x;
      x2 = ((GlowPoint *)a_points[i+1])->x;
      y1 = ((GlowPoint *)a_points[i])->y;
      y2 = ((GlowPoint *)a_points[i+1])->y;

      if ((x1 == x2 && y1 < y2 && 		// Vertical
         fabs( x1 - x) < dx && 
         y1 < y && y < y2)
	  ||
        (x1 == x2 && y1 > y2 && 		// Vertical
         fabs( x1 - x) < dx && 
         y2 < y && y < y1)  
	||
        (y1 == y2 && x1 < x2 &&		// Horizontal
         fabs( y1 - y) < dy && 
         x1 < x && x < x2)
          ||
        (y1 == y2 && x1 > x2 &&		// Horizontal
         fabs( y1 - y) < dy && 
         x2 < x && x < x1))
      {
//      cout << "Event handler: Hit in line" << endl;
        return 1;
      }  
      else if (( !(x1 == x2 || y1 == y2) && x1 < x2 && x1 <= x && x <= x2 &&
               fabs(y - (y2-y1)/(x2-x1) * x - y1 + (y2-y1)/(x2-x1) * x1) < dx)
             ||
             ( !(x1 == x2 || y1 == y2) && x1 > x2 && x2 <= x && x <= x1 &&
               fabs(y - (y2-y1)/(x2-x1) * x - y1 + (y2-y1)/(x2-x1) * x1) < dx))
      {
//        cout << "Event handler: Hit in line" << endl;
        return 1;
      }
    }
  }
  return 0;
}

int GrowPolyLine::event_handler( glow_eEvent event, double fx, double fy)
{
  double 	x, y;

  trf.reverse( fx, fy, &x, &y);
  return local_event_handler( event, x, y);
}

int GrowPolyLine::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;
  double rx, ry;

  // Convert koordinates to local koordinates
  trf.reverse( fx, fy, &rx, &ry);

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = local_event_handler( event, rx, ry);
    if ( sts)
    {
      /* Register node for potential movement */
      ctx->move_insert( this);
    }
    return sts;
  }
  switch ( event)
  {
    case glow_eEvent_CursorMotion:
    {
      int redraw = 0;

      if ( ctx->hot_mode == glow_eHotMode_TraceAction)
        sts = 0;
      else if ( ctx->hot_found)
        sts = 0;
      else
      {
        sts = local_event_handler( event, rx, ry);
        if ( sts)
          ctx->hot_found = 1;
      }
      if ( sts && !hot  &&
	   !(ctx->node_movement_active || ctx->node_movement_paste_active))
      {
        draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
        hot = 1;
        redraw = 1;
      }
      if ( !sts && hot)
      {
        if ( !ctx->hot_found)
          draw_set_cursor( ctx, glow_eDrawCursor_Normal);
        erase();
        hot = 0;
        redraw = 1;
      }
      if ( redraw)
      {
        if ( !inverse)
          ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
//          ((GlowLine *)this)->draw( (void *)&pzero, highlight, hot, NULL);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = local_event_handler( event, rx, ry);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowPolyLine::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;

  fp << int(glow_eSave_GrowPolyLine) << endl;
  fp << int(glow_eSave_GrowPolyLine_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_GrowPolyLine_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_GrowPolyLine_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_GrowPolyLine_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_GrowPolyLine_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_GrowPolyLine_polyline_part) << endl;
  GlowPolyLine::save( fp, mode);
  fp << int(glow_eSave_GrowPolyLine_original_border_drawtype) << FSPACE 
		<< int(original_border_drawtype) << endl;
  fp << int(glow_eSave_GrowPolyLine_original_fill_drawtype) << FSPACE 
		<< int(original_fill_drawtype) << endl;
  fp << int(glow_eSave_GrowPolyLine_fill_drawtype) << FSPACE << int(fill_drawtype) << endl;
  fp << int(glow_eSave_GrowPolyLine_border) << FSPACE << border << endl;
  fp << int(glow_eSave_GrowPolyLine_shadow) << FSPACE << shadow << endl;
  fp << int(glow_eSave_GrowPolyLine_shadow_width) << FSPACE << shadow_width << endl;
  fp << int(glow_eSave_GrowPolyLine_shadow_contrast) << FSPACE << shadow_contrast << endl;
  fp << int(glow_eSave_GrowPolyLine_relief) << FSPACE << int(relief) << endl;
  fp << int(glow_eSave_GrowPolyLine_disable_shadow) << FSPACE << disable_shadow << endl;
  fp << int(glow_eSave_GrowPolyLine_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowPolyLine_dynamic) << endl;
  if( dynamic)
  {
    fp << "\"";
    for ( s  = dynamic; *s; s++)
    {
      if ( *s == '"')
        fp << "\\";
      fp << *s;
    }
    fp << "\"" << endl;
  }
  fp << int(glow_eSave_GrowPolyLine_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_GrowPolyLine_fill_eq_border) << FSPACE << fill_eq_border << endl;
  fp << int(glow_eSave_GrowPolyLine_fill_eq_light) << FSPACE << fill_eq_light << endl;
  fp << int(glow_eSave_GrowPolyLine_fill_eq_shadow) << FSPACE << fill_eq_shadow << endl;
  fp << int(glow_eSave_End) << endl;
}

void GrowPolyLine::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		j;
  char		c;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowPolyLine: break;
      case glow_eSave_GrowPolyLine_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowPolyLine_x_right: fp >> x_right; break;
      case glow_eSave_GrowPolyLine_x_left: fp >> x_left; break;
      case glow_eSave_GrowPolyLine_y_high: fp >> y_high; break;
      case glow_eSave_GrowPolyLine_y_low: fp >> y_low; break;
      case glow_eSave_GrowPolyLine_original_border_drawtype: fp >> 
		tmp; original_border_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowPolyLine_original_fill_drawtype: fp >> 
		tmp; original_fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowPolyLine_fill_drawtype: fp >> 
		tmp; fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowPolyLine_border: fp >> border; break;
      case glow_eSave_GrowPolyLine_shadow_width: fp >> shadow_width; break;
      case glow_eSave_GrowPolyLine_shadow_contrast: fp >> shadow_contrast; break;
      case glow_eSave_GrowPolyLine_shadow: fp >> shadow; break;
      case glow_eSave_GrowPolyLine_relief: fp >> tmp; relief = (glow_eRelief)tmp; break;
      case glow_eSave_GrowPolyLine_disable_shadow: fp >> disable_shadow; break;
      case glow_eSave_GrowPolyLine_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowPolyLine_dynamic:
        fp.getline( dummy, sizeof(dummy));
        if ( dynamicsize)
        {
          dynamic = (char *) calloc( 1, dynamicsize);
	  fp.get();
          for ( j = 0; j < dynamicsize; j++)
	  {
	    if ((c = fp.get()) == '"')
	    {
	      if ( dynamic[j-1] == '\\')
	        j--;
	      else
              {
	        dynamic[j] = 0;
                break;
              }
	    }
            dynamic[j] = c;
	  }
          fp.getline( dummy, sizeof(dummy));
        }
        break;
      case glow_eSave_GrowPolyLine_polyline_part: 
        GlowPolyLine::open( fp);
        break;

      case glow_eSave_GrowPolyLine_trf: trf.open( fp); break;
      case glow_eSave_GrowPolyLine_fill_eq_border: fp >> fill_eq_border; break;
      case glow_eSave_GrowPolyLine_fill_eq_light: fp >> fill_eq_light; break;
      case glow_eSave_GrowPolyLine_fill_eq_shadow: fp >> fill_eq_shadow; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowPolyLine:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowPolyLine::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowPolyLine::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowPolyLine::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	obj_ur_x = int( x_right * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
  int	obj_ll_x = int( x_left * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
  int	obj_ur_y = int( y_high * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  int   obj_ll_y = int( y_low * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y)
  {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);

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

void GrowPolyLine::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);
}

void GrowPolyLine::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowPolyLine::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  int i;
  double x1, y1, x2, y2;

  for ( i = 0; i < a_points.a_size - 1; i++)
  {
    if  ( t)
    {
      if ( i == 0)
      {
        x1 = trf.x( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
        y1 = trf.y( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      }
      else
      {
        x1 = x2;
        y1 = y2;
      }
      x2 = trf.x( t, ((GlowPoint *)a_points[i+1])->x, ((GlowPoint *)a_points[i+1])->y);
      y2 = trf.y( t, ((GlowPoint *)a_points[i+1])->x, ((GlowPoint *)a_points[i+1])->y);
    }
    else
    {
      if ( i == 0)
      {
        x1 = trf.x( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
        y1 = trf.y( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      }
      else
      {
        x1 = x2;
        y1 = y2;
      }
      x2 = trf.x( ((GlowPoint *)a_points[i+1])->x, ((GlowPoint *)a_points[i+1])->y);
      y2 = trf.y( ((GlowPoint *)a_points[i+1])->x, ((GlowPoint *)a_points[i+1])->y);
    }

    if ( x1 < *x_left)
      *x_left = x1;
    if ( x2 < *x_left)
      *x_left = x2;
    if ( x1 > *x_right)
      *x_right = x1;
    if ( x2 > *x_right)
      *x_right = x2;
    if ( y1 < *y_low)
      *y_low = y1;
    if ( y2 < *y_low)
      *y_low = y2;
    if ( y1 > *y_high)
      *y_high = y1;
    if ( y2 > *y_high)
      *y_high = y2;
  }
}

void GrowPolyLine::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy)
{
  if ( select_policy == glow_eSelectPolicy_Surround )
  {
    if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
      ctx->select_insert( this);
  }
  else
  {
    if ( x_right > ll_x && x_left < ur_x && y_low < ur_y && y_high > ll_y)
      ctx->select_insert( this);
  }
}

void GrowPolyLine::add_points( glow_sPoint *point_array, int point_cnt)
{ 
  if ( ctx->grid_on && point_cnt == 1)
  {
    double x_grid, y_grid;

    // If one point, move to gridpoint, otherwise...
    ctx->find_grid( point_array->x, point_array->y, &x_grid, &y_grid);
    point_array->x = x_grid;
    point_array->y = y_grid;
  }

  GlowPolyLine::add_points( (void *)&pzero, point_array, point_cnt,
	  highlight, hot);
  get_node_borders();
}

void GrowPolyLine::set_fill( int fillval)
{
  erase();
  nav_erase();
  GlowPolyLine::set_fill( fillval);
  draw();
}

void GrowPolyLine::set_border( int borderval)
{
  erase();
  nav_erase();
  border = borderval;
  draw();
}

void GrowPolyLine::set_drawtype( glow_eDrawType drawtype)
{
  GlowPolyLine::set_drawtype( drawtype);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowPolyLine::set_linewidth( int linewidth)
{
  erase();
  nav_erase();
  GlowPolyLine::set_linewidth( linewidth);
  draw();
}

void GrowPolyLine::set_dynamic( char *code, int size)
{
  if ( !dynamic)
  {
    dynamic = (char *) calloc( 1, size+1);
    dynamicsize = size+1;
  }
  else if ( dynamicsize < size+1)
  {
    free( dynamic);
    dynamic = (char *) calloc( 1, size+1);
    dynamicsize = size+1;
  }
  strncpy( dynamic, code, size+1);
}

void GrowPolyLine::exec_dynamic()
{
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ((GrowCtx *)ctx)->dynamic_cb( this, dynamic, glow_eDynamicType_Object);
}

void GrowPolyLine::set_position( double x, double y)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.a13 == x && trf.a23 == y)
     return;
  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.posit( x, y);
  get_node_borders();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}


void GrowPolyLine::set_scale( double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.s_a11 && trf.s_a22 &&
       fabs( scale_x - trf.a11 / trf.s_a11) < FLT_EPSILON &&
       fabs( scale_y - trf.a22 / trf.s_a22) < FLT_EPSILON)
    return;

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
      x0 = x_left;
      y0 = y_low;
      break;
    case glow_eScaleType_LowerRight:
      x0 = x_right;
      y0 = y_low;
      break;
    case glow_eScaleType_UpperRight:
      x0 = x_right;
      y0 = y_high;
      break;
    case glow_eScaleType_UpperLeft:
      x0 = x_left;
      y0 = y_high;
      break;
    case glow_eScaleType_FixPoint:
      break;
    case glow_eScaleType_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_low + y_high) /2;
      break;
    default:
      ;
  }

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.scale_from_stored( scale_x, scale_y, x0, y0);
  get_node_borders();

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
      x_left = old_x_left;
      y_low = old_y_low;
      break;
    case glow_eScaleType_LowerRight:
      x_right = old_x_right;
      y_low = old_y_low;
      break;
    case glow_eScaleType_UpperRight:
      x_right = old_x_right;
      y_high = old_y_high;
      break;
    case glow_eScaleType_UpperLeft:
      x_left = old_x_left;
      y_high = old_y_high;
      break;
    case glow_eScaleType_FixPoint:
      break;
    case glow_eScaleType_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_low + y_high) /2;
      break;
    default:
      ;
  }
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowPolyLine::set_rotation( double angel, 
	double x0, double y0, glow_eRotationPoint type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( fabs( angel - trf.rotation + trf.s_rotation) < FLT_EPSILON)
    return;

  switch( type)
  {
    case glow_eRotationPoint_LowerLeft:
      x0 = x_left;
      y0 = y_low;
      break;
    case glow_eRotationPoint_LowerRight:
      x0 = x_right;
      y0 = y_low;
      break;
    case glow_eRotationPoint_UpperRight:
      x0 = x_right;
      y0 = y_high;
      break;
    case glow_eRotationPoint_UpperLeft:
      x0 = x_left;
      y0 = y_high;
      break;
    case glow_eRotationPoint_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_high + y_low) / 2;
      break;
    default:
      ;
  }

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.rotate_from_stored( angel, x0, y0);
  get_node_borders();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}


void GrowPolyLine::nav_draw(  GlowTransform *t, int highlight, void *node, void *colornode)
{
  int i;
  glow_eDrawType drawtype;
  int idx;
  if ( node && ((GrowNode *)node)->line_width)
    idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  double x1, y1;

  for ( i = 0; i < a_points.a_size; i++)
  {
    if (!t)
    {
      x1 = trf.x( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }
    else
    {
      x1 = trf.x( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }

    point_p->x = int( x1 * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    point_p->y = int( y1 * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
    point_p++;
  }
  if ( fill)
  {
    if ( fill_eq_border)
      drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
						 0, (GrowNode *)colornode, 0);
    else      
      drawtype = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
						 0, (GrowNode *)colornode, 1);
    if ( fill_eq_light && node && ((GrowNode *)node)->shadow)
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( drawtype, -shadow_contrast, 
						   (GrowNode *)colornode);
    else if ( fill_eq_shadow && node && ((GrowNode *)node)->shadow)
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( drawtype, shadow_contrast, 
						   (GrowNode *)colornode);
    glow_draw_nav_fill_polyline( ctx, points, a_points.a_size, drawtype);
  }
  int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow &&
    !fill_eq_light && !fill_eq_shadow;

  if ( display_shadow && shadow_width != 0) {
    glow_sShadowInfo *sp;
    int p_num;
    double trf_scale = trf.vertical_scale( t);
    int ish = int( shadow_width / 100 * trf_scale *
	  min((x_right - x_left)*ctx->nav_zoom_factor_x, (y_high - y_low)*ctx->nav_zoom_factor_y) + 0.5);

    if ( ish >= 1) {
      calculate_shadow( &sp, &p_num, ish, 0, colornode, 0);

      XPoint p[4];
      for ( i = 0; i < p_num - 1; i++) {
	p[0].x = points[i].x;
	p[0].y = points[i].y;
	p[1].x = sp[i].x;
	p[1].y = sp[i].y;
	p[3].x = points[i+1].x;
	p[3].y = points[i+1].y;
	p[2].x = sp[i+1].x;
	p[2].y = sp[i+1].y;

	glow_draw_nav_fill_polyline( ctx, p, 4, sp[i].drawtype);
      }
      free( sp);
    }
  }
  if ( border || !(fill || (display_shadow && shadow_width != 0))) {
    drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 0, (GrowNode *)colornode, 0);
    glow_draw_nav_polyline( ctx, points, a_points.a_size, drawtype, idx, 0);
  }
}

void GrowPolyLine::nav_erase(  GlowTransform *t, void *node)
{
  int i;
  int idx;
  if ( node && ((GrowNode *)node)->line_width)
    idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  double x1, y1;

  for ( i = 0; i < a_points.a_size; i++)
  {
    if (!t)
    {
      x1 = trf.x( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }
    else
    {
      x1 = trf.x( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }

    point_p->x = int( x1 * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    point_p->y = int( y1 * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
    point_p++;
  }
  int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow &&
    !fill_eq_light && !fill_eq_border;

  if ( border || !(fill || (display_shadow && shadow_width != 0)))
    glow_draw_nav_polyline_erase( ctx, points, a_points.a_size, idx);
  if ( fill || (display_shadow && shadow_width != 0))
    glow_draw_nav_fill_polyline( ctx, points, a_points.a_size, 
	glow_eDrawType_LineErase);
}


void GrowPolyLine::draw()
{
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowPolyLine::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
}


void GrowPolyLine::add_and_shift_y_value( double value)
{
  for ( int i = a_points.size() - 1; i > 0; i--)
  {
    ((GlowPoint *)a_points[i])->y = ((GlowPoint *)a_points[i-1])->y;
  }
  ((GlowPoint *)a_points[0])->y = value;
  zoom();
  nav_zoom();
}

void GrowPolyLine::add_and_shift_y_value_filled( double value)
{
  for ( int i = a_points.size() - 2; i > 1; i--)
  {
    ((GlowPoint *)a_points[i])->y = ((GlowPoint *)a_points[i-1])->y;
  }
  ((GlowPoint *)a_points[1])->y = value;
  zoom();
  nav_zoom();
}

void GrowPolyLine::align( double x, double y, glow_eAlignDirection direction)
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


void GrowPolyLine::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int i;
  int idx;
  if ( node && ((GrowNode *)node)->line_width)
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  double x1, y1;
  int jshadow = !disable_shadow && shadow_width != 0 && !fill_eq_light && !fill_eq_shadow;

  glow_sPoint *p = (glow_sPoint *)malloc(a_points.a_size * sizeof(glow_sPoint));

  for ( i = 0; i < a_points.a_size; i++)
  {
    if (!t)
    {
      x1 = trf.x( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }
    else
    {
      x1 = trf.x( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
      y1 = trf.y( t, ((GlowPoint *)a_points[i])->x, ((GlowPoint *)a_points[i])->y);
    }

    p[i].x = x1 * ctx->zoom_factor_x - ctx->offset_x;
    p[i].y = y1 * ctx->zoom_factor_y - ctx->offset_y;
    if ( jshadow) {
      points[i].x = int( p[i].x + 0.5);
      points[i].y = int( p[i].y + 0.5);
    }
  }

  int p_num;
  glow_sShadowInfo *sp = 0;
  if ( jshadow) {
    double trf_scale = trf.vertical_scale( t);
    int ish = int( shadow_width / 100 * trf_scale *
	  min((x_right - x_left)*ctx->zoom_factor_x, (y_high - y_low)*ctx->zoom_factor_y) + 0.5);

    calculate_shadow( &sp, &p_num, ish, 0, 0, 1);
  }
  int jborder =  border || !(fill || (!disable_shadow && shadow_width != 0));

  ((GrowCtx *)ctx)->export_jbean->polyline( p, a_points.a_size, fill, jborder,
		 fill_drawtype, draw_type, fill_eq_border, 
		 fill_eq_light, fill_eq_shadow, 
		 idx, jshadow, shadow, shadow_contrast, sp, p_num, pass, shape_cnt, node_cnt, fp);
  free( (char *) p);
  free( sp);
}

void GrowPolyLine::flip( double x0, double y0, glow_eFlipDirection dir)
{
  switch ( dir) {
  case glow_eFlipDirection_Horizontal:
    trf.store();
    set_scale( 1, -1, x0, y0, glow_eScaleType_FixPoint);
    break;
  case glow_eFlipDirection_Vertical:
    trf.store();
    set_scale( -1, 1, x0, y0, glow_eScaleType_FixPoint);
    break;
  }
}

void GrowPolyLine::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);
    original_border_drawtype = GlowColor::convert( version, original_border_drawtype);
    original_fill_drawtype = GlowColor::convert( version, original_fill_drawtype);
    fill_drawtype = GlowColor::convert( version, fill_drawtype);

    break;
  }
  }  
}









