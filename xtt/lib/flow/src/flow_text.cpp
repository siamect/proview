#include "flow_std.h"


#include <iostream.h>
#include "flow_text.h"
#include "flow_draw.h"

void FlowText::zoom()
{
  p.zoom();
}

void FlowText::nav_zoom()
{
  p.nav_zoom();
}

void FlowText::print_zoom()
{
  p.print_zoom();
}

void FlowText::traverse( int x, int y)
{
  p.traverse( x, y);
}

void FlowText::print( void *pos, void *node, int highlight)
{
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);

  if ( size <= 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->print_ps->text( p.print_z_x + ((FlowPoint *)pos)->print_z_x , 
	p.print_z_y + ((FlowPoint *)pos)->print_z_y, text, strlen(text),
	draw_type, size);
}

void FlowText::save( ofstream& fp, flow_eSaveMode mode)
{
  fp << int(flow_eSave_Text) << endl;
  fp << int(flow_eSave_Text_text_size) << FSPACE << text_size << endl;
  fp << int(flow_eSave_Text_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Text_text) << FSPACE << text << endl;
  fp << int(flow_eSave_Text_p) << endl;
  p.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
}

void FlowText::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Text: break;
      case flow_eSave_Text_text_size: fp >> text_size; break;
      case flow_eSave_Text_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Text_text: 
        fp.get();
        fp.getline( text, sizeof(text));
        break;
      case flow_eSave_Text_p: p.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowText:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowText::draw( void *pos, int highlight, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_text( ctx, p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, text, strlen(text),
	draw_type, idx, highlight, 0);
}

void FlowText::erase( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_text_erase( ctx, p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, text,  strlen(text),
	draw_type, idx, 0);
}

void FlowText::nav_draw( void *pos, int highlight, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * (text_size +4) 
		- 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_nav_text( ctx, 
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	text, strlen(text), draw_type, idx, highlight, 0);
}

void FlowText::nav_erase( void *pos, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * (text_size +4) -
		 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_nav_text_erase( ctx,
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	text, strlen(text), draw_type, idx, 0);
}

int FlowText::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  FlowPoint *p;

  p = (FlowPoint *) pos;
/***
  if ( p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x < x && 
       x < p2.z_x  + ((FlowPoint *)pos)->z_x - ctx->offset_x &&
       p1.z_y  + ((FlowPoint *)pos)->z_y - ctx->offset_y < y && 
       y < p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y)
  {
    cout << "Event handler: Hit in text" << endl;
    return 1;
  }  
  else
***/
    return 0;
}

void FlowText::get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
{
/*
  if ( pos_x + p1.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p2.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p1.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_x + p2.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_y + p1.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p2.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p1.y > *y_high)
    *y_high = pos_y + p2.y;
  if ( pos_y + p2.y > *y_high)
    *y_high = pos_y + p2.y;
*/
}

void FlowText::move( void *pos, double x, double y, int highlight, int hot)
{

  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p.x = x;
  p.y = y;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void FlowText::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p.x += delta_x;
  p.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

ostream& operator<< ( ostream& o, const FlowText t)
{
  o << 
  '(' << t.p.x << ',' << t.p.y << ')' << t.text;
  return o;
}

