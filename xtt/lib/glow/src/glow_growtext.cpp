/* 
 * Proview   $Id: glow_growtext.cpp,v 1.6 2007-05-07 14:35:03 claes Exp $
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
#include "glow_growtext.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowText::GrowText( GrowCtx *glow_ctx, char *name, char *text1, double x, double y, 
		glow_eDrawType d_type, glow_eDrawType color_d_type, int t_size, 
		glow_eFont t_font, glow_mDisplayLevel display_lev, int nodraw) : 
		GlowText(glow_ctx,text1,x,y,d_type,color_d_type,t_size,display_lev), 
    		hot(0), pzero(ctx), highlight(0), inverse(0), user_data(NULL),
		dynamic(0), dynamicsize(0), original_color_drawtype(glow_eDrawType_Line),
		font(t_font)
		
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();
  strcpy( last_group, "");

  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( p.x, p.y, &x_grid, &y_grid);
    p.posit( x_grid, y_grid);
  }
  get_node_borders();
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
}

GrowText::~GrowText()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase( &ctx->mw);
  erase( &ctx->navw);

  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  if ( hot)
    ctx->gdraw->set_cursor( &ctx->mw, glow_eDrawCursor_Normal);
}

void GrowText::move( double delta_x, double delta_y, int grid)
{
  ctx->set_defered_redraw();
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->find_grid( x_left + delta_x / ctx->mw.zoom_factor_x,
	y_low + delta_y / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    erase( &ctx->mw);
    erase( &ctx->navw);
    dx = delta_x / ctx->mw.zoom_factor_x;
    dy = delta_y / ctx->mw.zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  ctx->redraw_defered();

}

void GrowText::move_noerase( int delta_x, int delta_y, int grid)
{
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    ctx->find_grid( x_left + double( delta_x) / ctx->mw.zoom_factor_x,
	y_low + double( delta_y) / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    dx = double( delta_x) / ctx->mw.zoom_factor_x;
    dy = double( delta_y) / ctx->mw.zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);

}

int GrowText::local_event_handler( glow_eEvent event, double x, double y)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = x_left;
  ll_y = y_low;
  ur_x = x_right;
  ur_y = y_high;

  if ( ll_x <= x && x <= ur_x &&
       ll_y <= y && y <= ur_y)
  {
//    cout << "Event handler: Hit in rect" << endl;
    return 1;
  }  
  else
    return 0;
}

int GrowText::event_handler( GlowWind *w, glow_eEvent event, double fx, double fy)
{
  return local_event_handler( event, fx, fy);
}

int GrowText::event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;

  sts = 0;
  if ( event == ctx->event_move_node) {
    sts = local_event_handler( event, fx, fy);
    if ( sts) {
      /* Register node for potential movement */
      ctx->move_insert( this);
    }
    return sts;
  }
  switch ( event) {
    case glow_eEvent_CursorMotion: {
      int redraw = 0;

      if ( ctx->hot_mode == glow_eHotMode_TraceAction)
        sts = 0;
      else if ( ctx->hot_found)
        sts = 0;
      else {
        sts = local_event_handler( event, fx, fy);
        if ( sts)
          ctx->hot_found = 1;
      }
      if ( sts && !hot  &&
	   !(ctx->node_movement_active || ctx->node_movement_paste_active)) {
        ctx->gdraw->set_cursor( w, glow_eDrawCursor_CrossHair);
        hot = 1;
        redraw = 1;
      }
      if ( !sts && hot) {
        if ( !ctx->hot_found)
          ctx->gdraw->set_cursor( w, glow_eDrawCursor_Normal);
        erase( w);
        hot = 0;
        redraw = 1;
      }
      if ( redraw) {
	ctx->draw( w, x_left * w->zoom_factor_x - w->offset_x - DRAW_MP,
	     y_low * w->zoom_factor_y - w->offset_y - DRAW_MP,
  	     x_right * w->zoom_factor_x - w->offset_x + DRAW_MP,
	     y_high * w->zoom_factor_y - w->offset_y + DRAW_MP);
      }
      break;
    }
    default:
      sts = local_event_handler( event, fx, fy);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowText::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;

  fp << int(glow_eSave_GrowText) << endl;
  fp << int(glow_eSave_GrowText_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_GrowText_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_GrowText_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_GrowText_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_GrowText_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_GrowText_original_color_drawtype) << FSPACE 
		<< int(original_color_drawtype) << endl;
  fp << int(glow_eSave_GrowText_font) << FSPACE << int(font) << endl;
  fp << int(glow_eSave_GrowText_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowText_dynamic) << endl;
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
  fp << int(glow_eSave_GrowText_text_part) << endl;
  GlowText::save( fp, mode);
  fp << int(glow_eSave_GrowText_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowText::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		j;
  char		c;
  char          *new_text;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowText: break;
      case glow_eSave_GrowText_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowText_x_right: fp >> x_right; break;
      case glow_eSave_GrowText_x_left: fp >> x_left; break;
      case glow_eSave_GrowText_y_high: fp >> y_high; break;
      case glow_eSave_GrowText_y_low: fp >> y_low; break;
      case glow_eSave_GrowText_original_color_drawtype: fp >> 
		tmp; original_color_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowText_font: fp >> tmp; font = (glow_eFont)tmp; break;
      case glow_eSave_GrowText_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowText_dynamic:
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
      case glow_eSave_GrowText_text_part: 
        GlowText::open( fp);
        break;
      case glow_eSave_GrowText_trf: trf.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowText:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }

  // Translate the text to current language
  if ( ctx->translate_on && 
       ctx->event_callback[glow_eEvent_Translate]) {
    if ( ctx->translate_cb( this, text, &new_text))
      strcpy( text, new_text);
      get_node_borders();
  }
}

void GrowText::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
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

  if ( x_right * w->zoom_factor_x - w->offset_x >= ll_x &&
      	x_left * w->zoom_factor_x - w->offset_x <= ur_x &&
       	y_high * w->zoom_factor_y - w->offset_y >= ll_y &&
       	y_low * w->zoom_factor_y - w->offset_y <= ur_y) {
    draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  }
}

void GrowText::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowText::set_highlight( int on)
{
  if ( highlight) {
    erase( &ctx->mw);
    erase( &ctx->navw);
  }
  highlight = on;
  draw();
}

void GrowText::select_region_insert( double ll_x, double ll_y, double ur_x, 
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

void GrowText::set_dynamic( char *code, int size)
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

void GrowText::exec_dynamic()
{
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ctx->dynamic_cb( this, dynamic, glow_eDynamicType_Object);
}

void GrowText::set_position( double x, double y)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.a13 == x && trf.a23 == y)
     return;
  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase( &ctx->mw);
  erase( &ctx->navw);
  trf.posit( x, y);
  get_node_borders();
  ctx->draw( &ctx->mw, old_x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     old_y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     old_x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     old_y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);

}

void GrowText::set_scale( double scale_x, double scale_y, 
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
  erase( &ctx->mw);
  erase( &ctx->navw);
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
  ctx->draw( &ctx->mw, old_x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     old_y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     old_x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     old_y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowText::set_rotation( double angel, 
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
  erase( &ctx->mw);
  erase( &ctx->navw);
  trf.rotate_from_stored( angel, x0, y0);
  get_node_borders();
  ctx->draw( &ctx->mw, old_x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     old_y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     old_x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     old_y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowText::draw( GlowWind *w,  GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int x1, y1;
  int z_width, z_height, z_descent;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * w->zoom_factor_y / w->base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int highl = highlight;
  if ( node)
    highl = ((GrowNode *)node)->highlight; 

  if ( w == &ctx->navw) {
    if ( strcmp( text, "") == 0)
      return;
    highl = 0;
  }

  if (!t) {
    x1 = int( trf.x( p.x, p.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( p.x, p.y) * w->zoom_factor_y + 0.5) - w->offset_y;
  }
  else {
    x1 = int( trf.x( t, p.x, p.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( t, p.x, p.y) * w->zoom_factor_y + 0.5) - w->offset_y;
  }

  if ( strcmp( text, "")) {
    if ( highl) {
      ctx->gdraw->get_text_extent( text, strlen(text), draw_type, max( 0, idx), font,
		&z_width, &z_height, &z_descent);
      ctx->gdraw->rect( w, x1, y1 - (z_height-z_descent), z_width, z_height, 
	glow_eDrawType_FillHighlight, max( 1, min( idx + hot, 2)), 0);
    }
    else if ( hot && !node) {
      ctx->gdraw->get_text_extent( text, strlen(text), draw_type, max( 0, idx), font,
		&z_width, &z_height, &z_descent);
      ctx->gdraw->rect( w, x1, y1 - (z_height-z_descent), z_width, z_height,
	glow_eDrawType_LineGray, max( min(idx,2), 1), 0);
    }
    if ( idx >= 0) {
      glow_eDrawType color = ctx->get_drawtype( color_drawtype, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 2);
      ctx->gdraw->text( w, x1, y1, text, strlen(text), draw_type, color, idx, highlight, 
			0, font);
    }
  }
  else {
    ctx->gdraw->get_text_extent( "A", 1, draw_type, idx, font, &z_width, &z_height,
	&z_descent);
    ctx->gdraw->rect( w, x1, y1 - (z_height-z_descent), z_width, z_height, 
	glow_eDrawType_LineGray, idx, 0);
  }
}

void GrowText::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int x1, y1;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * w->zoom_factor_y / w->base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int z_width, z_height, z_descent;
  int highl = highlight;
  if ( node)
    highl = ((GrowNode *)node)->highlight; 

  if (!t)
  {
    x1 = int( trf.x( p.x, p.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( p.x, p.y) * w->zoom_factor_y + 0.5) - w->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, p.x, p.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( t, p.x, p.y) * w->zoom_factor_y + 0.5) - w->offset_y;
  }

  // w->set_draw_buffer_only();
  if ( strcmp( text, "")) {
    if ( highl) {
      ctx->gdraw->get_text_extent( text, strlen(text), draw_type, max( 0, idx),
		 font, &z_width, &z_height, &z_descent);
      ctx->gdraw->rect_erase( w, x1, y1 - (z_height-z_descent), z_width, z_height, 
		max( 1, min( idx + hot, 2)));
    }
    else if ( hot && !node) {
      ctx->gdraw->get_text_extent( text, strlen(text), draw_type, max( 0, idx), 
		font, &z_width, &z_height, &z_descent);
      ctx->gdraw->rect_erase( w, x1, y1 - (z_height-z_descent), z_width, z_height, 
		max (1, min(idx,2)));
    }
    if ( idx >= 0)
      ctx->gdraw->text_erase( w, x1, y1, text, strlen(text), draw_type, idx, 0, font);
  }
  else
  {
    ctx->gdraw->get_text_extent( "A", 1, draw_type, idx, font, &z_width, &z_height, 
	&z_descent);
    ctx->gdraw->rect_erase( w, x1, y1 - (z_height-z_descent), z_width, z_height, idx);
  }
  // w->reset_draw_buffer_only();
}

void GrowText::draw()
{
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowText::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  double ll_x, ur_x, ll_y, ur_y;
  int	z_width, z_height, z_descent;

  if ( t) {
    ll_x = trf.x( t, p.x, p.y);
    ur_y = trf.y( t, p.x, p.y);
  }
  else {
    ll_x = trf.x( p.x, p.y);
    ur_y = trf.y( p.x, p.y);
  }

  if ( strcmp( text, "") == 0)
    ctx->gdraw->get_text_extent( "A", 1, draw_type, text_size, font,
		&z_width, &z_height, &z_descent);
  else {
    double trf_scale = trf.vertical_scale( t);
    int idx = int( trf_scale * ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * (text_size +4) - 4);
    idx = max( 0, min( idx, DRAW_TYPE_SIZE-1));
    
    ctx->gdraw->get_text_extent( text, strlen(text), draw_type, idx, font,
		&z_width, &z_height, &z_descent);
  }
  ur_x = ll_x + double( z_width) / ctx->mw.zoom_factor_y;
  ll_y = ur_y - double( z_height - z_descent) / ctx->mw.zoom_factor_y;
  ur_y += double( z_descent) / ctx->mw.zoom_factor_y;

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

void GrowText::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
}

void GrowText::set_text( char *new_text) 
{ 
  double y_low_old = y_low;
  double y_high_old = y_high;
  double x_left_old = x_left;
  double x_right_old = x_right;

  erase( &ctx->mw);
  erase( &ctx->navw);
  strncpy( text, new_text, sizeof(text)-1);
  get_node_borders();
  // draw();

  y_low_old = min( y_low, y_low_old);
  y_high_old = max( y_high, y_high_old);
  x_left_old = min( x_left, x_left_old);
  x_right_old = max( x_right, x_right_old);
  ctx->draw( &ctx->mw, x_left_old * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low_old * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right_old * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high_old * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left_old * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low_old * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right_old * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high_old * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);

}

void GrowText::set_textsize( int size) 
{ 
  erase( &ctx->mw);
  erase( &ctx->navw);
  text_size = size;
  get_node_borders();
  draw();
}

void GrowText::set_textbold( int bold) 
{ 
  if ( ( bold && draw_type == glow_eDrawType_TextHelveticaBold) ||
       ( !bold && draw_type == glow_eDrawType_TextHelvetica))
    return;

  erase( &ctx->mw);
  erase( &ctx->navw);
  if ( bold)
    draw_type = glow_eDrawType_TextHelveticaBold;
  else
    draw_type = glow_eDrawType_TextHelvetica;
  get_node_borders();
  draw();
}

void GrowText::set_textfont( glow_eFont textfont) 
{ 
  erase( &ctx->mw);
  erase( &ctx->navw);
  font = textfont;
  get_node_borders();
  draw();
}

void GrowText::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int x1, y1;
  int bold;
  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t) {
    x1 = int( trf.x( p.x, p.y) * ctx->mw.zoom_factor_x) - ctx->mw.offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->mw.zoom_factor_y) - ctx->mw.offset_y;
  }
  else {
    x1 = int( trf.x( t, p.x, p.y) * ctx->mw.zoom_factor_x) - ctx->mw.offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->mw.zoom_factor_y) - ctx->mw.offset_y;
  }

  bold = (draw_type == glow_eDrawType_TextHelveticaBold);

  ctx->export_jbean->text( x1, y1, text, 
	draw_type, color_drawtype, bold, idx, pass, shape_cnt, node_cnt, fp);
}

void GrowText::flip( double x0, double y0, glow_eFlipDirection dir)
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

void GrowText::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->set_defered_redraw();
    draw();
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
    ctx->redraw_defered();
}

void GrowText::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);

    break;
  }
  }  
}
