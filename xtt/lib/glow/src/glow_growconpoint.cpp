#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growconpoint.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowConPoint::GrowConPoint( GlowCtx *glow_ctx, char *name, double x, double y, 
		int cp_num, glow_eDirection d, int nodraw) :
		GlowConPoint(glow_ctx,x,y,cp_num,d), 
    		hot(0), pzero(ctx), highlight(0), inverse(0), 
		arc( ctx, x - ctx->draw_delta, y - ctx->draw_delta, 
		x + ctx->draw_delta, y + ctx->draw_delta, 0, 360, 
		glow_eDrawType_Line, 2),
		user_data(NULL)
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();

  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( p.x, p.y, &x_grid, &y_grid);
    p.posit( x_grid, y_grid);
  }
  arc.ll.posit( - ctx->draw_delta, - ctx->draw_delta);
  arc.ur.posit( ctx->draw_delta, ctx->draw_delta);
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  get_node_borders();
}

GrowConPoint::~GrowConPoint()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase();
  nav_erase();

  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
}

void GrowConPoint::move( double delta_x, double delta_y, int grid)
{
  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  if ( grid)
  {
    double x, y, x_grid, y_grid;

    /* Move to closest grid point */
    erase();
    nav_erase();
    x = (x_right + x_left) / 2;
    y = (y_high + y_low) / 2;
    ctx->find_grid( x + delta_x / ctx->zoom_factor_x,
	y + delta_y / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x, y_grid - y);
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
//  x1 = trf.x( p.x, p.y);
//  y1 = trf.y( p.x, p.y);
//  arc.move( (void *)&pzero, x1 - ctx->draw_delta, 
//	y1 - ctx->draw_delta,
//	x1 + ctx->draw_delta, y1 + ctx->draw_delta, arc.angel1, arc.angel2,
//	highlight, hot);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowConPoint::move_noerase( int delta_x, int delta_y, int grid)
{
  if ( grid)
  {
    double x, y, x_grid, y_grid;

    /* Move to closest grid point */
    x = (x_right + x_left) / 2;
    y = (y_high + y_low) / 2;
    ctx->find_grid( x + double( delta_x) / ctx->zoom_factor_x,
	y + double( delta_y) / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x, y_grid - y);
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
//  x1 = trf.x( p.x, p.y);
//  y1 = trf.y( p.x, p.y);
//  arc.move( (void *)&pzero, x1 - ctx->draw_delta, 
//	y1 - ctx->draw_delta,
//	x1 + ctx->draw_delta, y1 + ctx->draw_delta, arc.angel1, arc.angel2,
//	highlight, hot);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

int GrowConPoint::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;
  GlowPoint p1(p);
  p1.x = trf.x( p.x, p.y);
  p1.y = trf.y( p.x, p.y);
  p1.zoom();

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = arc.event_handler( (void *)&p1, 
		event, x, y, NULL);
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

      if ( ctx->hot_found)
        sts = 0;
      else
      {
        sts = arc.event_handler( (void *)&p1, event, x, y, NULL);
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
        draw_set_cursor( ctx, glow_eDrawCursor_Normal);
        erase();
        hot = 0;
        redraw = 1;
      }
      if ( redraw)
      {
        if ( !inverse)
          ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
//          arc.draw( (void *)&pzero, highlight, hot, NULL);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = arc.event_handler( (void *)&p1, 
		event, x, y, NULL);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowConPoint::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  if ( mode == glow_eSaveMode_SubGraph)
    GlowConPoint::save( fp, mode);
  else
  {
    fp << int(glow_eSave_GrowConPoint) << endl;
    fp << int(glow_eSave_GrowConPoint_n_name) << FSPACE << n_name << endl;
    fp << int(glow_eSave_GrowConPoint_x_right) << FSPACE << x_right << endl;
    fp << int(glow_eSave_GrowConPoint_x_left) << FSPACE << x_left << endl;
    fp << int(glow_eSave_GrowConPoint_y_high) << FSPACE << y_high << endl;
    fp << int(glow_eSave_GrowConPoint_y_low) << FSPACE << y_low << endl;
    fp << int(glow_eSave_GrowConPoint_arc) << endl;
    arc.save( fp, mode);
    fp << int(glow_eSave_GrowConPoint_conpoint_part) << endl;
    GlowConPoint::save( fp, mode);
    fp << int(glow_eSave_End) << endl;
  }
}

void GrowConPoint::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowConPoint: break;
      case glow_eSave_GrowConPoint_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowConPoint_x_right: fp >> x_right; break;
      case glow_eSave_GrowConPoint_x_left: fp >> x_left; break;
      case glow_eSave_GrowConPoint_y_high: fp >> y_high; break;
      case glow_eSave_GrowConPoint_y_low: fp >> y_low; break;
      case glow_eSave_GrowConPoint_arc: arc.open( fp); break;
      case glow_eSave_GrowConPoint_conpoint_part: 
        GlowConPoint::open( fp);
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowConPoint:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowConPoint::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowConPoint::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowConPoint::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowConPoint::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowConPoint::set_highlight( int on)
{
  highlight = on;
  if ( !inverse)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  else
    draw_inverse();
  nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);
}

void GrowConPoint::select_region_insert( double ll_x, double ll_y, double ur_x, 
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

void GrowConPoint::zoom()
{
  GlowConPoint::zoom();
  arc.zoom();
}

void GrowConPoint::nav_zoom()
{
  GlowConPoint::nav_zoom();
  arc.nav_zoom();
}

void GrowConPoint::draw( GlowTransform *t, int highlight, int hot, void *node,
			 void *colornode)
{
  GlowPoint p1(p);
  if ( t)
  {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else
  {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  p1.zoom();
  arc.draw( (void *)&p1, highlight, hot, NULL);
}

void GrowConPoint::erase( GlowTransform *t, int hot, void *node)
{
  GlowPoint p1(p);
  if ( t)
  {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else
  {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  p1.zoom();
  ctx->set_draw_buffer_only();
  arc.erase( (void *)&p1, hot, NULL);
  ctx->reset_draw_buffer_only();
}

void GrowConPoint::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  GlowPoint p1(p);
  if ( t)
  {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else
  {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  p1.nav_zoom();
  arc.nav_draw( (void *)&p1, highlight, NULL);
}

void GrowConPoint::nav_erase( GlowTransform *t, void *node)
{
  GlowPoint p1(p);
  if ( t)
  {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else
  {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  p1.nav_zoom();
  arc.nav_erase( (void *)&p1, NULL);
}


void GrowConPoint::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();

}


void GrowConPoint::align( double x, double y, glow_eAlignDirection direction)
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

//    x1 = trf.x( p.x, p.y);
//    y1 = trf.y( p.x, p.y);
//    arc.move( (void *)&pzero, x1 - ctx->draw_delta, 
//	y1 - ctx->draw_delta,
//	x1 + ctx->draw_delta, y1 + ctx->draw_delta, arc.angel1, arc.angel2,
//	highlight, hot);
    ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
    ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowConPoint::get_node_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  GlowPoint p1(p);
  if ( t)
  {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else
  {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  arc.get_borders( p1.x, p1.y, x_right, x_left, y_high, y_low, NULL);
}

void GrowConPoint::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  double x1, y1;

  if ( t)
  {
    x1 = trf.x( t, p.x, p.y);
    y1 = trf.y( t, p.x, p.y);
  }
  else
  {
    x1 = trf.x( p.x, p.y);
    y1 = trf.y( p.x, p.y);
  }

  if ( x1 < *x_left)
    *x_left = x1;
  if ( x1 > *x_right)
    *x_right = x1;
  if ( y1 < *y_low)
    *y_low = y1;
  if ( y1 > *y_high)
    *y_high = y1;
}
