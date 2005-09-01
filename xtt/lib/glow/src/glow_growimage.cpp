/* 
 * Proview   $Id: glow_growimage.cpp,v 1.4 2005-09-01 14:57:53 claes Exp $
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


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#if defined OS_LINUX || defined OS_LYNX
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "glow_transform.h"
#include "glow_growimage.h"
#include "glow_grownode.h"
#include "glow_growctx.h"
#include "glow_draw.h"


#if defined IMLIB
static int rgb_tone( unsigned char *x0, unsigned char *y0, unsigned char *z0, int tone);
static int rgb_shift( unsigned char *x0, unsigned char *y0, unsigned char *z0, int shift);
#endif

extern "C" {
#include "co_dcli.h"
}
 
GrowImage::GrowImage( GlowCtx *glow_ctx, char *name, double x, double y, 
	        char *imagefile,
		glow_mDisplayLevel display_lev) : 
                ll(glow_ctx,x,y), ur(glow_ctx,x+1,y+1),
    		hot(0), pzero(glow_ctx), stored_pos(glow_ctx), 
                highlight(0), inverse(0), user_data(NULL),
		dynamic(0), dynamicsize(0), image(0), original_image(0), 
                pixmap(0), nav_pixmap(0), clip_mask(0), nav_clip_mask(0),
                ctx(glow_ctx), display_level(display_lev), 
		color_tone(glow_eDrawTone_No), color_lightness(0),
		color_intensity(0), color_shift(0), color_inverse(0),
		current_color_tone(glow_eDrawTone_No), current_color_lightness(0),
		current_color_intensity(0), current_color_shift(0), 
		current_color_inverse(0), current_direction(0),
		current_nav_color_tone(glow_eDrawTone_No), current_nav_color_lightness(0),
		current_nav_color_intensity(0), current_nav_color_shift(0), 
		current_nav_color_inverse(0), current_nav_direction(0)
{
  strcpy( n_name, name);
  strcpy( image_filename, "");
  strcpy( filename, "");
  strcpy( last_group, "");
  imlib = ((draw_tCtx)ctx->draw_ctx)->imlib;
  if ( imagefile)
    insert_image( imagefile);

  pzero.nav_zoom();

  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( ll.x, ll.y, &x_grid, &y_grid);
    ll.posit( x_grid, y_grid);
//    ctx->find_grid( ur.x, ur.y, &x_grid, &y_grid);
    if ( !pixmap)
      ur.posit( ll.x + 1, ll.y + 1);
    else
      ur.posit( ll.x + double( current_width) / ctx->zoom_factor_x,
	    ll.y + double( current_height) / ctx->zoom_factor_y);
  }
  draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  get_node_borders();
}

void GrowImage::copy_from( const GrowImage& im)
{
  memcpy( this, &im, sizeof(im));
  image = 0;
  pixmap = 0;
  nav_pixmap = 0;
  clip_mask = 0;
  nav_clip_mask = 0;
  if ( strcmp( image_filename, "") != 0) 
    insert_image( image_filename);
}

GrowImage::~GrowImage()
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

#if defined IMLIB
  if ( original_image)
    Imlib_destroy_image( imlib, original_image);
  if ( image)
    Imlib_destroy_image( imlib, image);
  if ( pixmap)
    Imlib_free_pixmap( imlib, pixmap);
  if ( nav_pixmap)
    Imlib_free_pixmap( imlib, nav_pixmap);
#endif
}

int GrowImage::insert_image( char *imagefile)
{
  int found = 0;
  char imagename[80];
  char *s;
  struct stat info;
  int sts;

  strcpy( image_filename, imagefile);

  // Find file
  if ( strncmp( image_filename, "jpwr/", 5) == 0) {
    if ( (s = strchr( &image_filename[5], '/')))
      strcpy( imagename, s+1);
    else
      strcpy( imagename, image_filename); 
  }
  else
    strcpy( imagename, image_filename);

  strcpy( filename, imagename);
  if ( check_file( filename))
    found = 1;

  if ( !found)
  {
    // Add some search path
    for ( int i = 0; i < ((GrowCtx *)ctx)->path_cnt; i++)
    {
      strcpy( filename, ((GrowCtx *)ctx)->path[i]);
      strcat( filename, imagename);
      dcli_translate_filename( filename, filename);
      if ( check_file( filename))
      {
        found = 1;
        break;
      }
    }
    if ( !found)
      return 0;
  }

#if defined IMLIB
  sts = stat( filename, &info);
  if ( sts == -1)
    return 0;

  date = info.st_ctime;

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

#if 0
  nav_image = Imlib_load_image( imlib, imagefile);
#endif

  current_width = int( image->rgb_width * ctx->zoom_factor_x / 
		       ctx->base_zoom_factor);
  current_height = int( image->rgb_height * ctx->zoom_factor_y / 
			ctx->base_zoom_factor);
  current_nav_width = int( image->rgb_width * ctx->nav_zoom_factor_x / 
			   ctx->base_zoom_factor);
  current_nav_height = int( image->rgb_height * ctx->nav_zoom_factor_y / 
			    ctx->base_zoom_factor);
  current_color_tone = color_tone;
  current_color_lightness = color_lightness;
  current_color_intensity = color_intensity;
  current_color_shift = color_shift;
  current_color_inverse = color_inverse;
  current_nav_color_tone = color_tone;
  current_nav_color_lightness = color_lightness;
  current_nav_color_intensity = color_intensity;
  current_nav_color_shift = color_shift;
  current_nav_color_inverse = color_inverse;

  set_image_color( image, NULL);
  Imlib_render( imlib, image, current_width, current_height);
  pixmap = Imlib_move_image( imlib, image);
  clip_mask = Imlib_move_mask( imlib, image);

  Imlib_render( imlib, image, current_nav_width, current_nav_height);
  nav_pixmap = Imlib_move_image( imlib, image);
  nav_clip_mask = Imlib_move_mask( imlib, image);

  ur.posit( ll.x + double( current_width) / ctx->zoom_factor_x,
	    ll.y + double( current_height) / ctx->zoom_factor_y);
#endif
  get_node_borders();
  return 1;
}

int GrowImage::update()
{

#if defined IMLIB
  struct stat info;
  int sts;

  sts = stat( filename, &info);
  if ( sts == -1)
    return 0;

  if ( date == info.st_ctime)
    return 0;
  date = info.st_ctime;

  if ( original_image)
    Imlib_kill_image( imlib, original_image);

  original_image = Imlib_load_image( imlib, filename);
  if ( !original_image) 
    return 0;

  if ( pixmap) {
    Imlib_free_pixmap( imlib, pixmap);
    pixmap = 0;
  }
  if ( nav_pixmap) {
    Imlib_free_pixmap( imlib, nav_pixmap);  
    nav_pixmap = 0;
  }

  // Make a copy
  if ( image)
    Imlib_kill_image( imlib, image);
  image = Imlib_clone_image( imlib, original_image);

  set_image_color( image, NULL);
  Imlib_render( imlib, image, current_width, current_height);
  pixmap = Imlib_move_image( imlib, image);

  draw();
  return 1;

#endif
  return 0;
}

void GrowImage::zoom()
{
  ll.zoom();
  ur.zoom();
}

void GrowImage::nav_zoom()
{
  ll.nav_zoom();
  ur.nav_zoom();
}

void GrowImage::print_zoom()
{
  ll.print_zoom();
  ur.print_zoom();
}

void GrowImage::traverse( int x, int y)
{
  ll.traverse( x, y);
  ur.traverse( x, y);
}

void GrowImage::move( double delta_x, double delta_y, int grid)
{
  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
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
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowImage::move_noerase( int delta_x, int delta_y, int grid)
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

int GrowImage::local_event_handler( glow_eEvent event, double x, double y)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = min( ll.x, ur.x);
  ur_x = max( ll.x, ur.x);
  ll_y = min( ll.y, ur.y);
  ur_y = max( ll.y, ur.y);

  if ( ll_x <= x && x <= ur_x &&
       ll_y <= y && y <= ur_y)
  {
//    cout << "Event handler: Hit in image" << endl;
    return 1;
  }  
  else
    return 0;
}

int GrowImage::event_handler( glow_eEvent event, double fx, double fy)
{
  double x, y;

  trf.reverse( fx, fy, &x, &y);
  return local_event_handler( event, x, y);
}

int GrowImage::event_handler( glow_eEvent event, int x, int y, double fx,
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
        redraw = 1;
        hot = 0;
      }
      if ( redraw)
      {
        if ( !inverse)
          ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
//          ((GlowImage *)this)->draw( (void *)&pzero, highlight, hot, NULL);
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


void GrowImage::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;

  fp << int(glow_eSave_GrowImage) << endl;
  fp << int(glow_eSave_GrowImage_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_GrowImage_image_filename) << FSPACE << image_filename << endl;
  fp << int(glow_eSave_GrowImage_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_GrowImage_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_GrowImage_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_GrowImage_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_GrowImage_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowImage_dynamic) << endl;
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
  fp << int(glow_eSave_GrowImage_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_GrowImage_display_level) << FSPACE << int(display_level) << endl;
  fp << int(glow_eSave_GrowImage_ll) << endl;
  ll.save( fp, mode);
  fp << int(glow_eSave_GrowImage_ur) << endl;
  ur.save( fp, mode);
  fp << int(glow_eSave_GrowImage_color_tone) << FSPACE << int(color_tone) << endl;
  fp << int(glow_eSave_GrowImage_color_lightness) << FSPACE << color_lightness << endl;
  fp << int(glow_eSave_GrowImage_color_intensity) << FSPACE << color_intensity << endl;
  fp << int(glow_eSave_GrowImage_color_shift) << FSPACE << color_shift << endl;
  fp << int(glow_eSave_End) << endl;
}

void GrowImage::open( ifstream& fp)
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
      case glow_eSave_GrowImage: break;
      case glow_eSave_GrowImage_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowImage_image_filename:
        fp.get();
        fp.getline( image_filename, sizeof(image_filename));
        break;
      case glow_eSave_GrowImage_x_right: fp >> x_right; break;
      case glow_eSave_GrowImage_x_left: fp >> x_left; break;
      case glow_eSave_GrowImage_y_high: fp >> y_high; break;
      case glow_eSave_GrowImage_y_low: fp >> y_low; break;
      case glow_eSave_GrowImage_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowImage_dynamic:
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
      case glow_eSave_GrowImage_trf: trf.open( fp); break;
      case glow_eSave_GrowImage_display_level: fp >> tmp; display_level = (glow_mDisplayLevel)tmp; break;
      case glow_eSave_GrowImage_ll: ll.open( fp); break;
      case glow_eSave_GrowImage_ur: ur.open( fp); break;
      case glow_eSave_GrowImage_color_tone: fp >> tmp; 
		color_tone = (glow_eDrawTone)tmp; break;
      case glow_eSave_GrowImage_color_lightness: fp >> color_lightness; break;
      case glow_eSave_GrowImage_color_intensity: fp >> color_intensity; break;
      case glow_eSave_GrowImage_color_shift: fp >> color_shift; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowImage:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }

  if ( strcmp( image_filename, "") != 0)
    insert_image( image_filename);
}

void GrowImage::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

  if ( x_right * ctx->zoom_factor_x - ctx->offset_x + 1 >= ll_x &&
      	x_left * ctx->zoom_factor_x - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor_y - ctx->offset_y + 1 >= ll_y &&
       	y_low * ctx->zoom_factor_y - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();
  }
}

void GrowImage::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowImage::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowImage::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowImage::set_highlight( int on)
{
  erase();

  highlight = on;
  draw();
}

void GrowImage::select_region_insert( double ll_x, double ll_y, double ur_x, 
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

void GrowImage::set_dynamic( char *code, int size)
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

void GrowImage::exec_dynamic()
{
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ((GrowCtx *)ctx)->dynamic_cb( this, dynamic, glow_eDynamicType_Object);
}

void GrowImage::set_position( double x, double y)
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

void GrowImage::set_scale( double scale_x, double scale_y, 
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

void GrowImage::set_rotation( double angel, 
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

void GrowImage::draw( GlowTransform *t, int highlight, int hot, void *node,
		      void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;

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

  if ( ll_x == ur_x || ll_y == ur_y)
    return;

  double rotation;
  int direction;

  if ( t)
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  else
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;

  if ( 45 >= rotation || rotation > 315)
    direction = 0;
  else if ( 45 < rotation && rotation <= 135)
    direction = -1;
  else if ( 135 < rotation && rotation <= 225)
    direction = 2;
  else // if ( 225 < rotation && rotation <= 315)
    direction = 1;

  if ( pixmap)
  {
#if defined IMLIB
    int sts;

    if ( (colornode && !(current_color_tone == ((GrowNode *)node)->color_tone &&
           current_color_lightness == ((GrowNode *)node)->color_lightness &&
	   current_color_intensity == ((GrowNode *)node)->color_intensity &&
           current_color_shift == ((GrowNode *)node)->color_shift &&
           current_color_inverse == ((GrowNode *)node)->color_inverse)) ||  
         ( !colornode && !(current_color_tone == color_tone &&
           current_color_lightness == color_lightness &&
	   current_color_intensity == color_intensity &&
           current_color_shift == color_shift &&
           current_color_inverse == color_inverse))) {
      set_image_color( image, colornode);
      current_direction = 0;
      sts = 1;
    }
    else
      sts = 0;

    if ( direction != current_direction) {
      if ( (direction == 0 && current_direction == 1) ||
	   (direction == 1 && current_direction == 0) ||
           (direction == 0 && current_direction == -1) ||
           (direction == -1 && current_direction == 0) ||
           (direction == 2 && current_direction == 1) ||
           (direction == 1 && current_direction == 2) ||
           (direction == 2 && current_direction == -1) ||
           (direction == -1 && current_direction == 2)) {
        Imlib_rotate_image( imlib, image, direction);
        // printf( "Rotating\n");
      }
      if ( (current_direction == 0 && direction == 2) ||
           (current_direction == 2 && direction == 0) ||
           (current_direction == 1 && direction == -1) ||
           (current_direction == -1 && direction == 1) ||
	   (current_direction == 0 && direction == 1) ||
	   (current_direction == 1 && direction == 2) ||
	   (current_direction == 2 && direction == -1) ||
           (current_direction == -1 && direction == 0)) {
        Imlib_flip_image_vertical( imlib, image);
	// printf( "Vertical flip\n");
      }
      if ( (current_direction == 0 && direction == 2) ||
           (current_direction == 2 && direction == 0) ||
           (current_direction == 1 && direction == -1) ||
           (current_direction == -1 && direction == 1) ||
           (current_direction == 0 && direction == -1) ||
           (current_direction == 1 && direction == 0) ||
	   (current_direction == -1 && direction == 2) ||
           (current_direction == 2 && direction == 1)) {
        Imlib_flip_image_horizontal( imlib, image);
        // printf( "Horizontal flip\n");
      }
      current_direction = direction;
      sts = 1;
    }

    if ( sts || ur_x - ll_x != current_width || ur_y - ll_y != current_height)
    {
      Imlib_render( imlib, image, ur_x - ll_x, ur_y - ll_y);

      Imlib_free_pixmap( imlib, pixmap);
      pixmap = Imlib_move_image( imlib, image);
      clip_mask = Imlib_move_mask( imlib, image);
      current_width = ur_x - ll_x;
      current_height = ur_y - ll_y;
      if ( colornode) {
        current_color_tone = ((GrowNode *)colornode)->color_tone;
        current_color_lightness = ((GrowNode *)colornode)->color_lightness;
        current_color_intensity = ((GrowNode *)colornode)->color_intensity;
        current_color_shift = ((GrowNode *)colornode)->color_shift;
        current_color_inverse = ((GrowNode *)colornode)->color_inverse;
      }
      else {
        current_color_tone = color_tone;
        current_color_lightness = color_lightness;
        current_color_intensity = color_intensity;
        current_color_shift = color_shift;
        current_color_inverse = color_inverse;
      }
    }
#endif

    glow_draw_image( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, pixmap, clip_mask);
  }
  else
    glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 
			 glow_eDrawType_LineGray);
  if ( highlight)
  {
    glow_draw_rect( ctx, ll_x, ll_y, ur_x - ll_x - 1, ur_y - ll_y - 1, 
		    glow_eDrawType_LineRed, 0, 0);
  }
  else if ( hot /*  && !((GrowCtx *)ctx)->enable_bg_pixmap */)
    glow_draw_rect( ctx, ll_x, ll_y, ur_x - ll_x - 1, ur_y - ll_y - 1, 
		    glow_eDrawType_LineGray, 0, 0);

}

void GrowImage::erase( GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
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
  if ( ((GrowCtx *)ctx)->enable_bg_pixmap)
    glow_draw_background( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y);
  else
    glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  ctx->reset_draw_buffer_only();
}

void GrowImage::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
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

  if ( ll_x == ur_x || ll_y == ur_y)
    return;

  if ( nav_pixmap)
  {
#if defined IMLIB
    int sts;

    if ( (colornode && !(current_color_tone == ((GrowNode *)node)->color_tone &&
           current_color_lightness == ((GrowNode *)colornode)->color_lightness &&
	   current_color_intensity == ((GrowNode *)colornode)->color_intensity &&
           current_color_shift == ((GrowNode *)colornode)->color_shift &&
           current_color_inverse == ((GrowNode *)colornode)->color_inverse)) ||  
         ( !node && !(current_color_tone == color_tone &&
           current_color_lightness == color_lightness &&
	   current_color_intensity == color_intensity &&
           current_color_shift == color_shift &&
           current_color_inverse == color_inverse))) {   
      set_image_color( image, node);
      sts = 1;
    }
    else
      sts = 0;

    if ( sts || ur_x - ll_x != current_nav_width || ur_y - ll_y != current_nav_height)
    {
      Imlib_render( imlib, image, ur_x - ll_x, ur_y - ll_y);

      Imlib_free_pixmap( imlib, nav_pixmap);
      nav_pixmap = Imlib_move_image( imlib, image);
      nav_clip_mask = Imlib_move_mask( imlib, image);
      current_nav_width = ur_x - ll_x;
      current_nav_height = ur_y - ll_y;
      if ( colornode) {
        current_color_tone = ((GrowNode *)colornode)->color_tone;
        current_color_lightness = ((GrowNode *)colornode)->color_lightness;
        current_color_intensity = ((GrowNode *)colornode)->color_intensity;
        current_color_shift = ((GrowNode *)colornode)->color_shift;
        current_color_inverse = ((GrowNode *)colornode)->color_inverse;
      }
      else {
        current_color_tone = color_tone;
        current_color_lightness = color_lightness;
        current_color_intensity = color_intensity;
        current_color_shift = color_shift;
        current_color_inverse = color_inverse;
      }
    }
#endif

    glow_draw_nav_image( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, nav_pixmap,
			 nav_clip_mask);
  }
  else
    glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	glow_eDrawType_LineGray);
}

void GrowImage::nav_erase( GlowTransform *t, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
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

  glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	glow_eDrawType_LineErase);
}

void GrowImage::draw()
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

void GrowImage::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  double ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

  if ( t)
  {
    x1 = trf.x( t, ll.x, ll.y);
    x2 = trf.x( t, ur.x, ur.y);
    y1 = trf.y( t, ll.x, ll.y);
    y2 = trf.y( t, ur.x, ur.y);
  }
  else
  {
    x1 = trf.x( ll.x, ll.y);
    x2 = trf.x( ur.x, ur.y);
    y1 = trf.y( ll.x, ll.y);
    y2 = trf.y( ur.x, ur.y);
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( display_level != glow_mDisplayLevel_1)
    return;
  if ( ll_x < *x_left)
    *x_left = ll_x;
  if ( ur_x > *x_right)
    *x_right = ur_x;
  if ( ll_y < *y_low)
    *y_low = ll_y;
  if ( ur_y > *y_high)
    *y_high = ur_y;
}

void GrowImage::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
}


void GrowImage::align( double x, double y, glow_eAlignDirection direction)
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


void GrowImage::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  if ( !(display_level & ctx->display_level))
    return;
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double rot;
  int transparent = 0;

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
    rot = trf.rot();
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
    rot = trf.rot(t);
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);
  if ( clip_mask)
    transparent = 1;

  ((GrowCtx *)ctx)->export_jbean->image( ll_x, ll_y, ur_x, ur_y, image_filename,
    	transparent, color_tone, color_lightness,
    	color_intensity, color_shift, rot,
    	pass, shape_cnt, node_cnt, in_nc, fp);
}
#if 0
int GrowImage::set_image_color( ImlibImage *im, void *n)
{
  GrowNode *node = (GrowNode *) n;
  glow_eDrawTone c_color_tone;
  int		c_color_lightness;
  int		c_color_intensity;
  int		c_color_shift;
  int		c_color_inverse;
  int           curve_red[256];
  int           curve_blue[256];
  int           curve_green[256];
  unsigned char color_curve[256];
  int           i;
  double        factor;
  double        red_k;
  int           red_m;
  double        blue_k;
  int           blue_m;
  double        green_k;
  int           green_m;

  if ( node) {
    c_color_lightness = node->color_lightness;
    c_color_tone = node->color_tone;
    c_color_inverse = node->color_inverse;
    c_color_shift = node->color_shift;
    c_color_intensity = node->color_intensity;
  }
  else {
    c_color_lightness = color_lightness;
    c_color_tone = color_tone;
    c_color_inverse = color_inverse;
    c_color_shift = color_shift;
    c_color_intensity = color_intensity;
  }

  if ( !(c_color_tone == glow_eDrawTone_No || c_color_tone == glow_eDrawTone_Gray ||
       c_color_tone >= glow_eDrawTone__)) {
    switch ( c_color_tone) {
      case glow_eDrawTone_YellowGreen:
        red_k   = 0.4;
        red_m   = 174;
        blue_k  = 1.2;
        blue_m  = -70;
        green_k = 0.5;
	green_m = 148;
        break;
      case glow_eDrawTone_Yellow:
        red_k   = 0.4;
        red_m   = 154;
        blue_k  = 1.2;
        blue_m  = -70;
        green_k = 0.83;
	green_m = 40;
        break;
      case glow_eDrawTone_Orange:
        red_k   = 0.32;
        red_m   = 174;
        blue_k  = 1.2;
        blue_m  = -70;
        green_k = 0.9;
	green_m = 26;
        break;
      case glow_eDrawTone_Red:
        red_k   = 0.25;
        red_m   = 180;
        blue_k  = 1.2;
        blue_m  = -70;
        green_k = 1.2;
	green_m = -70;
        break;
      case glow_eDrawTone_Magenta:
        red_k   = 0.5;
        red_m   = 128;
        blue_k  = 0.5;
        blue_m  = 128;
        green_k = 1.2;
	green_m = -70;
        break;
      case glow_eDrawTone_Blue:
        red_k   = 1.2;
        red_m   = -70;
        blue_k  = 0.25;
        blue_m  = 180;
        green_k = 1.2;
	green_m = -70;
        break;
      case glow_eDrawTone_Seablue:
        red_k   = 1.2;
        red_m   = -70;
        blue_k  = 0.5;
        blue_m  = 128;
        green_k = 0.5;
	green_m = 128;
        break;
      case glow_eDrawTone_Green:
        red_k   = 1.2;
        red_m   = -70;
        blue_k  = 1.2;
        blue_m  = -70;
        green_k = 0.25;
	green_m = 180;
        break;
      case glow_eDrawTone_DarkGray:
        red_k   = 0.8;
        red_m   = 0;
        blue_k  = 0.8;
        blue_m  = 0;
        green_k = 0.8;
	green_m = 0;
        break;
      default:
        ;
    }
    for ( i = 0; i < 256; i++) {
      curve_red[i] = int( i * red_k) + red_m;
      curve_blue[i] = int( i * blue_k) + blue_m;
      curve_green[i] = int( i * green_k) + green_m;
    }
  }
  else {
    for ( i = 0; i < 256; i++) {
      curve_red[i] = i;
      curve_blue[i] = i;
      curve_green[i] = i;
    }
  }

  if ( c_color_intensity) {
    if ( c_color_intensity > 0)
      factor = 1 + 0.1 * c_color_intensity;
    else
      factor = 1 + 0.1 * c_color_intensity;
    for ( i = 0; i < 256; i++) {
      curve_red[i] = int( factor * curve_red[i]) - c_color_intensity * 25;
      curve_blue[i] = int( factor * curve_blue[i]) - c_color_intensity * 25;
      curve_green[i] = int( factor * curve_green[i]) - c_color_intensity * 25;;
    }
  }

  if ( c_color_lightness) {
    if ( c_color_lightness > 0) {
      factor = 1 - 0.1 * c_color_lightness;
      for ( i = 0; i < 256; i++) {
        curve_red[i] = int( factor * curve_red[i]) + c_color_lightness * 25;
        curve_blue[i] = int( factor * curve_blue[i]) + c_color_lightness * 25;
        curve_green[i] = int( factor * curve_green[i]) + c_color_lightness * 25;
      }
    }
    else {
      factor = 1 + 0.1 * c_color_lightness;
      for ( i = 0; i < 256; i++) {
        curve_red[i] = int( factor * curve_red[i]);
        curve_blue[i] = int( factor * curve_blue[i]);
        curve_green[i] = int( factor * curve_green[i]);
      }
    }
  }

  if ( c_color_inverse) {
    for ( i = 0; i < 256; i++) {
      curve_red[i] = - curve_red[i] + 255;
      curve_blue[i] = - curve_blue[i] + 255;
      curve_green[i] = - curve_green[i] + 255;
    }
  }

  for ( i = 0; i < 256; i++) {
    if ( curve_red[i] > 255)
      curve_red[i] = 255;
    if ( curve_red[i] < 0)
      curve_red[i] = 0;
    color_curve[i] = (unsigned char) curve_red[i];
  }
  Imlib_set_image_red_curve( imlib, im, color_curve);

  for ( i = 0; i < 256; i++) {
    if ( curve_blue[i] > 255)
      curve_blue[i] = 255;
    if ( curve_blue[i] < 0)
      curve_blue[i] = 0;
    color_curve[i] = (unsigned char) curve_blue[i];
  }
  Imlib_set_image_blue_curve( imlib, im, color_curve);

  for ( i = 0; i < 256; i++) {
    if ( curve_green[i] > 255)
      curve_green[i] = 255;
    if ( curve_green[i] < 0)
      curve_green[i] = 0;
    color_curve[i] = (unsigned char) curve_green[i];
  }
  Imlib_set_image_green_curve( imlib, im, color_curve);

  return 1;
}
#endif

int GrowImage::set_image_color( ImlibImage *im, void *n)
{
#if defined IMLIB
  GrowNode *node = (GrowNode *) n;
  glow_eDrawTone c_color_tone;
  int		c_color_lightness;
  int		c_color_intensity;
  int		c_color_shift;
  int		c_color_inverse;
  int           i;
  unsigned char *rgb;
  int           value;
  float         factor_intens, factor_light;
  unsigned char transp[3] = {255,0,255};
  int           m;


  if ( node) {
    c_color_lightness = node->color_lightness;
    c_color_tone = node->color_tone;
    c_color_inverse = node->color_inverse;
    c_color_shift = node->color_shift;
    c_color_intensity = node->color_intensity;
  }
  else {
    c_color_lightness = color_lightness;
    c_color_tone = color_tone;
    c_color_inverse = color_inverse;
    c_color_shift = color_shift;
    c_color_intensity = color_intensity;
  }


  if ( image)
    Imlib_destroy_image( imlib, image);
  image = Imlib_clone_image( imlib, original_image);
  im = image;

  if ( !(c_color_tone == glow_eDrawTone_No || c_color_tone >= glow_eDrawTone__) ||
       c_color_shift) {

    rgb = im->rgb_data;
    for ( i = 0; i < im->rgb_height * im->rgb_width * 3; i+=3) {
      if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2]) {
        rgb += 3;
        continue;
      }
      if ( !(c_color_tone == glow_eDrawTone_No || c_color_tone >= glow_eDrawTone__)) {
        rgb_tone( rgb, rgb+1, rgb+2, c_color_tone);
      }
      if ( c_color_shift) {
        rgb_shift( rgb, rgb+1, rgb+2, c_color_shift);
      }
      rgb += 3;
    }
  }

  if ( c_color_intensity > 0)
    factor_intens = 1 + 0.1 * c_color_intensity;
  else
    factor_intens = 1 + 0.1 * c_color_intensity;

  if ( c_color_lightness > 0)
    factor_light = 1 - 0.1 * c_color_lightness;
  else
    factor_light = 1 + 0.1 * c_color_lightness;

  if ( c_color_intensity || c_color_lightness || c_color_inverse) {
    rgb = im->rgb_data;
    for ( i = 0; i < im->rgb_height * im->rgb_width * 3; i++) {
      if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2]) {
        rgb += 3;
	i += 2;
        continue;
      }

      if ( c_color_intensity) {
        value = int( factor_intens * *rgb) - c_color_intensity * 25;
        if ( value > 255)
          *rgb = 255;
        else if ( value < 0)
          *rgb = 0;
        else
          *rgb = value;
      }

      if ( c_color_lightness) {
        if ( c_color_lightness > 0) {
          value = int( factor_light * *rgb) + c_color_lightness * 25;
          if ( value < 0)
            *rgb = 0;
          else
            *rgb = value;
        }
        else {
          value = int( factor_light * *rgb);
          if ( value > 255)
            *rgb = 255;
          else
            *rgb = value;
	}
      }
      
      if ( c_color_inverse) {
	//        *rgb = 255 - *rgb;
        if ( i % 3 == 0) {
          m = ((int)(*rgb) + *(rgb+1) + *(rgb+2)) / 3;
	  value = 255 - m + ((int)*rgb - m);
          if ( value < 0)
            value = 0;
          if ( value > 255)
            value = 255;
          *rgb = (unsigned char) value;
	  value = 255 - m + ((int)*(rgb+1) - m);
          if ( value < 0)
            value = 0;
          if ( value > 255)
            value = 255;
          *(rgb+1) = (unsigned char) value;
	  value = 255 - m + ((int)*(rgb+2) - m);
          if ( value < 0)
            value = 0;
          if ( value > 255)
            value = 255;
          *(rgb+2) = (unsigned char) value;
        }
      }
      rgb++;
    }
  }

  Imlib_changed_image( imlib, image);
#endif
  return 1;
}

#if defined IMLIB
static int rgb_tone( unsigned char *x0, unsigned char *y0, unsigned char *z0, int tone)
{
  int a1, b2;
  int tmp, m;
  float ka1, kb1;
  int ka2, kb2;

  m = ((int)*x0 + *y0 + *z0) / 3;

  ka1 = 0.6; /* 0.6 */
  ka2 = 120; /* 130 */
  kb1 = 0.4;
  kb2 = 0;  
  if ( m > 75)
    a1 = (int)(m * ka1) + ka2;
  else
    a1 = (int)(m * 1.5);
  if ( a1 > 255)
    a1 = 255;
  b2 = int(kb1 * m) + kb2;
  if ( b2 < 0)
    b2 = 0;


  switch ( tone) {
    case glow_eDrawTone_Gray:
      *x0 = m;
      *y0 = m;
      *z0 = m;
      break;      
    case glow_eDrawTone_YellowGreen:
      *x0 = a1;
      *y0 = a1;
      *z0 = b2;
       break;
    case glow_eDrawTone_Yellow:
      tmp = int( 1.2 * a1);
      if  (tmp <= 255)
        *x0 = tmp;
      else
        *x0 = 255;
      *y0 = (unsigned char)(0.9 * a1);
      *z0 = b2;
       break;
    case glow_eDrawTone_Orange:
      tmp = (int)(1.2 * a1);
      if  (tmp <= 255)
        *x0 = (unsigned char)tmp;
      else
        *x0 = 255;
      *y0 = (unsigned char)(0.6 * a1);
      *z0 = b2;
       break;
    case glow_eDrawTone_Red:
      *x0 = a1;
      *y0 = b2;
      *z0 = b2;
       break;
    case glow_eDrawTone_Magenta:
      *x0 = a1;
      *y0 = b2;
      *z0 = a1;
       break;
    case glow_eDrawTone_Blue:
      *x0 = b2;
      *y0 = b2;
      *z0 = a1;
       break;
    case glow_eDrawTone_Seablue:
      *x0 = b2;
      *y0 = a1;
      *z0 = a1;
       break;
    case glow_eDrawTone_Green:
      *x0 = b2;
      *y0 = a1;
      *z0 = b2;
       break;
    case glow_eDrawTone_DarkGray:
      *x0 = (unsigned char)(m / 1.3);
      *y0 = (unsigned char)(m / 1.3);
      *z0 = (unsigned char)(m / 1.3);
       break;
  }
  return 1;
}
#endif

#if defined IMLIB
static int rgb_shift( unsigned char *x0, unsigned char *y0, unsigned char *z0, int shift)
{
  unsigned char x, y, z;
  int d;
  int step;

  shift = -shift;
  shift %= 8;
  if ( shift < 0)
    shift += 8;

  x = *x0;
  y = *y0;
  z = *z0;

  for( ;;) {
    if ( x == y && y == z)
      break;
    if ( x > y && y >= z && x > z) {
      d = x - z;
      step = 6 * d / 10 * shift;
//      printf("Section 1, d: %d, step: %d, lap: %d\n", d, step, 6 * d);

      if ( step <= z + d - y) {
        y += step;
        break;
      }
      step -= z + d - y;
      y = z + d;
      if ( step <= d) {
        x -= step;
	break;
      }
      x -= d;
      step -= d;
      if ( step <= d) {
        z += step; 
	break;
      }
      z += d;
      step -= d;
      if ( step <= d) {
        y -= step; 
	break;
      }
      y -= d;
      step -= d;
      if ( step <= d) {
        x += step;
	break;
      }
      x += d;
      step -= d;
      if ( step <= d) {
        z -= step;
	break;
      }
      z -= d;
      step -= d;
      if ( step <= d) {
        y += step;
	break;
      }
      y += d;
      printf( "Error, shift larger than one lap\n");
      break;
    }
    else if ( x > z && y >= x && y > z) {
      d = y - z;
      step = 6 * d / 10 * shift;
//      printf("Section 2, d: %d, step: %d\n", d, step);

      if ( step <= x - z) {
        x -= step;
        break;
      }
      step -= x - z;
      x = z;
      if ( step <= d) {
        z += step;
	break;
      }
      z += d;
      step -= d;
      if ( step <= d) {
        y -= step; 
	break;
      }
      y -= d;
      step -= d;
      if ( step <= d) {
        x += step; 
	break;
      }
      x += d;
      step -= d;
      if ( step <= d) {
        z -= step;
	break;
      }
      z -= d;
      step -= d;
      if ( step <= d) {
        y += step;
	break;
      }
      y += d;
      step -= d;
      if ( step <= d) {
        x -= step;
	break;
      }
      x -= d;
      printf( "Error, shift larger than one lap\n");
      break;
    }
    else if ( y > z && z >= x && y > x) {
      d = y - x;
      step = 6 * d / 10 * shift;
//      printf("Section 3, d: %d, step: %d\n", d, step);

      if ( step <= x + d - z) {
        z += step;
        break;
      }
      step -= x + d - z;
      z = x  + d;
      if ( step <= d) {
        y -= step;
	break;
      }
      y -= d;
      step -= d;
      if ( step <= d) {
        x += step; 
	break;
      }
      x += d;
      step -= d;
      if ( step <= d) {
        z -= step; 
	break;
      }
      z -= d;
      step -= d;
      if ( step <= d) {
        y += step;
	break;
      }
      y += d;
      step -= d;
      if ( step <= d) {
        x -= step;
	break;
      }
      x -= d;
      step -= d;
      if ( step <= d) {
        z += step;
	break;
      }
      z += d;
      printf( "Error, shift larger than one lap\n");
      break;
    }
    else if ( z >= y && y > x && z >= y) {
      d = z - x;
      step = 6 * d / 10 * shift;
//      printf("Section 4, d: %d, step: %d\n", d, step);

      if ( step <= y - x) {
        y -= step;
	break;
      }
      step -= y - x;
      y = x;
      if ( step <= d) {
        x += step;
	break;
      }
      x += d;
      step -= d;
      if ( step <= d) {
        z -= step; 
	break;
      }
      z -= d;
      step -= d;
      if ( step <= d) {
        y += step; 
	break;
      }
      y += d;
      step -= d;
      if ( step <= d) {
        x -= step;
	break;
      }
      x -= d;
      step -= d;
      if ( step <= d) {
        z += step;
	break;
      }
      z += d;
      step -= d;
      if ( step <= d) {
        y -= step;
	break;
      }
      y -= d;
      printf( "Error, shift larger than one lap\n");
      break;
    }
    else if ( z > x && x >= y && z > y) {
      d = z - y;
      step = 6 * d / 10 * shift;
//      printf("Section 5, d: %d, step: %d\n", d, step);

      if ( step <= y + d - x) {
        x += step;
        break;
      }
      step -= y + d - x;
      x = y + d;
      if ( step <= d) {
        z -= step;
	break;
      }
      z -= d;
      step -= d;
      if ( step <= d) {
        y += step; 
	break;
      }
      y += d;
      step -= d;
      if ( step <= d) {
        x -= step; 
	break;
      }
      x -= d;
      step -= d;
      if ( step <= d) {
        z += step;
	break;
      }
      z += d;
      step -= d;
      if ( step <= d) {
        y -= step;
	break;
      }
      y -= d;
      step -= d;
      if ( step <= d) {
        x += step;
	break;
      }
      x += d;
      printf( "Error, shift larger than one lap\n");
      break;
    }
    else /* if ( x >= z && z > y && x > y) */ {
      d = x - y;
      step = 6 * d / 8 * shift;
//      printf("Section 6, d: %d, step: %d\n", d, step);
      if ( d < 0)
        printf( "d: %d ( %d, %d, %d)\n", d, x, y, z);

      if ( step <= z - y) {
        z -= step;
        break;
      }
      step -= z - y;
      z = y;
      if ( step <= d) {
        y += step;
	break;
      }
      y += d;
      step -= d;
      if ( step <= d) {
        x -= step; 
	break;
      }
      x -= d;
      step -= d;
      if ( step <= d) {
        z += step; 
	break;
      }
      z += d;
      step -= d;
      if ( step <= d) {
        y -= step;
	break;
      }
      y -= d;
      step -= d;
      if ( step <= d) {
        x += step;
	break;
      }
      x += d;
      step -= d;
      if ( step <= d) {
        z -= step;
	break;
      }
      z -= d;
      printf( "Error, shift larger than one lap\n");
      break;
    }
   
  }
  *x0 = x;
  *y0 = y;
  *z0 = z;
  return 1;
}
#endif

int grow_image_to_pixmap( GlowCtx *ctx, char *imagefile, 
	    int width, int height, Pixmap *pixmap, int *w, int *h)
{
  int found = 0;
  char imagename[80];
  char filename[120];
  char *s;
  ImlibData  *imlib;  

  imlib = ((draw_tCtx)ctx->draw_ctx)->imlib;

  // Find file
  if ( strncmp( imagefile, "jpwr/", 5) == 0) {
    if ( (s = strchr( &imagefile[5], '/')))
      strcpy( imagename, s+1);
    else
      strcpy( imagename, imagefile); 
  }
  else
    strcpy( imagename, imagefile);

  strcpy( filename, imagename);
  if ( check_file( filename))
    found = 1;

  if ( !found)
  {
    // Add some search path
    for ( int i = 0; i < ((GrowCtx *)ctx)->path_cnt; i++)
    {
      strcpy( filename, ((GrowCtx *)ctx)->path[i]);
      strcat( filename, imagename);
      dcli_translate_filename( filename, filename);
      if ( check_file( filename))
      {
        found = 1;
        break;
      }
    }
    if ( !found)
      return 0;
  }

#if defined IMLIB
  ImlibImage *image;

  image = Imlib_load_image( imlib, filename);
  if ( !image) 
    return 0;

  if ( width == 0 || height == 0) {
    width = image->rgb_width;
    height = image->rgb_height;
  }
  Imlib_render( imlib, image, width, height);
  *pixmap = Imlib_move_image( imlib, image);
  Imlib_kill_image( imlib, image);
  *w = width;
  *h = height;

#endif
  return 1;
}
