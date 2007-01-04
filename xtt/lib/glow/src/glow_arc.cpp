/* 
 * Proview   $Id: glow_arc.cpp,v 1.4 2007-01-04 07:57:38 claes Exp $
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


#include <stdlib.h>
#include <iostream.h>
#include "glow_arc.h"
#include "glow_draw.h"
#include "glow_growctx.h"

void GlowArc::zoom()
{
  ll.zoom();
  ur.zoom();
}

void GlowArc::nav_zoom()
{
  ll.nav_zoom();
  ur.nav_zoom();
}

void GlowArc::traverse( int x, int y)
{
  ll.traverse( x, y);
  ur.traverse( x, y);
}

void GlowArc::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Arc) << endl;
  fp << int(glow_eSave_Arc_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Arc_line_width) << FSPACE << line_width << endl;
  fp << int(glow_eSave_Arc_angel1) << FSPACE << angel1 << endl;
  fp << int(glow_eSave_Arc_angel2) << FSPACE << angel2 << endl;
  fp << int(glow_eSave_Arc_fill) << FSPACE << fill << endl;
  fp << int(glow_eSave_Arc_ll) << endl;
  ll.save( fp, mode);
  fp << int(glow_eSave_Arc_ur) << endl;
  ur.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowArc::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Arc: break;
      case glow_eSave_Arc_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Arc_line_width: fp >> line_width; break;
      case glow_eSave_Arc_angel1: fp >> angel1; break;
      case glow_eSave_Arc_angel2: fp >> angel2; break;
      case glow_eSave_Arc_fill: fp >> fill; break;
      case glow_eSave_Arc_ll: ll.open( fp); break;
      case glow_eSave_Arc_ur: ur.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowArc:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowArc::draw( GlowWind *w, void *pos, int highlight, int hot, void *node)
{
  int ll_x, ll_y, ur_x, ur_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    ll_x = ll.nav_z_x;
    ll_y = ll.nav_z_y;
    ur_x = ur.nav_z_x;
    ur_y = ur.nav_z_y;
  }
  else {
    ll_x = ll.z_x;
    ll_y = ll.z_y;
    ur_x = ur.z_x;
    ur_y = ur.z_y;
  }
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  w->set_draw_buffer_only();
  if ( !fill) {
    ctx->gdraw->arc( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x,
	ll_y + ((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, angel1, angel2,
	draw_type, idx, highlight);
  }
  else
    ctx->gdraw->fill_arc( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	ll_y + ((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, angel1, angel2,
	draw_type, highlight);
  w->reset_draw_buffer_only();
}

void GlowArc::draw_shadow( GlowWind *w, int border, int shadow, int highlight, int hot)
{
  int ll_x, ll_y, ur_x, ur_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    ll_x = ll.nav_z_x;
    ll_y = ll.nav_z_y;
    ur_x = ur.nav_z_x;
    ur_y = ur.nav_z_y;
  }
  else {
    ll_x = ll.z_x;
    ll_y = ll.z_y;
    ur_x = ur.z_x;
    ur_y = ur.z_y;
  }
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int ish = 1;
  int offs = border;

  w->set_draw_buffer_only();
  if ( shadow && idx > 2) {
    if ( angel1 == 0) {
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx + offs,
		     ll_y - w->offset_y + idx/2 - idx + offs, 
		     ur_x - ll_x + idx - offs*2, ur_y - ll_y + idx - offs*2, angel1 + 45, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx + offs, 
		     ll_y - w->offset_y + idx/2 - idx + offs, 
		     ur_x - ll_x + idx - offs*2, ur_y - ll_y + idx - offs*2, angel1, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - offs, 
		     ll_y - w->offset_y + idx/2 - offs, 
		     max( 0, ur_x - ll_x - idx + offs*2), max( 0,  ur_y - ll_y - idx + offs*2), angel1 + 45, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - offs, 
		     ll_y - w->offset_y + idx/2 - offs, 
		     max( 0, ur_x - ll_x - idx + offs*2), max( 0,  ur_y - ll_y - idx + offs*2), angel1, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, highlight);
    }
    else if ( angel1 == 90) {
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx + offs, 
		     ll_y - w->offset_y + idx/2 - idx + offs, 
		     ur_x - ll_x + idx - offs*2, ur_y - ll_y + idx - offs*2, angel1, angel2,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - offs, 
		     ll_y - w->offset_y + idx/2 - offs, 
		     max( 0, ur_x - ll_x - idx + offs*2), max( 0,  ur_y - ll_y - idx + offs*2), angel1, angel2,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, highlight);
    }
    else if ( angel1 == 180) {
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx + offs, 
		     ll_y - w->offset_y + idx/2 - idx + offs, 
		     ur_x - ll_x + idx - offs*2, ur_y - ll_y + idx - offs*2, angel1 + 45, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx + offs, 
		     ll_y - w->offset_y + idx/2 - idx + offs, 
		     ur_x - ll_x + idx - offs*2, ur_y - ll_y + idx - offs*2, angel1, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - offs, 
		     ll_y - w->offset_y + idx/2 - offs, 
		     max( 0, ur_x - ll_x - idx + offs*2), max( 0,  ur_y - ll_y - idx + offs*2), angel1 + 45, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - offs, 
		     ll_y - w->offset_y + idx/2 - offs, 
		     max( 0, ur_x - ll_x - idx + offs*2), max( 0,  ur_y - ll_y - idx + offs*2), angel1, angel2 - 45,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, highlight);
    }
    else if ( angel1 == 270) {
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx + offs, 
		     ll_y - w->offset_y + idx/2 - idx + offs, 
		     ur_x - ll_x + idx - offs*2, ur_y - ll_y + idx - offs*2, angel1, angel2,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, highlight);
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - offs, 
		     ll_y - w->offset_y + idx/2 - offs, 
		     max( 0, ur_x - ll_x - idx + offs*2), max( 0,  ur_y - ll_y - idx + offs*2), angel1, angel2,
		     ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, highlight);
    }
  }
  if ( border) {
    ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2 - idx, 
		   ll_y - w->offset_y + idx/2 - idx, 
		   ur_x - ll_x + idx, ur_y - ll_y + idx, angel1, angel2,
		   glow_eDrawType_Line, 0, highlight);
    if ( idx > 0)
      ctx->gdraw->arc( w, ll_x - w->offset_x + idx/2, 
		   ll_y - w->offset_y + idx/2, 
		   max( 0, ur_x - ll_x - idx), max( 0,  ur_y - ll_y - idx), angel1, angel2,
		   glow_eDrawType_Line, 0, highlight);
  }
  w->reset_draw_buffer_only();
}

void GlowArc::erase( GlowWind *w, void *pos, int hot, void *node)
{
  int ll_x, ll_y, ur_x, ur_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    ll_x = ll.nav_z_x;
    ll_y = ll.nav_z_y;
    ur_x = ur.nav_z_x;
    ur_y = ur.nav_z_y;
  }
  else {
    ll_x = ll.z_x;
    ll_y = ll.z_y;
    ur_x = ur.z_x;
    ur_y = ur.z_y;
  }
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( !fill) {
    ctx->gdraw->arc_erase( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	ll_y + ((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, angel1, angel2, idx);
    // Erase border 
    ctx->gdraw->arc_erase( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x + idx/2 - idx, 
		   ll_y + ((GlowPoint *)pos)->z_x - w->offset_y + idx/2 - idx, 
		   ur_x - ll_x + idx, ur_y - ll_y + idx, angel1, angel2, 0);
    ctx->gdraw->arc_erase( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x + idx/2, 
		   ll_y + ((GlowPoint *)pos)->z_x - w->offset_y + idx/2, 
		   max( 0, ur_x - ll_x - idx), max( 0,  ur_y - ll_y - idx), angel1, angel2, 0);
  }
  else
    ctx->gdraw->fill_arc( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	ll_y + ((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, angel1, angel2, glow_eDrawType_LineErase,
	0);
}

int GlowArc::event_handler( GlowWind *w, void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
  if ( angel2 == 360 &&
       ll.z_x + ((GlowPoint *)pos)->z_x - w->offset_x <= x && 
       x <= ur.z_x  + ((GlowPoint *)pos)->z_x - w->offset_x &&
       ll.z_y  + ((GlowPoint *)pos)->z_y - w->offset_y <= y && 
       y <= ur.z_y + ((GlowPoint *)pos)->z_y - w->offset_y)
  {
    return 1;
  }  
  else
    return 0;
}

void GlowArc::get_borders( double pos_x, double pos_y, double *x_right, 
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

void GlowArc::move( void *pos, double x1, double y1, double x2, double y2,
	int ang1, int ang2, int highlight, int hot)
{
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  ll.x = x1;
  ll.y = y1;
  ur.x = x2;
  ur.y = y2;
  angel1 = ang1;
  angel2 = ang2;  
  zoom();
  nav_zoom();
  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowArc::move_noerase( void *pos, double x1, double y1, double x2, double y2,
	int ang1, int ang2, int highlight, int hot)
{
  ll.x = x1;
  ll.y = y1;
  ur.x = x2;
  ur.y = y2;
  angel1 = ang1;
  angel2 = ang2;  
  zoom();
  nav_zoom();
  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowArc::shift( void *pos, double delta_x, double delta_y, 
	int highlight, int hot)
{
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  ll.x += delta_x;
  ll.y += delta_y;
  ur.x += delta_x;
  ur.y += delta_y;
  zoom();
  nav_zoom();

  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowArc::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ((GrowCtx *)ctx)->export_jbean->arc( 
	ll.z_x - ctx->mw.offset_x, 
	ll.z_y - ctx->mw.offset_y, 
	ur.z_x - ll.z_x, ur.z_y - ll.z_y, angel1, angel2,
	fill, !fill, draw_type, draw_type, idx, 0.0, 0, glow_eDrawType_No,
	pass, shape_cnt, node_cnt, fp);
}

void GlowArc::export_javabean_shadow( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp, int border, int shadow)
{
  int idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  int ish = 1;
  int offs = border;

  if ( shadow) {
    if ( angel1 == 0) {
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx + offs,
		     ll.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		     ur.z_x - ll.z_x + idx - offs*2, ur.z_y - ll.z_y + idx - offs*2, angel1 + 45, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0),
		     ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		     ur.z_x - ll.z_x + idx - offs*2, ur.z_y - ll.z_y + idx - offs*2, angel1, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - offs, 
		     max( 0, ur.z_x - ll.z_x - idx + offs*2), max( 0,  ur.z_y - ll.z_y - idx + offs*2), angel1 + 45, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - offs, 
		     max( 0, ur.z_x - ll.z_x - idx + offs*2), max( 0,  ur.z_y - ll.z_y - idx + offs*2), angel1, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
    }
    else if ( angel1 == 90) {
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		     ur.z_x - ll.z_x + idx - offs*2, ur.z_y - ll.z_y + idx - offs*2, angel1, angel2,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - offs, 
		     max( 0, ur.z_x - ll.z_x - idx + offs*2), max( 0,  ur.z_y - ll.z_y - idx + offs*2), angel1, angel2,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
    }
    else if ( angel1 == 180) {
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		     ur.z_x - ll.z_x + idx - offs*2, ur.z_y - ll.z_y + idx - offs*2, angel1 + 45, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		     ur.z_x - ll.z_x + idx - offs*2, ur.z_y - ll.z_y + idx - offs*2, angel1, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - offs, 
		     max( 0, ur.z_x - ll.z_x - idx + offs*2), max( 0,  ur.z_y - ll.z_y - idx + offs*2), angel1 + 45, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - offs, 
		     max( 0, ur.z_x - ll.z_x - idx + offs*2), max( 0,  ur.z_y - ll.z_y - idx + offs*2), angel1, angel2 - 45,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
    }
    else if ( angel1 == 270) {
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		     ur.z_x - ll.z_x + idx - offs*2, ur.z_y - ll.z_y + idx - offs*2, angel1, angel2,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
      ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - offs, 
		     ll.z_y - ctx->mw.offset_y + idx/2 - offs, 
		     max( 0, ur.z_x - ll.z_x - idx + offs*2), max( 0,  ur.z_y - ll.z_y - idx + offs*2), angel1, angel2,
		     0, 1, glow_eDrawType_No, ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0), ish-1, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
    }
  }
  if ( border) {
    ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2 - idx, 
		   ll.z_y - ctx->mw.offset_y + idx/2 - idx, 
		   ur.z_x - ll.z_x + idx, ur.z_y - ll.z_y + idx, angel1, angel2,
		   0, 1, glow_eDrawType_No, glow_eDrawType_Line, 0, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->arc( ll.z_x - ctx->mw.offset_x + idx/2, 
		   ll.z_y - ctx->mw.offset_y + idx/2, 
		   max( 0, ur.z_x - ll.z_x - idx), max( 0,  ur.z_y - ll.z_y - idx), angel1, angel2,
		   0, 1, glow_eDrawType_No, glow_eDrawType_Line, 0, 0.0, 0, glow_eDrawType_No, pass, shape_cnt, node_cnt, fp);
  }
}

ostream& operator<< ( ostream& o, const GlowArc a)
{
  o << 
  '(' << a.ll.x << ',' << a.ll.y << ')' << 
  '(' << a.ur.x << ',' << a.ur.y << ')' << 
  '[' << a.ll.z_x << ',' << a.ll.z_y << ']' <<
  '[' << a.ur.z_x << ',' << a.ur.z_y << ']' ;
  return o;
}

void GlowArc::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);
    break;
  }
  }  
}



