#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growconglue.h"
#include "glow_grownode.h"
#include "glow_growrect.h"
#include "glow_conpoint.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowConGlue::GrowConGlue( GlowCtx *glow_ctx, char *name, double x1, double y1, int nodraw) : 
		GrowNode(glow_ctx,name,0,x1,y1,1,0),
		line_width_up(-1), line_width_down(-1), line_width_left(-1), line_width_right(-1)
{
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

  get_node_borders();
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);

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

#if 0
  double rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
  if ( rotation < 45 || rotation > 315) {
  }
  else if ( rotation < 135) {
    line_width_down = lw_left;
    line_width_right = lw_down;
    line_width_up = lw_right;
    line_width_left = lw_up;
  }
  else if ( rotation < 225) {
    line_width_down = lw_up;
    line_width_right = lw_left;
    line_width_up = lw_down;
    line_width_left = lw_right;
  }
  else {
    line_width_down = lw_right;
    line_width_right = lw_up;
    line_width_up = lw_left;
    line_width_left = lw_down;
  }
#endif
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

  for (;;)
  {
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

void GrowConGlue::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

  if ( x_right * ctx->zoom_factor_x - ctx->offset_x >= ll_x &&
      	x_left * ctx->zoom_factor_x - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor_y - ctx->offset_y >= ll_y &&
       	y_low * ctx->zoom_factor_y - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();
  }
}

void GrowConGlue::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowConGlue::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowConGlue::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowConGlue::draw( GlowTransform *t, int highlight, int hot, void *node, 
		      void *colornode)
{
  if ( ctx->nodraw)
    return;
  int idx, idx_up, idx_down, idx_left, idx_right;
  glow_eDrawType drawtype;
  glow_eDrawType shift_drawtype;


  idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  double x1, y1, x2, y2; 
  int ll_x, ll_y, ur_x, ur_y, m_x, m_y;

  if (!t)
  {
    x1 = x_left * ctx->zoom_factor_x - ctx->offset_x;
    y1 = y_low * ctx->zoom_factor_y - ctx->offset_y;
    x2 = x_right * ctx->zoom_factor_x - ctx->offset_x;
    y2 = y_high * ctx->zoom_factor_y - ctx->offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = t->y( x_left, y_low) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = t->x( x_right, y_high) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = t->y( x_right, y_high) * ctx->zoom_factor_y - ctx->offset_y;
  }

  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);
  m_x = int((x1 + x2) / 2 + 0.5);
  m_y = int((y1 + y2) / 2 + 0.5);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);

  int lw_up, lw_down, lw_left, lw_right;

  lw_down = line_width_down;
  lw_right = line_width_right;
  lw_up = line_width_up;
  lw_left = line_width_left;

  idx_up = int( ctx->zoom_factor_y / ctx->base_zoom_factor * lw_up - 1);
  idx_up += hot;
  idx_down = int( ctx->zoom_factor_y / ctx->base_zoom_factor * lw_down - 1);
  idx_down += hot;
  idx_left = int( ctx->zoom_factor_x / ctx->base_zoom_factor * lw_left - 1);
  idx_left += hot;
  idx_right = int( ctx->zoom_factor_x / ctx->base_zoom_factor * lw_right - 1);
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

  ctx->set_draw_buffer_only();
  if ( lw_up != -1 && lw_down == -1 &&
       lw_right == -1 && lw_left == -1) {
    // Up termination
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    if ( shadow && idx_up > 2) {
      if ( border) {
	idx_up -= 2;
	m_y++;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, shift_drawtype, 0, 0);

      if ( border) {
	idx_up += 2;
	m_y--;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up == -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Down termination
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
    if ( shadow && idx_down > 2) {
      if ( border) {
	idx_down -= 2;
	m_y--;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, 0);

      if ( border) {
	idx_down += 2;
	m_y++;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right != -1 && lw_left == -1) {
    // Right termination
    glow_draw_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    if ( shadow && idx_right > 2) {
      if ( border) {
	idx_right -= 2;
	m_x++;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, shift_drawtype, 0, 0);

      if ( border) {
	idx_right += 2;
	m_x--;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right == -1 && lw_left != -1) {
    // Left termination
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    if ( shadow && idx_left > 2) {
      if ( border) {
	idx_left -= 2;
	m_x--;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);

      if ( border) {
	idx_left += 2;
	m_x++;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_up != -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Vertical glue
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);

    if ( shadow && idx_down > 2) {
      if ( border) {
	idx_up -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_up += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      if ( idx_down != idx_up) {
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down == -1) {
    // Horizontal glue
    glow_draw_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_right += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      if ( idx_left != idx_right) {
	glow_draw_line( ctx, m_x, m_y - idx_right + idx_right/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x, m_y + idx_right/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left up corner
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, idx_up + 1, ur_y -  (m_y + idx_left/2), drawtype);
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x + 1, idx_left + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_up > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Right up corner
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y -  (m_y - idx_right + idx_right/2), drawtype);
    glow_draw_fill_rect( ctx, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x + idx_up/2), idx_right + 1, drawtype);
    if ( shadow && (idx_right > 2 || idx_up > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Right down corner
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - idx_right + idx_right/2 - ll_y, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, drawtype);
    if ( shadow && (idx_right > 2 || idx_down > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left down corner
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y + 1, drawtype);
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2 - ll_x, idx_left + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_down > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left right down threeway
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y + 1, drawtype);
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_down/2 - ll_x, idx_left + 1, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2 || idx_down > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      if ( idx_right == idx_left)
	glow_draw_line( ctx, ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, shift_drawtype, 0, 0);
      else if ( idx_right > idx_left) {
	glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      }
      else {
	glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      }
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      if ( idx_right == idx_left)
	glow_draw_line( ctx, ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      else if ( idx_right > idx_left) {
	glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_down/2, m_y + idx_right/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left right up threeway
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x, idx_left + 1, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_up + idx_up/2), idx_right + 1, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2 || idx_up > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( idx_right == idx_left)
	glow_draw_line( ctx, ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      else if ( idx_right > idx_left) {
	glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      }
      else {
	glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      if ( idx_right == idx_left)
	glow_draw_line( ctx, ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      else if ( idx_right > idx_left) {
	glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Left up down threeway
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, idx_up + 1, ur_y - (m_y - idx_left + idx_left/2), drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y, drawtype);
    if ( shadow && (idx_left > 2 || idx_down > 2 || idx_up > 2)) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);

      if ( idx_up == idx_down)
	glow_draw_line( ctx, m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, shift_drawtype, 0, 0);
      else if ( idx_down > idx_up) {
	glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      }
      else {
	glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      }
      if ( border) {
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);

      if ( idx_up == idx_down)
	glow_draw_line( ctx, m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, glow_eDrawType_Line, 0, 0);
      else if ( idx_down > idx_up) {
	glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right up down threeway
    glow_draw_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y - (m_y - idx_right + idx_right/2), drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_right/2 - ll_y, drawtype);
    if ( shadow && (idx_right > 2 || idx_down > 2 || idx_up > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);

      if ( idx_up == idx_down)
	glow_draw_line( ctx, m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, shift_drawtype, 0, 0);
      else if ( idx_down > idx_up) {
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
	// glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
      }
      else {
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);

      if ( idx_up == idx_down)
	glow_draw_line( ctx, m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, glow_eDrawType_Line, 0, 0);
      else if ( idx_down > idx_up) {
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      }
      else {
	glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
	glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right left up down fourway
    glow_draw_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    glow_draw_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    glow_draw_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
    if ( shadow && (idx_left > 2 || idx_right > 2 || idx_down > 2 || idx_up > 2)) {
      if ( border) {
	idx_right -= 2;
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, 0);
      if ( border) {
	idx_right += 2;
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      glow_draw_line( ctx, m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, 0);
      glow_draw_line( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, 0);
    }
  }
  else {
    glow_draw_line( ctx, m_x, ll_y, m_x, m_y, drawtype, idx, 0);
    glow_draw_line( ctx, m_x, m_y, m_x, ur_y, drawtype, idx, 0);
    glow_draw_line( ctx, ll_x, m_y, m_x, m_y, drawtype, idx, 0);
    glow_draw_line( ctx, m_x, m_y, ur_x, m_y, drawtype, idx, 0);
  }
  ctx->reset_draw_buffer_only();
}


void GrowConGlue::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  glow_eDrawType drawtype;
  int idx;
  int m_x, m_y, idx_left, idx_right, idx_up, idx_down;

  idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);

  double x1, y1, x2, y2;
  int ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y1 = y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
    x2 = x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y2 = y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y1 = t->y( x_left, y_low) * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
    x2 = t->x( x_right, y_high) * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y2 = t->y( x_right, y_high) * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
  }

  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);
  m_x = int((x1 + x2) / 2 + 0.5);
  m_y = int((y1 + y2) / 2 + 0.5);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);

  int lw_up, lw_down, lw_left, lw_right;

  lw_down = line_width_down;
  lw_right = line_width_right;
  lw_up = line_width_up;
  lw_left = line_width_left;

  idx_up = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * lw_up - 1);
  idx_down = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * lw_down - 1);
  idx_left = int( ctx->nav_zoom_factor_x / ctx->base_zoom_factor * lw_left - 1);
  idx_right = int( ctx->nav_zoom_factor_x / ctx->base_zoom_factor * lw_right - 1);

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
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
  }
  else if ( lw_up == -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Down termination
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right != -1 && lw_left == -1) {
    // Right termination
    glow_draw_nav_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right == -1 && lw_left != -1) {
    // Left termination
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
  }
  else if ( lw_up != -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Vertical glue
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down == -1) {
    // Horizontal glue
    glow_draw_nav_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left up corner
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y + idx_left/2, idx_up + 1, ur_y -  (m_y + idx_left/2), drawtype);
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x + 1, idx_left + 1, drawtype);
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Right up corner
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y -  (m_y - idx_right + idx_right/2), drawtype);
    glow_draw_nav_fill_rect( ctx, m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x + idx_up/2), idx_right + 1, drawtype);
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Right down corner
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - idx_right + idx_right/2 - ll_y, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, drawtype);
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left down corner
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2 - ll_x, idx_left + 1, drawtype);
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left right down threeway
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_down/2 - ll_x, idx_left + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, drawtype);
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left right up threeway
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x, idx_left + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_up + idx_up/2), idx_right + 1, drawtype);
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Left up down threeway
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, idx_up + 1, ur_y - (m_y - idx_left + idx_left/2), drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y, drawtype);
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right up down threeway
    glow_draw_nav_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y - (m_y - idx_right + idx_right/2), drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_right/2 - ll_y, drawtype);
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right left up down fourway
    glow_draw_nav_fill_rect( ctx, m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, drawtype);
    glow_draw_nav_fill_rect( ctx, m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, drawtype);
  }
  else {
    glow_draw_nav_line( ctx, m_x, ll_y, m_x, m_y, drawtype, idx, 0);
    glow_draw_nav_line( ctx, m_x, m_y, m_x, ur_y, drawtype, idx, 0);
    glow_draw_nav_line( ctx, ll_x, m_y, m_x, m_y, drawtype, idx, 0);
    glow_draw_nav_line( ctx, m_x, m_y, ur_x, m_y, drawtype, idx, 0);
  }
}

void GrowConGlue::erase( GlowTransform *t, int hot, void *node)
{
  double x1, y1, x2, y2;
  int ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = x_left * ctx->zoom_factor_x - ctx->offset_x;
    y1 = y_low * ctx->zoom_factor_y - ctx->offset_y;
    x2 = x_right * ctx->zoom_factor_x - ctx->offset_x;
    y2 = y_high * ctx->zoom_factor_y - ctx->offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = t->y( x_left, y_low) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = t->x( x_right, y_high) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = t->y( x_right, y_high) * ctx->zoom_factor_y - ctx->offset_y;
  }
  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);

  glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
}

void GrowConGlue::nav_erase( GlowTransform *t, void *node)
{
  double x1, y1, x2, y2;
  int ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y1 = y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
    x2 = x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y2 = y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y1 = t->y( x_left, y_low) * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
    x2 = t->x( x_right, y_high) * ctx->nav_zoom_factor_x - ctx->nav_offset_x;
    y2 = t->y( x_right, y_high) * ctx->nav_zoom_factor_y - ctx->nav_offset_y;
  }
  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);

  glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
}

void GrowConGlue::draw()
{
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowConGlue::align( double x, double y, glow_eAlignDirection direction)
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

void GrowConGlue::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2; 
  double ll_x, ll_y, ur_x, ur_y, m_x, m_y;
  glow_eDrawType drawtype, shift_drawtype;
  int idx_up, idx_down, idx_left, idx_right;

  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t)
  {
    x1 = x_left * ctx->zoom_factor_x - ctx->offset_x;
    y1 = y_low * ctx->zoom_factor_y - ctx->offset_y;
    x2 = x_right * ctx->zoom_factor_x - ctx->offset_x;
    y2 = y_high * ctx->zoom_factor_y - ctx->offset_y;
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = t->y( x_left, y_low) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = t->x( x_right, y_high) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = t->y( x_right, y_high) * ctx->zoom_factor_y - ctx->offset_y;
  }
  ll_x = int( min( x1, x2) + 0.5);
  ur_x = int( max( x1, x2) + 0.5);
  ll_y = int( min( y1, y2) + 0.5);
  ur_y = int( max( y1, y2) + 0.5);

  m_x = int((x1 + x2) / 2 + 0.5);
  m_y = int((y1 + y2) / 2 + 0.5);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, 0, 0);

  int lw_up, lw_down, lw_left, lw_right;

  lw_down = line_width_down;
  lw_right = line_width_right;
  lw_up = line_width_up;
  lw_left = line_width_left;

  idx_up = int( ctx->zoom_factor_y / ctx->base_zoom_factor * lw_up - 1);
  idx_up += hot;
  idx_down = int( ctx->zoom_factor_y / ctx->base_zoom_factor * lw_down - 1);
  idx_down += hot;
  idx_left = int( ctx->zoom_factor_x / ctx->base_zoom_factor * lw_left - 1);
  idx_left += hot;
  idx_right = int( ctx->zoom_factor_x / ctx->base_zoom_factor * lw_right - 1);
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
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_up -= 2;
	m_y++;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_up += 2;
	m_y--;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x + idx_up/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up == -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Down termination
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_down -= 2;
	m_y--;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_down += 2;
	m_y++;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right != -1 && lw_left == -1) {
    // Right termination
    ((GrowCtx *)ctx)->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	m_x++;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_right += 2;
	m_x--;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, m_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up == -1 && lw_down == -1 &&
	    lw_right == -1 && lw_left != -1) {
    // Left termination
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	m_x--;
      }

      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( border) {
	idx_left += 2;
	m_x++;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_left + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_up != -1 && lw_down != -1 &&
	    lw_right == -1 && lw_left == -1) {
    // Vertical glue
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);

    if ( shadow) {
      if ( border) {
	idx_up -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_up += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_down != idx_up) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y, m_x - idx_down + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y, m_x + idx_down/2, m_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down == -1) {
    // Horizontal glue
    ((GrowCtx *)ctx)->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_right += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_left != idx_right) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y - idx_right + idx_right/2, m_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y + idx_right/2, m_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left up corner
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y + idx_left/2, idx_up + 1, ur_y -  (m_y + idx_left/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x + 1, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Right up corner
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y -  (m_y - idx_right + idx_right/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x + idx_up/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Right down corner
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - idx_right + idx_right/2 - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left down corner
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2 - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x,  m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up == -1 && lw_down != -1) {
    // Left right down threeway
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x + idx_down/2 - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_down + idx_down/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_down -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_down += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x , m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x,  m_y + idx_left/2, ur_x, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y + idx_right/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down == -1) {
    // Left right up threeway
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2 - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x - (m_x - idx_up + idx_up/2), idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_right -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_left += 2;
	idx_right += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x , m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( idx_right == idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x,  m_y - idx_left + idx_left/2, ur_x, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_right > idx_left) {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_right + idx_right/2, m_x + idx_up/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right == -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Left up down threeway
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_left + idx_left/2, idx_up + 1, ur_y - (m_y - idx_left + idx_left/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_left/2 - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      if ( border) {
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2,  ll_y, m_x + idx_down/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_left/2, m_x + idx_down/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y - idx_left + idx_left/2, m_x + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left == -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right up down threeway
    ((GrowCtx *)ctx)->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, idx_up + 1, ur_y - (m_y - idx_right + idx_right/2), 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y + idx_right/2 - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	// ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;

      if ( idx_up == idx_down) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2,  ll_y, m_x - idx_down + idx_down/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else if ( idx_down > idx_up) {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
      else {
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
	((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y - idx_right + idx_right/2, m_x - idx_down + idx_down/2, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      }
    }
  }
  else if ( lw_left != -1 && lw_right != -1 &&
	    lw_up != -1 && lw_down != -1) {
    // Right left up down fourway
    ((GrowCtx *)ctx)->export_jbean->rect( m_x, m_y - idx_right + idx_right/2, ur_x - m_x, idx_right + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( ll_x, m_y - idx_left + idx_left/2, m_x - ll_x, idx_left + 1, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_up + idx_up/2, m_y, idx_up + 1, ur_y - m_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    ((GrowCtx *)ctx)->export_jbean->rect( m_x - idx_down + idx_down/2, ll_y, idx_down + 1, m_y - ll_y, 1, 0, drawtype, glow_eDrawType_No, 0, 0, 0, 0, pass, shape_cnt, node_cnt, fp);
    if ( shadow) {
      if ( border) {
	idx_right -= 2;
	idx_left -= 2;
	idx_down -= 2;
	idx_up -= 2;
      }
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, -2, 0); // Light
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( draw_type, 2, 0); // Dark
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, shift_drawtype, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      if ( border) {
	idx_right += 2;
	idx_left += 2;
	idx_down += 2;
	idx_up += 2;
      }
    }
    if ( border) {
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, m_y - idx_right + idx_right/2, ur_x, m_y - idx_right + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_down/2, ll_y, m_x + idx_down/2, m_y - idx_right + idx_right /2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right/2, ur_x, m_y + idx_right/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x + idx_up/2, m_y + idx_right /2, m_x + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y - idx_left + idx_left/2, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_down + idx_down/2, ll_y, m_x - idx_down + idx_down/2, m_y - idx_left + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y + idx_left/2, m_x - idx_up + idx_up/2, m_y + idx_left/2, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ((GrowCtx *)ctx)->export_jbean->line(  m_x - idx_up + idx_up/2, m_y + idx_left/2, m_x - idx_up + idx_up/2, ur_y, glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  else {
    ((GrowCtx *)ctx)->export_jbean->line(  m_x, ll_y, m_x, m_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y, m_x, ur_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    ((GrowCtx *)ctx)->export_jbean->line(  ll_x, m_y, m_x, m_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    ((GrowCtx *)ctx)->export_jbean->line(  m_x, m_y, ur_x, m_y, drawtype, idx, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
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



