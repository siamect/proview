#include "flow_std.h"


#include <iostream.h>
#include "flow_pixmap.h"
#include "flow_draw.h"

FlowPixmap::FlowPixmap( FlowCtx *flow_ctx, flow_sPixmapData *pix_data,
	 double x, double y, flow_eDrawType d_type, int size) :
	ctx(flow_ctx), p(flow_ctx,x,y), pixmap_size(size), draw_type(d_type)
{
  if ( pix_data == NULL)
    return;
  memcpy( pixmap_data, pix_data, sizeof( pixmap_data));
  flow_draw_pixmaps_create( ctx, pix_data, &pixmaps);
}

FlowPixmap::FlowPixmap( const FlowPixmap& p)
{
  memcpy( this, &p, sizeof(p));
  flow_draw_pixmaps_create( ctx, (flow_sPixmapData *) pixmap_data, &pixmaps);
}

FlowPixmap::~FlowPixmap()
{
  flow_draw_pixmaps_delete( ctx, pixmaps);
}

void FlowPixmap::zoom()
{
  p.zoom();
}

void FlowPixmap::nav_zoom()
{
  p.nav_zoom();
}

void FlowPixmap::print_zoom()
{
  p.print_zoom();
}

void FlowPixmap::traverse( int x, int y)
{
  p.traverse( x, y);
}

void FlowPixmap::print( void *pos, void *node)
{
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);

  if ( size <= 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);

  ctx->print_ps->pixmap( p.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	p.print_z_y + ((FlowPoint *)pos)->print_z_y,
	&pixmap_data[pixmap_size],
	draw_type);
}

void FlowPixmap::save( ofstream& fp, flow_eSaveMode mode)
{
#if 0
  fp << int(flow_eSave_Pixmap) << endl;
  fp << int(flow_eSave_Pixmap_pixmap_size) << FSPACE << pixmap _size << endl;
  fp << int(flow_eSave_Pixmap_draw_type) << FSPACE << draw_type << endl;
  fp << int(flow_eSave_Pixmap_text) << FSPACE << text << endl;
  fp << int(flow_eSave_Pixmap_p) << endl;
  p.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
#endif
}

void FlowPixmap::open( ifstream& fp)
{
#if 0
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Pixmap: break;
      case flow_eSave_Pixmap_pixmap_size: fp >> pixmap_size; break;
      case flow_eSave_Pixmap_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Pixmap_text: 
        fp.get();
        fp.getline( text, sizeof(text));
        break;
      case flow_eSave_Pixmap_p: p.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowPixmap:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
#endif
}

void FlowPixmap::draw( void *pos, int highlight, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (pixmap_size + 4) - 
		4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_pixmap( ctx, p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, &pixmap_data, pixmaps,
	draw_type, idx, highlight, 0);
}

void FlowPixmap::draw_inverse( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (pixmap_size + 4) -
	 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_pixmap_inverse( ctx, p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, &pixmap_data, pixmaps,
	draw_type, idx, 0);
}

void FlowPixmap::erase( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * (pixmap_size + 4) -
	 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_pixmap_erase( ctx, p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, &pixmap_data, pixmaps,
	draw_type, idx, 0);
}

void FlowPixmap::nav_draw( void *pos, int highlight, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_nav_pixmap( ctx, 
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&pixmap_data, pixmaps, draw_type, idx, highlight, 0);
}

void FlowPixmap::nav_erase( void *pos, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  flow_draw_nav_pixmap_erase( ctx,
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&pixmap_data, pixmaps, draw_type, idx, 0);
}

int FlowPixmap::event_handler( void *pos, flow_eEvent event, int x, int y,
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

void FlowPixmap::get_borders( double pos_x, double pos_y, double *x_right, 
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

void FlowPixmap::move( void *pos, double x, double y, int highlight, int hot)
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

void FlowPixmap::shift( void *pos, double delta_x, double delta_y,
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

ostream& operator<< ( ostream& o, const FlowPixmap t)
{
  o << 
  '(' << t.p.x << ',' << t.p.y << ')';
  return o;
}
