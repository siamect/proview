/* 
 * Proview   $Id: glow_growslider.cpp,v 1.6 2008-10-31 12:51:36 claes Exp $
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
#include "glow_growslider.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_conpoint.h"

#define SLIDER_BACKGROUND 21

GrowSlider::GrowSlider( GrowCtx *glow_ctx, const char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw, int rel_annot_pos) :
	GrowNode(glow_ctx,name,node_class,x1,y1,nodraw,rel_annot_pos),
	direction(glow_eDirection_Down),
	max_value(100), min_value(0), max_pos(0), min_pos(0)
{  
}

void GrowSlider::copy_from( const GrowSlider& n) 
{
  memcpy( this, &n, sizeof(n));
  for ( int i = 0; i < 10; i++) {
    if ( annotsize[i]) {
      annotv[i] = (char *) calloc( 1, annotsize[i]);
      memcpy( annotv[i], n.annotv[i], annotsize[i]);
    }
  }
  if ( n.dynamicsize) {
    dynamic = (char *) calloc( 1, n.dynamicsize);
    memcpy( dynamic, n.nc->dynamic, n.dynamicsize);
  }
  if ( ctx->userdata_copy_callback)
    (ctx->userdata_copy_callback)( this, user_data, &user_data, glow_eUserdataCbType_Node);
//  nc = new GlowNodeClass( *nc);
}

void GrowSlider::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowSlider) << endl;
  fp << int(glow_eSave_GrowSlider_direction) << FSPACE << int(direction) << endl;
  fp << int(glow_eSave_GrowSlider_max_value) << FSPACE << max_value << endl;
  fp << int(glow_eSave_GrowSlider_min_value) << FSPACE << min_value << endl;
  fp << int(glow_eSave_GrowSlider_max_pos) << FSPACE << max_pos << endl;
  fp << int(glow_eSave_GrowSlider_min_pos) << FSPACE << min_pos << endl;
  fp << int(glow_eSave_GrowSlider_grownode_part) << endl;
  GrowNode::save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowSlider::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;) {
    if ( !fp.good()) {
      fp.clear();
      fp.getline( dummy, sizeof(dummy));
      printf( "** Read error GrowSlider: \"%d %s\"\n", type, dummy);      
    }

    fp >> type;
    switch( type) {
      case glow_eSave_GrowSlider: break;
      case glow_eSave_GrowSlider_direction: fp >> tmp; 
	direction = (glow_eDirection)tmp; break;
      case glow_eSave_GrowSlider_max_value: fp >> max_value; break;
      case glow_eSave_GrowSlider_min_value: fp >> min_value; break;
      case glow_eSave_GrowSlider_max_pos: fp >> max_pos; break;
      case glow_eSave_GrowSlider_min_pos: fp >> min_pos; break;
      case glow_eSave_GrowSlider_grownode_part: 
        GrowNode::open( fp);
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowSlider:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowSlider::get_info( glow_eDirection *dir, double *max_val, double *min_val,
		double *max_position, double *min_position)
{
  *dir = direction;
  *max_val = max_value;
  *min_val = min_value;
  *max_position = max_pos;
  *min_position = min_pos;
}

void GrowSlider::get_info_pixel( glow_eDirection *dir, double *max_position,
				 double *min_position, int bg_dyn_type)
{
  GlowArrayElem *background;
  glow_eDirection bg_dir;
  double bg_max, bg_min;
  int bg_found;
  int sts;
  double origo;

  sts = ctx->get_background_object_limits(
               (glow_eTraceType) bg_dyn_type,
	       (x_right + x_left) / 2, (y_low + y_high) / 2, 
	       &background, &bg_min, &bg_max, &bg_dir);
  if ( ODD(sts))
    bg_found = 1;
  else
    bg_found = 0;

  if ( !bg_found) {
    *dir = direction;
    if ( direction == glow_eDirection_Left ||
         direction == glow_eDirection_Right) {
      *max_position = max_pos * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
      *min_position = min_pos * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    }
    else {
      *max_position = max_pos * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
      *min_position = min_pos * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    }
  }
  else {
    *dir = bg_dir;
    get_origo( bg_dir, &origo);
    switch ( bg_dir) {
      case glow_eDirection_Right: 
        *max_position = (bg_max - origo) * ctx->mw.zoom_factor_x- ctx->mw.offset_x;
        *min_position = (bg_min - origo) * ctx->mw.zoom_factor_x- ctx->mw.offset_x;
        break;
      case glow_eDirection_Left: 
        *max_position = (bg_max - (x_right - x_left - origo)) * 
	    ctx->mw.zoom_factor_x- ctx->mw.offset_x;
        *min_position = (bg_min - (x_right - x_left - origo)) * 
	    ctx->mw.zoom_factor_x- ctx->mw.offset_x;
        break;
      case glow_eDirection_Down: 
        *max_position = (bg_max - origo) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
        *min_position = (bg_min - origo) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
        break;
      case glow_eDirection_Up: 
        *max_position = (bg_max - (y_high - y_low - origo)) * 
	    ctx->mw.zoom_factor_y - ctx->mw.offset_y;
        *min_position = (bg_min - (y_high - y_low - origo)) * 
	    ctx->mw.zoom_factor_y - ctx->mw.offset_y;
        break;
      default:
        ;
    }
  }
}

void GrowSlider::get_origo( glow_eDirection direction, double *pos)
{
  GlowTransform t;
  double x1_right = 10e-37;
  double x1_left = 10e37;
  double y1_high = 10e-37;
  double y1_low = 10e37;

  // Get borders in the nodeclass koordinates
  nc->get_borders( &t, &x1_right, &x1_left, &y1_high, &y1_low);
  switch ( direction) {
    case glow_eDirection_Down:
    case glow_eDirection_Up:
      if ( fabs(y1_high - y1_low) < DBL_EPSILON)
        *pos = y_low;
      else
        *pos = (nc->y0 - y1_low) * (y_high - y_low) / (y1_high - y1_low);
      break;
    case glow_eDirection_Left:
    case glow_eDirection_Right:
      if ( fabs(y1_high - y1_low) < DBL_EPSILON)
        *pos = y_low;
      else
        *pos = (nc->y0 - y1_low) * (x_right - x_left) / (y1_high - y1_low);
      break;     
    default:
      ;
  }
}

void GrowSlider::set_info( glow_eDirection dir, double max_val, double min_val,
		double max_position, double min_position)
{
  direction = dir;
  max_value = max_val;
  min_value = min_val;
  max_pos = max_position;
  min_pos = min_position;
}

void GrowSlider::set_range( double min_val, double max_val)
{
  max_value = max_val;
  min_value = min_val;
  draw();
}

void GrowSlider::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2, rot;
  char java_name[40];

  if (!t) {
    x1 = x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    rot = trf.rot();
  }
  else {
    x1 = t->x( x_left, y_low) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = t->y( x_left, y_low) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = t->x( x_right, y_high) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = t->y( x_right, y_high) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    rot = trf.rot(t);
  }

  nc->get_java_name( java_name);
  ctx->export_jbean->slider( x1, y1, x2, y2, java_name, 
    	draw_type, fill_drawtype, text_drawtype, color_tone, color_lightness,
    	color_intensity, color_shift, line_width, rot, shadow,
    	pass, shape_cnt, node_cnt, in_nc, fp);
}





