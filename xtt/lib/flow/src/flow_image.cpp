#include "flow_std.h"

#include <float.h>

#include <iostream.h>
#include "flow_image.h"
#include "flow_draw.h"
extern "C" {
#include "co_dcli.h"
}

FlowImage::FlowImage( FlowCtx *flow_ctx, char *imagefile, double x = 0, double y = 0,
		      flow_mDisplayLevel display_lev = flow_mDisplayLevel_1) : 
	ctx(flow_ctx), ll(flow_ctx,x,y), ur(flow_ctx,x,y), display_level(display_lev), 
	image(0), original_image(0), 
	pixmap(0), nav_pixmap(0), clip_mask(0), nav_clip_mask(0), current_zoom_factor(0)
{
  imlib = ((draw_tCtx)ctx->draw_ctx)->imlib;
  if ( imagefile)
    insert_image( imagefile);
}

int FlowImage::insert_image( char *imagefile)
{
  char filename[120];
  bool found = false;

  strcpy( image_filename, imagefile);

  // Look for file in $pwrp_exe, $pwr_doc and $pwr_exe
  for ( int i = 0; i < 3; i++) {
    switch( i) {
    case 1: strcpy( filename, "$pwrp_exe/");  break;
    case 2: strcpy( filename, "$pwr_doc/");  break;
    case 3: strcpy( filename, "$pwr_exe/");  break;
    }
    strcat( filename, imagefile);
    dcli_translate_filename( filename, filename);
    FILE *fp = fopen( filename, "r");
    if ( !fp)
      continue;
    fclose( fp);
    found = true;
    break;
  }
  if ( !found)
    return 0;

#if defined IMLIB
  if ( pixmap) {
    Imlib_free_pixmap( imlib, pixmap);
    pixmap = 0;
  }
  if ( nav_pixmap) {
    Imlib_free_pixmap( imlib, nav_pixmap);  
    nav_pixmap = 0;
  }
  if ( image)
    Imlib_destroy_image( imlib, image);

  original_image = Imlib_load_image( imlib, filename);
  if ( !original_image) 
    return 0;
  // Make a copy
  image = Imlib_clone_image( imlib, original_image);

  current_width = int( image->rgb_width * ctx->zoom_factor / 
		       ctx->base_zoom_factor);
  current_height = int( image->rgb_height * ctx->zoom_factor / 
			ctx->base_zoom_factor);
  current_nav_width = int( image->rgb_width * ctx->nav_zoom_factor / 
			   ctx->base_zoom_factor);
  current_nav_height = int( image->rgb_height * ctx->nav_zoom_factor / 
			    ctx->base_zoom_factor);

  Imlib_render( imlib, image, current_width, current_height);
  pixmap = Imlib_move_image( imlib, image);
  clip_mask = Imlib_move_mask( imlib, image);

  Imlib_render( imlib, image, current_nav_width, current_nav_height);
  nav_pixmap = Imlib_move_image( imlib, image);
  nav_clip_mask = Imlib_move_mask( imlib, image);

  ur.posit( ll.x + double( current_width) / ctx->zoom_factor,
	    ll.y + double( current_height) / ctx->zoom_factor);
  current_zoom_factor = ctx->zoom_factor;
#endif
  return 1;
}

void FlowImage::zoom()
{
  ll.zoom();
  ur.zoom();
}

void FlowImage::nav_zoom()
{
  ll.nav_zoom();
  ur.nav_zoom();
}

void FlowImage::print_zoom()
{
  ll.print_zoom();
  ur.print_zoom();
}

void FlowImage::traverse( int x, int y)
{
  ll.traverse( x, y);
  ur.traverse( x, y);
}

void FlowImage::print( void *pos, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( fix_line_width && line_width < 0)
    return;
  if ( draw_type == flow_eDrawType_LineErase)
    return;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * line_width - 
		1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( !fill)
    ctx->print_ps->rect( ll.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	ll.print_z_y + ((FlowPoint *)pos)->print_z_y,
	ur.print_z_x - ll.print_z_x, ur.print_z_y - ll.print_z_y, 
	draw_type, idx);
  else
    ctx->print_ps->filled_rect( ll.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	ll.print_z_y + ((FlowPoint *)pos)->print_z_y,
	ur.print_z_x - ll.print_z_x, ur.print_z_y - ll.print_z_y, 
	draw_type, idx);
}

void FlowImage::save( ofstream& fp, flow_eSaveMode mode)
{
  fp << int(flow_eSave_Rect) << endl;
  fp << int(flow_eSave_Rect_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Rect_line_width) << FSPACE << line_width << endl;
  fp << int(flow_eSave_Rect_display_level) << FSPACE << int(display_level) << endl;
  fp << int(flow_eSave_Rect_ll) << endl;
  ll.save( fp, mode);
  fp << int(flow_eSave_Rect_ur) << endl;
  ur.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
}

void FlowImage::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Rect: break;
      case flow_eSave_Rect_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Rect_line_width: fp >> line_width; break;
      case flow_eSave_Rect_display_level: fp >> tmp; display_level = (flow_mDisplayLevel)tmp; break;
      case flow_eSave_Rect_ll: ll.open( fp); break;
      case flow_eSave_Rect_ur: ur.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowImage:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowImage::draw( void *pos, int highlight, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;

  if ( pixmap)
  {
#if defined IMLIB

    if ( fabs( current_zoom_factor - ctx->zoom_factor) > DBL_EPSILON)
    {
      current_zoom_factor = ctx->zoom_factor;
      current_width = int( image->rgb_width * ctx->zoom_factor / 
		       ctx->base_zoom_factor);
      current_height = int( image->rgb_height * ctx->zoom_factor / 
			ctx->base_zoom_factor);

      Imlib_render( imlib, image, current_width, current_height);

      Imlib_free_pixmap( imlib, pixmap);
      pixmap = Imlib_move_image( imlib, image);
      clip_mask = Imlib_move_mask( imlib, image);
    }
#endif

    flow_draw_image( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, ll.z_y + 
		     ((FlowPoint *)pos)->z_y - ctx->offset_y,
		     ur.z_x - ll.z_x, ur.z_y - ll.z_y, pixmap, clip_mask);
  }
  else
    flow_draw_fill_rect( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, ll.z_y + 
	((FlowPoint *)pos)->z_y - ctx->offset_y, 
	ur.z_x - ll.z_x, ur.z_y - ll.z_y, flow_eDrawType_LineGray);
}

void FlowImage::erase( void *pos, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  flow_draw_fill_rect( ctx, ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, ll.z_y + 
		       ((FlowPoint *)pos)->z_y - ctx->offset_y, 
		       ur.z_x - ll.z_x, ur.z_y - ll.z_y, flow_eDrawType_LineErase);
}

void FlowImage::nav_draw( void *pos, int highlight, void *node)
{
}

void FlowImage::nav_erase( void *pos, void *node)
{
}

int FlowImage::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  FlowPoint *p;

  p = (FlowPoint *) pos;
  if ( ll.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x <= x && 
       x <= ur.z_x  + ((FlowPoint *)pos)->z_x - ctx->offset_x &&
       ll.z_y  + ((FlowPoint *)pos)->z_y - ctx->offset_y <= y && 
       y <= ur.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y)
  {
//    cout << "Event handler: Hit in rect" << endl;
    return 1;
  }  
  else
    return 0;
}

void FlowImage::get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
{
  if ( display_level != flow_mDisplayLevel_1)
    return;
  if ( pos_x + ll.x < *x_left)
    *x_left = pos_x + ll.x;
  if ( pos_x + ur.x > *x_right)
    *x_right = pos_x + ur.x;
  if ( pos_y + ll.y < *y_low)
    *y_low = pos_y + ll.y;
  if ( pos_y + ur.y > *y_high)
    *y_high = pos_y + ur.y;
}

void FlowImage::move( void *pos, double x, double y, int highlight, int hot)
{
  double width, height;

  width = ur.x - ll.x;
  height = ur.y - ll.y;
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  ll.x = x;
  ll.y = y;
  ur.x = x + width;
  ur.y = y + height;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void FlowImage::shift( void *pos, double delta_x, double delta_y,
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

ostream& operator<< ( ostream& o, const FlowImage r)
{
  o << 
  '(' << r.ll.x << ',' << r.ll.y << ')' << 
  '(' << r.ur.x << ',' << r.ur.y << ')' << 
  '[' << r.ll.z_x << ',' << r.ll.z_y << ']' <<
  '[' << r.ur.z_x << ',' << r.ur.z_y << ']' ;
  return o;
}


