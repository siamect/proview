/* 
 * Proview   $Id: glow_growconglue.cpp,v 1.5 2008-10-31 12:51:35 claes Exp $
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
#include "glow_growconglue.h"
#include "glow_grownode.h"
#include "glow_growrect.h"
#include "glow_conpoint.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowConGlue::GrowConGlue( GrowCtx *glow_ctx, const char *name, double x1, double y1, int nodraw) : 
		GrowNode(glow_ctx,name,0,x1,y1,1,0),
		line_width_up(-1), line_width_down(-1), line_width_left(-1), line_width_right(-1)
{
  double x_grid, y_grid;

  // Get nodeclass
  nc = (GlowNodeClass *) ctx->get_nodeclass_from_name( "pwr_conglue");
  if ( !nc) {
    // Create the nodeclass
    GlowConPoint *cp;
    GrowRect *r;

    int grid_on = ctx->grid_on;
    ctx->grid_on = 0;

    nc = new GlowNodeClass( ctx, "pwr_conglue");
    cp = new GlowConPoint( ctx, 0, -0.5, 0, glow_eDirection_Down);
    nc->insert( cp);
    cp = new GlowConPoint( ctx, 0.5, 0, 1, glow_eDirection_Right);
    nc->insert( cp);
    cp = new GlowConPoint( ctx, 0, 0.5, 2, glow_eDirection_Up);
    nc->insert( cp);
    cp = new GlowConPoint( ctx, -0.5, 0, 3, glow_eDirection_Left);
    nc->insert( cp);
    r = new GrowRect( ctx, "", -0.5, -0.5, 1, 1, glow_eDrawType_Line, 1, 0, 
		      glow_mDisplayLevel_1, 0, 1, 0, glow_eDrawType_Line, 1);
    nc->insert( r);
    ctx->grid_on = grid_on;

    ctx->nodeclass_insert( nc);
  }
  draw_type = glow_eDrawType_Line;

  if ( ctx->grid_on) {
    ctx->find_grid( x1, y1, &x_grid, &y_grid);
    trf.move( x_grid - x1,  y_grid - y1);
  }
  get_node_borders();
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);

}

GrowConGlue::~GrowConGlue()
{
  erase( &ctx->mw, (GlowTransform *)NULL, hot, NULL);
  erase( &ctx->navw, (GlowTransform *)NULL, hot, NULL);
}

void GrowConGlue::configure( GlowCon *con) 
{
  int i;
  GlowCon *c;
  int max_width = -1;
  int lw_up, lw_down, lw_left, lw_right;

  lw_up = -1;
  lw_down = -1;
  lw_left = -1;
  lw_right = -1;
  draw_type = glow_eDrawType_Line;

  // if con is created now, it is not inserted in the ctx list yet
  if ( con) {
    if ( con->source() == this) {
      c = con;
      switch( conpoint_to_direction( c->source_conpoint)) {
      case glow_eDirection_Up:
	lw_up = c->cc->line_width;
	break;
      case glow_eDirection_Down:
	lw_down = c->cc->line_width;
	break;
      case glow_eDirection_Left:
	lw_left = c->cc->line_width;
	break;
      case glow_eDirection_Right:
	lw_right = c->cc->line_width;
	break;
      default: ;
      }
      max_width = c->cc->line_width;
      draw_type = c->cc->draw_type;
      shadow = c->shadow;
      border = c->border;
    }
    if ( con->destination() == this) {
      c = con;
      switch( conpoint_to_direction( c->dest_conpoint)) {
      case glow_eDirection_Up:
	if ( lw_up < c->cc->line_width)
	  lw_up = c->cc->line_width;
	break;
      case glow_eDirection_Down:
	if ( lw_down < c->cc->line_width)
	  lw_down = c->cc->line_width;
	break;
      case glow_eDirection_Left:
	if ( lw_left < c->cc->line_width)
	  lw_left = c->cc->line_width;
	break;
      case glow_eDirection_Right:
	if ( lw_right < c->cc->line_width)
	  lw_right = c->cc->line_width;
	break;
      default: ;
      }
      // Select properties from the widest connection
      if ( c->cc->line_width > max_width) {
	max_width = c->cc->line_width;
	draw_type = c->cc->draw_type;
	shadow = c->shadow;
	border = c->border;
      }
    }
  }

  for ( i = 0; i < ctx->a.a_size; i++) {
    if ( ctx->a.a[i]->type() == glow_eObjectType_Con) {
      if ( ((GlowCon *) ctx->a.a[i])->source() == this) {
	c = (GlowCon *) ctx->a.a[i];
	switch( conpoint_to_direction( c->source_conpoint)) {
	case glow_eDirection_Up:
	  if ( lw_up < c->cc->line_width)
	    lw_up = c->cc->line_width;
	  break;
	case glow_eDirection_Down:
	  if ( lw_down < c->cc->line_width)
	    lw_down = c->cc->line_width;
	  break;
	case glow_eDirection_Left:
	  if ( lw_left < c->cc->line_width)
	    lw_left = c->cc->line_width;
	  break;
	case glow_eDirection_Right:
	  if ( lw_right < c->cc->line_width)
	    lw_right = c->cc->line_width;
	  break;
	default: ;
	}
	// Select properties from the widest connection
	if ( c->cc->line_width > max_width) {
	  max_width = c->cc->line_width;
	  draw_type = c->cc->draw_type;
	  shadow = c->shadow;
	  border = c->border;
	}
      }
      if ( ((GlowCon *) ctx->a.a[i])->destination() == this) {
	c = (GlowCon *) ctx->a.a[i];
	switch( conpoint_to_direction( c->dest_conpoint)) {
	case glow_eDirection_Up:
	  if ( lw_up < c->cc->line_width)
	    lw_up = c->cc->line_width;
	  break;
	case glow_eDirection_Down:
	  if ( lw_down < c->cc->line_width)
	    lw_down = c->cc->line_width;
	  break;
	case glow_eDirection_Left:
	  if ( lw_left < c->cc->line_width)
	    lw_left = c->cc->line_width;
	  break;
	case glow_eDirection_Right:
	  if ( lw_right < c->cc->line_width)
	    lw_right = c->cc->line_width;
	  break;
	default: ;
	}
	// Select properties from the widest connection
	if ( c->cc->line_width > max_width) {
	  max_width = c->cc->line_width;
	  draw_type = c->cc->draw_type;
	  shadow = c->shadow;
	  border = c->border;
	}
      }
    }
  }
  line_width_down = lw_down;
  line_width_right = lw_right;
  line_width_up = lw_up;
  line_width_left = lw_left;

}

void GrowConGlue::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowConGlue) << endl;
  fp << int(glow_eSave_GrowConGlue_line_width_up) << FSPACE << line_width_up << endl;
  fp << int(glow_eSave_GrowConGlue_line_width_down) << FSPACE << line_width_down << endl;
  fp << int(glow_eSave_GrowConGlue_line_width_left) << FSPACE << line_width_left << endl;
  fp << int(glow_eSave_GrowConGlue_line_width_right) << FSPACE << line_width_right << endl;
  fp << int(glow_eSave_GrowConGlue_border) << FSPACE << border << endl;
  fp << int(glow_eSave_GrowConGlue_node_part) << endl;
  GrowNode::save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowConGlue::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;) {
    if ( !fp.good()) {
      fp.clear();
      fp.getline( dummy, sizeof(dummy));
      printf( "** Read error GrowConGlue: \"%d %s\"\n", type, dummy);      
    }

    fp >> type;
    switch( type) {
      case glow_eSave_GrowConGlue: break;
      case glow_eSave_GrowConGlue_line_width_up: fp >> line_width_up; break;
      case glow_eSave_GrowConGlue_line_width_down: fp >> line_width_down; break;
      case glow_eSave_GrowConGlue_line_width_left: fp >> line_width_left; break;
      case glow_eSave_GrowConGlue_line_width_right: fp >> line_width_right; break;
      case glow_eSave_GrowConGlue_border: fp >> border; break;
      case glow_eSave_GrowConGlue_node_part: 
        GrowNode::open( fp);
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowConGlue:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowConGlue::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowConGlue::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowConGlue::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowConGlue::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, 
			void *colornode)
{
  if ( ctx->nodraw)
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int idx, idx_up, idx_down, idx_left, idx_right;
  glow_eDrawType drawtype;
  glow_eDrawType shift_drawtype;


  idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  double x1, y1, x2, y2; 
  int ll_x, ll_y, ur_x, ur_y, m_x, m_y;

  if (!t)
  {
    x1 = x_left * w->zoom_factor_x - w->offset_x;
    y1 = y_low * w->zoom_factor_y - w->offset_y;
    x2 = x_right * w->zoom_factor_x - w->offset_x;
    y2 = y_high * w->zoom_factor_y - w->offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * w->zoom_factor_x - w->offset_x;
    y1 = t->y( x_left, y_low) * w->zoom_factor_y - w->offset_y;
    x2 = t->x( x_right, y_high) * w->zoom_factor_x - w->offset_x;
    y2 = t->y( x_right, y_high) * w->zoom_factor_y - w->offset_y;
  }

  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);
  m_x = int((x1 + x2) / 2 + 0.5);
  m_y = int((y1 + y2) / 2 + 0.5);

  drawtype = ctx->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);

  int lw_up, lw_down, lw_left, lw_right;

  lw_down = line_width_down;
  lw_right = line_width_right;
  lw_up = line_width_up;
  lw_left = line_width_left;

  idx_up = int( w->zoom_factor_y / w->base_zoom_factor * lw_up - 1);
  idx_up += hot;
  idx_down = int( w->zoom_factor_y / w->base_zoom_factor * lw_down - 1);
  idx_down += hot;
  idx_left = int( w->zoom_factor_x / w->base_zoom_factor * lw_left - 1);
  idx_left += hot;
  idx_right = int( w->zoom_factor_x / w->base_zoom_factor * lw_right - 1);
  idx_right += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  idx_up = max( 0, idx_up);
  idx_up = min( idx_up, DRAW_TYPE_SIZE-1);
  idx_down = max( 0, idx_down);
  idx_down = min( idx_down, DRAW_TYPE_SIZE-1);
  idx_left = max( 0, idx_left);
  idx_left = min( idx_left, DRAW_TYPE_SIZE-1);
  idx_right = max( 0, idx_right);
  idx_right = min( idx_right, DRAW_TYPE_SIZE-1);

  w->set_draw_buffer_only();
  if ( lw_up != -1 && lw_down == -1 &&
       lw_right == -1 && lw_left == -1) {
    // Up termination
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    if ( shadow && idx_up > 2) {
      if ( border) {
	idx_up -= 2;
	m_y++;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, shift_drawtype, 0, 0);

      if ( border) {
	idx_up += 2;
	m_y--;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up == -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Down termination
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
    if ( shadow && idx_down > 2) {
      if ( border) {
	idx_down -= 2;
	m_y--;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, 0);

      if ( border) {
	idx_down += 2;
	m_y++;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right != -1 && lw_left == -1) {
    // Right termination
    ctx->gdraw->fill_rect( w, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    if ( shadow && idx_right > 2) {
      if ( border) {
	idx_right -= 2;
	m_x++;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, shift_drawtype, 0, 0);

      if ( border) {
	idx_right += 2;
	m_x--;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right == -1 && lw_left != -1) {
    // Left termination
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    if ( shadow && idx_left > 2) {
      if ( border) {
	idx_left -= 2;
	m_x--;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);

      if ( border) {
	idx_left += 2;
	m_x++;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up != -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Vertical glue
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);

    if ( shadow && idx_down > 2) {
      if ( border) {
	idx_up -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_up += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      if ( idx_down != idx_up) {
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down == -1) {
    // Horizontal glue
    ctx->gdraw->fill_rect( w, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_right += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      if ( idx_left != idx_right) {
	ctx->gdraw->line( w, m_x, m_y - idx_right + idx_right/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x, m_y + idx_right/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left up corner
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, idx_up + 1, ur_y -  (m_y + idx_left/2), drawtype);
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x + 1, idx_left + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_up > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Right up corner
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y -  (m_y - idx_right + idx_right/2), drawtype);
    ctx->gdraw->fill_rect( w, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x + idx_up/2), idx_right + 1, drawtype);
    if ( shadow && (idx_right > 2 || idx_up > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Right down corner
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - idx_right + idx_right/2 - ll_y, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, drawtype);
    if ( shadow && (idx_right > 2 || idx_down > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left down corner
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y + 1, drawtype);
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2 - ll_x, idx_left + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_down > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left right down threeway
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y + 1, drawtype);
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_down/2 - ll_x, idx_left + 1, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2 || idx_down > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      if ( idx_right == idx_left)
	ctx->gdraw->line( w, ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      else if ( idx_right > idx_left) {
	ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      }
      else {
	ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      }
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      if ( idx_right == idx_left)
	ctx->gdraw->line( w, ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      else if ( idx_right > idx_left) {
	ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_down/2, m_y + idx_right/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left right up threeway
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x, idx_left + 1, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_up + idx_up/2), idx_right + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2 || idx_up > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( idx_right == idx_left)
	ctx->gdraw->line( w, ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      else if ( idx_right > idx_left) {
	ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      }
      else {
	ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      if ( idx_right == idx_left)
	ctx->gdraw->line( w, ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      else if ( idx_right > idx_left) {
	ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Left up down threeway
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, idx_up + 1, ur_y - (m_y - idx_left + idx_left/2), drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y, drawtype);
    if ( shadow && (idx_left > 2 || idx_down > 2 || idx_up > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);

      if ( idx_up == idx_down)
	ctx->gdraw->line( w, m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, shift_drawtype, 0, 0);
      else if ( idx_down > idx_up) {
	ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      }
      else {
	ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      }
      if ( border) {
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);

      if ( idx_up == idx_down)
	ctx->gdraw->line( w, m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, glow_eDrawType_Line, 0, 0);
      else if ( idx_down > idx_up) {
	ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right up down threeway
    ctx->gdraw->fill_rect( w, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y - (m_y - idx_right + idx_right/2), drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_right/2 - ll_y, drawtype);
    if ( shadow && (idx_right > 2 || idx_down > 2 || idx_up > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);

      if ( idx_up == idx_down)
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, shift_drawtype, 0, 0);
      else if ( idx_down > idx_up) {
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
	// ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
      }
      else {
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);

      if ( idx_up == idx_down)
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, glow_eDrawType_Line, 0, 0);
      else if ( idx_down > idx_up) {
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right left up down fourway
    ctx->gdraw->fill_rect( w, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    ctx->gdraw->fill_rect( w, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    ctx->gdraw->fill_rect( w, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2 || idx_down > 2 || idx_up > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->gdraw->line( w, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      ctx->gdraw->line( w, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else {
    ctx->gdraw->line( w, m_x, ll_y, m_x, m_y, drawtype, idx, 0);
    ctx->gdraw->line( w, m_x, m_y, m_x, ur_y, drawtype, idx, 0);
    ctx->gdraw->line( w, ll_x, m_y, m_x, m_y, drawtype, idx, 0);
    ctx->gdraw->line( w, m_x, m_y, ur_x, m_y, drawtype, idx, 0);
  }
  w->reset_draw_buffer_only();
}


void GrowConGlue::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  double x1, y1, x2, y2;
  int ll_x, ll_y, ur_x, ur_y;

  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }

  if (!t) {
    x1 = x_left * w->zoom_factor_x - w->offset_x;
    y1 = y_low * w->zoom_factor_y - w->offset_y;
    x2 = x_right * w->zoom_factor_x - w->offset_x;
    y2 = y_high * w->zoom_factor_y - w->offset_y;
  }
  else {
    x1 = t->x( x_left, y_low) * w->zoom_factor_x - w->offset_x;
    y1 = t->y( x_left, y_low) * w->zoom_factor_y - w->offset_y;
    x2 = t->x( x_right, y_high) * w->zoom_factor_x - w->offset_x;
    y2 = t->y( x_right, y_high) * w->zoom_factor_y - w->offset_y;
  }
  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);

  ctx->gdraw->fill_rect( w, ll_x, ll_y, ur_x - ll_x + 1, ur_y - ll_y + 1, glow_eDrawType_LineErase);
}

void GrowConGlue::draw()
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

void GrowConGlue::align( double x, double y, glow_eAlignDirection direction)
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

void GrowConGlue::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2; 
  double ll_x, ll_y, ur_x, ur_y, m_x, m_y;
  glow_eDrawType drawtype, shift_drawtype;
  int idx_up, idx_down, idx_left, idx_right;

  int idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * line_width - 1);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t)
  {
    x1 = x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = t->y( x_left, y_low) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = t->x( x_right, y_high) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = t->y( x_right, y_high) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  }
  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);

  m_x = int((x1 + x2) / 2 + 0.5);
  m_y = int((y1 + y2) / 2 + 0.5);

  drawtype = ctx->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, 0, 0);

  int lw_up, lw_down, lw_left, lw_right;

  lw_down = line_width_down;
  lw_right = line_width_right;
  lw_up = line_width_up;
  lw_left = line_width_left;

  idx_up = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * lw_up - 1);
  idx_up += hot;
  idx_down = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * lw_down - 1);
  idx_down += hot;
  idx_left = int( ctx->mw.zoom_factor_x / ctx->mw.base_zoom_factor * lw_left - 1);
  idx_left += hot;
  idx_right = int( ctx->mw.zoom_factor_x / ctx->mw.base_zoom_factor * lw_right - 1);
  idx_right += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  idx_up = max( 0, idx_up);
  idx_up = min( idx_up, DRAW_TYPE_SIZE-1);
  idx_down = max( 0, idx_down);
  idx_down = min( idx_down, DRAW_TYPE_SIZE-1);
  idx_left = max( 0, idx_left);
  idx_left = min( idx_left, DRAW_TYPE_SIZE-1);
  idx_right = max( 0, idx_right);
  idx_right = min( idx_right, DRAW_TYPE_SIZE-1);

  if ( lw_up != -1 && lw_down == -1 &&
       lw_right == -1 && lw_left == -1) {
    // Up termination
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_up -= 2;
	m_y++;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_up += 2;
	m_y--;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up == -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Down termination
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_down -= 2;
	m_y--;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_down += 2;
	m_y++;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right != -1 && lw_left == -1) {
    // Right termination
    ctx->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	m_x++;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_right += 2;
	m_x--;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right == -1 && lw_left != -1) {
    // Left termination
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	m_x--;
      }

      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_left += 2;
	m_x++;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up != -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Vertical glue
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);

    if ( shadow) {
      if ( border) {
	idx_up -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_up += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_down != idx_up) {
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down == -1) {
    // Horizontal glue
    ctx->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_right += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_left != idx_right) {
	ctx->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x, m_y + idx_right/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left up corner
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y + idx_left/2, idx_up + 1, ur_y -  (m_y + idx_left/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x + 1, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Right up corner
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y -  (m_y - idx_right + idx_right/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x + idx_up/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Right down corner
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - idx_right + idx_right/2 - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left down corner
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2 - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left right down threeway
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x + idx_down/2 - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	ctx->export_jbean->line(  ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	ctx->export_jbean->line(  ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_down/2, m_y + idx_right/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left right up threeway
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_up + idx_up/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	ctx->export_jbean->line(  ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	ctx->export_jbean->line(  ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Left up down threeway
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, idx_up + 1, ur_y - (m_y - idx_left + idx_left/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	ctx->export_jbean->line(  m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      if ( border) {
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	ctx->export_jbean->line(  m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right up down threeway
    ctx->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y - (m_y - idx_right + idx_right/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_right/2 - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	// ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right left up down fourway
    ctx->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    ctx->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, 0, glow_eGradient_No, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ctx->shift_drawtype( draw_type, -2, 0); // Light
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ctx->shift_drawtype( draw_type, 2, 0); // Dark
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ctx->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else {
    ctx->export_jbean->line(  m_x, ll_y, m_x, m_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    ctx->export_jbean->line(  m_x, m_y, m_x, ur_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    ctx->export_jbean->line(  ll_x, m_y, m_x, m_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    ctx->export_jbean->line(  m_x, m_y, ur_x, m_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
  }
}

void GrowConGlue::set_transform( GlowTransform *t)
{
  GrowNode::set_transform( t);
  configure(0);
}

glow_eDirection GrowConGlue::conpoint_to_direction( int point)
{
  double rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;

  if ( rotation < 45 || rotation > 315) {
    switch( point) {
    case 0: return glow_eDirection_Down;
    case 1: return glow_eDirection_Right;
    case 2: return glow_eDirection_Up;
    case 3: return glow_eDirection_Left;
    default: ;
    }
  }
  else if ( rotation < 135) {
    switch( point) {
    default: ;
    case 0: return glow_eDirection_Right;
    case 1: return glow_eDirection_Up;
    case 2: return glow_eDirection_Left;
    case 3: return glow_eDirection_Down;
    }
  }
  else if ( rotation < 225) {
    switch( point) {
    case 0: return glow_eDirection_Up;
    case 1: return glow_eDirection_Left;
    case 2: return glow_eDirection_Down;
    case 3: return glow_eDirection_Right;
    default: ;
    }
  }
  else {
    switch( point) {
    case 0: return glow_eDirection_Left;
    case 1: return glow_eDirection_Down;
    case 2: return glow_eDirection_Right;
    case 3: return glow_eDirection_Up;
    default: ;
    }
  }
  return glow_eDirection_Center;
}



