#include "glow_std.h"


#include <stdlib.h>
#include <iostream.h>
#include "glow_line.h"
#include "glow_draw.h"
#include "glow_growctx.h"

void GlowLine::zoom()
{
  p1.zoom();
  p2.zoom();
}

void GlowLine::nav_zoom()
{
  p1.nav_zoom();
  p2.nav_zoom();
}

void GlowLine::print_zoom()
{
  p1.print_zoom();
  p2.print_zoom();
}

void GlowLine::traverse( int x, int y)
{
  p1.traverse( x, y);
  p2.traverse( x, y);
}

void GlowLine::print( void *pos, void *node)
{
  if ( p1.print_z_x == p2.print_z_x && p1.print_z_y == p2.print_z_y)
    return;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
	line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ctx->print_ps->line( p1.print_z_x + ((GlowPoint *)pos)->print_z_x, 
	p1.print_z_y + ((GlowPoint *)pos)->print_z_y, 
	p2.print_z_x + ((GlowPoint *)pos)->print_z_x, 
	p2.print_z_y + ((GlowPoint *)pos)->print_z_y,
	draw_type, idx);
}

void GlowLine::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Line) << endl;
  fp << int(glow_eSave_Line_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Line_line_width) << FSPACE << line_width << endl;
  fp << int(glow_eSave_Line_p1) << endl;
  p1.save( fp, mode);
  fp << int(glow_eSave_Line_p2) << endl;
  p2.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowLine::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Line: break;
      case glow_eSave_Line_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Line_line_width: fp >> line_width; break;
      case glow_eSave_Line_p1: p1.open( fp); break;
      case glow_eSave_Line_p2: p2.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowLine:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowLine::draw( void *pos, int hightlight, int hot, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_line( ctx, p1.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p1.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	p2.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p2.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y,
	draw_type, idx, hightlight);
}

void GlowLine::erase( void *pos, int hot, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_line_erase( ctx, p1.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p1.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	p2.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p2.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, idx);
}

void GlowLine::nav_draw( void *pos, int highlight, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_line( ctx, 
	p1.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p1.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p2.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p2.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y,
	draw_type, idx, highlight);
}

void GlowLine::nav_erase( void *pos, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_line_erase( ctx,
	p1.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p1.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p2.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p2.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y,
	idx);
}

int GlowLine::event_handler( void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
  int x1 = p1.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  int x2 = p2.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  int y1 = p1.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;
  int y2 = p2.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;

  if ((x1 == x2 && y1 < y2 && 		// Vertical
       abs( x1 - x) < 3 && 
       y1 < y && y < y2)
	||
      (x1 == x2 && y1 > y2 && 		// Vertical
       abs( x1 - x) < 3 && 
       y2 < y && y < y1)
	||
      (y1 == y2 && x1 < x2 &&		// Horizontal
       abs( y1 - y) < 3 && 
       x1 < x && x < x2)
        ||
      (y1 == y2 && x1 > x2 &&		// Horizontal
       abs( y1 - y) < 3 && 
       x2 < x && x < x1))
  {
//    cout << "Event handler: Hit in line" << endl;
    return 1;
  }  
  else if (( !(x1 == x2 || y1 == y2) && x1 < x2 && x1 <= x && x <= x2 &&
             abs(y - 1.0 * (y2-y1)/(x2-x1) * x - y1 + 1.0 * (y2-y1)/(x2-x1) * x1) < 3)
           ||
           ( !(x1 == x2 || y1 == y2) && x1 > x2 && x2 <= x && x <= x1 &&
             abs(y - 1.0 * (y2-y1)/(x2-x1) * x - y1 + 1.0 * (y2-y1)/(x2-x1) * x1) < 3))
  {
//    cout << "Event handler: Hit in line" << endl;
    return 1;
  }
  else
    return 0;
}

void GlowLine::get_borders( double pos_x, double pos_y,
	double *x_right, double *x_left, double *y_high, double *y_low, 
	void *node)
{
  if ( pos_x + p1.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p2.x < *x_left)
    *x_left = pos_x + p2.x;
  if ( pos_x + p1.x > *x_right)
    *x_right = pos_x + p1.x;
  if ( pos_x + p2.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_y + p1.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p2.y < *y_low)
    *y_low = pos_y + p2.y;
  if ( pos_y + p1.y > *y_high)
    *y_high = pos_y + p1.y;
  if ( pos_y + p2.y > *y_high)
    *y_high = pos_y + p2.y;

}

void GlowLine::move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p1.x = x1;
  p1.y = y1;
  p2.x = x2;
  p2.y = y2;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void GlowLine::move_noerase( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot)
{
  p1.x = x1;
  p1.y = y1;
  p2.x = x2;
  p2.y = y2;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void GlowLine::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p1.x += delta_x;
  p1.y += delta_y;
  p2.x += delta_x;
  p2.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void GlowLine::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ((GrowCtx *)ctx)->export_jbean->line( 
	p1.z_x - ctx->offset_x, 
	p1.z_y - ctx->offset_y, 
	p2.z_x - ctx->offset_x, 
	p2.z_y - ctx->offset_y,
	draw_type, idx, pass, shape_cnt, node_cnt, fp);
  (*shape_cnt)++;
}

ostream& operator<< ( ostream& o, const GlowLine l)
{
  o << 
  '(' << l.p1.x << ',' << l.p1.y << ')' << 
  '(' << l.p2.x << ',' << l.p2.y << ')' << 
  '[' << l.p1.z_x << ',' << l.p1.z_y << ']' <<
  '[' << l.p2.z_x << ',' << l.p2.z_y << ']' ;
  return o;
}

