#include "flow_std.h"


#include <iostream.h>
#include "flow_arc.h"
#include "flow_draw.h"

void FlowArc::zoom()
{
  ll.zoom();
  ur.zoom();
}

void FlowArc::nav_zoom()
{
  ll.nav_zoom();
  ur.nav_zoom();
}

void FlowArc::print_zoom()
{
  ll.print_zoom();
  ur.print_zoom();
}

void FlowArc::traverse( int x, int y)
{
  ll.traverse( x, y);
  ur.traverse( x, y);
}

void FlowArc::print( void *pos, void *node)
{
  double idx = ctx->print_zoom_factor / ctx->base_zoom_factor * 
		line_width;
  idx = MAX( 0.5, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->print_ps->arc( ll.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	ll.print_z_y + ((FlowPoint *)pos)->print_z_y,
	ur.print_z_x - ll.print_z_x, ur.print_z_y - ll.print_z_y, angel1, angel2,
	draw_type, idx);
}

void FlowArc::save( ofstream& fp, flow_eSaveMode mode)
{
  fp << int(flow_eSave_Arc) << endl;
  fp << int(flow_eSave_Arc_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Arc_line_width) << FSPACE << line_width << endl;
  fp << int(flow_eSave_Arc_angel1) << FSPACE << angel1 << endl;
  fp << int(flow_eSave_Arc_angel2) << FSPACE << angel2 << endl;
  fp << int(flow_eSave_Arc_ll) << endl;
  ll.save( fp, mode);
  fp << int(flow_eSave_Arc_ur) << endl;
  ur.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
}

void FlowArc::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Arc: break;
      case flow_eSave_Arc_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Arc_line_width: fp >> line_width; break;
      case flow_eSave_Arc_angel1: fp >> angel1; break;
      case flow_eSave_Arc_angel2: fp >> angel2; break;
      case flow_eSave_Arc_ll: ll.open( fp); break;
      case flow_eSave_Arc_ur: ur.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowArc:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowArc::draw( void *pos, int highlight, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_arc( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	ll.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	ur.z_x - ll.z_x, ur.z_y - ll.z_y, angel1, angel2,
	draw_type, idx, highlight);
}

void FlowArc::erase( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_arc_erase( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	ll.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	ur.z_x - ll.z_x, ur.z_y - ll.z_y, angel1, angel2, idx);
}

void FlowArc::nav_draw( void *pos, int highlight, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_nav_arc( ctx, 
	ll.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	ll.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	ur.nav_z_x - ll.nav_z_x, ur.nav_z_y - ll.nav_z_y, angel1, angel2,
	draw_type, idx, highlight);
}

void FlowArc::nav_erase( void *pos, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_nav_arc_erase( ctx,
	ll.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	ll.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	ur.nav_z_x - ll.nav_z_x, ur.nav_z_y - ll.nav_z_y, angel1, angel2,
	idx);
}

int FlowArc::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  FlowPoint *p;

  p = (FlowPoint *) pos;
  if ( angel2 == 360 &&
       ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x <= x && 
       x <= ur.z_x  + ((FlowPoint *)pos)->z_x - ctx->offset_x &&
       ll.z_y  + ((FlowPoint *)pos)->z_y - ctx->offset_y <= y && 
       y <= ur.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y)
  {
    return 1;
  }  
  else
    return 0;
}

void FlowArc::get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
{
  if ( pos_x + ll.x < *x_left)
    *x_left = pos_x + ll.x;
  if ( pos_x + ur.x > *x_right)
    *x_right = pos_x + ur.x;
  if ( pos_y + ll.y < *y_low)
    *y_low = pos_y + ll.y;
  if ( pos_y + ur.y > *y_high)
    *y_high = pos_y + ur.y;
}

void FlowArc::move( void *pos, double x1, double y1, double x2, double y2,
	int ang1, int ang2, int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  ll.x = x1;
  ll.y = y1;
  ur.x = x2;
  ur.y = y2;
  angel1 = ang1;
  angel2 = ang2;  
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void FlowArc::shift( void *pos, double delta_x, double delta_y, 
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  ll.x += delta_x;
  ll.y += delta_y;
  ur.x += delta_x;
  ur.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

ostream& operator<< ( ostream& o, const FlowArc a)
{
  o << 
  '(' << a.ll.x << ',' << a.ll.y << ')' << 
  '(' << a.ur.x << ',' << a.ur.y << ')' << 
  '[' << a.ll.z_x << ',' << a.ll.z_y << ']' <<
  '[' << a.ur.z_x << ',' << a.ur.z_y << ']' ;
  return o;
}

