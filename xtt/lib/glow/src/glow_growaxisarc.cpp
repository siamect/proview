/* 
 * Proview   $Id$
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
#include "glow_growaxisarc.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "pwr.h"
#include "co_time.h"

GrowAxisArc::GrowAxisArc( GrowCtx *glow_ctx, const char *name, double x1, double y1, 
			  double x2, double y2, int ang1, int ang2, glow_eDrawType border_d_type, 
			  int line_w, int t_size, glow_eDrawType t_drawtype, 
			  int nodraw) : 
  GrowArc(glow_ctx,name,x1,y1,x2,y2,ang1, ang2, border_d_type,line_w,
	  0,0,0, glow_eDrawType_Line,1),
  text_size(t_size), text_drawtype(t_drawtype), text_color_drawtype(glow_eDrawType_Line),
  max_value(100), min_value(0), lines(11), linelength(0.15),
  longquotient(1), valuequotient(1), increment(0)
{
  strcpy( format, "%3.0f");

  configure();
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);

}

GrowAxisArc::~GrowAxisArc()
{
  if ( ctx->nodraw) return;
  erase( &ctx->mw);
  erase( &ctx->navw);
}

void GrowAxisArc::configure() 
{
  if ( lines <= 1)
    lines = 2;
  if ( longquotient <= 0)
    longquotient = 1;
  if ( valuequotient <= 0)
    valuequotient = 1;
  increment = (max_value - min_value) / (lines - 1);
}

void GrowAxisArc::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowAxisArc) << endl;
  fp << int(glow_eSave_GrowAxisArc_max_value) << FSPACE << max_value << endl;
  fp << int(glow_eSave_GrowAxisArc_min_value) << FSPACE << min_value << endl;
  fp << int(glow_eSave_GrowAxisArc_arc_part) << endl;
  GrowArc::save( fp, mode);
  fp << int(glow_eSave_GrowAxisArc_lines) << FSPACE << lines << endl;
  fp << int(glow_eSave_GrowAxisArc_linelength) << FSPACE << linelength << endl;
  fp << int(glow_eSave_GrowAxisArc_longquotient) << FSPACE << longquotient << endl;
  fp << int(glow_eSave_GrowAxisArc_valuequotient) << FSPACE << valuequotient << endl;
  fp << int(glow_eSave_GrowAxisArc_format) << FSPACE << format << endl;
  fp << int(glow_eSave_GrowAxisArc_text_size) << FSPACE << text_size << endl;
  fp << int(glow_eSave_GrowAxisArc_text_drawtype) << FSPACE << int(text_drawtype) << endl;
  fp << int(glow_eSave_GrowAxisArc_text_color_drawtype) << FSPACE << int(text_color_drawtype) << endl;
  fp << int(glow_eSave_End) << endl;
}

void GrowAxisArc::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int           tmp;

  for (;;)
  {
    if ( !fp.good()) {
      fp.clear();
      fp.getline( dummy, sizeof(dummy));
      printf( "** Read error GrowAxisArc: \"%d %s\"\n", type, dummy);      
    }

    fp >> type;
    switch( type) {
      case glow_eSave_GrowAxisArc: break;
      case glow_eSave_GrowAxisArc_max_value: fp >> max_value; break;
      case glow_eSave_GrowAxisArc_min_value: fp >> min_value; break;
      case glow_eSave_GrowAxisArc_arc_part: 
        GrowArc::open( fp);
        break;
      case glow_eSave_GrowAxisArc_lines: fp >> lines; break;
      case glow_eSave_GrowAxisArc_linelength: fp >> linelength; break;
      case glow_eSave_GrowAxisArc_longquotient: fp >> longquotient; break;
      case glow_eSave_GrowAxisArc_valuequotient: fp >> valuequotient; break;
      case glow_eSave_GrowAxisArc_format:
        fp.get();
        fp.getline( format, sizeof(format));
        break;
      case glow_eSave_GrowAxisArc_text_size: fp >> text_size; break;
      case glow_eSave_GrowAxisArc_text_drawtype: fp >> tmp; text_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowAxisArc_text_color_drawtype: fp >> tmp; text_color_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowAxisArc:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  configure();
}

void GrowAxisArc::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowAxisArc::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowAxisArc::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowAxisArc::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, 
		     void *colornode)
{
  if ( ctx->nodraw)
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int i;
  int draw_text = (fabs(increment) > DBL_EPSILON);
  int idx;
  char text[20];
  int z_height, z_width, z_descent;
  double rotation;
  glow_eDrawType drawtype;
  int text_idx = int( w->zoom_factor_y / w->base_zoom_factor * (text_size +4) - 4);
  double tsize = w->zoom_factor_y / w->base_zoom_factor * (8+2*text_size);
  text_idx = min( text_idx, DRAW_TYPE_SIZE-1);

  if ( node && ((GrowNode *)node)->line_width)
    idx = int( w->zoom_factor_y / w->base_zoom_factor * 
	       ((GrowNode *)node)->line_width - 1);
  else
    idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, xt, yt;

  if (!t) {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
  }
  else {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);
  drawtype = ctx->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
				highlight, (GrowNode *)colornode, 0);

  if ( 1) {
    // Lines inwards
    ctx->gdraw->arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 
		     angle1 - rotation, angle2, drawtype, idx, 0);

    if ( lines == 1)
      return;

    if ( increment > 0)
      format_text( text, format, min_value + (lines - 2) * increment);
    else 
      format_text( text, format, min_value + increment);
    ctx->gdraw->get_text_extent( text, strlen(text), text_drawtype,
				 max( 0, text_idx), glow_eFont_Helvetica, 
				 &z_width, &z_height, &z_descent, tsize);

    // z_descent=0;
    double line_angle = (double)angle2 / (lines - 1);
    for ( i = 0; i < lines; i++) {
      double sin1 = sin( ((double)angle1 + i * line_angle) / 180 * M_PI);
      double cos1 = cos( ((double)angle1 + i * line_angle) / 180 * M_PI);
      y1 = int( ((double)ur_y - ll_y) / 2 * ( -sin1 + 1) + ll_y);
      x1 = int( ((double)ur_x - ll_x) / 2 * ( cos1 + 1) + ll_x);
      if ( i % longquotient == 0) {
	y2 = int( ((double)ur_y - ll_y) / 2 * ( -sin1 * (1.0 - linelength) + 1) + ll_y);
	x2 = int( ((double)ur_x - ll_x) / 2 * ( cos1 * (1.0 - linelength) + 1) + ll_x);
      }
      else {
	y2 = int( ((double)ur_y - ll_y) / 2 * ( -sin1 * (1.0 - linelength/2) + 1) + ll_y);
	x2 = int( ((double)ur_x - ll_x) / 2 * ( cos1 * (1.0 - linelength/2) + 1) + ll_x);
      }
      yt = int( ((double)ur_y - ll_y) / 2 * ( -sin1 * (1.0 - linelength) + 1) + ll_y +
		sin1 * (z_height - z_descent)/2);
      xt = int( ((double)ur_x - ll_x) / 2 * ( cos1 * (1.0 - linelength) + 1) + ll_x -
		cos1 * z_width/2);

      ctx->gdraw->line( w, x1, y1, 
			x2, y2, drawtype, idx, 0);
      if ( draw_text) {
	if ( text_idx >= 0 && i % valuequotient == 0 &&
	     !(angle2 == 360 && 
	       ((increment > 0 && i == lines - 1) || (increment < 0 && i == 0)))) {
	  format_text( text, format, min_value + i * increment);
	  ctx->gdraw->get_text_extent( text, strlen(text), text_drawtype,
				       max( 0, text_idx), glow_eFont_Helvetica, 
				       &z_width, &z_height, &z_descent, tsize);
	  
	  if ( i == lines - 1 && angle1 == 0 && angle2 == 180) {
	    //yt = yt - (z_height-z_descent)/2;
	    xt = xt - z_width/2;
	  }
	  else if ( i == 0 && angle1 == 0 && angle2 != 360) {
	    //yt = yt - (z_height-z_descent)/2;
	    xt = xt - z_width/2;
	  }
	  else {
	    yt = yt + (z_height-z_descent)/2;
	    xt = xt - z_width/2;
	  }
	  ctx->gdraw->text( w, xt, yt,
			    text, strlen(text), text_drawtype, text_color_drawtype, 
			    text_idx, highlight, 0, glow_eFont_Helvetica, tsize);
	}
      }
    }
  }
}

void GrowAxisArc::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  if ( ctx->nodraw)
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double rotation;
  int idx;
  if ( hot && ctx->environment != glow_eEnv_Development &&
       ctx->hot_indication != glow_eHotIndication_LineWidth)
    hot = 0;

  if ( node && ((GrowNode *)node)->line_width)
    idx = int( w->zoom_factor_y / w->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t) {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
    rotation = int( trf.rot());
  }
  else {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
    rotation = int( trf.rot( t));
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  w->set_draw_buffer_only();
  ctx->gdraw->arc_erase( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			 angle1 - rotation, angle2, idx);
  ctx->gdraw->fill_rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  w->reset_draw_buffer_only();
}

void GrowAxisArc::draw()
{
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowAxisArc::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->set_defered_redraw();
    draw();
    switch ( direction) {
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

void GrowAxisArc::set_textsize( int size) 
{ 
  erase( &ctx->mw);
  erase( &ctx->navw);
  text_size = size;
  get_node_borders();
  draw();
}

void GrowAxisArc::set_textbold( int bold) 
{ 
  if ( ( bold && text_drawtype == glow_eDrawType_TextHelveticaBold) ||
       ( !bold && text_drawtype == glow_eDrawType_TextHelvetica))
    return;

  erase( &ctx->mw);
  erase( &ctx->navw);
  if ( bold)
    text_drawtype = glow_eDrawType_TextHelveticaBold;
  else
    text_drawtype = glow_eDrawType_TextHelvetica;
  get_node_borders();
  draw();
}

void GrowAxisArc::set_range( double min, double max)
{
  max_value = max;
  min_value = min;
  configure();
}

void GrowAxisArc::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  double rotation;
  int bold;
  int idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * (text_size +4) - 4);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  bold = (text_drawtype == glow_eDrawType_TextHelveticaBold);

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( t)
    rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
  else
    rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;

  ((GrowCtx *)ctx)->export_jbean->axisarc( ll_x, ll_y, ur_x, ur_y, angle1, angle2,
     draw_type, text_color_drawtype, min_value, max_value, lines, longquotient, valuequotient,
     linelength, line_width, rotation, bold, idx, format,
     pass, shape_cnt, node_cnt, fp);
}

void GrowAxisArc::set_conf( double max_val, double min_val, int no_of_lines, 
     int long_quot, int value_quot, double rot, const char *value_format)
{
  erase( &ctx->mw);
  max_value = max_val;
  min_value = min_val;
  lines = no_of_lines;
  longquotient = long_quot;
  valuequotient = value_quot;
  trf.rotation = rot;
  if ( format)
    strcpy( format, value_format);

  configure();
  draw();
}

void GrowAxisArc::set_axis_info( glow_sAxisInfo *info)
{
  max_value = info->max_value;
  min_value = info->min_value;
  lines = info->lines;
  longquotient = info->longquotient;
  valuequotient = info->valuequotient;
  strcpy( format, info->format);
}

void GrowAxisArc::format_text( char *text, char *fmt, double value) 
{
  if ( strcmp( fmt, "%1t") == 0) {
    // Hours, minutes and seconds, value in seconds
    int val = (int) nearbyint(value);
    int hours = val / 3600;
    int minutes = (val - hours * 3600) / 60; 
    int seconds = (val - hours * 3600 - minutes * 60); 
    sprintf( text, "%d:%02d:%02d", hours, minutes, seconds);
  }
  else if ( strcmp( fmt, "%2t") == 0) {
    // Hours and minutes, value in seconds
    int val = (int) nearbyint(value);
    int hours = val / 3600;
    int minutes = (val - hours * 3600) / 60; 
    sprintf( text, "%d:%02d", hours, minutes);
  }
  else if ( strcmp( fmt, "%3t") == 0) {
    // Days, hours and minues, value in seconds
    int val = (int) nearbyint(value);
    int days = val / (24 * 3600);
    int hours = (val - days * 24 * 3600) / 3600; 
    int minutes = (val - days * 24 * 3600 - hours * 3600) / 60; 
    sprintf( text, "%d %02d:%02d", days, hours, minutes);
  }
  else if ( strcmp( fmt, "%10t") == 0) {
    // Date
    char timstr[40];
    pwr_tTime t;
    t.tv_sec = (int) nearbyint(value);
    t.tv_nsec = 0;
    
    time_AtoAscii( &t, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
    timstr[19] = 0;
    strcpy( text, timstr);
  }
  else if ( strcmp( fmt, "%11t") == 0) {
    // Date, no seconds
    char timstr[40];
    pwr_tTime t;
    t.tv_sec = (int) nearbyint(value);
    t.tv_nsec = 0;
    
    time_AtoAscii( &t, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
    timstr[16] = 0;
    strcpy( text, timstr);
  }
  else
    sprintf( text, fmt, value);
}

void GrowAxisArc::convert( glow_eConvert version) 
{
}












